
//16F80
void InitData(void) {
    //stub
}

//17084
void DO_GROS_MOTEUR_NORMAL(u8 a1) {
    if (CarteSonAutorisee && gele != 1) {
        manage_snd(); //TODO
    }
    do_record(&record); //TODO
    if (gele) {
        DO_MOTEUR_GELE(); //TODO
    } else {
        DO_MOTEUR(); //TODO
        if (!gele) {
            DO_MOTEUR2(); //TODO
        }
    }
    get_luciole(); //TODO
    if (a1) {
        update_display_map(&BIG_MAP); //TODO
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
        ++map_time; //TODO: remove this (should get incremented in rayMayLandOnAnObject()
        if (dead_time >= 32 && !gele) {
            DoPaletteSpecialPC(); //TODO
        }
        DO_SWAP_PALETTE(); //TODO
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
        //DO_GROS_MOTEUR_NORMAL(0); //TODO
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

        DRAW_MAP(DrawBufferNormal, &BIG_MAP); //TODO
        display_grp_stars(); //TODO
        DISPLAY_ALL_OBJECTS();
        display_pix_gerbes(); //TODO
        Display_Sprite_On_Front(plan0_width, plan0_height, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2); //TODO

        if (MapAvecPluieOuNeige) {
            display_flocons_before(); //TODO
        }

        Display_and_free_luciole(DrawBufferNormal); //TODO
        clear_borders_Normal(DrawBufferNormal + Bloc_lim_W1 + (Bloc_lim_H1 << 8) - 4 + (Bloc_lim_H1 << 6),
                             Bloc_lim_H2 - Bloc_lim_H1,
                             Bloc_lim_W2 - Bloc_lim_W1 + 4); //TODO

        if (is_fee) {
            DISPLAY_TEXT_FEE(); //TODO
        } else {
            DISPLAY_FIXE(left_time);
        }

        if (JeuCracker) {
            DisplayCrackers(); //TODO
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
        DO_GROS_MOTEUR_NORMAL(1);

        bool end_of_frame = true;

        if (need_timer && use_sync && end_of_frame) {
            //stub
        }
        endsynchro();
    }

    //stub
}

//176A8
void DO_GROS_MOTEUR_X(u8* a1) {
    //stub
}

//177E0
void DO_MAIN_LOOP_PC_X(void) {
    //stub
}

//17CB8
void init_arg(i32 argc, char** argv) {
    //stub
}

//17DD8
void PrintDosInfo(void) {
    //stub
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
    AllocVariablesAutorisee();
}

//18228
void AllocVariablesAutorisee(void) {
    MessageProgram = 0;
    flocon_tab = calloc(512, sizeof(flocon_t));
    if (!flocon_tab) {
        fatal_error();
    }
    GameModeVideo = (ModeNormalAutorise == 0);
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
    //init_cheats(); // stub

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

        if (fin_du_jeu || status_bar.lives < 0 || new_world == 0) {
            FIN_GAME_LOOP();
            if (ModeDemo) {
                FinDemoJeu();
            }
            continue;
        }

        MakeMyRand(1);
        SPECIAL_INIT();
        default_sprite_clipping();
        DO_WORLD_MAP();
        sprite_clipping(0, 320, 0, 200);
        DEPART_WORLD();
        if (!SonLimite) {
            LoadBnkWorld(num_world_choice);
        }

        while (!(fin_du_jeu || new_world || new_level == 0)) {
            WaitNSynchro(5);
			speaker_enable();
            DEPART_LEVEL();
            init_divers_level_PC(&v1);
            if ((GameModeVideo == 0 && num_world == 6 && num_level == 4) || get_casse_brique_active()) {
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
                if (GameModeVideo != 0) {
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

//	DoEffectBloodOut();
    if (CarteSonAutorisee) {
        stop_all_snd();
		DigiMusicDone();
    }
    END_GAME();

}

