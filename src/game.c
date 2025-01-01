


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

//3C4FC
void clear_palette(rgb_palette_t* palette) {
    for (i32 i = 0; i < 256*3; ++i) {
        rvb_fade[i] = ((u8*)(palette->colors))[i];
    }
}

//inlined
void SetPalette(void) {
    //stub
}

//3C54C
void start_fade_in(i16 speed) {
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


//23B8C
void init_bonus_perfect(void) {
	for (i32 i = 0; i < 24; ++i) {
		bonus_perfect[i] = 0;
	}
}



//3D9D4
void InitTextMode(void) {
	//textmode();
	ModeVideoActuel = 255;
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
//		memset(DrawBufferNormal, 0, 320 * 200);
//		memset(DisplayBufferNormal, 0, 320 * 200);
		//set_vga_frequency();
		ModeVideoActuel = 0;
		//dword_CD0F4 = sub_16A18;
		//dword_CD114 = sub_16AFC;
		DrawSpriteColorNormalEtX = DrawSpriteColorNormal;
		//dword_CD11C = (int)sub_16C89;
		draw_buffer = DrawBufferNormal->memory;
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



//18420
void PcMain(void) {
    MakeMyRand();
    InitMusic();
    input_mode = 1;
    atexit(ToDoAtExit);
    Init_Clavier();
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
		DO_NEW_MENUS();

		if (!fin_de_rayman && !ModeDemo && Frequence == 70) {
//			play_movie("intro.dat", 20);
		}

		if (fin_du_jeu || status_bar.lives < 0 || new_world == 0) {
			FIN_GAME_LOOP();
			if (ModeDemo) {
				FinDemoJeu();
			}
			continue;
		}

		MakeMyRand();
		SPECIAL_INIT();
		default_sprite_clipping();
		DO_WORLD_MAP();
		sprite_clipping(0, 320, 0, 200);
		DEPART_WORLD();
		if (!SonLimite) {
			LoadBnkWorld(num_world_choice);
		}

		while (!(fin_du_jeu || new_world == 0 || new_level == 0)) {
			WaitNSynchro(5);
//			speaker_enable();
			DEPART_LEVEL();
			init_divers_level_PC(&v1);
			if ((GameModeVideo == 0 && num_world == 6 && num_level == 4) || get_casse_brique_active()) {
				InitClipping();
			}

			INIT_MOTEUR_LEVEL(new_level);
			init_fee();
//			init_moustique();
//			InitPaletteSpecialPC();

            if (byte_CFA2A != 0) {
                fade_out(2, &rvb_plan3);
            }

            while(!(fin_du_jeu || new_level == 0 || new_world == 0)) {
                WaitNSynchro(15);
                INIT_MOTEUR_DEAD();
                INIT_RAY_ON_MS();
                START_LEVEL_ANIM();
                BackgroundOn = IsBackgroundOn();
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
                    InitClipping();
                    InitModeNormalWithFrequency(VGA_FREQ);
                    //set_frequency_mode(Frequence);
                    //INIT_GAME_MODE_NORMAL();
                    During_The_Menu = 0;
                    //DO_MAIN_LOOP_PC_NORMAL();
                    During_The_Menu = 1;
                    //FIN_GAME_MODE_NORMAL();
                }

                if (CarteSonAutorisee) {
                    stop_all_snd();
                }

                InitModeNormalWithFrequency(VGA_FREQ);
                sprite_clipping(0, 320, 0, 200);
                START_LEVEL_ANIM();
                if (ExitMenu) {
                    //INIT_CONTINUE();
                } else {
                    DO_CONTINUE();
                    DO_VICTOIRE();
                }
                ExitMenu = 0;
            }

            DONE_MOTEUR_LEVEL();
            FIN_DEAD_LOOP();
		}


		if (MusicCdActive) {
			stop_cd();
		}
		FIN_MAP_LOOP();
		if (ModeDemo) {
			fin_du_jeu = 1;
		}
	}

//	DoEffectBloodOut();
	if (CarteSonAutorisee) {
		stop_all_snd();
//		DigiMusicDone();
	}
	END_GAME();

}
