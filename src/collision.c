
//2B2D0
s32 get_nb_zdc(obj_t* obj) {
    return obj->zdc >> 11;
}

//2B2E0
s32 get_zdc_index(obj_t* obj) {
    return (s32)(obj->zdc & 0x7FF);
}

//2B2E8
zdc_t* get_zdc(obj_t* obj, s32 a2) {
    return zdc_tab + get_zdc_index(obj) + a2;
}

//2B300
u16 get_ZDCPTR(void) {
    return ZDCPTR;
}

//2B308
s32 in_coll_sprite_list(obj_t* obj, s16 a2) {
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
bool box_inter_v_line(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    print_once("Not implemented: box_inter_v_line");
    return false; //stub
}

//2B3F4
bool box_inter_h_line(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    print_once("Not implemented: box_inter_h_line");
    return false; //stub
}

//2B430
s16 inter_box(s32 x_1, s32 y_1, s32 w_1, s32 h_1, s16 x_2, s16 y_2, s32 w_2, s32 h_2) {
    /* 1B57C 8013FD7C -O2 -msoft-float */

    // Re-implemented from Rayman Designer
    if (debug_show_obj_collision) {
        draw_collision_box(x_1 - xmap + 8, y_1 - ymap, w_1, h_1);
        draw_collision_box(x_2 - xmap + 8, y_2 - ymap, w_2, h_2);
    }

    s16 comp_x = x_1 - w_2;
    s16 comp_y = y_1 - h_2;
    s16 sum_w = w_1 + w_2;
    s16 sum_h = h_1 + h_2;
    u8 res = false;

    if (x_2 >= comp_x && y_2 >= comp_y && (comp_x + sum_w >= x_2))
        res = (comp_y + sum_h < y_2) ^ 1;
    return res;
}

//2B48C
void GET_OBJ_ZDC(obj_t* obj, s16 *out_x, s16 *out_y, s16 *out_w, s16 *out_h) {
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
            if (obj->param != 2)
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

    // Re-implemented from Rayman Designer
    if (debug_show_obj_collision) {
        draw_collision_box(*out_x - xmap + 8, *out_y - ymap, *out_w, *out_h);
    }
}

//2C028
s16 GET_SPRITE_ZDC(obj_t* obj, s16 index, s16 *out_x, s16 *out_y, s16 *out_w, s16 *out_h) {
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
s32 BOX_HIT_SPECIAL_ZDC(s16 in_x, s16 in_y, s16 in_w, s16 in_h, obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 frame;
    s16 d;
    s16 bb1_x_1; s16 bb1_y_1; s16 bb1_w_1; s16 bb1_h_1;
    s16 bb1_x_2; s16 bb1_y_2; s16 bb1_w_2; s16 bb1_h_2;
    s16 res = -1;

    switch (obj->type) {
        case TYPE_BAG1:
            frame = obj->anim_frame;
            d = bagDy[frame];
            if (d != -1) {
                if (inter_box(in_x, in_y, in_w, in_h,
                        obj->x + obj->offset_bx - (bagW[frame] >> 1),
                        obj->y + obj->offset_by + d - bagH[frame],
                        bagW[frame],
                        bagH[frame]
                ))
                    res = 1;
            }
            break;
        case TYPE_BB1:
        case TYPE_BB12:
        case TYPE_BB13:
        case TYPE_BB14:
            if (in_h == ray_zdc_h) {
                GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);

                /* TODO: write a bit nicer, not sure how yet */
                if (!inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)) {
                    if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2))
                        res = 1;
                }
                else
                    res = 1;
            } else {
                GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);

                if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1))
                    res = 6;
                if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2))
                    res = 1;

                if (obj->main_etat == 0 && obj->sub_etat == 10) {
                    GET_SPRITE_POS(obj, 9, &bb1_x_1, &bb1_y_1, &bb1_h_1, &bb1_w_1);
                    if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1))
                        res = 9;
                }
            }
            break;
    }
    return res;
}

//2C670
s32 BOX_IN_COLL_ZONES(s16 type, s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
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

    ASSERT(obj->anim_index != 255);

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
s32 COLL_BOX_SPRITE(s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    if (GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h)) {
        if (inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h)) {
            return obj->hit_sprite;
        }
    }
    return -1;
}

//2CB1C
s16 CHECK_BOX_COLLISION(s16 type, s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
    if (obj->hit_sprite == 0xFD) {
        return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);
    } else if (obj->hit_sprite < 0xFD) {
        return COLL_BOX_SPRITE(x, y, w, h, obj);
    } else {
        return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);
    }
}

//2CB9C
s16 possible_sprite(obj_t* obj, s16 index) {
    /* 1CF08 80141708 -O2 -msoft-float */
    s16 spr[12];

    switch (obj->type) {
        case TYPE_BON3:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 2;
            spr[3] = 3;
            spr[4] = 4;
            spr[5] = 255;
            break;
        case TYPE_CYMBALE:
        case TYPE_CYMBAL2:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 2;
            spr[3] = 255;
            break;
        case TYPE_ROULETTE:
            spr[0] = 5;
            spr[1] = 6;
            spr[2] = 7;
            spr[3] = 8;
            spr[4] = 255;
            break;
        case TYPE_ROULETTE2:
            spr[0] = 1;
            spr[1] = 2;
            spr[2] = 3;
            spr[3] = 4;
            spr[4] = 255;
            break;
        case TYPE_ROULETTE3:
            spr[0] = 1;
            spr[1] = 2;
            spr[2] = 3;
            spr[3] = 255;
            break;
        case TYPE_FALLPLAT:
        case TYPE_LIFTPLAT:
        case TYPE_INTERACTPLT:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 255;
            break;
        case TYPE_TIBETAIN_6:
            if (ray.flags.flip_x)
            {
                spr[0] = 4;
                spr[1] = 5;
                spr[2] = 6;
                spr[3] = 7;
                spr[4] = 8;
                spr[5] = 9;
            }
            else
            {
                spr[0] = 9;
                spr[1] = 8;
                spr[2] = 7;
                spr[3] = 6;
                spr[4] = 5;
                spr[5] = 4;
            }
            spr[6] = 255;
            break;
        case TYPE_TIBETAIN_2:
            spr[0] = 4;
            spr[1] = 5;
            spr[2] = 0x00FF;
            break;
    }
    return spr[index];
}

//2CE34
s16 setToleranceDist(s16 x, s16 w, s16 y) {
    /* 1D11C 8014191C -O2 -msoft-float */
    s16 dist;
    s16 ray_x = ray.x + ray.offset_bx;
    s16 ray_y = ray.y + ray.offset_by;
    s16 unk_1 = x + w - 1;

    s16 unk_2 = (RayEvts.tiny) ? 4 : 8;
    if ((ray_x >= x - unk_2) && (ray_x <= unk_1 + unk_2)) {
        if (ray_x >= x && ray_x <= unk_1) {
            /* abs??? */
            if (ray_y - y >= 0)
                dist = ray_y - y;
            else
                dist = y - ray_y;
        } else {
            if (ray_y - y < 0)
                dist = ray_y - y;
            else
                dist = y - ray_y;
            dist--;
        }
    }
    else
        dist = 10000;
    return dist;
}

//2CF14
void SET_RAY_DIST_SLOPEY_PLAT(obj_t* obj) {
    print_once("Not implemented: SET_RAY_DIST_SLOPEY_PLAT"); //stub
}

//2D0D4
void SET_RAY_DIST_MULTISPR_CANTCHANGE(obj_t* obj) {
    /* 1D228 80141A28 -O2 -msoft-float */

    s16 i;
    s16 new_dist;
    s16 ray_x; s16 ray_y;
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    s16 diff_x;
    s16 sprite;

    s16 unk_1 = (RayEvts.tiny) ? 4 : 8;
    i = 0;
    new_dist = 10000;
    ray_x = ray.x + ray.offset_bx;
    ray_y = ray.y + ray.offset_by;

    if (ray.follow_id == obj->id) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
        spr_y += obj->offset_hy;
        if (obj->type == TYPE_ROULETTE || obj->type == TYPE_ROULETTE2 || obj->type == TYPE_ROULETTE3) {
            spr_w -= 10;
            spr_x += 5;
        }

        if (obj->type == TYPE_TIBETAIN_6 && ray.main_etat == 0 && (obj->anim_frame >= 10 && obj->anim_frame < 40))
        {
            diff_x = (ray.x + ray.offset_bx) - (spr_x + (spr_w >> 1));
            if (diff_x > 0)
                ray.x--;
            else if (diff_x < 0)
                ray.x++;
            ray_x = ray.x + ray.offset_bx;
        }
        new_dist = setToleranceDist(spr_x, spr_w, spr_y);
    }

    if (new_dist == 10000)
    {
        sprite = possible_sprite(obj, i++);
        while (sprite != 0xFF)
        {
            GET_SPRITE_POS(obj, sprite, &spr_x, &spr_y, &spr_w, &spr_h);
            spr_y += obj->offset_hy;
            if (obj->type == TYPE_ROULETTE || obj->type == TYPE_ROULETTE2 || obj->type == TYPE_ROULETTE3)
            {
                spr_w -= 10;
                spr_x += 5;
            }

            if ((ray_x <= spr_x + spr_w + unk_1) && (ray_x >= spr_x - unk_1))
            {
                new_dist = ray_y - spr_y;
                if (obj->type == TYPE_TIBETAIN_6 && (new_dist >= 8 && new_dist <= 10))
                    new_dist = 0;
            }
            else
                new_dist = 10000;

            if (new_dist != 10000)
                obj->follow_sprite = sprite;

            sprite = possible_sprite(obj, i++);
            if (Abs(new_dist) < 8)
                break;
        }
    }
    obj->ray_dist = new_dist;
}

//2D334
void SET_RAY_DIST_PI(obj_t* obj) {
    /* 1D594 80141D94 -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;
    s16 new_dist;
    s16 prev_flip_x = obj->flags.flip_x;

    obj->flags.flip_x = 0;
    GET_SPRITE_POS(obj, 2, &x, &y, &w, &h);
    obj->flags.flip_x = prev_flip_x;
    y += obj->offset_hy;
    x += 4;
    w = 55;
    new_dist = setToleranceDist(x, w, y);
    if (new_dist != 10000)
        obj->follow_sprite = 2;
    obj->ray_dist = new_dist;
}

//2D3EC
void SET_RAY_DIST_BAG(obj_t* obj) {
    /* 1D66C 80141E6C -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;
    s16 new_w;
    s16 new_dist = 0;

    // NOTE: slight difference from PS1 version: parentheses placed differently
    if ((obj->sub_etat == 3 ||obj->sub_etat == 6) && obj->anim_frame < 12) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);
        new_w = 45;
        y += obj->offset_hy;
        x += ((s16) (w - new_w) >> 1);
        w = new_w;
        new_dist = setToleranceDist(x, w, y);
    } else {
        new_dist = 10000;
    }

    if (ray.follow_id == obj->id && obj->sub_etat == 6 && obj->anim_frame == 11) {
        ray.follow_id = -1;
        set_main_and_sub_etat(&ray, 2, 0);
        new_dist = 10000;
        ray.speed_y -= 10;
    }
    obj->ray_dist = new_dist;
}

//2D4D0
void SET_RAY_DIST(obj_t* obj) {
    /* 1D798 80141F98 -O2 -msoft-float */
    ObjType type;
    s16 x; s16 y; s16 w; s16 h;

    // Added for debugging
    if (is_debug_mode && obj->id == debug_obj_id) {
        BREAKPOINT;
    }

    type = (ObjType)obj->type;
    if (flags[type] & flags1_8_ray_dist_multispr_cantchange) {
        SET_RAY_DIST_MULTISPR_CANTCHANGE(obj);
    } else {
        if (type == TYPE_PI || type == TYPE_BBL)
            SET_RAY_DIST_PI(obj);
        else if (type == TYPE_BAG3)
            SET_RAY_DIST_BAG(obj);
        else
        {
            GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);
            y += obj->offset_hy;
            type = (ObjType)obj->type;
            if (
                    type == TYPE_FALLING_OBJ || type == TYPE_FALLING_OBJ2 || type == TYPE_FALLING_OBJ3 ||
                    type == TYPE_FALLING_YING || type == TYPE_FALLING_YING_OUYE
                    )
            {
                x -= 2;
                w += 4;
            }
            else if (type == TYPE_MOVE_START_NUA || (type == TYPE_PLATFORM && num_world == 2))
                w -= 8;

            obj->ray_dist = setToleranceDist(x, w, y);
        }
    }
}

//2D5CC
void do_boum(void) {
    allocatePoingBoum();
    poing.is_boum = true;
    if (!poing.is_returning) {
        poing.is_returning = true;
        poing.charge = 0;
        poing_obj->speed_x = 0;
    }
}

//2D5FC
void DO_POING_COLLISION(void) {
    s16 x, y, w, h;
    GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);
    u8 btyp = 0;

    btyp = BTYP((poing_obj->x + poing_obj->offset_bx) >> 4,
                   (poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4);
    if (block_flags[btyp] & 2) {
        do_boum();
        fin_poing_follow(1);
    } else {
        for (s32 i = 0; i < actobj.num_active_objects; ++i) {
            obj_t* obj = level.objects + actobj.objects[i];
            if (new_world || new_level || fin_boss || boss_mort) {
                break;
            }
            if (debug_obj_id == obj->id) {
                BREAKPOINT;
            }
            if (obj->display_prio != 0 && obj->hit_points != 0 && (get_eta(obj)->flags & 8)) {
                s16 sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj);
                if (sprite != -1) {
                    s8 old_hp = obj->hit_points;
                    ObjectsFonctions[obj->type].poing_collision(obj, sprite);
                    do_boum();
                    obj->gravity_value_1 = 0;
                    s32 unk_1 = get_eta(obj)->anim_speed >> 4;
                    if (!(unk_1 == 10 || unk_1 == 11)) {
                        obj->gravity_value_2 = 0;
                    }
                    if (obj->hit_points == old_hp && (flags[obj->type] & flags3_1_poing_collision_snd)) {
                        PlaySnd(214, obj->id);
                    }
                    break;
                }
            }
        }
    }
}

//2D858
void DoPoingCollisionDefault(obj_t* obj, s16 sprite) {
    //nullsub
}

//2D85C
void COLL_BOX_ALL_SPRITES(s16 a1, s16 a2, s16 a3, s16 a4, obj_t* obj) {
    print_once("Not implemented: COLL_BOX_ALL_SPRITES"); //stub
}

//2D914
bool COLL_RAY_PIC(void) {
    /* 1F1B4 801439B4 -O2 -msoft-float */
    s32 ray_dist;
    s32 unk_1;
    s32 unk_2;
    s32 unk_3;
    s32 unk_4;
    s32 unk_5;
    s16 res = false;

    if ((ray.y + ray.offset_by) < (mp.height * 16)) {
        ray_dist = ray.ray_dist;
        if (ray.follow_id == -1) {
            res = (mp.map[ray_dist].tile_type) == BTYP_HURT;
        }
        if (!res) {
            unk_1 = ray_dist - mp.width;
            res = (mp.map[unk_1].tile_type) == BTYP_HURT;
            if (!res)
            {
                unk_2 = unk_1 + 1;
                unk_3 = unk_1 - 1;
                if (ray.main_etat != 2 || ray.speed_x != 0) {
                    res = false;
                    if ((mp.map[unk_3].tile_type) == BTYP_HURT || (mp.map[unk_2].tile_type) == BTYP_HURT) {
                        res = true;
                    }
                }

                if (!res && !(RayEvts.tiny) && !(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)) {
                    unk_4 = unk_1 - mp.width;
                    unk_5 = unk_4 - mp.width;
                    while (unk_5 < 0)
                        unk_5 += mp.width;
                    while (unk_4 < 0)
                        unk_4 += mp.width;
                    res = false;
                    if ((mp.map[unk_5].tile_type) == BTYP_HURT || (mp.map[unk_4].tile_type) == BTYP_HURT) {
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}

//2DA98
void COLL_RAY_BLK_MORTEL(void) {
    if ((ray.y + ray.offset_by) < (mp.height * 16)) {
        s16 btyp = mp.map[ray.ray_dist].tile_type;
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
    ray.config = 0;
}

//2DBDC
void RAY_HIT(bool hurt, obj_t* obj) {
    if (hurt) {
        RAY_HURT();
        s32 tile_x = (ray.x + ray.offset_bx) / 16;
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
                s32 eject_sens = -1;
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
                    s32 xspeed_delta = obj->speed_x - ray.speed_x;
                    if (xspeed_delta == 0) {
                        eject_sens = ray.flags.flip_x ? 1 : -1;
                    } else {
                        eject_sens = xspeed_delta > 0 ? 1 : -1;
                    }
                }
                s32 bump_speed;
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

            in_air_because_hit = true;
            jump_time = 0;
            helico_time = -1;
            ray.gravity_value_1 = 0;
            ray.gravity_value_2 = 0;
            ray.follow_id = -1;
            poing.is_charging = false;
            decalage_en_cours = 0;
            ray.nb_cmd = 0;
            if (RayEvts.super_helico) {
                button_released = 1;
            }
            Reset_air_speed(false);
        }
    }
}

//2DF34
void standard_frontZone(obj_t* obj, s16* x, s16* w) {
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
            obj->flags.hurtbyfist = 0;
    }
}

//2E738
void goToRay(obj_t* obj) {
    /* 201D8 801449D8 -O2 -msoft-float */
    s32 follow_sprite;

    PlaySnd(204, obj->id); // spawn sound
    switch (obj->type)
    {
        case TYPE_STALAG:
            if (obj->sub_etat == 4)
                calc_obj_dir(obj);
            else
                obj->flags.flip_x = 0;
            break;
        case TYPE_PLATFORM:
        case TYPE_FALLPLAT:
        case TYPE_LIFTPLAT:
        case TYPE_MOVE_PLAT:
        case TYPE_INST_PLAT:
        case TYPE_CRUMBLE_PLAT:
        case TYPE_BOING_PLAT:
        case TYPE_ONOFF_PLAT:
        case TYPE_AUTOJUMP_PLAT:
        case TYPE_OUYE:
        case TYPE_SIGNPOST:
        case TYPE_MOVE_OUYE:
        case TYPE_STONEBOMB2:
        case TYPE_CAGE:
        case TYPE_MOVE_START_NUA:
        case TYPE_BIG_BOING_PLAT:
        case TYPE_STONEBOMB3:
        case TYPE_JAUGEUP:
        case TYPE_MORNINGSTAR_MOUNTAI:
        case TYPE_MARTEAU:
        case TYPE_MOVE_MARTEAU:
        case TYPE_GROSPIC:
        case TYPE_PI:
        case TYPE_ONEUP:
        case TYPE_KILLING_EYES:
        case TYPE_RUBIS:
        case TYPE_MARK_AUTOJUMP_PLAT:
            obj->flags.flip_x = 0;
            break;
        case TYPE_BADGUY1:
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
            calc_obj_dir(obj);
            if (obj->flags.flip_x)
                skipToLabel(obj, 3, true);
            else
                skipToLabel(obj, 2, true);
            break;
        case TYPE_BLACKTOON1:
            follow_sprite = obj->follow_sprite;
            if (follow_sprite < 2 || (follow_sprite >= 4 && ((s16) follow_sprite >= 8 || follow_sprite < 6))) {
                calc_obj_dir(obj);
                if (obj->flags.flip_x)
                    skipToLabel(obj, 3, true);
                else
                    skipToLabel(obj, 2, true);
            }
            break;
        case TYPE_BIG_CLOWN:
            calc_obj_dir(obj);
            skipToLabel(obj, 7, true);
            break;
        default:
            calc_obj_dir(obj);
            break;
    }
}

//2E998
void unleashMonsterHost(obj_t* obj) {
    /* 20304 80144B04 -O2 -msoft-float */
    obj_t* linked_obj;
    s16 prev_id;
    s16 linked_id = obj->link;

    if (linked_id != obj->id) {
        do {
            linked_obj = &level.objects[linked_id];
            if (!((flags[linked_obj->type] & flags1_2_is_collectible) && (bonus_taken(linked_obj->id)))) {
                obj_init(linked_obj);
                linked_obj->flags.alive = 1;
                make_active(linked_obj, true);
                goToRay(linked_obj);
            }
            prev_id = linked_id;
            linked_id = link_init[linked_obj->id];
            suppressFromLinkList(linked_obj);
        } while (prev_id != linked_id);
    }
}

//2EA2C
void SHOW_COLLISIONS_ZONES(void) {
    for (s16 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = &level.objects[actobj.objects[i]];
        if (!(flags[obj->type] & flags0_4_no_collision) && (get_eta(obj)->flags & 0x20)) {
            CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, obj);
        }
    }
}

//2EAE8
void DO_COLLISIONS(void) {
    for (s32 i = 0; i < actobj.num_active_objects; ++i) {
        if (new_world || new_level || fin_boss || boss_mort) {
            break;
        }
        obj_t* obj = level.objects + actobj.objects[i];

        // Added for debugging
        if (obj->id == debug_obj_id) {
            BREAKPOINT;
        }

        if (get_eta(obj)->flags & 0x20)  {
            if (flags[obj->type] & flags2_0x10_do_not_check_ray_collision) {
                if (!(ray.main_etat == 3 && ray.sub_etat == 32)) {
                    s16 collision;
                    if (obj->type  == TYPE_81_CYMBALE || obj->type == TYPE_169_CYMBAL2) {
                        collision = 0;
                    } else {
                        collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, obj);
                    }
                    if (collision != -1) {
                        ObjectsFonctions[obj->type].rayman_collision(obj);
                    }
                }
            } else if (ray.config == 0 && ray.iframes_timer == -1) {
                if ((get_eta(&ray)->flags & 8) && !(flags[obj->type] & flags0_4_no_collision)) {
                    s16 collision;
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
    if (ray.config != 0) {
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
            s16 command_par3 = ray.iframes_timer;
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
void DO_OBJ_COLLISIONS(obj_t* obj, s16 offs) {
    /* 218C0 801460C0 -O2 -msoft-float */
    s16 in_x; s16 in_y; s16 in_w; s16 in_h;
    u8 done;
    s16 i;
    obj_t* cur_obj;
    s16 cur_x; s16 cur_y; s16 cur_w; s16 cur_h;

    GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);
    done = false;
    if (obj->type == TYPE_FALLING_OBJ || obj->type == TYPE_FALLING_OBJ2 || obj->type == TYPE_FALLING_OBJ3)
    {
        in_x += 10;
        in_y += 20;
        in_w += -20;
        in_h += -25;
    }
    else
    {
        in_x = offs + in_x;
        in_y = offs + in_y;
        in_w -= offs * 2;
        in_h -= offs * 2;
    }
    i = 0;
    cur_obj = &level.objects[actobj.objects[i]];
    if (i < actobj.num_active_objects)
    {
        do
        {
            if (obj != cur_obj)
            {
                switch (cur_obj->type)
                {
                    case TYPE_BADGUY1:
                        if (
                                !(cur_obj->main_etat == 2 || (cur_obj->main_etat == 0 && cur_obj->sub_etat == 3)) &&
                                (!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40) &&
                                 GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) &&
                                 inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)
                                )
                                )
                        {
                            set_main_and_sub_etat(cur_obj, 0, 4);
                            cur_obj->follow_sprite = 4;
                            cur_obj->follow_y = 0;
                            cur_obj->speed_x = 0;
                            cur_obj->speed_y = 0;
                            cur_obj->offset_hy = 11;
                            cur_obj->flags.follow_enabled = 1;
                            if (ray.follow_id == obj->id)
                                RAY_TOMBE();
                            if (cur_obj->cmd == GO_WAIT)
                            {
                                if ((cur_obj->flags.flip_x))
                                    skipToLabel(cur_obj, 3, true);
                                else
                                    skipToLabel(cur_obj, 2, true);
                            }
                            set_sub_etat(obj, 9);
                            obj->is_active = 0;
                            done = true;
                        }
                        break;
                    case TYPE_BOUEE_JOE:
                        if (
                                GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) &&
                                obj->type == TYPE_POI2 &&
                                inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h) &&
                                !(cur_obj->main_etat == 2 && cur_obj->sub_etat == 6)
                                )
                        {
                            /* sgn() on android */
                            if (
                                    obj->flags.flip_x &&
                                    (cur_obj->x + cur_obj->offset_bx) - obj->x - obj->offset_bx > -1
                                    )
                            {
                                cur_obj->config = 1;
                                skipToLabel(obj, 3, true);
                                cur_obj->speed_x = 8;
                            }
                            if (
                                    !(obj->flags.flip_x) &&
                                    (cur_obj->x + cur_obj->offset_bx) - obj->x - obj->offset_bx < 0
                                    )
                            {
                                cur_obj->config = 1;
                                skipToLabel(obj, 4, true);
                                cur_obj->speed_x = -8;
                            }
                        }
                        break;
                }

                if (done)
                    break;
            }
            i++;
            cur_obj = &level.objects[actobj.objects[i]];
        } while (i < actobj.num_active_objects);
    }
}

//2F1E0
u8 test_allowed(obj_t* obj, s16 center_x, s16 center_y) {
    /* 487A4 8016CFA4 -O2 -msoft-float */
    s16 map_index =
            ((center_x + obj->x) >> 4) +
            ((center_y + obj->y) >> 4) * mp.width;
    u8 res = false;

    if (map_index != obj->test_block_index) {
        obj->test_block_index = map_index;
        res = true;
    }
    return res;
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
            if (obj->config & 2) {
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
    /* 49320 8016DB20 -O2 */
    s16 x_cen;
    s16 y_cen;
    s16 chdir;

    x_cen = get_center_x(obj);
    y_cen = get_center_y(obj);
    chdir = on_block_chdir(obj, x_cen, y_cen);
    /* TODO: remove cast once sure of test_allowed() return */
    if (chdir && (s16) test_allowed(obj, x_cen, y_cen))
        skipToLabel(obj, 99, true);

    obj->flags.flip_x = 0;
    if (obj->cmd == GO_SPEED) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
    }
}

//2F378
void DoPTGRAPPINPoingCollision(obj_t* obj, s16 sprite) {
    if (RayEvts.grap && ray_mode != MODE_3_MORT_DE_RAYMAN) {
        if (Abs((ray.y + ray.offset_by) - obj->y + obj->offset_by) < 250) {
            SET_RAY_BALANCE();
            id_obj_grapped = obj->id;
            s16 angle = ANGLE_RAYMAN(obj);
            obj->follow_x = angle;
            if (angle > 256) {
                obj->link = -1;
            } else if (angle <= 256) {
                if (angle == 256 && !(ray.flags.flip_x)) {
                    obj->link = -1;
                } else {
                    obj->link = 1;
                }
            }
            else /* TODO: ??? */ {
                obj->link = 1;
            }

            if (obj->main_etat == 0 && obj->sub_etat == 0) {
                skipToLabel(obj, 0, true);
            }

            poing.is_active = false;
            poing.is_returning = false;
            poing_obj->flags.alive = 0;
            poing_obj->is_active = 0;
            fin_poing_follow(false);
        }
    }
}

//2F44C
void DO_ONE_CMD_WAIT(obj_t* obj) {
    if (obj->main_etat == 1) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 0);
    } else if (obj->type == TYPE_10_FISH && obj->main_etat == 0 && obj->sub_etat == 0) {
        obj->speed_y = 0;
    }
}

//2F488
void DO_ONE_CMD_LR_ATTENTE(obj_t* obj) {
    /* 4956C 8016DD6C -O2 */
    s16 sub_etat = obj->sub_etat;
    s16 type = obj->type;

    if (!(sub_etat == 1 || sub_etat == 2 || sub_etat == 11 || sub_etat == 22 ||
          sub_etat == 3 || sub_etat == 9 || sub_etat == 4 || sub_etat == 5 ||
          sub_etat == 6 || sub_etat == 15 || sub_etat == 16 || sub_etat == 17)
    ) {
        if (!(type == TYPE_CHASSEUR1 || type == TYPE_CHASSEUR2))
            set_main_and_sub_etat(obj, 1, 0);

        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
    }
    else if (sub_etat == 3 || sub_etat == 6) {
        obj->flags.read_commands = 0;
        obj->speed_y = -8;
    } else if (sub_etat == 11 || sub_etat == 2 || sub_etat == 22) {
        switch (type) {
            case TYPE_CHASSEUR1:
            case TYPE_CHASSEUR2:
                DO_TIR(obj);
                break;
            case TYPE_BIG_CLOWN:
                DO_BIG_CLOWN_ATTAK(obj);
                break;
            case TYPE_WAT_CLOWN:
                DO_WAT_CLOWN_ATTAK(obj);
                break;
        }
    }
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
            if (obj->nb_cmd > 0) {
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
    /* 49844 8016E044 -O2 */
    s16 etat = obj->main_etat;
    special_pour_liv(obj);
    switch (obj->cmd) {
        case GO_WAIT:
            DO_ONE_CMD_WAIT(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT) {
                obj->flags.flip_x = 0;
            } else {
                obj->flags.flip_x = 1;
            }

            if (etat == 1) {
                SET_X_SPEED(obj);
                CALC_MOV_ON_BLOC(obj);
            }
            else if (etat == 2) {
                SET_X_SPEED(obj);
            } else if (etat == 0) {
                DO_ONE_CMD_LR_ATTENTE(obj);
            }
            break;
        case GO_UP:
        case GO_DOWN:
            DO_ONE_CMD_UPDOWN(obj);
            break;
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_NOP:
            break;
    }
}

//2F714
void DO_ROLL_EYES(obj_t* obj) {
    /* 4BC40 80170440 -O2 -msoft-float */
    // TODO: fix? (PS1 decomp is non-matching)
    s32 temp_a0_3;
    s16 temp_v0_1;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_5;
    s16 unk_x_1;
    s16 unk_y_1;
    s16 diff_x;
    s32 temp_a0_2;
    s16 diff_y;
    s16 temp_lo;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0_4;
    s32 var_v0_1;
    s32 var_v0_2;
    s16 temp_s2;
    u16 temp_v1_2;
    u8 temp_v1_1;
    s16 fire_par_3;
    u8 y_gt_0;
    u8 x_gt_0;

    if (obj->type == TYPE_145_KILLING_EYES) {
        switch (obj->sub_etat) {
            case 0:
                if (--obj->iframes_timer == 0) {
                    obj->sub_etat = 3;
                    obj->iframes_timer = 30;
                }
                break;
            case 1:
                if (--obj->iframes_timer == 0) {
                    obj->sub_etat = 2;
                    obj->iframes_timer = 5;
                }
                break;
            case 2:
            case 5:
                if (--obj->iframes_timer == 0)
                {
                    obj->iframes_timer = obj->param;
                    obj->sub_etat = 0;
                    obj->param = 100;
                }
                break;
        }
        obj->change_anim_mode = ANIMMODE_NONE;
    }
    diff_x = ray.x - obj->x;
    diff_y = ray.y - obj->y;
    unk_x_1 = diff_x + 0x20;
    unk_y_1 = diff_y + 0x19;
    if (obj->type == TYPE_145_KILLING_EYES)
    {
        unk_x_1 = diff_x + 0x34;
        unk_y_1 = diff_y + 0x23;
    }
    x_gt_0 = unk_x_1 > 0;
    y_gt_0 = (unk_y_1 > 0);
    if (x_gt_0 == 0)
    {
        unk_x_1 = -unk_x_1;
    }
    if (y_gt_0 == 0)
    {
        unk_y_1 = -unk_y_1;
    }
    if (unk_x_1 >= 0xC9)
    {
        unk_x_1 = ashr16(unk_x_1, 1U);
        unk_y_1 = ashr16(unk_y_1, 1U);
    }

    if (unk_y_1 == 0)
    {
        if (x_gt_0)
        {
            fire_par_3 = 0x1B;
        }
        else
        {
            if (unk_x_1 != 0)
            {
                fire_par_3 = 9;
            }
            else
                fire_par_3 = -1; /* s32 or s16? */
        }
    }
    else
    {
        temp_lo = unk_x_1 * 0x2D / unk_y_1;
        if (x_gt_0)
        {
            if (y_gt_0)
            {
                if (temp_lo >= (s16) LEN(N_anim))
                {
                    fire_par_3 = 0x1B;
                }
                else
                {
                    fire_par_3 = N_anim[temp_lo] + 0x1B;
                }
            }
            else
            {
                fire_par_3 = 0;
                if (temp_lo < (s16) LEN(N_anim))
                {
                    fire_par_3 = N_anim[temp_lo];
                }
            }
        }
        else if (y_gt_0)
        {
            fire_par_3 = 0x12;
            if (temp_lo < (s16) LEN(N_anim))
            {
                fire_par_3 = N_anim[temp_lo] + 0x12;
            }
        }
        else
        {
            fire_par_3 = 9;
            if (temp_lo < (s16) LEN(N_anim))
            {
                fire_par_3 = N_anim[temp_lo] + 9;
            }
        }
    }
    if (obj->sub_etat == 3)
    {
        if ((--obj->iframes_timer) == 0)
        {
            temp_s2 = obj->screen_y;
            /*unk_y_1 = obj->screen_y;*/
            temp_a0_3 = obj->screen_x;
            obj->sub_etat = 1;
            obj->iframes_timer = 60;
            if (
                    (temp_a0_3 >= -0x33) && (temp_s2 >= -0x23) &&
                    (temp_a0_3 < 0x131) && (temp_s2 < 0xEB)
                    )
            {
                DO_REDEYE_FIRE(obj->x, obj->y, fire_par_3);
            }
            else {
                obj->param = 40;
            }
        }
    }
    if ((fire_par_3) != -1)
    {
        obj->anim_frame = fire_par_3 - 1;
    }
}

//2F978
void DoKillingEyesPoingCollision(obj_t* obj, s16 sprite) {
    --obj->hit_points;
    if (obj->hit_points != 0) {
        set_sub_etat(obj, 5);
        obj->iframes_timer = 50;
    } else {
        set_sub_etat(obj, 6);
    }
}

//2F9B8
void YaUnBloc(obj_t* obj) {
    print_once("Not implemented: YaUnBloc"); //stub
}

//2FBF4
void DO_BAG1_COMMAND(obj_t* obj) {
    /* 4C2C8 80170AC8 -O2 */
    DO_ONE_CMD(obj);
    if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0 &&
            obj->screen_x > -160 && obj->screen_x < 250 &&
            obj->screen_y < SCREEN_HEIGHT - 100
    ) {
        obj->y += 86;
        allocateLandingSmoke(obj);
        obj->y -= 86;
    }
}

//2FC64
void DoRaymanBag1Collision(obj_t* obj) {
    if (!RayEvts.squashed && scroll_x == -1 && scroll_y == -1 && ray.main_etat <= 1) {
        Ray_RayEcrase(); //TODO
    }
}

//2FCB0
s16 MarCoince(obj_t* obj, s16 dir) {
    /* 4CB88 80171388 -O2 -msoft-float */
    s32 map_ind;
    s32 pos_to_check;
    s16 res = false;
    s16 x = obj->x + obj->offset_bx - 25;
    s16 y = obj->y + obj->offset_hy;

    switch (dir)
    {
        case 2:
            pos_to_check = y - (y / 16 * 16);
            if ((s16) pos_to_check < 3)
            {
                map_ind = (x >> 4) + mp.width * (y >> 4);
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
            }
            break;
        case 3:
            pos_to_check = y - (y / 16 * 16);
            if ((s16) pos_to_check > 12)
            {
                map_ind = (x >> 4) + mp.width * ((y + 16 * 3) >> 4);
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
            }
            break;
        case 0:
            pos_to_check = x - (x / 16 * 16);
            if ((s16) pos_to_check < 3)
            {
                map_ind = ((x >> 4) + mp.width * ((y + 16) >> 4)) - 1;
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
            }
            break;
        case 1:
            pos_to_check = x - (x / 16 * 16);
            if ((s16) pos_to_check > 12)
            {
                map_ind = ((x >> 4) + mp.width * ((y + 16) >> 4)) + 3;
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
            }
            break;
    }

    return res;
}

//2FFDC
void DO_MOVE_MARACAS_COMMAND(obj_t* obj) {
    /* 4CF4C 8017174C -O2 */
    s16 cen_x;
    s16 cen_y;
    s16 speed_x;
    s16 diff_x;
    s16 diff_x_inv;
    s32 diff_x_less;

    if (obj->main_etat == 0 && obj->sub_etat == 0x0C)
    {
        cen_x = get_center_x(obj);
        cen_y = get_center_y(obj);
        if ((s16) on_block_chdir(obj, cen_x, cen_y) && (s16) test_allowed(obj, cen_x, cen_y))
            skipToLabel(obj, 99, true);

        obj->flags.flip_x = 0;
        speed_x = 0;
        if (obj->cmd == 0x14)
            obj->speed_y = obj->follow_id;

        if (ray.follow_id == obj->id)
        {
            diff_x = (ray.offset_bx + ray.x) - (obj->offset_bx + obj->x);
            if (diff_x > 0)
            {
                if (diff_x >= 11)
                    speed_x = 1;
                if (diff_x >= 16)
                    speed_x += 1;
                if (diff_x >= 19)
                    speed_x += 1;
                diff_x_less = diff_x < 43;
            }
            else
            {
                diff_x_inv = -1;
                diff_x_inv = diff_x * diff_x_inv;
                if (diff_x_inv >= 11)
                    speed_x = -1;
                if (diff_x_inv >= 16)
                    speed_x -= 1;
                if (diff_x_inv >= 19)
                    speed_x -= 1;
                diff_x_less = diff_x_inv < 42;
            }
            if (!diff_x_less)
                speed_x = 0;
        }
        obj->speed_x = speed_x;
        if (obj->speed_y < 0)
        {
            if (MarCoince(obj, 2))
            {
                obj->speed_y = 0;
                obj->param++;
                if (obj->param >= 241)
                {
                    obj->y -= 80;
                    allocateExplosion(obj);
                    PlaySnd(0x73, obj->id);
                    obj->x = obj->init_x;
                    obj->y = obj->init_y;
                    set_main_and_sub_etat(obj, 0, 4);
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    DO_NOVA(obj);
                }
            }
            else
                obj->param = 0;
        }
        if (obj->speed_x > 0)
        {
            if (MarCoince(obj, 1))
                obj->speed_x = 0;
        }
        else if (obj->speed_x < 0)
        {
            if (MarCoince(obj, 0))
                obj->speed_x = 0;
        }
    }
}

//301A4
void DO_FLASH_COMMAND(obj_t* obj) {
    /* 4D1F4 801719F4 -O2 -msoft-float */
    obj->init_x += obj->iframes_timer;
    while (Abs(obj->init_x) >= 128) {
        if (obj->iframes_timer > 0) {
            obj->x++;
            obj->init_x -= 128;
        } else {
            obj->x--;
            obj->init_x += 128;
        }
    }

    obj->init_y = obj->init_y + obj->follow_id;
    while (Abs(obj->init_y) >= 128) {
        if (obj->follow_id > 0) {
            obj->y++;
            obj->init_y -= 128;
        } else {
            obj->y--;
            obj->init_y += 128;
        }
    }

    calc_obj_pos(obj);
    obj->anim_frame = obj->hit_points - 1;
}

//30260
void DO_WLKNOT_COMMAND(obj_t* obj) {
    /* 4D7DC 80171FDC -O2 */
    s16 bx; s16 by;

    switch (obj->cmd) {
        case GO_WAIT:
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = false;
            else
                obj->flags.flip_x = true;
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            break;
    }

    if (obj->speed_x != 0) {
        bx = obj->offset_bx;
        by = obj->offset_by - 8;
        if ((s16) test_allowed(obj, bx, by) && (s16) on_block_chdir(obj, bx, by)) {
            if (!(obj->flags.flip_x)) {
                obj->flags.flip_x = true;
                skipToLabel(obj, 2, true);
            } else {
                obj->flags.flip_x = false;
                skipToLabel(obj, 1, true);
            }
        }
    }
}

//30318
void ACTIVE_L_EAU(obj_t* eau_obj) {
    /* 4D908 80172108 -O2 -msoft-float */
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;

    eau_obj->param = 130;
    eau_obj->y = ymapmax + (SCREEN_HEIGHT - 10);
    eau_obj->x = xmap - eau_obj->offset_bx;
    i = 0;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (cur_obj->type == TYPE_157_EAU && !cur_obj->is_active) {
            cur_obj->flags.flip_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->speed_x = 0;
            cur_obj->x = eau_obj->x + 101 * 1;
            cur_obj->y = eau_obj->y;
            calc_obj_pos(cur_obj);
            cur_obj->param = 130;
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            cur_obj->sub_etat = 1;
            break;
        }
        cur_obj++;
        i++;
    }
    cur_obj++;
    cur_obj->flags.flip_x = 0;
    cur_obj->speed_y = 0;
    cur_obj->speed_x = 0;
    cur_obj->x = eau_obj->x + 101 * 2;
    cur_obj->y = eau_obj->y;
    calc_obj_pos(cur_obj);
    cur_obj->param = 130;
    cur_obj->flags.alive = 1;
    cur_obj->is_active = 1;
    cur_obj->sub_etat = 2;

    cur_obj++;
    cur_obj->flags.flip_x = 0;
    cur_obj->speed_y = 0;
    cur_obj->speed_x = 0;
    cur_obj->x = eau_obj->x + 101 * 3;
    cur_obj->y = eau_obj->y;
    calc_obj_pos(cur_obj);
    cur_obj->param = 130;
    cur_obj->flags.alive = 1;
    cur_obj->is_active = 1;
    cur_obj->sub_etat = 3;
}

//30490
void DO_EAU_QUI_MONTE(obj_t* obj) {
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v1_1;
    s16 temp_v1_2;
    s16 var_v0_2;
    u16 temp_a0;
    u16 temp_v1_3;
    u8 temp_v0_1;
    u8 temp_v0_6;
    u8 temp_v0_7;
    s16 other_1;

    if ((ray.flags.alive) && !(ray.main_etat == 3 && ray.sub_etat == 23)) {
        switch (num_world)
        {
            case 1:
                if (obj->hit_points != 0) {
                    if (obj->hit_points == 1) {
                        obj->hit_points = 0;
                        ACTIVE_L_EAU(obj);
                    } else {
                        obj->hit_points--;
                    }
                } else if (obj->param > 40 || level.objects[eau_obj_id].iframes_timer == 1) {
                    if (horloge[2] != 0 && obj->y >= (SCREEN_HEIGHT - 80)) {
                        obj->speed_y = -1;
                    } else {
                        obj->speed_y = 0;
                    }
                    if (obj->param > 0) {
                        --obj->param;
                    }
                } else {
                    obj->speed_y = 0;
                }
                break;
            case 3:
                if (pierreAcorde_obj_id == -1) {
                    if (scroll_y != pierreAcorde_obj_id) {
                        obj->init_y = ymap + (SCREEN_HEIGHT - 80);
                        if (obj->y - 100 > obj->init_y) {
                            obj->y -= 3;
                        }
                        --obj->y;
                        if (obj->y > obj->init_y && horloge[2] != 0) {
                            obj->y -= 1; // NOTE: changed from 2 to 1 in PC version
                        }
                        obj->iframes_timer = 1;
                    } else {
                        if (obj->iframes_timer == 1) {
                            obj->init_y = (ray.offset_by + ray.y) - 30;
                            obj->iframes_timer = 0;
                        }
                        if (obj->init_y < obj->y) {
                            obj->y--;
                        }
                    }
                } else if ((level.objects[pierreAcorde_obj_id].hit_points != 0) && (obj->y < (ymapmax + (SCREEN_HEIGHT - 80)))) {
                    obj->sub_speed += 14;
                    if (obj->sub_speed > 16) {
                        obj->sub_speed -= 16;
                        ++obj->y;
                    }
                }
                scroll_end_y = obj->y - (Bloc_lim_H2 - 80);
            case 4:
            case 5:
                if (obj->screen_x < -166) {
                    obj->x += 505;
                    if (++obj->sub_etat >= 4)
                        obj->sub_etat = 0;
                    if (num_level == 8 && num_world == 5) {
                        var_v0_2 = obj->init_y - 5;
                        goto block_53;
                    }
                } else if (obj->screen_x > 366) {
                    obj->x -= 505;
                    if (--obj->sub_etat >= 4)
                        obj->sub_etat = 3;
                    if (num_level == 8 && num_world == 5) {
                        var_v0_2 = obj->init_y + 5;
                        block_53:
                        obj->y =
                        obj->init_y =
                                var_v0_2;
                        if (obj->y < (ymapmax + (Bloc_lim_H2 - 80)))
                        {
                            obj->y = ymapmax + (Bloc_lim_H2 - 80);
                        }
                    }
                }
                break;
        }
    } else {
        obj->speed_y = 0;
        obj->speed_x = 0;
    }
    if (obj->y < (ymap + (Bloc_lim_H2 - 80))) {
        obj->y = ymap + (Bloc_lim_H2 - 80);
    }
}

//3076C
void allocateOtherPosts(obj_t* her_bh_obj) {
    obj_t *cur_obj;
    s32 unk_1;
    s16 cur_id = link_init[her_bh_obj->id];
    u8 her_bh_frames = her_bh_obj->animations[get_eta(her_bh_obj)->anim_index].frames_count;
    s16 i = 0;

    while (cur_id != her_bh_obj->id) {
        i++;
        cur_obj = &level.objects[cur_id];
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
        cur_obj->anim_frame = (her_bh_obj->anim_frame + i) % her_bh_frames;
        cur_obj->init_x = her_bh_obj->init_x + i * ((her_bh_obj->hit_points - 2) << 5);
        cur_obj->init_y = her_bh_obj->init_y;
        unk_1 = i << 8;
        cur_obj->param = her_bh_obj->param - unk_1;
        cur_obj->x = cur_obj->init_x;
        cur_obj->y = cur_obj->init_y;
        calc_obj_pos(her_bh_obj);
        cur_id = link_init[cur_id];
    }
}

//3088C
void doHerseCommand(obj_t* obj) {
    /* 4EF0C 8017370C -O2 -msoft-float */
    if ((obj->type == TYPE_178_HERSE_BAS || obj->type == TYPE_HERSE_HAUT) && obj->link == 0) {
        obj->anim_frame = myRand(obj->animations[obj->anim_index].frames_count - 1);
        obj->param = 0x1000;
        allocateOtherPosts(obj);
        obj->link = 1;
    } else {
        obj->speed_y = sinYspeed(obj, 24, 40, &obj->param);
        obj->speed_x = 0;
    }
}

//30908
void doBlackRaymanCommand(obj_t* obj) {
    print_once("Not implemented: doBlackRaymanCommand"); //stub
}

//30BE4
void doBlKRayRaymanCollision(obj_t* obj) {
    if (ray_stack_is_full == 1) {
        ray.hit_points = 0;
    }
}

//30BF8
void DO_POELLE_COMMAND(obj_t* po_obj) {
    /* 4F818 80174018 -O2 -msoft-float */
    s32 abs_ray_spd_x;
    s16 po_x; s16 po_y; s16 po_w; s16 po_h;
    s32 sub_etat;

    po_obj->flags.flip_x = 1;
    if (ray_on_poelle == true) {
        po_obj->flags.follow_enabled = 0;
        if ((ray.speed_x > 0 && ray.flags.flip_x) || (ray.speed_x < 0 && !ray.flags.flip_x)) {
            abs_ray_spd_x = Abs(ray.speed_x);
            if (abs_ray_spd_x < 2)
                po_obj->param = ray.speed_x > 0 ? 2 : -2;
            else if (abs_ray_spd_x >= 7)
                po_obj->param = ray.speed_x > 0 ? 7 : -7;
            else
                po_obj->param = ray.speed_x;
        }
        if (po_obj->iframes_timer == 0)
        {
            if (Abs(ray.speed_y) <= 0) /* ray.speed_y == 0 */
            {
                if (Abs(ray.speed_x) > 2)
                    po_obj->anim_frame = 1;
            }
            else if (ray.speed_y > 0)
            {
                if (ray.speed_x > 0)
                    po_obj->anim_frame = 0;
                else if (ray.speed_x < 0)
                    po_obj->anim_frame = 2;
                else
                    po_obj->anim_frame = 1;
            }
            else if (ray.speed_y < 0)
            {
                if (ray.speed_x > 0)
                    po_obj->anim_frame = 2;
                else if (ray.speed_x < 0)
                    po_obj->anim_frame = 0;
                else
                    po_obj->anim_frame = 1;
            }

            po_obj->iframes_timer = 8;
        }
        else
            po_obj->iframes_timer--;

        GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h);
        po_y = (po_y + po_obj->offset_hy) - (po_obj->y + po_obj->offset_by);
        po_obj->x = (ray.offset_bx + ray.x) - po_obj->offset_bx;
        po_obj->y = (ray.offset_by + ray.y) - po_obj->offset_by - po_y;
        if ((ray.main_etat == 3 && ((sub_etat = ray.sub_etat, sub_etat == 23) || sub_etat == 22)) || !RAY_DEAD()) {
            RayEvts = SauveRayEvts;
            ray_on_poelle = false;
        }
    } else {
        po_obj->flags.follow_enabled = 1;
        po_obj->iframes_timer = 0;
        po_obj->anim_frame = 1;
        if (ray.main_etat == 3 && ray.sub_etat == 22)
            po_obj->speed_x = po_obj->param;
        else if (
                ray.main_etat == 2 &&
                ((sub_etat = ray.sub_etat, sub_etat == 1) || sub_etat == 2 || sub_etat == 6)
                )
            ray.speed_x = 0;
        else
            po_obj->param = 0;
    }
}

//30EA8
void DO_CORDE_COMMAND(obj_t* obj) {
    /* 4FB58 80174358 -O2 -msoft-float */
    s32 pa_id;
    s32 bVar2;
    s16 obj_x_pos;
    s16 obj_y_pos;
    s16 ray_x_pos;
    s16 sVar4;
    s16 ray_y_pos;
    u8 obj_y_offs;
    u8 obj_h;
    s16 inter;
    u8 hp;

    pa_id = pierreAcorde_obj_id;
    if (pa_id != -1)
    {
        if (obj->main_etat == 0)
        {
            if (obj->x < level.objects[pa_id].x)
                obj->y = level.objects[pa_id].y + 112;
            else
                obj->y = level.objects[pa_id].y + 120;
        }

        if (ray.main_etat == 2 && ray.sub_etat == 15)
        {
            /* TODO: ... */
            ray_y_pos = obj->y;

            obj_x_pos = obj->x;
            obj_y_pos = obj->y;
            bVar2 = ray.x;
            ray_x_pos = bVar2;
            ray_y_pos = ray.y;
            bVar2 = true;
            switch (obj->sub_etat)
            {
                case 16:
                    obj_y_offs = 48;
                    obj_h = 24;
                    break;
                case 17:
                    obj_y_offs = 48;
                    obj_h = 32;
                    break;
                case 18:
                case 19:
                    obj_y_offs = 56;
                    obj_h = 40;
                    break;
                case 20:
                    obj_y_offs = 56;
                    obj_h = 64;
                    break;
                default:
                    bVar2 = false;
                    break;
            }
            if (bVar2 && inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h)) {
                if (--obj->hit_points == 0)
                {
                    obj->hit_points = obj->init_hit_points;
                    if (obj->sub_etat < 19)
                        set_sub_etat(obj, obj->sub_etat + 1);
                    else
                        DO_CORDE_CASSE(obj);
                }

                if (level.objects[pierreAcorde_obj_id].iframes_timer == 0)
                    DO_FUMEE_CORDE(obj_x_pos + 128, ray_y_pos + 30);

                level.objects[pierreAcorde_obj_id].param = 0;
                return;
            }
        }
        level.objects[pierreAcorde_obj_id].param++;
    }
}

//31060
void DO_PAC_COMMAND(obj_t* obj) {
    /* 4FDF4 801745F4 -O2 -msoft-float */
    if (obj->hit_points == 0) {
        if (obj->x < (ray.x + 47) && (obj->x + 186) > ray.x) {
            obj->hit_points = 10;
            set_main_and_sub_etat(obj, 2, 4);
        }
    }

    if (obj->hit_points >= 2) {
        if (obj->speed_y < 16) {
            if (horloge[8] == 0)
                obj->speed_y++;
        } else if (horloge[8] == 0 && horloge[3] == 0) { // NOTE: changed from the PS1 to the PC version
            obj->y++;
        }
    } else {
        obj->speed_y = 0;
    }

    if (obj->param >= 2) {
        obj->iframes_timer = 0;
    } else {
        obj->iframes_timer = 1;
    }
    obj->param = 0;
}

//310F8
void DO_CFUMEE_COMMAND(obj_t* obj) {
    /* 4FEF8 801746F8 -O2 -msoft-float */
    obj->y = ray.y + -98;
    if (
            obj->anim_frame == obj->animations[obj->anim_index].frames_count - 1 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
            level.objects[pierreAcorde_obj_id].iframes_timer == 0
            )
    {
        obj->flags.alive = 0;
        obj->is_active = 0;
    }
}

//31190
void DO_NOVA2_COMMAND(obj_t* obj) {
    if (obj->param != 255) {
        if (obj->param == -1) {
            if (EOA(obj)) {
                obj->flags.alive = 0;
                obj->is_active = 0;
            }
        } else if (obj->param != 0) {
            --obj->param;
        } else {
            obj->anim_frame = 0;
            obj->param = -1;
            obj->display_prio = 2;
        }
    }
}

//31208
void doShipCommand(obj_t* obj) {
    // NOTE(Falcury): PS1 non-matching function - needs checking!
    obj_t *temp_s0_2;
    s16 temp_a0_1;
    s16 temp_v1_2;
    s32 temp_s0_1;
    u8 var_s2;
    s32 var_v0_1;
    s32 var_v0_2;
    u8 var_v1;
    u8 *var_v0_3;
    u8 temp_a0_2;
    u8 temp_v1;
    s32 temp_v1_3;
    u8 var_s3;
    obj_t *test_1;
//    LevelData *test_2;
    u8 test_3;
    u32 test_4;

#ifdef PC
    scroll_end_y = ymapmax;
    scroll_start_y = ymapmax;
    ymap = ymapmax;
    if (GameModeVideo == MODE_NORMAL) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
#endif

    /*var_s3 = saved_reg_s3;*/
    temp_v1 = obj->cmd;
    switch (temp_v1)
    {
        case 2:
            obj->speed_y = 0;
            obj->speed_x = 0;
            return;
        case 20:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            return;
        case 30:
            if (obj->link_has_gendoor) {
                temp_a0_1 = obj->param;
                var_v1 = 1;
                if (temp_a0_1 != -1)
                {
                    test_4 = level.objects[temp_a0_1].is_active;
                    var_v1 = !test_4;
                }
                if ((var_v1) && (obj->timer != 0) && (horloge[2] != 0))
                {
                    obj->timer--;
                }
                do
                {
                    var_s2 = 0;
                    if (obj->timer == 0)
                    {
                        switch (obj->link)
                        {
                            case 0:
                                obj->link = 2;
                            case 4:
                                var_s3 = obj->type;
                                break;
                            case 1:
                                obj->link = 3;
                                var_s3 = TYPE_77_PIRATE_GUETTEUR;
                                break;
                            case 2:
                                obj->link = 1;
                                var_s3 = TYPE_72_PIRATE_NGAWE;
                                var_s2 = 1;
                                break;
                            case 3:
                                obj->link = 4;
                                var_s3 = TYPE_187_MAMA_PIRATE;
                                break;
                        }
                        if (var_s2 == 0)
                        {
                            obj->timer = 50;
                        }
                        if (var_s3 != obj->type)
                        {
                            skipOneCommand(obj);
                            obj->nb_cmd = 0;
                            test_3 = obj->id;
                            do
                            {
                                test_3 = link_init[test_3];
                                temp_s0_2 = &level.objects[test_3];
                                temp_v1_3 = temp_s0_2->type;
                                if (temp_v1_3 == var_s3)
                                {
                                    break;
                                }
                            } while (temp_v1_3 != obj->type);

                            if (temp_v1_3 != obj->type)
                            {
                                switch (temp_v1_3) {
                                    case TYPE_77_PIRATE_GUETTEUR:
                                        guetteurFollowsShip(temp_s0_2);
                                        break;
                                    case TYPE_72_PIRATE_NGAWE:
                                        ngaweTriesToGrabShip(temp_s0_2);
                                        break;
                                    case TYPE_187_MAMA_PIRATE:
                                        pmamaFollowsShip(temp_s0_2);
                                        PMA_SORT_DU_CANON(temp_s0_2);
                                        break;
                                }
                                suppressFromLinkList(temp_s0_2);
                                MOVE_OBJECT(temp_s0_2);
                                calc_obj_pos(temp_s0_2);
                                temp_s0_2->flags.alive = 1;
                                obj->param = temp_s0_2->id;
                            }
                        }
                    }
                } while (var_s2 != 0);
            }
            return;
    }
}

//31400
void DO_PROP_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_PROP_COMMAND"); //stub
}

//314A4
void move_fruit_in_water(obj_t* obj) {
    s16 btyp;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s32 x2;

    u8 obj_flip_x = obj->flags.flip_x;
    if (obj->main_etat == 0)
    {
        if (obj->sub_etat == 13 && ray.follow_id == obj->id)
            set_main_and_sub_etat(obj, 0, 15);

        if (obj->main_etat == 0 && obj->sub_etat == 15)
        {
            if (obj->type == TYPE_FALLING_OBJ2 && ray.follow_id == -1)
            {
                set_sub_etat(obj, 13);
                obj->speed_x = 0;
            }
            else if (ray.follow_id == obj->id)
            {
                obj->flags.flip_x = ray.flags.flip_x;
                SET_X_SPEED(obj);
                obj->flags.flip_x = obj_flip_x;
            }

            if (obj->speed_x != 0)
            {
                GET_ANIM_POS(obj, &x, &y, &w, &h);
                x2 = x;
                if (obj->speed_x > -1)
                    x2 += w;
                btyp = BTYP(x2 >> 4, (y + 16) >> 4);
                if (block_flags[btyp] & 2) {
                    set_main_and_sub_etat(obj, 0, 13);
                    obj->speed_x = 0;
                }
            }
        }
    }
}

//315F0
void DO_FALLING_OBJ_CMD(obj_t* obj) {
    /* 50A30 80175230 -O2 */
    if (obj->main_etat == 2 && obj->btypes[0] == BTYP_WATER) {
        set_main_and_sub_etat(obj, 0, 14);
        obj->y = ((obj->offset_by + obj->y) & 0xfff0) - obj->offset_by + 7;
    }
    move_fruit_in_water(obj);
    DO_OBJ_COLLISIONS(obj, 10);
    if (obj->sub_etat == 9)
        REINIT_OBJECT(obj);
}

//3167C
void DoFallingObjPoingCollision(obj_t* obj, s16 sprite) {
    s32 fall_x_accel = 0;
    switch (obj->type)
    {
        case TYPE_FALLING_OBJ:
        case TYPE_FALLING_OBJ2:
        case TYPE_FALLING_OBJ3:
            PlaySnd(44, obj->id);
            break;
        case TYPE_FALLING_YING:
        case TYPE_FALLING_YING_OUYE:
            PlaySnd(139, obj->id);
            break;
    }

    switch (poing.sub_etat)
    {
        case 1:
        case 3:
        case 5:
            fall_x_accel = poing.damage;
            break;
        case 8:
        case 10:
        case 12:
            fall_x_accel = poing.damage - 2;
        case 2:
        case 4:
        case 6:
        case 7:
        case 9:
        case 11:
            break;
    }
    fall_x_accel = fall_x_accel * 5 + 10;
    if (poing_obj->speed_x < 0)
        fall_x_accel = -fall_x_accel;

    make_my_fruit_go_down(obj, fall_x_accel);
}

//3171C
void DO_BLKTOON_EYES_CMD(obj_t* obj) {
    /* 50C60 80175460 -O2 -msoft-float */
    s16 prev_spd_x = obj->speed_x;

    SET_X_SPEED(obj);
    if (prev_spd_x * obj->speed_x < 0) {
        obj->speed_x = prev_spd_x;
        obj->flags.flip_x ^= 1;
    }
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
