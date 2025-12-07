
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

i8 byte_49290[4] = {0, 0, 0, 0}; //49290

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
void NewFrequency(u8 a1) {
    print_once("Not implemented: NewFrequency"); //stub
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
    print_once("Not implemented: IsBackgroundOn");
    return true; //stub
}

//498A4
void init_key2txt(void) {
    print_once("Not implemented: init_key2txt"); //stub
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
    i16 NewMenusArgs = 0;
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
    //SaveOptionsOnDisk();

}

//49B7C
void DO_OPTIONS_IN_GAME(void) {
    if (GoMenu == 1) {
        //NOTE: temporary method for quitting out of a level (will return to the main menu)
        new_world = 1;
        new_level = 1;
        GoMenu = 0;
    }
    print_once("Not implemented: DO_OPTIONS_IN_GAME"); //stub
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
    for (i32 i = 0; i < nbre_options; ++i) {
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
    for (i32 i = 0; i < nbre_options; ++i) {
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
    for (i32 i = first_option; i <= first_option + nbre_options; ++i) {
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
                    i32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
                                     calc_largmax_text(v39, -1, 8, 1, 2) + calc_largmax_text(v32, -1, 8, 1, 2);
                    to_display->xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 1: {
                    // Text "sound" offsetted to the left
                    i32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
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
        i16 v21 = debut_titre - 25;
        i16 v42_hi = menu_to_display[option_exit].ypos - debut_titre + 35;
        i16 v22_x = (basex >= word_E4CEC_x) ? word_E4CEC_x : basex;
        i16 v23_y = word_E4CF0_y;
        if (v21 < word_E4CF0_y) {
            v23_y = debut_titre - 25;
        }
        i16 v24 = word_E4CF4_w + word_E4CEC_x;
        i32 v37 = basex;
        i32 v33 = v24;
        i32 v25 = basex + 2 * byte_E4CFC;
        i32 v34 = (v25 >= v24) ? v25 - v22_x : v24 - v22_x;
        i16 unk_w = v34;
        i32 v26 = word_E4CEE_h + word_E4CF0_y;
        i32 v31 = v21;
        i32 v38 = v42_hi;
        i32 v36 = v42_hi + v21;
        if (v26 <= v36) {
            v26 = v36;
        }
        i32 v27_h = v26 - v23_y;
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
    for (i32 option_index = first_option; option_index <= first_option + nbre_options; ++option_index) {
        i16 x = menu_to_display[option_index].xpos;
        i16 y = menu_to_display[option_index].ypos;
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
            i32 v5 = calc_largmax_text(menu_to_display[0].text, -1, 8, 1, 2);
            i32 v6 = v5 + x;
            const char* nonselected_text;
            const char* other_text;
            i32 v18;
            i32 v8;
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
            i32 draw_x = x + calc_largmax_text(menu_to_display[1].text, -1, 8, 1, 2u) + 16;
            DrawBlackBorderBox(draw_x, y - 14, 15, 20 * ecart_barre + 1, 1);
            for (i32 i = 0; i < options.sound_volume; ++i) {
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
    print_once("Not implemented: INIT_KEY_SCREEN"); //stub
}

//4B514
void INIT_AFFICHE_KEY_SCREEN(void) {
    print_once("Not implemented: INIT_AFFICHE_KEY_SCREEN"); //stub
}

//4B8A4
void AFFICHE_KEY_SCREEN(void) {
    print_once("Not implemented: AFFICHE_KEY_SCREEN"); //stub
}

//4BACC
void DO_COMMANDE_KEY(void) {
    print_once("Not implemented: DO_COMMANDE_KEY"); //stub
}

//4BD50
void END_KEY_SCREEN(void) {
    print_once("Not implemented: END_KEY_SCREEN"); //stub
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

//4CD94
void InitParamGraphicsScreen(void) {
    print_once("Not implemented: InitParamGraphicsScreen"); //stub
}

//4D094
void INIT_GRAPHIC_SCREEN(void) {
    print_once("Not implemented: INIT_GRAPHIC_SCREEN"); //stub
}

//4D178
void INIT_AFFICHE_GRAPHIC_SCREEN(void) {
    print_once("Not implemented: INIT_AFFICHE_GRAPHIC_SCREEN"); //stub
}

//4D55C
void AFFICHE_GRAPHIC_SCREEN(void) {
    print_once("Not implemented: AFFICHE_GRAPHIC_SCREEN"); //stub
}

//4DA8C
void DO_COMMANDE_GRAPHIC(void) {
    print_once("Not implemented: DO_COMMANDE_GRAPHIC"); //stub
}

//4DF88
void END_GRAPHIC_SCREEN(void) {
    print_once("Not implemented: END_GRAPHIC_SCREEN"); //stub
}

//4E0A0
void INIT_VIDEOMODE_SCREEN(void) {
    print_once("Not implemented: INIT_VIDEOMODE_SCREEN"); //stub
}

//4E1CC
void INIT_AFFICHE_VIDEOMODE_SCREEN(void) {
    print_once("Not implemented: INIT_AFFICHE_VIDEOMODE_SCREEN"); //stub
}

//4E470
void AFFICHE_VIDEOMODE_SCREEN(void) {
    print_once("Not implemented: AFFICHE_VIDEOMODE_SCREEN"); //stub
}

//4E574
void DO_COMMANDE_VIDEOMODE(void) {
    print_once("Not implemented: DO_COMMANDE_VIDEOMODE"); //stub
}

//4E708
void END_VIDEOMODE_SCREEN(void) {
    print_once("Not implemented: END_VIDEOMODE_SCREEN"); //stub
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
    rgb_palette_t* palette;
    if ((num_world == world_1_jungle && num_level == 9) ||
            (num_world == world_2_music && num_level == 4) ||
            (num_world == world_4_image && num_level == 4) ||
            (num_world == world_5_cave && num_level == 4)
    ) {
        palette = rvb_special + numero_palette_special;
    } else {
        palette = rvb + current_pal_id;
        rvb[current_pal_id] = rvb_plan3;
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
    print_once("Not implemented: CalcSpeed");
    return 0; //stub
}

//4F698
void general_init_screen(i16 a1, i16 a2, i16 a3) {
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
i16 display_box_msg_prg(u32 a1) {
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
void SYNCHRO_LOOP_PALETTE(void* func) {
    print_once("Not implemented: SYNCHRO_LOOP_PALETTE"); //stub
}

//4FF3C
void apparition_prg(void) {
    print_once("Not implemented: apparition_prg"); //stub
}

//500FC
void fondu_prg(void) {
    print_once("Not implemented: fondu_prg"); //stub
}

//501E8
void change_couleur_prg(void) {
    print_once("Not implemented: change_couleur_prg"); //stub
}

//502FC
void FonduOption(i16 x, i16 y, i16 w, i16 h, u8 a5) {
    print_once("Not implemented: FonduOption"); //stub
}

//50A38
i16 FonduPixel_prg(u32 a1) {
    print_once("Not implemented: FonduPixel_prg");
    return false; //stub
}

//50B0C
void FonduPixel(i16 x, i16 y, i16 w, i16 h) {
    xFondu = x;
    yFondu = y;
    wFondu = w;
    hFondu = h;
    draw_buffer = EffetBufferNormal + 64000;
    pAFFICHE_SCREEN();
    draw_buffer = DrawBufferNormal;
    SYNCHRO_LOOP(FonduPixel_prg);
}
