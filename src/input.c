
//41AF0
void POINTEUR_BOUTONS_OPTIONS_BIS(void) {
    switch(options_jeu.jump) {
        case 0: {
            options_jeu.test_fire1 = but0pressed;
            TEST_KEY_BUTTON0 = TestKeyJump;
        } break;
        case 1: {
            options_jeu.test_fire1 = but1pressed;
            TEST_KEY_BUTTON1 = TestKeyJump;
        } break;
        case 2: {
            options_jeu.test_fire1 = but2pressed;
            TEST_KEY_BUTTON2 = TestKeyJump;
        } break;
        case 3: {
            options_jeu.test_fire1 = but3pressed;
            TEST_KEY_BUTTON3 = TestKeyJump;
        } break;
        default: break;
    }

    switch(options_jeu.fist) {
        case 0: {
            options_jeu.test_fire0 = but0pressed;
            TEST_KEY_BUTTON0 = TestKeyWeapon;
        } break;
        case 1: {
            options_jeu.test_fire0 = but1pressed;
            TEST_KEY_BUTTON1 = TestKeyWeapon;
        } break;
        case 2: {
            options_jeu.test_fire0 = but2pressed;
            TEST_KEY_BUTTON2 = TestKeyWeapon;
        } break;
        case 3: {
            options_jeu.test_fire0 = but3pressed;
            TEST_KEY_BUTTON3 = TestKeyWeapon;
        } break;
        default: break;
    }

    switch(options_jeu.field_14) {
        case 0: {
            options_jeu.test_button4 = but0pressed;
        } break;
        case 1: {
            options_jeu.test_button4 = but1pressed;
        } break;
        case 2: {
            options_jeu.test_button4 = but2pressed;
        } break;
        case 3: {
            options_jeu.test_button4 = but3pressed;
        } break;
        default: break;
    }

    switch(options_jeu.action) {
        case 0: {
            options_jeu.test_button3 = but0pressed;
            TEST_KEY_BUTTON0 = TestKeyAction;
        } break;
        case 1: {
            options_jeu.test_button3 = but1pressed;
            TEST_KEY_BUTTON1 = TestKeyAction;
        } break;
        case 2: {
            options_jeu.test_button3 = but2pressed;
            TEST_KEY_BUTTON2 = TestKeyAction;
        } break;
        case 3: {
            options_jeu.test_button3 = but3pressed;
            TEST_KEY_BUTTON3 = TestKeyAction;
        } break;
        default: break;
    }
}

//41CA8
u8 TestKeyWeapon(void) {
   // return key_fist; // this seems to be a bug in the original game
    return Touche_Enfoncee[key_fist & 0x7F];
}

//41CB0
u8 TOUCHE(u8 key) {
    return Touche_Enfoncee[key & 0x7F];
}

//41CBC
u8 TestKeyJump(void) {
    return Touche_Enfoncee[key_jump & 0x7F];
}

//41CD0
u8 TestKeyAction(void) {
    return Touche_Enfoncee[key_action & 0x7F];
}

//41CE4
i32 leftjoy(void) {
    i32 result = 0;
    if (Main_Control == 2) {
        //stub?
    }
    if (input_mode == 1) {
        result |= (TOUCHE(key_left) && !(TOUCHE(key_right)));
    }
    if (record.is_playing) {
        result = joy_rec_left;
    }
    return result;
}

//41D54
i32 rightjoy(void) {
    i32 result = 0;
    if (Main_Control == 2) {
        //stub?
    }
    if (input_mode == 1) {
        result |= (TOUCHE(key_right) && !(TOUCHE(key_left)));
    }
    if (record.is_playing) {
        result = joy_rec_right;
    }
    return result;
}

//41DC4
i32 downjoy(void) {
    i32 result = 0;
    if (Main_Control == 2) {
        //stub?
    }
    if (input_mode == 1) {
        result |= (TOUCHE(key_down) && !(TOUCHE(key_up)));
    }
    if (record.is_playing) {
        result = joy_rec_down;
    }
    return result;
}

//41E34
i32 upjoy(void) {
    i32 result = 0;
    if (Main_Control == 2) {
        //stub?
    }
    if (input_mode == 1) {
        result |= (TOUCHE(key_up) && !(TOUCHE(key_down)));
    }
    if (record.is_playing) {
        result = joy_rec_up;
    }
    return result;
}

//41EA4
i32 but0pressed(void) {
    i32 result = 0;
    if (nb_fade == 0) {
        if (Main_Control && notbut[0] != 1) {
            result = (joy_buttonA1 == 1 || (input_mode == 1 && TEST_KEY_BUTTON0()));
        } else {
            result = (input_mode == 1 && TEST_KEY_BUTTON0());
            if (ModeDemo) {
                result |= (joy_buttonA1 == 1);
            }
        }
    }
    return result;
}

//41F30
i32 but1pressed(void) {
    i32 result = 0;
    if (nb_fade == 0) {
        if (Main_Control && notbut[1] != 1) {
            result = (joy_buttonA2 == 1 || (input_mode == 1 && TEST_KEY_BUTTON1()));
        } else {
            result = (input_mode == 1 && TEST_KEY_BUTTON1());
            if (ModeDemo) {
                result |= (joy_buttonA2 == 1);
            }
        }
    }
    return result;
}

//41FBC
i32 but2pressed(void) {
    i32 result = 0;
    if (nb_fade == 0) {
        if (Main_Control && notbut[2] != 1) {
            result = (joy_buttonB1 == 1 || (input_mode == 1 && TEST_KEY_BUTTON2()));
        } else {
            result = (input_mode == 1 && TEST_KEY_BUTTON2());
            if (ModeDemo) {
                result |= (joy_buttonB1 == 1);
            }
        }
    }
    return result;
}

//42048
i32 but3pressed(void) {
    i32 result = 0;
    if (nb_fade == 0) {
        if (Main_Control && notbut[3] != 1) {
            result = (joy_buttonB2 == 1 || (input_mode == 1 && TEST_KEY_BUTTON3()));
        } else {
            result = (input_mode == 1 && TEST_KEY_BUTTON3());
            if (ModeDemo) {
                result |= (joy_buttonA1 == 1); // NOTE: original code says A1, not B2 (maybe a bug?)
            }
        }
    }
    return result;
}

//420D4
i32 butX0pressed(void) {
    bool result = false;
    if (nb_fade == 0) {
        result = bux01 == 1 || (input_mode == 1 && TEST_KEY_BUTTON2());
    }
    return result;
}

//4210C
i32 butX1pressed(void) {
    bool result = false;
    if (nb_fade == 0) {
        result = bux11 == 1;
    }
    return result;
}

//4212C
void sub_4212C(i16 a1) {
    word_E0CD0 = a1;
}

//42134
void readinput(void) {
    if (Read_Input_Func) {
        Read_Input_Func();
    }
}

//4213C
void readinput_calibrate_joystick(void) {
    //stub
}

//42184
void readinput_center_joystick(void) {
    //stub
}

//421C0
void update_pad_limits(i16* xmax, i16* xmin, i16* ymax, i16* ymin, i16* xcentre, i16* ycentre) {
    //stub
}

//4225C
void sub_4225C(void) {
    //stub
}

//422C8
void read_record(const char* filename) {
    //stub
}

//42328
void load_record(void) {
    //stub
}

//42408
void sub_42408(u8 a1) {
    //stub
}

//42490
void do_record(record_t* rec) {
    //stub
}

//4259C
void keyboard_interrupt_handler(void) {
    //stub
}

//425AB
void sub_425AB(void) {
    //stub
}

//4270C
void RESET_TOUCHE(u8 key) {
    Touche_Enfoncee[key & 0x7F] = 0;
}

//4271C
void RESET_ALL_TOUCHE(void) {
    for (i32 i = 0; i < COUNT(Touche_Enfoncee); ++i) {
        Touche_Enfoncee[i] = 0;
    }
}

//42734
void Init_Clavier(void) {
    //stub
}

//42774
void Reset_Clavier(void) {
    //stub
}

//42790
bool JoystickPresent(void) {
    return true; //stub
}
