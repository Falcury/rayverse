

// NOTE: We use SDL_QueueAudio() to push audio for now. The alternative method of using a callback might be better
// but this requires a secondary sound buffer. The code below works, but currently the reading is not well matched
// with the writing, so there is a long ~1 second audio latency (-> fix bug? or delete this code?).
#if 0

void audio_callback(void *userdata, u8 *stream, int len) {
    linux_sound_output_t* sound = &global_app_state.linux.sound_output;
    u32 samples_requested = len / sound->bytes_per_sample;
    ASSERT(sound->bytes_per_sample == 2 * sizeof(i16));

    u32 samples_to_read = MIN(samples_requested, sound->samples_per_second);
    i64 region1_samples_available = sound->samples_per_second - sound->running_sample_index_for_reading;
    bool wraps_around = samples_to_read > region1_samples_available;
    u32 region1_sample_count = MIN(region1_samples_available, samples_to_read);

    i16* dest_sample = (i16*)stream;
    i16* source_sample = (i16*)(sound->secondary_buffer + sound->running_sample_index_for_reading * sound->bytes_per_sample);
    for (u32 i = 0; i < region1_sample_count; ++i) {
        *dest_sample++ = *source_sample++;//left
        *dest_sample++ = *source_sample++;//right
        ++sound->running_sample_index_for_reading;
    }
    if (wraps_around) {
        u32 region2_sample_count = samples_to_read - region1_sample_count;
        source_sample = (i16*)sound->secondary_buffer;
        sound->running_sample_index_for_reading = 0;
        for (u32 sample_index = 0; sample_index < region2_sample_count; ++sample_index) {
            *dest_sample++ = *source_sample++;//left
            *dest_sample++ = *source_sample++;//right
            ++sound->running_sample_index_for_reading;
        }
    }
}

static void linux_fill_sound_buffer(linux_sound_output_t* sound, game_sound_buffer_t* source_buffer) {
    SDL_LockAudio();
    u32 samples_to_write = MIN(source_buffer->sample_count, sound->samples_per_second);
    i64 region1_samples_available = sound->samples_per_second - sound->running_sample_index_for_writing;
    bool wraps_around = samples_to_write > region1_samples_available;
    u32 region1_sample_count = MIN(region1_samples_available, samples_to_write);

    i16* source_sample = source_buffer->samples;
    i16* dest_sample = (i16*) (sound->secondary_buffer + sound->running_sample_index_for_writing * sound->bytes_per_sample);
    for (u32 sample_index = 0; sample_index < region1_sample_count; ++sample_index) {
        *dest_sample++ = *source_sample++;//left
        *dest_sample++ = *source_sample++;//right
        ++sound->running_sample_index_for_writing;
    }

    if (wraps_around) {
        u32 region2_sample_count = samples_to_write - region1_sample_count;
        dest_sample = (i16*)sound->secondary_buffer;
        sound->running_sample_index_for_writing = 0;
        for (u32 sample_index = 0; sample_index < region2_sample_count; ++sample_index) {
            *dest_sample++ = *source_sample++;//left
            *dest_sample++ = *source_sample++;//right
            ++sound->running_sample_index_for_writing;
        }
    }
    SDL_UnlockAudio();
}

#endif


static void linux_produce_sound_for_frame(app_state_t* app_state, sdl_sound_output_t* sound, game_sound_buffer_t* game_sound_buffer, i64 flip_clock) {
	u32 expected_sound_bytes_per_frame = (u32)((float)(sound->samples_per_second * sound->bytes_per_sample) / app_state->target_game_hz);
    // Determine how many audio samples we will ask the game to give us back.
    game_sound_buffer->sample_count = expected_sound_bytes_per_frame / sound->bytes_per_sample;

    game_get_sound_samples(game_sound_buffer);
//    linux_fill_sound_buffer(sound, game_sound_buffer);
    if (SDL_QueueAudio(app_state->sdl.sound_output.audio_device, game_sound_buffer->samples, expected_sound_bytes_per_frame) < 0) {
        printf("Failed to SDL_QueueAudio: %s\n", SDL_GetError());
    }
}


