
//41AF0
void POINTEUR_BOUTONS_OPTIONS_BIS(void) {
    //stub
}

//41CA8
u8 TestKeyWeapon(void) {
    return key_fist;
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
    return 0; //stub
}

//41D54
i32 rightjoy(void) {
    return 0; //stub
}

//41DC4
i32 downjoy(void) {
    return 0; //stub
}

//41E34
i32 upjoy(void) {
    return 0; //stub
}

//41EA4
i32 but0pressed(void) {
    return 0; //stub
}

//41F30
i32 but1pressed(void) {
    return 0; //stub
}

//41FBC
i32 but2pressed(void) {
    return 0; //stub
}

//42048
i32 but3pressed(void) {
    return 0; //stub
}

//420D4
i32 butX0pressed(void) {
    return 0; //stub
}

//4210C
i32 butX1pressed(void) {
    return 0; //stub
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
