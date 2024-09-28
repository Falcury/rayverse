// 0CD794
char save_names[4][3];
u8 medaillion_saved_data[24];
u8 collected_events_data[2592];
u8 bonus_completed_data[24];

// sub_742E0
void set_medaillion_saved_data() {
	for (i32 i = 0; i < 24; ++i) {
		medaillion_t* medaillion = t_world_info + i;
		u8 unlocked_bit = medaillion->state & 1;
		u8 status_flag_4_bit = (medaillion->state & 4) >> 2;
		u8 save_byte = (unlocked_bit) | (status_flag_4_bit << 1) | ((medaillion->nb_cages & 7) << 2);
		medaillion_saved_data[i] = save_byte;
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

		mem_read(save_names[which_save-1], raw, 4);
		mem_read(&nb_continue, raw, 1);
		mem_read(medaillion_saved_data, raw, 24);
		mem_read(&RayEvts, raw, 2);
		mem_read(&poing, raw, 20);
		mem_read(&status_bar, raw, 10);
		mem_read(&ray.hitp, raw, 1);
		mem_read(collected_events_data, raw, 2592);
		mem_read(bonus_completed_data, raw, 24);
		u16 map_location = dans_la_map_monde ? num_world_choice : world_index;
		// Note: Game bug: saved_map_location is only 1 byte, but we are writing/reading 2 bytes,
		mem_read(&map_location, raw, 2);
		mem_read(&finBossLevel, raw, 2);
		free(raw);

	}

}

// sub_74400
void SaveGameOnDisk(u8 which_save) {
	if (!(which_save >= 1 && which_save <= 3)) {
		return; // out of bounds
	}


	size_t capacity = 4*1024; // TODO: what is the exact uncompressed save file filesize?
	mem_t* raw = (mem_t*)calloc(1, sizeof(mem_t) + capacity);
	raw->capacity = capacity;

	//set_medaillion_saved_data();
	
	mem_write(save_names[which_save-1], raw, 4);
	mem_write(&nb_continue, raw, 1);
	mem_write(medaillion_saved_data, raw, 24);
	mem_write(&RayEvts, raw, 2);
	mem_write(&poing, raw, 20);
	mem_write(&status_bar, raw, 10);
	mem_write(&ray.hitp, raw, 1);
	mem_write(collected_events_data, raw, 2592);
	mem_write(bonus_completed_data, raw, 24);
	u16 map_location = dans_la_map_monde ? num_world_choice : world_index;
	// Note: Game bug: saved_map_location is only 1 byte, but we are writing/reading 2 bytes,
	mem_write(&map_location, raw, 2);
	mem_write(&finBossLevel, raw, 2);
	raw->cursor = 0;

	mem_t* compressed = compress_sav(raw);
	
	u8 checksum = 0;
	mem_t* encoded = sav_xor_stream(compressed, &checksum);
	
	
	char filename[512];
	snprintf(filename, 512, "RAYMAN%d_.SAV", which_save);
	// stub: writing out to a temporary file for encryption

	FILE* fp = fopen(filename, "wb");
	fwrite(encoded->data, encoded->len, 1, fp);
	fclose(fp);

	free(raw);
	free(compressed);
	free(encoded);
}


void reset_items_and_bosses() {
	memset(collected_events_data, 0, 2592);
	memset(bonus_completed_data, 0, 24);
	finBossLevel = 0;

	for (i32 i = 0; i < 24; ++i) {
		medaillion_saved_data[i] &= ~(7 << 2);
	}

}

