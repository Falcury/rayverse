
//23A00
i16 get_allowed_time(void) {
    return allowed_time[32 * (num_world - 1) + (num_level - 1)];
}

//23A28
void calc_left_time(void) {
    if (bonus_map) {
        i16 allowed_time_in_seconds = allowed_time[32 * (num_world - 1) + (num_level - 1)];
        if (map_time == 1) {
            if (allowed_time_in_seconds == -2) {
                left_time = -2;
            } else {
                left_time = allowed_time_in_seconds * 60;
            }
        } else if (allowed_time_in_seconds != -2 && map_time > 120) {
            if (left_time != 0) {
                ray.is_active = 1;
                left_time = 60 * allowed_time_in_seconds - map_time + 120;
                if (left_time == 0) {
                    if (nb_wiz != 0) {
                        status_bar.num_wiz = nb_wiz_save;
                        departlevel = 0;
                        fix_numlevel(&ray);
                        DO_FADE_OUT();
                    }
                }
            }
        }
    }
}

//23AF8
u8 get_bonus_map_complete(i16 world, i16 lvl) {
    /* 35FB4 8015A7B4 -O2 */
    return getbit(bonus_perfect, (s16) (lvl - 1 + ((world - 1) << 5)));
}

//23B10
void set_bonus_map_complete(i16 world, i16 lvl) {
    /* 35FEC 8015A7EC -O2 */
    setbit(bonus_perfect, (s16) (lvl - 1 + ((world - 1) << 5)));
}

//23B28
void DO_WIZ_AFTER_BONUS_MAP(void) {
    /* 360C8 8015A8C8 -O2 */
    if (!bonus_map && nb_wiz_collected != 0 && horloge[4] == 0) {
        status_bar.num_wiz++;
        nb_wiz_collected--;
        if (status_bar.num_wiz >= 100) {
            status_bar.num_wiz -= 100;
            id_Cling_1up = NOVA_STATUS_BAR();
            if (id_Cling_1up == -1)
                Add_One_RAY_lives();
        }
    }
}

//23B8C
void init_bonus_perfect(void) {
    for (i32 i = 0; i < 24; ++i) {
        bonus_perfect[i] = 0;
    }
}

//23BA0
i16 do_perfect_bonus(u32 a1) {
    horloges(1);
    readinput();

    if (loop_time == 0) {
        INIT_FADE_IN();
        i32 minutes = allowed_time[(s16) (((num_world-1) * 32) + (num_level-1))] - ((left_time / 60) + 1);
        i32 seconds = (map_time - 120) % 60;
        snprintf(bonus_time_display_text, 20, "/%2d : %02d/", minutes, seconds);
    }

    if (get_bonus_map_complete(num_world, num_level)) {
        if (nb_fade == 0 && horloge[4] == 0) {
            cyclage_palette(0, 128, 191);
        }
        if (nb_fade == 0 && horloge[6] == 0) {
            cyclage_palette(0, 192, 239);
        }
        DISPLAY_FOND3();
        display_text(language_txt[157], 160, 60, 0, 5);  // /great/
        display_text(language_txt[158], 160, 110, 2, 1); // /your time:/
        display_text(bonus_time_display_text, 160, 150, 0, 5);
    } else {
        if (nb_fade == 0 && horloge[4] == 0) {
            cyclage_palette(0, 128, 191);
        }
        if (nb_fade == 0 && horloge[6] == 0) {
            cyclage_palette(0, 192, 239);
        }
        DISPLAY_FOND3();
        if (loop_time == 15) {
            obj_t* nova = allocateNOVA();
            nova->screen_x = 75;
            nova->screen_y = 50;
            dword_D0028 = nova;
            nova->is_active = 1;
            DO_ANIM(nova);
            Add_One_RAY_lives();
        }
        if (loop_time > 20) {
            DO_ANIM(dword_D0028);
            switchOff(dword_D0028);
            if (dword_D0028->is_active) {
                display2(dword_D0028);
            }
        }

    }

    bool done = false;
    if (loop_time > 500 || ((but0pressed() || but1pressed() || but2pressed() || but3pressed()) && loop_time > 50)) {
        done = true;
        fix_numlevel(&ray);
    }

    ++loop_time;
    return done;
}

//23DF0
void DO_PERFECT_BONUS_MAP(void) {
    i16 saved_loop_time = loop_time;
    bool is_mode_x = false;
    if (loop_time < 0) {
        ++loop_time;
        if (loop_time == -1) {
            departlevel = 0;
            loop_time = saved_loop_time + 2;
            if (ModeVideoActuel == MODE_X) {
                is_mode_x = true;
                InitModeNormalWithFrequency(VGA_FREQ);
            }
            init_memory(&main_mem_tmp, 64000);
            if (get_bonus_map_complete(num_world, num_level)) {
                LOAD_PERFTIME_SCREEN(main_mem_tmp);
            } else {
                LOAD_PERFECT_SCREEN(main_mem_tmp);
            }
            SAVE_PALETTE(&rvb_plan3);
            EFFACE_VIDEO();
            start_cd_gros_rayman();
            if (CarteSonAutorisee) {
                stop_all_snd();
                PlayTchatchVignette(6);
            }
            SYNCHRO_LOOP(do_perfect_bonus);
            if (CarteSonAutorisee) {
                FreeTchatchVignette();
            }
            DO_FADE_OUT();
            status_bar.num_wiz = nb_wiz_save;
            nb_wiz_save = 0;
            set_bonus_map_complete(num_world, num_level);
            if (is_mode_x) {
                InitModeXWithFrequency(VGA_FREQ);
            }
            EFFACE_VIDEO();
            RESTORE_PALETTE();
            free(main_mem_tmp);
        }
    }
}

