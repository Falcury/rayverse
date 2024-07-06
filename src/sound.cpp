

// sub_71E44
i16 get_sound_to_interrupt(i32 event_index) {
	i32 i = 0;
	for (; i < 20; ++i) {
		if (voice_obj_ids[i] == event_index) break;
	}
	if (i == 20) {
		while (i > 0) {
			--i;
			if (sounds_playing[i] == 0) break;
		}
	}
	return sounds_playing[i];
}


// sub_71FC8
void sub_71FC8(i32 event_index) {
	// stub
}

// sub_71F6C
bool sub_71F6C(i32 event_index, i32 sound_id) {
	// stub
	return false;
}

// sub_7228C
void play_sound(u16 sound_id, i32 event_index) {
	if (is_sound_available) {
		if ((ray.scale != 0 && event_index == rayman_obj_id_) || event_index == some_obj_id_related_to_sound_) {
			event_index = -1;
		}
		if (event_index == -1 && sound_id != 15) {
			word_E5750 = 0;
		}
		i32 sound_to_interrupt = get_sound_to_interrupt(event_index);
		if (sound_to_interrupt != sound_id && !(sound_flags[sound_to_interrupt] & 8)) {
			sub_71FC8(event_index);
			i16 unk = sub_71F6C(event_index, sound_to_interrupt);
			if (unk >= 0) {

			}
			// stub
		}
	}
}



ogg_t open_cd_vorbis(i32 track_number) {
	ogg_t result = {0};
	if (track_number >= 2 && track_number <= 20) {
		char filename[512];
		snprintf(filename, sizeof(filename), "data/Music/rayman%02d.ogg", track_number);
		mem_t* mem = read_entire_file(filename);
		if (mem) {
			int error = 0;
			stb_vorbis* decoder = stb_vorbis_open_memory(mem->data, mem->len, &error, NULL);
			if (!decoder) {
				free(mem);
				return result; // TODO: failure condition
			} else {
				result.decoder = decoder;
				result.file = mem;
				result.sample_count = stb_vorbis_stream_length_in_samples(decoder);
				return result;
			}
		}
	}
	return result;
}

//float volume = 0.5f;

void play_ogg(game_sound_buffer_t* sound_buffer, ogg_t* ogg) {
	i32 samples_requested = sound_buffer->sample_count;
	i32 samples_filled = stb_vorbis_get_samples_short_interleaved(ogg->decoder, 2, sound_buffer->samples, samples_requested * 2);
	if (samples_filled < samples_requested) {
		i32 bytes_per_sample = sizeof(short) * 2;
		i32 bytes_filled = samples_filled * bytes_per_sample;
		i32 remaining_bytes = (samples_requested - samples_filled) * bytes_per_sample;
		memset(sound_buffer->samples + bytes_filled, 0, remaining_bytes);
	}
}

float t_pitch_sine;
float t_sine;

void game_get_sound_samples(game_sound_buffer_t* sound_buffer) {
	if (is_ogg_playing) {
		if (ogg_cd_track.decoder) {
			play_ogg(sound_buffer, &ogg_cd_track);
		}
	} else {
#if 1
		i16 tone_volume = 3000;
		float tone_hz = 400.0f;
		//	real32 wave_period = sound_buffer.samples_per_second/tone_hz;
		float pitch_hz = 2.0f;
		float pitch_period = sound_buffer->samples_per_second / pitch_hz;


		i16* sample_out = sound_buffer->samples;

		for (u32 sample_index = 0; sample_index < sound_buffer->sample_count; ++sample_index) {
#if 1
			t_pitch_sine += two_pi32 / pitch_period;
			if (t_pitch_sine > two_pi32) t_pitch_sine -= two_pi32;
			float pitch_sine_value = sinf(t_pitch_sine);
			float curr_hz = tone_hz + 100.0f * pitch_sine_value;
			float curr_wave_period = sound_buffer->samples_per_second / curr_hz;
			float t = t_sine += two_pi32 / curr_wave_period;
			if (t_sine > two_pi32) t_sine -= two_pi32;
			float sine_value = sinf(t);
			i16 sample_value = (i16) (sine_value * tone_volume);
#else
			i16 sample_value = 0;
#endif

			*sample_out++ = sample_value;//left
			*sample_out++ = sample_value;//right
		}
#endif
	}

}
