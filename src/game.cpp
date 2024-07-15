
static inline eta_t* get_eta(event_t* event) {
	return &event->ETA[event->etat][0][event->subetat];
}

#define facing_right(event) ((event)->flags & event_flags_8_dir_right)

static inline void set_facing_right(event_t* event) {
	event->flags |= event_flags_8_dir_right;
}

static inline void set_facing_left(event_t* event) {
	event->flags &= ~event_flags_8_dir_right;
}

// sub_1a3f0
void display_in_game_text(const char* text, int unknown1, int x, int y, int unknown2) {
	// stub
}

struct wld_header_t {
	u16 bg_width;
	u16 bg_height;
	u8 unk_data_table_count;
	u8 unknown1;
	u8 unknown2;


};

void load_level(const char* filename) {
	mem_t* mem = read_entire_file(filename);
	if (mem) {
		//stream_t stream = mem_create_stream(mem);

		// Header
		u32 event_block_offset;
		u32 texture_block_offset;	
		mem_read(&event_block_offset, mem, 4);
		mem_read(&texture_block_offset, mem, 4);
		mem_read(&level_width_tiles, mem, 2);
		mem_read(&level_height_tiles, mem, 2);
		mem_read(color_palettes, mem, sizeof(color_palettes));
		mem_read(&active_palette, mem, 1);

		// Map
		level_tile_count = level_width_tiles * level_height_tiles;
		maptile_t* tiles = (maptile_t*) malloc(level_tile_count * sizeof(maptile_t));
		mem_read(tiles, mem, level_tile_count * sizeof(maptile_t));

		u8 background_image_static;
		u8 background_image_scrolling;
		u32 background_DES;
		mem_read(&background_image_static, mem, 1);
		mem_read(&background_image_scrolling, mem, 1);
		mem_read(&background_DES, mem, 4);

		// X Mode Textures
		u32 x_texture_count;
		u32 x_unk_table_size;
		u8 x_texture_checksum;
		u32 x_texture_index_table[1200];
		mem_read(&x_texture_count, mem, 4);
		mem_read(&x_unk_table_size, mem, 4);
		x_texture_t* x_textures = (x_texture_t*) malloc(x_texture_count * sizeof(x_texture_t));
		mem_read(x_textures, mem, x_texture_count * sizeof(x_texture_t));
		mem_read(&x_texture_checksum, mem, 1);
		mem_read(x_texture_index_table, mem, sizeof(x_texture_index_table));

		u8* x_unk_table = (u8*) malloc(x_unk_table_size);
		u8 x_unk_table_checksum;
		u32 x_unk_offsets[1200];
		mem_read(x_unk_table, mem, x_unk_table_size);
		mem_read(&x_unk_table_checksum, mem, 1);
		mem_read(x_unk_offsets, mem, sizeof(x_unk_offsets));

		// Normal Mode Textures
		u32 texture_offsets[1200];
		u32 texture_count;
		u32 opaque_texture_count;
		u32 texture_data_size;
		u8 texture_data_checksum;
		mem_read(texture_offsets, mem, sizeof(texture_offsets));
		mem_read(&texture_count, mem, 4);
		mem_read(&opaque_texture_count, mem, 4);
		mem_read(&texture_data_size, mem, 4);
		u8* texture_data = (u8*) malloc(texture_data_size);
		mem_read(texture_data, mem, texture_data_size);
		mem_read(&texture_data_checksum, mem, 1);
		// TODO: further parse texture data block


		// Events
		





	}
}

void load_world(const char* filename) {
	mem_t* mem = read_entire_file(filename);
	if (mem) {
		u16 background_width;
		u16 background_height;
		u8 plan0_num_pcx_count;
		u8 video_bios_checksum;
		u8 bios_checksum;

		// Header block
		mem_read(&background_width, mem, 2);
		mem_read(&background_height, mem, 2);
		mem_read(&plan0_num_pcx_count, mem, 1);
		mem_read(&video_bios_checksum, mem, 1);
		mem_read(&bios_checksum, mem, 1);

		mem_read(plan0_num_pcx, mem, plan0_num_pcx_count);

		for (i32 i = 0; i < plan0_num_pcx_count; ++i) {
			plan0_num_pcx[i] ^= 0x15;
		}


		// Sprites block
		u16 group_count;
		mem_read(&group_count, mem, 2);

		for (i32 group_index = 0; group_index < group_count; ++group_index) {
			u8 need_clear_background;
			u32 atlas_image_len;
			mem_read(&need_clear_background, mem, 1);
			mem_read(&atlas_image_len, mem, 4);

			u8* atlas_pixels = (u8*) malloc(atlas_image_len);
			mem_read(atlas_pixels, mem, atlas_image_len);

			u8 atlas_image_checksum;
			mem_read(&atlas_image_checksum, mem, 1);

			u16 sprite_count;
			mem_read(&sprite_count, mem, 2);

			for (i32 sprite_index = 0; sprite_index < sprite_count; ++sprite_index) {
				sprite_desc_t sprite_desc = {0};
				mem_read(&sprite_desc, mem, sizeof(sprite_desc));
			}

			u8 anim_count;
			mem_read(&anim_count, mem, 1);

			for (i32 anim_index = 0; anim_index < anim_count; ++anim_index) {
				anim_desc_t anim_desc = {0};
				mem_read(&anim_desc, mem, sizeof(anim_desc));

				// Need to seek to the specified offset later from the current position in the stream.
				u16 frame_table_cursor = mem->cursor + anim_desc.layers_per_frame;

				i32 layer_count = anim_desc.layers_per_frame * anim_desc.frame_count;
				for (i32 layer_index = 0; layer_index < layer_count; ++layer_index) {
					anim_layer_t layer = {0};
					mem_read(&layer, mem, sizeof(layer));
				}

				mem->cursor = frame_table_cursor; // seek
				for (i32 frame_index = 0; frame_index < anim_desc.frame_count; ++frame_index) {
					anim_frame_t frame = {0};
					mem_read(&frame, mem, sizeof(frame));
				}

			}

		}

		// ETA block


		// to be continued...
		

	}
}

struct language_info_t {
	u32 file_offset;
	u32 buffer_size;
	u8 decoding_byte;
	u8 checksum_byte;
};

language_info_t language_infos[] = {
	{0, 0x108A, 0x30, 0xB4},
};

char* language_buffer;

void load_language_txt(const char* filename) {
	mem_t* mem = read_entire_file(filename);
	if (mem) {
		language_info_t* language = &language_infos[0];
		language_buffer = (char*) malloc(language->buffer_size);
		mem->cursor = language->file_offset;
		i32 bytes_read = mem_read(language_buffer, mem, language->buffer_size);
		if (bytes_read != (i32) language->buffer_size) {
			// error
			exit(1);
		}

		// decode language txts
		u8* buffer_pos = (u8*) language_buffer;
		u8* buffer_end = (u8*) language_buffer + language->buffer_size;
		u8 checksum_byte = language->checksum_byte;
		u8 decoding_byte = language->decoding_byte;
		for (; buffer_pos < buffer_end; ++buffer_pos) {
			u8 c = *buffer_pos;
			checksum_byte -= c;
			c ^= decoding_byte;
			*buffer_pos = c;
		}

		// dump to file
		FILE* fp = fopen("language_dump.txt", "wb");
		fwrite(language_buffer, language->buffer_size, 1, fp);
		fclose(fp);

	}
}

struct snd8b_file_info_t {
	u32 offset;
	u32 size;
	u8 encoding_byte;
	u8 checksum_byte;
	u8 unknown1;
	u8 unknown2;
};

snd8b_file_info_t sndh8b_info[7] = {
	{0,      0x0800, 0x4D, 0xC3, 0x00, 0x00},
	{0x0800, 0x0800, 0xD9, 0xC1, 0x00, 0x00},
	{0x1000, 0x0800, 0x24, 0x8E, 0x00, 0x00},
	{0x1800, 0x0800, 0xFA, 0x16, 0x00, 0x00},
	{0x2000, 0x0800, 0x67, 0x49, 0x00, 0x00},
	{0x2800, 0x0800, 0xAB, 0xB7, 0x00, 0x00},
	{0x3000, 0x0800, 0x63, 0xDE, 0x00, 0x00},
};

snd8b_file_info_t sndd8b_info[7] = {
	{0,        0x01D140, 0xC0, 0x3C, 0x00, 0x00},
	{0x01D140, 0x02224C, 0x94, 0x68, 0x00, 0x00},
	{0x03F38C, 0x046DE8, 0x29, 0x95, 0x00, 0x00},
	{0x086174, 0x03181C, 0xED, 0x17, 0x00, 0x00},
	{0x0B7990, 0x030E98, 0x24, 0xB4, 0x00, 0x00},
	{0x0E8828, 0x02F2F0, 0xF3, 0x3B, 0x00, 0x00},
	{0x117B18, 0x02D588, 0xF8, 0x33, 0x00, 0x00}
};


// Decodes and verifies xor-encoded data
u8 decode_xor(u8* data, u32 size, u8 encoding_byte, u8 checksum_byte) {
	u8* end = data + size;
	for (u8* pos = data; pos < end; ++pos) {
		u8 curr_byte = *pos;
		checksum_byte -= curr_byte; // overflows
		curr_byte ^= encoding_byte;
		*pos = curr_byte;
	}
	return checksum_byte; // should be 0
}


void load_snd8b(u8** sound_buffer, i32 sound_set) {
	// Load sound header
	mem_t* mem = read_entire_file("SNDH8B.DAT");
	if (!mem) fatal_error();

	snd8b_file_info_t* header_info = &sndh8b_info[sound_set];

	mem_seek(mem, header_info->offset);
	mem_read(snd8b_headers, mem, header_info->size); // always 0x800 bytes

	u8 checksum = decode_xor(snd8b_headers, header_info->size, header_info->encoding_byte, header_info->checksum_byte);
	if (checksum != 0) {
		printf("[warning] load_snd8b(): incorrect checksum for SNDH8B.DAT\n");
	}

	free(mem);

	// Load sound data
	snd8b_file_info_t* data_info = &sndd8b_info[sound_set];
	size_t sound_size = data_info->size;
	void* temp = realloc(*sound_buffer, sound_size);
	if (!temp) fatal_error();
	*sound_buffer = (u8*)temp;
	

	FILE* fp = open_data_file("SNDD8B.DAT");
	if (!fp) fatal_error();

	fseek(fp, data_info->offset, SEEK_SET);
	size_t bytes_read = fread(*sound_buffer, 1, sound_size, fp);
	if (bytes_read != data_info->size) fatal_error();

	fclose(fp);

	checksum = decode_xor(*sound_buffer, (u32)sound_size, data_info->encoding_byte, data_info->checksum_byte);
	if (checksum != 0) {
		printf("[warning] load_snd8b(): incorrect checksum for SNDD8B.DAT\n");
	}


}




// sub_1D808
void set_subetat(event_t* event, u8 subetat) {
	event->change_anim_mode = 1;
	event->subetat = subetat;
}

// sub_1D810
void set_etat(event_t* event, u8 etat) {
	event->change_anim_mode = 1;
	event->etat = etat;
}

// sub_1D818
void set_event_state(event_t* event, u8 etat, u8 subetat) {
	event->change_anim_mode = 1;
	event->etat = etat;
	event->subetat = subetat;
}



// sub_55EE4
// (?)
void update_x_momentum(event_t* event) {
	u8 anim_index = event->anim_index;
	// some unknown stuff
	// STUB
	if (1) {
		eta_t* eta = &event->ETA[event->etat][0][event->subetat];
		if (event->flags & event_flags_8_dir_right) {

		}

	}
}

// sub_1E588
u8 get_tile_type(i32 tile_x, i32 tile_y) {
	if (tile_x >= 0 && tile_x <= (level_width_tiles - 1) && tile_y >= 0 && tile_y <= (level_height_tiles - 1)) {
		return level_tiles[tile_y * level_width_tiles + tile_x].tile_type;
	} else {
		return 0;
	}
}

// sub_24F60
u8 sub_24F60(event_t* event, i32 unk) {
	return 0; // stub
}

// sub_70408
void ray_hurt() {

}

// sub_2DBDC
void ray_hit(bool put_above_solid_tiles, event_t* other_event) {
	if (put_above_solid_tiles) {
		ray_hurt();
		i32 tile_x = (ray.xpos + ray.offset_bx) / 16;
		for (;;) {
			i32 tile_y = (ray.ypos + ray.offset_by) / 16;
			if (get_tile_type(tile_x, tile_y) != 10) {
				--ray.ypos;
			} else {
				break;
			}
		}
	}
	if (ray.etat == 6) {
		set_event_state(&ray, 6, 8);
		ray.yspeed = 0;
		ray.xspeed = 0;
		byte_CE780 = 0;
	} else if (ray.flags & event_flags_4_switched_on) {
		if (!(ray.etat == 3 && (ray.subetat == 22 || ray.subetat == 32)) &&
		    !(ray.etat == 2 && ray.subetat == 31)
	    ) {
			if ((get_eta(&ray)->interaction_flags & 0x40) && (tile_flags[sub_24F60(&ray, 2)] & 0x10)) {
				set_event_state(&ray, 0, 61);
			} else {
				set_event_state(&ray, 2, 8);
			}
			word_CF83C = 0;
			if (ray.etat == 0 && ray.subetat == 61) {
				ray.xspeed = (ray.flags & event_flags_8_dir_right) ? -2 : 2;
				ray.yspeed = -3;
			} else {
				if (other_event != NULL) {
					i32 bump_direction = -1;
					if (other_event->type == event_180_mr_sax_chasing) {
						//bump_direction = sub_778CC();
					} else if (other_event->type == event_150_mr_skops) {
						//bump_direction = sub_79688();
					} else if (other_event->type == event_198_mr_stone_chase) {
						bump_direction = 1;
					} else if (other_event->type == event_200) {
						bump_direction = -1;
					} else if (other_event->type == event_100_moth) {
						bump_direction = (other_event->flags & event_flags_8_dir_right) ? 1 : -1;
					} else if (other_event->type == event_120_red_drummer) {
						// bump_direction = sub_2008C(other_event);
					} else if (other_event->type == event_187_viking_mama) {
						// bump_direction = sub_6628C(other_event);
					} else if (other_event->type == event_209_mr_dark_flame_left) {
						bump_direction = 1;
					} else if (other_event->type == event_210_mr_dark_flame_right) {
						bump_direction = -1;
					} else {
						i32 xspeed_delta = other_event->xspeed - ray.xspeed;
						if (xspeed_delta == 0) {
							bump_direction = (ray.flags & event_flags_8_dir_right) ? 1 : -1;
						} else {
							bump_direction = xspeed_delta > 0 ? 1 : -1;
						}
					}
					i32 bump_speed;
					if (obj_type_flags[other_event->type] & obj_type_flags_bit_10_fast_bump) {
						bump_speed = 5;
					} else if (other_event->type == event_180_mr_sax_chasing) {
						bump_speed = 4;
					} else {
						bump_speed = 2;
					}
					ray.xspeed = bump_speed * bump_direction; // Note: can it be a different value except 1 or -1?
					ray.yspeed = -bump_speed;

				}
			}

			// stub

		}
	}
}


// Command system

void script_goto_label(event_t* event, u8 unk1, u8 unk2) {
	// stub
}

// sub_2F44C
void do_cmd_up(event_t* event) {
	if (event->etat == 1) {
		event->xspeed = 0;
		event->yspeed = 0;
		set_event_state(event, 0, 0);
	} else {
		
		if (!(event->type == event_10_piranha && event->etat == 0 && event->subetat == 0)) {
			event->yspeed = 0;
		}
	}
}

// sub_2F594
void do_cmd_3_4(event_t* event) {
	if (event->type == event_1_platform) {
		if (event->command == cmd_3_down) {
			event->yspeed = -2;
		} else if (event->command == cmd_4) {
			event->yspeed = 2;
		}
	} else if (event->type == event_10_piranha) {
		if (event->etat == 0 && event->subetat == 0) {
			if (event->command == cmd_3_down) {
				event->yspeed = -2;
			} else {
				event->yspeed = 2;
			}
		}
	} else if (event->type == event_24) {
		// This procedure is only called for commands 3 and 4, so, not sure why we are checking for command 2 here?
		if (event->command != cmd_2_up) {
			--event->command_count;
			if (event->command_count <= 0) {
				if (event->command == cmd_3_down) {
					event->yspeed = -1;
				} else if (event->command == cmd_4) {
					event->yspeed = 1;
				}
			} else {
				event->yspeed = 0;
				event->command_count = 0;
				event->command = cmd_2_up;
				event->spawn_y = event->ypos;
			}

		}
	}
}

// sub_2F63C
void sub_2F63C(event_t* event) {
	if (event->type == event_0_livingstone && event->etat == 1 && event->subetat == 11) {
		event->flags &= ~0x10;
	}
}

// sub_3CB54
void enter_scene(void* scene_func) {
	//stub
}

rgb_palette_t current_source_palette;
u8 fade_mode; //CFA87 (?)
u16 fade_temp[256*3];
u16 fade_speed; //CF7EA


void do_fade_step(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
	if (nb_fade > 0) {
		--nb_fade;
		if (fade_mode == 1) {
			for (i32 i = 0; i < 256*3; ++i) {
				u8 c1 = ((u8*)(source_pal->colors))[i];
				u32 temp = c1 << fade_speed;
				fade_temp[i] += temp;
			}
		} else if (fade_mode == 2) {
			for (i32 i = 0; i < 256*3; ++i) {
				if (fade_temp[i] > 0) {
					u8 c1 = ((u8*)(source_pal->colors))[i];
					u32 temp = c1 << fade_speed;
					fade_temp[i] -= temp;
				}
			}
		}
		for (i32 i = 0; i < 256*3; ++i) {
			u16 temp = fade_temp[i] >> 6;
			((u8*)(dest_pal->colors))[i] = temp;
		}
		// (apply palette)
		if (nb_fade == 0) {
			fade_mode |= 0x40;
		}
	}
}

//3C54C
void start_fade_in(u32 speed) {
	// apply palette par_0?
	nb_fade = 1 << (6 - speed);
	fade_mode = 1; // fade in
	for (i32 i = 0; i < 256*3; ++i) {
		fade_temp[i] = ((u8*)(global_game->draw_buffer.pal->colors))[i];
	}
	memset(global_game->draw_buffer.pal, 0, sizeof(rgb_palette_t));
	fade_speed = speed;
	// apply palette
}

void start_fade_out(u32 speed) {
	nb_fade = 1 << (6 - speed);
	fade_mode = 2; // fade out
	fade_speed = speed;
}

void fade_out(u32 speed, rgb_palette_t* palette) {
	start_fade_out(speed);
	u16 steps = nb_fade;
	for (i32 i = 0; i < steps; ++i) {
		advance_frame();
		do_fade_step(palette, global_game->draw_buffer.pal);
	}

}


void ubisoft_logo_loop(i32 par_0, i32 par_1, i32 par_2) {
	start_fade_in(2);
	wait_frames(5);
	for (i32 i = 0; i < par_0; ++i) {
		do_fade_step(&current_source_palette, global_game->draw_buffer.pal);
		advance_frame();
	}
	while (is_ogg_playing) {
		advance_frame();
	}
	fade_out(2, &current_source_palette);
	wait_frames(1);
}

void start_basic_fade_in() {
	start_fade_in(2);
}

//71B34
void do_ubisoft_intro() {
	image_t ubisoft_logo = load_vignet_pcx(29);
	copy_full_image_to_draw_buffer(&ubisoft_logo);
	current_source_palette = *ubisoft_logo.pal;
	destroy_image(&ubisoft_logo);
	start_basic_fade_in();
	play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
	ubisoft_logo_loop(60, -1, 8);


}

//35CC4
void do_big_ray_animation() {
	load_big_ray();
	image_t background = load_vignet_pcx(12);
	copy_full_image_to_draw_buffer(&background);
	current_source_palette = *background.pal;
	destroy_image(&background);
	start_basic_fade_in();
	play_cd_track(19); // Menu music - "World Map"


	u16 steps = nb_fade;
	for (i32 i = 0; i < steps; ++i) {
		do_fade_step(&current_source_palette, global_game->draw_buffer.pal);
		advance_frame();
	}
	while (is_ogg_playing) {
		advance_frame();
	}
	fade_out(2, &current_source_palette);
	wait_frames(600);


}

void do_big_ray_intro() {
	do_big_ray_animation();
}

void rayman_main() {
	do_ubisoft_intro();
	do_big_ray_animation();
}
