// 0CD794
u8 collected_events_data[2592];
u8 bonus_completed_data[24];

// sub_742E0
void set_medaillion_saved_data(void) {
	for (i32 i = 0; i < 24; ++i) {
		world_info_t* medaillion = t_world_info + i;
		u8 unlocked_bit = medaillion->state & 1;
		u8 status_flag_4_bit = (medaillion->state & 4) >> 2;
		u8 save_byte = (unlocked_bit) | (status_flag_4_bit << 1) | ((medaillion->nb_cages & 7) << 2);
		wi_save_zone[i] = save_byte;
	}
}

// http://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
u8 bit_reverse_u8(u8 b) {
	b = (u8)(((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
	return b;
}



i32 sav_read_byte(mem_t* mem, u8* checksum) {
	i32 byte = 0;
	i32 bytes_read = mem_read(&byte, mem, 1);
	if (bytes_read <= 0) byte = -1;
	if (byte != -1) {
		*checksum -= byte;
	}
	return byte;
}



mem_t* sav_xor_stream(mem_t* source, u8* checksum_result) {
	mem_t* dest = (mem_t*)calloc(1, sizeof(mem_t) + source->capacity);
	dest->capacity = source->capacity;
	u8 checksum;
	mem_read(&checksum, source, 1);
	checksum ^= 0x53;
	mem_write(&checksum, dest, 1);

	u32 decompressed_size;
	mem_read(&decompressed_size, source, 4);
	decompressed_size ^= 0x54555657;
	mem_write(&decompressed_size, dest, 4);

	u8 compr_incremental_xor = 0x57;
	while (source->cursor < (i32)source->len) {
		u8 byte = 0;
		mem_read(&byte, source, 1);

		byte ^= bit_reverse_u8(compr_incremental_xor) ^ 0xB9;
		++compr_incremental_xor; // N.B. intentional overflow happens at 0xFF

		checksum -= byte; // underflow
		mem_write(&byte, dest, 1);
	}
	dest->cursor = 0;
	*checksum_result = checksum;
	return dest;
}

mem_t* decompress_sav(mem_t* compressed) {
	u8 compr_big_window[256 * 8];
	for (i32 i = 0; i < 256; ++i) {
		for (i32 j = 0; j < 8; ++j) {
			compr_big_window[i * 8 + j] = (u8)i;
		}
	}

	u8 compr_window[8] = {0};
	u8 window_update_bit_array = 0;
	u8 bytes_in_window = 0;

	u8 checksum = 0;
	u32 decompressed_size = 0;
	mem_read(&checksum, compressed, 1);
	mem_read(&decompressed_size, compressed, 4);

	ASSERT(decompressed_size > 0 && decompressed_size < 16*1024);
	mem_t* decompressed = (mem_t*)calloc(1, sizeof(mem_t) + decompressed_size);
	decompressed->capacity = decompressed_size;

	bool finished = false;
	while (!finished) {
		u32 compressed_bytes_left = compressed->len - compressed->cursor;
		u32 decompressed_bytes_left = decompressed->capacity - decompressed->cursor;
		if (bytes_in_window > 0) {
			if (compressed_bytes_left == 1) {
				u8 final_byte = 0;
				mem_read(&final_byte, compressed, 1);
				bytes_in_window = MIN(final_byte, bytes_in_window);
			}
			u8 bytes_to_write = MIN(bytes_in_window, decompressed_bytes_left);
			mem_write(compr_window, decompressed, bytes_to_write);
		}

		if (compressed->cursor >= (i32)compressed->len) {
			finished = true;
		} else {
			u8 big_window_index = 0;
			mem_read(&big_window_index, compressed, 1);
			mem_read(&window_update_bit_array, compressed, 1);

			u8* big_window_pos = compr_big_window + (big_window_index * 8);
			for (i32 i = 0; i < 8; ++i) {
				if (window_update_bit_array & 1) {
					mem_read(&big_window_pos[i], compressed, 1);
				}
				compr_window[i] = big_window_pos[i];
				window_update_bit_array >>= 1;
			}
			bytes_in_window = 8;

		}
	}

	decompressed->cursor = 0;
	return decompressed;
}

u8 sav_write_byte(mem_t* mem, u8 byte) {
	mem_write(&byte, mem, 1);
	return byte;
}

mem_t* compress_sav(mem_t* raw) {
	u8 compr_big_window[256 * 8];
	for (i32 i = 0; i < 256; ++i) {
		for (i32 j = 0; j < 8; ++j) {
			compr_big_window[i * 8 + j] = (u8)i;
		}
	}
	u8 compr_window[8] = {0};
	u8 checksum = 0;

	size_t capacity = 2 * raw->len; // conservative worst case assumption
	mem_t* compressed = (mem_t*)calloc(1, sizeof(mem_t) + capacity);
	compressed->capacity = capacity;

	compressed->cursor = 1; // checksum (1 byte) is written at the end
    raw->cursor = 0;
	mem_write(&raw->len, compressed, 4); // decompressed size
	bool finished = false;
	while (!finished) {
		i32 bytes_to_compress = 0;
		i32 i;
		for (i = 0; i < 8; ++i) {
			if (raw->cursor >= (i32)raw->len) break;
			++bytes_to_compress;
			u8 byte = (u8)mem_read_byte(raw);
			compr_window[i] = byte;
		}
		if (bytes_to_compress > 0) {
			i32 max_occurrences_in_big_window = -1;
			i32 best_big_window_index = 0;
			for (i = 0; i < 256; ++i) {
				i32 occurrences = 0;
				for (i32 j = 0; j < bytes_to_compress; ++j) {
					if (compr_big_window[i * 8 + j] == compr_window[j]) {
						++occurrences;
					}
				}
				if (occurrences > max_occurrences_in_big_window) {
					max_occurrences_in_big_window = occurrences;
					best_big_window_index = i;
					if (occurrences == bytes_to_compress) {
						break; // already ideal, no need to look further
					}
				}
			}
			checksum += sav_write_byte(compressed, best_big_window_index);
			u8 window_update_bit_array = 0;
			for (i = 0; i < bytes_to_compress; ++i) {
				bool match = (compr_big_window[best_big_window_index * 8 + i] == compr_window[i]);
				if (!match) window_update_bit_array |= (1 << i);
			}
			checksum += sav_write_byte(compressed, window_update_bit_array);
			for (i =0; i < 8; ++i) {
				if (window_update_bit_array & 1) {
					checksum += sav_write_byte(compressed, compr_window[i]);
				}
				window_update_bit_array >>= 1;
			}
			if (bytes_to_compress >= 8) {
				for (i = 0; i < 8; ++i) {
					compr_big_window[best_big_window_index * 8 + i] = compr_window[i];
				}
			} else {
				checksum += sav_write_byte(compressed, bytes_to_compress); // number of bytes in the last compression window
				finished = true;
			}
		} else {
			finished = true;
		}
	}
	compressed->cursor = 0;
	mem_write(&checksum, compressed, 1);
	compressed->cursor = 0;
	return compressed;
}

#define NEED_SAV_ENCODE_DECODE_TEST
#ifdef NEED_SAV_ENCODE_DECODE_TEST
bool test_sav_encode_decode(mem_t* encoded) {
	bool success = true;
	u8 checksum = 0;
	mem_t* compressed = sav_xor_stream(encoded, &checksum);
	mem_t* raw = decompress_sav(compressed);
	mem_t* recompressed = compress_sav(raw);
	u32 i;
	for (i = 0; i < recompressed->len; ++i) {
		u8 original_byte = compressed->data[i];
		u8 new_byte = recompressed->data[i];
		if (original_byte != new_byte) {
			success = false;
		}
	}

	mem_t* reencoded = sav_xor_stream(recompressed, &checksum);
	for (i = 0; i < recompressed->len; ++i) {
		u8 original_byte = encoded->data[i];
		u8 new_byte = reencoded->data[i];
		if (original_byte != new_byte) {
			success = false;
		}
	}

	encoded->cursor = 0;
	free(compressed);
	free(recompressed);
	free(reencoded);
	return success;
}
#endif //NEED_SAV_ENCODE_DECODE_TEST

#if 0
void sav_set_ALL_WORLD(u8 which_save) {
    char save_filename[32];
    snprintf(save_filename, 64, "RAYMAN%d.SAV", which_save);
    mem_t* encoded = read_entire_file(save_filename, false);
    bool has_error = false;
    if (encoded) {
        u8 checksum = 0;
        mem_t* compressed = sav_xor_stream(encoded, &checksum);
        free(encoded);
        mem_t* raw = decompress_sav(compressed);
        if (raw) {

            raw->data[2679] = 1; // Sets ALL_WORLD to 1

            mem_t* recompressed = compress_sav(raw);
            if (recompressed) {
                mem_t* reencoded = sav_xor_stream(recompressed, &checksum);
                if (reencoded) {

                    snprintf(save_filename, 64, "RAYMAN%d_ALL_WORLD.SAV", which_save);
                    FILE* fp = fopen(save_filename, "wb");
                    fwrite(reencoded->data, reencoded->len, 1, fp);
                    fclose(fp);

                    free(reencoded);
                }
                free(recompressed);
            }

            free(raw);
        } else {
            has_error = true;
        }
        free(compressed);
    } else {
        has_error = true;
    }
}
#endif

// sub_746D0
// Partly based on https://github.com/RayCarrot/RayCarrot.Rayman/blob/3bf249b64da4254c03fa49a15bb9d5691ddfce66/RayCarrot.Rayman/Encoder/Rayman12PCSaveDataEncoder.cs
void load_sav(u8 which_save) {
	char filename[512];
	snprintf(filename, 512, "RAYMAN%d.SAV", which_save);
	mem_t* encoded = read_entire_file(filename, false);
	if (encoded) {
#ifdef NEED_SAV_ENCODE_DECODE_TEST
		test_sav_encode_decode(encoded);
#endif
		u8 checksum = 0;
		mem_t* compressed = sav_xor_stream(encoded, &checksum);
		free(encoded);
		mem_t* raw = decompress_sav(compressed);
		free(compressed);

		mem_read(save_ray[which_save-1], raw, 4);
		mem_read(&nb_continue, raw, 1);
		mem_read(wi_save_zone, raw, 24);
		mem_read(&RayEvts, raw, 2);
		mem_read(&poing, raw, 20);
		mem_read(&status_bar, raw, 10);
		mem_read(&ray.hit_points, raw, 1);
		mem_read(collected_events_data, raw, 2592);
		mem_read(bonus_completed_data, raw, 24);
		u16 map_location = dans_la_map_monde ? num_world_choice : world_index;
		// Note: Game bug: saved_map_location is only 1 byte, but we are writing/reading 2 bytes,
		mem_read(&map_location, raw, 2);
		mem_read(&finBosslevel, raw, 2);
		free(raw);

	}

}




void reset_items_and_bosses(void) {
	memset(collected_events_data, 0, 2592);
	memset(bonus_completed_data, 0, 24);
    memset(&finBosslevel, 0, sizeof(finBosslevel));

	for (i32 i = 0; i < 24; ++i) {
		wi_save_zone[i] &= ~(7 << 2);
	}

}

//73B80
void initGameSave(void) {
    save1.is_just_saved = 0;
}

//73B80
void doneGameSave(void) {
	save1.is_just_saved = 0;
}

//73B8C
void saveGameState(obj_t* save_obj, save_state_t* save_state) {
	save_state->status_bar_tings = status_bar.num_wiz;
	save_state->x_map = xmap;
	save_state->y_map = ymap;
	save_state->ray_x_pos = (i16)ray.x;
	save_state->ray_y_pos = (i16)ray.y;
	save_state->ray_screen_x = ray.screen_x;
	save_state->ray_screen_y = ray.screen_y;
	save_state->ray_flip_x = ray.flags.flip_x;
	save_state->rayevts_reverse = RayEvts.reverse;
	save_state->rayevts_super_helico = RayEvts.super_helico;
	save_state->rayevts_poing = RayEvts.poing;
	if (save_obj) {
		save_state->save_obj_id = save_obj->id;
		save_state->save_obj_x_pos = (i16)save_obj->x;
		save_state->save_obj_y_pos = (i16)save_obj->y;
		save_state->save_obj_detect_zone_flag = save_obj->detect_zone_flag;
		save_state->save_obj_flag_1 = save_obj->flags.flag_1;
	} else {
		save_state->save_obj_id = -1;
	}
	memcpy(save_state->link_init, link_init, level.nb_objects * sizeof(u16));
	for (i32 i = 0; i < 5; ++i) {
		save_state->ray_coll_btype[i] = ray.coll_btype[i];
	}
	save_state->ray_anim_index = ray.anim_index;
	save_state->ray_anim_frame = ray.anim_frame;
	save_state->ray_main_etat = ray.main_etat;
	save_state->ray_sub_etat = ray.sub_etat;
	for (i32 i = 0; i < 8; ++i) {
		save_state->nb_floc[i] = nb_floc[i];
	}
	save_state->vent_x = VENT_X;
	save_state->vent_y = VENT_Y;
	save_state->poing_sub_etat = poing.sub_etat;
	for (i32 obj_id = 0; obj_id < level.nb_objects; ++obj_id) {
		obj_t* obj = level.objects + obj_id;
		if (obj->type == TYPE_141_NEIGE || obj->type == TYPE_164_GENERATING_DOOR || obj->type == TYPE_179_HERSE_BAS_NEXT || obj->type == TYPE_242_HERSE_HAUT_NEXT) {
			u32 bit = 1 << (obj_id & 0x1F);
			u32 index = obj_id >> 5;
			if (obj->flags.alive) {
				save_state->triggered_objects[index] |= bit;
			} else {
				save_state->triggered_objects[index] &= ~bit;
			}
		}
	}
	save_state->is_just_saved = 1; // NOTE: not yet sure what this does exactly; variable name is provisional...
	save_state->dead_time = dead_time;
	save_state->current_pal_id = current_pal_id;
}

//73DC4
void restoreGameState(save_state_t* save_state) {
    if (save_state->is_just_saved) {
        if (RayEvts.tiny) {
            ray = rms;
        }
        status_bar.num_wiz = save_state->status_bar_tings;
        xmap = save_state->x_map;
        ymap = save_state->y_map;
        ray.x = save_state->ray_x_pos;
        ray.y = save_state->ray_y_pos;
        ray.screen_x = save_state->ray_screen_x;
        ray.screen_y = save_state->ray_screen_y;
        ray.flags.flip_x = save_state->ray_flip_x;
        RayEvts.super_helico = save_state->rayevts_super_helico;
        if (num_world == world_6_cake && num_level == 3) {
            RayEvts.poing = save_state->rayevts_poing;
            RayEvts.reverse = save_state->rayevts_reverse;
        }
        if (save_state->save_obj_id != -1) {
            obj_t* save_obj = level.objects + save_state->save_obj_id;
            save_obj->x = save_state->save_obj_x_pos;
            save_obj->y = save_state->save_obj_y_pos;
            save_obj->detect_zone_flag = save_state->save_obj_detect_zone_flag;
            save_obj->flags.flag_1 = save_state->save_obj_flag_1;
        }
        if (save_state == &save1) {
            for (i32 i = 0; i < level.nb_objects; ++i) {
                link_init[i] = save_state->link_init[i];
                level.objects[i].link_has_gendoor = (link_init[i] != i);
            }
        }
        for (i32 i = 0; i < 5; ++i) {
            ray.coll_btype[i] = save_state->ray_coll_btype[i];
        }
        ray.anim_index = save_state->ray_anim_index;
        ray.anim_frame = save_state->ray_anim_frame;
        ray.main_etat = save_state->ray_main_etat;
        ray.sub_etat = save_state->ray_sub_etat;
        if (save_state == &save1) {
            switch(save_state->poing_sub_etat) {
                case 1: case 3: case 8: case 10: {
                    save_state->poing_sub_etat = 1;
                } break;
                case 5: case 12: {
                    save_state->poing_sub_etat = 3;
                } break;
            }
        }
        poing_obj->init_sub_etat = save_state->poing_sub_etat;
        poing.sub_etat = save_state->poing_sub_etat;
        dead_time = save_state->dead_time;
        decalage_en_cours = 0;
        ray_wind_force = 0;
        for (i32 i = 0; i < 8; ++i) {
            nb_floc[i] = save_state->nb_floc[i];
        }
        VENT_X = save_state->vent_x;
        ray.speed_x = 0;
        ray.speed_y = 0;
        ray.iframes_timer = -1;
        ray.is_active = 1;
        VENT_Y = save_state->vent_y;
        ray.flags.alive = true;
        for (i32 obj_id = 0; obj_id < level.nb_objects; ++obj_id) {
            obj_t* obj = level.objects + obj_id;
            u16 type = obj->type;
            if (type == TYPE_3_LIDOLPINK
                || type == TYPE_13_BALLE1
                || type == TYPE_15_BALLE2
                || type == TYPE_59_CAGE2
                || type == TYPE_36_STONEBOMB
                || type == TYPE_66_DARD
                || type == TYPE_62_DROP
                || type == TYPE_99_RAY_POS
            ) {
                obj->flags.alive = false;
            } else {
                if (type != TYPE_141_NEIGE && type != TYPE_164_GENERATING_DOOR) {
                    bool need_check_triggered;
                    if (type == TYPE_179_HERSE_BAS_NEXT || type == TYPE_242_HERSE_HAUT_NEXT) {
                        obj->field_1C = 1;
                        need_check_triggered = true;
                    } else {
                        need_check_triggered = false;
                    }
                    if (!need_check_triggered) {
                        continue;
                    }
                }
                if (((1 << (obj_id & 0x1F)) & (save_state->triggered_objects[obj_id >> 5])) == 0) {
                    obj->flags.alive = false;
                } else {
                    obj->flags.alive = true;
                }
            }
        }
        gerbe = 0;
        current_pal_id = save_state->current_pal_id + 1; // ?
        //actualize_palette(save_state->current_pal_id); // stub


	}
}

//741C8
i32 get_offset_in_safe_zone(i16 obj_id) {
    i32 offset = 0;
    if (num_world > 1) {
        for (i32 w = 1; w < num_world; ++w) {
            offset += 32 * nb_levels_in_world[w];
        }
    }
    if (num_level > 1) {
        for (i32 l = 1; l < num_level; ++l) {
            offset += 32;
        }
    }
    offset += ashr16(obj_id, 3);
    return offset;
}

//74250
void reset_save_zone_level(void) {
    //stub
}

//74270
void take_bonus(i16 a1) {
    //stub
}

//742A8
u8 bonus_taken(i16 obj_id) {
    u8 result = save_zone[get_offset_in_safe_zone(obj_id)] & (0x80 >> (obj_id & 7));
    return result;
}

//742E0
void storeWorldInfoAcces(void) {
    //stub
}

//7435C
void retrieveWorldInfoAccess(void) {
    for (i32 i = 0; i < COUNT(t_world_info); ++i) {
        world_info_t* world_info = t_world_info + i;
        world_info->state = (world_info->state & ~1) | (wi_save_zone[i] & 1); // unlocked bit
        world_info->state = (world_info->state & ~4) | (((u8)(wi_save_zone[i] << 6) >> 7) * 4); // unknown flag
        world_info->nb_cages =  ((u8)(wi_save_zone[i] << 3) >> 5); // cages
    }
}

//743C8
void file_size(void) {
    //stub
}

//74400
void SaveGameOnDisk(u8 which_save) {
    if (!(which_save >= 1 && which_save <= 3)) {
        return; // out of bounds
    }


    size_t capacity = 4*1024; // TODO: what is the exact uncompressed save file filesize?
    mem_t* raw = (mem_t*)calloc(1, sizeof(mem_t) + capacity);
    raw->capacity = capacity;

    //set_medaillion_saved_data();

    mem_write(save_ray[which_save-1], raw, 4);
    mem_write(&nb_continue, raw, 1);
    mem_write(wi_save_zone, raw, 24);
    mem_write(&RayEvts, raw, 2);
    mem_write(&poing, raw, 20);
    mem_write(&status_bar, raw, 10);
    mem_write(&ray.hit_points, raw, 1);
    mem_write(collected_events_data, raw, 2592);
    mem_write(bonus_completed_data, raw, 24);
    u16 map_location = dans_la_map_monde ? num_world_choice : world_index;
    // Note: Game bug: saved_map_location is only 1 byte, but we are writing/reading 2 bytes,
    mem_write(&map_location, raw, 2);
    mem_write(&finBosslevel, raw, 2);
    raw->cursor = 0;

    mem_t* compressed = compress_sav(raw);

    u8 checksum = 0;
    mem_t* encoded = sav_xor_stream(compressed, &checksum);


    char filename[512];
    snprintf(filename, 512, "RAYMAN%d.SAV", which_save);
    // stub: writing out to a temporary file for encryption

    FILE* fp = fopen(filename, "wb");
    fwrite(encoded->data, encoded->len, 1, fp);
    fclose(fp);

    free(raw);
    free(compressed);
    free(encoded);
}

//746D0
bool LoadGameOnDisk(u8 which_save) {
    char save_filename[32];
    snprintf(save_filename, sizeof(save_filename), "RAYMAN%d.SAV", which_save);
    mem_t* encoded = read_entire_file(save_filename, false);
    bool has_error = false;
    if (encoded) {
        u8 checksum = 0;
        mem_t* compressed = sav_xor_stream(encoded, &checksum);
        free(encoded);
        mem_t* raw = decompress_sav(compressed);
        if (raw) {
            mem_read(save_ray[which_save-1], raw, 4);
            mem_read(&nb_continue, raw, 1);
            mem_read(wi_save_zone, raw, 24);
            mem_read(&RayEvts, raw, 2);
            mem_read(&poing, raw, 20);
            mem_read(&status_bar, raw, 10);
            mem_read(&ray.hit_points, raw, 1);
            mem_read(save_zone, raw, 2592);
            mem_read(bonus_perfect, raw, 24);
            // NOTE: the original code reads 2 bytes into world_index, even though it's a 1-byte value.
            // Were world_index and ALL_WORLD a 2-byte struct together? (Doesn't seem to be the case in the Android code though.)
            mem_read(&world_index, raw, 1);
            mem_read(&ALL_WORLD, raw, 1);
            mem_read(&finBosslevel, raw, 2);

            retrieveWorldInfoAccess();
            xwldmapsave = 0;
            ywldmapsave = 0;

            free(raw);
        } else {
            has_error = true;
        }
        free(compressed);
    } else {
        has_error = true;
    }

    if (JeuCracker) {
        save_ray[which_save-1][0] = '\0';
    }

    return has_error;
}


//749C8
bool LoadInfoGame(u8 which_save) {
    char save_filename[32];
    snprintf(save_filename, sizeof(save_filename), "RAYMAN%d.SAV", which_save);
    mem_t* encoded = read_entire_file(save_filename, false);
    bool has_error = false;
    if (encoded) {
        u8 checksum = 0;
        mem_t* compressed = sav_xor_stream(encoded, &checksum);
        free(encoded);
        mem_t* raw = decompress_sav(compressed);
        if (raw) {
            mem_read(save_ray[which_save-1], raw, 4);
            mem_read(&nb_continue, raw, 1);
            mem_read(wi_save_zone, raw, 24);
            mem_read(&RayEvts, raw, 2);
            mem_read(&poing, raw, 20);
            mem_read(&status_bar, raw, 10);

            loadinforay_t* info = LoadInfoRay + (which_save - 1);
            info->continues = nb_continue;
            i32 cages = 0;
            for (i32 i = 0; i < 24; ++i) {
                u8 w = wi_save_zone[i];
                cages += (w >> 2) & 7;
            }
            info->cages = cages;
            info->lives = status_bar.lives;
            info->tings = status_bar.num_wiz;
            free(raw);
        } else {
            has_error = true;
        }
        free(compressed);
    } else {
        has_error = true;
    }

    if (JeuCracker) {
        save_ray[which_save-1][0] = '\0';
    }

    return has_error;
}

//74CC8
bool SaveOptionsOnDisk(void) {
    return 0; //stub
}

//75268
bool LoadOptionsOnDisk(void) {
    mem_t* mem = read_entire_file("RAYMAN.CFG", false);
    if (mem) {
        if (mem->len != 0x84) {
            return false;
        }
        mem_read(&language, mem, 1);
        mem_read(&Port, mem, 4);
        mem_read(&Irq, mem, 4);
        mem_read(&Dma, mem, 4);
        mem_read(&Param, mem, 4);
        mem_read(&DeviceID, mem, 4);
        mem_read(&NumCard, mem, 1);
        mem_read(&options_jeu_KeyJump, mem, 2);
        mem_read(&options_jeu_KeyWeapon, mem, 2);
        mem_read(&options_jeu_KeyUnknown_default_02, mem, 2);
        mem_read(&options_jeu_KeyAction, mem, 2);
        mem_read(&options_jeu_music_enabled, mem, 2); // 13 = default?
        mem_read(&options_jeu_sound_volume, mem, 2); // 13 = default?
        mem_read(&options_jeu_is_stereo, mem, 2);
        mem_read(&Mode_Pad, mem, 1);
        mem_read(&Port_Pad, mem, 1);
        mem_read(&xpadmax, mem, 2);
        mem_read(&xpadmin, mem, 2);
        mem_read(&ypadmax, mem, 2);
        mem_read(&ypadmin, mem, 2);
        mem_read(&xpadcentre, mem, 2);
        mem_read(&ypadcentre, mem, 2);
        for (i32 i = 0; i < 4; ++i) {
            notbut[i] = 0;
            mem_read(notbut + i, mem, 1);
        }
        for (i32 i = 0; i < 7; ++i) {
            mem_read(tab_key[i], mem, 1);
        }
        mem_read(&GameModeVideo, mem, 1);
        mem_read(&P486, mem, 1);
        mem_read(&SizeScreen, mem, 1);
        if (Frequence != 70) {
            mem_read(&Frequence, mem, 1);
        } else {
            mem_read(&fixon, mem, 1); // in effect, skips this byte
        }
        mem_read(&fixon, mem, 1);
        mem_read(&BackgroundOptionOn, mem, 1);
        mem_read(&ScrollDiffOn, mem, 1);
        mem_read(&tRefRam2VramNormalFix, mem, 16);
        mem_read(&tRefRam2VramNormal, mem, 16);
        mem_read(&tRefTransFondNormal, mem, 16);
        mem_read(&tRefSpriteNormal, mem, 4);
        mem_read(&tRefRam2VramX, mem, 4);
        mem_read(&tRefVram2VramX, mem, 4);
        mem_read(&tRefSpriteX, mem, 4);

        free(mem);
        return true;
    } else {
        return false;
    }
}
