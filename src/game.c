


// sub_1a3f0
void display_in_game_text(const char* text, int unknown1, int x, int y, int unknown2) {
	// stub
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



// Command system


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






//inlined
void SetPalette(void) {
    //stub
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



//38400
i16 dummy_scene_func(u32 par_0) {
	return 1;
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



//145F1
void select_display_buffer(u8* buffer) {
	//stub
}


//16EAA
void set_speaker_on(void) {
	//stub
}



