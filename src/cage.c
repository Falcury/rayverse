
//28B40
void DO_CAGE2(obj_t* obj) {
    obj->speed_x = 0;
    obj->speed_y = -8;
}

//28B50
void DO_CAGE(obj_t* obj) {
    //stub
}

//28BA4
void DoCagePoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//28C2C
void DO_MEDAILLON_TOON(obj_t* obj) {
    //stub
}

//28CA0
void DO_MEDAILLON_TOON_GELE(void) {
    //stub
}

//28DD4
void ALLOCATE_MEDAILLON_TOON(void) {
    //stub
}

//28F14
void allocate_toons(obj_t* src_obj, u8 count) {
    obj_t* obj = findfirstObject(TYPE_3_LIDOLPINK);
    obj_t* max_obj = obj + 14; //NOTE: do levels contain up to
    u8 layers;
    if (src_obj->type == TYPE_23_RAYMAN) {
        layers = ray.animations[ray.anim_index].layers_per_frame;
    } else {
        layers = 1;
    }

    count = MIN(count, COUNT(Toon_Speeds_y)); // added for safety (prevent out-of-bounds reading)

    i32 toon_index = 0;
    for (; obj < max_obj; ++obj) {
        if (!obj->is_active && obj->type == TYPE_3_LIDOLPINK) {
            set_main_and_sub_etat(obj, 2, 0);
            obj->iframes_timer = 20;
            obj->gravity_value_1 = 0;
            obj->gravity_value_2 = 5;
            obj->flags.alive = 1;
            obj->is_active = 1;

            obj->speed_y = Toon_Speeds_y[toon_index];
            obj->speed_x = Toon_Speeds_x[toon_index];
            add_alwobj(obj);
            if (obj->speed_x >= 1) {
                obj->eta[2][0].speed_x_right = obj->speed_x;
                obj->eta[2][0].speed_x_left = -obj->speed_x;
                obj->flags.flip_x = 1;
                skipToLabel(obj, 2, true);
            }
            else {
                obj->eta[2][0].speed_x_right = -obj->speed_x;
                obj->eta[2][0].speed_x_left = obj->speed_x;
                obj->flags.flip_x = 0;
                skipToLabel(obj, 2, true);
            }
            obj->eta[2][1].speed_x_right = obj->eta[2][0].speed_x_right;
            obj->eta[2][1].speed_x_left = obj->eta[2][0].speed_x_left;
            s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
            GET_SPRITE_POS(src_obj, toon_index % layers, &spr_x, &spr_y, &spr_w, &spr_h);
            obj->x = spr_x + (spr_w >> 1) - obj->offset_bx;
            obj->y = spr_y + (spr_h >> 1) - obj->offset_by;
            calc_obj_pos(obj);
            obj->anim_frame = 0;
            obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
            obj->flags.flag_0x40 = 0;
            ++toon_index;
            if (toon_index == count) {
                break;
            }
        }
    }
}

//2912C
void allocateGrille(obj_t* obj) {
    //stub
}

//291B4
void DO_ONE_PINK_CMD(obj_t* obj) {
    if (obj->iframes_timer != 0)
        obj->iframes_timer--;

    if (
            (gerbe && (obj->eta[obj->main_etat][obj->sub_etat].flags & 2)) ||
            (obj->main_etat == 2 && obj->sub_etat == 1 && obj->speed_y > 2)
            )
    {
        set_main_and_sub_etat(obj, 0, 5);
        obj->speed_x = 0;
        obj->speed_y = -10;
    }

    switch (obj->main_etat * 0x100 + obj->sub_etat)
    {
        case 0x2:
            if (EOA(obj))
                obj->flags.flip_x = !obj->flags.flip_x;
            break;
        case 0x100:
            SET_X_SPEED(obj);
            if (block_flags[calc_typ_travd(obj, false)] & 1) {
                obj->flags.flip_x = !obj->flags.flip_x;
            }
            CALC_MOV_ON_BLOC(obj);
            return;
        case 0x202:
            obj->speed_x = 0;
        case 0x200:
        case 0x201:
            if (obj->speed_y < 0 && (block_flags[(u8) calc_typ_trav(obj, 1)] & 0x10))
            {
                if (!gerbe)
                    set_sub_etat(obj, 2);
                obj->speed_y = 0;
            }
            break;
        case 0x0:
        case 0x1:
        case 0x3:
        case 0x4:
        case 0x5:
        default:
            break;
    }
}

//2937C
void DoLidolPinkRaymanZDD(obj_t* obj) {
    if (get_eta(obj)->flags & 1) {
        skipToLabel(obj, 0, 1);
    }
}
