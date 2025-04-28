
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











ogg_t open_cd_vorbis(i32 track_number) {
	ogg_t result = {0};
	if (track_number >= 2 && track_number <= 20) {
		char filename[512];
		snprintf(filename, sizeof(filename), "Music/rayman%02d.ogg", track_number);
		mem_t* mem = read_entire_file(filename, false);
		if (!mem) {
			// Try to find a Music directory one level up (for GOG Rayman Forever version)
			snprintf(filename, sizeof(filename), "../Music/rayman%02d.ogg", track_number);
			mem = read_entire_file(filename, false);
		}
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

void stop_ogg(ogg_t* ogg) {
	if (ogg->decoder) {
		stb_vorbis_close(ogg->decoder);
	}
	if (ogg->file) {
		free(ogg->file);
	}
}

void play_cd_track(i32 track_number) {
	stop_ogg(&ogg_cd_track);
	ogg_cd_track = open_cd_vorbis(track_number);
	is_ogg_playing = true;
	MusicCdActive = true;
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
		if (samples_filled == 0) {
			is_ogg_finished = true;
			is_ogg_playing = false;
            SetCompteurTrameAudio();
		}
	}
}

float t_pitch_sine;
float t_sine;

void debug_produce_test_sound(game_sound_buffer_t* sound_buffer) {
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
}


i16 play_digi_snd(snd_t* snd) {
    for (i32 i = 0; i < COUNT(digi_voices); ++i) {
        snd_t* voice = digi_voices + i;
        if (!voice->is_playing) {
            *voice = *snd;
            voice->cursor = 0;
            voice->is_playing = true;
            break;
        }
    }
    return -1; // NOTE: should we return something more appropriate here?
}

void lock_audio(void) {
    //stub // TODO: prevent reading after sound banks have been freed (need to acquire a lock!)
}

void unlock_audio(void) {
    //stub // TODO: prevent reading after sound banks have been freed (need to acquire a lock!)
}

void game_get_sound_samples(game_sound_buffer_t* output_buffer) {
    // Play music
	if (is_ogg_playing) {
		if (ogg_cd_track.decoder) {
			play_ogg(output_buffer, &ogg_cd_track);
		}
	} else {
        i32 bytes_per_sample = sizeof(short) * 2;
        memset(output_buffer->samples, 0, output_buffer->sample_count * bytes_per_sample);
	}

    // Play digi sounds
    lock_audio();
    for (i32 i = 0; i < COUNT(digi_voices); ++i) {
        snd_t* voice = digi_voices + i;
        if (voice->is_playing) {
            // Source sample rate is 11025 Hz, target sample rate is 44100 Hz
            // We can implement a simple upsampling with linear interpolation
            // TODO: more advanced upsampling to account for pitch differences
            i32 total_source_samples = voice->sample_count * 4;
            i32 dest_samples_remaining = total_source_samples - voice->dest_cursor;
            if (dest_samples_remaining > 0) {

                i32 dest_samples_to_output = MIN(output_buffer->sample_count, dest_samples_remaining);
                i32 prev_source_sample = (voice->cursor > 0) ? (i32)voice->data[voice->cursor - 1] - 128 : 0;
                i32 next_source_sample = (i32)(voice->data[voice->cursor]) - 128;
                i16* dest_pos = output_buffer->samples;
                for (i32 j = 0; j < dest_samples_to_output; ++j) {

                    // Do the linear interpolation while also converting from 8-bit to 16-bit samples
                    // TODO: account for volume
                    i32 dest_sample;
                    switch(voice->dest_cursor % 4) {
                        default:
                        case 0: {
                            dest_sample = prev_source_sample * (256/2);
                        } break;
                        case 1: {
                            dest_sample = prev_source_sample * (192/2) + next_source_sample * (64/2);
                        } break;
                        case 2: {
                            dest_sample = prev_source_sample * (128/2) + next_source_sample * (128/2);
                        } break;
                        case 3: {
                            dest_sample = prev_source_sample * (64/2) + next_source_sample * (192/2);
                            // we're done with this source sample, move to the next one
                            prev_source_sample = next_source_sample;
                            next_source_sample = (i32)voice->data[voice->cursor] - 128;
                            ++voice->cursor;
                        } break;
                    }

                    // Mix sound by adding together voices, then cap between INT16_MIN and INT16_MAX to prevent rollover
                    i32 mixed = *dest_pos + dest_sample;
                    mixed = MIN(32767, MAX(-32768, mixed));

                    *dest_pos++ = (i16)mixed;
                    *dest_pos++ = (i16)mixed;
                    ++voice->dest_cursor;
                }
            } else {
                voice->is_playing = false; // sound finished playing
            }
        }
    }
    unlock_audio();
}

//71C00
void manage_snd_event(void) {
    //nullsub
}

//71C04
void SetVolumeSound(i16 volume) {
    Volume_Snd = volume;
}

//71C10
void stop_all_snd(void) {
    //stub
}

//71C40
void stop_ray_snd(void) {
    //stub; unused in the PC version
}

//71CA8
u8 get_pan_snd(obj_t* obj) {
    return 0; //stub
}

//71CEC
i32 get_vol_snd(obj_t* obj) {
    return 0; //stub
}

//71D80
void raj_env_sound(i16 volume) {
    SetVolumeSound((volume * 127) / 20);
}

//71DA0
void raj_env_audio(i16 enabled) {
    //nullsub
}

//71DA4
bool raj_env_stereo(i16 stereo) {
    return stereo != 0;
}

//71DA8
bool InitSnd(void) {
    if (CarteSonAutorisee) {
        for (i32 i = 0; i < 20; ++i) {
            stk_obj[i] = -2;
            stk_snd[i] = 0;
        }
        pt_pile_snd = 0;
        indice_snd_wiz = 0;
        indice_ray_wait = 0;
        indice_trz_wait = -2;
        for (i32 i = 0; i < 32; ++i) {
            voice_table[i].field_0 = -2;
        }
        raj_env_sound(18);
        return raj_env_stereo(options_jeu.is_stereo);
    } else {
        return false;
    }
}

//71E44
i16 last_snd(i32 obj_id) {
    i32 i = 0;
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
i32 get_pile_obj(i32 a1) {
    return 0; //stub
}

//71EF4
i32 get_voice_obj(i32 a1) {
    return 0; //stub
}

//71F30
i32 get_voice_snd(i32 a1) {
    return 0; //stub; unused in the PC version
}

//71F6C
i16 get_voice_obj_snd(i16 obj_id, i16 sound_id) {
    return 0; //stub
}

//71FC8
void erase_pile_snd(i16 obj_id) {
    //stub
}

//720B8
void nettoie_pile_snd(void) {
    //stub
}

//721C4
void erase_voice_table(i32 a1) {
    //stub
}

//72200
i32 snd_in_pile_snd(i16 a1) {
    return 0; //stub
}

//72254
i32 vol_r(i16 a1, i16 a2) {
    i32 result = (a1 * snd_sqrt_table[a2]) >> 8;
    return result;
}

//7226C
i32 vol_l(i16 a1, i16 a2) {
    i32 result = (a1 * snd_sqrt_table[127 - a2]) >> 8;
    return result;
}

//7228C
void PlaySnd(u16 sound_id, i16 obj_id) {
    PlaySnd_old(sound_id); //TODO: remove this after PlaySnd works properly by itself

    if (CarteSonAutorisee) {
        if ((ray.scale != 0 && obj_id == reduced_rayman_id) || obj_id == rayman_obj_id) {
            obj_id = -1;
        }
        if (obj_id == -1 && sound_id != 15) {
            indice_ray_wait = 0;
        }
        i16 sound_to_interrupt = last_snd(obj_id);
        if (sound_to_interrupt != sound_id && (sound_table[sound_to_interrupt] & 8) != 0) {
            erase_pile_snd(obj_id);
            i16 voice_obj_snd = get_voice_obj_snd(obj_id, sound_to_interrupt);
            if (voice_obj_snd >= 0) {
                voice_t* voice = voice_table + voice_obj_snd;
                sound_table_entry_t* sound = hard_sound_table + voice->sound_id;
                KeyOff(voice_obj_snd, bank_to_use[voice->sound_id], sound->prog, sound->tone, sound->note);
                voice->field_0 = -2;
                voice->sound_id = -1;
                voice_is_working[voice_obj_snd] = 0;
                // stub
            }
            // stub
        }
    }
}

//72960
void PlaySnd_old(u16 sound_id) {
    i16 voice_index = -1;
    if (CarteSonAutorisee) {
        u8 prog = hard_sound_table[sound_id].prog;
        if (prog != 255) {
            voice_index = KeyOn(bank_to_use[sound_id], prog,
                                hard_sound_table[sound_id].tone,
                                hard_sound_table[sound_id].note,
                                Volume_Snd * hard_sound_table[sound_id].volume >> 7, 64);
            if (voice_index != -1) {
                voice_table[voice_index].field_0 = -2;
                voice_table[voice_index].field_2 = 64;
                voice_table[voice_index].field_4 = 64;
                if (sound_table[sound_id] & 0x10) {
                    voice_is_working[voice_index] = 1;
                }
            }
        }
    }
}

//72A1C
void setvol(i16 obj_id) {
    //stub
}

//72E74
void setpan(i16 obj_id) {
    //stub
}

//72F38
void manage_snd(void) {
    //stub
}

//730DC
void mute_snd_bouclant(void) {
    //stub
}

//73138
void mute_snd(void) {
    //stub
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
void PlayTchatchVignette(i32 snd) {
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
        PlayTchatch(snd, Volume_Snd); //TODO
    }
}

//73204
void TestCdLoop(void) {
    if (cdTime != 0) {
        if (cdTime != -1) {
            //dos_get_time_of_day();
            cdTime = 10 * (cdTimeEnd = cdTimeStart) / 182;
            i32 duration = timeCd[22 * (num_world - 1) + num_level];
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
        play_cd_track(map_cd_tracks[22 * (num_world - 1) + num_level -1 ]);
        phaseCd = 0;
        cdTime = 1;
    }
}

//73384
void InitMusic(void) {
    MusicCdActive = 1;
    //stub
}

//733BC
void DoneMusic(void) {
    //stub
}
