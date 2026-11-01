
//48EF0
void LOAD_FND(void) {
    if (Plan0NumPcx[no_fnd] != -1) {
        LoadPlan0InVignet(Plan0NumPcx[no_fnd]);
    }
    if (GameModeVideo == MODE_NORMAL) {
        Init_Bande(no_fnd, plan0_width, plan0_height, PLAN0BIT, DrawBufferNormal);
    }
}

//49098
void INIT_FND(void) {
    sp_y = 0;
    type_fnd = 16 * mp.height >= plan0_height ? 0 : 3;
    type_fndscrX = 0;
    switch (num_world) {
        case 1: {
            switch (num_level) {
                case 6:
                case 9: {
                    type_fndscrX = 1;
                    sp_y = 2;
                    type_fnd = 1;
                } break;
                case 7:
                case 10: {
                    type_fnd = 3;
                } break;
                case 16:
                case 22: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                default: break;
            }
        } break;
        case 2: {
            switch (num_level) {
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 18: {
                    type_fnd = 1;
                    sp_y = 4;
                } break;
                default: break;
            }
        } break;
        case 3: {
            switch (num_level) {
                case 7: {
                    type_fnd = 1;
                    sp_y = 4;
                } break;
                case 10: {
                    type_fnd = 3;
                } break;
                default: break;
            }
        } break;
        case 4: {
            switch (num_level) {
                case 4:
                case 11: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                default: break;
            }
        } break;
        case 5: {
            switch (num_level) {
                case 3:
                case 11: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                default: break;
            }
        } break;
        case 6: {
            if (num_level == 4) {
                type_fndscrX = 1;
                type_fnd = 3;
            }
        } break;
        default:break;
    }
    if (GameModeVideo == MODE_NORMAL) {
        type_fndscrX = 0;
    }
    LOAD_FND();
}

//491D4
void calc_off_fnd(void) {
    print_once("Not implemented: calc_off_fnd"); //stub
}

s8 byte_49290[4] = {0, 0, 0, 0}; //49290

/*
 * The PC executable reuses the first seven words of `options` while the key
 * screen is open.  Keep an explicit edit buffer here instead: those words are
 * otherwise the live sound/video options in this translation, and aliasing
 * them makes the submenu unnecessarily fragile.
 */
static u8 key_screen_bindings[7];
static char key_screen_title[200];

//49308
void check_key_table(void) {
    bool valid = true;
    for (s32 i = 0; i < 7; ++i) {
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
        LoadLanguageTxt(main_mem_fix, 0); // English
        if (tRefRam2VramNormalFix[0] >= 128 && xpadmax == -1) {
            // NOTE: this may occur if a RAYMAN.CFG is present that contains values that meet this condition.
            // TODO: figure out why this causes Mode X to be used instead of normal mode.
            GameModeVideo = MODE_X;
            P486 = 0;
        }
        if (FondAutorise == 2) {
            GameModeVideo = MODE_X;
            P486 = 1;
        }
        POINTEUR_BOUTONS_OPTIONS_BIS();

        // STUB: this section seems to be related to sound card initialization
        if (CarteSonAutorisee && DeviceID != 999) {
            SetPort_(Port);
            SetIrq(Irq);
            SetDma(Dma);
            SetParam(Param);
            SetDeviceID(DeviceID);
            //if (sub_3E780()) {} // seems to do something with locking memory regions (DOS protected mode stuff)

        } else {
            SetDeviceID(DeviceID);
            CarteSonAutorisee = 0;
        }

        if (CarteSonAutorisee) {
            LoadBnkFixe();
            InitSnd();
        }

        if (JoystickPresent()) {
            // stub: init joypad
        } else {
            if (xpadmax == -1) {
                xpadmax = -2;
            }
        }

        // stub
    } else {
        // We didn't load a .cfg file -> load defaults
        if (CarteSonAutorisee) {
            LoadBnkFixe();
            InitSnd();
        }
        language = 0; // English
        LoadLanguageTxt(main_mem_fix, language);
        if (JoystickPresent()) {
            pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
            CalcTab();
            DO_FADE_OUT();
            MAIN_CALIBRATE_JOYSTICK();
        } else {
            xpadmax = -2;
        }
        options_jeu.music_enabled = 2;
        options_jeu.sound_volume = 18;
        options_jeu.is_stereo = 1;
        options_jeu.jump = 1;
        options_jeu.fist = 0;
        options_jeu.action = 2;
        SetVolumeSound((options_jeu.sound_volume * 127) / 20);
        GameModeVideo = MODE_NORMAL;
        POINTEUR_BOUTONS_OPTIONS_BIS();
    }

    set_special_key_descriptions(key_descriptions_azerty);
    set_special_key_descriptions(key_descriptions_qwerty);
    check_key_table();
}

//49638
void NewFrequency(u8 new_frequency) {
    // PC menu values are selectors, not Hz. Do not overwrite Frequence here:
    // the graphics screen previews changes and needs it to restore on cancel.
    switch (new_frequency) {
        case 0:
        case 1:
            use_sync = 1;
            display_mode = 0;
            set_vga_frequency(new_frequency == 0 ? 60 : 50);
            break;
        case 2:
            use_sync = 0;
            set_vga_frequency(60);
            // PC "max" omits VGA retrace waits. The portable backend retains
            // its 60 Hz frame limiter, so gameplay cannot run at CPU speed.
            break;
        case 70:
        case 80:
        case 100:
            use_sync = 1;
            display_mode = 0;
            set_vga_frequency(new_frequency);
            break;
        default:
            break;
    }
    // The PC also recalibrates DOS sound-card timing here; host audio does not
    // require (or support) that hardware reinitialization.
}

//49720
void InitClipping(void) {
    if (GameModeVideo != MODE_NORMAL) {
        default_sprite_clipping();
    } else if (is_fee || (num_world == world_6_cake && num_level == 4) || get_casse_brique_active()) {
        set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
        sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    } else {
        set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
        sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
        N_CLRSCR(DrawBufferNormal);
    }
}

//4980C
bool IsBackgroundOn(void) {
    if ((num_world == world_1_jungle && num_level == 16) ||
        (num_world == world_2_music && num_level == 16) ||
        (num_world == world_3_mountain && num_level == 10) ||
        (num_world == world_4_image && (num_level == 4 || num_level == 11)) ||
        (num_world == world_5_cave && (num_level == 3 || num_level == 11)) ||
        (num_world == world_6_cake && num_level == 4)) {
        return true;
    }
    return BackgroundOptionOn != 0;
}

//498A4
void init_key2txt(void) {
    set_special_key_descriptions(key_descriptions);
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
        MusicCdActive = options_jeu.music_enabled;
        if (!MusicCdActive) {
            stop_cd();
        }
    }
    Keyflush(); //TODO
    CalcTab();
    default_key();
    s16 NewMenusArgs = 0;
    while (!ModeDemo && !fin_du_jeu && !NewMenusArgs) {
        raj_env_sound(options_jeu.sound_volume);
        raj_env_stereo(options_jeu.is_stereo);
        raj_env_audio(options_jeu.music_enabled);
        switch(menuEtape) {
            default: break;
            case 0: {
                // Main menu screen
                pINIT_SCREEN = INIT_GENERAL_CHOICE;
                pLOAD_SCREEN = LOAD_GENERAL_SCREEN;
                INIT_FADE_IN();
                DO_MENU();
            } break;
            case 3: {
                // Save selection screen
                DO_SAVE_CHOICE();
                if (!MENU_RETURN) {
                    fin_du_jeu = 0;
                    NewMenusArgs = 1;
                    new_world = 1;
                }
            } break;
            case 4: {
                // Options screen
                pINIT_SCREEN = INIT_OPTIONS_CHOICE;
                if (pLOAD_SCREEN) {
                    byte_E4CFD = 1;
                    pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
                }
                DO_MENU();
            } break;
            case 7: {
                // Setup keys screen (in options)
                pINIT_SCREEN = INIT_KEY_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 8: {
                // Setup pad screen (in options)
                pINIT_SCREEN = INIT_PAD_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 9: {
                // Graphics details screen (in options)
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
    SaveOptionsOnDisk();

}

//49B7C
void DO_OPTIONS_IN_GAME(void) {

    if (is_debug_mode && GoMenu == 255) {
        //NOTE: debug cheat for quickly quitting out of a level (will return to the world map)
        new_world = 1;
        new_level = 1;
        GoMenu = 0;
        return;
    }

    u8 button_released_saved = button_released;
    u16 SizeScreen_saved;

    SetCompteurTrameAudio();
    menuEtape = 4;
    byte_E4CFD = 1;
    MENU_SUITE = 0;
    MENU_RETURN = 0;
    if (GoMenu == 1) {
        if (byte_96884 == 64) {
            set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            ++GoMenu;
        } else {
            GoMenu = 0;
        }
    } else if (GoMenu == 2 || GameModeVideo == MODE_X) {
        if ((num_world == 6 && num_level == 4) || is_fee) {
            SizeScreen_saved = SizeScreen;
            SizeScreen = 0;
        }
        rvb_options_in_game = rvb[0];
        Keyflush();
        default_key();
        GoMenu = 0;
        OptionGame = 1;
        stop_cd();
        cdTime = 0;
        if (CarteSonAutorisee) {
            start_freeze_snd();
        }
        while (menuEtape != 0) {
            raj_env_sound(options_jeu.sound_volume);
            raj_env_stereo(options_jeu.is_stereo);
            raj_env_audio(options_jeu.music_enabled);
            RESET_ALL_TOUCHE();
            switch(menuEtape) {
                case 4: {
                    pINIT_SCREEN = INIT_OPTIONS_CHOICE;
                    pLOAD_SCREEN = NULL;
                    if (MENU_RETURN) {
                        menuEtape = 4;
                    }
                    DO_MENU();
                } break;
                case 7: {
                    pINIT_SCREEN = INIT_KEY_SCREEN;
                    pLOAD_SCREEN = NULL;
                    if (MENU_RETURN) {
                        menuEtape = 4;
                    }
                    DO_MENU();
                } break;
                case 8: {
                    pINIT_SCREEN = INIT_PAD_SCREEN;
                    pLOAD_SCREEN = NULL;
                    if (MENU_RETURN) {
                        menuEtape = 4;
                    }
                    DO_MENU();
                } break;
                case 9: {
                    pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
                    pLOAD_SCREEN = NULL;
                    if (MENU_RETURN) {
                        menuEtape = 4;
                    }
                    DO_MENU();
                } break;
                default: break;
            }
            /* Keep the in-game overlay alive across its nested PC menu screens. */
            MENU_RETURN = 0;
        }
        OptionGame = 0;
        new_key();
        if (GameModeVideo == MODE_NORMAL) {
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            N_CLRSCR(DrawBufferNormal);
        } else {
            if (P486 == 1) {
                //sub_1268A(plan0bit_length >> 2);
            }
        }
        if (CarteSonAutorisee) {
            stop_freeze_snd();
        }
        in_pause = 0;
        ProchainEclair = 1;
        numero_palette_special = 0;
        MapTimePause = 0;
        if ((num_world == 6 && num_level == 4) || is_fee) {
            SizeScreen = SizeScreen_saved;
        }
        byte_96884 = dead_time;
        button_released = button_released_saved;
    }
}

//49E98
void INIT_GENERAL_CHOICE(void) {
    pINIT_AFFICHE_SCREEN = NULL;
    pINIT_SCROLL = NULL;
    pDO_COMMANDE = DO_COMMANDE_GENERAL;
    pEND_SCREEN = END_GENERAL_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_GENERAL;
    u8 max_options = 0;
    option_exit = 3;
    nbre_options = 4;
    basex = 160;
    for (s32 i = 0; i < nbre_options; ++i) {
        display_item_t* display_item = menu_to_display + i;
        strncpy(display_item->text, language_txt[190], 200); // "music"
        display_item->color = byte_49290[i];
        switch(i) {
            default: break;
            case 0: {
                strncpy(display_item->text, language_txt[109], 200); // "/start/"
                display_item->ypos = 30;
            } break;
            case 1: {
                strncpy(display_item->text, language_txt[181], 200); // "/options/"
                display_item->ypos = 60;
            } break;
            case 2: {
                strncpy(display_item->text, language_txt[110], 200); // "/credits/"
                display_item->ypos = 155;
            } break;
            case 3: {
                strncpy(display_item->text, language_txt[154], 200); // "/exit/"
                display_item->ypos = 185;
            } break;
        }
        display_item->xpos = basex;
        display_item->font_size = 1;
        display_item->is_fond = 1;
        display_item->field_D5 = 0;
        INIT_TXT_BOX(display_item);
    }
    positiony = 0;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    choix_menu = 0;
    sortie_options = 0;

}

//4A0DC
void AFFICHE_ECRAN_GENERAL(void) {
    DISPLAY_FOND_MENU();
    for (s32 i = 0; i < nbre_options; ++i) {
        display_item_t* to_display = menu_to_display + i;
        if (i == positiony) {
            display_box_text_plasma(to_display, 1);
        } else {
            display_text(to_display->text, to_display->xpos, to_display->ypos, to_display->font_size, to_display->color);
        }
    }
}

//4A178
void DO_COMMANDE_GENERAL(void) {
    TestCompteur();
    ++TempsDemo;
    if (TempsDemo >= RunTimeDemo && !fin_du_jeu) {
        InitDemoJeu();
    }
    if (SelectButPressed()) {
        RESET_TOUCHE(SC_ESCAPE);
        if (confirmation_msg(0)) {
            MENU_RETURN = 1;
        }
    }
    if (upjoy() && (button_released || (delai_repetition < compteur && compteur % repetition == 0))) {
        PlaySnd_old(68);
        if (positiony <= first_option) {
            positiony = option_exit;
        } else {
            --positiony;
        }
        TempsDemo = 0;
    }
    if (downjoy() && (button_released || (delai_repetition < compteur && compteur % repetition == 0))) {
        PlaySnd_old(68);
        if (positiony < option_exit) {
            ++positiony;
        } else {
            positiony = first_option;
        }
        TempsDemo = 0;
    }
    if (ValidButPressed() && button_released) {
        PlaySnd_old(69);
        switch(positiony) {
            default: break;
            case 0: {
                TempsDemo = 0;
                sortie_options = 1;
                menuEtape = 3;
            } break;
            case 1: {
                TempsDemo = 0;
                sortie_options = 1;
                menuEtape = 4;
            } break;
            case 2: {
                You_Win = 1;
                loop_nb_trames = 0;
                new_txt_fee = 0;
                LoadPlan3InVignet(main_mem_level, 61);
                loop_timing = 4;
                INIT_TEXT_TO_DISPLAY();
                DO_FADE_OUT();
                INIT_FADE_IN();
                DISPLAY_CREDITS_MENU();
                block_free(main_mem_level);
                You_Win = 0;
            } break;
            case 3: {
                if (confirmation_msg(0)) {
                    MENU_RETURN = 1;
                }
            } break;
        }
    }
    TestButtonReleased();
}

//4A3D8
void END_GENERAL_SCREEN(void) {
    if (MENU_RETURN) {
        fin_du_jeu = 1;
        fin_de_rayman = 1;
        if (pAFFICHE_SCREEN) {
            pAFFICHE_SCREEN();
            advance_frame();
            InitPlasma(1);
            SWAP_BUFFERS();
        }
    } else if (!ModeDemo) {
        RunTimeDemo = 1800;
        if (First_Hist == 2) {
            First_Hist = 0;
        }
        DO_FADE_OUT();
        INIT_FADE_IN();
    }
}

//4A44C
void INIT_OPTIONS_CHOICE(void) {
    /*if (WindowsLance == 0) {
        if (__readdr(0) || __readdr(1) || __readdr(2) || __readdr(3)) { exit(); }
    }*/
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_ECRAN_OPTIONS;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_OPTIONS;
    pDO_COMMANDE = DO_COMMANDE_OPTIONS;
    pEND_SCREEN = END_OPTIONS_SCREEN;
    delai_repetition = 12;
    repetition = 6;
    max_sound = 20;
    ecart_barre = 4;
    compteur = 0;
    max_compteur = 100;
    button_released = 1;
    delai_barre = 3;
    delai_stereo = 8;
    if (language != 0) {
        byte_E4CFC = 90;
    } else {
        byte_E4CFC = 75;
    }
    basex = 160 - byte_E4CFC;
    if (OptionGame) {
        position = 6;
        option_exit = 6;
        nbre_options = 6;
    } else {
        position = 5;
        option_exit = 5;
        nbre_options = 5;
    }
    first_option = 0;
    general_init_screen(20, 60, 5);
    if (byte_96883) {
        options.sound_volume = byte_E4CD9;
        byte_96883 = 0;
        options.music_enabled = byte_E4CD8;
    } else {
        options.music_enabled = options_jeu.music_enabled;
        options.sound_volume = options_jeu.sound_volume;
    }
    ExitMenu = 0;
    sortie_options = 0;
}

//4A628
void INIT_AFFICHE_ECRAN_OPTIONS(void) {
    char* v32 = "off";
    u8 v28[8] = {5, 5, 5, 5, 5, 5, 3, 0}; //0x5050505;
//    u8 v29[4] = {5, 5, 3, 0}; // 0x30505; //dword_49294[1]
    u32 v30 = 0; //dword_49294[2];
    menu_to_display[9].is_fond = 1;
    menu_to_display[9].font_size = 2;
    menu_to_display[9].field_D5 = 0;
    menu_to_display[9].color = 1;
    char v39[4];
    strncpy(v39, "on", 4);
    for (s32 i = first_option; i <= first_option + nbre_options; ++i) {
        display_item_t* to_display = menu_to_display + i;
        to_display->font_size = 2;
        to_display->field_D5 = 0;
        to_display->is_fond = 1;
        if (i == option_exit) {
            strcpy(to_display->text, language_txt[232]); // /exit/
            to_display->xpos = (SCREEN_WIDTH / 2);
            to_display->ypos = debut_sortie;
            to_display->color = v28[6];
        } else {
            to_display->color = v28[i];
            strcpy(to_display->text, language_txt[190 + i]); // { "music", "sound", "/setup keys/", "/graphics details/", "/quit game/" }
            switch(i) {
                case 0: {
                    // Text "music" offsetted to the left with "on off" to the right
                    s32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
                                     calc_largmax_text(v39, -1, 8, 1, 2) + calc_largmax_text(v32, -1, 8, 1, 2);
                    to_display->xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 1: {
                    // Text "sound" offsetted to the left
                    s32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
                             20 * calc_largmax_text("|", -1, 8, 1, 2);
                    to_display->xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 2:
                case 3:
                case 4:
                case 5:
                    // Text centered
                    to_display->xpos = (SCREEN_WIDTH / 2);
                    break;
                default:
                    break;
            }
            to_display->ypos = i * (ecart_options + 15) + debut_options;
        }
        INIT_TXT_BOX(to_display);
    }

    strcpy(msg_to_display[0].text, language_txt[136]); ///no joystick detected !/check if cable is connected/
    msg_to_display[0].xpos = (SCREEN_WIDTH / 2);
    msg_to_display[0].ypos = (SCREEN_HEIGHT / 2);
    msg_to_display[0].font_size = 2;
    msg_to_display[0].is_fond = 2;
    msg_to_display[0].field_D5 = 0;
    INIT_TXT_BOX(&msg_to_display[0]);
    msg_to_display[1].xpos = (SCREEN_WIDTH / 2);
    msg_to_display[1].ypos = (SCREEN_HEIGHT / 2);
    msg_to_display[1].font_size = 2;
    msg_to_display[1].is_fond = 2;
    msg_to_display[1].field_D5 = 0;

    if (byte_E4CFD) {
        if (OptionGame) {
            word_E4CEC_x = basex;
            word_E4CF4_w = 2 * byte_E4CFC;
            word_E4CF0_y = debut_titre - 25;
            word_E4CEE_h = menu_to_display[option_exit].ypos - debut_titre + 35;
            FonduOption(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h, 0);
        }
    } else {
        s16 v21 = debut_titre - 25;
        s16 v42_hi = menu_to_display[option_exit].ypos - debut_titre + 35;
        s16 v22_x = (basex >= word_E4CEC_x) ? word_E4CEC_x : basex;
        s16 v23_y = word_E4CF0_y;
        if (v21 < word_E4CF0_y) {
            v23_y = debut_titre - 25;
        }
        s16 v24 = word_E4CF4_w + word_E4CEC_x;
        s32 v37 = basex;
        s32 v33 = v24;
        s32 v25 = basex + 2 * byte_E4CFC;
        s32 v34 = (v25 >= v24) ? v25 - v22_x : v24 - v22_x;
        s16 unk_w = v34;
        s32 v26 = word_E4CEE_h + word_E4CF0_y;
        s32 v31 = v21;
        s32 v38 = v42_hi;
        s32 v36 = v42_hi + v21;
        if (v26 <= v36) {
            v26 = v36;
        }
        s32 v27_h = v26 - v23_y;
        word_E4CEC_x = basex;
        word_E4CF0_y = debut_titre - 25;
        word_E4CEE_h = v42_hi;
        word_E4CF4_w = 2 * byte_E4CFC;
        if (OptionGame) {
            FonduOption(v22_x, v23_y, unk_w, v27_h, 1); //TODO

        } else {
            FonduPixel(v22_x, v23_y, unk_w, v27_h); //TODO
        }
    }
    byte_E4CFD = 0;
}

//4AB34
void AFFICHE_ECRAN_OPTIONS(void) {
    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(language_txt[181], SCREEN_WIDTH / 2, debut_titre, 1, 1); // /options/
    for (s32 option_index = first_option; option_index <= first_option + nbre_options; ++option_index) {
        s16 x = menu_to_display[option_index].xpos;
        s16 y = menu_to_display[option_index].ypos;
        if (option_index == position) {
            display_box_text_plasma(menu_to_display + option_index, 1);
        } else if (option_index == option_exit) {
            display_text(language_txt[232], x, y, 2, menu_to_display[option_index].color); // /exit/
        } else {
            display_text(language_txt[190 + option_index], x, y, 2, menu_to_display[option_index].color); // { "music", "sound", "/setup keys/", "/graphics details/", "/quit game/" }
        }

        // NOTE: some parts of the code below don't seem to make sense in the disassembly.
        // I rewrote it slightly to make it functionally correct.
        if (option_index == 0) {
            s32 v5 = calc_largmax_text(menu_to_display[0].text, -1, 8, 1, 2);
            s32 v6 = v5 + x;
            const char* nonselected_text;
            const char* other_text;
            s32 v18;
            s32 v8;
            if (options.music_enabled) {
                nonselected_text = "off";
                v18 = v6 + calc_largmax_text("on", -1, 8, 1, 2) + 32;
                v8 = v6 + 16;
                other_text = "on";
            } else {
                nonselected_text = "on";
                v18 = v6 + 16;
                v8 = v6 + calc_largmax_text("on", -1, 8, 1, 2) + 32;
                other_text = "off";
            }
            display_text(nonselected_text, v18, y, 2, 5);
            if (position == option_index) {
                display_item_t* to_display = menu_to_display + 9;
                strcpy(to_display->text, other_text);
                to_display->xpos = v8;
                to_display->ypos = menu_to_display[0].ypos;
                INIT_TXT_BOX(to_display);
                display_box_text_plasma(to_display, 0);
            } else {
                display_text(other_text, v8, y, 2, 5);
            }
        } else if (option_index == 1) {
            s32 draw_x = x + calc_largmax_text(menu_to_display[1].text, -1, 8, 1, 2u) + 16;
            DrawBlackBorderBox(draw_x, y - 14, 15, 20 * ecart_barre + 1, 1);
            for (s32 i = 0; i < options.sound_volume; ++i) {
                u8 color;
                if (i >= (2 * max_sound) / 3) {
                    color = 1;
                } else if (i >= (max_sound / 3)) {
                    color = 3;
                } else {
                    color = 5;
                }
                display_text("|", draw_x + ecart_barre * i, y, 2, color);
            }
        }
    }
}

//4AF28
void DO_COMMANDE_OPTIONS(void) {
    TestCompteur();
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }
    if (upjoy() && !rightjoy() && !leftjoy() &&
            (button_released || (delai_repetition < compteur && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == first_option) {
            position = option_exit;
        } else {
            --position;
        }
    }
    if (downjoy() && !rightjoy() && !leftjoy() &&
        (button_released || (delai_repetition < compteur && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == option_exit) {
            position = first_option;
        } else {
            ++position;
        }
    }
    if (position == 0 && button_released && (ValidButPressed() || rightjoy() || leftjoy())) {
        options.music_enabled = !options.music_enabled;
        if (options.music_enabled) {
            SetCompteurTrameAudio();
            cdTime = 0;
            if (OptionMusicCdActive) {
                MusicCdActive = 1;
            }
        } else {
            stop_cd();
            if (OptionMusicCdActive) {
                MusicCdActive = 0;
            }
        }
    } else if (position == 1 && rightjoy()) {
        if (options.sound_volume < max_sound) {
            if (button_released || (delai_repetition < compteur && (compteur % repetition) == 0)) {
                PlaySnd_old(68);
                ++options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    } else if (position == 1 && leftjoy()) {
        if (options.sound_volume > 0) {
            if (button_released || (delai_repetition < compteur && (compteur % repetition) == 0)) {
                PlaySnd_old(68);
                --options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    } else if (position == 3 && ValidButPressed()) {
        if (JoystickPresent()) {
            byte_E4CD8 = options.music_enabled;
            byte_E4CD9 = options.sound_volume;
            byte_96883 = 1;
            menuEtape = 8; // go to setup pad
            sortie_options = 1;
        } else {
            display_box_msg(&msg_to_display[0]); // no joystick connected
        }
    } else if (position == 2 && ValidButPressed()) {
        byte_E4CD8 = options.music_enabled;
        byte_E4CD9 = options.sound_volume;
        menuEtape = 7; // go to setup keys
        sortie_options = 1;
        byte_96883 = 1;
    } else if (position == 4 && ValidButPressed()) {
        byte_E4CD8 = options.music_enabled;
        byte_E4CD9 = options.sound_volume;
        menuEtape = 9; // go to graphics details
        byte_96883 = 1;
        sortie_options = 1;
    } else if (position == 5 && ValidButPressed() && button_released && OptionGame) {
        if (confirmation_msg(0)) {
            ExitMenu = 1;
            fin_du_jeu = 1;
            new_world = 1;
        }

    }
    TestButtonReleased();
}

//4B334
void END_OPTIONS_SCREEN(void) {
    if ((position == option_exit && MENU_RETURN != 1) || fin_du_jeu) {
        options_jeu.music_enabled = options.music_enabled;
        options_jeu.sound_volume = options.sound_volume;
    }
    if (position == option_exit || MENU_RETURN == 1 || fin_du_jeu) {
        if (OptionMusicCdActive) {
            MusicCdActive = options_jeu.music_enabled;
        }
        menuEtape = 0;
        if (OptionGame) {
            FonduOption(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h, 2);
        } else {
            DO_FADE_OUT();
        }
    }
}

//4B3EC
void INIT_KEY_SCREEN(void) {
    key_descriptions = language == 1 ? key_descriptions_azerty : key_descriptions_qwerty;
    init_key2txt();

    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_KEY_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_KEY_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_KEY;
    pEND_SCREEN = END_KEY_SCREEN;

    position = 8;
    nbre_options = 8;
    option_exit = 8;
    for (s32 i = 0; i < 7; ++i) {
        key_screen_bindings[i] = tab_key_sav[i];
    }

    key_select = 0;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    button_released = 1;
    repetition = 6;

    switch (language) {
        case 1:
            byte_E4CFC = 105;
            break;
        case 2:
            byte_E4CFC = 120;
            break;
        case 0:
        default:
            /* The PC routine only has layouts for the three shipped languages. */
            byte_E4CFC = 90;
            break;
    }
    basex = (SCREEN_WIDTH / 2) - byte_E4CFC;
    general_init_screen(15, 68, 5);
}

//4B514
void INIT_AFFICHE_KEY_SCREEN(void) {
    static const u8 colors[9] = {5, 5, 5, 5, 5, 5, 5, 5, 3};
    s16 label_offset;

    switch (language) {
        case 1:
            label_offset = 25;
            break;
        case 2:
            label_offset = 45;
            break;
        case 0:
        default:
            label_offset = 20;
            break;
    }

    /* PC surrounds the localized title with an additional pair of line marks. */
    snprintf(key_screen_title, sizeof(key_screen_title), "/%s/", language_txt[233]);

    for (s32 i = 0; i <= nbre_options; ++i) {
        display_item_t* item = &menu_to_display[i];
        item->color = colors[i];
        item->font_size = 2;
        item->field_D5 = 0;
        item->is_fond = 1;

        if (i == option_exit) {
            strcpy(item->text, language_txt[232]); /* /exit/ */
            item->xpos = SCREEN_WIDTH / 2;
            item->ypos = debut_sortie;
        } else {
            strcpy(item->text, language_txt[182 + i]);
            item->xpos = i == 7 ? SCREEN_WIDTH / 2 : basex + label_offset;
            item->ypos = debut_options + i * (ecart_options + 15);
        }
        INIT_TXT_BOX(item);
    }

    text_to_display[0].is_fond = 1;
    text_to_display[0].xpos = basex + 80;
    text_to_display[0].field_D5 = 0;
    text_to_display[0].font_size = 2;

    strcpy(msg_to_display[0].text, language_txt[222]);
    msg_to_display[0].xpos = SCREEN_WIDTH / 2;
    msg_to_display[0].ypos = SCREEN_HEIGHT / 2;
    msg_to_display[0].font_size = 2;
    msg_to_display[0].is_fond = 2;
    msg_to_display[0].field_D5 = 0;
    INIT_TXT_BOX(&msg_to_display[0]);

    const s16 new_x = basex - 2;
    const s16 new_y = debut_titre - 25;
    const s16 new_w = 2 * byte_E4CFC;
    const s16 new_h = menu_to_display[option_exit].ypos - debut_titre + 35;
    const s16 fade_x = MIN(new_x, word_E4CEC_x);
    const s16 fade_y = MIN(new_y, word_E4CF0_y);
    const s16 fade_right = MAX(new_x + new_w, word_E4CEC_x + word_E4CF4_w);
    const s16 fade_bottom = MAX(new_y + new_h, word_E4CF0_y + word_E4CEE_h);

    word_E4CEC_x = new_x;
    word_E4CF0_y = new_y;
    word_E4CF4_w = new_w;
    word_E4CEE_h = new_h;
    if (OptionGame) {
        FonduOption(fade_x, fade_y, fade_right - fade_x, fade_bottom - fade_y, 1);
    } else {
        FonduPixel(fade_x, fade_y, fade_right - fade_x, fade_bottom - fade_y);
    }
}

//4B8A4
void AFFICHE_KEY_SCREEN(void) {
    s16 key_offset;

    switch (language) {
        case 1:
            key_offset = 70;
            break;
        case 2:
            key_offset = 75;
            break;
        case 0:
        default:
            key_offset = 62;
            break;
    }

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(key_screen_title, SCREEN_WIDTH / 2, debut_titre, 1, 1); /* /setup keys/ */

    for (s32 i = 0; i <= nbre_options; ++i) {
        display_item_t* item = &menu_to_display[i];
        if (i == position) {
            display_box_text_plasma(item, 1);
        } else {
            display_text(item->text, item->xpos, item->ypos, item->font_size, item->color);
        }

        if (i >= 7) {
            continue;
        }

        const char* key_name = key_descriptions[key_screen_bindings[i]];
        if (key_select && i == position) {
            display_item_t* selected_key = &text_to_display[0];
            strcpy(selected_key->text, key_name);
            selected_key->color = item->color;
            selected_key->xpos = item->xpos + key_offset;
            selected_key->ypos = item->ypos;
            INIT_TXT_BOX(selected_key);
            display_box_text_plasma(selected_key, 0);
        } else {
            display_text(key_name, item->xpos + key_offset, item->ypos, 2, item->color);
        }
    }
}

//4BACC
void DO_COMMANDE_KEY(void) {
    TestCompteur();

    if (key_select) {
        for (s32 key = 0; key < 128; ++key) {
            if (TOUCHE((u8)key) && key_descriptions[key] != NULL) {
                key_screen_bindings[position] = (u8)key;
                key_select = 0;
                RESET_ALL_TOUCHE();
            }
        }
        TestButtonReleased();
        return;
    }

    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }

    if (!rightjoy() && !leftjoy() &&
        (button_released || (compteur > delai_repetition && (compteur % repetition) == 0))) {
        if (upjoy()) {
            PlaySnd_old(68);
            position = position == 0 ? option_exit : position - 1;
        } else if (downjoy()) {
            PlaySnd_old(68);
            position = position == option_exit ? 0 : position + 1;
        }
    }

    if (position == 7 && ValidButPressed() && button_released) {
        static const u8 default_bindings[7] = {
            SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, SC_CONTROL, SC_ALT, SC_X
        };
        memcpy(key_screen_bindings, default_bindings, sizeof(key_screen_bindings));
    } else if (position >= 0 && position < 7 && ValidButPressed() && button_released) {
        key_select = 1;
        RESET_ALL_TOUCHE();
        TestButtonReleased();
        return;
    } else if (position == option_exit && ValidButPressed()) {
        bool duplicate = false;
        for (s32 i = 0; i < 6 && !duplicate; ++i) {
            for (s32 j = i + 1; j < 7; ++j) {
                if (key_screen_bindings[i] == key_screen_bindings[j]) {
                    duplicate = true;
                    break;
                }
            }
        }
        if (duplicate) {
            display_box_msg(&msg_to_display[0]);
            TestButtonReleased();
            return;
        }
    }

    TestButtonReleased();
}

//4BD50
void END_KEY_SCREEN(void) {
    menuEtape = 4;
    key_select = 0;
    if (!MENU_RETURN) {
        memcpy(tab_key_sav, key_screen_bindings, sizeof(key_screen_bindings));
    } else {
        byte_96883 = 0;
        if (!OptionGame) {
            DO_FADE_OUT();
        }
    }
}

//4BDA4
void INIT_BUTTON_SCREEN(void) {
    print_once("Not implemented: INIT_BUTTON_SCREEN"); //stub
}

//4BE04
void INIT_AFFICHE_BUTTON_SCREEN(void) {
    print_once("Not implemented: INIT_AFFICHE_BUTTON_SCREEN"); //stub
}

//4BF60
void AFFICHE_BUTTON_JOYSTICK(void) {
    print_once("Not implemented: AFFICHE_BUTTON_JOYSTICK"); //stub
}

//4C138
void DO_BUTTON_COMMANDE(void) {
    print_once("Not implemented: DO_BUTTON_COMMANDE"); //stub
}

//4C390
void END_BUTTON_COMMAND(void) {
    print_once("Not implemented: END_BUTTON_COMMAND"); //stub
}

//4C3D0
void INIT_PAD_SCREEN(void) {
    print_once("Not implemented: INIT_PAD_SCREEN"); //stub
}

//4C4D0
void INIT_AFFICHE_PAD_SCREEN(void) {
    print_once("Not implemented: INIT_AFFICHE_PAD_SCREEN"); //stub
}

//4C854
void AFFICHE_PAD_SCREEN(void) {
    print_once("Not implemented: AFFICHE_PAD_SCREEN"); //stub
}

//4C9D8
void DO_COMMANDE_PAD(void) {
    print_once("Not implemented: DO_COMMANDE_PAD"); //stub
}

//4CD38
void END_PAD_SCREEN(void) {
    print_once("Not implemented: END_PAD_SCREEN"); //stub
}

// Keep the PC state machine below, including its dormant Mode-X branches.
// Restrict only transitions into unsupported backends (both the video selector
// and the automatic switch when disabling backgrounds).
static bool graphics_mode_supported(s16 mode) {
    return mode == 0;
}

static void graphics_screen_transition(void) {
    s16 x = basex;
    s16 y = debut_titre - 25;
    s16 w = 2 * byte_E4CFC;
    s16 h = menu_to_display[option_exit].ypos - debut_titre + 35;
    s16 fade_x = MIN(x, word_E4CEC_x);
    s16 fade_y = MIN(y, word_E4CF0_y);
    s16 right = MAX(x + w, word_E4CEC_x + word_E4CF4_w);
    s16 bottom = MAX(y + h, word_E4CF0_y + word_E4CEE_h);
    word_E4CEC_x = x;
    word_E4CF0_y = y;
    word_E4CF4_w = w;
    word_E4CEE_h = h;
    if (OptionGame) {
        FonduOption(fade_x, fade_y, right - fade_x, bottom - fade_y, 1);
    } else {
        FonduPixel(fade_x, fade_y, right - fade_x, bottom - fade_y);
    }
}

static void graphics_screen_value(const char* text, s16 row, s16 x, bool active) {
    display_item_t* item = &text_to_display[0];
    if (active && position == row) {
        strcpy(item->text, text);
        item->xpos = x;
        item->ypos = menu_to_display[row].ypos;
        item->font_size = 2;
        item->color = 1;
        item->is_fond = 1;
        item->field_D5 = 0;
        INIT_TXT_BOX(item);
        display_box_text_plasma(item, 0);
    } else {
        display_text(text, x, menu_to_display[row].ypos, 2, active ? 1 : 5);
    }
}

static void graphics_screen_navigation(bool skip_viewport) {
    if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
        if (upjoy()) {
            if (position == first_option) {
                position = option_exit;
            } else {
                position -= skip_viewport && position == 3 ? 2 : 1;
            }
            PlaySnd_old(68);
        } else if (downjoy()) {
            if (position == option_exit) {
                position = first_option;
            } else {
                position += skip_viewport && position == 1 ? 2 : 1;
            }
            PlaySnd_old(68);
        }
    }
}

//4CD94
void InitParamGraphicScreen(void) {
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    if (options.video_mode == 0 && ModeNormalAutorise) {
        if (OptionGame) {
            bool fixed_size = is_fee || (num_world == world_6_cake && num_level == 4);
            position = fixed_size ? 4 : 5;
            first_option = fixed_size ? 3 : 2;
            nbre_options = fixed_size ? 3 : 4;
            option_exit = 5;
            byte_E4CFC = language == 2 ? 120 : 110;
            basex = SCREEN_WIDTH / 2 - byte_E4CFC;
            general_init_screen(20, 60, 5);
        } else {
            position = 7;
            first_option = 1;
            nbre_options = 7;
            option_exit = 7;
            byte_E4CFC = language == 2 ? 125 : 120;
            basex = SCREEN_WIDTH / 2 - byte_E4CFC;
            general_init_screen(15, 70, 5);
        }
        --nbre_options; // PC reserves the first row for the performance indicator.
    } else if (FondAutorise == 1 || FondAutorise == 2) {
        position = 5;
        option_exit = 6;
        first_option = OptionGame || FondAutorise == 2 ? 3 : 1;
        nbre_options = OptionGame || FondAutorise == 2 ? 4 : 5;
        byte_E4CFC = language == 2 ? 120 : 110;
        basex = SCREEN_WIDTH / 2 - byte_E4CFC;
        general_init_screen(20, 60, 5);
    }
}

//4D094
void INIT_GRAPHIC_SCREEN(void) {
    pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_GRAPHIC_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_GRAPHIC_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_GRAPHIC;
    pEND_SCREEN = END_GRAPHIC_SCREEN;
    if (GameModeVideo == MODE_NORMAL) {
        options.video_mode = 0;
        byte_E4CFE = 2;
    } else {
        options.video_mode = GameModeVideo == MODE_X && !P486 ? 1 : 2;
        byte_E4CFE = 1;
    }
    // DO_COMMANDE_OPTIONS saves pending music/volume in byte_E4CD8/9 before
    // entering this screen; INIT_OPTIONS_CHOICE restores them on return.
    options.SizeScreen = SizeScreen;
    options.Frequence = Frequence;
    options.fixon = fixon;
    options.BackgroundOn = BackgroundOptionOn;
    options.ScrollDiffOn = ScrollDiffOn;
    InitParamGraphicScreen();
}

//4D178
void INIT_AFFICHE_GRAPHIC_SCREEN(void) {
    menu_to_display[0].ypos = debut_options;
    for (s32 i = first_option; i <= first_option + nbre_options; ++i) {
        display_item_t* item = &menu_to_display[i];
        item->font_size = 2;
        item->is_fond = 1;
        item->field_D5 = 0;
        item->color = i == option_exit || i == 7 ? 3 : 5;
        strcpy(item->text, language_txt[i == option_exit ? 232 : 196 + i]);
        item->xpos = i == option_exit ? SCREEN_WIDTH / 2 : basex + 10;
        s32 row = i - first_option;
        if (options.video_mode == 0 || i == 1 || OptionGame || FondAutorise == 2) {
            ++row;
        }
        item->ypos = i == option_exit ? debut_sortie : debut_options + row * (ecart_options + 15);
        INIT_TXT_BOX(item);
    }
    graphics_screen_transition();
}

//4D55C
void AFFICHE_GRAPHIC_SCREEN(void) {
    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(language_txt[235], SCREEN_WIDTH / 2, debut_titre, 1, 1);
    display_text(language_txt[196], basex + 10, debut_options, 2, 5);
    // PC CalcSpeed estimates VGA transfer cost using DOS benchmark tables.
    // Those tables are not measurements of our renderer: show no estimate
    // rather than a misleading gauge derived from their default 255 values.
    display_text("--", byte_E4CFC + 102, debut_options, 2, 5);

    for (s32 i = first_option; i <= option_exit; ++i) {
        if ((options.video_mode != 0 && i == 2) ||
            (OptionGame && options.video_mode == 0 && i == 6)) {
            continue;
        }
        display_item_t* item = &menu_to_display[i];
        if (i == position) {
            display_box_text_plasma(item, 1);
        } else {
            display_text(item->text, item->xpos, item->ypos, 2, item->color);
        }
        switch (i) {
            case 1:
                display_text(options.video_mode == 0 ? "pci1" : options.video_mode == 1 ? "pci2" : "vesa",
                             byte_E4CFC + 118, item->ypos, 2, item->color);
                break;
            case 2:
                gauge(byte_E4CFC + 70, item->ypos - 12, 80, 12, 0, 3,
                      3 - options.SizeScreen);
                break;
            case 3: {
                bool rate60 = (options.Frequence == 0 && GameModeVideo == MODE_NORMAL) ||
                    (GameModeVideo == MODE_X && (options.Frequence == 0 || options.Frequence == 70 ||
                                               options.Frequence == 80 || options.Frequence == 100));
                bool rate50 = !rate60 && options.Frequence == 1;
                bool maximum = !rate60 && !rate50;
                // PC always draws the selected value and one alternative; the
                // second alternative is only shown in normal mode.
                if (maximum || options.video_mode == 0) {
                    graphics_screen_value("max", i, byte_E4CFC + 54, maximum);
                }
                if (!maximum || options.video_mode == 0) {
                    graphics_screen_value("60", i, byte_E4CFC + 102, rate60);
                }
                graphics_screen_value("50", i, byte_E4CFC + 134, rate50);
            } break;
            case 4:
            case 5:
            case 6: {
                if ((i == 5 && (!FondAutorise || (OptionGame && options.video_mode == 0))) ||
                    (i == 6 && (OptionGame || !FondAutorise || !options.BackgroundOn || options.video_mode != 0))) {
                    break;
                }
                bool on = i == 4 ? options.fixon : i == 5 ? options.BackgroundOn : options.ScrollDiffOn;
                graphics_screen_value("on", i, byte_E4CFC + 102, on);
                graphics_screen_value("off", i, byte_E4CFC + 134, !on);
            } break;
        }
    }
}

//4DA8C
void DO_COMMANDE_GRAPHIC(void) {
    TestCompteur();
    menuEtape = 4;
    if (SelectButPressed()) {
        MENU_RETURN = 1;
        TestButtonReleased();
        return;
    }
    graphics_screen_navigation(options.video_mode != 0);
    bool action = ValidButPressed() || rightjoy() || leftjoy();
    if (options.video_mode == 0) {
        if (position == 2 && !upjoy() && !downjoy() && button_released) {
            bool resized = false;
            if (leftjoy()) {
                options.SizeScreen = options.SizeScreen < 3 ? options.SizeScreen + 1 : 0;
                resized = true;
            } else if (rightjoy()) {
                options.SizeScreen = options.SizeScreen > 0 ? options.SizeScreen - 1 : 3;
                resized = true;
            }
            if (resized) {
                s32 size = options.SizeScreen;
                set_xywhmap(TabW1[size], TabW2[size], TabH1[size], TabH2[size]);
            }
        } else if (position == 6 && !OptionGame && button_released && action) {
            options.ScrollDiffOn = !options.ScrollDiffOn;
        }
    }
    if (position == 3 && !upjoy() && !downjoy() && button_released) {
        if (GameModeVideo == MODE_X && options.Frequence > byte_E4CFE) {
            options.Frequence = byte_E4CFE - 1;
        }
        if (ValidButPressed() || rightjoy()) {
            options.Frequence = options.Frequence < byte_E4CFE ? options.Frequence + 1 : 0;
            NewFrequency(options.Frequence);
        } else if (leftjoy()) {
            options.Frequence = options.Frequence > 0 ? options.Frequence - 1 : byte_E4CFE;
            NewFrequency(options.Frequence);
        }
    } else if (position >= 4 && position < option_exit && position != 6 && button_released && action) {
        if (position == 4) {
            options.fixon = !options.fixon;
        } else if (position == 5) {
            options.BackgroundOn = !options.BackgroundOn;
        }
        if (position == 5 && !OptionGame && !options.BackgroundOn && options.video_mode == 0 && graphics_mode_supported(1)) {
            // PC automatically selects PCI2 here. Keep its state/layout path,
            // but gate entry until the planar renderer is available. Normal
            // mode can already draw a black background in the portable port.
            options.video_mode = 1;
            byte_E4CFE = 1;
            P486 = 0;
            if (options.Frequence == 2) {
                options.Frequence = 0;
            }
            InitParamGraphicScreen();
            position = 5;
            INIT_AFFICHE_GRAPHIC_SCREEN();
            FonduPixel(18, debut_titre + 14, 270, menu_to_display[option_exit].ypos - debut_titre - 36);
        }
    } else if (position == 1 && ValidButPressed() && button_released) {
        menuEtape = 10;
        sortie_options = 1;
    }
    TestButtonReleased();
}

//4DF88
void END_GRAPHIC_SCREEN(void) {
    if (!MENU_RETURN) {
        SizeScreen = options.SizeScreen;
        Frequence = options.Frequence;
        fixon = options.fixon;
        BackgroundOptionOn = options.BackgroundOn;
        BackgroundOn = IsBackgroundOn();
        ScrollDiffOn = options.ScrollDiffOn;
        switch (options.video_mode) {
            case 0: GameModeVideo = MODE_NORMAL; P486 = 0; break;
            case 1: GameModeVideo = MODE_X; P486 = 0; break;
            case 2: GameModeVideo = MODE_X; P486 = 1; break;
        }
    } else {
        byte_96883 = 0;
        if (GameModeVideo == MODE_NORMAL) {
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
        }
        WaitNSynchro(25);
        NewFrequency(Frequence);
        if (!OptionGame) {
            DO_FADE_OUT();
        }
    }
}

//4E0A0
void INIT_VIDEOMODE_SCREEN(void) {
    pINIT_SCREEN = INIT_VIDEOMODE_SCREEN;
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_VIDEOMODE_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_VIDEOMODE_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_VIDEOMODE;
    pEND_SCREEN = END_VIDEOMODE_SCREEN;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    position = option_exit = nbre_options = 3;
    first_option = 0;
    byte_E4CFC = 140;
    basex = SCREEN_WIDTH / 2 - byte_E4CFC;
    general_init_screen(15, 68, 5);
    options.video_pci1 = 0;
    options.video_pci2 = 0;
    options.video_vesa = 0;
    if (GameModeVideo == MODE_NORMAL) {
        options.video_pci1 = 1;
    } else if (GameModeVideo == MODE_X && !P486) {
        options.video_pci2 = 1;
    } else {
        options.video_vesa = 1;
    }
}

//4E1CC
void INIT_AFFICHE_VIDEOMODE_SCREEN(void) {
    for (s32 i = 0; i <= option_exit; ++i) {
        display_item_t* item = &menu_to_display[i];
        strcpy(item->text, language_txt[i == option_exit ? 232 : 223 + i]);
        item->xpos = SCREEN_WIDTH / 2;
        item->ypos = i == option_exit ? debut_sortie : debut_options + i * (ecart_options + 15);
        item->font_size = 2;
        item->is_fond = 1;
        item->field_D5 = 0;
        item->color = i == option_exit ? 3 : 5;
        INIT_TXT_BOX(item);
    }
    graphics_screen_transition();
}

//4E470
void AFFICHE_VIDEOMODE_SCREEN(void) {
    const s16 selected[3] = {options.video_pci1, options.video_pci2, options.video_vesa};
    DISPLAY_FOND_MENU();
    display_text(language_txt[203], SCREEN_WIDTH / 2, debut_titre, 1, 1);
    for (s32 i = 0; i <= option_exit; ++i) {
        display_item_t* item = &menu_to_display[i];
        item->color = i == option_exit ? 3 : selected[i] ? 1 : 5;
        if (i == position) {
            display_box_text_plasma(item, 1);
        } else {
            display_text(item->text, item->xpos, item->ypos, 2, item->color);
        }
    }
}

//4E574
void DO_COMMANDE_VIDEOMODE(void) {
    TestCompteur();
    if (SelectButPressed()) {
        MENU_RETURN = 1;
        TestButtonReleased();
        return;
    }
    graphics_screen_navigation(false);
    if (button_released && position >= 0 && position < option_exit &&
        (ValidButPressed() || leftjoy() || rightjoy())) {
        s16* choices[3] = {&options.video_pci1, &options.video_pci2, &options.video_vesa};
        if (graphics_mode_supported(position)) {
            for (s32 i = 0; i < 3; ++i) {
                *choices[i] = i == position;
            }
        } else if (!*choices[position]) {
            // Preserve the original hardware choices as visible entries, but never
            // enter the unfinished Mode-X main loop. Only PCI1 maps to our renderer.
            display_item_t* message = &msg_to_display[0];
            strcpy(message->text, language == 1 ? "/mode video indisponible/escape : retour/" :
                language == 2 ? "/videomodus nicht verfugbar/escape : zuruck/" :
                "/video mode unavailable/escape : back/");
            message->xpos = SCREEN_WIDTH / 2;
            message->ypos = SCREEN_HEIGHT / 2;
            message->font_size = 2;
            message->is_fond = 2;
            message->field_D5 = 0;
            message->color = 1;
            INIT_TXT_BOX(message);
            display_box_msg(message);
        }
    }
    TestButtonReleased();
}

//4E708
void END_VIDEOMODE_SCREEN(void) {
    if (!MENU_RETURN) {
        if (options.video_pci1) {
            GameModeVideo = MODE_NORMAL;
            P486 = 0;
            BackgroundOptionOn = 1;
        } else {
            GameModeVideo = MODE_X;
            P486 = options.video_pci2 ? 0 : 1;
            if (Frequence == 2) {
                Frequence = 0;
                NewFrequency(0);
            }
        }
    }
    menuEtape = 9;
}

//4E790
void INIT_CALIBRATE_JOYSTICK(void) {
    print_once("Not implemented: INIT_CALIBRATE_JOYSTICK"); //stub
}

//4E804
void AFFICHE_CALIBRATE_JOYSTICK(void) {
    print_once("Not implemented: AFFICHE_CALIBRATE_JOYSTICK"); //stub
}

//4E99C
void calibrate_joystick_prg(void) {
    print_once("Not implemented: calibrate_joystick_prg"); //stub
}

//4E9E8
void AFFICHE_CENTER_JOYSTICK(void) {
    print_once("Not implemented: AFFICHE_CENTER_JOYSTICK"); //stub
}

//4EAE8
void center_joystick_prg(void) {
    print_once("Not implemented: center_joystick_prg"); //stub
}

//4EB28
void MAIN_CALIBRATE_JOYSTICK(void) {
    print_once("Not implemented: MAIN_CALIBRATE_JOYSTICK"); //stub
}

//4F17C
s16 menu_prg(u32 a1) {
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
    s16 need_exit = 0;
    if (fin_du_jeu || sortie_options || MENU_RETURN || ModeDemo) {
        need_exit = 1;
    }
    if (!need_exit) {
        if (pDO_COMMANDE && nb_fade == 0) {
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
    print_once("Not implemented: DO_INTER_MENU"); //stub
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
    if ((num_world == world_1_jungle && num_level == 9) ||
            (num_world == world_2_music && num_level == 4) ||
            (num_world == world_4_image && num_level == 9) ||
            (num_world == world_5_cave && num_level == 4)
    ) {
        menu_rvb = rvb_special[numero_palette_special];
    } else {
        menu_rvb = rvb[current_pal_id];
        rvb[current_pal_id] = rvb_plan3;
    }
    InitGauge();
    InitPlasma(0);
    SYNCHRO_LOOP(menu_prg);
    if (pEND_SCREEN) {
        pEND_SCREEN();
    }
    rvb[current_pal_id] = menu_rvb;
}

//4F488
void default_key(void) {
    check_key_table();
    for (s32 i = 0; i < 7; ++i) {
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
    for (s32 i = 0; i < 7; ++i) {
        *(tab_key[i]) = tab_key_sav[i];
    }
}

//4F50C
s32 CalcSpeed(void) {
    print_once("Not implemented: CalcSpeed");
    return 0; //stub
}

//4F698
void general_init_screen(s16 a1, s16 a2, s16 a3) {
    debut_titre = a1 + 11;
    ecart_options = (2 * a2 - 15 * nbre_options) / (nbre_options + 1);
    debut_options = 2 * a1 + ecart_options + 15;
    debut_sortie = a3 + (2 * a2) + 11 + (2 * a1);
    sortie_options = 0;
}

//4F76C
void TestCompteur(void) {
    if (compteur < max_compteur && button_released) {
        ++compteur;
    } else {
        if (button_released) {
            compteur = 0;
        } else {
            compteur = delai_repetition + 1;
        }
    }
}

//4F7B0
void TestButtonReleased(void) {
    if (upjoy() || downjoy() || rightjoy() || leftjoy() || ValidButPressed()) {
        button_released = 0;
    } else {
        button_released = 1;
        compteur = 0;
    }
}

//4F800
void display_box_text_plasma(display_item_t* box, u8 a2) {
    if (msg_box_being_displayed) {
        display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
    } else {
        PlasmaBox(box->centered_x_pos, box->centered_y_pos, box->width, box->height, a2);
        display_box_text(box);
    }
}

//4F878
void display_box_text_fire(display_item_t* a1) {
    FireBox(a1->centered_x_pos, a1->centered_y_pos, a1->width, a1->height);
    display_box_text(a1);
}

//4F8B4
s16 display_box_msg_prg(u32 a1) {
    yesno_finished = 0;
    readinput();
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    display_box_text_fire(dialog_display_item_ptr);
    if (pCOMMANDE_BOX) {
        pCOMMANDE_BOX();
    }
    if (SelectButPressed() || yesno_finished) {
        return 1;
    } else {
        return 0;
    }
}

//4F904
void display_box_msg(display_item_t* box) {
    dialog_display_item_ptr = box;
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    endsynchro();
    synchro();
    InitFire();
    SWAP_BUFFERS();
    pCOMMANDE_BOX = NULL;
    SYNCHRO_LOOP(display_box_msg_prg);
    msg_box_being_displayed = 0;
    ReInitPlasma = 1;
    RESET_ALL_TOUCHE();
}

//4F960
void display_box_msg_commande(display_item_t* box, void_func_t commande_box_func) {

    dialog_display_item_ptr = box;
    pCOMMANDE_BOX = commande_box_func;
    msg_box_being_displayed = 1;
    endsynchro();
    synchro();
    InitFire();

    do {
        if (pAFFICHE_SCREEN) {
            pAFFICHE_SCREEN();
        }
        display_box_text_fire(dialog_display_item_ptr);
        AfficheYesNo();
        endsynchro();
        synchro();
        DO_FADE();
        SWAP_BUFFERS();
        readinput();
    } while (ValidButPressed() || nb_fade != 0);

    RESET_ALL_TOUCHE(); // added: fix enter not released leading to immediate closure of dialog //TODO: find out the cause of this bug?

    SYNCHRO_LOOP(display_box_msg_prg);
    msg_box_being_displayed = 0;
    ReInitPlasma = 1;
    RESET_ALL_TOUCHE();
}

//4F9F0
void AfficheYesNo(void) {
    display_item_t* to_display = dialog_display_item_ptr;
    if (pos_YN == 0) {
        display_text(language_txt[147], to_display->xpos - 25, to_display->centered_y_pos + to_display->height, 2, 1);
        display_text(language_txt[148], to_display->xpos + 35, to_display->centered_y_pos + to_display->height, 2, 0);
    } else {
        display_text(language_txt[147], to_display->xpos - 25, to_display->centered_y_pos + to_display->height, 2, 0);
        display_text(language_txt[148], to_display->xpos + 35, to_display->centered_y_pos + to_display->height, 2, 1);
    }
}

//4FAC4
void DO_YESNOBIS(void) {
    TestCompteur();
    if ((leftjoy() || rightjoy()) && !(downjoy() || upjoy())) {
        if (button_released || (compteur > delai_repetition && (compteur % repetition == 0))) {
            pos_YN = !pos_YN;
            PlaySnd_old(68);
        }
    }
    AfficheYesNo();
    if (SelectButPressed()) {
        pos_YN = 1;
        yesno_finished = 1;
    }
    if (ValidButPressed() && button_released) {
        yesno_finished = 1;
    }
    TestButtonReleased();
}

//4FB94
u8 confirmation_msg(u8 which_message) {
    display_item_t* to_display = &msg_to_display[1];
    to_display->xpos = 160;
    to_display->ypos = 100;
    to_display->font_size = 2;
    to_display->is_fond = 2;
    to_display->field_D5 = 0;
    switch(which_message) {
        default: break;
        case 0: {
            pos_YN = 1;
            if (OptionGame) {
                strncpy(to_display->text, language_txt[131], sizeof(to_display->text)); // /do you really want/to quit the game ?/ /
            } else {
                strncpy(to_display->text, language_txt[132], sizeof(to_display->text)); // /return to dos !/are you sure ?/ /
            }
            INIT_TXT_BOX(to_display);
            display_box_msg_commande(to_display, DO_YESNOBIS);
        } break;
        case 1: {
            pos_YN = 1;
            pAFFICHE_SCREEN = AFFICHE_ECRAN_SAVE;
            strncpy(to_display->text, language_txt[129], sizeof(to_display->text)); // /do you really want to erase/this game ?/ /
            INIT_TXT_BOX(to_display);
            display_box_msg_commande(to_display, DO_YESNOBIS);
        } break;
        case 2: {
            pos_YN = 1;
            pAFFICHE_SCREEN = AFFICHE_ECRAN_SAVE;
            strncpy(to_display->text, language_txt[130], sizeof(to_display->text)); // /do you really want to erase/the previously saved game ?/ /
            INIT_TXT_BOX(to_display);
            display_box_msg_commande(to_display, DO_YESNOBIS);
        } break;
        case 3: {
            default_key();
            pos_YN = 1;
            rvb_menu_save = rvb[0];
            InitMenuPalette();
            CaptureVideo(global_game->prev_frame.memory, EffetBufferNormal, 1);
            DO_FADE_OUT();
            // not sure what happens here exactly (something with display buffer copying)
            rvb[0] = MenuPalette;
            INIT_FADE_IN();
            strncpy(to_display->text, language_txt[130], sizeof(to_display->text)); // /do you really want to erase/the previously saved game ?/ /
            pAFFICHE_SCREEN = DISPLAY_FOND_MENU;
            INIT_TXT_BOX(to_display);
            display_box_msg_commande(to_display, DO_YESNOBIS);
            DO_FADE_OUT();
            rvb[0] = rvb_menu_save;
            INIT_FADE_IN();
            new_key();
        } break;
        case 4: {
            default_key();
            pos_YN = 1;
            rvb_menu_save = rvb[0];
            InitMenuPalette();
            CaptureVideo(global_game->prev_frame.memory, EffetBufferNormal, 1);
            DO_FADE_OUT();
            // not sure what happens here exactly (something with display buffer copying)
            rvb[0] = MenuPalette;
            INIT_FADE_IN();
            strncpy(to_display->text, language_txt[131], sizeof(to_display->text)); // /do you really want/to quit the game ?/ /
            pAFFICHE_SCREEN = DISPLAY_FOND_MENU;
            INIT_TXT_BOX(to_display);
            display_box_msg_commande(to_display, DO_YESNOBIS);
            DO_FADE_OUT();
            rvb[0] = rvb_menu_save;
            INIT_FADE_IN();
            ReInitPlasma = 0;
            new_key();
        } break;

    }
    return pos_YN == 0;
}

//4FEF0
void SYNCHRO_LOOP_PALETTE(synchro_loop_palette_func_t func) {
    s16 ended = 0;
    do {
        endsynchro();
        synchro();
        SetPalette(&rvb_E40AC, 0, 256);
        u32 timer = 0;
        ended = func();
    } while(!ended);
}

// NOTE: ChatGPT (GPT-5.2) was used to help reconstruct the functions below (apparition_prg up until FonduOption)
//4FF3C
s16 apparition_prg(u32 a1) {
    (void)a1;

    s32 w = wFondu;
    s32 h = hFondu;
    s32 src_row_skip = SCREEN_WIDTH - w;

    DoCdRap();
    DISPLAY_FOND_MENU();

    u8* src = EffetBufferNormal + (SCREEN_WIDTH * SCREEN_HEIGHT) + yFondu * SCREEN_WIDTH + xFondu;

    s32 yoff = -((h-1)/2);
    const s32 yoff_end = h/2;

    while (yoff < yoff_end) {
        s32 dst_y = (SCREEN_HEIGHT/2) + EtapeFondu * yoff;

        if (dst_y > Bloc_lim_H1 && dst_y < Bloc_lim_H2) {
            u8* dst_row = (u8*)DrawBufferNormal + dst_y * SCREEN_WIDTH;

            s32 xoff = -((w-1)/2);
            s32 xoff_end = w/2;
            u8* s = src;
            while (xoff < xoff_end) {
                s32 dst_x = (SCREEN_WIDTH/2) + EtapeFondu * xoff;

                if (dst_x > Bloc_lim_W1 && dst_x < Bloc_lim_W2) {
                    dst_row[dst_x] = *s;
                }
                ++xoff;
                ++s;
            }

            src += w + src_row_skip; // advance exactly one source scanline
        } else {
            src += SCREEN_WIDTH; // skip full source scanline
        }

        ++yoff;
    }

    // Update EtapeFondu / decide end
    if (InOut != 0) {
        // decreasing until 1
        --EtapeFondu;
        return (EtapeFondu == 1) ? 1 : 0;
    } else {
        // increasing until 32; at 16 trigger INIT_FADE_OUT
        if (EtapeFondu == 16) {
            INIT_FADE_OUT();
        }
        ++EtapeFondu;
        return (EtapeFondu == 32) ? 1 : 0;
    }
}

//500FC
s16 fondu_prg(u32 a1) {
    (void)a1;

    s32 x = xFondu;
    s32 y = yFondu;
    s32 w = wFondu;
    s32 h = hFondu;

    s32 row_skip = SCREEN_WIDTH - w;

    u8* dst = DrawBufferNormal + y * SCREEN_WIDTH + x;
    u8* src = EffetBufferNormal + (SCREEN_WIDTH * SCREEN_HEIGHT) + y * SCREEN_WIDTH + x;

    s32 E = (s16)EtapeFondu;
    s32 mul = 16 - E;
    s32 div = 17 - E;

    for (s32 j = 0; j < h; ++j) {
        for (s32 i = 0; i < w; ++i) {
            u8 a = dst[i];
            u8 b = src[i];
            if (a != b) {
                dst[i] = (u8)((a * mul + b) / div);
            }
        }
        dst += w + row_skip;
        src += w + row_skip;
    }

    EtapeFondu = (s16)(E + 1);
    return (EtapeFondu == 16) ? 1 : 0;
}

//501E8
s16 change_couleur_prg(void) {
    // Preserve PC timing for expanded colors; RGB8 can also retain fractional
    // DAC units from assets, with a final shorter step to the exact target.
    const s32 step = use_rgb8_palettes ? 8 : 2;
    s16 done = 1;
    for (s32 i = 0; i < 256; ++i) {
        rgb_t* rgb_cur = rvb_E40AC.colors + i;
        rgb_t* rgb_goal = rvb_menu_save.colors + i;
        for (s32 c = 0; c < 3; ++c) {
            u8* cur = ((u8*)rgb_cur) + c;
            u8 goal = ((u8*)rgb_goal)[c];
            if (*cur == goal) {
                continue;
            }
            done = 0;
            if (*cur > goal + step) {
                *cur -= step;
            } else if (*cur + step < goal) {
                *cur += step;
            } else {
                *cur = goal;
            }
        }
    }
    return done;
}

//502FC
void FonduOption(s16 x, s16 y, s16 w, s16 h, u8 mode) {
    switch(mode) {
        case 1:
            EtapeFondu = 0;
            break;
        case 2:
            EtapeFondu = 1;
            InOut = 0;
            break;
        case 0:
            EtapeFondu = 32;
            InOut = 1;
            break;
    }
    xFondu = (x & ~3) + 4;
    yFondu = y;
    wFondu = (w & ~3) + 6;
    hFondu = h + 2;

    if (mode == 0) {
        if (GameModeVideo == MODE_NORMAL) {
            CaptureVideo(draw_buffer, EffetBufferNormal, 1);
            if (RayEvts.firefly) {
                set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
                sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            }
            DO_FADE_OUT();
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            N_CLRSCR(DrawBufferNormal);
        } else {
            CaptureVideo(display_buffer, EffetBufferNormal, 1);
            DO_FADE_OUT();
            InitModeNormalWithFrequency(VGA_FREQ);
            NewFrequency(Frequence);
        }

        if (
            (num_world == 1 && num_level == 9) ||
            (num_world == 2 && num_level == 4) ||
            (num_world == 4 && num_level == 9) ||
            (num_world == 5 && num_level == 4)
        ) {
            rvb[current_pal_id] = rvb_special[15];
        }

        InitMenuPalette();
        draw_buffer = DrawBufferNormal;
        DISPLAY_FOND_MENU();
    } else if (mode == 1) {
        for (s32 i = 0; i < 256; ++i) {
            rgb_t rgb = MenuPalette.colors[i];
            u8 gray = palette_gray_index(rgb);
            rvb_menu_save.colors[i] = MenuPalette.colors[gray];
        }
        rvb_E40AC = MenuPalette;
        SYNCHRO_LOOP_PALETTE(change_couleur_prg);

        // pixel remap to menu gray index
        u8* row = DrawBufferNormal + SCREEN_WIDTH * yFondu + xFondu;
        for (s32 j = 0; j < hFondu; ++j) {
            u8* pos = row;
            for (s32 i = 0; i < wFondu; ++i) {
                u8 src = *pos;
                rgb_t rgb = MenuPalette.colors[src];
                u8 gray = palette_gray_index(rgb);
                *pos++ = gray;
            }
            row += SCREEN_WIDTH;
        }

    }

    // Draw menu to offscreen area
    draw_buffer = EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT;
    pAFFICHE_SCREEN();
    draw_buffer = DrawBufferNormal;

    if (mode == 1) {
        // pixel remap to menu gray index
        u8* row = (EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT) + SCREEN_WIDTH * yFondu + xFondu;
        for (s32 j = 0; j < hFondu; ++j) {
            u8* pos = row;
            for (s32 i = 0; i < wFondu; ++i) {
                u8 src = *pos;
                rgb_t rgb = MenuPalette.colors[src];
                u8 gray = palette_gray_index(rgb);
                *pos++ = gray;
            }
            row += SCREEN_WIDTH;
        }
    }

    if (mode == 0) {
        rvb_menu_save = rvb[current_pal_id];
        rvb[current_pal_id] = MenuPalette;
        INIT_FADE_IN();
        SYNCHRO_LOOP(apparition_prg);
        rvb[current_pal_id] = rvb_menu_save;
    } else if (mode == 1) {
        SYNCHRO_LOOP(fondu_prg);
        for (s32 i = 0; i < 256; ++i) {
            rgb_t rgb = MenuPalette.colors[i];
            u8 gray = palette_gray_index(rgb);
            rvb_E40AC.colors[i] = MenuPalette.colors[gray];
        }
        rvb_menu_save = MenuPalette;
        pAFFICHE_SCREEN();
        SYNCHRO_LOOP_PALETTE(change_couleur_prg);

    } else if (mode == 2) {
        // DO_MENU restores this palette before the outgoing transition.
        rvb_menu_save = rvb[current_pal_id];

        rvb[current_pal_id] = MenuPalette;
        SYNCHRO_LOOP(apparition_prg);
        if (GameModeVideo == MODE_X) {
            InitModeXWithFrequency(VGA_FREQ);
            NewFrequency(Frequence);
        }

        if (
            (num_world == 1 && num_level == 9) ||
            (num_world == 2 && num_level == 4) ||
            (num_world == 4 && num_level == 9) ||
            (num_world == 5 && num_level == 4)
        ) {
            rvb[current_pal_id] = rvb_special[numero_palette_special];
        } else {
            rvb[current_pal_id] = rvb_menu_save;
        }

        if (current_pal_id != 0) {
            rvb[0] = rvb_options_in_game;
        }
        INIT_FADE_IN();

    }
}

//50A38
s16 FonduPixel_prg(u32 a1) {
    (void)a1;
    const s32 step = EtapeFondu;
    if (step <= 0) {
        return 1;
    }

    const s32 left = MAX(0, xFondu);
    const s32 top = MAX(0, yFondu);
    const s32 right = MIN(SCREEN_WIDTH, (s32)xFondu + wFondu);
    const s32 bottom = MIN(SCREEN_HEIGHT, (s32)yFondu + hFondu);
    const u8* source = EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT;

    for (s32 y = top; y < bottom; ++y) {
        /* PC carries the sample counter across rows, excluding scanline padding.
         * Keep that phase relative to the requested rectangle even when clipped.
         * It tests before incrementing, so pixel zero is deliberately untouched;
         * the ordinary menu redraw supplies that corner after the transition.
         */
        s32 pixel_index = (y - yFondu) * wFondu + left - xFondu;
        for (s32 x = left; x < right; ++x, ++pixel_index) {
            if (pixel_index != 0 && pixel_index % step == 0) {
                const s32 offset = y * SCREEN_WIDTH + x;
                DrawBufferNormal[offset] = source[offset];
            }
        }
    }

    --EtapeFondu;
    return EtapeFondu == 0;
}

//50B0C
void FonduPixel(s16 x, s16 y, s16 w, s16 h) {
    xFondu = x;
    yFondu = y;
    wFondu = w;
    hFondu = h;
    EtapeFondu = 16;
    draw_buffer = EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT;
    pAFFICHE_SCREEN();
    draw_buffer = DrawBufferNormal;
    /* PC runs all 16 display ticks here. Android retains the preparation and
     * callback, but its FonduPixel wrapper omits this blocking loop.
     */
    SYNCHRO_LOOP(FonduPixel_prg);
}
