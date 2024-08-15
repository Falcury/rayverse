


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
	mem_t* mem = read_entire_file(filename, true);
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
	mem_t* mem = read_entire_file(filename, true);
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
				sprite_t sprite_desc = {0};
				mem_read(&sprite_desc, mem, sizeof(sprite_desc));
			}

			u8 anim_count;
			mem_read(&anim_count, mem, 1);

			for (i32 anim_index = 0; anim_index < anim_count; ++anim_index) {
				anim_t anim_desc = {0};
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

archive_header_t language_infos[] = {
	{0, 4234, 48, 180},      // English
	{4234, 4713, 130, 161},  // French
	{8947, 4903, 207, 92},   // German
	{13850, 2511, 208, 192}, // Japanese
	{16361, 2366, 149, 20},  // Chinese
};

char* language_buffer;

char* GetStringTxt(char* txt, char* out_buf) {
	char* pos = txt;
	char c = *pos;
	if (c != ',') {
		for (;;) {
			c = *pos;
			if (c == '*') {
				break;
			}
			if (c >= ' ') {
				*out_buf++ = c;
			}
			++pos;
			if (*pos == ',') {
				break;
			}
		}
	}
	if (*pos == '*') {
		return NULL;
	} else {
		*out_buf = '\0';
		return pos + 1;
	}
}

void LoadLanguageTxt(i32 language_index) {
	mem_t* mem = read_entire_file("RAY.LNG", true);
	if (mem) {
		archive_header_t* lng_archive_header = &language_infos[language_index]; // is language_index used here?
		language_buffer = (char*) malloc(lng_archive_header->size);
		mem->cursor = lng_archive_header->offset;
		i32 bytes_read = mem_read(language_buffer, mem, lng_archive_header->size);
		if (bytes_read != lng_archive_header->size) {
			// error
			exit(1);
		}

		// decode language txts
		u8* buffer_pos = (u8*) language_buffer;
		u8* buffer_end = (u8*) language_buffer + lng_archive_header->size;
		u8 checksum_byte = lng_archive_header->checksum_byte;
		u8 decoding_byte = lng_archive_header->xor_byte;
		for (; buffer_pos < buffer_end; ++buffer_pos) {
			u8 c = *buffer_pos;
			checksum_byte -= c;
			c ^= decoding_byte;
			*buffer_pos = c;
		}

		char* txt = language_buffer;
		char* next_txt = txt;
		for (i32 i = 0; next_txt != NULL; ++i) {
			char temp_buffer[400];
			next_txt = GetStringTxt(txt, temp_buffer);
			if (next_txt != NULL) {
				language_txt[i] = txt;
				next_txt[-1] = '\0'; // zero-terminate txt
				txt = next_txt + 2; // skip \r\n
			}
		}

#if 0
		// DEBUG: dump to file
		FILE* fp = fopen("language_dump.txt", "wb");
		fwrite(language_buffer, language->size, 1, fp);
		fclose(fp);
#endif

	}
}


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

// sub_1D808
void set_subetat(obj_t* obj, u8 subetat) {
	obj->change_anim_mode = 1;
	obj->subetat = subetat;
}

// sub_1D810
void set_etat(obj_t* obj, u8 etat) {
	obj->change_anim_mode = 1;
	obj->etat = etat;
}

// sub_1D818
void set_obj_state(obj_t* obj, u8 etat, u8 subetat) {
	obj->change_anim_mode = 1;
	obj->etat = etat;
	obj->subetat = subetat;
}



// sub_55EE4
// (?)
void update_x_momentum(obj_t* obj) {
	u8 anim_index = obj->anim_index;
	// some unknown stuff
	// STUB
	if (1) {
		eta_t* eta = get_eta(obj);
		if (obj->flags & obj_flags_8_flipped) {

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
u8 sub_24F60(obj_t* obj, i32 unk) {
	return 0; // stub
}

// sub_70408
void ray_hurt() {

}

// sub_2DBDC
void ray_hit(bool put_above_solid_tiles, obj_t* other_obj) {
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
		set_obj_state(&ray, 6, 8);
		ray.yspeed = 0;
		ray.xspeed = 0;
		byte_CE780 = 0;
	} else if (ray.flags & obj_flags_4_switched_on) {
		if (!(ray.etat == 3 && (ray.subetat == 22 || ray.subetat == 32)) &&
		    !(ray.etat == 2 && ray.subetat == 31)
	    ) {
			if ((get_eta(&ray)->interaction_flags & 0x40) && (tile_flags[sub_24F60(&ray, 2)] & 0x10)) {
				set_obj_state(&ray, 0, 61);
			} else {
				set_obj_state(&ray, 2, 8);
			}
			word_CF83C = 0;
			if (ray.etat == 0 && ray.subetat == 61) {
				ray.xspeed = (ray.flags & obj_flags_8_flipped) ? -2 : 2;
				ray.yspeed = -3;
			} else {
				if (other_obj != NULL) {
					i32 bump_direction = -1;
					if (other_obj->type == obj_180_mr_sax_chasing) {
						//bump_direction = sub_778CC();
					} else if (other_obj->type == obj_150_mr_skops) {
						//bump_direction = sub_79688();
					} else if (other_obj->type == obj_198_mr_stone_chase) {
						bump_direction = 1;
					} else if (other_obj->type == obj_200) {
						bump_direction = -1;
					} else if (other_obj->type == obj_100_moth) {
						bump_direction = (other_obj->flags & obj_flags_8_flipped) ? 1 : -1;
					} else if (other_obj->type == obj_120_red_drummer) {
						// bump_direction = sub_2008C(other_event);
					} else if (other_obj->type == obj_187_viking_mama) {
						// bump_direction = sub_6628C(other_event);
					} else if (other_obj->type == obj_209_mr_dark_flame_left) {
						bump_direction = 1;
					} else if (other_obj->type == obj_210_mr_dark_flame_right) {
						bump_direction = -1;
					} else {
						i32 xspeed_delta = other_obj->xspeed - ray.xspeed;
						if (xspeed_delta == 0) {
							bump_direction = (ray.flags & obj_flags_8_flipped) ? 1 : -1;
						} else {
							bump_direction = xspeed_delta > 0 ? 1 : -1;
						}
					}
					i32 bump_speed;
					if (obj_type_flags[other_obj->type] & obj_type_flags_bit_10_fast_bump) {
						bump_speed = 5;
					} else if (other_obj->type == obj_180_mr_sax_chasing) {
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

void script_goto_label(obj_t* obj, u8 unk1, u8 unk2) {
	// stub
}

// sub_2F44C
void do_cmd_up(obj_t* obj) {
	if (obj->etat == 1) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_obj_state(obj, 0, 0);
	} else {
		
		if (!(obj->type == obj_10_piranha && obj->etat == 0 && obj->subetat == 0)) {
			obj->yspeed = 0;
		}
	}
}

// sub_2F594
void do_cmd_3_4(obj_t* event) {
	if (event->type == obj_1_platform) {
		if (event->command == cmd_3_down) {
			event->yspeed = -2;
		} else if (event->command == cmd_4) {
			event->yspeed = 2;
		}
	} else if (event->type == obj_10_piranha) {
		if (event->etat == 0 && event->subetat == 0) {
			if (event->command == cmd_3_down) {
				event->yspeed = -2;
			} else {
				event->yspeed = 2;
			}
		}
	} else if (event->type == obj_24) {
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
void sub_2F63C(obj_t* event) {
	if (event->type == obj_0_livingstone && event->etat == 1 && event->subetat == 11) {
		event->flags &= ~0x10;
	}
}




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
	advance_frame();
}

//3CA8C
void INIT_FADE_IN() {
	start_fade_in(2);
}


void ubisoft_logo_loop(i32 par_0, i32 par_1, i32 par_2) {
	start_fade_in(2);
	wait_frames(5);
	for (i32 i = 0; i < par_0; ++i) {
		do_fade_step(&fade_source_palette, global_game->draw_buffer.pal);
		advance_frame();
	}
	while (is_ogg_playing) {
		advance_frame();
	}
	fade_out(2, &fade_source_palette);
	wait_frames(1);
}



//71B34
void DO_UBI_LOGO() {
	image_t ubisoft_logo = load_vignet_pcx(29);
	copy_full_image_to_draw_buffer(&ubisoft_logo);
	fade_source_palette = *ubisoft_logo.pal;
	destroy_image(&ubisoft_logo);
	INIT_FADE_IN();
	play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
	ubisoft_logo_loop(60, -1, 8);
}

i32 screen_xmin = 8;
i32 screen_xmax = 312;
i32 screen_ymin = 0;
i32 screen_ymax = 200;

//16194
void set_default_sprite_clipping() {
	screen_xmin = 8;
	screen_xmax = 312;
	screen_ymin = 0;
	screen_ymax = 200;
}

//161FA
void sprite_clipping(i32 xmin, i32 xmax, i32 ymin, i32 ymax) {
	if (xmin < 0) xmin = 0;
	if (xmin > 320) xmin = 320;
	screen_xmin = xmin;
	if (xmax < 0) xmax = 0;
	if (xmax > 320) xmax = 320;
	screen_xmax = xmax;
	if (ymin < 0) ymin = 0;
	if (ymin > 200) ymin = 200;
	screen_ymin = ymin;
	if (ymax < 0) ymax = 0;
	if (ymax > 200) ymax = 200;
	screen_ymax = ymax;
}

//1626D
bool clip_sprite_on_screen(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
	saved_sprite_width = proj_size->x;
	if (*proj_x < screen_xmin) {
		i32 x_left_of_screen = -(*proj_x - screen_xmin);
		if (proj_size->x <= x_left_of_screen) {
			return false;
		} else {
			x_left_of_screen &= 0xFFFF;
			proj_size->x -= (u8) x_left_of_screen;
			*proj_x = screen_xmin;
			*image_data += x_left_of_screen;
		}
	}
	i32 proj_right = *proj_x + proj_size->x;
	if (proj_right > screen_xmax) {
		if (*proj_x >= screen_xmax) {
			return false;
		} else {
			proj_size->x = screen_xmax - *proj_x;
		}
	}
	if (*proj_y < screen_ymin) {
		if (*proj_y + proj_size->y < screen_ymin) {
			return false;
		} else {
			i32 y_above_screen = -(*proj_y - screen_ymin);
			y_above_screen &= 0xFFFF;
			proj_size->y -= (u8) y_above_screen;
			*image_data += saved_sprite_width * y_above_screen;
			*proj_y = screen_ymin;
		}
	}
	i32 proj_bottom = *proj_y + proj_size->y;
	if (proj_bottom > screen_ymax) {
		if (*proj_y >= screen_ymax) {
			return false;
		} else {
			proj_size->y = screen_ymax - *proj_y;
			return true;
		}
	} else {
		return true;
	}
}

//16323
bool clip_sprite_on_screen_flipped(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
	saved_sprite_width = proj_size->x;
	if (*proj_x < screen_xmin) {
		i32 x_left_of_screen = -(*proj_x - screen_xmin);
		if (proj_size->x <= x_left_of_screen) {
			return false;
		} else {
			x_left_of_screen &= 0xFFFF;
			proj_size->x -= (u8) x_left_of_screen;
			*proj_x = screen_xmin;
		}
	}
	i32 proj_right = *proj_x + proj_size->x;
	if (proj_right > screen_xmax) {
		if (*proj_x >= screen_xmax) {
			return false;
		} else {
			proj_size->x = screen_xmax - *proj_x;
			*image_data += saved_sprite_width - proj_size->x;
		}
	}
	if (*proj_y < screen_ymin) {
		if (*proj_y + proj_size->y < screen_ymin) {
			return false;
		} else {
			i32 y_above_screen = -(*proj_y - screen_ymin);
			y_above_screen &= 0xFFFF;
			proj_size->y -= (u8) y_above_screen;
			*image_data += saved_sprite_width * y_above_screen;
			*proj_y = screen_ymin;
		}
	}
	i32 proj_bottom = *proj_y + proj_size->y;
	if (proj_bottom > screen_ymax) {
		if (*proj_y >= screen_ymax) {
			return false;
		} else {
			proj_size->y = screen_ymax - *proj_y;
			return true;
		}
	} else {
		return true;
	}
}

//16A9D
void draw_simple(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	if (clip_sprite_on_screen(&proj_x, &proj_y, &proj_size, &image_data) && proj_size.x > 0) {
		u8* draw_pos = draw_buffer->memory + proj_y * draw_buffer->width /*320*/ + proj_x;
		u8* draw_end = draw_pos + proj_size.y * draw_buffer->width;
		ASSERT(draw_pos >= draw_buffer->memory && draw_pos < draw_buffer->memory + draw_buffer->memory_size);
		ASSERT(draw_end >= draw_buffer->memory && draw_end < draw_buffer->memory + draw_buffer->memory_size);
		i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
		u8* sprite_pos = image_data;
		while (draw_pos < draw_end) {
			for (i32 i = 0; i < proj_size.x; ++i) {
				u8 c = sprite_pos[i];
				if (c != 0) {
					draw_pos[i] = c;
				}
			}
			sprite_pos += sprite_width;
			draw_pos += draw_buffer->width;
		}
	}
}

//16B88
void draw_simple_flipped(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	if (clip_sprite_on_screen_flipped(&proj_x, &proj_y, &proj_size, &image_data) && proj_size.x > 0) {
		u8* draw_pos = draw_buffer->memory + proj_y * draw_buffer->width /*320*/ + proj_x;
		u8* draw_end = draw_pos + proj_size.y * draw_buffer->width;
		ASSERT(draw_pos >= draw_buffer->memory && draw_pos < draw_buffer->memory + draw_buffer->memory_size);
		ASSERT(draw_end >= draw_buffer->memory && draw_end < draw_buffer->memory + draw_buffer->memory_size);
		i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
		u8* sprite_pos = image_data;
		while (draw_pos < draw_end) {
			for (i32 i = 0; i < proj_size.x; ++i) {
				u8 c = sprite_pos[i];
				if (c != 0) {
					draw_pos[proj_size.x-i-1] = c; // reverse order (flipped)
				}
			}
			sprite_pos += sprite_width;
			draw_pos += draw_buffer->width;
		}
	}
}

//16A24
void sub_16A24(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	//stub
}

//16B08
void sub_16B08(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	//stub
}

//55498
void horloges(u32 ticks) {
	for (i32 i = 0; i < 25; ++i) {
		horloge[i] += 1;
		if (horloge[i] >= i) {
			horloge[i] = 0;
		}
	}
	map_time += ticks;
}

//1D570
i32 get_proj_dist(i32 scale, i32 outer_dim) {
	//NOTE: needs checking
	if (zoom_mode == 0) {
		return outer_dim;
	} else {
		i32 temp = ((256*256) / (scale + 256)) * (outer_dim);
		return (temp / 256);
	}
}

//1D5D8
i32 get_proj_x(i32 scale, i32 par_1) {
	//NOTE: needs checking
	i32 temp = ((256*256) / (scale + 256)) * (par_1 - proj_center_x);
	return ((temp / 256) + proj_center_x);
}

//1D614
i32 get_proj_y(i32 scale, i32 par_1) {
	//NOTE: needs checking
	i32 temp = ((256*256) / (scale + 256)) * (par_1 - proj_center_y);
	return ((temp / 256) + proj_center_y);
}



//18CE8
void display2(obj_t* obj) {
	anim_t* anim = obj->animations + obj->anim_index;
	u16 layers_per_frame = anim->layers_per_frame & 0x3FFF;
	anim_layer_t* layer = anim->layers + (obj->anim_frame * layers_per_frame);
	for (i32 layer_index = 0; layer_index < layers_per_frame; ++layer_index) {
		i32 proj_y = get_proj_y(obj->scale, layer->y + obj->screen_y);
		if (layer->sprite_index != 0) {
			sprite_t* sprite = obj->sprites + layer->sprite_index;
			if (sprite->unk_index != 0) {
				i32 x;
				if (obj->flags & obj_flags_8_flipped) {
					if (obj->scale == 256 && layer_index == 5 && obj->anim_index >= 14 && obj->anim_index <= 16) {
						x = -16;
					} else {
						x = 0;
					}
					x += obj->offset_bx * 2 - layer->x - sprite->outer_width + obj->screen_x;
				} else {
					x = layer->x + obj->screen_x;
				}

				draw_func_t* draw_func = NULL;
				if ((obj->flags & obj_flags_8_flipped) ^ layer->mirrored) {
					draw_func = curr_obj_draw_proc_flipped;
				} else {
					draw_func = curr_obj_draw_proc;
				}

				i32 proj_x = get_proj_x(obj->scale, x);
				i32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
				i32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
				vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
				u8 sprite_field_A = sprite->field_A >> 4;

				u8* image_data = obj->image_atlas + sprite->offset_in_atlas;

				draw_func(proj_x /*eax*/, sprite_field_A /*edx*/, proj_y /*ebx*/, proj_size /*ecx*/, /*edi*/ &global_game->draw_buffer, image_data /*esi*/);

			}
		}

		++layer;
	}

}

//55EE4
void set_x_speed(obj_t* obj) {
	i32 xspeed = 0;
	anim_t* anim = obj->animations + obj->anim_index;
	u8 horloge_index = ((anim->layers_per_frame & 0xC000) >> 14) + 1;
	if (horloge[horloge_index] == 0) {
		eta_t* eta = get_eta(obj);
		if (obj->flags & obj_flags_8_flipped) {
			xspeed = eta->right_speed * horloge_index;
		} else {
			xspeed = eta->left_speed * horloge_index;
		}
		if (obj->type == obj_23_rayman && (skills & skills_0x200_tiny)) {
			xspeed /= 2;
		}
	}
	obj->xspeed = (i16)xspeed;
}

//567AC
void do_anim(obj_t* obj) {
	i32 prev_anim_frame = obj->anim_frame;
	i32 prev_anim_index = obj->anim_index;
	eta_t* eta = get_eta(obj);
	anim_t* anim = obj->animations + obj->anim_index;
	u8 anim_speed = eta->anim_speed & 15;
	if (anim_speed != 0 && horloge[anim_speed] == 0) {
		if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
			--obj->anim_frame;
		} else {
			++obj->anim_frame;
		}
	}
	obj->anim_index = eta->anim_index;
	anim = obj->animations + obj->anim_index;
	if ((obj->change_anim_mode == 1 && obj->anim_index != prev_anim_index) || obj->change_anim_mode == 2) {
		if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
			obj->anim_frame = anim->frame_count - 1;
		} else {
			obj->anim_frame = 0;
		}
		if (obj->is_active != 0) {
			play_sound(eta->sound_index, obj->obj_index);
		}
	}
	if (obj->anim_frame >= anim->frame_count || obj->anim_frame == 255) {
		// animation ended
		obj->etat = eta->next_etat;
		obj->subetat = eta->next_subetat;
		eta = get_eta(obj);
		obj->anim_index = eta->anim_index;
		anim = obj->animations + obj->anim_index;
		if ((obj->type == obj_23_rayman && (ray_old_etat == 2 || ray_old_etat == 6)) ||
		    (obj->subetat == 61 && ray_old_subetat == 61 && ray_old_etat == 0)) {
			if (ray.timer > 60 && !(skills & skills_0x8000_squashed)) {
				ray.timer = 60;
			}
		}
		if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
			obj->anim_frame = anim->frame_count - 1;
		} else {
			obj->anim_frame = 0;
		}
		if (obj->is_active != 0) {
			play_sound(eta->sound_index, obj->obj_index);
		}
	}
	obj->change_anim_mode = 0;
	if (obj->flags & obj_flags_0x20_follow_enabled) {
		//calc_follow_sprite_speed(); //STUB
	}
	u8 anim_changed_bit = 0;
	if (obj->anim_frame != prev_anim_frame || obj->anim_index != prev_anim_index) {
		anim_changed_bit = 1;
	}
	obj->flags &= ~obj_flags_0x80_anim_changed;
	obj->flags |= (anim_changed_bit << 7);

}

//1D074 - original name: EOA
bool end_of_animation(obj_t* obj) {
	eta_t* eta = get_eta(obj);
	bool on_last_frame;
	if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
		on_last_frame = (obj->anim_frame == 0);
	} else {
		anim_t* anim = obj->animations + obj->anim_index;
		on_last_frame = (obj->anim_frame == anim->frame_count - 1);
	}
	if (on_last_frame && horloge[eta->anim_speed & 15] == 0) {
		return true;
	} else {
		return false;
	}
}



//38400
i16 dummy_scene_func(u32 par_0) {
	return 1;
}

// sub_3CB54
void synchro_loop(scene_func_t scene_func) {
	bool scene_ended = false;
	do {
		advance_frame();
		do_fade_step(&fade_source_palette, global_game->draw_buffer.pal);
		u32 timer = 0;
		scene_ended = scene_func(timer);
	} while(!scene_ended);
}

bool LoadOptionsOnDisk() {
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
			mem_read(key_table[i], mem, 1);
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

//41AF0
void POINTEUR_BOUTONS_OPTIONS_BIS() {
	// stub
}


//42790
bool set_joy_input_funcs() {
	//stub
	return true;
}

//498A4
void set_special_key_descriptions(const char** descriptions) {
	descriptions[SC_BACKSPACE] = language_txt[212]; // backspace
	descriptions[SC_TAB] = language_txt[213]; // tab
	descriptions[SC_ENTER] = language_txt[214]; // return
	descriptions[SC_CONTROL] = language_txt[210]; // ctrl
	descriptions[SC_ALT] = language_txt[211]; // alt
	descriptions[SC_SPACE] = language_txt[209]; // space
	descriptions[SC_CAPSLOCK] = language_txt[215]; // caps lock
	descriptions[SC_HOME] = language_txt[216]; // home
	descriptions[SC_UP] = language_txt[205]; // up arrow
	descriptions[SC_PAGEUP] = language_txt[217]; // page up
	descriptions[SC_LEFT] = language_txt[204]; // left arrow
	descriptions[SC_RIGHT] = language_txt[206]; // right arrow
	descriptions[SC_END] = language_txt[218]; // end
	descriptions[SC_DOWN] = language_txt[207]; // down arrow
	descriptions[SC_PAGEDOWN] = language_txt[219]; // page down
	descriptions[SC_INSERT] = language_txt[220]; // insert
	descriptions[SC_DELETE] = language_txt[221]; // del
}

//49308
void check_key_table() {
	bool valid = true;
	for (i32 i = 0; i < 7; ++i) {
		u8 key = *(key_table[i]);
		if (key > SC_DELETE || key_descriptions_qwerty[key] == 0 || key_descriptions_azerty[key] == 0) {
			valid = false;
			break; // added
		}
	}
	if (!valid) {
		// load defaults
		*(key_table[0]) = SC_LEFT;
		*(key_table[1]) = SC_UP;
		*(key_table[2]) = SC_RIGHT;
		*(key_table[3]) = SC_DOWN;
		*(key_table[4]) = SC_CONTROL;
		*(key_table[5]) = SC_ALT;
		*(key_table[6]) = SC_X;
	}
}

//49388
void LOAD_CONFIG() {
	if (LoadOptionsOnDisk()) {
		LoadLanguageTxt(0); // English
		if (xpadmax == -1) {
			GameModeVideo = 1;
			P486 = 0;
		}
		if (is_background_available == 2) {
			GameModeVideo = 1;
			P486 = 1;
		}
		POINTEUR_BOUTONS_OPTIONS_BIS();

		// STUB: this section seems to be related to sound card initialization
		/*if (CarteSonAutorisee && DeviceID != 999) {
			//SetPort(Port);
			//SetIrq(Irq);
			//SetDma(Dma);
			//SetParam(Param);
			//if (sub_3E780()) {} // seems to do something with locking memory regions (DOS protected mode stuff)

		} else {
			//SetDeviceID(DeviceID);
			//CarteSonAutorisee = 0;
		}*/

		if (CarteSonAutorisee) {
			load_snd8b(&base_snd8b_data, 2);
			InitSnd();
		}

		if (set_joy_input_funcs()) {
			// stub: init joypad
		} else {
			if (xpadmax == -1) {
				xpadmax = 2;
			}
		}

		// stub
	} else {
		// We didn't load a .cfg file -> load defaults
		if (CarteSonAutorisee) {
			load_snd8b(&base_snd8b_data, 2);
			InitSnd();
		}
		if (set_joy_input_funcs()) {
			//vignet_load_proc = load_vignet_12;
			//sub_23F30();
			//start_fade_out(2);
			//do_calibrate_joystick_menu(); //stub
		} else {
			xpadmax = -2;
		}
		options_jeu_music_enabled = 2;
		options_jeu_sound_volume = 18;
		options_jeu_is_stereo = 1;
		options_jeu_KeyJump = 1;
		options_jeu_KeyWeapon = 0;
		options_jeu_KeyAction = 2;
		SetVolumeSound((options_jeu_sound_volume * 127) / 20);
		GameModeVideo = 0;
	}

	set_special_key_descriptions(key_descriptions_azerty);
	set_special_key_descriptions(key_descriptions_qwerty);
	check_key_table();
}

//17F00
void InitMemoryVariable() {
	P486 = 0;
	is_normal_mode_available = 1;
	is_zoom_effect_available = 1;
	CarteSonAutorisee = 1;
	son_limite = 0;
	is_background_clearing_needed = 1;
	is_background_available = 1;
	TailleMainMemTmp = 0x22000;
	TailleMainMemWorld = 0xF4C00;
	TailleMainMemLevel = 0x87C00;
	TailleMainMemSprite = 0xDF400;
	TailleMainMemFix = 0x4D800;
}

void rayman_main() {
	InitMemoryVariable();
	sprite_clipping(0, 320, 0, 200);
	wait_frames(10); // added
	DO_UBI_LOGO();
	DO_GROS_RAYMAN();
	LOAD_CONFIG();
	//init_cheats(); // stub

	// let the music finish playing while the main menu is not implemented yet :(
	while (is_ogg_playing) {
		advance_frame();
	}
}
