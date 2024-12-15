
// 1D808
void set_subetat(obj_t* obj, u8 sub_etat) {
    obj->change_anim_mode = 1;
    obj->sub_etat = sub_etat;
}

// 1D810
void set_main_etat(obj_t* obj, u8 etat) {
    obj->change_anim_mode = 1;
    obj->main_etat = etat;
}

// 1D818
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat) {
    obj->change_anim_mode = 1;
    obj->main_etat = etat;
    obj->sub_etat = sub_etat;
}

//1D824
void get_center_x(obj_t* obj) {
    //stub
}

//1D954
void get_center_y(obj_t* obj) {
    //stub
}

//1DAC8
void on_block_chdir(obj_t* obj, i16 center_x, i16 center_y) {
    //stub
}

//1DB04
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, i32 a2, i32 a3, i16 a4) {
    //stub
}

//1DBB4
void GET_SPRITE_POS(obj_t* obj, i16 a2, i16* a3, i16* a4, i16* a5, i16* a6) {
    //stub
}

//1DCFC
void GET_RAY_ZDC(obj_t* obj, i16* a2, i16* a4, i16* a5) {
    //stub
}

//1DEC0
void GET_BB1_ZDCs(obj_t* obj, i16* a2, i16* a3, i16* a4, i16* a5, i16* a6, i16* a7, i16* a8, i16* a9) {
    //stub
}

//1DFB8
void calc_obj_dir(obj_t* obj) {
    if (ray.xpos + ray.offset_bx > obj->xpos + obj->offset_bx) {
        obj->flags |= obj_flags_8_flipped;
    } else {
        obj->flags &= ~obj_flags_8_flipped;
    }
}

//1DFF4
void OBJ_IN_ZONE(obj_t* obj) {
    //stub
}

//1E030
void calc_obj_pos(obj_t* obj) {
    //stub
}

//1E05C
void makeUturn(obj_t* obj) {
    //stub
}

//1E588
u8 BTYP(i32 tile_x, i32 tile_y) {
    if (tile_x >= 0 && tile_x <= (mp.width - 1) && tile_y >= 0 && tile_y <= (mp.height - 1)) {
        return mp.map[tile_y * mp.width + tile_x].tile_type;
    } else {
        return 0;
    }
}

//1E608
void calc_btyp_square(obj_t* obj) {
    //stub
}

//1E76C
void DO_OBJ_REBOND_EN_X(obj_t* obj) {
    //stub
}

//1E790
void calc_btyp(obj_t* obj) {
    //stub
}

//1EAA8
void init_obj_in_the_air(obj_t* obj) {
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
}

//1EAB4
void make_my_fruit_go_down(obj_t* obj, i16 a2) {
    //stub
}

//1EB18
void switchOff(obj_t* obj) {
    //stub
}

//1EB80
void obj_hurt(obj_t* target) {
    if (poing.damage > target->hitp) {
        target->hitp = 0;
    } else {
        target->hitp -= poing.damage;
    }
}

//1EBA0
void Projectil_to_RM(obj_t* obj, i16* a2, i16* a3, i16 a4, i16 a5) {
    //stub
}

//1EE18
void del_actobj(obj_t* obj) {
    //stub
}

//1EEB8
void calc_let_Width(u8 a1, u8 a2) {
    //stub
}

//1EF1C
void calc_let_Width2(u8 a1, u8 a2) {
    //stub
}

//1EF80
void INIT_TEXT_TO_DISPLAY(void) {
    //stub
}

//1EFB4
void deter_num_let(u8 a1) {
    //stub
}

//1F194
void calc_num_let_spr(u8 a1, u8* a2) {
    //stub
}

//1F1B0
void calc_largmax_text(void* a1, i16 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//1F21C
void INIT_TXT_BOX(void* a1) {
    //stub
}

//1F4A0
void Deter_Option_Caract(u8* a1, i16 a2, i16 a3) {
    //stub
}

//1F50C
void SwapAB(i16* a, i16* b) {
    i16 temp = *a;
    *a= *b;
    *b = temp;
}

//1F520
void Bresenham(void* func, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    //stub
}

//1F880
void init_finBossLevel(void) {
    finBossLevel &= 0xF000;
}

//1F8A4
void Change_Wait_Anim(void) {
    //stub
}

//1F8E8
void add_alwobj(obj_t* obj) {
    //stub
}

//1F918
void del_alwobj(i16 obj_index) {
    //stub
}

//1F988
void FatalError(obj_t* obj) {
    //stub
}

//1FA00
u8 sprite_of_ref(obj_t* obj, u8 a2) {
    return 0; //stub
}

