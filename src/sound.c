
//71B80
u8 snd_sqrt_table[128] = {
        40,  46,  51,  55,  60,  64,  67,  70,  72,  74,  77,  80,  83,  86,  89,  91,
        93,  95,  96,  98,  101, 104, 107, 109, 111, 113, 115, 117, 120, 122, 124, 125,
        127, 129, 131, 133, 135, 137, 138, 140, 142, 144, 145, 147, 149, 150, 152, 153,
        155, 156, 158, 160, 161, 163, 164, 166, 167, 168, 170, 171, 173, 174, 176, 177,
        178, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198,
        199, 200, 201, 202, 203, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 217,
        218, 219, 220, 222, 223, 224, 225, 226, 227, 229, 230, 231, 232, 233, 234, 235,
        236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
};


// Portable music-file lookup, shared by numbered CD tracks and movie overrides.
static ogg_t open_music_vorbis(const char* basename, bool looping) {
    ogg_t result = {0};
    char filename[512];
    snprintf(filename, sizeof(filename), "Music/%s", basename);
    mem_t* mem = read_entire_file(filename, false);
    if (!mem) {
        // GOG Rayman Forever may keep Music one directory above the game data.
        snprintf(filename, sizeof(filename), "../Music/%s", basename);
        mem = read_entire_file(filename, false);
    }
    if (!mem) {
        // Also accept *.ogg files in the base data folder
        mem = read_entire_file(basename, false);
    }
    if (mem) {
        int error = 0;
        stb_vorbis* decoder = stb_vorbis_open_memory(mem->data, mem->len, &error, NULL);
        if (!decoder) {
            free(mem);
        } else {
            result.decoder = decoder;
            result.file = mem;
            result.sample_count = stb_vorbis_stream_length_in_samples(decoder);
            result.sample_rate = stb_vorbis_get_info(decoder).sample_rate;
            result.looping = looping;
        }
    }
    return result;
}

ogg_t open_cd_vorbis(s32 track_number, bool looping) {
    if (track_number >= 2 && track_number <= 20) {
        char basename[32];
        snprintf(basename, sizeof(basename), "rayman%02d.ogg", track_number);
        return open_music_vorbis(basename, looping);
    }
    ogg_t result = {0};
    return result;
}

void stop_ogg(ogg_t* ogg) {
	if (ogg->decoder) {
		stb_vorbis_close(ogg->decoder);
        ogg->decoder = NULL;
	}
	if (ogg->file) {
		free(ogg->file);
        ogg->file = NULL;
	}
}

void play_cd_track(s32 track_number, bool looping) {
	stop_ogg(&ogg_cd_track);
	ogg_cd_track = open_cd_vorbis(track_number, looping);
	is_ogg_playing = true;
	MusicCdActive = true;
}

void play_movie_music(const char* movie_filename, s32 cd_track) {
    // Rayverse extension: later PC/GOG releases omit the original movie CD
    // audio. Prefer explicit movie tracks without remapping normal CD playback.
    const char* basename = NULL;
    if (strcasecmp(movie_filename, "intro.dat") == 0) {
        basename = "intro.ogg";
    } else if (strcasecmp(movie_filename, "conclu.dat") == 0) {
        basename = "conclu.ogg";
    }
    if (basename) {
        ogg_t movie_track = open_music_vorbis(basename, false);
        if (movie_track.decoder) {
            stop_ogg(&ogg_cd_track);
            ogg_cd_track = movie_track;
            is_ogg_playing = true;
            MusicCdActive = true;
            return;
        }
    }
    // Missing or undecodable overrides retain the original numbered lookup.
    if (cd_track > 0) {
        play_cd_track(cd_track, false);
    }
}

//float volume = 0.5f;

static void rewind_ogg_resampler(ogg_t* ogg) {
    stb_vorbis_seek_start(ogg->decoder);
    ogg->resample_buffer_start = 0;
    ogg->resample_buffer_count = 0;
}

static void play_ogg_resampled(game_sound_buffer_t* sound_buffer, ogg_t* ogg) {
    const u32 output_rate = sound_buffer->samples_per_second;
    const u64 end_position = (u64)ogg->sample_count * output_rate;
    u32 output_index = 0;
    if (output_rate == 0 || ogg->sample_rate == 0 || end_position == 0) {
        is_ogg_playing = false;
    } else {
        for (; output_index < sound_buffer->sample_count; ++output_index) {
            if (ogg->resample_position >= end_position) {
                ogg->resample_position %= end_position;
                rewind_ogg_resampler(ogg);
                SetCompteurTrameAudio();
                if (!ogg->looping) {
                    ogg->resample_position = 0;
                    is_ogg_playing = false;
                    break;
                }
            }
            u32 source_index = (u32)(ogg->resample_position / output_rate);
            u32 next_index = MIN(source_index + 1, ogg->sample_count - 1);
            while (next_index >= ogg->resample_buffer_start + ogg->resample_buffer_count) {
                // Preserve the boundary sample for interpolation across decode blocks.
                u32 carry = ogg->resample_buffer_count != 0;
                if (carry) {
                    u32 last = ogg->resample_buffer_count - 1;
                    ogg->resample_buffer[0] = ogg->resample_buffer[2 * last];
                    ogg->resample_buffer[1] = ogg->resample_buffer[2 * last + 1];
                    ogg->resample_buffer_start += last;
                }
                s32 frames = stb_vorbis_get_samples_short_interleaved(ogg->decoder, 2,
                        ogg->resample_buffer + 2 * carry,
                        COUNT(ogg->resample_buffer) - 2 * carry);
                ogg->resample_buffer_count = carry + frames;
                if (frames == 0) {
                    // An unexpectedly truncated stream must not spin in the refill loop.
                    is_ogg_playing = false;
                    goto silence_tail;
                }
            }
            u32 index = source_index - ogg->resample_buffer_start;
            u32 next = next_index - ogg->resample_buffer_start;
            u32 fraction = (u32)(ogg->resample_position % output_rate);
            for (u32 channel = 0; channel < 2; ++channel) {
                s32 a = ogg->resample_buffer[2 * index + channel];
                s32 b = ogg->resample_buffer[2 * next + channel];
                sound_buffer->samples[2 * output_index + channel] =
                        (s16)(a + (s64)(b - a) * fraction / output_rate);
            }
            ogg->resample_position += ogg->sample_rate;
        }
    }
silence_tail:
    memset(sound_buffer->samples + 2 * output_index, 0,
           (sound_buffer->sample_count - output_index) * 2 * sizeof(s16));
}

void play_ogg(game_sound_buffer_t* sound_buffer, ogg_t* ogg) {
    if (ogg->sample_rate != (u32)sound_buffer->samples_per_second) {
        play_ogg_resampled(sound_buffer, ogg);
        return;
    }
	s32 samples_requested = sound_buffer->sample_count;
	s32 samples_filled = stb_vorbis_get_samples_short_interleaved(ogg->decoder, 2, sound_buffer->samples, samples_requested * 2);
	if (samples_filled < samples_requested) {
		s32 bytes_per_sample = sizeof(short) * 2;
		s32 bytes_filled = samples_filled * bytes_per_sample;
		s32 remaining_bytes = (samples_requested - samples_filled) * bytes_per_sample;
		// bytes_filled is a byte offset, not an s16 sample index.
		memset((u8*)sound_buffer->samples + bytes_filled, 0, remaining_bytes);
		if (samples_filled == 0) {
            stb_vorbis_seek_start(ogg->decoder); // loop track (rewind to start)
            SetCompteurTrameAudio();
            if (!ogg->looping) {
                is_ogg_playing = false;
            }
		}
	}
}

float t_pitch_sine;
float t_sine;

void debug_produce_test_sound(game_sound_buffer_t* sound_buffer) {
	s16 tone_volume = 3000;
	float tone_hz = 400.0f;
	//	real32 wave_period = sound_buffer.samples_per_second/tone_hz;
	float pitch_hz = 2.0f;
	float pitch_period = sound_buffer->samples_per_second / pitch_hz;


	s16* sample_out = sound_buffer->samples;

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
		s16 sample_value = (s16) (sine_value * tone_volume);
#else
		s16 sample_value = 0;
#endif

		*sample_out++ = sample_value;//left
		*sample_out++ = sample_value;//right
	}
}


s16 play_digi_snd(snd_t* snd) {
    for (s32 i = 0; i < COUNT(digi_voices); ++i) {
        snd_t* voice = digi_voices + i;
        if (!voice->is_playing) {
            *voice = *snd;
            voice->position = 0.0f;
            voice->is_playing = true;
            return (s16)i;
        }
    }
    return -1;
}

static s32 get_digi_sample(snd_t* voice, s32 frame_index, s32 channel_index) {
    s32 channel_count = voice->is_stereo ? 2 : 1;
    s32 sample_index = frame_index * channel_count + channel_index;
    u8* sample = voice->data + sample_index * voice->bytes_per_sample;

    if (voice->bytes_per_sample == 2) {
        u16 raw_sample = (u16)sample[0] | ((u16)sample[1] << 8);
        return voice->is_signed ? (s32)(s16)raw_sample : (s32)raw_sample - 32768;
    }

    return voice->is_signed ? (s32)(s8)sample[0] * 256 : ((s32)sample[0] - 128) * 256;
}

void lock_audio(void) {
    print_once("Not implemented: lock_audio");
    //stub // TODO: prevent reading after sound banks have been freed (need to acquire a lock!)
}

void unlock_audio(void) {
    print_once("Not implemented: unlock_audio");
    //stub // TODO: prevent reading after sound banks have been freed (need to acquire a lock!)
}

void game_get_sound_samples(game_sound_buffer_t* output_buffer) {
    // Start with silence (zero everything)
    s32 bytes_per_sample = sizeof(short) * 2;
    memset(output_buffer->samples, 0, output_buffer->sample_count * bytes_per_sample);

    // Play music
	if (is_ogg_playing) {
		if (ogg_cd_track.decoder) {
			play_ogg(output_buffer, &ogg_cd_track);
		}
	}

    // Play digi sounds
    lock_audio();

    // We need to resample the sound's sample rate (which may also vary depending on pitch shifting) to the output rate.
    const float output_sample_rate = 44100.0f;
    for (s32 i = 0; i < COUNT(digi_voices); ++i) {
        snd_t* voice = digi_voices + i;
        if (!voice->is_playing) {
            continue;
        }

        // Calculate the step increment for the source sample position.
        // This is the ratio of the sound's sample rate to the output sample rate.
        float step = (float)voice->sample_rate / output_sample_rate;

        // Get a pointer to the start of the output buffer for this frame
        s16* dest = output_buffer->samples;

        // Loop through the number of samples we need to generate for this frame
        for (s32 j = 0; j < output_buffer->sample_count; ++j) {

            // 1. Find the current position and check boundaries
            s32 source_index = (s32)voice->position;

            // Stop if we've read past the end of the source data.
            // We need source_index and source_index + 1 for interpolation, so we check against size - 1.
            if (source_index >= voice->sample_count - 1) {
                if (voice->is_looping && voice->loop_start >= 0 && voice->loop_start < voice->loop_end && voice->loop_end <= voice->sample_count) {
                    voice->position = (float)voice->loop_start;
                    source_index = voice->loop_start;
                } else {
                    voice->is_playing = false;
                    voice_table[i].obj = -2;
                    voice_table[i].snd = -1;
                    voice_is_working[i] = 0;
                    break; // Stop processing this voice for this frame
                }
            }

            // 2. Get samples for interpolation
            s32 left_s1 = get_digi_sample(voice, source_index, 0);
            s32 left_s2 = get_digi_sample(voice, source_index + 1, 0);
            s32 right_s1 = voice->is_stereo ? get_digi_sample(voice, source_index, 1) : left_s1;
            s32 right_s2 = voice->is_stereo ? get_digi_sample(voice, source_index + 1, 1) : left_s2;

            // 3. Perform Linear Interpolation
            // Find the fractional part of our position, which is our interpolation factor 't'
            float t = voice->position - (float)source_index;

            // Interpolate between the two samples
            float interpolated_sample_l = (1.0f - t) * (float)left_s1 + t * (float)left_s2;
            float interpolated_sample_r = (1.0f - t) * (float)right_s1 + t * (float)right_s2;

            // 4. Scale to 16-bit and apply volume
            s32 pan = MIN(127, MAX(0, voice->pan));
            float left_gain = (float)snd_sqrt_table[127 - pan] / 256.0f;
            float right_gain = (float)snd_sqrt_table[pan] / 256.0f;
            s32 dest_sample_l = (s32)(interpolated_sample_l * voice->volume * left_gain);
            s32 dest_sample_r = (s32)(interpolated_sample_r * voice->volume * right_gain);

            // 5. Mix into the output buffer (mono to stereo)
            // Add the new sample to the existing data in the buffer
            s32 mixed_l = dest[0] + dest_sample_l;
            s32 mixed_r = dest[1] + dest_sample_r;

            // Clamp the values to prevent 16-bit integer overflow (clipping)
            dest[0] = (s16)MIN(32767, MAX(-32768, mixed_l));
            dest[1] = (s16)MIN(32767, MAX(-32768, mixed_r));
            dest += 2; // Move to the next stereo sample pair

            // 6. Advance our position in the source sample data
            voice->position += step;
        }
    }
    unlock_audio();
}

//71C00
void manage_snd_event(void) {
    //nullsub
}

//71C04
void SetVolumeSound(s16 volume) {
    Volume_Snd = volume;
}

//71C10
void stop_all_snd(void) {
    if (CarteSonAutorisee) {
        for (s32 i = 0; i < COUNT(voice_table); ++i) {
            KeyOff(i, 0, 0, 0, 0);
            voice_table[i].obj = -2;
            voice_table[i].snd = -1;
            voice_is_working[i] = 0;
        }
        pt_pile_snd = 0;
    }
}

//71C40
void stop_ray_snd(void) {
    //stub; unused in the PC version
}

//71CA8
u8 get_pan_snd(obj_t* obj) {
    s32 x = obj->offset_bx + obj->screen_x;
    s32 result;
    if (x > 416) {
        return 127;
    } else if (x >= -96) {
        result = (x + 96) >> 2;
    } else {
        result = 0;
    }
    if (result > 127)
        return 127;
    else {
        return (u8)result;
    }
}

//71CEC
s32 get_vol_snd(obj_t* obj) {
    s16 x, y, w, h;
    GET_ANIM_POS(obj, &x, &y, &w, &h);
    s16 vol = (s16)(127 - ((Abs(y + (h >> 1) - (ray.y + ray.offset_by) + 40) + Abs(x + (w >> 1) - (ray.x + ray.offset_bx))) >> 2));
    if (vol < 0) {
        return 0;
    } else {
        return vol;
    }
}

//71D80
void raj_env_sound(s16 volume) {
    SetVolumeSound((volume * 127) / 20);
}

//71DA0
void raj_env_audio(s16 enabled) {
    //nullsub
}

//71DA4
bool raj_env_stereo(s16 stereo) {
    return stereo != 0;
}

//71DA8
bool InitSnd(void) {
    if (CarteSonAutorisee) {
        for (s32 i = 0; i < 20; ++i) {
            stk_obj[i] = -2;
            stk_snd[i] = 0;
        }
        pt_pile_snd = 0;
        indice_snd_wiz = 0;
        indice_ray_wait = 0;
        indice_trz_wait = -2;
        for (s32 i = 0; i < 32; ++i) {
            voice_table[i].obj = -2;
        }
        raj_env_sound(18);
        return raj_env_stereo(options_jeu.is_stereo);
    } else {
        return false;
    }
}

//71E44
s16 last_snd(s32 obj_id) {
    s32 i = 0;
    for (; i < 20; ++i) {
        if (stk_obj[i] == obj_id) break;
    }
    if (i == 20) {
        while (i > 0) {
            --i;
            if (stk_snd[i] == 0) break;
        }
    }
    return stk_snd[i];
}

//71EA0
s32 get_pile_obj(s16 obj_id) {
    if (pt_pile_snd == 0) {
        return -1;
    }
    pile_snd_t* cur_pile_snd = pile_snd;
    s32 index = 0;
    if (pile_snd[0].obj != obj_id && pt_pile_snd > 0) {
        s16 next;
        do {
            ++cur_pile_snd;
            ++index;
        } while (pt_pile_snd > index && cur_pile_snd->obj != obj_id);
    }
    if (index == pt_pile_snd) {
        return -1;
    }

    return index;
}

//71EF4
s32 get_voice_obj(s32 obj_id) {
    voice_t* cur_voice = voice_table;
    s32 index = 0;

    if (voice_table[0].obj != obj_id) {
        do {
            ++index;
            ++cur_voice;
        } while (index < COUNT(voice_table) && cur_voice->obj != obj_id);
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }

    return index;
}

//71F30 - unused in the PC version
s32 get_voice_snd(s32 snd) {
    voice_t* cur_voice = voice_table;
    s32 index = 0;

    if (voice_table[0].snd != snd) {
        do {
            ++index;
            ++cur_voice;
        } while (index < COUNT(voice_table) && cur_voice->snd != snd);
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }

    return index;

}

//71F6C
s32 get_voice_obj_snd(s16 obj_id, s16 snd) {
    voice_t* voice = voice_table;
    s32 index = 0;
    // NOTE: there is a conditional here, but the branches are identical?
    // if (snd = 48) { ... } else { ... }
    while (index < COUNT(voice_table) && (voice->snd != snd || voice->obj != obj_id)) {
        ++index;
        ++voice;
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }
    return index;
}

//71FC8
void erase_pile_snd(s16 obj_id) {
    s16 pile_obj_id = get_pile_obj(obj_id);
    if (pile_obj_id != -1) {
        for (s32 i = pile_obj_id; i < pt_pile_snd; ++i) {
            pile_snd[i] = pile_snd[i+1];
        }
        if (pt_pile_snd > 0) {
            --pt_pile_snd;
        }
    }
}

//720B8
void nettoie_pile_snd(void) {
    s32 index = 0;
    while (index < pt_pile_snd) {
        pile_snd_t* pile = pile_snd + index;
        if (pile->end_time < map_time && pile->end_time != 0) {
            // erase entry
            for (s32 i = index; i < pt_pile_snd; ++i) {
                pile_snd[i] = pile_snd[i+1];
            }
            if (pt_pile_snd > 0) {
                --pt_pile_snd;
            }
        } else {
            ++index;
        }
    }
}

//721C4
void erase_voice_table(s32 obj_id) {
    voice_t* cur_voice = voice_table;
    s32 result = 0;

    if (voice_table[0].obj != obj_id) {
        do {
            ++result;
            ++cur_voice;
        } while (result < COUNT(voice_table) && cur_voice->obj != obj_id);
    }
    if (result != COUNT(voice_table)) {
        cur_voice->obj = -2;
    }
}

//72200
u8 snd_in_pile_snd(s16 snd) {
    u8 result = false;
    s32 index = 0;
    if (pt_pile_snd != 0) {
        pile_snd_t* pile = pile_snd;
        if (pile_snd[0].snd != snd) {
            do {
                ++index;
                ++pile;
            } while (index < pt_pile_snd && pile->snd != snd);
        }
        return pt_pile_snd != index;
    }
    return false;
}

//72254
s32 vol_r(s16 a1, s16 a2) {
    s32 result = (a1 * snd_sqrt_table[a2]) >> 8;
    return result;
}

//7226C
s32 vol_l(s16 a1, s16 a2) {
    s32 result = (a1 * snd_sqrt_table[127 - a2]) >> 8;
    return result;
}

//7228C
void PlaySnd(s16 snd, s16 obj_id) {
    if (CarteSonAutorisee) {
        if ((ray.scale != 0 && obj_id == reduced_rayman_id) || obj_id == rayman_obj_id) {
            obj_id = -1;
        }
        if (obj_id == -1 && snd != 15) {
            indice_ray_wait = 0;
        }
        s16 sound_to_interrupt = last_snd(obj_id);
        // manage_snd() uses -1 after clearing a managed voice.
        u8 previous_flags = sound_to_interrupt >= 0 ? sound_table[sound_to_interrupt] : 0;
//        printf("playing sound %d: want to interrupt sound %d for obj %d\n", sound_id, sound_to_interrupt, obj_id);
        if (sound_to_interrupt != snd && (previous_flags & 8) != 0) {
            erase_pile_snd(obj_id);
            s16 voice_id = get_voice_obj_snd(obj_id, sound_to_interrupt);
            if (voice_id >= 0) {
                voice_t* voice = voice_table + voice_id;
                sound_table_entry_t* sound = hard_sound_table + voice->snd;
                KeyOff(voice_id, bank_to_use[voice->snd], sound->prog, sound->tone, sound->note);
                voice->obj = -2;
                voice->snd = -1;
                voice_is_working[voice_id] = 0;
            }
        }

        u8 pan_snd = 0;
        u8 vol_snd = 0;
        if (!dans_la_map_monde && snd != 0 && (snd != sound_to_interrupt || (previous_flags & 4) != 0)) {
            if (obj_id == -1 || (obj_id == rayman_obj_id && rayman_obj_id != -1)) {
                pan_snd = get_pan_snd(&ray);
                vol_snd = 127;
            } else {
                pan_snd = get_pan_snd(level.objects + obj_id);
                if (sound_table[snd] & 1) {
                    vol_snd = get_vol_snd(level.objects + obj_id);
                } else {
                    vol_snd = 127;
                }
            }

            s16 prog = hard_sound_table[snd].prog;
            u8 tone = hard_sound_table[snd].tone;
            u8 note = hard_sound_table[snd].note;

            switch(snd) {
                case 53:
                    if (level.objects[obj_id].type != TYPE_238_POING_FEE) {
                        prog = hard_sound_table[53].prog;
                        tone = hard_sound_table[53].tone;
                        note = hard_sound_table[53].note;
                    } else {
                        note = hard_sound_table[0].note;
                    }
                    break;
                case 47:
                    if (num_world != 5) {
                        prog = hard_sound_table[47].prog;
                        tone = hard_sound_table[47].tone;
                        note = hard_sound_table[47].note;
                        vol_snd = 127;
                    } else {
                        prog = hard_sound_table[170].prog;
                        tone = hard_sound_table[170].tone;
                        note = hard_sound_table[170].note;
                        vol_snd = 127;
                    }
                    break;
                case 15:
                    ++indice_ray_wait;
                    if (indice_ray_wait > 2) {
                        indice_ray_wait = 0;
                        prog = hard_sound_table[15].prog;
                        tone = hard_sound_table[15].tone;
                        note = hard_sound_table[15].note;
                    }
                    else {
                        prog = 255;
                    }
                    erase_pile_snd(-1);
                    break;
                case 80:
                    ++indice_trz_wait;
                    if (indice_trz_wait > 1) {
                        prog = hard_sound_table[80].prog;
                        tone = hard_sound_table[80].tone;
                        note = hard_sound_table[80].note;
                        indice_trz_wait = 0;
                    }
                    else {
                        prog = 255;
                    }
                    break;
                case 14:
                    prog = hard_sound_table[14].prog;
                    tone = hard_sound_table[14].tone;
                    note = hard_sound_table[14].note + not_snd_wiz[level.objects[obj_id].sub_etat - 24]; // ting note depends on the sub_etat
                    break;
                case 19:
                    prog = hard_sound_table[19].prog;
                    tone = hard_sound_table[19].tone;
                    note = hard_sound_table[19].note;
                    break;
                case 245:
                    prog = hard_sound_table[245].prog;
                    tone = hard_sound_table[245].tone;
                    note = hard_sound_table[245].note;
                    break;
                    // It seems these are PS1 only:
                    /*case 55:
                        prog = -1;
                        if ((s16) PS1_SongIsPlaying(0xc) == 0)
                        {
                            PS1_PlaySnd(0xc, 0);
                        }
                        break;
                    case 57:
                        prog = -1;
                        PS1_StopPlayingSnd(0xc);
                        break;
                    case 103:
                        prog = -1;
                        if (SsIsEos(PS1_SepInfos[22].access_num, PS1_SepInfos[22].seq_num) == 0)
                        {
                            PS1_PlaySnd(0x16, 0);
                            D_801CEFCC = true;
                            D_801CEFCE = obj_id;
                        }
                        break;*/
                default:
                    prog = hard_sound_table[snd].prog;
                    tone = hard_sound_table[snd].tone;
                    note = hard_sound_table[snd].note;
                    break;
            }

            if (prog != -1 && prog != 255) {
                erase_pile_snd(obj_id);
                s32 voll = vol_l(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14,
                                 pan_snd); // NOTE: has no effect?
                s32 volr = vol_r(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14,
                                 pan_snd); // NOTE: has no effect?

                if (frame_snd[snd] == 0) {
                    // NOTE: the PS1 version implements left/right directional sound here, using SsUtKeyOn
                    s32 voice_id = KeyOn(bank_to_use[snd], prog, tone, note,
                                         Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd);
                    if (voice_id != -1) {
                        erase_voice_table(obj_id);
                        voice_table[voice_id].obj = obj_id;
                        voice_table[voice_id].vol = vol_snd;
                        voice_table[voice_id].pan = pan_snd;
                        voice_table[voice_id].snd = snd;
                        if ((sound_table[snd] & 0x10) != 0) {
                            voice_is_working[voice_id] = true;
                        }
                    }
                } else {
                    nettoie_pile_snd();
                    erase_pile_snd(obj_id);
                    pile_snd[pt_pile_snd].obj = obj_id;
                    pile_snd[pt_pile_snd].snd = snd;
                    pile_snd[pt_pile_snd].prog = prog;
                    pile_snd[pt_pile_snd].tone = tone;
                    pile_snd[pt_pile_snd].note = note;
                    pile_snd[pt_pile_snd].vol = hard_sound_table[snd].volume;
                    pile_snd[pt_pile_snd].field_C = vol_snd;
                    pile_snd[pt_pile_snd].pan = pan_snd;
                    pile_snd[pt_pile_snd].end_time = frame_snd[snd] + map_time;
                    if (snd_bis[snd] != 0) {
                        pile_snd[pt_pile_snd].field_14 = 1;
                    } else {
                        pile_snd[pt_pile_snd].field_14 = 0;
                    }
                    if (pt_pile_snd < 9) {
                        ++pt_pile_snd;
                    }
                }
                if ((snd == 203 || snd == 237 || snd == 209 || snd == 225 || snd == 236) &&
                    (dead_time == 64 || dead_time == 128)) {
                    start_cd_bbdead();
                }
            }
        }

        // PC loc_728E4 also runs when playback is skipped, notably for sound 0.
        // Record that stop so a moving plum (or another loop) can restart the
        // same sound later instead of being rejected as a duplicate forever.
        for (s32 i = 0; i < 20; ++i) {
            if (stk_obj[i] == obj_id || stk_obj[i] == -2) {
                stk_obj[i] = obj_id;
                stk_snd[i] = snd;
                return;
            }
        }
        // A stop updates an existing/free slot, but must not evict another
        // object's history when the table is full (PC loc_72909).
        if (snd != 0) {
            s32 index = 19;
            while (index >= 0) {
                s16 owner = stk_obj[index];
                // Rayman's -1 owner is valid history, not level.objects[-1].
                // Stale IDs from another level are reusable inactive entries.
                if (owner != -1 && (owner < 0 || owner >= level.nb_objects || !level.objects[owner].is_active)) {
                    break;
                }
                --index;
            }
            if (index == -1) {
                index = 0;
            }
            stk_obj[index] = obj_id;
            stk_snd[index] = snd;
        }

    }
}

//72960
void PlaySnd_old(s16 sound_id) {
    s16 voice_index = -1;
    if (CarteSonAutorisee) {
        u8 prog = hard_sound_table[sound_id].prog;
        if (prog != 255) {
            voice_index = KeyOn(bank_to_use[sound_id], prog,
                                hard_sound_table[sound_id].tone,
                                hard_sound_table[sound_id].note,
                                Volume_Snd * hard_sound_table[sound_id].volume >> 7, 64);
            if (voice_index != -1) {
                voice_table[voice_index].obj = -2;
                voice_table[voice_index].vol = 64;
                voice_table[voice_index].pan = 64;
                voice_table[voice_index].snd = sound_id;
                if (sound_table[sound_id] & 0x10) {
                    voice_is_working[voice_index] = 1;
                }
            }
        }
    }
}

//72A1C
static u8 get_managed_sound_volume(s16 obj_id, s16 snd) {
    if (obj_id == -1 || (obj_id == rayman_obj_id && rayman_obj_id != -1)) {
        return 127;
    }
    if ((sound_table[snd] & 1) != 0 && (sound_table[snd] & 2) != 0) {
        return (u8)get_vol_snd(level.objects + obj_id);
    }
    return 127;
}

static s32 get_effective_sound_volume(s16 snd, s16 obj_volume) {
    return Volume_Snd * obj_volume * hard_sound_table[snd].volume >> 14;
}

static void remove_pile_snd_at(s32 index) {
    if (index < 0 || index >= pt_pile_snd) {
        return;
    }
    for (s32 i = index; i < pt_pile_snd - 1; ++i) {
        pile_snd[i] = pile_snd[i + 1];
    }
    if (pt_pile_snd > 0) {
        --pt_pile_snd;
    }
}

void setvol(s16 obj_id) {
    if (!CarteSonAutorisee) {
        return;
    }
    if (ray.scale != 0 && obj_id == reduced_rayman_id) {
        obj_id = -1;
    }

    s32 pile_index = get_pile_obj(obj_id);
    if (pile_index != -1) {
        pile_snd_t* pile = pile_snd + pile_index;
        u8 vol = get_managed_sound_volume(obj_id, pile->snd);
        pile->field_C = vol;

        if (pile->end_time == map_time) {
            s16 snd = pile->snd;
            s32 effective_volume = get_effective_sound_volume(snd, vol);
            s16 voice_id = KeyOn(bank_to_use[snd], (u8)pile->prog, (u8)pile->tone, (u8)pile->note, (u8)effective_volume, (u8)pile->pan);
            if (voice_id != -1) {
                erase_voice_table(obj_id);
                voice_table[voice_id].obj = obj_id;
                voice_table[voice_id].vol = vol;
                voice_table[voice_id].pan = pile->pan;
                voice_table[voice_id].snd = snd;
                if ((sound_table[snd] & 0x10) != 0) {
                    voice_is_working[voice_id] = 1;
                }
            }

            if (pile->field_14 != 0 && snd_bis[snd] != 0) {
                s16 next_snd = snd_bis[snd];
                pile->snd = next_snd;
                pile->prog = hard_sound_table[next_snd].prog;
                pile->tone = hard_sound_table[next_snd].tone;
                pile->note = hard_sound_table[next_snd].note;
                pile->vol = hard_sound_table[next_snd].volume;
                pile->end_time = map_time + frame_snd_bis[snd];
                pile->field_14 = 0;
            } else {
                remove_pile_snd_at(pile_index);
            }
        }
    }

    s32 voice_id = get_voice_obj(obj_id);
    if (voice_id != -1) {
        voice_t* voice = voice_table + voice_id;
        u8 vol = get_managed_sound_volume(obj_id, voice->snd);
        voice->vol = vol;
        if (voice_is_working[voice_id]) {
            KeyVol(voice_id, get_effective_sound_volume(voice->snd, vol), voice->pan);
        }
    }
}

//72E74
void setpan(s16 obj_id) {
    if (!CarteSonAutorisee) {
        return;
    }
    if (ray.scale != 0 && obj_id == reduced_rayman_id) {
        obj_id = -1;
    }

    u8 pan = obj_id == -1 ? 64 : get_pan_snd(level.objects + obj_id);

    s32 voice_id = get_voice_obj(obj_id);
    if (voice_id != -1) {
        voice_table[voice_id].pan = pan;
        if (voice_is_working[voice_id]) {
            KeyVol(voice_id, get_effective_sound_volume(voice_table[voice_id].snd, voice_table[voice_id].vol), pan);
        }
        return;
    }

    s32 pile_index = get_pile_obj(obj_id);
    if (pile_index != -1) {
        pile_snd[pile_index].pan = pan;
    }
}

//72F38
void manage_snd(void) {
    if (!CarteSonAutorisee) {
        return;
    }

    for (s32 i = 0; i < COUNT(voice_table); ++i) {
        voice_t* voice = voice_table + i;
        bool should_clear = false;
        if (!digi_voices[i].is_playing && voice->snd != -1) {
            should_clear = true;
        }
        if (voice->obj >= 0 && (voice->obj >= level.nb_objects || !level.objects[voice->obj].is_active)) {
            should_clear = true;
        }
        if (voice->obj == -2 && voice->snd != -1) {
            should_clear = true;
        }

        if (should_clear) {
            if (voice->snd >= 0 && (sound_table[voice->snd] & 0x10) != 0) {
                KeyOff(i, bank_to_use[voice->snd], hard_sound_table[voice->snd].prog, hard_sound_table[voice->snd].tone, hard_sound_table[voice->snd].note);
                for (s32 j = 0; j < COUNT(stk_snd); ++j) {
                    if (stk_obj[j] == voice->obj) {
                        stk_snd[j] = -1;
                        break;
                    }
                }
            }
            voice->obj = -2;
            voice->snd = -1;
            voice_is_working[i] = 0;
            continue;
        }

        if (voice->obj >= 0 && voice->obj < level.nb_objects && level.objects[voice->obj].is_active) {
            setvol(voice->obj);
            setpan(voice->obj);
        }
    }

    for (s32 i = 0; i < pt_pile_snd; ++i) {
        s16 obj_id = pile_snd[i].obj;
        if (obj_id >= 0 && obj_id < level.nb_objects && level.objects[obj_id].is_active) {
            setvol(obj_id);
            setpan(obj_id);
        } else if (obj_id == -1) {
            setvol(-1);
            setpan(-1);
        }
    }
}

//730DC
void mute_snd_bouclant(void) {
    if (CarteSonAutorisee) {
        for (s32 i = 0; i < COUNT(voice_table); ++i) {
            s16 snd = voice_table[i].snd;
            if (snd == 6 || snd == 245 || snd == 2 || (snd >= 0 && (sound_table[snd] & 0x10) != 0)) {
                KeyVol(i, 0, 0);
            }
        }
    }
}

//73138
void mute_snd(void) {
    for (s32 i = 0; i < COUNT(voice_table); ++i) {
        KeyVol(i, 0, 0);
    }
}

//73164
void start_freeze_snd(void) {
    mute_snd();
    was_in_freeze = 1;
}

//73178
void stop_freeze_snd(void) {
    was_in_freeze = 0;
}

//7319C
void PlayTchatchVignette(s32 snd) {
    if (CarteSonAutorisee) {
        if (snd == 0) {
            switch(num_world_choice) {
                case 1: {
                    snd = 1;
                } break;
                case 3: {
                    snd = 2;
                } break;
                case 4: {
                    snd = 4;
                } break;
                case 5: {
                    if (num_level_choice != 3) {
                        return;
                    }
                    snd = 3;
                } break;
                case 6: {
                    snd = 7;
                } break;
                default: break;
            }
        }
        PlayTchatch(snd, Volume_Snd);
    }
}

//73204
void TestCdLoop(void) {
    if (cdTime != 0) {
        if (cdTime != -1) {
            //dos_get_time_of_day();
            cdTime = 10 * (cdTimeEnd = cdTimeStart) / 182;
            s32 duration = timeCd[22 * (num_world - 1) + num_level];
            cdTime += 2;
            if (cdTime >= duration) {
                if (flagCDPlay != 0) {
                    if (flagCDPlay >= 4) {
                        //dos_get_time_of_day();
                        flagCDPlay = 0;
                        cdTime = 0;
                        phaseCd = !phaseCd;
                        //sub_2A654(map_cd_tracks[22 * (num_world - 1) + num_level -1 ]); // cd loop?
                    } else {
                        ++flagCDPlay;
                    }
                } else {
                    ++flagCDPlay;
                    //sub_2A6C4();
                }
            }
        }
    } else {
        play_cd_track(map_cd_tracks[22 * (num_world - 1) + num_level -1 ], true);
        phaseCd = 0;
        cdTime = 1;
    }
}

//73384
void InitMusic(void) {
    MusicCdActive = 1;
    print_once("Not implemented: InitMusic"); //stub
}

//733BC
void DoneMusic(void) {
    print_once("Not implemented: DoneMusic"); //stub
}
