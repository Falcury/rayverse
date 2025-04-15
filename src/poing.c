
//666A0
void fist_U_turn(u8 a1) {
    //stub
}

//66700
void CALC_FIST_POS(void) {
    //stub
}

//6676C
void RAY_THROW_FIST(void) {
    //stub
}

//66AF0
void RAY_PREPARE_FIST(void) {
    //stub
}

//66BD0
void RAY_GROW_FIST(void) {
    //stub
}

//66BF4
void fin_poing_follow(u8 a1) {
    if (poing_obj->cmd_arg_2 != -1) {
        obj_t* unk_obj = level.objects + poing_obj->cmd_arg_2;
        unk_obj->speed_x = 0;
        unk_obj->speed_y = 0;
        poing_obj->cmd_arg_2 = -1;
        if (a1 || !(poing_obj->flags.alive)) {
            unk_obj->x = ray.x + ray.offset_bx - unk_obj->offset_bx;
            i32 ray_y = ray_zdc_y + (ray_zdc_h >> 1);
            i32 unk_y = ((unk_obj->offset_by + unk_obj->offset_hy) >> 1) + 5;
            unk_obj->y = ray_y - unk_y;
        } else {
            unk_obj->x = unk_obj->init_x;
            unk_obj->y = unk_obj->init_y;
        }
    }
}

//66CA0
void POING_FOLLOW(obj_t* obj) {
    //stub
}

//66CEC
void alter_fist_speed(obj_t* obj) {
    //stub
}

//66D54
void switch_off_fist(void) {
    poing.is_active = 0;
    poing.is_boum = 0;
    poing.charge = 5;
    poing.is_returning = 0;
    poing_obj->flags.alive = 0;
    poing_obj->is_active = 0;
    poing_obj->sub_etat = poing_obj->init_sub_etat;
    fin_poing_follow(0);
}

//66DA4
void DO_POING(obj_t* obj) {
    //stub
}

//66FF8
void allocatePoingBoum(void) {
    //stub
}

