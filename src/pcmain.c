
//16F80
void InitData(void) {
    //stub
}

//17084
void DO_GROS_MOTEUR_NORMAL(void) {
    //stub
}

//170E0
void DO_MAIN_LOOP_PC_NORMAL(void) {
    //stub
}

//176A8
void DO_GROS_MOTEUR_X(void) {
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
    NormalModeAutorise = 1;
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
}

//18228
void AllocVariablesAutorisee(void) {
    //stub
}

//18290
void AfficheEntente(void) {
    //stub
}

//1836C
int main_Ray(int argc, char** argv) {
    //unlink("C:\\raytmp.tmp");
    AfficheEntente();
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

        MakeMyRand();
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
                    INIT_GAME_MODE_NORMAL();
                    During_The_Menu = 0;
                    DO_MAIN_LOOP_PC_NORMAL();
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

