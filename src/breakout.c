
//26A20
u8 get_casse_brique_ON(void) {
    return CasseBriqueON;
}

//26A28
void set_casse_brique_ON(u8 on) {
    CasseBriqueON = on;
}

//26A30
u8 get_casse_brique_active(void) {
    return casse_brique_active;
}

//26A38
void set_casse_brique_active(void) {
    casse_brique_active = 1;
}

//26A40
void enter_casse_brique(void) {
    if (casse_brique_active) {
        old_num_level_choice = num_level_choice;
        num_level_choice = 22;
        old_num_world_choice = num_world_choice;
        num_world_choice = 1;
        num_world = 1;
        CasseBriqueON = 1;
    }
}

//26A90
void enter_casse_brique2(void) {
    print_once("Not implemented: enter_casse_brique2"); //stub
}

//26A9C
void exit_casse_brique(void) {
    print_once("Not implemented: exit_casse_brique"); //stub
}

//26AB8
void exit_casse_brique2(void) {
    print_once("Not implemented: exit_casse_brique2"); //stub
}

//26AD0
i32 get_speed_ball(void) {
    print_once("Not implemented: get_speed_ball");
    return 0; //stub
}

//26C00
void speed_ball_null(obj_t* obj) {
    print_once("Not implemented: speed_ball_null"); //stub
}

//26C30
void speed_ball_init(obj_t* obj) {
    print_once("Not implemented: speed_ball_init"); //stub
}

//26C5C
void speed_up_ball(obj_t* obj) {
    print_once("Not implemented: speed_up_ball"); //stub
}

//26C8C
void invert_speed_ball_x(obj_t* obj) {
    print_once("Not implemented: invert_speed_ball_x"); //stub
}

//26CB8
void invert_speed_ball_y(obj_t* obj) {
    print_once("Not implemented: invert_speed_ball_y"); //stub
}

//26CE4
void set_ball_pos(obj_t* obj, i16 a2, i16 a3) {
    print_once("Not implemented: set_ball_pos"); //stub
}

//26D0C
void angle_direction(i16 a1, i16 a2) {
    print_once("Not implemented: angle_direction"); //stub
}

//26D88
void move_ball(obj_t* obj) {
    print_once("Not implemented: move_ball"); //stub
}

//26E4C
void get_brik_bonus(void) {
    print_once("Not implemented: get_brik_bonus"); //stub
}

//26ED0
void do_bonus(void) {
    print_once("Not implemented: do_bonus"); //stub
}

//26F2C
void get_brik_info(obj_t* obj, i16 a2, i16* a3, i16* a4, u8* a5) {
    print_once("Not implemented: get_brik_info"); //stub
}

//2701C
void frame_briques(obj_t* obj) {
    print_once("Not implemented: frame_briques"); //stub
}

//27140
void AllocateCasseBriqueBoum(obj_t* obj) {
    print_once("Not implemented: AllocateCasseBriqueBoum"); //stub
}

//271F8
void AllocateBonus(obj_t* obj, i16 a2, u8 a3, u8 a4) {
    print_once("Not implemented: AllocateBonus"); //stub
}

//272D8
void bonus_fall(obj_t* obj) {
    print_once("Not implemented: bonus_fall"); //stub
}

//2737C
void collision_briques(obj_t* obj) {
    print_once("Not implemented: collision_briques"); //stub
}

//27798
void collision_rayman(obj_t* obj) {
    print_once("Not implemented: collision_rayman"); //stub
}

//279BC
void move_briques(obj_t* obj) {
    print_once("Not implemented: move_briques"); //stub
}

//27A68
void init_briques(obj_t* obj) {
    print_once("Not implemented: init_briques"); //stub
}

//27DC8
void reset_briques(void) {
    print_once("Not implemented: reset_briques"); //stub
}

//27E14
void casse_brique_init(obj_t* obj) {
    print_once("Not implemented: casse_brique_init"); //stub
}

//28004
void casse_brique_start(obj_t* obj) {
    print_once("Not implemented: casse_brique_start"); //stub
}

//28170
void ray_face_brik(i16 a1) {
    print_once("Not implemented: ray_face_brik"); //stub
}

//281AC
void casse_brique_play(obj_t* obj) {
    print_once("Not implemented: casse_brique_play"); //stub
}

//28484
void casse_brique_end(obj_t* obj) {
    print_once("Not implemented: casse_brique_end"); //stub
}

//28504
void casse_brique_done(obj_t* obj) {
    print_once("Not implemented: casse_brique_done"); //stub
}

//28590
void DISPLAY_FIXE_CB(obj_t* obj) {
    print_once("Not implemented: DISPLAY_FIXE_CB"); //stub
}

//287D4
void DO_CB_BALL_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_CB_BALL_COMMAND"); //stub
}

//28874
void DO_CB_BRIK_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_CB_BRIK_COMMAND"); //stub
}

//28888
void DO_RAY_CASSE_BRIQUE(void) {
    print_once("Not implemented: DO_RAY_CASSE_BRIQUE"); //stub
}
