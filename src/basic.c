
//1D074
bool EOA(obj_t* obj) {
    eta_t* eta = get_eta(obj);
    bool on_last_frame;
    if (eta->flags & eta_flags_0x10_anim_reverse) {
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
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        set2bits((u32*) &saveobj[i >> 4], i & 0xF, 2 * (obj->is_active & 1) + obj->flags.alive);
    }
}

//1D15C
void restore_objects_flags(void) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (obj->type != TYPE_58_CAGE) {
            u32 high_bit = 0;
            u32 low_bit = 0;
            read2bits((u32*) &saveobj[i >> 4], i & 0xF, &high_bit, &low_bit);
            obj->is_active = high_bit;
            obj->flags.alive = low_bit;
        }
    }
}

//1D204
void snapToSprite(obj_t* obj1, obj_t* obj2, u8 a3, s16 a4, s16 a5) {
    print_once("Not implemented: snapToSprite"); //stub
}

//1D4C4
obj_t* findfirstObject(s16 obj_type) {
    obj_t* obj = level.objects;
    for (s32 i = 0; i < level.nb_objects; ++i) {
        if (obj->type == obj_type) {
            break;
        }
        ++obj;
    }
    return obj;
}

//1D4F4
void findfirstInactiveObject(s16 a1) {
    print_once("Not implemented: findfirstInactiveObject"); //stub
}

//1D528
s32 sinYspeed(obj_t* obj, s16 a2, s16 a3, s16* a4) {
    /* 225B8 80146DB8 -O2 -msoft-float */
    s16 unk_1 = (*a4 + a2) & 0xFFF;
    s16 diff_y = obj->y - obj->init_y;
    s32 unk_2 = sinus(unk_1 >> 3) * a3;

    *a4 = unk_1;
    return (unk_2 >> 9) - diff_y;
}

//1D560
void set_proj_center(s16 x, s16 y) {
    PROJ_CENTER_X = x;
    PROJ_CENTER_Y = y;
}

//1D570
s32 get_proj_dist(s16 scale, s16 outer_dim) {
    //NOTE: needs checking
    if (zoom_mode == 0) {
        return outer_dim;
    } else {
        s32 temp = ((256 * 256) / (scale + 256)) * (outer_dim);
        return (temp / 256);
    }
}

//1D5B0
s32 get_proj_dist2(s16 scale, s16 a2) {
    return (a2 * ((256*256) / (scale + 256))) >> 8;
}

//1D5D8
s32 get_proj_x(s16 scale, s16 a2) {
    //NOTE: needs checking
    s32 temp = ((256 * 256) / (scale + 256)) * (a2 - PROJ_CENTER_X);
    return ((temp / 256) + PROJ_CENTER_X);
}

//1D614
s32 get_proj_y(s16 scale, s16 a2) {
    //NOTE: needs checking
    s32 temp = ((256 * 256) / (scale + 256)) * (a2 - PROJ_CENTER_Y);
    return ((temp / 256) + PROJ_CENTER_Y);
}

//1D650
void set_zoom_mode(u8 mode) {
    zoom_mode = mode;
}

//1D658
s32 inverse_proj_x(s16 scale, s16 a2) {
    return PROJ_CENTER_X + ((a2 - PROJ_CENTER_X) << 8) / ((256*256) / (scale + 256));
}

//1D690
s32 inverse_proj_y(s16 scale, s16 a2) {
    return PROJ_CENTER_Y + ((a2 - PROJ_CENTER_Y) << 8) / ((256*256) / (scale + 256));
}

//1D6C8
s32 vblToEOA(obj_t* obj, u8 a2) {
    eta_t* eta = get_eta(obj);
    s32 anim_speed = eta->anim_speed & 15;
    s32 result = obj->animations[eta->anim_index].frame_count * anim_speed * a2 - horloge[anim_speed] + 1;
    return (result <= 0) ? 1 : result;
}

//1D738
void GET_ANIM_POS(obj_t* obj, s16* x, s16* y, s16* w, s16* h) {
    ASSERT(obj->anim_index != 255);
    anim_t* anim = obj->animations + obj->anim_index;
    anim_frame_t* frame = anim->frames + obj->anim_frame;
    *w = frame->width;
    *h = frame->height;
    if (obj->flags.flip_x) {
        *x = obj->x + 2 * obj->offset_bx - frame->x - *w;
    } else {
        *x = obj->x + frame->x;
    }
    *y = obj->y + frame->y;
}

//1D7D8
void add_actobj(s16 a1) {
    print_once("Not implemented: add_actobj"); //stub
}

// 1D808
void set_sub_etat(obj_t* obj, u8 sub_etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->sub_etat = sub_etat;
}

// 1D810
void set_main_etat(obj_t* obj, u8 etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->main_etat = etat;
}

// 1D818
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->main_etat = etat;
    obj->sub_etat = sub_etat;
}

//1D824
s16 get_center_x(obj_t* obj) {
    /* 22C84 80147484 -O2 -msoft-float */
    s16 res;

    switch (obj->type) {
        case TYPE_MOVE_MARTEAU:
            res = 70;
            break;
        case TYPE_MOVE_PLAT:
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_MOVE_OUYE:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            res = 18;
            break;
        case TYPE_MOVE_RUBIS:
            res = 18;
            break;
        case TYPE_MOVE_START_PLAT:
            res = 128;
            break;
        case TYPE_BLACKTOON1:
            switch (obj->follow_sprite) {
                case 5:
                case 6:
                case 7:
                    res = 80;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_PIRATE_P_45:
        case TYPE_PIRATE_P_D_45:
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_POELLE_D:
            res = 80;
            break;
        case TYPE_ROULETTE:
        case TYPE_ROULETTE2:
        case TYPE_ROULETTE3:
            res = 104;
            break;
        case TYPE_PT_GRAPPIN:
            res = 36;
            break;
        default:
            res = 40;
            break;
    }

    return res;
}

//1D954
s16 get_center_y(obj_t* obj) {
    /* 22DEC 801475EC -O2 -msoft-float */
    s16 res;

    switch (obj->type) {
        case TYPE_SPIDER_PLAFOND:
            res = 90;
            break;
        case TYPE_MOVE_MARTEAU:
            res = 40;
            break;
        case TYPE_MOVE_PLAT:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 40;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_MOVE_OUYE:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            res = 18;
            break;
        case TYPE_MOVE_RUBIS:
            res = 18;
            break;
        case TYPE_MOVE_START_PLAT:
            res = 64;
            break;
        case TYPE_BLACKTOON1:
            switch (obj->follow_sprite) {
                case 6:
                case 7:
                    if (obj->speed_y > 0)
                        res = 64;
                    else
                        res = 78;
                    break;
                case 5:
                    res = 72;
                    break;
                default:
                    /* TODO: ??? */
                    res = 40; // NOTE: added to prevent returning an uninitialized value
                    break;
            }
            break;
        case TYPE_PIRATE_P_45:
        case TYPE_PIRATE_P_D_45:
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_POELLE_D:
            res = 96;
            break;
        case TYPE_ROULETTE:
        case TYPE_ROULETTE2:
        case TYPE_ROULETTE3:
            res = 104;
            break;
        case TYPE_PT_GRAPPIN:
            res = 32;
            break;
        default:
            res = 40;
            break;
    }

    return res;
}

//1DAC8
u8 on_block_chdir(obj_t* obj, s16 center_x, s16 center_y) {
    return (block_flags[BTYP((obj->x + center_x) >> 4, (obj->y + center_y) >> 4)] & 0x20) != 0;
}

//1DB04
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, anim_t* anim_1, anim_t* anim_2, s16 anim_frame_2) {
    u8 width_1;
    s32 unk_1;
    s32 x_1;
    s32 x_2;
    s32 unk_2;
    u8 foll_spr = obj->follow_sprite;
    anim_layer_t* layer_1 = &anim_1->layers[(anim_1->layers_per_frame & 0x3FFF) * obj->anim_frame];
    anim_layer_t* layer_2 = &anim_2->layers[(anim_2->layers_per_frame & 0x3FFF) * anim_frame_2];

    width_1 = obj->sprites[layer_1[foll_spr].sprite_index].outer_width;
    if (obj->flags.flip_x) {
        unk_1 = (u16) obj->x + (obj->offset_bx * 2) - width_1;
        x_2 = unk_1 - layer_2[foll_spr].x;
        x_1 = unk_1 - layer_1[foll_spr].x;
    } else {
        unk_2 = (u16) obj->x;
        x_1 = unk_2 + layer_1[foll_spr].x;
        x_2 = unk_2 + layer_2[foll_spr].x;
    }
    obj->follow_y = (layer_1[foll_spr].y + obj->y) - (layer_2[foll_spr].y + obj->y);
    obj->follow_x = x_1 - x_2;
}

//1DBB4
u8 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h) {
    /* 230E0 801478E0 -O2 -msoft-float */
    anim_t *anim;
    anim_layer_t *layer;
    u8 sprite_ind;
    sprite_t *sprite;
    s16 res;

    anim = &obj->animations[obj->anim_index];
    layer = &anim->layers[(anim->layers_per_frame & 0x3FFF) * obj->anim_frame];
    layer += index;
    sprite_ind = layer->sprite_index;
    sprite = &obj->sprites[sprite_ind];
    if (sprite_ind != 0 && sprite->id != 0) {
        *w = sprite->inner_width;
        *h = sprite->inner_height;
        if (obj->flags.flip_x != ((sprite->flags & 4) != 0)) {
            *x = obj->x + (obj->offset_bx * 2 - (layer->x + (sprite->sprite_pos & 0xF)) - sprite->outer_width);
        } else {
            *x = obj->x + layer->x + (sprite->sprite_pos & 0xF);
        }
        *y = layer->y + (sprite->sprite_pos >> 4) + obj->y;
        res = true;
    }
    else
        res = false;
    return res;
}

//1DCFC
void GET_RAY_ZDC(obj_t* obj, s16* x, s16* y, s16* w, s16* h) {
    /* 23204 80147A04 -O2 -msoft-float */
    u8 main_etat = obj->main_etat;
    s32 frame;

    if (obj->eta[main_etat][obj->sub_etat].flags & 0x40)
    {
        *x = obj->x + 72;
        *y = obj->y + 64;
        *w = 16;
        *h = 14;
    }
    else if (main_etat == 5)
    {
        *x = obj->x + 72;
        *y = obj->y + 40;
        *w = 16;
        *h = 46;
    }
    else if (main_etat == 7)
    {
        frame = obj->anim_frame;
        if (frame > 32)
            frame -= 32;
        frame -= 10;
        if ((s16) frame < 13U)
        {
            *x = obj->x + 64;
            *y = obj->y + 54;
            *w = 36;
            *h = 20;
        }
        else
        {
            *x = obj->x + 70;
            *y = obj->y + 40;
            *w = 20;
            *h = 40;
        }
    }
    else
    {
        *x = obj->x + 72;
        *y = obj->y + 24;
        *w = 16;
        *h = 54;
    }
    if (obj->scale != 0)
    {
        set_proj_center(obj->x + obj->offset_bx, obj->y + obj->offset_by);
        *x = get_proj_x(obj->scale, *x);
        *y = get_proj_y(obj->scale, *y);
        *w = get_proj_dist2(obj->scale, *w);
        *h = get_proj_dist2(obj->scale, *h);
        set_proj_center(obj->screen_x + obj->offset_bx, obj->screen_y + obj->offset_by);
    }
}

//1DEC0
void GET_BB1_ZDCs(obj_t* obj, s16* a2, s16* a3, s16* a4, s16* a5, s16* a6, s16* a7, s16* a8, s16* a9) {
    print_once("Not implemented: GET_BB1_ZDCs"); //stub
}

//1DFB8
void calc_obj_dir(obj_t* obj) {
    if (ray.x + ray.offset_bx > obj->x + obj->offset_bx) {
        obj->flags.flip_x = true;
    } else {
        obj->flags.flip_x = false;
    }
}

//1DFF4
u8 OBJ_IN_ZONE(obj_t* obj) {
    /* 236C4 80147EC4 -O2 -msoft-float */
    switch(obj->type) {
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
        case TYPE_LIDOLPINK:
        case TYPE_LIDOLPINK2:
            return obj->detect_zone_flag == 1;
        default:
            return obj->detect_zone_flag != 0;
    }
}

//1E030
void calc_obj_pos(obj_t* obj) {
    obj->screen_y = obj->y - ymap;
    obj->screen_x = obj->x - xmap + 8;
}

//1E05C
void makeUturn(obj_t* obj) {
    /* 2375C 80147F5C -O2 -msoft-float */
    u8 label = 255;
    u8 foll_spr;

    switch (obj->type)
    {
        case TYPE_BLACKTOON1:
            foll_spr = obj->follow_sprite;
            if (foll_spr == 1 || foll_spr == 2 || obj->follow_sprite == 4 || obj->follow_sprite == 7)
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 3;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 2;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_MITE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 3))
            {
                label = 3;
                obj->follow_id = 0;
            }
            break;
        case TYPE_BADGUY1:
            if (
                    !(obj->main_etat == 0 &&
                      (obj->sub_etat == 3 || obj->sub_etat == 1 || obj->sub_etat == 4 ||
                       obj->sub_etat == 5 || obj->sub_etat == 6))
                    )
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
            if (!(obj->main_etat == 0 && (obj->sub_etat == 3 || obj->sub_etat == 1)))
            {
                if (obj->flags.read_commands)
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 1;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 0;
                        obj->flags.flip_x = 0;
                    }
                }
                else
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 6;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 5;
                        obj->flags.flip_x = 0;
                    }
                }
            }
            break;
        case TYPE_GENEBADGUY:
            if (!(obj->main_etat == 0 && obj->sub_etat == 3))
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_STONEMAN1:
        case TYPE_STONEMAN2:
            if (obj->cmd == GO_LEFT)
            {
                label = 5;
                obj->flags.flip_x = 1;
            }
            else if (obj->cmd == GO_RIGHT)
            {
                label = 4;
                obj->flags.flip_x = 0;
            }
            break;
        case TYPE_TROMPETTE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 1))
            {
                if (obj->flags.read_commands)
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 1;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 0;
                        obj->flags.flip_x = 0;
                    }
                }
            }
            break;
        case TYPE_BIG_CLOWN:
        case TYPE_WAT_CLOWN:
            if (obj->flags.read_commands)
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_SPIDER:
            if (obj->cmd == GO_LEFT)
            {
                label = 2;
                obj->flags.flip_x = 1;
            }
            else
            {
                label = 0;
                obj->flags.flip_x = 0;
            }
            break;
        case TYPE_MITE2:
            if (!(obj->flags.flip_x))
                obj->flags.flip_x = 1;
            else
                obj->flags.flip_x = 0;

            if (!(obj->main_etat == 0 && obj->sub_etat == 3))
            {
                label = 3;
                obj->follow_id = 0;
            }
            break;
        case TYPE_CAISSE_CLAIRE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 2))
            {
                set_main_and_sub_etat(obj, 1, 1);
                if (!(obj->flags.flip_x))
                {
                    obj->flags.flip_x = 1;
                    label = 3;
                }
                else
                {
                    obj->flags.flip_x = 0;
                    label = 2;
                }
            }
            break;
        case TYPE_WALK_NOTE_1:
            if (obj->cmd == GO_LEFT)
            {
                label = 2;
                obj->flags.flip_x = 1;
            }
            else if (obj->cmd == GO_RIGHT)
            {
                label = 1;
                obj->flags.flip_x = 0;
            }
            break;
        case TYPE_SPIDER_PLAFOND:
            if (!(obj->main_etat == 0 && (obj->sub_etat == 24 || obj->sub_etat == 18 || obj->sub_etat == 19)))
            {
                set_main_and_sub_etat(obj, 0, 24);
                obj->speed_y = 0;
                obj->speed_x = 0;
                obj->flags.flip_x = 1 - obj->flags.flip_x;
            }
            break;
    }

    if (label != 255) {
        // NOTE: difference with PS1 version
        if (flags[obj->type] & flags1_0x10_move_x) {
            obj->x -= instantSpeed(obj->speed_x);
        } else {
            obj->x -= obj->speed_x;
        }
        obj->speed_x = 0;
        skipToLabel(obj, label, true);
    }
}

//1E588
u8 BTYP(s32 tile_x, s32 tile_y) {
    if (tile_x >= 0 && tile_x <= (mp.width - 1) && tile_y >= 0 && tile_y <= (mp.height - 1)) {
        return mp.map[tile_y * mp.width + tile_x].tile_type;
    } else {
        return BTYP_NONE;
    }
}

//1E608
void calc_btyp_square(obj_t* obj) {
    s16 x_offset;
    if (obj->type == TYPE_23_RAYMAN) {
        x_offset = RayEvts.tiny ? 4 : 8;
    } else {
        x_offset = 16;
    }

    s16 x = obj->x + obj->offset_bx;
    s16 y = obj->y + obj->offset_by;
    s16 tile_x = ashr16(x, 4);
    s16 tile_y = ashr16(y, 4);
    s16 tile_x_left = ashr16(x - x_offset, 4);
    s16 tile_x_right = ashr16(x + x_offset, 4);

    obj->btypes[3] = BTYP(tile_x, tile_y - 1); // up
    obj->btypes[1] = BTYP(tile_x_left, tile_y); // left

    // center
    if (obj->main_etat == 2) {
        obj->btypes[0] = bloc_floor(BTYP(tile_x, tile_y), x % 16, y % 16);
    } else {
        obj->btypes[0] = BTYP(tile_x, tile_y);
    }

    obj->btypes[2] = BTYP(tile_x_right, tile_y); // right
    obj->btypes[4] = BTYP(tile_x, tile_y + 1); // down
}

//1E76C
void DO_OBJ_REBOND_EN_X(obj_t* obj) {
    if (block_flags[obj->btypes[2]] & 0x10 || block_flags[obj->btypes[1]] & 0x10) {
        obj->speed_x = -obj->speed_x;
    }
}

//1E790
u8 calc_btyp(obj_t* obj) {
    /* 23F80 80148780 -O2 -msoft-float */
    u8 btyp;
    u8 btypes_1_2_solid;
    s32 ray_x; s32 ray_y;
    u8* ray_btypes;
    u8* new_btyp;
    u8 foll_spr;

    calc_btyp_square(obj);

    if (obj->type == TYPE_RAYMAN)
    {
        ray.param = -1;
        switch (ray.btypes[0])
        {
            case BTYP_NONE:
            case BTYP_CHDIR:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SOLID_LEFT_45:
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_LIANE:
            case BTYP_SOLID_PASSTHROUGH:
            case BTYP_SOLID:
                ray_last_ground_btyp = true;
                break;
            case BTYP_SLIPPERY_RIGHT_45:
            case BTYP_SLIPPERY_LEFT_45:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
            case BTYP_SLIPPERY:
                ray_last_ground_btyp = false;
                break;
        }
    }

    if (!(block_flags[obj->btypes[0]] & 2)) {
        if (obj->type == TYPE_RAYMAN) {
            btyp = mp.map[ray.ray_dist].tile_type;
        } else {
            btyp = BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4);
        }

        if (obj->main_etat == 2 && !(block_flags[btyp] & 2)) {
            btyp = BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by + 16) >> 4);
        }

        if (!(block_flags[btyp] & 2)) {
            btypes_1_2_solid = ((block_flags[obj->btypes[1]] & 2) != 0) | (block_flags[obj->btypes[2]] & 2);
            if (btypes_1_2_solid == 3) /* both 1 and 2 have solid flag */ {
                if (((obj->offset_bx + (u16) obj->x) & 0xF) < 8)
                    btypes_1_2_solid = 1;
                else
                    btypes_1_2_solid = 2;
            }

            if (btypes_1_2_solid != 0) {
                btypes_1_2_solid--;
                if (obj->type == TYPE_RAYMAN) {
                    if (ray_last_ground_btyp == true) {
                        ray_x = ray.x + ray.offset_bx;
                        if (btypes_1_2_solid)
                            ray_x += 16;
                        else
                            ray_x -= 16;
                        ray_y = ray.y + ray.offset_by - 16;
                        if (!(block_flags[(u8) BTYP(ray_x >> 4, ray_y >> 4)] & 2)) {
                            if (ray.main_etat != 2) {
                                /* ??? don't understand */
                                if (btypes_1_2_solid != (ray.flags.flip_x))
                                    obj->param = 0;
                                else
                                    obj->param = 1;
                            }

                            ray_btypes = ray.btypes;
                            if (btypes_1_2_solid)
                                new_btyp = &ray_btypes[2];
                            else
                                new_btyp = &ray_btypes[1];
                            ray_btypes[0] = *new_btyp;
                        }
                    }
                } else {
                    if ((obj->flags.flip_x) != btypes_1_2_solid && obj->main_etat != 2 &&
                        ((obj->type == TYPE_BADGUY1 && obj->flags.read_commands) ||
                            obj->type == TYPE_BADGUY2 || obj->type == TYPE_BADGUY3 || obj->type == TYPE_GENEBADGUY ||
                            obj->type == TYPE_STONEMAN1 || obj->type == TYPE_STONEMAN2 ||
                            obj->type == TYPE_BIG_CLOWN || obj->type == TYPE_WAT_CLOWN ||
                         (obj->type == TYPE_SPIDER && obj->param != 0) ||
                            obj->type == TYPE_TROMPETTE || obj->type == TYPE_MITE || obj->type == TYPE_MITE2 ||
                            obj->type == TYPE_CAISSE_CLAIRE || obj->type == TYPE_WALK_NOTE_1 || obj->type == TYPE_SPIDER_PLAFOND ||
                            (obj->type == TYPE_BLACKTOON1 &&
                             (foll_spr = obj->follow_sprite, foll_spr == 1 || foll_spr == 4 || (foll_spr == 7 && obj->config == 2)) &&
                             !(obj->main_etat == 0 && obj->sub_etat == 4)
                            )
                        )
                    ) {
                        makeUturn(obj);
                        obj->btypes[0] = BTYP_SOLID;
                    }
                }
            }
            btyp = obj->btypes[0];
            return btyp;
        } else {
            return btyp;
        }
    } else {
        btyp = obj->btypes[0];
        return btyp;
    }
}

//1EAA8
void init_obj_in_the_air(obj_t* obj) {
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
}

//1EAB4
void make_my_fruit_go_down(obj_t* obj, s16 x_accel) {
    /* 243C4 80148BC4 -O2 -msoft-float */
    if (obj->main_etat == 2 && Abs(obj->speed_x) < 32)
        obj->speed_x += x_accel;
    if (obj->type == TYPE_FALLING_YING_OUYE)
    {
        if (obj->main_etat != 2 || obj->sub_etat != 0)
        {
            set_main_and_sub_etat(obj, 2, 0);
            init_obj_in_the_air(obj);
        }
    }
    else
    {
        if (obj->main_etat != 2 || obj->sub_etat != 2)
        {
            set_main_and_sub_etat(obj, 2, 2);
            init_obj_in_the_air(obj);
        }
    }
}

//1EB18
void switchOff(obj_t* obj) {
    if (EOA(obj)) {
        obj->flags.alive = 0;
        obj->is_active = 0;
        if (!(ray_mode == MODE_5_CASSE_BRIQUE && obj->type == TYPE_161_WIZ)) {
            del_alwobj(obj->id);
        }
        if (obj->type == TYPE_75_PAILLETTE || obj->type == TYPE_170_RAYON) {
            obj->x = -32000;
            obj->y = -32000;
        }
    }
}

//1EB80
void obj_hurt(obj_t* target) {
    if (poing.damage > target->hit_points) {
        target->hit_points = 0;
    } else {
        target->hit_points -= poing.damage;
    }
}

//1EBA0
void Projectil_to_RM(obj_t* obj, s16* out_spd_x, s16* out_spd_y, s16 new_spd_x, s16 new_spd_y) {
    /* 249D0 801491D0 -O2 -msoft-float */
    s16 diff_x; s16 diff_y;
    s16 diff_x_abs; s16 diff_y_abs;
    s16 unk_x; s16 unk_y;
    s32 spd_x;

    *out_spd_x = 0;
    *out_spd_y = 0;
    diff_x = ray.offset_bx + obj->follow_x - obj->x - obj->offset_bx;
    diff_y = ray.offset_by + obj->follow_y - obj->y - obj->offset_by;
    diff_x_abs = Abs(diff_x);
    diff_y_abs = Abs(diff_y);
    if (diff_x != 0 && diff_y != 0) {
        if (diff_x_abs > diff_y_abs) {
            unk_x = diff_x / diff_y_abs;
            unk_y = diff_y / diff_y_abs;
            *out_spd_x = new_spd_x;
            if (diff_x <= 0) {
                *out_spd_x = -new_spd_x;
            }
            *out_spd_y = diff_y * new_spd_y / diff_x_abs;
            if (*out_spd_y == 0) {
                spd_x = *out_spd_x;
                if (diff_x > 0) {
                    spd_x += 16;
                } else {
                    spd_x -= 16;
                }
                *out_spd_x = spd_x;
            }
        } else {
            unk_y = diff_y / diff_x_abs;
            unk_x = diff_x / diff_x_abs;
            *out_spd_y = new_spd_y;
            if (diff_y <= 0) {
                *out_spd_y = -new_spd_y;
            }
            *out_spd_x = diff_x * new_spd_x / diff_y_abs;
        }
    } else if (diff_y == 0) {
        *out_spd_x = new_spd_x;
        if (diff_x > 0) {
            *out_spd_x = -new_spd_x;
        }
        unk_x = -1;
        if (diff_x > 0) {
            unk_x = 1;
        }
        unk_y = 0;
    } else {
        *out_spd_y = new_spd_y;
        if (diff_y <= 0) {
            *out_spd_y = -new_spd_y;
        }
        unk_x = 0;
        unk_y = -1;
        if (diff_y > 0) {
            unk_y = 1;
        }
    }

    if ((!(obj->flags.flip_x) && *out_spd_x > 0) || (obj->flags.flip_x && *out_spd_x < 0)) {
        *out_spd_x = -*out_spd_x;
        if (*out_spd_y == 0) {
            *out_spd_y = 2;
        }
    }

    obj->follow_x += unk_x * 2;
    obj->follow_y += unk_y * 2;
}

//1EE18
void del_actobj(obj_t* obj) {
    print_once("Not implemented: del_actobj"); //stub
}

//1EEB8
s32 calc_let_Width(u8 font_size, s32 num_let) {
    obj_t* obj = NULL;
    sprite_t* sprite;
    if (num_let > 1000) {
        sprite = alpha_numbers->sprites + (num_let - 1000);
    } else {
        switch (font_size) {
            case 0:
                sprite = alpha2->sprites + num_let;
                break;
            case 1:
                sprite = alpha2->sprites + num_let + 41;
                break;
            case 2:
                sprite = alpha->sprites + num_let;
                break;
            default:
                // NOTE: possible bug here with an uninitialized pointer in the PC version (?)
                return 0;
        }
    }
    if (sprite) {
        return sprite->outer_width;
    } else {
        return 0;
    }
}

//1EF1C
s32 calc_let_Width2(u8 font_size, s32 num_let) {
    obj_t* obj = NULL;
    sprite_t* sprite;
    if (num_let > 1000) {
        sprite = alpha_numbers->sprites + (num_let - 1000);
    } else {
        if (font_size <= 1) {
            if (font_size == 1) {
                num_let += 41;
            }
            sprite = alpha2->sprites + num_let;
        } else {
            if (font_size != 2) {
                return 0; // NOTE: possible bug here with an uninitialized pointer in the PC version (?)
            }
            sprite = alpha->sprites + num_let;
        }
    }
    if (sprite) {
        s32 width = (sprite->sprite_pos & 0xF) + sprite->inner_width;
        return width;
    } else {
        return 0;
    }
}

//1EF80
void INIT_TEXT_TO_DISPLAY(void) {
    for (s32 i = 0; i < COUNT(text_to_display); ++i) {
        text_to_display[i].text[0] = '\0';
        text_to_display[i].is_fond = 0;
        text_to_display[i].field_D5 = 0;
    }
}

//1EFB4
s32 deter_num_let(u8 c, const char* next_chars) {
    if (c == ']') {
        char next_text[4] = {0};
        strncpy(next_text, next_chars, 3);
        return atoi(next_text) + 1000;
    }
    if (c >= 'a' && c <= 'z') {
        return c - 93;
    } else if (c >= '0' && c <= '9') {
        return c - 18;
    } else if (c == '?') {
        return 1;
    } else if (c == '|') {
        return 49;
    } else if (c == '~') {
        return 46;
    } else if (c == 129) {
        return 44;
    } else if (c == 148) {
        return 43;
    } else if (c == 248) {
        return 45;
    } else if (c == 156) {
        return 42;
    } else if (c == '#') {
        return 47;
    } else if (c == '%') {
        return 45;
    } else if (c == '!') {
        return 2;
    } else if (c == '\'') {
        return 41;
    } else if (c == '*') {
        return 48;
    } else if (c == '.') {
        return 3;
    } else if (c == ':') {
        return 40;
    }
    return 0;
}

//1F194
void calc_num_let_spr(u8 a1, u8* a2) {
    print_once("Not implemented: calc_num_let_spr"); //stub
}

//1F1B0
s16 calc_largmax_text(const char* text, s16 char_index, s16 space_width, s16 char_spacing, u8 font_size) {
    s16 width = 0;
    for (;;) {
        ++char_index;
        char c = text[char_index];
        if (c == '\0' || c == '/') {
            break;
        }
        if (c == ' ') {
            width += space_width;
        } else {
            s32 num_let = deter_num_let(c, text + char_index + 1);
            if (num_let != 0) {
                width += calc_let_Width2(font_size, num_let) - char_spacing;
                if (num_let >= 1000) {
                    char_index += 3;
                }
            }
        }
    }
    return width;
}

//1F21C
void INIT_TXT_BOX(display_item_t* box) {
    char text[200];
    u8 font_size = box->font_size;
    s32 total_width = 0;
    s32 max_width = 0;
    s32 line_height = 0;
    s32 num_lines = 0;
    s32 max_chars = 0;
    s32 total_chars = 0;
    s32 is_slash = 0;
    s32 char_spacing = 0;
    s32 space_width = 0;
    s32 num_let = 0;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }
    memcpy(text, box->text, sizeof(text));
    if (text[0] != '\0') {
        for (s32 i = 0; i < sizeof(text); ++i) {
            char c = text[i];
            if (c == '\0') {
                break;
            }
            if (c == '/') {
                is_slash = 1;
                ++num_lines;
                if (total_width > max_width) {
                    max_width = total_width;
                }
                total_width = 0;
                if (total_chars > max_chars) {
                    max_chars = total_chars;
                }
                total_chars = 0;
                num_let = 0;
            } else if (c == ' ') {
                num_let = 0;
                total_width += space_width;
                ++total_chars;
            } else {
                num_let = deter_num_let(text[i], text + i + 1);
            }
            if (num_let != 0) {
                total_chars += 1;
                total_width += calc_let_Width2(font_size, num_let) - char_spacing;
                if (num_let >= 1000) {
                    i += 3;
                }
            }
        }
        if (num_lines != 0) {
            --num_lines;
        } else {
            max_width = total_width;
            num_lines = 1;
        }
        if (is_slash) {
            box->centered_x_pos = box->xpos - (max_width >> 1);
        } else {
            box->centered_x_pos = box->xpos;
        }
        box->width = max_width + 2;
        box->centered_y_pos = box->ypos - line_height + 2;
        box->height = num_lines * line_height - 2;
    }
}

//1F4A0
void Deter_Option_Caract(char* text, s16 key, s16 offset) {
    if (offset == 255) {
        s16 len = (s16)strlen(text);
        for (s16 i = 0; i < len; ++i) {
            char c = text[i];
            if (c == '$') {
                offset = i;
            }
        }
    }
    if (offset != 255) {
        switch(key) {
            case 0: {
                text[offset] = 'a';
            } break;
            case 1: {
                text[offset] = 'b';
            } break;
            case 2: {
                text[offset] = 'c';
            } break;
            case 3: {
                text[offset] = 'd';
            } break;
            default: break;
        }
    }
}

//1F50C
void SwapAB(s16* a, s16* b) {
    s16 temp = *a;
    *a= *b;
    *b = temp;
}

// Taken from the PS1 decomp for now (reconstructing this doesn't look fun)
//1F520
void Bresenham(void (*func)(s16, s16), s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 param_6, s16 percent)
{
    s16 sp10;
    s16 sp12;
    s16 sp14;
    s16 sp16;
    void (*sp18)(s16, s16);
    s16 sp20;
    s32 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s32 var_s1_1;
    s16 var_s1_2;
    s16 var_s1_3;
    s16 var_s1_4;
    s16 var_s3;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_lo_1;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_s6_1;
    s32 temp_s6_2;
    s32 temp_s6_3;
    s32 temp_s6_4;
    s16 var_s0_1;
    s32 var_s2_1;
    s32 var_s2_2;
    s32 var_s4;
    s32 var_s5;
    s32 var_s7_1;
    s32 var_s7_2;
    s32 var_v0;
    s32 test_1;
    s32 test_2;

    /*sp18 = param_1;*/
    sp10 = origin_x;
    sp14 = origin_y;
    sp12 = dest_x;
    var_s0_1 = 0;
    var_s5 = dest_x - origin_x;
    sp16 = dest_y;
    var_s4 = dest_y - origin_y;
    test_1 = param_6;
    sp20 = test_1;
    temp_s6_1 = percent;
    if (var_s5 == 0)
    {
        var_s3 = 0;
    }
    else
    {
        var_s3 = Abs(var_s4 / var_s5) < 1;
    }
    if (var_s3 == 1)
    {
        if (var_s5 < 0)
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            SwapAB(&sp14, &sp16);
            var_s5 = -var_s5;
            var_s4 = -var_s4;
        }
        else if ((var_s5 == 0) && (var_s4 < 0))
        {
            var_s0_1 = 1;
            SwapAB(&sp14, &sp16);
            var_s4 = -var_s4;
        }

        if (var_s4 <= 0)
        {
            var_s7_1 = var_s4 >> 0x1F;
        }
        else
            var_s7_1 = 1;

        var_s4 = Abs(var_s4);
        var_s2_1 = (var_s4 * 2) - var_s5;
        temp_a3 = var_s4 - var_s5;
        if (var_s0_1 != 0)
        {
            var_s0_2 = sp12;
            temp_s6_1 = var_s0_2 + ((sp10 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp16;

            var_s7_1 = -var_s7_1;
            while (var_s0_2 >= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a3 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s4 * 2);
                }

                if (sp20 < var_s3)
                {
                    func(var_s0_2, var_s1_1);
                    var_s3 = 0;
                }
                var_s0_2 -= 1;
            }
        }
        else
        {
            var_s0_2 = sp10;
            temp_s6_1 = var_s0_2 + ((sp12 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2 = var_s0_2 + 1;
            var_s1_1 = sp14;

            while (var_s0_2 <= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a3 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s4 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s0_2, var_s1_1);
                    var_s3 = 0;
                }
                var_s0_2 += 1;
            }
        }
    }
    else
    {
        if (var_s4 < 0)
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            SwapAB(&sp14, &sp16);
            var_s5 = -var_s5;
            var_s4 = -var_s4;
        }
        else if ((var_s4 == 0) && (var_s5 < 0))
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            var_s5 = -var_s5;
        }

        if (var_s5 <= 0)
        {
            var_s7_1 = var_s5 >> 0x1F;
        }
        else
            var_s7_1 = 1;
        var_s5 = Abs(var_s5);
        var_s2_1 = (var_s5 * 2) - var_s4;
        temp_a0 = var_s5 - var_s4;
        if (var_s0_1 != 0)
        {
            var_s0_2 = sp16;
            temp_s6_1 = var_s0_2 + ((sp14 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp12;
            var_s7_1 = -var_s7_1;
            while (var_s0_2 >= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a0 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s5 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s1_1, var_s0_2);
                    var_s3 = 0;
                }
                var_s0_2 -= 1;
            }
        }
        else
        {
            var_s0_2 = sp14;
            temp_s6_1 = var_s0_2 + ((sp16 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp10;
            while (var_s0_2 <= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a0 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s5 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s1_1, var_s0_2);
                    var_s3 = 0;
                }
                var_s0_2 += 1;
            }
        }
    }
}

//1F880
void init_finBossLevel(void) {
    memset(&finBosslevel, 0, sizeof(finBosslevel));
}

//1F8A4
void Change_Wait_Anim(void) {
    eta_t** ray_eta = ray.eta;
    u8 prev_00 = ray_eta[0][0].anim_index;
    ray_eta[3][20].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][0].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][56].anim_index = prev_00;
    first_boss_meet = 1 - first_boss_meet;
}

//1F8E8
void add_alwobj(obj_t* obj) {
    // Added check to prevent duplicate entries
    s32 found_index = -1;
    for (s32 i = 0; i < level_obj.nb_objects; ++i) {
        if (level_obj.obj_ids[i] == obj->id) {
            found_index = i;
            break;
        }
    }
    if (found_index == -1) {
        level_obj.obj_ids[level_obj.nb_objects++] = obj->id;
    } else {
        printf("add_alwobj(): tried to add an object for the second time\n");
    }
}

//1F918
void del_alwobj(s16 obj_index) {
    s32 found_index = -1;
    for (s32 i = 0; i < level_obj.nb_objects; ++i) {
        if (level_obj.obj_ids[i] == obj_index) {
            found_index = i;
            break;
        }
    }
    if (found_index >= 0 && found_index < level_obj.nb_objects) {
        --level_obj.nb_objects;
        level_obj.obj_ids[found_index] = level_obj.obj_ids[level_obj.nb_objects];
    }
}

//1F988
void FatalError(char* message) {
    stop_cd();
    InitTextMode();
    printf("Rayman says fatal error :\n%s\n", message);
    //PrintMemInfo();
    exit(-1);
    //stub
}

//1FA00
u8 sprite_of_ref(obj_t* obj, u8 ref) {
    for (s32 i = 0; i < obj->nb_sprites; ++i) {
        if (obj->sprites[i].id == ref) {
            return i;
        }
    }
    return 0;
}

