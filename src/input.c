
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
s32 leftjoy(void) {
    s32 result = 0;
    if (Main_Control == 2) {
        print_once("Not implemented: leftjoy"); //stub?
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
s32 rightjoy(void) {
    s32 result = 0;
    if (Main_Control == 2) {
        print_once("Not implemented: rightjoy"); //stub?
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
s32 downjoy(void) {
    s32 result = 0;
    if (Main_Control == 2) {
        print_once("Not implemented: downjoy"); //stub?
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
s32 upjoy(void) {
    s32 result = 0;
    if (Main_Control == 2) {
        print_once("Not implemented: upjoy"); //stub?
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
s32 but0pressed(void) {
    s32 result = 0;
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
s32 but1pressed(void) {
    s32 result = 0;
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
s32 but2pressed(void) {
    s32 result = 0;
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
s32 but3pressed(void) {
    s32 result = 0;
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
s32 butX0pressed(void) {
    bool result = false;
    if (nb_fade == 0) {
        result = bux01 == 1 || (input_mode == 1 && TEST_KEY_BUTTON2());
    }
    return result;
}

//4210C
s32 butX1pressed(void) {
    bool result = false;
    if (nb_fade == 0) {
        result = bux11 == 1;
    }
    return result;
}

//4212C
void sub_4212C(s16 a1) {
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
    print_once("Not implemented: readinput_calibrate_joystick"); //stub
}

//42184
void readinput_center_joystick(void) {
    print_once("Not implemented: readinput_center_joystick"); //stub
}

//421C0
void update_pad_limits(s16* xmax, s16* xmin, s16* ymax, s16* ymin, s16* xcentre, s16* ycentre) {
    print_once("Not implemented: update_pad_limits"); //stub
}

//4225C
void DemoUpdateRemainingPercent(void) {
    print_once("Not implemented: DemoUpdateRemainingPercent"); //stub
}

//422C8
bool read_record(const char* filename, record_t* rec) {
    if (!filename || !rec || !temp_mem_buf) {
        return false;
    }

    FILE* file = open_data_file(filename, false);
    if (!file) {
        printf("FILE NOT FOUND: %s\n", filename);
        return false;
    }

    bool loaded = false;
    if (fseek(file, 0, SEEK_END) == 0) {
        long file_size = ftell(file);
        if (file_size >= 2 && file_size <= 0x7fffffffL &&
            (file_size & 1) == 0 && fseek(file, 0, SEEK_SET) == 0) {
            u8* data = block_malloc(temp_mem_buf, (u32) file_size);
            if (fread(data, 1, (size_t) file_size, file) == (size_t) file_size) {
                rec->current_offset = 0;
                rec->repeat_length = 0;
                rec->repeat_index = 0;
                rec->length = (s32) file_size;
                rec->data = data;
                rec->is_finished = 0;
                loaded = true;
            }
        }
    }
    fclose(file);
    return loaded;
}

//42328
bool load_record(record_t* rec) {
    /*
     * The PC developer path switches to text mode, asks for a name, appends
     * ".REC", and defaults to DANY.REC.  The normal menu demos do not use it:
     * they point directly into DemoRecord.  Keep the default-file path useful
     * without reproducing a blocking DOS console prompt in the modern port.
     */
    return read_record("DANY.REC", rec);
}

//42408
void sub_42408(u8 a1) {
    joy_buttonB2 = 0;
    joy_buttonB1 = 0;
    joy_buttonA1 = 0;

    if (a1 & 0x01) {
        joy_buttonA1 = 1;
    }
    if (a1 & 0x02) {
        joy_buttonB1 = 1;
    }

    /* Both of the original middle bits feed the same logical A2 button. */
    joy_buttonA2 = (a1 & 0x0c) != 0;
    joy_rec_left = (a1 & 0x10) != 0;
    joy_rec_right = (a1 & 0x20) != 0;
    joy_rec_up = (a1 & 0x40) != 0;
    joy_rec_down = (a1 & 0x80) != 0;
}

//42490
void do_record(record_t* rec) {
    /* Preserve the physical-button snapshot used by the two extended tests. */
    bux00 = joy_buttonA1;
    bux01 = joy_buttonA2;
    bux10 = joy_buttonB1;
    bux11 = joy_buttonB2;

    if (!rec || !rec->is_playing) {
        return;
    }

    if (rec->current_offset == 0 && fee_obj_id == -1 && ModeDemo == 0) {
        load_record(rec);
    }

    /* A stream is a sequence of [input mask, duration] byte pairs. */
    if (!rec->data || rec->length < 2 || (rec->length & 1) != 0) {
        rec->repeat_index = rec->repeat_length;
        rec->is_playing = 0;
        sub_42408(0);
        if (ModeDemo == 1) {
            ModeDemo = 2;
            new_world = 1;
            DO_FADE_OUT();
        }
        return;
    }

    if (rec->current_offset == 0) {
        rec->repeat_index = 0;
        rec->current_offset = 1;
        rec->repeat_length = rec->data[1];
        rec->is_finished = 0;
        sub_42408(rec->data[0]);
        return;
    }

    if (rec->current_offset < 1 || rec->current_offset >= rec->length) {
        rec->repeat_index = rec->repeat_length;
        rec->is_playing = 0;
        sub_42408(0);
        if (ModeDemo == 1) {
            ModeDemo = 2;
            new_world = 1;
            DO_FADE_OUT();
        }
        return;
    }

    ++rec->repeat_index;
    if (rec->repeat_index >= rec->repeat_length) {
        if (rec->is_finished || rec->current_offset + 1 >= rec->length) {
            rec->is_playing = 0;
            if (ModeDemo == 1) {
                ModeDemo = 2;
                new_world = 1;
                DO_FADE_OUT();
            }
        } else {
            rec->current_offset += 2;
            rec->repeat_index = 0;
            rec->repeat_length = rec->data[rec->current_offset];
        }
    }

    /* PC reapplies the current pair even on the tick that ends playback. */
    sub_42408(rec->data[rec->current_offset - 1]);
}

//4259C
void PC_keyboard_interrupt_handler(u8 scancode) {
    u8 v3 = 0;
    if (ModeDemo == 1 && (fade & 0x40) != 0) {
        if (scancode == SC_ESCAPE) {
            ModeDemo = 4;
        } else {
            ModeDemo = 3;
        }
    }
    if (!(scancode & 0x80) && (scancode ^ 0x96) != text_input_buffer[0]) {
        need_check_cheats = 1;
        for (s32 i = 8; i > 0; --i) {
            text_input_buffer[i] = text_input_buffer[i-1];
        }
        text_input_buffer[0] = scancode ^ 0x96;
    }
    if (dans_la_map_monde) {
        ModeAutoJumelle = 0;
        ChangeJumelleSizeOK &= ~4;
        if (PositionStageNameCalcule == 1) {
            PositionStageNameCalcule = 2;
        }
    }
    if (keyboard_pause_sequence_state != 0) {
        if (keyboard_pause_sequence_state <= 1) {
            if (scancode == SC_CONTROL) {
                keyboard_pause_sequence_state = 2;
            } else {
                keyboard_pause_sequence_state = 0;
            }
        } else if (keyboard_pause_sequence_state == 2) {
            if (scancode == SC_NUMLOCK && RaymanDansUneMapDuJeu) {
                in_pause = !in_pause;
            }
            keyboard_pause_sequence_state = 0;
        } else {
            keyboard_pause_sequence_state = 0;
        }
    } else if (scancode == 0xE1) { // PC Pause make sequence: E1 1D 45.
        keyboard_pause_sequence_state = 1;
    } else {
        v3 = 1;
    }
    if (v3) {
        Touche_Enfoncee[scancode & 0x7F] = (scancode & 0x80) ^ 0x80;
    }
    print_once_dos("Not implemented: PC_keyboard_interrupt_handler"); //stub
}

//4270C
void RESET_TOUCHE(u8 key) {
    Touche_Enfoncee[key & 0x7F] = 0;
}

//4271C
void RESET_ALL_TOUCHE(void) {
    for (s32 i = 0; i < COUNT(Touche_Enfoncee); ++i) {
        Touche_Enfoncee[i] = 0;
    }
}

//42734
void Init_Clavier(void) {
    print_once("Not implemented: Init_Clavier"); //stub
}

//42774
void Reset_Clavier(void) {
    print_once("Not implemented: Reset_Clavier"); //stub
}

//42790
bool JoystickPresent(void) {
    print_once("Not implemented: JoystickPresent");
    return false; //stub
}
