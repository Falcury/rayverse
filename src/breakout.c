
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
    //stub
}

//26A9C
void exit_casse_brique(void) {
    //stub
}

//26AB8
void exit_casse_brique2(void) {
    //stub
}

//26AD0
i32 get_speed_ball(void) {
    return 0; //stub
}

//26C00
void speed_ball_null(obj_t* obj) {
    //stub
}

//26C30
void speed_ball_init(obj_t* obj) {
    //stub
}

//26C5C
void speed_up_ball(obj_t* obj) {
    //stub
}

//26C8C
void invert_speed_ball_x(obj_t* obj) {
    //stub
}

//26CB8
void invert_speed_ball_y(obj_t* obj) {
    //stub
}

//26CE4
void set_ball_pos(obj_t* obj, i16 a2, i16 a3) {
    //stub
}

//26D0C
void angle_direction(i16 a1, i16 a2) {
    //stub
}

//26D88
void move_ball(obj_t* obj) {
    //stub
}

//26E4C
void get_brik_bonus(void) {
    //stub
}

//26ED0
void do_bonus(void) {
    //stub
}

//26F2C
void get_brik_info(obj_t* obj, i16 a2, i16* a3, i16* a4, u8* a5) {
    //stub
}

//2701C
void frame_briques(obj_t* obj) {
    //stub
}

//27140
void AllocateCasseBriqueBoum(obj_t* obj) {
    //stub
}

//271F8
void AllocateBonus(obj_t* obj, i16 a2, u8 a3, u8 a4) {
    //stub
}

//272D8
void bonus_fall(obj_t* obj) {
    //stub
}

//2737C
void collision_briques(obj_t* obj) {
    //stub
}

//27798
void collision_rayman(obj_t* obj) {
    //stub
}

//279BC
void move_briques(obj_t* obj) {
    //stub
}

//27A68
void init_briques(obj_t* obj) {
    //stub
}

//27DC8
void reset_briques(void) {
    //stub
}

//27E14
void casse_brique_init(obj_t* obj) {
    //stub
}

//28004
void casse_brique_start(obj_t* obj) {
    //stub
}

//28170
void ray_face_brik(i16 a1) {
    //stub
}

//281AC
void casse_brique_play(obj_t* obj) {
    //stub
}

//28484
void casse_brique_end(obj_t* obj) {
    //stub
}

//28504
void casse_brique_done(obj_t* obj) {
    //stub
}

//28590
void DISPLAY_FIXE_CB(obj_t* obj) {
    //stub
}

//287D4
void DO_CB_BALL_COMMAND(obj_t* obj) {
    //stub
}

//28874
void DO_CB_BRIK_COMMAND(obj_t* obj) {
    //stub
}

//28888
void DO_RAY_CASSE_BRIQUE() {
    //stub
}
