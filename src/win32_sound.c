

#define FUNC_DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID lpGUID, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef FUNC_DIRECT_SOUND_CREATE(Func_DirectSoundCreate);



static void win32_init_dsound(HWND window, win32_sound_output_t* sound) {
	HMODULE dsound_library = LoadLibraryA("dsound.dll");
	if (dsound_library) {

		Func_DirectSoundCreate* DirectSoundCreate = (Func_DirectSoundCreate *)
				GetProcAddress(dsound_library, "DirectSoundCreate");
		if (DirectSoundCreate != NULL && SUCCEEDED(DirectSoundCreate(NULL, &sound->dsound, NULL))) {

			WAVEFORMATEX waveformat = {0};
			waveformat.wFormatTag = WAVE_FORMAT_PCM;
			waveformat.nChannels = 2;
			waveformat.wBitsPerSample = 16;
			waveformat.nBlockAlign = (waveformat.nChannels * waveformat.wBitsPerSample) / 8;
			waveformat.nSamplesPerSec = sound->samples_per_second;
			waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;
			waveformat.cbSize = 0;

			if (SUCCEEDED(sound->dsound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
				// "create" a primary buffer
				DSBUFFERDESC buffer_description = {0};
				buffer_description.dwSize = sizeof(buffer_description);
				buffer_description.dwFlags = DSBCAPS_PRIMARYBUFFER; // TODO: DSBCAPS_GLOBALFOCUS?
				IDirectSoundBuffer* primary_buffer;

				// NOTE: this is NOT a true buffer, just a handle to the actual sound device
				// so that we can set the correct waveformat!
				if (SUCCEEDED(sound->dsound->CreateSoundBuffer(&buffer_description, &primary_buffer, 0))) {
					if (SUCCEEDED(primary_buffer->SetFormat(&waveformat))) {
						OutputDebugStringA("Primary buffer format was set\n");
					} else {
						// TODO: diagnostic
					}
				}

			} else {
				// TODO: diagnostic (SetCooperativeLevel failed)
			}

			// "create" a secondary buffer
			DSBUFFERDESC secondary_buffer_description = {0};
			secondary_buffer_description.dwSize = sizeof(secondary_buffer_description);
			secondary_buffer_description.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			secondary_buffer_description.dwBufferBytes = sound->secondary_buffer_size;
			secondary_buffer_description.lpwfxFormat = &waveformat;

			if (SUCCEEDED(sound->dsound->CreateSoundBuffer(&secondary_buffer_description, &sound->secondary_buffer, 0))) {
				OutputDebugStringA("Secondary buffer format was created successfully\n");
			}



		} else {
			// TODO: diagnostic (DirectSoundCreate failed)
		}
	} else {
		// TODO: diagnostic (failed to load dsound.dll)
	}

}

static void win32_clear_sound_buffer(win32_sound_output_t* sound) {
	VOID* region1;
	DWORD region1_size;
	VOID* region2;
	DWORD region2_size;
	if (FAILED(sound->secondary_buffer->Lock(0, sound->secondary_buffer_size,
	                                         &region1, &region1_size,
	                                         &region2, &region2_size, 0))) {
		// TODO: diagnostics - could not lock the buffer
	} else {
		// NOTE: could just use memset.....
		memset(region1, 0, region1_size);
		memset(region2, 0, region2_size);
#if 0
		u8* dest_byte = (u8*) region1;
		for (DWORD byte_index = 0; byte_index < region1_size; ++byte_index) {
			*dest_byte++ = 0;
		}
		dest_byte = (u8*) region2;
		for (DWORD byte_index = 0; byte_index < region2_size; ++byte_index) {
			*dest_byte++ = 0;
		}
#endif
		sound->secondary_buffer->Unlock(region1, region1_size, region2, region2_size);
	}
}

static void win32_fill_sound_buffer(win32_sound_output_t* sound, DWORD byte_to_lock, DWORD bytes_to_write, game_sound_buffer_t* source_buffer) {
	VOID* region1;
	DWORD region1_size;
	VOID* region2;
	DWORD region2_size;

	if (FAILED(sound->secondary_buffer->Lock(byte_to_lock, bytes_to_write,
	                                         &region1, &region1_size,
	                                         &region2, &region2_size, 0))) {
		// TODO: diagnostics - could not lock the buffer
	} else {

		// TODO: assert that region1_size/region2_size is valid
		DWORD region1_sample_count = region1_size / sound->bytes_per_sample;

		i16* source_sample = source_buffer->samples;
		// region 1
		i16* dest_sample = (i16*) region1;
		for (DWORD sample_index = 0; sample_index < region1_sample_count; ++sample_index) {
			*dest_sample++ = *source_sample++;//left
			*dest_sample++ = *source_sample++;//right
			++sound->running_sample_index;
		}

		// region 2
		DWORD region2_sample_count = region2_size / sound->bytes_per_sample;
		dest_sample = (i16*) region2;
		for (DWORD sample_index = 0; sample_index < region2_sample_count; ++sample_index) {
			*dest_sample++ = *source_sample++;//left
			*dest_sample++ = *source_sample++;//right
			++sound->running_sample_index;
		}

		sound->secondary_buffer->Unlock(region1, region1_size, region2, region2_size);
	}
}

static void win32_produce_sound_for_frame(app_state_t* app_state, win32_sound_output_t* sound, game_sound_buffer_t* game_sound_buffer, i64 flip_clock) {
	float from_begin_to_audio_seconds = get_seconds_elapsed(flip_clock, get_clock());
//	printf("from_begin_to_audio_seconds = %g\n", from_begin_to_audio_seconds);

	// NOTE: Computing where to write into the sound buffer
	DWORD play_cursor;
	DWORD write_cursor;

	if (sound->secondary_buffer->GetCurrentPosition(&play_cursor, &write_cursor) == DS_OK) {

		/*
		We define a safety value that is the number of samples we think
		our game update loop may vary by (let's say up to 2ms).

		When we wake up to write audio, we will look and see what the play
		cursor position is and wel will forecast ahead where we think the
		play cursor will be on the next frame boundary.

		We will then look to see if the write cursor is before that
		by at least our safety value.
		If it is, the target fill position is that frame boundary plus one frame.
		This gives us perfect audio sync in the case of a card that
		has low latency.

		If the write cursor is _after_ the next frame boundary,
		then we assume we can never sync the audio perfectly, so we will
		write one frame's worth of audio plus the safety margin's worth
		of guard samples.
		*/
		if (!sound->is_valid) {
			sound->running_sample_index = write_cursor / sound->bytes_per_sample;
			sound->is_valid = true;
		}
		DWORD byte_to_lock = sound->running_sample_index * sound->bytes_per_sample %
		                     sound->secondary_buffer_size;

		DWORD expected_sound_bytes_per_frame = (DWORD)((float)(sound->samples_per_second*sound->bytes_per_sample) / app_state->target_game_hz);
		float seconds_left_until_flip = app_state->target_seconds_per_frame - from_begin_to_audio_seconds;
		DWORD expected_bytes_until_flip = (DWORD) (seconds_left_until_flip/app_state->target_seconds_per_frame * ((float)expected_sound_bytes_per_frame));
		DWORD expected_frame_boundary_byte = play_cursor + expected_bytes_until_flip;
		DWORD unwrapped_write_cursor = (write_cursor > play_cursor) ? write_cursor : write_cursor + sound->secondary_buffer_size;
		ASSERT(unwrapped_write_cursor >= play_cursor);

		DWORD audio_latency_bytes = unwrapped_write_cursor - play_cursor;
		float audio_latency_seconds = ((float)audio_latency_bytes / (float)sound->bytes_per_sample) /
		                        (float)sound->samples_per_second;

		// TODO: this doesn't seem to be right. How to detect that the latency is 'low enough'?
		bool32 audio_card_is_low_latency = (audio_latency_bytes + sound->safety_bytes < expected_sound_bytes_per_frame);

		DWORD target_cursor;
		if (audio_card_is_low_latency) {
			target_cursor = expected_frame_boundary_byte + expected_sound_bytes_per_frame;
		} else {
			target_cursor = write_cursor + expected_sound_bytes_per_frame + sound->safety_bytes;
		}
		target_cursor %= sound->secondary_buffer_size;

		DWORD bytes_to_write = 0;
		if (byte_to_lock > target_cursor) {
			bytes_to_write = (sound->secondary_buffer_size - byte_to_lock) + target_cursor;
		} else {
			bytes_to_write = target_cursor - byte_to_lock;
		}

		// Determine how many audio samples we will ask the game to give us back.
		game_sound_buffer->sample_count = bytes_to_write / sound->bytes_per_sample;

		game_get_sound_samples(game_sound_buffer);

		win32_fill_sound_buffer(sound, byte_to_lock, bytes_to_write, game_sound_buffer);
#if HANDMADE_INTERNAL
		//			printf("audio_latency_bytes = %d, audio_latency_seconds = %.3f\n",
//			 audio_latency_bytes, audio_latency_seconds);

			Win32_Debug_Time_Marker& marker = debug_time_markers[debug_time_marker_index];
			marker.output_play_cursor = play_cursor;
			marker.output_write_cursor = write_cursor;
			marker.output_location = byte_to_lock;
			marker.output_byte_count = bytes_to_write;
			marker.expected_flip_play_cursor = expected_frame_boundary_byte;
#endif
	} else {
		sound->is_valid = false;
	}
}