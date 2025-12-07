
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
            voice->position = 0.0f;
            voice->is_playing = true;
            return (i16)i;
        }
    }
    return -1;
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

    // We need to resample the sound's sample rate (which may also vary depending on pitch shifting) to the output rate.
    const float output_sample_rate = 44100.0f;
    for (i32 i = 0; i < COUNT(digi_voices); ++i) {
        snd_t* voice = digi_voices + i;
        if (!voice->is_playing) {
            continue;
        }

        // Calculate the step increment for the source sample position.
        // This is the ratio of the sound's sample rate to the output sample rate.
        float step = (float)voice->sample_rate / output_sample_rate;

        // Get a pointer to the start of the output buffer for this frame
        i16* dest = output_buffer->samples;

        // Loop through the number of samples we need to generate for this frame
        for (i32 j = 0; j < output_buffer->sample_count; ++j) {

            // 1. Find the current position and check boundaries
            i32 source_index = (i32)voice->position;

            // Stop if we've read past the end of the source data.
            // We need source_index and source_index + 1 for interpolation, so we check against size - 1.
            if (source_index >= voice->sample_count - 1) {
                voice->is_playing = false;
                break; // Stop processing this voice for this frame
            }

            // 2. Get samples for interpolation
            // Convert 8-bit unsigned samples to a signed range (-128 to 127)
            i32 s1 = (i32)voice->data[source_index] - 128;
            i32 s2 = (i32)voice->data[source_index + 1] - 128;

            // 3. Perform Linear Interpolation
            // Find the fractional part of our position, which is our interpolation factor 't'
            float t = voice->position - (float)source_index;

            // Interpolate between the two samples
            float interpolated_sample = (1.0f - t) * (float)s1 + t * (float)s2;

            // 4. Scale to 16-bit and apply volume
            // Scale from the -128 to 127 range to the -32768 to 32767 range
            // Also, apply the volume for this voice
            i32 dest_sample = (i32)(interpolated_sample * 256.0f * voice->volume);

            // 5. Mix into the output buffer (mono to stereo)
            // Add the new sample to the existing data in the buffer
            i32 mixed_l = dest[0] + dest_sample;
            i32 mixed_r = dest[1] + dest_sample;

            // Clamp the values to prevent 16-bit integer overflow (clipping)
            dest[0] = (i16)MIN(32767, MAX(-32768, mixed_l));
            dest[1] = (i16)MIN(32767, MAX(-32768, mixed_r));
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
void SetVolumeSound(i16 volume) {
    Volume_Snd = volume;
}

//71C10
void stop_all_snd(void) {
    if (CarteSonAutorisee) {
        for (i32 i = 0; i < COUNT(voice_table); ++i) {
            KeyOff(i, 0, 0, 0, 0);
        }
    }
}

//71C40
void stop_ray_snd(void) {
    //stub; unused in the PC version
}

//71CA8
u8 get_pan_snd(obj_t* obj) {
    i32 x = obj->offset_bx + obj->screen_x;
    i32 result;
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
i32 get_vol_snd(obj_t* obj) {
    i16 x, y, w, h;
    GET_ANIM_POS(obj, &x, &y, &w, &h);
    i16 vol = (i16)(127 - ((Abs(y + (h >> 1) - (ray.y + ray.offset_by) + 40) + Abs(x + (w >> 1) - (ray.x + ray.offset_bx))) >> 2));
    if (vol < 0) {
        return 0;
    } else {
        return vol;
    }
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
            voice_table[i].obj = -2;
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
i32 get_pile_obj(i16 obj_id) {
    if (pt_pile_snd == 0) {
        return -1;
    }
    pile_snd_t* cur_pile_snd = pile_snd;
    i32 index = 0;
    if (pile_snd[0].obj != obj_id && pt_pile_snd > 0) {
        i16 next;
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
i32 get_voice_obj(i32 obj_id) {
    voice_t* cur_voice = voice_table;
    i32 index = 0;

    if (voice_table[0].obj != obj_id) {
        do {
            ++index;
        } while (index < COUNT(voice_table) && cur_voice->obj != obj_id);
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }

    return index;
}

//71F30 - unused in the PC version
i32 get_voice_snd(i32 snd) {
    voice_t* cur_voice = voice_table;
    i32 index = 0;

    if (voice_table[0].snd != snd) {
        do {
            ++index;
        } while (index < COUNT(voice_table) && cur_voice->snd != snd);
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }

    return index;

}

//71F6C
i32 get_voice_obj_snd(i16 obj_id, i16 snd) {
    voice_t* voice = voice_table;
    i32 index = 0;
    // NOTE: there is a conditional here, but the branches are identical?
    // if (snd = 48) { ... } else { ... }
    while ((voice->snd != snd || voice->obj != obj_id) && index < COUNT(voice_table)) {
        ++index;
        ++voice;
    }
    if (index == COUNT(voice_table)) {
        return -1;
    }
    return index;
}

//71FC8
void erase_pile_snd(i16 obj_id) {
    i16 pile_obj_id = get_pile_obj(obj_id);
    if (pile_obj_id != -1) {
        for (i32 i = pile_obj_id; i < pt_pile_snd; ++i) {
            pile_snd[i] = pile_snd[i+1];
        }
        if (pt_pile_snd > 0) {
            --pt_pile_snd;
        }
    }
}

//720B8
void nettoie_pile_snd(void) {
    i32 index = 0;
    while (index < pt_pile_snd) {
        pile_snd_t* pile = pile_snd + index;
        if (pile->end_time < map_time && pile->end_time != 0) {
            // erase entry
            for (i32 i = index; i < pt_pile_snd; ++i) {
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
void erase_voice_table(i32 obj_id) {
    voice_t* cur_voice = voice_table;
    i32 result = 0;

    if (voice_table[0].obj != obj_id) {
        do {
            ++result;
        } while (result < COUNT(voice_table) && cur_voice->obj != obj_id);
    }
    if (result != COUNT(voice_table)) {
        cur_voice->obj = -2;
    }
}

//72200
u8 snd_in_pile_snd(i16 snd) {
    u8 result = false;
    i32 index = 0;
    if (pt_pile_snd != 0) {
        pile_snd_t* pile = pile_snd;
        if (pile_snd[0].snd != snd) {
            do {
                ++index;
            } while (index < pt_pile_snd && pile->snd != snd);
        }
        return pt_pile_snd != index;
    }
    return false;
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
void PlaySnd(i16 snd, i16 obj_id) {
    if (CarteSonAutorisee) {
        if ((ray.scale != 0 && obj_id == reduced_rayman_id) || obj_id == rayman_obj_id) {
            obj_id = -1;
        }
        if (obj_id == -1 && snd != 15) {
            indice_ray_wait = 0;
        }
        i16 sound_to_interrupt = last_snd(obj_id);
//        printf("playing sound %d: want to interrupt sound %d for obj %d\n", sound_id, sound_to_interrupt, obj_id);
        if (sound_to_interrupt != snd && (sound_table[sound_to_interrupt] & 8) != 0) {
            erase_pile_snd(obj_id);
            i16 voice_id = get_voice_obj_snd(obj_id, sound_to_interrupt);
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
        if (!dans_la_map_monde && snd != 0 && (snd != sound_to_interrupt || (sound_table[sound_to_interrupt] & 4) != 0)) {
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
        }

        i16 prog = hard_sound_table[snd].prog;
        u8 tone = hard_sound_table[snd].tone;
        u8 note;

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
                } else {
                    prog = hard_sound_table[170].prog;
                    tone = hard_sound_table[170].tone;
                    note = hard_sound_table[170].note;
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
            i32 voll = vol_l(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd); // NOTE: has no effect?
            i32 volr = vol_r(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd); // NOTE: has no effect?

            if (frame_snd[snd] == 0) {
                // NOTE: the PS1 version implements left/right directional sound here, using SsUtKeyOn
                i32 voice_id = KeyOn(bank_to_use[snd], prog, tone, note, Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd);
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
                // NOTE: I don't yet know what causes the sound to be emitted in this code path.
                // So, I added this for now (delete again when it's figured out).
                KeyOn(bank_to_use[snd], prog, tone, note, Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd);
//                printf("Debug: snd %d, prog %d, tone %d, note %d, vol %d, pan %d\n", snd, prog, tone, note, Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd);

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
            if ((snd == 203 || snd == 237 || snd == 209 || snd == 225 || snd == 236) && (dead_time == 64 || dead_time == 128)) {
                start_cd_bbdead();
            }
            for (i32 i = 0; i < 20; ++i) {
                if (stk_obj[i] == obj_id || stk_obj[i] == -2) {
                    stk_obj[i] = obj_id;
                    stk_snd[i] = snd;
                    return;
                }
            }
            if (snd != -1) {
                i32 index = 19;
                while(level.objects[stk_obj[index]].is_active) {
                    --index;
                    if (index == -1) {
                        index = 0;
                        break;
                    }
                }
                stk_obj[index] = obj_id;
                stk_snd[index] = snd;
            }
        }

    }
}

//72960
void PlaySnd_old(i16 sound_id) {
    i16 voice_index = -1;
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
                if (sound_table[sound_id] & 0x10) {
                    voice_is_working[voice_index] = 1;
                }
            }
        }
    }
}

//72A1C
void setvol(i16 obj_id) {
    print_once("Not implemented: setvol"); //stub
}

//72E74
void setpan(i16 obj_id) {
    print_once("Not implemented: setpan"); //stub
}

//72F38
void manage_snd(void) {
    print_once("Not implemented: manage_snd"); //stub
}

//730DC
void mute_snd_bouclant(void) {
    print_once("Not implemented: mute_snd_bouclant"); //stub
}

//73138
void mute_snd(void) {
    for (i32 i = 0; i < COUNT(voice_table); ++i) {
        KeyVol(i, 0, 0);
    }
    print_once("Not implemented: mute_snd"); //stub
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
    print_once("Not implemented: InitMusic"); //stub
}

//733BC
void DoneMusic(void) {
    print_once("Not implemented: DoneMusic"); //stub
}
