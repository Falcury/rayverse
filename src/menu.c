
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
        u8 key = *(key_table[i]);
        if (key > SC_DELETE || key_descriptions_qwerty[key] == 0 || key_descriptions_azerty[key] == 0) {
            valid = false;
            break; // added
        }
    }
    if (!valid) {
        // load defaults
        *(key_table[0]) = SC_LEFT;
        *(key_table[1]) = SC_UP;
        *(key_table[2]) = SC_RIGHT;
        *(key_table[3]) = SC_DOWN;
        *(key_table[4]) = SC_CONTROL;
        *(key_table[5]) = SC_ALT;
        *(key_table[6]) = SC_X;
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
    //stub
}

//49B7C
void DO_OPTIONS_IN_GAME(void) {
    //stub
}

//49E98
void INIT_GENERAL_CHOICE(void) {
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
void menu_prg(void) {
    //stub
}

//4F2E0
void DO_INTER_MENU(void) {
    //stub
}

//4F318
void DO_MENU(void) {
    //stub
}

//4F488
void default_key(void) {
    //stub
}

//4F4EC
void new_key(void) {
    //stub
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
