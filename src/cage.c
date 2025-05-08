
//28B40
void DO_CAGE2(obj_t* obj) {
    obj->speed_x = 0;
    obj->speed_y = -8;
}

//28B50
void DO_CAGE(obj_t* obj) {
    /* 3CF18 80161718 -O2 */
    if (obj->main_etat == 0 && obj->sub_etat == 7 && obj->anim_frame == 0 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0x0f] == 0
    ) {
        obj->display_prio = 5;
        allocateGrille(obj);
    }
}

//28BA4
void DoCagePoingCollision(obj_t* obj, i16 a2) {
    obj_hurt(obj);
    if (obj->hit_points != 0) {
        set_sub_etat(obj, 10);
    } else {
        obj->init_main_etat = 0;
        obj->init_sub_etat = 8;
        obj->init_x = obj->x;
        obj->init_y = obj->y;
        set_sub_etat(obj, 12);
        obj->init_flag = 7;
        ALLOCATE_MEDAILLON_TOON();
        ++Nb_total_cages;
        lidol_to_allocate = 5;
        PlaySnd(17, obj->id);
        lidol_source_obj = obj;
        take_bonus(obj->id);
    }
}

//28C2C
void DO_MEDAILLON_TOON(obj_t* obj) {
    /* 3CFA0 801617A0 -O2 -msoft-float */
    if (obj->sub_etat == 32 || obj->sub_etat == 33 || obj->sub_etat == 34 ||
            obj->sub_etat == 35 || obj->sub_etat == 36 || obj->sub_etat == 37
    ) {
        if (obj->anim_frame == 41)
            PlaySnd(192, obj->id);
        else if (obj->anim_frame == 64)
            PlaySnd(193, obj->id);
    }
    else if (obj->sub_etat == 38) {
        obj->flags.alive = 0;
        obj->is_active = 0;
    }
}

//28CA0
void DO_MEDAILLON_TOON_GELE(void) {
    /* 3D050 80161850 -O2 -msoft-float */
    for (i16 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = &level.objects[actobj.objects[i]];
        if (obj->type == TYPE_MEDAILLON_TOON) {
            if (!snd_flag_medaillon) {
                mute_snd_bouclant();
                snd_flag_medaillon = true;
            }
            xmap = xmap_old; // added in PC version
            ymap = ymap_old; // added in PC version
            DO_ANIM(obj);
            if (obj->sub_etat == 32 || obj->sub_etat == 33 || obj->sub_etat == 34 ||
                    obj->sub_etat == 35 || obj->sub_etat == 36 || obj->sub_etat == 37
            ) {
                if (obj->anim_frame == 41) {
                    PlaySnd(192, obj->id);
                } else if (obj->anim_frame == 64) {
                    PlaySnd(193, obj->id);
                }
            } else if (obj->sub_etat == 38) {
                gele = 0;
                snd_flag_medaillon = false;
                obj->flags.alive = 0;
                obj->is_active = 0;
                PlaySnd(61, -1);
            }
            horloges(1); // added in PC version
            break; // added - though I guess this doesn't matter if there is only one TYPE_MEDAILLON_TOON in the level
        }
    }
}

//28DD4
void ALLOCATE_MEDAILLON_TOON(void) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_197_MEDAILLON_TOON && !cur_obj->is_active) {
            cur_obj->init_x = cur_obj->x = xmap - (cur_obj->offset_bx - 160);
            cur_obj->init_y = cur_obj->y = ymap - cur_obj->offset_by;
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            cur_obj->flags.read_commands = 0;
            set_sub_etat(cur_obj, t_world_info[world_index].nb_cages + 32);
            if (t_world_info[world_index].nb_cages == 6)
                t_world_info[world_index].nb_cages = 5;
            t_world_info[world_index].nb_cages++;
            gele = 1;
            h_scroll_speed = 0;
            v_scroll_speed = 0;
            xmap = xmap_old;
            ymap = ymap_old;
            break;
        }
    }
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
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_59_CAGE2 && !cur_obj->is_active) {
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->x = obj->x;
            cur_obj->y = obj->y;
            cur_obj->screen_x = cur_obj->x - xmap;
            cur_obj->screen_y = cur_obj->y - ymap;
            cur_obj->speed_y = -4;
            break;
        }
    }
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
