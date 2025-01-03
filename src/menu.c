
//48EF0
void LOAD_FND(void) {
    //stub
}

//49098
void INIT_FND(void) {
    //stub
}

//491D4
void calc_off_fnd(void) {
    //stub
}

//49308
void check_key_table(void) {
    bool valid = true;
    for (i32 i = 0; i < 7; ++i) {
        u8 key = *(tab_key[i]);
        if (key > SC_DELETE || key_descriptions_qwerty[key] == 0 || key_descriptions_azerty[key] == 0) {
            valid = false;
            break; // added
        }
    }
    if (!valid) {
        // load defaults
        *(tab_key[0]) = SC_LEFT;
        *(tab_key[1]) = SC_UP;
        *(tab_key[2]) = SC_RIGHT;
        *(tab_key[3]) = SC_DOWN;
        *(tab_key[4]) = SC_CONTROL;
        *(tab_key[5]) = SC_ALT;
        *(tab_key[6]) = SC_X;
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

//49638
void NewFrequency(u8 a1) {
    //stub
}

//49720
void InitClipping(void) {
    //stub
}

//4980C
bool IsBackgroundOn(void) {
    return true; //stub
}

//498A4
void init_key2txt(void) {
    //stub
}

//49998
void DO_NEW_MENUS(void) {
    dans_la_map_monde = 0;
    SetCompteurTrameAudio();
    current_pal_id = 0;
    choix_menu = 0;
    fin_du_jeu = 0;
    MENU_SUITE = 0;
    MENU_RETURN = 0;
    menuEtape = 0;
    if (OptionMusicCdActive) {
        MusicCdActive = options_jeu_music_enabled;
        if (!MusicCdActive) {
            stop_cd();
        }
    }
    Keyflush(); //TODO
    CalcTab();
    default_key();
    i16 NewMenusArgs = 0;
    while (!ModeDemo && !fin_du_jeu && !NewMenusArgs) {
        raj_env_sound(options_jeu_sound_volume);
        raj_env_stereo(options_jeu_is_stereo);
        raj_env_audio(options_jeu_music_enabled);
        switch(menuEtape) {
            default: break;
            case 0: {
                pINIT_SCREEN = INIT_GENERAL_CHOICE;
                pLOAD_SCREEN = LOAD_GENERAL_SCREEN;
                INIT_FADE_IN();
                DO_MENU();
            } break;
            case 3: {
                DO_SAVE_CHOICE();
                if (!MENU_RETURN) {
                    fin_du_jeu = 0;
                    NewMenusArgs = 1;
                    new_world = 1;
                }
                DO_MENU();
            } break;
            case 4: {
                pINIT_SCREEN = INIT_OPTIONS_CHOICE;
                if (pLOAD_SCREEN) {
                    byte_E4CFD = 1;
                    pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
                }
                DO_MENU();
            } break;
            case 7: {
                pINIT_SCREEN = INIT_KEY_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 8: {
                pINIT_SCREEN = INIT_PAD_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 9: {
                pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 10: {
                pINIT_SCREEN = INIT_VIDEOMODE_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
        }
        RESET_ALL_TOUCHE();
        if (MENU_RETURN) {
            menuEtape = 0;
            MENU_RETURN = 0;
        }
    }
    new_key();
    //SaveOptionsOnDisk();

}

//49B7C
void DO_OPTIONS_IN_GAME(void) {
    //stub
}

//49E98
void INIT_GENERAL_CHOICE(void) {
    pINIT_AFFICHE_SCREEN = NULL;
    pINIT_SCROLL = NULL;
    pDO_COMMANDE = DO_COMMANDE_GENERAL;
    pEND_SCREEN = END_GENERAL_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_GENERAL;
    //stub
}

//4A0DC
void AFFICHE_ECRAN_GENERAL(void) {
    //stub
}

//4A178
void DO_COMMANDE_GENERAL(void) {
    //stub
}

//4A3D8
void END_GENERAL_SCREEN(void) {
    //stub
}

//4A44C
void INIT_OPTIONS_CHOICE(void) {
    //stub
}

//4A628
void INIT_AFFICHE_ECRAN_OPTIONS(void) {
    //stub
}

//4AB34
void AFFICHE_ECRAN_OPTIONS(void) {
    //stub
}

//4AF28
void DO_COMMANDE_OPTIONS(void) {
    //stub
}

//4B334
void END_OPTIONS_SCREEN(void) {
    //stub
}

//4B3EC
void INIT_KEY_SCREEN(void) {
    //stub
}

//4B514
void INIT_AFFICHE_KEY_SCREEN(void) {
    //stub
}

//4B8A4
void AFFICHE_KEY_SCREEN(void) {
    //stub
}

//4BACC
void DO_COMMANDE_KEY(void) {
    //stub
}

//4BD50
void END_KEY_SCREEN(void) {
    //stub
}

//4BDA4
void INIT_BUTTON_SCREEN(void) {
    //stub
}

//4BE04
void INIT_AFFICHE_BUTTON_SCREEN(void) {
    //stub
}

//4BF60
void AFFICHE_BUTTON_JOYSTICK(void) {
    //stub
}

//4C138
void DO_BUTTON_COMMANDE(void) {
    //stub
}

//4C390
void END_BUTTON_COMMAND(void) {
    //stub
}

//4C3D0
void INIT_PAD_SCREEN(void) {
    //stub
}

//4C4D0
void INIT_AFFICHE_PAD_SCREEN(void) {
    //stub
}

//4C854
void AFFICHE_PAD_SCREEN(void) {
    //stub
}

//4C9D8
void DO_COMMANDE_PAD(void) {
    //stub
}

//4CD38
void END_PAD_SCREEN(void) {
    //stub
}

//4CD94
void InitParamGraphicsScreen(void) {
    //stub
}

//4D094
void INIT_GRAPHIC_SCREEN(void) {
    //stub
}

//4D178
void INIT_AFFICHE_GRAPHIC_SCREEN(void) {
    //stub
}

//4D55C
void AFFICHE_GRAPHIC_SCREEN(void) {
    //stub
}

//4DA8C
void DO_COMMANDE_GRAPHIC(void) {
    //stub
}

//4DF88
void END_GRAPHIC_SCREEN(void) {
    //stub
}

//4E0A0
void INIT_VIDEOMODE_SCREEN(void) {
    //stub
}

//4E1CC
void INIT_AFFICHE_VIDEOMODE_SCREEN(void) {
    //stub
}

//4E470
void AFFICHE_VIDEOMODE_SCREEN(void) {
    //stub
}

//4E574
void DO_COMMANDE_VIDEOMODE(void) {
    //stub
}

//4E708
void END_VIDEOMODE_SCREEN(void) {
    //stub
}

//4E790
void INIT_CALIBRATE_JOYSTICK(void) {
    //stub
}

//4E804
void AFFICHE_CALIBRATE_JOYSTICK(void) {
    //stub
}

//4E99C
void calibrate_joystick_prg(void) {
    //stub
}

//4E9E8
void AFFICHE_CENTER_JOYSTICK(void) {
    //stub
}

//4EAE8
void center_joystick_prg(void) {
    //stub
}

//4EB28
void MAIN_CALIBRATE_JOYSTICK(void) {
    //stub
}

//4F17C
i16 menu_prg(u32 a1) {
    readinput();
    DoCdRap();
    bool pad_limits_changed = false;
    if (Main_Control) {
        if (joyx0 < xpadmin) {
            xpadmin = joyx0;
            pad_limits_changed = true;
        } else if (joyx0 > xpadmax) {
            xpadmax = joyx0;
            pad_limits_changed = true;
        }
        if (joyy0 < ypadmin) {
            ypadmin = joyy0;
            pad_limits_changed = true;
        } else if (joyy0 > ypadmax) {
            ypadmax = joyy0;
            pad_limits_changed = true;
        }
    }
    if (pad_limits_changed) {
        // NOTE: this is not present in the Android version.
        update_pad_limits(&xpadmax, &xpadmin, &ypadmax, &ypadmin, &xpadcentre, &ypadcentre);
    }
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        advance_frame();
        InitPlasma(1);
        SWAP_BUFFERS();
    }
    i16 need_exit = 0;
    if (fin_du_jeu || sortie_options || MENU_RETURN || ModeDemo) {
        need_exit = 1;
    }
    if (need_exit) {
        if (pDO_COMMANDE && nb_fade != 0) {
            pDO_COMMANDE();
        }
        if (ValidButPressed() && option_exit == position) {
            sortie_options = 1;
            PlaySnd_old(69);
        }
        if (MENU_RETURN) {
            PlaySnd_old(77);
        }
    }
    return need_exit;
}

//4F2E0
void DO_INTER_MENU(void) {
    //stub
}

//4F318
void DO_MENU(void) {
    if (pINIT_SCREEN) {
        pINIT_SCREEN();
    }
    if (pINIT_AFFICHE_SCREEN) {
        pINIT_AFFICHE_SCREEN();
    }
    if (pLOAD_SCREEN) {
        pLOAD_SCREEN();
    }
    rgb_palette_t* palette;
    if ((num_world == world_1_jungle && num_level == 9) ||
            (num_world == world_2_music && num_level == 4) ||
            (num_world == world_4_image && num_level == 4) ||
            (num_world == world_5_cave && num_level == 4)
    ) {
        palette = rvb_special + numero_palette_special;
    } else {
        palette = rvb + current_pal_id;
    }
    menu_rvb = *palette;
    InitGauge(); // TODO
    InitPlasma(0); // TODO
    SYNCHRO_LOOP(menu_prg);
    if (pEND_SCREEN) {
        pEND_SCREEN();
    }
    menu_rvb = rvb[current_pal_id];
}

//4F488
void default_key(void) {
    check_key_table();
    for (i32 i = 0; i < 7; ++i) {
        tab_key_sav[i] = *(tab_key[i]);
    }
    *(tab_key[0]) = SC_LEFT;
    *(tab_key[1]) = SC_UP;
    *(tab_key[2]) = SC_RIGHT;
    *(tab_key[3]) = SC_DOWN;
    *(tab_key[4]) = SC_CONTROL;
    *(tab_key[5]) = SC_ALT;
    *(tab_key[6]) = SC_X;
}

//4F4EC
void new_key(void) {
    for (i32 i = 0; i < 7; ++i) {
        *(tab_key[i]) = tab_key_sav[i];
    }
}

//4F50C
i32 CalcSpeed(void) {
    return 0; //stub
}

//4F698
void general_init_screen(i16 a1, i16 a2, i16 a3) {
    //stub
}

//4F76C
void TestCompteur(void) {
    //stub
}

//4F7B0
void TestButtonReleased(void) {
    //stub
}

//4F800
void display_box_text_plasma(void* a1, u8 a2) {
    //stub
}

//4F878
void display_box_text_fire(void* a1) {
    //stub
}

//4F8B4
void display_box_msg_prg(void) {
    //stub
}

//4F904
void display_box_msg(void* a1) {
    //stub
}

//4F960
void display_box_msg_commande(void* a1, void* a2) {
    //stub
}

//4F9F0
void AfficeYesNo(void) {
    //stub
}

//4FAC4
void DO_YESNOBIS(void) {
    //stub
}

//4FB94
void confirmation_msg(void) {
    //stub
}

//4FEF0
void SYNCHRO_LOOP_PALETTE(void* func) {
    //stub
}

//4FF3C
void apparition_prg(void) {
    //stub
}

//500FC
void fondu_prg(void) {
    //stub
}

//501E8
void change_couleur_prg(void) {
    //stub
}

//502FC
void FonduOption(i16 a1, i16 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//50A38
bool FonduPixel_prg(void) {
    return false; //stub
}

//50B0C
void FonduPixel(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}
