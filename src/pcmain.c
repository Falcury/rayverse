
//16F80
void InitData(void) {
    print_once("Not implemented: InitData"); //stub
}

//17084
void DO_GROS_MOTEUR_NORMAL(u8 need_update_display_map) {
    if (CarteSonAutorisee && gele != 1) {
        manage_snd(); //TODO
    }
    do_record(&record); //TODO
    if (gele) {
        DO_MOTEUR_GELE();
    } else {
        DO_MOTEUR();
        if (!gele) {
            DO_MOTEUR2(); //TODO
        }
    }
    get_luciole(); //TODO
    if (need_update_display_map) {
        update_display_map(&BIG_MAP);
    }
}

//170E0
void DO_MAIN_LOOP_PC_NORMAL(u8* a1) {
    RaymanDansUneMapDuJeu = 1;
    DoFirstFlocons();
    fin_continue = 0;
    MapTimePause = 0;
    ProchainEclair = 0;
    numero_palette_special = 0;
    if (MusicCdActive) {
        TestCdLoop();
    }

    while (dead_time != 0 && !new_level && !new_world && ModeDemo != 2) {
        speaker_enable();
        if (use_sync) {
            synchro();
        }
        if (need_timer) {
            clock_ticks();
        }
        /*
        if (need_timer) {
            ticks += word_CF7E6; // (?)
        }
        */
        fades();
        if (dead_time >= 32 && !gele) {
            DoPaletteSpecialPC(); //TODO
        }
        DO_SWAP_PALETTE();
        if (fixon || fixontemp) {
            if (Mode_Pad == 1 && Main_Control) {
                Swap_And_Test_Joystick(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
            } else {
                Swap_To_Screen(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2); //TODO
            }
        } else {
            // stub
            // TODO: work out what is happening here
            if (Mode_Pad == 1 && Main_Control) {
                Swap_And_Test_Joystick(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
            } else {
                Swap_To_Screen(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2); //TODO
            }
        }

        if (need_timer) {
            clock_ticks();
        }

        if (MusicCdActive) {
            TestCdLoop();
        }
        if (Mode_Pad == 0) {
            readinput();
        }
        // NOTE: may be repeated depending on display_mode?
        DO_GROS_MOTEUR_NORMAL(1);
        if (need_timer) {
            clock_ticks();
        }
        CLEAR_FIXE_LUCIOLE(); //TODO

        // Draw background
        if (BackgroundOn && FondAutorise == 1) {
            Calcul_Deplacement_Bande(xmap, plan0_width, plan0_height);
            Display_Back_Screen(plan0_width, plan0_height, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2);
        } else {
            memset(draw_buffer, 0, 320*200);
        }

        if (MapAvecPluieOuNeige) {
            flocon_clipping(Bloc_lim_H1, Bloc_lim_H2 - 6, Bloc_lim_W1, Bloc_lim_W2 - 4);
            display_flocons_behind(); //TODO
        }

        DRAW_MAP(DrawBufferNormal, &BIG_MAP);
        display_grp_stars();
        DISPLAY_ALL_OBJECTS();
        display_pix_gerbes();
        Display_Sprite_On_Front(plan0_width, plan0_height, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2); //TODO

        if (MapAvecPluieOuNeige) {
            display_flocons_before(); //TODO
        }

        Display_and_free_luciole(DrawBufferNormal); //TODO
        clear_borders_Normal(DrawBufferNormal + Bloc_lim_W1 + (Bloc_lim_H1 << 8) - 4 + (Bloc_lim_H1 << 6),
                             Bloc_lim_H2 - Bloc_lim_H1,
                             Bloc_lim_W2 - Bloc_lim_W1 + 4); //TODO

        if (is_fee) {
            DISPLAY_TEXT_FEE();
        } else {
            DISPLAY_FIXE(left_time);
        }

        // debug cheat: full hit points
        if (TOUCHE(SC_F2)) {
            status_bar.max_hitp = 4;
            ray.hit_points = 4;
        }
        // DEBUG: display ray main and sub etat
        static bool display_debug_etat_info;
        if (TOUCHE(SC_F3)) {
            Touche_Enfoncee[SC_F3] = false;
            is_debug_mode = !is_debug_mode;
            display_debug_etat_info = is_debug_mode;
        }
        if (display_debug_etat_info) {
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
            DO_FADE_OUT();
            ChangeLevel();
        }
        // DEBUG: reset all bosses
        if (TOUCHE(SC_F5)) {
            memset(&finBosslevel, 0, sizeof(finBosslevel));
        }
        // DEBUG: free movement
        if (TOUCHE(SC_F7)) {
            Touche_Enfoncee[SC_F7] = false;
            ray.cmd_arg_2 = -1;
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

        if (JeuCracker) {
            DisplayCrackers();
        }

        if (MessageProgram) {
            DisplayProgrammerMessage();
        }

        display_time(left_time);
        if (Vignet_To_Display) {
            DISPLAY_GAME_VIGNET(); //TODO
        }

        if (TOUCHE(SC_ESCAPE)) {
            if (nb_fade == 0 && !GoMenu) {
                if (!get_casse_brique_ON() && !ModeDemo && !gele && dead_time == 64) {
                     GoMenu = 1;
                }
            }
        }
        DO_OPTIONS_IN_GAME(); //TODO
        if (get_casse_brique_ON() && !get_casse_brique_active()) {
            set_casse_brique_ON(0);
            RESET_TOUCHE(SC_ESCAPE);
        }
        if (rightjoy() || leftjoy() || downjoy() || upjoy() || but0pressed() || but1pressed() || but2pressed() || but3pressed()) {
            MapTimePause = 0;
        }
        ++MapTimePause;
        if (MapTimePause >= 21600) {
            in_pause = 1;
        }
        if (in_pause) {
            if (!GoMenu) {
                if (fade == 65) {
                    Do_Effect_Pause();
                    MapTimePause = 0;
                } else {
                    in_pause = 0;
                }
            }
        }
        HANDLE_KEY(a1);

        // NOTE: timer shenanigans?
//        DO_GROS_MOTEUR_NORMAL(1);

        bool end_of_frame = true;

        if (need_timer && use_sync && end_of_frame) {
            //stub
        }
        endsynchro();
    }
    RaymanDansUneMapDuJeu = 0;
}

//176A8
void DO_GROS_MOTEUR_X(u8* a1) {
    print_once("Not implemented: DO_GROS_MOTEUR_X"); //stub
}

//177E0
void DO_MAIN_LOOP_PC_X(void) {
    print_once("Not implemented: DO_MAIN_LOOP_PC_X"); //stub
}

//17CB8
void init_arg(s32 argc, char** argv) {
    print_once("Not implemented: init_arg"); //stub
}

//17DD8
void PrintDosInfo(void) {
    print_once("Not implemented: PrintDosInfo"); //stub
}

//17F00
void InitMemoryVariable(void) {
    P486 = 0;
    ModeNormalAutorise = 1;
    JumelleEffetAutorise = 0; // TODO: re-enable once implemented
    CarteSonAutorisee = 1;
    SonLimite = 0;
    ToutSpriteAutorise = 1;
    FondAutorise = 1;
    TailleMainMemTmp = 0x22000;
    TailleMainMemWorld = 0xF4C00;
    TailleMainMemLevel = 0x87C00;
    TailleMainMemSprite = 0xDF400;
    TailleMainMemFix = 0x4D800;

    // NOTE: Structs and pointers might have a larger size on 64-bit systems.
    // To prevent out-of-memory errors we have to increase the memory requirements a little.
    TailleMainMemTmp *= 2;
    TailleMainMemWorld *= 2;
    TailleMainMemLevel *= 2;
    TailleMainMemSprite *= 2;
    TailleMainMemFix *= 2;

    AllocVariablesAutorisee();
}

//18228
void AllocVariablesAutorisee(void) {
    MessageProgram = 0;
    flocon_tab = (flocon_t*)calloc(512, sizeof(flocon_t));
    if (!flocon_tab) {
        fatal_error();
    }
    GameModeVideo = ModeNormalAutorise ? MODE_NORMAL : MODE_X;
    MenuCredits = 0;
}

//18290
void AfficheEntete(void) {
    InitTextMode();
    printf("RAYMAN (US) v1.21\n");
    //stub
}

//1836C
int main_Ray(int argc, char** argv) {
    //unlink("C:\\raytmp.tmp");
    AfficheEntete();
    WindowsLance = 131; //detect_windows();
    HVersionWindows = 4;
    NVersionWindows = 0;
    OptionMusicCdActive = 1;
    MusicCdActive = 1;
    init_arg(argc, argv);
    PcMain();
    return 0;
}

//18420
void PcMain(void) {
    MakeMyRand(1);
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
    INIT_EXTENSIONS();
    LOAD_CONFIG();
    PC_init_cheats();

    NBRE_SAVE = 3;

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

        while (!fin_du_jeu && status_bar.lives > -1 && new_world)  {
            MakeMyRand(1);
            SPECIAL_INIT();
            default_sprite_clipping();
            DO_WORLD_MAP();
            sprite_clipping(0, 320, 0, 200);
            DEPART_WORLD();
            if (!SonLimite) {
                LoadBnkWorld(num_world_choice);
            }

            while (!fin_du_jeu && !new_world && new_level) {
                WaitNSynchro(5);
                speaker_enable();
                DEPART_LEVEL();
                init_divers_level_PC(&v1);
                if ((GameModeVideo == MODE_NORMAL && num_world == 6 && num_level == 4) || get_casse_brique_active()) {
                    InitClipping();
                }

                INIT_MOTEUR_LEVEL(new_level);
                init_fee();
                init_moustique();
                InitPaletteSpecialPC(); //TODO

                if (byte_CFA2A != 0) {
                    fade_out(2, &rvb_plan3);
                }

                while(!(fin_du_jeu || new_level || new_world)) {
                    WaitNSynchro(15);
                    INIT_MOTEUR_DEAD();
                    INIT_RAY_ON_MS();
                    START_LEVEL_ANIM(); //TODO: implement DO_LEVEL_ANIM()
                    BackgroundOn = IsBackgroundOn();
                    if (GameModeVideo != MODE_NORMAL) {
                        default_sprite_clipping();
                        InitModeXWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        //INIT_GAME_MODE_X(xmap, ymap);
                        if (P486 == 1) {
                            //sub_1268A(...);
                        }
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_X();
                        During_The_Menu = 1;
                    } else {
                        InitClipping();
                        InitModeNormalWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        INIT_GAME_MODE_NORMAL(); //TODO
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_NORMAL(NULL);
                        During_The_Menu = 1;
                        FIN_GAME_MODE_NORMAL();
                    }

                    if (CarteSonAutorisee) {
                        stop_all_snd();
                    }

                    InitModeNormalWithFrequency(VGA_FREQ);
                    sprite_clipping(0, 320, 0, 200);
                    START_LEVEL_ANIM();
                    if (ExitMenu) {
                        INIT_CONTINUE();
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

        FIN_GAME_LOOP();
        if (ModeDemo) {
            FinDemoJeu();
        }
    }

//	DoEffectBloodOut();
    if (CarteSonAutorisee) {
        stop_all_snd();
		DigiMusicDone();
    }
    END_GAME();

}

