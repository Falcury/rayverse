


// sub_1a3f0
void display_in_game_text(const char* text, int unknown1, int x, int y, int unknown2) {
	// stub
}


void load_level(const char* filename) {
	mem_t* mem = read_entire_file(filename, true);
	if (mem) {
		//stream_t stream = mem_create_stream(mem);

		// Header
		u32 event_block_offset;
		u32 texture_block_offset;	
		mem_read(&event_block_offset, mem, 4);
		mem_read(&texture_block_offset, mem, 4);
		mem_read(&mp.width, mem, 2);
		mem_read(&mp.height, mem, 2);
		mem_read(rvb, mem, sizeof(rvb));
		mem_read(&active_palette, mem, 1);

		// Map
		mp.length = mp.width * mp.height;
		map_tile_t* tiles = (map_tile_t*) malloc(mp.length * sizeof(map_tile_t));
		mem_read(tiles, mem, mp.length * sizeof(map_tile_t));

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


// sub_24F60
u8 calc_typ_trav(obj_t* obj, i32 unk) {
	return 0; // stub
}

// sub_2DBDC
void RAY_HIT(bool put_above_solid_tiles, obj_t* other_obj) {
	if (put_above_solid_tiles) {
		RAY_HURT();
		i32 tile_x = (ray.xpos + ray.offset_bx) / 16;
		for (;;) {
			i32 tile_y = (ray.ypos + ray.offset_by) / 16;
			if (BTYP(tile_x, tile_y) != 10) {
				--ray.ypos;
			} else {
				break;
			}
		}
	}
	if (ray.main_etat == 6) {
		set_main_and_sub_etat(&ray, 6, 8);
		ray.yspeed = 0;
		ray.xspeed = 0;
		poing.is_charging = 0;
	} else if (ray.flags & obj_flags_4_triggered) {
		if (!(ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32)) &&
		    !(ray.main_etat == 2 && ray.sub_etat == 31)
	    ) {
			if ((get_eta(&ray)->interaction_flags & 0x40) && (tile_flags[calc_typ_trav(&ray, 2)] & 0x10)) {
				set_main_and_sub_etat(&ray, 0, 61);
			} else {
				set_main_and_sub_etat(&ray, 2, 8);
			}
			ray_speed_inv = 0;
			if (ray.main_etat == 0 && ray.sub_etat == 61) {
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
	if (obj->main_etat == 1) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_main_and_sub_etat(obj, 0, 0);
	} else {
		
		if (!(obj->type == obj_10_piranha && obj->main_etat == 0 && obj->sub_etat == 0)) {
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
		if (event->main_etat == 0 && event->sub_etat == 0) {
			if (event->command == cmd_3_down) {
				event->yspeed = -2;
			} else {
				event->yspeed = 2;
			}
		}
	} else if (event->type == obj_24) {
		// This procedure is only called for commands 3 and 4, so, not sure why we are checking for command 2 here?
		if (event->command != cmd_2_up) {
			--event->nb_cmd;
			if (event->nb_cmd <= 0) {
				if (event->command == cmd_3_down) {
					event->yspeed = -1;
				} else if (event->command == cmd_4) {
					event->yspeed = 1;
				}
			} else {
				event->yspeed = 0;
				event->nb_cmd = 0;
				event->command = cmd_2_up;
				event->spawn_y = event->ypos;
			}

		}
	}
}

// sub_2F63C
void special_pour_liv(obj_t* event) {
	if (event->type == obj_0_livingstone && event->main_etat == 1 && event->sub_etat == 11) {
		event->flags &= ~0x10;
	}
}



//3C5CC
void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
	if (nb_fade > 0) {
		--nb_fade;
		if (fade == 1) {
			for (i32 i = 0; i < 256*3; ++i) {
				u8 c1 = ((u8*)(source_pal->colors))[i];
				u32 temp = c1 << fade_speed;
                rvb_fade[i] += temp;
			}
		} else if (fade == 2) {
			for (i32 i = 0; i < 256*3; ++i) {
				if (rvb_fade[i] > 0) {
					u8 c1 = ((u8*)(source_pal->colors))[i];
					u32 temp = c1 << fade_speed;
                    rvb_fade[i] -= temp;
				}
			}
		}
		for (i32 i = 0; i < 256*3; ++i) {
			u16 temp = rvb_fade[i] >> 6;
			((u8*)(dest_pal->colors))[i] = (u8)temp;
		}
		// (apply palette)
		if (nb_fade == 0) {
			fade |= 0x40;
		}
	}
}

//3C54C
void start_fade_in(u32 speed) {
	// apply palette par_0?
	nb_fade = 1 << (6 - speed);
	fade = 1; // fade in
	for (i32 i = 0; i < 256*3; ++i) {
        rvb_fade[i] = ((u8*)(global_game->draw_buffer.pal->colors))[i];
	}
	memset(global_game->draw_buffer.pal, 0, sizeof(rgb_palette_t));
	fade_speed = speed;
	// apply palette
}

void start_fade_out(u32 speed) {
	nb_fade = 1 << (6 - speed);
	fade = 2; // fade out
	fade_speed = speed;
}

void fade_out(u32 speed, rgb_palette_t* palette) {
	start_fade_out(speed);
	u16 steps = nb_fade;
	for (i32 i = 0; i < steps; ++i) {
		advance_frame();
		do_fade(palette, global_game->draw_buffer.pal);
	}
	advance_frame();
}

//3CA8C
void INIT_FADE_IN(void) {
	start_fade_in(2);
}

//71940
void updateLogo(i32 par_0, i32 par_1, i32 par_2) {
	start_fade_in(2);
	WaitNSynchro(5);
	for (i32 i = 0; i < par_0; ++i) {
		do_fade(&fade_source_palette, global_game->draw_buffer.pal);
		advance_frame();
	}
	while (is_ogg_playing) {
		advance_frame();
	}
	fade_out(2, &fade_source_palette);
	WaitNSynchro(1);
}



//71B34
void DO_UBI_LOGO(void) {
	image_t ubisoft_logo = load_vignet_pcx(29);
	copy_full_image_to_draw_buffer(&ubisoft_logo);
	fade_source_palette = *ubisoft_logo.pal;
	destroy_image(&ubisoft_logo);
	INIT_FADE_IN();
	play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
    updateLogo(60, -1, 8);
}

i32 XMIN = 8;
i32 XMAX = 312;
i32 YMIN = 0;
i32 YMAX = 200;

//16194
void default_sprite_clipping(void) {
	XMIN = 8;
	XMAX = 312;
	YMIN = 0;
	YMAX = 200;
}

//161FA
void sprite_clipping(i32 xmin, i32 xmax, i32 ymin, i32 ymax) {
	if (xmin < 0) xmin = 0;
	if (xmin > 320) xmin = 320;
	XMIN = xmin;
	if (xmax < 0) xmax = 0;
	if (xmax > 320) xmax = 320;
	XMAX = xmax;
	if (ymin < 0) ymin = 0;
	if (ymin > 200) ymin = 200;
	YMIN = ymin;
	if (ymax < 0) ymax = 0;
	if (ymax > 200) ymax = 200;
	YMAX = ymax;
}

//1626D
bool clip_sprite_on_screen(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
	saved_sprite_width = proj_size->x;
	if (*proj_x < XMIN) {
		i32 x_left_of_screen = -(*proj_x - XMIN);
		if (proj_size->x <= x_left_of_screen) {
			return false;
		} else {
			x_left_of_screen &= 0xFFFF;
			proj_size->x -= (u8) x_left_of_screen;
			*proj_x = XMIN;
			*image_data += x_left_of_screen;
		}
	}
	i32 proj_right = *proj_x + proj_size->x;
	if (proj_right > XMAX) {
		if (*proj_x >= XMAX) {
			return false;
		} else {
			proj_size->x = XMAX - *proj_x;
		}
	}
	if (*proj_y < YMIN) {
		if (*proj_y + proj_size->y < YMIN) {
			return false;
		} else {
			i32 y_above_screen = -(*proj_y - YMIN);
			y_above_screen &= 0xFFFF;
			proj_size->y -= (u8) y_above_screen;
			*image_data += saved_sprite_width * y_above_screen;
			*proj_y = YMIN;
		}
	}
	i32 proj_bottom = *proj_y + proj_size->y;
	if (proj_bottom > YMAX) {
		if (*proj_y >= YMAX) {
			return false;
		} else {
			proj_size->y = YMAX - *proj_y;
			return true;
		}
	} else {
		return true;
	}
}

//16323
bool clip_sprite_on_screen_flipped(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
	saved_sprite_width = proj_size->x;
	if (*proj_x < XMIN) {
		i32 x_left_of_screen = -(*proj_x - XMIN);
		if (proj_size->x <= x_left_of_screen) {
			return false;
		} else {
			x_left_of_screen &= 0xFFFF;
			proj_size->x -= (u8) x_left_of_screen;
			*proj_x = XMIN;
		}
	}
	i32 proj_right = *proj_x + proj_size->x;
	if (proj_right > XMAX) {
		if (*proj_x >= XMAX) {
			return false;
		} else {
			proj_size->x = XMAX - *proj_x;
			*image_data += saved_sprite_width - proj_size->x;
		}
	}
	if (*proj_y < YMIN) {
		if (*proj_y + proj_size->y < YMIN) {
			return false;
		} else {
			i32 y_above_screen = -(*proj_y - YMIN);
			y_above_screen &= 0xFFFF;
			proj_size->y -= (u8) y_above_screen;
			*image_data += saved_sprite_width * y_above_screen;
			*proj_y = YMIN;
		}
	}
	i32 proj_bottom = *proj_y + proj_size->y;
	if (proj_bottom > YMAX) {
		if (*proj_y >= YMAX) {
			return false;
		} else {
			proj_size->y = YMAX - *proj_y;
			return true;
		}
	} else {
		return true;
	}
}

//16A9D
void DrawSpriteNormal256(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
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

//16A24
void DrawSpriteNormal(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	//stub
}

//16B08
void DrawSpriteFlipNormal(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
	//stub
}

//16B88
void DrawSpriteFlipNormal256(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
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

//16BEA
void DrawSpriteColorNormal(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buffer /*edi*/, u8* image_data /*esi*/) {
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

//1D560
void set_proj_center(i16 x, i16 y) {
	PROJ_CENTER_X = x;
	PROJ_CENTER_Y = y;
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
	i32 temp = ((256*256) / (scale + 256)) * (par_1 - PROJ_CENTER_X);
	return ((temp / 256) + PROJ_CENTER_X);
}

//1D614
i32 get_proj_y(i32 scale, i32 par_1) {
	//NOTE: needs checking
	i32 temp = ((256*256) / (scale + 256)) * (par_1 - PROJ_CENTER_Y);
	return ((temp / 256) + PROJ_CENTER_Y);
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
					draw_func = DrawSpriteFlipNormalEtX;
				} else {
					draw_func = DrawSpriteNormalEtX;
				}

				i32 proj_x = get_proj_x(obj->scale, x);
				i32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
				i32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
				vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
				u8 sprite_field_A = sprite->field_A >> 4;

				u8* image_data = obj->img_buffer + sprite->offset_in_atlas;

				draw_func(proj_x /*eax*/, sprite_field_A /*edx*/, proj_y /*ebx*/, proj_size /*ecx*/, /*edi*/ &global_game->draw_buffer, image_data /*esi*/);

			}
		}

		++layer;
	}

}

//5C9F0
void set_default_Bloc_clipping(void) {
	Bloc_lim_H1 = 0;
	Bloc_lim_H2 = 200;
	Bloc_lim_W1 = 4;
	Bloc_lim_W2 = 320;
}

//55EE4
void SET_X_SPEED(obj_t* obj) {
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
		if (obj->type == obj_23_rayman && RayEvts.tiny) {
			xspeed /= 2;
		}
	}
	obj->xspeed = (i16)xspeed;
}

//567AC
void DO_ANIM(obj_t* obj) {
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
            PlaySnd(eta->sound_index, obj->obj_index);
		}
	}
	if (obj->anim_frame >= anim->frame_count || obj->anim_frame == 255) {
		// animation ended
		obj->main_etat = eta->next_etat;
		obj->sub_etat = eta->next_subetat;
		eta = get_eta(obj);
		obj->anim_index = eta->anim_index;
		anim = obj->animations + obj->anim_index;
		if ((obj->type == obj_23_rayman && (ray_old_etat == 2 || ray_old_etat == 6)) ||
		    (obj->sub_etat == 61 && ray_old_subetat == 61 && ray_old_etat == 0)) {
			if (ray.timer > 60 && !RayEvts.squashed) {
				ray.timer = 60;
			}
		}
		if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
			obj->anim_frame = anim->frame_count - 1;
		} else {
			obj->anim_frame = 0;
		}
		if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->obj_index);
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

//1D074
bool EOA(obj_t* obj) {
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

//3CB54
void SYNCHRO_LOOP(scene_func_t scene_func) {
	i16 scene_ended = 0;
	do {
		advance_frame();
		do_fade(&fade_source_palette, global_game->draw_buffer.pal);
		u32 timer = 0;
		scene_ended = scene_func(timer);
	} while(!scene_ended);
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
void POINTEUR_BOUTONS_OPTIONS_BIS(void) {
	// stub
}


//42790
bool JoystickPresent(void) {
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
void check_key_table(void) {
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
void LOAD_CONFIG(void) {
	if (LoadOptionsOnDisk()) {
		LoadLanguageTxt(0); // English
		if (xpadmax == -1) {
			GameModeVideo = 1;
			P486 = 0;
		}
		if (FondAutorise == 2) {
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
			LoadBnkFile(&base_snd8b_data, 2);
			InitSnd();
		}

		if (JoystickPresent()) {
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
			LoadBnkFile(&base_snd8b_data, 2);
			InitSnd();
		}
		if (JoystickPresent()) {
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
void InitMemoryVariable(void) {
	P486 = 0;
	NormalModeAutorise = 1;
	JumelleEffetAutorise = 1;
	CarteSonAutorisee = 1;
	SonLimite = 0;
	ToutSpriteAutorise = 1;
	FondAutorise = 1;
	TailleMainMemTmp = 0x22000;
	TailleMainMemWorld = 0xF4C00;
	TailleMainMemLevel = 0x87C00;
	TailleMainMemSprite = 0xDF400;
	TailleMainMemFix = 0x4D800;
}

//554EC
void init_allowed_time(void) {
	for (i32 i = 0; i < 192; ++i) {
		allowed_time[i] = -2;
	}
	i16* allowed_time_world = allowed_time;
	allowed_time_world[17] = 30; //magician (swinging plums)
	allowed_time_world[18] = 45; //magician (flower platforms)
	allowed_time_world[19] = 25; //magician (grass platforms)
	allowed_time_world[20] = 35; //magician (low flower platforms)
	allowed_time_world += 32;
	allowed_time_world[16] = 40; //magician (slippery platforms)
	allowed_time_world[17] = 25; //magician (rotating platforms)
	allowed_time_world += 32;
	allowed_time_world[11] = 45; //magician (disappearing clouds)
	allowed_time_world[12] = 20; //magician (bouncing clouds)
	allowed_time_world += 32;
	allowed_time_world[11] = 35; //magician (parkour)
	allowed_time_world[12] = 50; //magician (bouncy erasers)
	allowed_time_world += 32;
	allowed_time_world[11] = 35; //magician (ring parkour)
}

//23B8C
void init_bonus_perfect(void) {
	for (i32 i = 0; i < 24; ++i) {
		bonus_perfect[i] = 0;
	}
}

//25BB0
i32 blocs1_empty(i32 a1, i32 a2) {
	return a2 + 1;
}

//25BB4
i32 blocs1_right_45(i32 a1, i32 a2) {
	return 15 - a1;
}

//25BC0
i32 blocs1_left_45(i32 a1, i32 a2) {
	return a1;
}

//25BC4
i32 blocs1_right1_30(i32 a1, i32 a2) {
	return 15 - (a1 >> 1);
}

//25BD4
i32 blocs1_right2_30(i32 a1, i32 a2) {
	return 7 - (a1 >> 1);
}

//25BE4
i32 blocs1_left1_30(i32 a1, i32 a2) {
	return a1 >> 1;
}

//25BE8
i32 blocs1_left2_30(i32 a1, i32 a2) {
	return (a1 >> 1) + 8;
}

//25BF8
i32 blocs2_empty(i32 a1, i32 a2) {
	return 16;
}

//25BF0
i32 blocs3_empty(i32 a1, i32 a2) {
	return a2;
}
#define blocs1_liane blocs3_empty

//25BF4
i32 blocs4_empty(i32 a1, i32 a2) {
	return 0;
}
#define blocs1_hor blocs4_empty

//25C7C
void init_calcbloc_func(void) {
	for (i32 i = 0; i < 31; ++i) {
		switch(i) {
			default: {
				calcbloc1[i] = blocs1_empty;
				calcbloc2[i] = blocs2_empty;
				calcblocfloor[i] = blocs3_empty;
				calcblocrecal[i] = blocs4_empty;
			} break;
			case 2:
			case 18: {
				calcbloc1[i] = blocs1_right_45;
				calcbloc2[i] = blocs1_right_45;
				calcblocfloor[i] = blocs1_right_45;
				calcblocrecal[i] = blocs1_right_45;
			} break;
			case 3:
			case 19: {
				calcbloc1[i] = blocs1_left_45;
				calcbloc2[i] = blocs1_left_45;
				calcblocfloor[i] = blocs1_left_45;
				calcblocrecal[i] = blocs1_left_45;
			} break;
			case 4:
			case 20: {
				calcbloc1[i] = blocs1_right1_30;
				calcbloc2[i] = blocs1_right1_30;
				calcblocfloor[i] = blocs1_right1_30;
				calcblocrecal[i] = blocs1_right1_30;
			} break;
			case 5:
			case 21: {
				calcbloc1[i] = blocs1_right2_30;
				calcbloc2[i] = blocs1_right2_30;
				calcblocfloor[i] = blocs1_right2_30;
				calcblocrecal[i] = blocs1_right2_30;
			} break;
			case 6:
			case 22: {
				calcbloc1[i] = blocs1_left1_30;
				calcbloc2[i] = blocs1_left1_30;
				calcblocfloor[i] = blocs1_left1_30;
				calcblocrecal[i] = blocs1_left1_30;
			} break;
			case 7:
			case 23: {
				calcbloc1[i] = blocs1_left2_30;
				calcbloc2[i] = blocs1_left2_30;
				calcblocfloor[i] = blocs1_left2_30;
				calcblocrecal[i] = blocs1_left2_30;
			} break;
			case 9:
			case 14:
			case 15:
			case 30: {
				calcbloc1[i] = blocs1_hor;
				calcbloc2[i] = blocs1_hor;
				calcblocfloor[i] = blocs1_hor;
				calcblocrecal[i] = blocs1_hor;
			} break;
			case 12: {
				calcbloc1[i] = blocs1_liane;
				calcbloc2[i] = blocs2_empty;
				calcblocfloor[i] = blocs3_empty;
				calcblocrecal[i] = blocs4_empty;
			}
		}
	}
}

//59900
void INIT_RAY_BEGIN(void) {
	RayEvts.super_helico = 0;
	RayEvts.magicseed = 0;
	RayEvts.tiny = 0;
	ray_max_hitp = 2;
	status_bar.num_wiz = 0;
	fin_continue = 0;
	ray.flags &= ~obj_flags_1;
}

//6BF60
i16 myRand(i16 max) {
	i32 r = RandomIndex + 1;
	if (r >= 256) {
		r = 0;
	}
	RandomIndex = r;
	return (i16)(RandArray[r] % (max + 1));
}

//6BF98
void MakeMyRand(void) {
	srand(time(NULL));
	for (u32 i = 0; i < COUNT(RandArray); ++i) {
		i16 r = (i16)(rand() & 0x7FFF); // original: and ah, 0EFh ; does not clear the sign bit, maybe a bug?
		RandArray[i] = r;
	}
	RandomIndex = 0;
}

//5A5B4
void INIT_MOTEUR_BEGIN(void) {
	init_allowed_time();
	init_bonus_perfect();
	init_calcbloc_func();
	new_level = 1;
	new_world = 1;
	status_bar.lives = 3;
	ray.hitp = 2;
	gele = 0;
	departlevel = 1;
	poing.sub_etat = 1;
	nb_continue = 9;
	You_Win = 0;
	set_proj_center(160, 170);
	INIT_RAY_BEGIN();
	scroll_x = -1;
	scroll_y = -1;
	special_ray_mov_win_x_left = 0;
	special_ray_mov_win_x_right = 0;
	fin_de_rayman = 0;
	NumDemo = myRand(5);
	if (DemoRecordWorld[NumDemo] == 5 && DemoRecordMap[NumDemo] == 10) {
		++NumDemo;
		if (NumDemo == COUNT(DemoRecordWorld)) {
			NumDemo = 0;
		}
	}
	First_Hit = 1;
	First_Menu = 1;
	time_left = -2;
	life_becoz_wiz = 0;
	dontdoit = 0;
	RunTimeDemo = 1800;
}

//3D9D4
void InitTextMode(void) {
	//textmode();
	ModeVideoActuel = 255;
}

//4368C
i32 ChangeJumelleVariable(void) {
	LargeurJumelle = (7 * RayonJumelle) / 2;
	i32 var1 = LargeurJumelle / 2;
	JumelleXMin = JumellePosX - var1;
	i32 result = ((2 * RayonJumelle) / 2);
	JumelleYMin = JumellePosY - result;
	scroll_start_x = (i16)(LargeurJumelle + 2);
	scroll_start_y = (i16)(result + 2);
	scroll_end_x = (i16)(WidthNworld - var1 - 4);
	HauteurJumelle = 2 * RayonJumelle;
	scroll_end_y = (i16)(HeightNworld - result - 4);
	return result;
}

//437FC
i16 PrepareJumelleZoom(void) {
	return 0;
	//stub
}

//43038
i16 DefaultJumelleVariable(void) {
	if (JumelleEffetAutorise) {
		JumelleZoomActif = 1;
	} else {
		JumelleZoomActif = 0;
	}
	ChangeJumelleSizeOK = 3;
	RayonJumelle = 25;
	LargeurJumelle = 87;
	HauteurJumelle = 50;
	JumellePosX = 160;
	JumellePosY = 100;
	PositionJumelleX16 = 2560;
	PositionJumelleY16 = 1600;
	ModeAutoJumelle = 0;
	PositionStageNameCalcule = 0;
	i16 result = (i16)ChangeJumelleVariable();
	if (JumelleEffetAutorise) {
		JumelleZoomAmp = -26 * RayonJumelle + 32150;
		result = PrepareJumelleZoom();
	}
	return result;
}

//42EC4
i16 InitMatriceJumelle(void) {
	DistPointX = (i32*)calloc(161, sizeof(i32));
	DistPointY = (i32*)calloc(65, sizeof(i32));
	ExpPoint = (i32*)calloc(5120, sizeof(i32));
	JumelleZoomDef = (i32*)calloc(29025, sizeof(i32));
	if (JumelleEffetAutorise) {
		if (DistPointX && DistPointY && ExpPoint && JumelleZoomDef) {
			for (i32 y = 0; y < 65; ++y) {
				DistPointY[y] = y * y;
			}
			// NOTE: the for loop for X is inside the Y loop in the original (probably a bug?)
			for (i32 x = 0; x < 161; ++x) {
				DistPointX[x] = x * x;
			}
			for (i32 i = 0; i < 5120; ++i) {
				ExpPoint[i] = (i32)(expf((float)(-i) / 1024.0f ) * 65536.0f);
			}
		} else {
			if (DistPointX) {
				free(DistPointX);
			}
			if (DistPointY) {
				free(DistPointY);
			}
			if (ExpPoint) {
				free(ExpPoint);
			}
			if (JumelleZoomDef) {
				free(JumelleZoomDef);
			}
			JumelleEffetAutorise = 0;
			fatal_error();
		}
	}
	return DefaultJumelleVariable();
}

//145F1
void select_display_buffer(u8* buffer) {
	//stub
}

//3D9E4
void InitModeNormalWithFrequency(u8 freq) {
	if (ModeVideoActuel == 255) {
		//InitFirstModeVideo();
		//GetVideoRegister()
	}
	if (ModeVideoActuel != 0) {
		//InitModeNormal();
		memset(DrawBufferNormal, 0, 320 * 200);
//		memset(DisplayBufferNormal, 0, 320 * 200);
		//set_vga_frequency();
		ModeVideoActuel = 0;
		//dword_CD0F4 = sub_16A18;
		//dword_CD114 = sub_16AFC;
		DrawSpriteColorNormalEtX = DrawSpriteColorNormal;
		//dword_CD11C = (int)sub_16C89;
		draw_buffer = DrawBufferNormal;
		DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
		DrawSpriteNormalEtX = DrawSpriteNormal;
//		drawflocon1NormalETX = draw_flocon1_Normal;
//		drawflocon2NormalETX = draw_flocon2_Normal;
//		drawflocon3NormalETX = draw_flocon3_Normal;
//		drawflocon4NormalETX = draw_flocon4_Normal;
//		drawflocon5NormalETX = draw_flocon5_Normal;
//		drawflocon6NormalETX = draw_flocon6_Normal;
//		drawflocon7NormalETX = draw_flocon7_Normal;
//		drawpluie4NormalETX = draw_pluie4_Normal;
//		drawpluie6NormalETX = draw_pluie6_Normal;
//		drawpluie5NormalETX = draw_pluie5_Normal;
//		drawpluie7NormalETX = draw_pluie7_Normal;
//		fplotNormalETX = fplot_Normal;
	}
}

//16EAA
void set_speaker_on(void) {
	//stub
}

//3622C
void FIRST_INIT(void) {
	init_memory(&temp_mem_buf, TailleMainMemTmp); // NOTE: need to figure out when (if ever) this gets freed?
	InitTextMode();
	freeze = 0;
	record.is_recording = 0;
	record.is_playing = 0;
	gele = 0;
	JoystickPresent();
	word_E0CD0 = 3; // inlined sub_4212C()
	DrawBufferNormal = (u8*)malloc(64064);
	EffetBufferNormal = (u8*)malloc(128000);
	if (!DrawBufferNormal || !EffetBufferNormal) {
		fatal_error();
	}
	InitMatriceJumelle();
	if (Frequence == 70) {
		VGA_FREQ = Frequence;
	} else {
		VGA_FREQ = 60;
	}
	InitModeNormalWithFrequency(VGA_FREQ);
	use_sync = 1;
	set_speaker_on();
	//sub_35F70();
	path_has_changed = 0;
}

//42134
void readinput(void) {
	// stub // get joystick input
}

//362E4
void init_divers_level_PC(u8* a1) {
	if (fin_du_jeu) {
		return;
	}
	*a1 = 255;
	readinput();
	if (record.current_offset != 0 || record.is_playing || record.is_recording) {
		return;
	}
	//sub_42490(&record);
}



//36D30
void DEPART_INIT_LOOP(void) {
	//nullsub
}

//36D34
void FIN_GAME_LOOP(void) {
	if (!fin_du_jeu) {
		DEPART_INIT_LOOP();
		return;
	}
	doneGameSave();
	block_free(main_mem_world);
	block_free(main_mem_sprite);
}

//36D8C
void FIN_DEAD_LOOP(void) {
    //nullsub
}

//6B550
void SPECIAL_INIT(void) {
	dark_phase = 0;
	if (ray_on_poelle == 1) {
		RayEvts = SauveRayEvts;
		ray_on_poelle = 0;
	}
}

//18420
void PcMain(void) {
	InitMemoryVariable();
	sprite_clipping(0, 320, 0, 200);
	WaitNSynchro(10); // added
	INIT_MOTEUR_BEGIN();
	FIRST_INIT();
	DO_UBI_LOGO();
	DO_GROS_RAYMAN();
	LOAD_CONFIG();
	//init_cheats(); // stub

	NBRE_SAVE = 3;

	// let the music finish playing while the main menu is not implemented yet :(
	while (is_ogg_playing) {
		advance_frame();
	}

	if (MusicCdActive) {
		stop_cd();
	}

	u8 v1 = 0;

	while (!fin_de_rayman) {
		DEPART_INIT_LOOP();
		INIT_WORLD_INFO();
		set_default_Bloc_clipping();
		//DO_NEW_MENUS();

		if (!fin_de_rayman && !ModeDemo && Frequence == 70) {
			//play_movie("intro.dat", 20);
		}

		if (fin_du_jeu || status_bar.lives < 0 || new_world == 0) {
			FIN_GAME_LOOP();
			if (ModeDemo) {
				//FinDemoJeu();
			}
			continue;
		}

		MakeMyRand();
		SPECIAL_INIT();
		default_sprite_clipping();
		//DO_WORLD_MAP();
		sprite_clipping(0, 320, 0, 200);
		DEPART_WORLD();
		if (!SonLimite) {
			LoadBnkWorld(num_world_choice);
		}

		while (!(fin_du_jeu || new_world == 0 || new_level == 0)) {
			WaitNSynchro(5);
			//speaker_enable();
			//DEPART_LEVEL();
			init_divers_level_PC(&v1);
			if ((GameModeVideo == 0 && num_world == 6 && num_level == 4) || get_casse_brique_active()) {
//				InitClipping();
			}

			//INIT_MOTEUR_LEVEL(new_level);
			//init_fee();
			//init_moustique();
			//InitPaletteSpecialPC();

            if (byte_CFA2A != 0) {
                fade_out(2, &rvb_plan3);
            }

            while(!(fin_du_jeu || new_level == 0 || new_world == 0)) {
                WaitNSynchro(15);
                //INIT_MOTEUR_DEAD();
                //INIT_RAY_ON_MS();
                //START_LEVEL_ANIM();
                //BackgroundOn = IsBackgroundOn();
                if (GameModeVideo == 0) {
                    default_sprite_clipping();
                    //InitModeXWithFrequency(VGA_FREQ);
                    //set_frequency_mode(Frequence);
                    //INIT_GAME_MODE_X(xmap, ymap);
                    if (P486 == 1) {
                        //sub_1268A(...);
                    }
                    During_The_Menu = 0;
                    //DO_MAIN_LOOP_PC_X();
                    During_The_Menu = 1;
                } else {
                    //InitClipping();
                    InitModeNormalWithFrequency(VGA_FREQ);
                    //set_frequency_mode(Frequence);
                    //INIT_GAME_MODE_NORMAL();
                    During_The_Menu = 0;
                    //DO_MAIN_LOOP_PC_NORMAL();
                    During_The_Menu = 1;
                    //FIN_GAME_MODE_NORMAL();
                }

                if (CarteSonAutorisee) {
                    //stop_all_snd();
                }

                InitModeNormalWithFrequency(VGA_FREQ);
                sprite_clipping(0, 320, 0, 200);
                //START_LEVEL_ANIM();
                if (ExitMenu) {
                    //INIT_CONTINUE();
                } else {
                    //DO_CONTINUE();
                    //DO_VICTOIRE();
                }
                ExitMenu = 0;
            }

            //DONE_MOTEUR_LEVEL();
            //FIN_DEAD_LOOP();
		}


		if (MusicCdActive) {
			stop_cd();
		}
		//FIN_MAP_LOOP();
		if (ModeDemo) {
			fin_du_jeu = 1;
		}
	}

	//DoEffectBloodOut();
	if (CarteSonAutorisee) {
		//stop_all_snd();
		//DigiMusicDone();
	}
	//END_GAME();

}
