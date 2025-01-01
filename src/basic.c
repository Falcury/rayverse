
//1D074
bool EOA(obj_t* obj) {
    eta_t* eta = get_eta(obj);
    bool on_last_frame;
    if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
        on_last_frame = (obj->anim_frame == 0);
    } else {
        anim_t* anim = obj->animations + obj->anim_index;
        on_last_frame = (obj->anim_frame == anim->frame_count - 1);
    }
    if (on_last_frame && horloge[eta->anim_speed & 15] == 0) {
        return true;
    } else {
        return false;
    }
}

//1D0EC
void save_objects_flags(void) {
    //stub
}

//1D15C
void restore_objects_flags(obj_t* obj) {
    //stub
}

//1D204
void snapToSprite(obj_t* obj1, obj_t* obj2, u8 a3, i16 a4, i16 a5) {
    //stub
}

//1D4C4
void findfirstObject(i16 a1) {
    //stub
}

//1D4F4
void findfirstInactiveObject(i16 a1) {
    //stub
}

//1D528
void sinYspeed(obj_t* obj, i16 a2, i16 a3, i16* a4) {
    //stub
}

//1D560
void set_proj_center(i16 x, i16 y) {
    PROJ_CENTER_X = x;
    PROJ_CENTER_Y = y;
}

//1D570
i32 get_proj_dist(i16 scale, i16 outer_dim) {
    //NOTE: needs checking
    if (zoom_mode == 0) {
        return outer_dim;
    } else {
        i32 temp = ((256*256) / (scale + 256)) * (outer_dim);
        return (temp / 256);
    }
}

//1D5B0
i32 get_proj_dist2(i16 a1, i16 a2) {
    return 0; //stub
}

//1D5D8
i32 get_proj_x(i16 scale, i16 a2) {
    //NOTE: needs checking
    i32 temp = ((256*256) / (scale + 256)) * (a2 - PROJ_CENTER_X);
    return ((temp / 256) + PROJ_CENTER_X);
}

//1D614
i32 get_proj_y(i16 scale, i16 a2) {
    //NOTE: needs checking
    i32 temp = ((256*256) / (scale + 256)) * (a2 - PROJ_CENTER_Y);
    return ((temp / 256) + PROJ_CENTER_Y);
}

//1D650
void set_zoom_mode(u8 a1) {
    //stub
}

//1D658
i32 inverse_proj_x(i16 a1, i16 a2) {
    return 0; //stub
}

//1D690
i32 inverse_proj_y(i16 a1, i16 a2) {
    return 0; //stub
}

//1D6C8
void vblToEOA(obj_t* obj, u8 a2) {
    //stub
}

//1D738
void GET_ANIM_POS(obj_t* obj, i16* a2, i16* a3, i16* a4, i16* a5) {
    //stub
}

//1D7D8
void add_actobj(i16 a1) {
    //stub
}

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
    obj->screen_y = obj->ypos - ymap;
    obj->screen_x = obj->xpos - xmap + 8;
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

