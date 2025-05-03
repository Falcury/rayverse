
//2B2D0
i32 get_nb_zdc(obj_t* obj) {
    return obj->zdc >> 11;
}

//2B2E0
i32 get_zdc_index(obj_t* obj) {
    return (i32)(obj->zdc & 0x7FF);
}

//2B2E8
zdc_t* get_zdc(obj_t* obj, i32 a2) {
    return zdc_tab + get_zdc_index(obj) + a2;
}

//2B300
u16 get_ZDCPTR(void) {
    return ZDCPTR;
}

//2B308
i32 in_coll_sprite_list(obj_t* obj, i16 a2) {
    /* 1B2E0 8013FAE0 -O2 -msoft-float */
    u8 unk_1[16];
    s16 i;
    s16 res = false;

    switch (obj->type)
    {
        case TYPE_GENEBADGUY:
            unk_1[0] = 0;
            unk_1[1] = 1;
            unk_1[2] = 2;
            unk_1[3] = 0xFF;
            break;
        case TYPE_BADGUY3:
            if (obj->main_etat == 0)
            {
                if (obj->sub_etat == 15 || obj->sub_etat == 16 || obj->sub_etat == 17)
                    unk_1[0] = 4;
                else
                    unk_1[0] = 2;
            }
            else
                unk_1[0] = 2;
            unk_1[1] = 0xFF;
            break;
        case TYPE_MITE:
        case TYPE_MITE2:
            unk_1[0] = 1;
            if (obj->sub_etat == 11)
            {
                if (obj->main_etat == 2)
                    unk_1[1] = 3;
                else
                {
                    if (obj->anim_frame > 15)
                        unk_1[1] = 2;
                    else
                        unk_1[1] = 0xFF;
                }
                unk_1[2] = 0xFF;
            }
            else
                unk_1[1] = 0xFF;
            break;
        default:
            unk_1[0] = 0;
            unk_1[1] = 0xFF;
            break;
    }

    i = 0;
    do
    {
        res = a2 == unk_1[i];
        i++;
    } while (unk_1[i] != 0xFF && res != true);

    return res;
}

//2B3B8
bool box_inter_v_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    return false; //stub
}

//2B3F4
bool box_inter_h_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    return false; //stub
}

//2B430
i32 inter_box(i16 box1_x, i16 box1_y, i16 box1_width, i16 box1_height, i16 box2_x, i16 box2_y, i16 box2_width, i16 box2_height) {
    i32 temp1 = box1_x - box2_width;
    i32 temp2 = box1_y - box2_height;
    if (box2_x < temp1 || box2_y < temp2) {
        return 0;
    }
    i32 temp3 = temp1 + (box1_width + box2_width);
    if (box2_x <= temp3 && box2_width <= (box1_height + box2_height) + temp2) {
        return 1;
    } else {
        return 0;
    }
}

//2B48C
void GET_OBJ_ZDC(obj_t* obj, i16 *out_x, i16 *out_y, i16 *out_w, i16 *out_h) {
    s16 anim_x; s16 anim_y; s16 anim_w; s16 anim_h;
    s32 unk_1;
    s32 unk_2;
    s16 old_flip_x;
    s32 bc_x_pos;
    s16 new_x;
    u8 unk_3;
    s16 new_y;

    switch (obj->type)
    {
        case TYPE_STALAG:
            GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);
            unk_1 = 3;
            if ((s32) *out_w > *out_h)
            {
                *out_x += 4;
                *out_w -= 8;
                *out_y += 8;
                *out_h -= 12;
            }
            else
            {
                *out_x = *out_x - unk_1 + (*out_w >> 1);
                *out_w = 6;
                *out_y += 16;
                *out_h -= 32;
            }
            break;
        case TYPE_BLACK_RAY:
            GET_RAY_ZDC(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_STONECHIP:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 13;
            *out_y = anim_y + 13;
            *out_w = 4;
            *out_h = 4;
            break;
        case TYPE_POWERUP:
        case TYPE_FALLING_OBJ:
        case TYPE_FALLING_OBJ2:
        case TYPE_FALLING_OBJ3:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 10;
            *out_y = anim_y + 10;
            *out_w = anim_w - 20;
            *out_h = anim_h - 20;
            break;
        case TYPE_FLASH:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            unk_2 = 2;
            *out_x = *out_x - unk_2 + (*out_w >> 1);
            *out_y = *out_y - unk_2 + (*out_h >> 1);
            *out_w = 4;
            *out_h = 4;
            break;
        case TYPE_ECLAIR:
        case TYPE_ETINC:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 10;
            *out_y = anim_y + 4;
            *out_w = anim_w - 30;
            *out_h = anim_h - 8;
            break;
        case TYPE_CLASH:
        case TYPE_NOTE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 2;
            *out_y = anim_y - 4;
            *out_w = anim_w - 4;
            *out_h = anim_h + 3;
            break;
        case TYPE_BBL:
            old_flip_x = obj->flags.flip_x;
            obj->flags.flip_x = 0;
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h); /* ... */
            obj->flags.flip_x = old_flip_x;
            if (obj->cmd_arg_1 != 2)
            {
                *out_x = anim_x + 3;
                *out_y = anim_y + 20;
                *out_w = 50;
                *out_h = 40;
            }
            else
            {
                *out_x = anim_x + 20;
                *out_y = anim_y + 35;
                *out_w = 1;
                *out_h = 1;
            }
            break;
        case TYPE_MARTEAU:
        case TYPE_MOVE_MARTEAU:
            GET_SPRITE_POS(obj, 2, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x;
            *out_y = anim_y;
            *out_w = 16;
            *out_h = 32;
            break;
        case TYPE_STONEDOG:
        case TYPE_STONEDOG2:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 4;
            *out_y = anim_y + 2;
            *out_w = anim_w - 8;
            *out_h = anim_h - 6;
            break;
        case TYPE_CAGE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 8;
            *out_y = anim_y + 8;
            *out_w = anim_w - 16;
            *out_h = anim_h - 16;
            break;
        case TYPE_STONEMAN1:
        case TYPE_STONEMAN2:
        case TYPE_STONEWOMAN2:
        case TYPE_STONEWOMAN:
            if (obj->main_etat == 0 && (obj->sub_etat == 3 || obj->sub_etat == 4))
            {
                *out_x = obj->x;
                *out_y = obj->y;
                *out_w = 0;
                *out_h = 0;
            }
            else
            {
                *out_x = obj->x + obj->offset_bx - 10;
                *out_y = obj->y + obj->offset_by - 60;
                *out_w = 20;
                *out_h = 50;
            }
            break;
        case TYPE_CAGE2:
            *out_x = obj->x;
            *out_y = obj->y;
            *out_w = 0;
            *out_h = 0;
            break;
        case TYPE_WIZARD1:
            *out_x = obj->x;
            *out_y = obj->y;
            *out_w = 50;
            *out_h = 110;
            break;
        case TYPE_BIG_CLOWN:
            if (obj->main_etat == 0 && obj->sub_etat == 2 && obj->anim_frame >= 16)
            {
                bc_x_pos = obj->x;
                if (obj->flags.flip_x)
                    new_x = bc_x_pos + 80;
                else
                    new_x = bc_x_pos + 16;
                *out_x = new_x;
                *out_y = obj->y + 40;
                *out_w = 68;
            }
            else if (obj->main_etat == 0 && obj->sub_etat == 2)
            {
                bc_x_pos = obj->x;
                if (obj->flags.flip_x)
                    new_x = bc_x_pos + 80;
                else
                    new_x = bc_x_pos + 48;
                *out_x = new_x;
                *out_y = obj->y + 38;
                *out_w = 32;
            }
            else
            {
                *out_x = obj->x + 64;
                *out_y = obj->y + 38;
                *out_w = 32;
            }
            *out_h = 48;
            break;
        case TYPE_DROP:
            GET_SPRITE_POS(obj, 0, &anim_x, &anim_y, &anim_w, &anim_h);
            if (obj->main_etat == 2)
            {
                if (obj->sub_etat == 2)
                {
                    *out_x = anim_x + 5;
                    *out_y = anim_y + 28;
                    *out_w = 6;
                    *out_h = 22;
                }
                else if (obj->sub_etat == 1)
                {
                    *out_x = anim_x + 6;
                    *out_y = anim_y + 18;
                    *out_w = 13;
                    *out_h = 11;
                }
                else if (obj->sub_etat == 0)
                {
                    *out_x = anim_x + 7;
                    *out_y = anim_y + 8;
                    *out_w = 19;
                    *out_h = 11;
                }
            }
            break;
        case TYPE_TROMPETTE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 24;
            *out_y = anim_y + 14;
            *out_w = anim_w - 48;
            *out_h = anim_h - 27;
            break;
        case TYPE_TNT_BOMB:
            *out_x = obj->x + 76;
            *out_y = obj->y + 85;
            *out_w = 8;
            *out_h = 8;
            break;
        case TYPE_EXPLOSION:
            *out_x = obj->x;
            *out_y = obj->y;
            if (obj->sub_etat == 0)
            {
                *out_w = 22;
                *out_h = 22;
                *out_x += 68;
                *out_y += 65;
                break;
            }
            *out_w = 0;
            *out_h = 0;
            break;
        case TYPE_COUTEAU:
            get_cou_zdc(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_SPIDER_PLAFOND:
            get_spi_zdc(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_POI3:
            if (
                    (obj->main_etat == 2) &&
                    (obj->sub_etat == 15 || obj->sub_etat == 12 || obj->sub_etat == (unk_3 = 13))
                    )
            {
                *out_x = obj->x + 54;
                new_y = obj->y + 55;
            }
            else
            {
                *out_x = obj->x + 74;
                new_y = obj->y + 45;
            }
            *out_y = new_y;
            *out_w = 14;
            *out_h = 21;
            break;
        case TYPE_PETIT_COUTEAU:
            if (obj->hit_points == 1)
            {
                *out_x = obj->x + 138;
                *out_y = obj->y + 138;
                *out_w = 43;
                *out_h = 15;
            }
            if (obj->hit_points == 2)
            {
                *out_x = obj->x + 131;
                *out_y = obj->y + 88;
                *out_w = 17;
                *out_h = 49;
            }
            if (obj->hit_points == 3)
            {
                *out_x = obj->x + 62;
                *out_y = obj->y + 138;
                *out_w = 43;
                *out_h = 16;
            }
            if (obj->hit_points == 4)
            {
                *out_x = obj->x + 96;
                *out_y = obj->y + 87;
                *out_w = 17;
                *out_h = 48;
            }
            break;
        default:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            break;
    }
}

//2C028
i16 GET_SPRITE_ZDC(obj_t* obj, i16 index, i16 *out_x, i16 *out_y, i16 *out_w, i16 *out_h) {
    s16 type;
    s32 unk_x; s32 unk_y; s32 unk_w; s32 unk_h;
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    s16 succ;
    s16 temp_h;

    type = obj->type;
    unk_h = 0;
    unk_w = 0;
    unk_y = 0;
    unk_x = 0;
    succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);
    if (succ)
    {
        switch (type)
        {
            case TYPE_CYMBALE:
                switch (index)
                {
                    case 3:
                        unk_x = 4;
                        unk_y = 2;
                        unk_w = -4;
                        unk_h = -10;
                        break;
                    case 4:
                        unk_x = 0;
                        unk_y = 2;
                        unk_w = 0;
                        unk_h = -10;
                        break;
                    case 5:
                        unk_x = 0;
                        unk_y = 2;
                        unk_w = -4;
                        unk_h = -10;
                        break;
                }
                break;
            case TYPE_GENEBADGUY:
                if (
                        (obj->main_etat == 0 && obj->sub_etat == 0) ||
                        (obj->main_etat == 1 && obj->sub_etat == 0)
                        )
                {
                    switch (index)
                    {
                        case 0:
                            unk_x = 10;
                            unk_y = 0;
                            spr_w = 34;
                            spr_h = 32;
                            break;
                        case 1:
                            unk_x = 4;
                            unk_y = 0;
                            spr_w = 26;
                            break;
                        case 2:
                            unk_x = 4;
                            unk_y = 0;
                            spr_w = 48;
                            spr_h = 24;
                            break;
                    }
                }
                else if (obj->main_etat == 0 && obj->sub_etat == 1)
                {
                    spr_x = obj->x + 112;
                    spr_y = obj->y + 184;
                    spr_w = 32;
                    spr_h = 16;
                }
                else
                {
                    spr_w = 0;
                    spr_h = 0;
                    succ = false;
                }
                break;
            case TYPE_BADGUY3:
                if (index != 2)
                {
                    if (index == 4)
                    {
                        unk_x = -3;
                        unk_y = 0;
                        spr_w = 21;
                        spr_h = 10;
                        break;
                    }
                }
                else
                {
                    unk_x = 1;
                    unk_y = -4;
                    unk_w = -2;
                    unk_h = 6;
                }
                break;
            case TYPE_MITE:
            case TYPE_MITE2:
                switch (index)
                {
                    case 1:
                        if (obj->main_etat == 0)
                        {
                            if (obj->sub_etat == 9 || obj->sub_etat == 10 || obj->sub_etat == 11)
                            {
                                unk_x = -8;
                                unk_y = 5;
                                spr_w = 26;
                                temp_h = 8;
                            }
                            else
                            {
                                unk_x = 1;
                                unk_y = -5;
                                spr_w = 16;
                                temp_h = 30;
                            }
                        }
                        else
                        {
                            unk_x = 1;
                            unk_y = -5;
                            spr_w = 16;
                            temp_h = 30;
                        }
                        spr_h = temp_h;
                        break;
                    case 2:
                        unk_x = 0;
                        unk_y = 7;
                        spr_w = 16;
                        spr_h = 16;
                        break;
                    case 3:
                        unk_x = 0;
                        unk_y = 2;
                        spr_w = 13;
                        spr_h = 9;
                        break;
                }
                break;
        }
        *out_x = unk_x + spr_x;
        *out_y = unk_y + spr_y;
        *out_w = unk_w + spr_w;
        *out_h = unk_h + spr_h;
    }
    return succ;
}

//2C33C
i32 BOX_HIT_SPECIAL_ZDC(i16 x, i16 y, i16 w, i16 h, obj_t* obj) {
    return -1; //stub
}

//2C670
i32 BOX_IN_COLL_ZONES(i16 type, i16 x, i16 y, i16 w, i16 h, obj_t* obj) {
    /* 1C7E4 80140FE4 -O2 -msoft-float */
    s16 res;
    s16 unk_x; s16 unk_y; s16 unk_w; s16 unk_h;
    s16 nb_zdc;
    s16 i;
    zdc_t* cur_zdc;
    anim_t* cur_anim;
    anim_layer_t* cur_layer;
    s16 layers_count;

    unk_h = 0;
    unk_w = 0;
    unk_y = 0;
    unk_x = 0;
    res = -1;

    if (obj->zdc != 0)
    {
        nb_zdc = get_nb_zdc(obj);
        if (
                num_world == 1 &&
                (
                        ((obj->type == TYPE_MOSKITO || obj->type == TYPE_MOSKITO2) &&
                         !(obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)) ||
                        (obj->type == TYPE_BADGUY3 &&
                         obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)
                )
                )
            nb_zdc--;

        for (i = 0; i < nb_zdc; i++)
        {
            cur_zdc = get_zdc(obj, i);
            if (!(cur_zdc->flags & 4) || type == TYPE_POING)
            {
                if (cur_zdc->flags & 2)
                {
                    cur_anim = &obj->animations[obj->anim_index];
                    cur_layer = &cur_anim->layers[(cur_anim->layers_per_frame & 0x3FFF) * obj->anim_frame];
                    if (cur_layer[cur_zdc->sprite].sprite_index != 0)
                    {
                        GET_SPRITE_POS(obj, cur_zdc->sprite, &unk_x, &unk_y, &unk_w, &unk_h);
                        if (obj->flags.flip_x)
                            unk_x += unk_w - cur_zdc->width - cur_zdc->x_pos;
                        else
                            unk_x += cur_zdc->x_pos;

                        unk_y += cur_zdc->y_pos;
                        unk_w = cur_zdc->width;
                        unk_h = cur_zdc->height;
                    }
                }
                else
                {
                    unk_x = obj->x + cur_zdc->x_pos;
                    unk_y = obj->y + cur_zdc->y_pos;
                    unk_w = cur_zdc->width;
                    unk_h = cur_zdc->height;
                    /* TODO: can't take << 1 or * 2??? */
                    if (obj->flags.flip_x)
                        unk_x =
                                (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) -
                                (unk_x + unk_w);
                }

                if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h))
                {
                    res = cur_zdc->sprite;
                    break;
                }
            }
        }
    }
    else
    {
        if (obj->hit_sprite == 0xFE)
        {
            GET_OBJ_ZDC(obj, &unk_x, &unk_y, &unk_w, &unk_h);
            if (obj->flags.flip_x)
                unk_x =
                        (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) -
                        (unk_x + unk_w);

            res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h);
            res = res ? res : -1;
        }
        else if (obj->hit_sprite == 0xFF)
        {
            layers_count = obj->animations[obj->anim_index].layers_per_frame & 0x3FFF;
            for (i = 0; i < layers_count; i++)
            {
                if (
                        in_coll_sprite_list(obj, i) &&
                        GET_SPRITE_ZDC(obj, i, &unk_x, &unk_y, &unk_w, &unk_h) &&
                        inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)
                        )
                {
                    res = i;
                    break;
                }
            }
        }
    }

    return res;
}

//2CA84
i32 COLL_BOX_SPRITE(i16 x, i16 y, i16 w, i16 h, obj_t* obj) {
    i16 spr_x;
    i16 spr_y;
    i16 spr_w;
    i16 spr_h;
    if (GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h)) {
        if (inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h)) {
            return obj->hit_sprite;
        }
    }
    return -1;
}

//2CB1C
i16 CHECK_BOX_COLLISION(i16 type, i16 x, i16 y, i16 w, i16 h, obj_t* obj) {
    if (obj->hit_sprite == 0xFD) {
        return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);
    } else if (obj->hit_sprite < 0xFD) {
        return COLL_BOX_SPRITE(x, y, w, h, obj);
    } else {
        return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);
    }
}

//2CB9C
void possible_sprite(obj_t* obj, i16 a2) {
    //stub
}

//2CE34
void setToleranceDist(i16 a1, i16 a2, i16 a3) {
    //stub
}

//2CF14
void SET_RAY_DIST_SLOPEY_PLAT(obj_t* obj) {
    //stub
}

//2D0D4
void SET_RAY_DIST_MULTISPR_CANTCHANGE(obj_t* obj) {
    //stub
}

//2D334
void SET_RAY_DIST_PI(obj_t* obj) {
    //stub
}

//2D3EC
void SET_RAY_DIST_BAG(obj_t* obj) {
    //stub
}

//2D4D0
void SET_RAY_DIST(obj_t* obj) {
    //stub
}

//2D5CC
void do_boum(void) {
    //stub
}

//2D5FC
void DO_POING_COLLISION(void) {
    //stub
}

//2D858
void DoPoingCollisionDefault(obj_t* obj, i16 a2) {
    //stub
}

//2D85C
void COLL_BOX_ALL_SPRITES(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj) {
    //stub
}

//2D914
bool COLL_RAY_PIC(void) {
    return false; //stub
}

//2DA98
void COLL_RAY_BLK_MORTEL(void) {
    if ((ray.y + ray.offset_by) < (mp.height * 16)) {
        i16 btyp = mp.map[ray.ray_dist].tile_type;
        if (btyp == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            ray.speed_y = 0;
            ray.speed_x = 0;
            ray.y += 5;
        } else if (ray.btypes[3] == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            ray.speed_y = 0;
            ray.speed_x = 0;
        } else if (btyp == BTYP_CLIFF) {
            set_main_and_sub_etat(&ray, 2, 31);
            scroll_end_y = ymap;
            scroll_start_y = ymap;
        }
    }
}

//2DBAC
void RAY_KO(void) {
    set_main_and_sub_etat(&ray, 3, 0);
    ray.speed_y = 0;
    ray.speed_x = 0;
    ray.configuration = 0;
}

//2DBDC
void RAY_HIT(bool hurt, obj_t* obj) {
    if (hurt) {
        RAY_HURT();
        i32 tile_x = (ray.x + ray.offset_bx) / 16;
        while (BTYP(tile_x, (ray.y + ray.offset_by) / 16) == BTYP_WATER) {
            --ray.y;
        }
        ray.btypes[0] = BTYP_NONE;
    }
    if (ray.main_etat == 6) {
        set_main_and_sub_etat(&ray, 6, 8);
        ray.speed_y = 0;
        ray.speed_x = 0;
        poing.is_charging = 0;
    } else if (ray.flags.alive) {
        if (!(ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32)) &&
            !(ray.main_etat == 2 && ray.sub_etat == 31)
        ) {
            if ((get_eta(&ray)->flags & 0x40) && (block_flags[calc_typ_trav(&ray, 2)] & 0x10)) {
                set_main_and_sub_etat(&ray, 0, 61);
            } else {
                set_main_and_sub_etat(&ray, 2, 8);
            }
            ray_speed_inv = 0;
            if (obj == NULL && !(ray.main_etat == 0 && ray.sub_etat == 61)) {
                ray.speed_x = ray.flags.flip_x ? -2 : 2;
                ray.speed_y = -3;
            } else if (obj && !(ray.main_etat == 0 && ray.sub_etat == 61)){
                i32 eject_sens = -1;
                if (obj->type == TYPE_180_SAXO2) {
                    eject_sens = saxo2_get_eject_sens();
                } else if (obj->type == TYPE_150_SCORPION) {
                    eject_sens = sko_get_eject_sens();
                } else if (obj->type == TYPE_198_BB12) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_200_BB13) {
                    eject_sens = -1;
                } else if (obj->type == TYPE_100_MITE) {
                    eject_sens = obj->flags.flip_x ? 1 : -1;
                } else if (obj->type == TYPE_120_BATTEUR_FOU) {
                    eject_sens = bat_get_eject_sens(obj);
                } else if (obj->type == TYPE_187_MAMA_PIRATE) {
                    eject_sens = pma_get_eject_sens(obj);
                } else if (obj->type == TYPE_209_FIRE_LEFT) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_210_FIRE_RIGHT) {
                    eject_sens = -1;
                } else {
                    i32 xspeed_delta = obj->speed_x - ray.speed_x;
                    if (xspeed_delta == 0) {
                        eject_sens = ray.flags.flip_x ? 1 : -1;
                    } else {
                        eject_sens = xspeed_delta > 0 ? 1 : -1;
                    }
                }
                i32 bump_speed;
                if (flags[obj->type] & flags1_4_fast_bump) {
                    bump_speed = 5;
                } else if (obj->type == TYPE_180_SAXO2) {
                    bump_speed = 4;
                } else {
                    bump_speed = 2;
                }
                ray.speed_x = bump_speed * eject_sens; // Note: eject_sens is always 1 or -1
                ray.speed_y = ~bump_speed;
            }

        }
    }
}

//2DF34
void standard_frontZone(obj_t* obj, i16* x, i16* w) {
    /* 1FA54 80144254 -O2 -msoft-float */
    if (!obj->flags.flip_x)
        *x -= obj->detect_zone;
    else
        *x += (*w >> 1);
    *w = obj->detect_zone + (*w >> 1);
}

//2DF88
void SET_DETECT_ZONE_FLAG(obj_t* obj) {
    /* 1FAB4 801442B4 -O2 -msoft-float */

    s16 obj_x; s16 obj_y; s16 obj_w; s16 obj_h;
    s32 unk_1;
    u16 det_zn_flg;

    GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
    switch (obj->type)
    {
        case TYPE_FEE:
            obj_x += (obj_h >> 1) - (obj->detect_zone >> 1);
            obj_y -= 16;
            obj_w = obj->detect_zone;
            obj_h += 64;
            break;
        case TYPE_MST_SHAKY_FRUIT:
            obj_y += obj_h;
            obj_h += 240;
            break;
        case TYPE_PIRATE_GUETTEUR:
        case TYPE_PIRATE_GUETTEUR2:
            standard_frontZone(obj, &obj_x, &obj_w);
            if (obj->main_etat == 0 && obj->sub_etat == 4)
            {
                obj_y += (obj_h >> 1);
                obj_h += (obj->detect_zone << 2) + (obj_h >> 1);
            }
            break;
        case TYPE_BLACKTOON1:
            unk_1 = -1;
            switch (obj->follow_sprite)
            {
                case 3:
                    if (obj->main_etat == 0 && obj->sub_etat == 0)
                    {
                        obj_h += obj->detect_zone;
                        obj_y -= obj->detect_zone;
                        obj_w += obj->detect_zone;
                        obj_x -= (obj->detect_zone >> 1);
                    }
                    break;
                case 7:
                    if (obj->main_etat == 1 && obj->sub_etat == 1)
                    {
                        obj_x = obj_x + unk_1 + (obj_w >> 1);
                        obj_y += (obj_h >> 1);
                        obj_w = 20;
                        obj_h += obj->detect_zone;
                        if (!obj->flags.flip_x)
                            obj_x -= obj_w;
                    }
                    break;
                case 4:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_x -= (obj_w >> 1);
                    obj_w += obj_w;
                    obj_h += obj->detect_zone;
                    break;
                case 2:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_h = obj->detect_zone + obj_h;
                    obj_w += obj->detect_zone * 2;
                    obj_x -= obj->detect_zone;
                    break;
            }
            break;
        case TYPE_POI3:
            obj_x += (obj->flags.flip_x) ? obj_w : -obj_w;
            obj_y -= 50;
            obj_h += 50;
            break;
        case TYPE_MST_SCROLL:
            if (obj->hit_points != 0)
            {
                obj_y = 0;
                obj_h = mp.height << 4;
            }
            break;
        case TYPE_SCROLL_SAX:
        case TYPE_BB1_VIT:
            obj_x = obj->x + obj->offset_bx;
            obj_w = 16;
            obj_y = 0;
            obj_h = mp.height << 4;
            break;
        case TYPE_WAT_CLOWN:
            obj_h += (obj->detect_zone >> 1);
            obj_y -= (obj->detect_zone >> 1);
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
        case TYPE_CHASSEUR2:
            obj_h = obj->detect_zone;
            obj_y -= (obj->detect_zone >> 1);
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_WIZARD1:
        case TYPE_CHASSEUR1:
        case TYPE_GENEBADGUY:
        case TYPE_CYMBALE:
        case TYPE_CYMBAL1:
        case TYPE_CYMBAL2:
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_CLOWN_TNT:
            if (!obj->flags.flip_x)
                obj_x += (obj_w >> 1) - (obj->detect_zone >> 1);
            else
                obj_x += (obj_w >> 1);

            obj_w = (obj->detect_zone >> 1);
            obj_y += (obj_h >> 1);
            obj_h = (obj_h >> 1) + obj->detect_zone;
            break;
        case TYPE_STONEWOMAN:
        case TYPE_STONEWOMAN2:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = (obj_h << 1);
            break;
        case TYPE_MITE:
            unk_1 = 150;
            if (!obj->flags.flip_x)
                obj_x -= obj->detect_zone;
            else
                obj_x = obj_x + -unk_1 + (obj_w >> 1);

            obj_w = obj->detect_zone + unk_1;
            obj_y -= ((obj->detect_zone - obj_h) >> 1);
            obj_h = obj->detect_zone;
            break;
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_P_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            break;
        case TYPE_PIRATE_POELLE_D:
        case TYPE_PIRATE_P_D_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            obj_x += 70;
            obj_y += 50;
            break;
        case TYPE_SPIDER_PLAFOND:
            if (
                    (
                            obj->main_etat == 0 &&
                            (obj->sub_etat == 24 || obj->sub_etat == 30 || obj->sub_etat == 11)
                    ) ||
                    (obj->main_etat == 1 && obj->sub_etat == 2)
                    )
            {
                obj_h = 10;
                obj_x -= 40;
                obj_w += 80;
            }
            else
            {
                obj_h = 200;
                obj_x -= 130;
                obj_w += 260;
            }
            break;
        case TYPE_DARK:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 250;
            break;
        case TYPE_JOE:
            obj_h += 20;
            break;
        case TYPE_PHOTOGRAPHE:
            break;
        default:
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
    }

    if (inter_box(obj_x, obj_y, obj_w, obj_h, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h))
    {
        det_zn_flg = obj->detect_zone_flag;
        if (det_zn_flg == 0 || det_zn_flg == 1)
        {
            if (det_zn_flg == 0)
                obj->detect_zone_flag = 1;
            else if (det_zn_flg == 1)
                obj->detect_zone_flag = 2;
        }
        else
        {
            obj->detect_zone_flag++;
            if (obj->detect_zone_flag == 20)
                obj->detect_zone_flag = 2;
        }
    }
    else
    {
        obj->detect_zone_flag = 0;
        if (obj->type == TYPE_PHOTOGRAPHE)
            obj->flags.flag_1 = 0;
    }
}

//2E738
void goToRay(obj_t* obj) {
    //stub
}

//2E998
void unleashMonsterHost(obj_t* obj) {
    //stub
}

//2EA2C
void SHOW_COLLISIONS_ZONES(void) {
    //stub
}

//2EAE8
void DO_COLLISIONS(void) {
    for (i32 i = 0; i < actobj.num_active_objects; ++i) {
        if (new_world || new_level || fin_boss || boss_mort) {
            break;
        }
        obj_t* obj = level.objects + actobj.objects[i];
        if (get_eta(obj)->flags & 0x20)  {
            if (flags[obj->type] & flags2_0x10_do_not_check_ray_collision) {
                if (!(ray.main_etat == 3 && ray.sub_etat == 32)) {
                    i16 collision;
                    if (obj->type  == TYPE_81_CYMBALE || obj->type == TYPE_169_CYMBAL2) {
                        collision = 0;
                    } else {
                        collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, obj);
                    }
                    if (collision != -1) {
                        ObjectsFonctions[obj->type].rayman_collision(obj);
                    }
                }
            } else if (ray.configuration == 0 && ray.iframes_timer == -1) {
                if ((get_eta(&ray)->flags & 8) && !(flags[obj->type] & flags0_4_no_collision)) {
                    i16 collision;
                    if ((obj->type == TYPE_170_RAYON && (obj->anim_frame == 0 || obj->anim_frame > 3)) ||
                            (obj->type == TYPE_147_MST_SCROLL && obj->hit_points != 0)
                    ) {
                        collision = -1;
                    } else {
                        collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, obj);
                    }
                    if (collision != -1) {
                        ObjectsFonctions[obj->type].rayman_collision(obj);
                        if ((flags[obj->type] & flags0_8_hurts_rayman) && ray.iframes_timer == -1 &&
                                !(ray.main_etat == 3 && ray.sub_etat  == 32)
                        ) {
                            RAY_HIT(1, obj);
                        }
                    }
                }
            }
        }
    }
    if (ray.configuration != 0) {
        RAY_KO();
    }
    if (ray.iframes_timer > -1) {
        --ray.iframes_timer;
    }
}

//2ED74
void DoRaymanCollisionDefault(obj_t* obj) {
    if ((flags[obj->type] & flags2_0x10_do_not_check_ray_collision) && get_eta(obj)->flags & 0x20) {
        if (!(ray.main_etat == 3 && ray.sub_etat == 32)) {
            i16 command_par3 = ray.iframes_timer;
            if (command_par3 >= 60 || command_par3 == -1) {
                RAY_HIT(0, obj);
                ray.iframes_timer = command_par3;
            } else {
                RAY_HIT(1, obj);
                if (!(ray_mode == MODE_3_MORT_DE_RAYMAN || ray_mode == MODE_4_MORT_DE_RAYMAN_ON_MS)) {
                    ray.iframes_timer = 60;
                }
            }
        }
    }
}

//2EE20
void DO_OBJ_COLLISIONS(obj_t* obj, i16 a2) {
    //stub
}

//2F1E0
void test_allowed(obj_t* obj, i16 center_x, i16 center_y) {
    //stub
}

//2F224
void obj_jump(obj_t* obj) {
    /* 4880C 8016D00C -O2 -msoft-float */
    s16 spd_y;
    u8 gravity_value_2 = 0;
    s16 spd_x = obj->speed_x;
    u8 label = 255;

    switch (obj->type) {
        case TYPE_172_STONEWOMAN:
            if (get_eta(obj)->flags & 1) {
                label = 15;
                set_main_and_sub_etat(obj, 2, 3);
                spd_y = -3;
                gravity_value_2 = 5;
            }
            break;
        case TYPE_123_BLACKTOON1:
            if (obj->configuration & 2) {
                label = 1;
                set_main_and_sub_etat(obj, 2, 5);
                spd_y = -3;
            }
            break;
    }

    if (label != 255) {
        skipToLabel(obj, label, true);
        obj->speed_x = spd_x;
        obj->speed_y = spd_y;
        obj->gravity_value_1 = 0;
        obj->gravity_value_2 = gravity_value_2;
        obj->y -= 12;
        calc_btyp(obj);
    }
}

//2F2FC
void DO_MOVING_PLATFORM_COMMAND(obj_t* obj) {
    //stub
}

//2F378
void DoPTGRAPPINPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//2F44C
void DO_ONE_CMD_WAIT(obj_t* obj) {
    if (obj->main_etat == 1) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 0);
    } else {
        if (!(obj->type == TYPE_10_FISH && obj->main_etat == 0 && obj->sub_etat == 0)) {
            obj->speed_y = 0;
        }
    }
}

//2F488
void DO_ONE_CMD_LR_ATTENTE(obj_t* obj) {
    //stub
}

//2F594
void DO_ONE_CMD_UPDOWN(obj_t* obj) {
    if (obj->type == TYPE_1_PLATFORM) {
        if (obj->cmd == GO_UP) {
            obj->speed_y = -2;
        } else if (obj->cmd == GO_DOWN) {
            obj->speed_y = 2;
        }
    } else if (obj->type == TYPE_10_FISH) {
        if (obj->main_etat == 0 && obj->sub_etat == 0) {
            if (obj->cmd == GO_UP) {
                obj->speed_y = -2;
            } else {
                obj->speed_y = 2;
            }
        }
    } else if (obj->type == TYPE_24_INTERACTPLT) {
        // This procedure is only called for commands 3 and 4, so, not sure why we are checking for command 2 here?
        if (obj->cmd != GO_WAIT) {
            --obj->nb_cmd;
            if (obj->nb_cmd <= 0) {
                if (obj->cmd == GO_UP) {
                    obj->speed_y = -1;
                } else if (obj->cmd == GO_DOWN) {
                    obj->speed_y = 1;
                }
            } else {
                obj->speed_y = 0;
                obj->nb_cmd = 0;
                obj->cmd = GO_WAIT;
                obj->init_y = obj->y;
            }

        }
    }
}

//2F63C
void special_pour_liv(obj_t* event) {
    if (event->type == TYPE_0_BADGUY1 && event->main_etat == 1 && event->sub_etat == 11) {
        event->flags.read_commands = false;
    }
}

//2F658
void DO_ONE_CMD(obj_t* obj) {
    special_pour_liv(obj);
    u8 cmd = obj->cmd;
    u8 etat = obj->main_etat;
    if (cmd == GO_LEFT || cmd == GO_RIGHT) {
        if (cmd == GO_LEFT) {
            obj->flags.flip_x = false;
        } else {
            obj->flags.flip_x = true;
        }
        if (etat == 1) {
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
        } else if (etat == 2) {
            SET_X_SPEED(obj);
        } else if (etat == 0) {
            DO_ONE_CMD_LR_ATTENTE(obj);
        }
    } else if (cmd == GO_WAIT) {
        DO_ONE_CMD_WAIT(obj);
    } else if (cmd == GO_UP || cmd == GO_DOWN) {
        DO_ONE_CMD_WAIT(obj);
    } else if (cmd == GO_SPEED) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->cmd_arg_2;
    }
}

//2F714
void DO_ROLL_EYES(obj_t* obj) {
    //stub
}

//2F978
void DoKillingEyesPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//2F9B8
void YaUnBloc(obj_t* obj) {
    //stub
}

//2FBF4
void DO_BAG1_COMMAND(obj_t* obj) {
    //stub
}

//2FC64
void DoRaymanBag1Collision(obj_t* obj) {
    //stub
}

//2FCB0
void MarCoince(obj_t* obj, i16 a2) {
    //stub
}

//2FFDC
void DO_MOVE_MARACAS_COMMAND(obj_t* obj) {
    //stub
}

//301A4
void DO_FLASH_COMMAND(obj_t* obj) {
    //stub
}

//30260
void DO_WLKNOT_COMMAND(obj_t* obj) {
    //stub
}

//30318
void ACTIVE_L_EAU(obj_t* obj) {
    //stub
}

//30490
void DO_EAU_QUI_MONTE(obj_t* obj) {
    //stub
}

//3076C
void allocateOtherPosts(obj_t* obj) {
    //stub
}

//3088C
void doHerseCommand(obj_t* obj) {
    //stub
}

//30908
void doBlackRaymanCommand(obj_t* obj) {
    //stub
}

//30BE4
void doBlKRayRaymanCollision(obj_t* obj) {
    //stub
}

//30BF8
void DO_POELLE_COMMAND(obj_t* obj) {
    //stub
}

//30EA8
void DO_CORDE_COMMAND(obj_t* obj) {
    //stub
}

//31060
void DO_PAC_COMMAND(obj_t* obj) {
    //stub
}

//310F8
void DO_CFUMEE_COMMAND(obj_t* obj) {
    //stub
}

//31190
void DO_NOVA2_COMMAND(obj_t* obj) {
    //stub
}

//31208
void doShipCommand(obj_t* obj) {
    //stub
}

//31400
void DO_PROP_COMMAND(obj_t* obj) {
    //stub
}

//314A4
void move_fruit_in_water(obj_t* obj) {
    //stub
}

//315F0
void DO_FALLING_OBJ_CMD(obj_t* obj) {
    //stub
}

//3167C
void DoFallingObjPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//3171C
void DO_BLKTOON_EYES_CMD(obj_t* obj) {
    //stub
}

//31768
void DO_RAY_POS_CMD(obj_t* obj) {
    if (obj->timer != 255) {
        if (obj->timer == 0) {
            if (obj->main_etat != 0) {
                if (obj->main_etat == 5) {
                    ray.is_active = 1;
                    ray.flags.alive = 1;
                    set_main_and_sub_etat(&ray, 3, 40);
                }
            } else {
                ray.is_active = 1;
                ray.flags.alive = 1;
            }
            obj->flags.alive = 0;
            del_alwobj(obj->id);
            dead_time = 64;
            ray.x = obj->x + obj->offset_bx - ray.offset_bx;
            ray.y = obj->y + obj->offset_by - ray.offset_by;
        }
        --obj->timer;
    }
}
