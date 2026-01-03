
// Note: the filename cheat.c is probably not correct (it doesn't fit the alphabetical order).

//372F8
void PC_clear_text_input_buffer(void) {
    memset(text_input_buffer, 0, sizeof(text_input_buffer));
}

//37310
void PC_init_cheats(void) {
#if 0
    // debug: decode cheat code table (see data.c)
    u8 decoded_cheat_codes[327];
    for (s32 i = 0; i < sizeof(cheat_codes); ++i) {
        decoded_cheat_codes[i] = cheat_codes[i] ^ 0x96;
    }

    u8 letters[] = {SC_A, SC_B, SC_C, SC_D, SC_E, SC_F, SC_G, SC_H, SC_I, SC_J, SC_K, SC_L, SC_M, SC_N, SC_O,
                    SC_P, SC_Q, SC_R, SC_S, SC_T, SC_U, SC_V, SC_W, SC_X, SC_Y, SC_Z};
    u8 numbers[] = {SC_0, SC_1, SC_2, SC_3, SC_4, SC_5, SC_6, SC_7, SC_8, SC_9};

    s32 num_lines = (327 / 10) + 1;
    for (s32 line = 0; line < num_lines; ++line) {
        s32 i = line * 10;
        s32 line_len = (327 - i >= 10) ? 10 : 327 - i;
        for (s32 j = 0; j < line_len; ++j) {
            u8 c = decoded_cheat_codes[i+j];
            bool printed = false;
            for (s32 k = 0; k < COUNT(letters); ++k) {
                if (c == letters[k]) {
                    printf("_(SC_%c), ", 'A'+k);
                    printed = true;
                    break;
                }
            }
            if (!printed) {
                for (s32 k = 0; k < COUNT(numbers); ++k) {
                    if (c == numbers[k]) {
                        printf("_(SC_%c), ", '0'+k);
                        printed = true;
                        break;
                    }
                }
            }
            if (!printed && c == SC_TAB) {
                printf("_(SC_TAB), ");
                printed = true;
            }
            if (!printed && c == SC_BACKSPACE) {
                printf("_(SC_BACKSPACE), ");
                printed = true;
            }
            if (!printed && c == SC_SPACE) {
                printf("_(SC_SPACE), ");
                printed = true;
            }
            if (!printed) {
                printf("_(0x%x), ", c);
//                printf("%d, ", cheat_codes[i+j]);
            }
        }
        putc('\n', stdout);
    }
#endif


    if (language == 1 /* french keyboard layout */) {
        cheat_code_breakout = &cheat_codes[84];
        cheat_code_breakout_before_victory = cheat_codes;
        cheat_code_unlock_all_levels = &cheat_codes[94];
        cheat_code_level_select = &cheat_codes[18];
        cheat_code_lens = &cheat_codes[34];
        cheat_code_freq100 = &cheat_codes[42];
        cheat_code_freq80 = &cheat_codes[54];
        cheat_code_99_lives = &cheat_codes[108];
        cheat_code_5hp = &cheat_codes[134];
        cheat_code_10_wiz = &cheat_codes[166];
        cheat_code_all_powers = &cheat_codes[124];
        cheat_code_golden_fist = &cheat_codes[150];
        cheat_code_free_movement = &cheat_codes[66];
        cheat_code_win_map = &cheat_codes[178];
        cheat_code_breakout_alt = &cheat_codes[190];
        cheat_code_unlock_all_levels_alt = &cheat_codes[206];
        cheat_code_99_lives_alt = &cheat_codes[224];
        cheat_code_5hp_alt = &cheat_codes[256];
        cheat_code_10_wiz_alt = &cheat_codes[286];
        cheat_code_all_powers_alt = &cheat_codes[242];
        cheat_code_golden_fist_alt = &cheat_codes[270];
        cheat_code_win_map_alt = &cheat_codes[296];
        cheat_code_programmer_message = &cheat_codes[308];
    } else /* regular keyboard layout */{
        cheat_code_breakout = &cheat_codes[89];
        cheat_code_breakout_before_victory = &cheat_codes[9];
        cheat_code_unlock_all_levels = &cheat_codes[101];
        cheat_code_level_select = &cheat_codes[26];
        cheat_code_lens = &cheat_codes[38];
        cheat_code_freq100 = &cheat_codes[48];
        cheat_code_freq80 = &cheat_codes[60];
        cheat_code_99_lives = &cheat_codes[116];
        cheat_code_5hp = &cheat_codes[142];
        cheat_code_10_wiz = &cheat_codes[172];
        cheat_code_all_powers = &cheat_codes[129];
        cheat_code_golden_fist = &cheat_codes[158];
        cheat_code_free_movement = &cheat_codes[75];
        cheat_code_win_map = &cheat_codes[184];
        cheat_code_breakout_alt = &cheat_codes[198];
        cheat_code_unlock_all_levels_alt = &cheat_codes[215];
        cheat_code_99_lives_alt = &cheat_codes[233];
        cheat_code_5hp_alt = &cheat_codes[263];
        cheat_code_10_wiz_alt = &cheat_codes[291];
        cheat_code_all_powers_alt = &cheat_codes[249];
        cheat_code_golden_fist_alt = &cheat_codes[278];
        cheat_code_win_map_alt = &cheat_codes[302];
        cheat_code_programmer_message = &cheat_codes[316];
    }

}

//37528
void PC_do_cheats_in_world_map(void) {
    if (!need_check_cheats) {
        return;
    }
    need_check_cheats = false;
    if (!dans_la_map_monde || JeuCracker) {
        return;
    }
    if (memcmp(text_input_buffer, cheat_code_breakout, 5) == 0 || memcmp(text_input_buffer, cheat_code_breakout_alt, 8) == 0) {
        if (finBosslevel.mr_dark) {
            PositionStageNameCalcule = 0;
            set_casse_brique_active();
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_breakout_before_victory, 9) == 0) {
        PositionStageNameCalcule = 0;
        set_casse_brique_active();
    }
    else if (memcmp(text_input_buffer, cheat_code_lens, 4) == 0) {
        if (JumAmpDefMul) {
            JumAmpDefMul = 0;
            JumAmpDefPlus = 0;
        } else {
            JumAmpDefMul = 26;
            JumAmpDefPlus = 32150;
        }
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
    }
    else if (memcmp(text_input_buffer, cheat_code_all_powers, 5) == 0 || memcmp(text_input_buffer, cheat_code_all_powers_alt, 7) == 0) {
        RayEvts.poing = 1;
        RayEvts.hang = 1;
        RayEvts.helico = 1;
        RayEvts.grap = 1;
        RayEvts.run = 1;
    }
    else if (memcmp(text_input_buffer, cheat_code_unlock_all_levels, 7) == 0 || memcmp(text_input_buffer, cheat_code_unlock_all_levels_alt, 9) == 0) {
        ALL_WORLD = 0;
        for (s32 i = 0; i < COUNT(t_world_info); ++i) {
            world_info_t* wi = t_world_info + i;
            if ((wi->state & 1) == 0 && wi->world != world_6_cake) {
                wi->state |= 4;
                chemin_percent = 0;
            }
        }
        // This cheat will normally not unlock Mr Dark's Dare.
        // However, there's an exception: if you are standing on Crystal Palace, Mr Dark's Dare *WILL* unlock!
        if (num_world_choice == 14) {
            for (s32 i = 0; i < COUNT(t_world_info); ++i) {
                world_info_t* wi = t_world_info + i;
                if ((wi->state & 1) == 0) {
                    wi->state |= 4;
                    chemin_percent = 0;
                }
            }
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_level_select, 8) == 0) {
        ALL_WORLD = 1;
    }
    else if (memcmp(text_input_buffer, cheat_code_freq100, 6) == 0) {
        if (Frequence != 100) {
            if (Frequence != 80) {
                saved_frequence = Frequence;
            }
            Frequence = 80;
            NewFrequency(80);
        } else {
            Frequence = saved_frequence;
            NewFrequency(saved_frequence);
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_freq80, 6) == 0) {
        if (Frequence != 80) {
            if (Frequence != 100) {
                saved_frequence = Frequence;
            }
            Frequence = 100;
            NewFrequency(100);
        } else {
            Frequence = saved_frequence;
            NewFrequency(saved_frequence);
        }
    }
    // Added: debug cheat "debug" (enables debug mode)
    else if (memcmp(text_input_buffer, cheat_code_debug, 5) == 0) {
        is_debug_mode = !is_debug_mode;
        printf("Debug cheats %s\n", is_debug_mode ? "on" : "off");
    }
}

//3784C
void PC_do_cheats(void) {
    if (!need_check_cheats) {
        return;
    }
    need_check_cheats = false;
    if (!RaymanDansUneMapDuJeu || num_world == world_6_cake || JeuCracker || ray_mode == MODE_5_CASSE_BRIQUE) {
        return;
    }

    if (memcmp(text_input_buffer, cheat_code_99_lives, 8) == 0 || memcmp(text_input_buffer, cheat_code_99_lives_alt, 9) == 0) {
        status_bar.lives = 99;
    }
    else if (memcmp(text_input_buffer, cheat_code_5hp, 8) == 0 || memcmp(text_input_buffer, cheat_code_5hp_alt, 7) == 0) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
    }
    else if (memcmp(text_input_buffer, cheat_code_all_powers, 5) == 0 || memcmp(text_input_buffer, cheat_code_all_powers_alt, 7) == 0) {
        if (!RayEvts.squashed) {
            RayEvts.poing = 1;
            RayEvts.hang = 1;
            RayEvts.helico = 1;
            RayEvts.grap = 1;
            RayEvts.run = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_golden_fist, 8) == 0 || memcmp(text_input_buffer, cheat_code_golden_fist_alt, 5) == 0) {
        if (!RayEvts.squashed && RayEvts.poing) {
            poing_obj->init_sub_etat = 12;
            poing.sub_etat = poing_obj->init_sub_etat;
            save1.poing_sub_etat = poing_obj->init_sub_etat;
            poing_obj->sub_etat = poing_obj->init_sub_etat;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_10_wiz, 6) == 0 || memcmp(text_input_buffer, cheat_code_10_wiz_alt, 5) == 0) {
        status_bar.num_wiz = (status_bar.num_wiz + 10 < 99) ? status_bar.num_wiz + 10 : 99;
    }
    else if (memcmp(text_input_buffer, cheat_code_win_map, 6) == 0 || memcmp(text_input_buffer, cheat_code_win_map_alt, 6) == 0) {
        if (ray.main_etat == 0 && ray.speed_x == 0 && ray.speed_y == 0 && ray.follow_id == -1) {
            fin_boss = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_free_movement, 9) == 0) {
        ray.follow_id = -1;
        ray_mode = -ray_mode;
        dead_time = 64;
        set_main_and_sub_etat(&ray, 2, 2);
    }
    else if (memcmp(text_input_buffer, cheat_code_programmer_message, 9) == 0) {
        MessageProgram = 1 - MessageProgram;
    }
    // Added: debug cheat "debug" (enables debug mode)
    else if (memcmp(text_input_buffer, cheat_code_debug, 5) == 0) {
        is_debug_mode = !is_debug_mode;
        printf("Debug cheats %s\n", is_debug_mode ? "on" : "off");
    }

}


//(newly added)
void rayverse_draw_white_outline(s16 x, s16 y, s16 w, s16 h) {
    x -= xmap - 8;
    y -= ymap;

    // bounds checks
    if (x < XMIN) {
        w += x;
        x = XMIN;
    }
    if (y < YMIN) {
        h += y;
        y = YMIN;
    }
    if (x + w > XMAX) {
        w = XMAX - x;
    }
    if (y + h > YMAX) {
        h = YMAX - y;
    }
    // Draw white outline
    if (w > 0 && h > 0) {
        u8* dest = draw_buffer + SCREEN_WIDTH * y + x;
        u8 color = 255;
        memset(dest, color, w);
        for (s32 j = 1; j < h - 1; ++j) {
            dest += SCREEN_WIDTH;
            dest[0] = color;
            dest[w - 1] = color;
        }
        dest += SCREEN_WIDTH;
        memset(dest, color, w);
    }
}

//(newly added)
void rayverse_do_debug_cheats(void) {
    if (is_debug_mode) {
        // debug cheat: full hit points
        if (TOUCHE(SC_F2)) {
            status_bar.max_hitp = 4;
            ray.hit_points = 4;
        }
        // DEBUG: display ray main and sub etat
        if (TOUCHE(SC_F3)) {
            Touche_Enfoncee[SC_F3] = false;
            need_display_debug_info = !need_display_debug_info;
        }
        if (need_display_debug_info) {
            // display Rayman's main and sub etat at the top of the screen
            char debug_text[64];
            snprintf(debug_text, 64, "%d : %d", ray.main_etat, ray.sub_etat);
            display_text(debug_text, 132, 26, 1, 0);

            // display basic info on the selected object
            if (debug_obj_id != -1 && debug_obj_id < level.nb_objects) {
                obj_t* obj = level.objects + debug_obj_id;
                u8 color = 0;
                s32 line = 0;
                snprintf(debug_text, 64, "%d : type %d", obj->id, obj->type);
                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
                snprintf(debug_text, 64, "etat %d : %d", obj->main_etat, obj->sub_etat);
                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
                snprintf(debug_text, 64, "hitp %d", (u8)obj->hit_points);
                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
//                snprintf(debug_text, 64, "cmd %d", obj->cmd);
//                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
//                snprintf(debug_text, 64, "coll %d %d %d %d %d", obj->btypes[0], obj->btypes[1], obj->btypes[2], obj->btypes[3], obj->btypes[4]);
//                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
                snprintf(debug_text, 64, "x %d y %d", obj->x, obj->y);
                display_text(debug_text, 20, 50 + line * 15, 2, color); ++line;
            }
        }
        // DEBUG: skip level
        if (TOUCHE(SC_F4)) {
            Touche_Enfoncee[SC_F4] = false;
            DO_FADE_OUT();
            ChangeLevel();
        }
        // DEBUG: reset all bosses
        if (TOUCHE(SC_F5)) {
            Touche_Enfoncee[SC_F5] = false;
            memset(&finBosslevel, 0, sizeof(finBosslevel));
            printf("Bosses have been reset\n");
        }
        // DEBUG: free movement
        if (TOUCHE(SC_F7)) {
            Touche_Enfoncee[SC_F7] = false;
            ray.follow_id = -1;
            ray_mode = -ray_mode;
            dead_time = 64;
            set_main_and_sub_etat(&ray, 2, 2);
        }
        // DEBUG: freeze screen
        if (TOUCHE(SC_F8)) {
            Touche_Enfoncee[SC_F8] = false;
            for (;;) {
                advance_frame();
                if (TOUCHE(SC_F8)) {
                    Touche_Enfoncee[SC_F8] = false;
                    break;
                }
            }
        }
        // DEBUG: show collisions
        static bool show_collisions = false;
        if (TOUCHE(SC_F9)) {
            Touche_Enfoncee[SC_F9] = false;
            show_collisions = !show_collisions;
        }
        if (show_collisions) {
            rayverse_draw_white_outline(ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h);
            for (s16 i = 0; i < actobj.num_active_objects; ++i) {
                obj_t* obj = &level.objects[actobj.objects[i]];
                if (!(flags[obj->type] & flags0_4_no_collision) && (get_eta(obj)->flags & 0x20)) {
                    CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, obj);

                    s16 spr_x;
                    s16 spr_y;
                    s16 spr_w;
                    s16 spr_h;
                    if (obj->hit_sprite == 0xFD) {
                        // TODO
//                    return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);
                    } else if (obj->hit_sprite < 0xFD) {
                        if (GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h)) {
                            rayverse_draw_white_outline(spr_x, spr_y, spr_w, spr_h);
                        }
                    } else {
                        s16 unk_h = 0;
                        s16 unk_w = 0;
                        s16 unk_y = 0;
                        s16 unk_x = 0;
                        if (obj->zdc != 0) {
                            s16 nb_zdc = get_nb_zdc(obj);
                            if (num_world == 1 && (
                                            ((obj->type == TYPE_MOSKITO || obj->type == TYPE_MOSKITO2) &&
                                             !(obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)) ||
                                            (obj->type == TYPE_BADGUY3 &&
                                             obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)
                            )) {
                                nb_zdc--;
                            }

                            for (s32 j = 0; j < nb_zdc; j++) {
                                zdc_t* cur_zdc = get_zdc(obj, j);
                                if (!(cur_zdc->flags & 4) || obj->type == TYPE_POING) {
                                    if (cur_zdc->flags & 2) {
                                        anim_t* cur_anim = &obj->animations[obj->anim_index];
                                        anim_layer_t* cur_layer = &cur_anim->layers[(cur_anim->layers_per_frame & 0x3FFF) * obj->anim_frame];
                                        if (cur_layer[cur_zdc->sprite].sprite_index != 0) {
                                            GET_SPRITE_POS(obj, cur_zdc->sprite, &unk_x, &unk_y, &unk_w, &unk_h);
                                            if (obj->flags.flip_x)
                                                unk_x += unk_w - cur_zdc->width - cur_zdc->x_pos;
                                            else
                                                unk_x += cur_zdc->x_pos;

                                            unk_y += cur_zdc->y_pos;
                                            unk_w = cur_zdc->width;
                                            unk_h = cur_zdc->height;
                                        }
                                    } else {
                                        unk_x = obj->x + cur_zdc->x_pos;
                                        unk_y = obj->y + cur_zdc->y_pos;
                                        unk_w = cur_zdc->width;
                                        unk_h = cur_zdc->height;
                                        /* TODO: can't take << 1 or * 2??? */
                                        if (obj->flags.flip_x)
                                            unk_x = (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) - (unk_x + unk_w);
                                    }
                                }
                                rayverse_draw_white_outline(unk_x, unk_y, unk_w, unk_h);
                            }
                        } else {
                            if (obj->hit_sprite == 0xFE) {
                                GET_OBJ_ZDC(obj, &unk_x, &unk_y, &unk_w, &unk_h);
                                if (obj->flags.flip_x) {
                                    unk_x = (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) - (unk_x + unk_w);
                                }
                                rayverse_draw_white_outline(unk_x, unk_y, unk_w, unk_h);
                            } else if (obj->hit_sprite == 0xFF) {
                                s16 layers_count = obj->animations[obj->anim_index].layers_per_frame & 0x3FFF;
                                for (s32 j = 0; j < layers_count; j++) {
                                    if (in_coll_sprite_list(obj, j)) {
                                        GET_SPRITE_ZDC(obj, j, &unk_x, &unk_y, &unk_w, &unk_h);
                                    }
                                    rayverse_draw_white_outline(unk_x, unk_y, unk_w, unk_h);
                                }
                            }
                        }


                    }

                }
            }
        }
    }


}
