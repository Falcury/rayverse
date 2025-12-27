
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
        cheat_code_programmer_mode = &cheat_codes[308];
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
        cheat_code_programmer_mode = &cheat_codes[316];
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
        if (!finBosslevel.mr_dark) {
            return;
        }
        PositionStageNameCalcule = 0;
        set_casse_brique_active();
        return;
    }
    if (memcmp(text_input_buffer, cheat_code_breakout_before_victory, 9) == 0) {
        PositionStageNameCalcule = 0;
        set_casse_brique_active();
        return;
    }
    if (memcmp(text_input_buffer, cheat_code_lens, 4) == 0) {
        if (JumAmpDefMul) {
            JumAmpDefMul = 0;
            JumAmpDefPlus = 0;
        } else {
            JumAmpDefMul = 26;
            JumAmpDefPlus = 32150;
        }
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
        return;
    }
    if (memcmp(text_input_buffer, cheat_code_all_powers, 5) == 0 || memcmp(text_input_buffer, cheat_code_all_powers_alt, 7) == 0) {
        RayEvts.poing = 1;
        RayEvts.hang = 1;
        RayEvts.helico = 1;
        RayEvts.grap = 1;
        RayEvts.run = 1;
        return;
    }

    if (memcmp(text_input_buffer, cheat_code_unlock_all_levels, 7) == 0 || memcmp(text_input_buffer, cheat_code_unlock_all_levels_alt, 9) == 0) {
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

    //stub
}

//3784C
void PC_do_cheats(void) {
    //stub
}

