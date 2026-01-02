
//55480
void INIT_HORLOGES(void) {
    memset(horloge, 0, sizeof(horloge));
}

//55498
void horloges(u8 ticks) {
    for (s32 i = 0; i < 25; ++i) {
        horloge[i] += 1;
        if (horloge[i] >= i) {
            horloge[i] = 0;
        }
    }
    map_time += ticks;
}

//554EC
void init_allowed_time(void) {
    for (s32 i = 0; i < 192; ++i) {
        allowed_time[i] = -2;
    }
    s16* allowed_time_world = allowed_time;
    allowed_time_world[17] = 30; //magician (swinging plums)
    allowed_time_world[18] = 45; //magician (flower platforms)
    allowed_time_world[19] = 25; //magician (grass platforms)
    allowed_time_world[20] = 35; //magician (low flower platforms)
    allowed_time_world += 32;
    allowed_time_world[16] = 40; //magician (slippery platforms)
    allowed_time_world[17] = 25; //magician (rotating platforms)
    allowed_time_world += 32;
    allowed_time_world[11] = 45; //magician (disappearing clouds)
    allowed_time_world[12] = 20; //magician (bouncing clouds)
    allowed_time_world += 32;
    allowed_time_world[11] = 35; //magician (parkour)
    allowed_time_world[12] = 50; //magician (bouncy erasers)
    allowed_time_world += 32;
    allowed_time_world[11] = 35; //magician (ring parkour)
}

//55AC8
void fades(void) {
    if (map_time == 0) {
        INIT_FADE_IN();
    }
    if (dead_time >= 32 && dead_time <= display_mode + 33) {
        if (fade == 65) {
            INIT_FADE_OUT();
        }
    }
    if ((fade & 0x40) == 0) {
        DO_FADE();
    }
}

//55B24
s16 frapsol(s16 a1) {
    return ashr16(expsin[a1], 7);
}

//55B48
void DO_SCREEN_TREMBLE(void) {
    // NOTE(Falcury): PS1 and PC versions appear to be identical
    s16 unk_1;

    if (screen_trembling != 0) {
        unk_1 = frapsol(screen_trembling);
        screen_trembling++;
        if (unk_1 == 255) {
            unk_1 = 0;
            screen_trembling = 0;
        }
        ymap += unk_1;
    }
}

//55B90
void DO_SCREEN_TREMBLE3(void) {
    // NOTE(Falcury): PS1 and PC versions appear to be identical
    s16 unk_1 = expsin2[screen_trembling3];

    if (unk_1 != 255) {
        screen_trembling3++;
        ymap += unk_1;
    } else {
        screen_trembling3 = 0;
    }
}

//55BCC
void DO_SCREEN_TREMBLE2(void) {
    // NOTE(Falcury): PS1 and PC versions appear to be identical
    s16 unk_1 = expsin2[screen_trembling2];

    if (unk_1 != 255) {
        screen_trembling2++;
        xmap += unk_1;
    } else {
        screen_trembling2 = 0;
    }
}

//55C08
void DO_SCROLL(s16* h_speed, s16* v_speed) {
    if (*v_speed == 255) {
        *v_speed = 0;
    }
    if (num_world == world_6_cake && num_level == 4) {
        *h_speed = 0;
    }
    if (num_world == world_5_cave && num_level == 11) {
        *h_speed = 0;
    }
    if (num_world == world_5_cave && num_level == 3) {
        *h_speed = 0;
        *v_speed = 0;
    }
    DO_SCREEN_TREMBLE(); //TODO
    if (screen_trembling2 > 0) {
        DO_SCREEN_TREMBLE2(); //TODO
    }
    if (screen_trembling3 > 0) {
        DO_SCREEN_TREMBLE3(); //TODO
    }

    ymap += *v_speed;
    xmap += *h_speed;

    if (xmap < scroll_start_x) {
        *h_speed += scroll_start_x - xmap;
        xmap = scroll_start_x;
        dhspeed = 0;
    } else if (xmap > scroll_end_x) {
        *h_speed -= xmap - scroll_end_x;
        xmap = scroll_end_x;
        dhspeed = 0;
    }

    if (ymap < scroll_start_y) {
        *v_speed += scroll_start_y - ymap;
        ymap = scroll_start_y;
        dvspeed = 0;
    } else if (ymap > scroll_end_y) {
        *v_speed -= ymap - scroll_end_y;
        ymap = scroll_end_y;
        dvspeed = 0;
    }

    calc_obj_pos(&ray);
}

//55D78
void allocateLandingSmoke(obj_t* in_obj) {
    /* 29F3C 8014E73C -O2 -msoft-float */
    s16 i;
    s16 j;
    obj_t* cur_obj;
    s16 nb_objs;

    for (i = 0; i <= 2; i += 2)
    {
        cur_obj = &level.objects[0];
        nb_objs = level.nb_objects;
        j = 0;
        while (j < nb_objs)
        {
            if (cur_obj->type == TYPE_208_LANDING_SMOKE && !cur_obj->is_active) {
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                add_alwobj(cur_obj);
                cur_obj->active_timer = 0;
                cur_obj->active_flag = ACTIVE_ALIVE;
                set_main_and_sub_etat(cur_obj, 0, i != 0);
                cur_obj->x = in_obj->x + in_obj->offset_bx - cur_obj->offset_bx;
                cur_obj->y = in_obj->y + in_obj->offset_by - cur_obj->offset_by;
                if (i != 0)
                    cur_obj->display_prio = 2;
                else
                    cur_obj->display_prio = 7;
                calc_obj_pos(cur_obj);
                break;
            }
            cur_obj++;
            j++;
        }
    }
}

//55E60
s32 instantSpeed(s16 speed) {
    s32 spd_shr = ashr16(speed, 4);
    s32 spd_abs_max = Abs(speed) & 0xF;
    if (spd_abs_max != 0) {
        spd_shr += sgn(speed) * (ashr32(map_time * spd_abs_max, 4) - ashr32(map_time * spd_abs_max - spd_abs_max, 4));
    }
    return spd_shr;
}

//55EE4
void SET_X_SPEED(obj_t* obj) {
    s32 xspeed = 0;
    anim_t* anim = obj->animations + obj->anim_index;
    u8 horloge_index = ((anim->layers_per_frame & 0xC000) >> 14) + 1;
    if (horloge[horloge_index] == 0) {
        eta_t* eta = get_eta(obj);
        if (obj->flags.flip_x) {
            xspeed = eta->speed_x_right * horloge_index;
        } else {
            xspeed = eta->speed_x_left * horloge_index;
        }
        if (obj->type == TYPE_23_RAYMAN && RayEvts.tiny) {
            xspeed /= 2;
        }
    }
    obj->speed_x = (s16)xspeed;
}

//55F9C
void make_active2(obj_t* obj, u8 do_nova) {
    obj->is_active = 1;
    obj->field_1C = 1;
    obj->active_flag = 0;
    if (do_nova) {
        DO_NOVA(obj);
    }
}


//55FBC
void make_active(obj_t* obj, u8 do_nova) {
    if (obj->flags.alive) {
        obj->is_active = 1;
        obj->field_1C = 1;
        obj->active_flag = 0;
        if (do_nova) {
            DO_NOVA(obj);
        }
    }
}

//55FE4
bool in_action_zone(s16 screen_x, s16 screen_y, obj_t* obj, u8 active) {
    s32 in_zone = 0;
    s16 zdiff_x = zonediffx[ot];
    s16 zdiff_y = zonediffy[ot];
    u8 offset_bx = obj->offset_bx;
    if (flags[ot] & flags0_0x10_keep_active) {
        if ((flags[ot] & flags0_0x80_boss) && IsBossThere) {
            in_zone = 1;
        } else {
            //TODO: check that this is right for the PC version, there seem to be some oddities here but PC/Android sadly don't decompile cleanly
            switch(ot) {
                case TYPE_153_FEE:
                case TYPE_94_POING:
                case TYPE_120_BATTEUR_FOU:
                case TYPE_204_BLACK_RAY:
                case TYPE_205_BLACK_FIST:
                case TYPE_190_COUTEAU:
                case TYPE_215_DARK_SORT:
                case TYPE_143_NOVA2:
                    in_zone = 1;
                    break;
                case TYPE_10_FISH:
                case TYPE_97_BBL:
                    if ((u16) (screen_x + 283) < 727)
                        in_zone = 1;
                    break;
                case TYPE_220_PIERREACORDE:
                    if (((s16) screen_y + zdiff_y > -50) && (u16) (screen_x + 283) < 783)
                    {
                        if (level.objects[pierreAcorde_obj_id].hit_points == 0)
                        {
                            level.objects[pierreAcorde_obj_id].hit_points = 10;
                            set_main_and_sub_etat(&level.objects[pierreAcorde_obj_id], 2, 4);
                        }
                    }
                case TYPE_219_CORDE:
                    if (level.objects[pierreAcorde_obj_id].hit_points != 0)
                        in_zone = 1;
                    break;
                case TYPE_157_EAU:
                    if ((u16) (screen_x + 283) < 783)
                        in_zone = 1;
                    break;
                case TYPE_122_STONEDOG2:
                    if (((u16) (screen_x + 283) < 727) && ((u16) (screen_y + 99) < 349))
                        in_zone = 1;
                    break;
                case TYPE_244_SMA_PETIT_LASER:
                case TYPE_192_SMA_GRAND_LASER:
                    offset_bx = 128;
                    break;
                // NOTE: the below cases only seems to be present in the PS1 version?
                /*case TYPE_75_PAILLETTE:
                case TYPE_76_DESTROYING_DOOR:
                    if (((u16) (obj->y - 50) > 320) || !IsBossThere)
                    {
                        if (ray.cmd_arg_2 == obj->id)
                        {
                            ray.cmd_arg_2 = -1;
                            obj->ray_dist = 1000;
                            set_main_and_sub_etat(&ray, 2, 2);
                        }
                        in_zone = 2;
                    }
                    else
                        in_zone = 1;
                    break;*/
                default: break;
            }
        }
    }

    if (in_zone == 0) {
        if (active) {
            zdiff_x += 60;
            zdiff_y += 60;
        }
        in_zone = 1;
        if (((s16) screen_x < -284 - zdiff_x) || ((s16) screen_x > zdiff_x + 444) ||
                ((s16) screen_y < -144 - zdiff_y) || ((s16) screen_y > zdiff_y + 304) ||
                (obj->y + obj->offset_by < -30) || (obj->y > mp.height * 16 + 30) ||
                (obj->x + offset_bx * 2 < 0) || (obj->x > mp.width * 16)
        ) {
            in_zone = 0;
        }
    }

    if (in_zone && (flags[ot] & flags0_0x80_boss)) {
        IsBossThere = true;
        if ((!first_boss_meet && obj->hit_points != 0 && scrollLocked) || (first_boss_meet && obj->hit_points == 0)) {
            Change_Wait_Anim();
        }
    }

    return in_zone;
}

//56310
void kill_obj(obj_t* obj) {
    obj->x = -32000;
    obj->y = -32000;
    obj->flags.alive = 0;
    obj->active_flag = 1;
}

//56338
void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj) {
    ot = obj->type;
    s32 was_active = obj->is_active;
    obj->is_active = 0;
    if (obj->flags.alive) {
        if (in_action_zone(screen_x, screen_y, obj, was_active)) {
            if (obj->active_timer != 0 ||
                    (!(flags[ot] & flags1_1_keep_linked_objects_active) &&
                    obj->active_flag >= 2 &&
                     (obj->active_flag == 2 || ot != TYPE_10_FISH))
            ) {
                if (obj->active_timer > 0) {
                    --obj->active_timer;
                }
            } else {
                obj->is_active = was_active;
                if ((flags[ot] & flags0_1_always) || (flags[ot] & flags3_0x40_no_link)) {
                    make_active2(obj, !obj->is_active && (flags[ot] & flags1_1_keep_linked_objects_active));
                } else {
                    s16 id = obj->id;
                    do {
                        obj_t* cur_obj = level.objects + id;
                        make_active(cur_obj, !cur_obj->is_active && (flags[obj->type] & flags1_1_keep_linked_objects_active));
                        id = link_init[cur_obj->id];
                    } while (id != obj->id);
                }
            }
        } else {
            if (obj->active_flag != 1) {
                obj->active_timer = 0;
                if (flags[ot] & flags2_0x20_kill_if_outside_active_zone) {
                    kill_obj(obj);
                } else if (obj->active_flag == 2) {
                    obj->active_flag = 1;
                } else if (obj->active_flag == 0 || obj->active_flag == 4) {
                    if (flags[ot] & flags0_1_always) {
                        REINIT_OBJECT(obj);
                    } else {
                        obj->active_flag = 4;
                        obj->is_active = 1;

                        bool dont_reinit_linked_objects = false;
                        if (obj->type != TYPE_10_FISH) {
                            s16 id = obj->id;
                            while (1) {
                                obj_t* cur_obj = level.objects + id;
                                if (cur_obj->active_flag != 4) {
                                    dont_reinit_linked_objects = true;
                                    break;
                                } else {
                                    id = link_init[cur_obj->id];
                                    if (id == obj->id) {
                                        break;
                                    }
                                }
                            }
                        }

                        if (obj->y + obj->offset_by < -30) {
                            obj->is_active = 0;
                        }

                        if (!dont_reinit_linked_objects) {
                            s16 id = obj->id;
                            do {
                                obj_t* cur_obj = level.objects + id;
                                REINIT_OBJECT(cur_obj);
                                id = link_init[cur_obj->id];
                            } while (id != obj->id);
                        }
                    }
                }
            }
        }
    }
}

//565D0
s32 DO_PESANTEUR(obj_t* obj) {
    /* 2C214 80150A14 -O2 -msoft-float */

    s16 spd_y;
    s32 res = false;
    s16 y_accel = 0;
    u8 anim_speed_div = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;

    if (anim_speed_div != 0)
    {
        if (obj->type == TYPE_TAMBOUR1 || obj->type == TYPE_TAMBOUR2)
        {
            if (obj->config != 0)
                obj->config--;
            else if (obj->gravity_value_1 == 0)
            {
                obj->speed_y++;
                res = true;
            }
        }
        else
        {
            switch (anim_speed_div)
            {
                case 6:
                    if (horloge[2] == 0)
                    {
                        y_accel = 1;
                        res = true;
                    }
                    break;
                case 1:
                    if (obj->gravity_value_1 == 0)
                    {
                        y_accel = 1;
                        res = true;
                    }
                    break;
                case 2:
                    if (obj->gravity_value_2 == 0)
                    {
                        y_accel = 1;
                        res = true;
                    }
                    break;
                case 3:
                    spd_y = obj->speed_y;
                    if (spd_y > 0)
                    {
                        if (spd_y > 1)
                            y_accel = -1;
                    }
                    else
                        y_accel = 1;
                    break;
                case 4:
                    spd_y = obj->speed_y;
                    if (spd_y < -1)
                    {
                        if (spd_y < -2)
                            y_accel = 1;
                    }
                    else
                        y_accel = -1;
                    break;
                case 5:
                    if (obj->gravity_value_1 == 0)
                        y_accel = -1;
                    break;
                case 10:
                    obj->gravity_value_1++;
                    if (obj->gravity_value_1 >= obj->gravity_value_2)
                    {
                        obj->gravity_value_1 = 0;
                        y_accel = 1;
                        res = true;
                    }
                    break;
                case 11:
                    obj->gravity_value_1++;
                    if (obj->gravity_value_1 >= obj->gravity_value_2)
                    {
                        obj->gravity_value_1 = 0;
                        y_accel = -1;
                        res = true;
                    }
                    break;
            }
        }

        if (flags[obj->type] & flags1_0x20_move_y)
            y_accel = ashl16(y_accel, 4);
        obj->speed_y += y_accel;
    }
    return res;
}

//56734
void freezeAnim(obj_t* obj, u8 a2) {
    /* 2C45C 80150C5C -O2 -msoft-float */
    s16 count = obj->animations[obj->anim_index].frame_count - 1;
    s32 count_capped;

    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x10)
    {
        /* duplicate... */
        count_capped = count;
        MIN_3(count_capped, a2);
        obj->anim_frame = count - count_capped + 1;
    }
    else
    {
        count_capped = count;
        MIN_3(count_capped, a2);
        obj->anim_frame = count_capped;
    }
}

//567AC
void DO_ANIM(obj_t* obj) {
    s32 prev_anim_frame = obj->anim_frame;
    s32 prev_anim_index = obj->anim_index;
    eta_t* eta = get_eta(obj);
    anim_t* prev_anim = obj->animations + obj->anim_index;
    u8 anim_speed = eta->anim_speed & 15;
    if (anim_speed != 0 && horloge[anim_speed] == 0) {
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            --obj->anim_frame;
        } else {
            ++obj->anim_frame;
        }
    }
    obj->anim_index = eta->anim_index;

    if (obj->anim_index == 255) {
        // Added for debugging.
        // Some invalid ETA exist (e.g. moskito's main_etat 0, sub_etat 21), which have anim_index == 255. These may be
        // leftovers from removed sub_etats that were present in the PS1 version.
        // Arriving here is a bug! We'll throw a FatalError now, to avoid out-of-bounds reading elsewhere in the code.
        char error_msg[512];
        printf(error_msg, sizeof(error_msg),
               "DO_ANIM(): invalid anim_index 255; id=%d, type=%d, x=%d, y=%d, main_etat=%d, sub_etat=%d\n",
               obj->id, obj->type, obj->x, obj->y, obj->main_etat, obj->sub_etat);
        FatalError(error_msg);
    }

    anim_t* anim = obj->animations + obj->anim_index;
    if ((obj->change_anim_mode == ANIMMODE_RESET_IF_NEW && obj->anim_index != prev_anim_index) || obj->change_anim_mode == ANIMMODE_RESET) {
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->id);
        }
    }
    if (obj->anim_frame >= anim->frame_count || obj->anim_frame == 255) {
        // animation ended
        obj->main_etat = eta->next_etat;
        obj->sub_etat = eta->next_subetat;
        eta = get_eta(obj);
        obj->anim_index = eta->anim_index;
        anim = obj->animations + obj->anim_index;
        if ((obj->type == TYPE_23_RAYMAN && ray_old_sub_etat == 8 && (ray_old_main_etat == 2 || ray_old_main_etat == 6) && obj->sub_etat != 8) ||
                (ray_old_sub_etat == 61 && ray_old_main_etat == 0 && obj->sub_etat != 61)
        ) {
            if (ray.timer > 60 && !RayEvts.squashed) {
                ray.timer = 60;
            }
        }
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->id);
        }
    }
    obj->change_anim_mode = ANIMMODE_NONE;
    if (obj->flags.follow_enabled) {
        CALC_FOLLOW_SPRITE_SPEED(obj, anim, prev_anim, prev_anim_frame);
    }
    u8 anim_changed_bit = 0;
    if (obj->anim_frame != prev_anim_frame || obj->anim_index != prev_anim_index) {
        anim_changed_bit = 1;
    }
    obj->flags.anim_changed = anim_changed_bit;

}

//56A38
s16 prof_in_bloc(obj_t* obj) {
    return (obj->y + obj->offset_by) & 0xF;
}

//56A4C
void do_boing(obj_t* obj, u8 main_etat, u8 sub_etat) {
    /* 2C8D0 801510D0 -O2 -msoft-float */
    if (obj->speed_y < 2) {
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, main_etat, sub_etat);
        if (main_etat != 2)
            recale_position(obj);
    } else {
        if (horloge[2] == 0) {
            switch (obj->btypes[0]) {
                case BTYP_NONE:
                    break;
                case BTYP_SOLID_RIGHT_45:
                case BTYP_SLIPPERY_RIGHT_45:
                    obj->speed_y = 1 - (obj->speed_y >> 1);
                    obj->speed_x -= 2;
                    break;
                case BTYP_SOLID_LEFT_45:
                case BTYP_SLIPPERY_LEFT_45:
                    obj->speed_y = 1 - (obj->speed_y >> 1);
                    obj->speed_x += 2;
                    break;
                case BTYP_SOLID_RIGHT1_30:
                case BTYP_SOLID_RIGHT2_30:
                case BTYP_SLIPPERY_RIGHT1_30:
                case BTYP_SLIPPERY_RIGHT2_30:
                    obj->speed_y = 1 - ((obj->speed_y + 1) >> 1);
                    obj->speed_x--;
                    break;
                case BTYP_SOLID_LEFT1_30:
                case BTYP_SOLID_LEFT2_30:
                case BTYP_SLIPPERY_LEFT1_30:
                case BTYP_SLIPPERY_LEFT2_30:
                    obj->speed_y = 1 - ((obj->speed_y + 1) >> 1);
                    obj->speed_x++;
                    break;
                case BTYP_SOLID_PASSTHROUGH:
                case BTYP_SOLID:
                case BTYP_SLIPPERY:
                    obj->speed_y = -((obj->speed_y + 1) >> 1);
                    break;
                case BTYP_RESSORT:
                    obj->speed_y = -((obj->speed_y + 3) >> 1);
                    break;
            }
        }

        if (obj->speed_y < 0)
            set_sub_etat(obj, 0);
    }
}

//56BC4
u8 underSlope(obj_t* obj) {
    /* 2CA58 80151258 -O2 -msoft-float */
    u8 res = false;
    if (block_flags[obj->btypes[0]] & 1)
        res = block_flags[obj->btypes[3]] & 0x40;

    return res;
}

//56BEC
void DO_STONEBOMB_REBOND(obj_t* obj) {
    /* 2CAAC 801512AC -O2 -msoft-float */
    s16 new_spd_x; s16 new_spd_y;
    ObjType obj_type = (ObjType)obj->type;

    if (obj_type == TYPE_STONEBOMB2) {
        new_spd_x = 32;
        new_spd_y = -4;
    }
    else if (obj_type == TYPE_STONEBOMB3) {
        new_spd_x = 48;
        new_spd_y = -6;
    }

    if (underSlope(obj))
        obj->btypes[0] = obj->btypes[3];

    switch (obj->btypes[0]) {
        case BTYP_SOLID_LEFT1_30:
        case BTYP_SOLID_LEFT2_30:
        case BTYP_SLIPPERY_LEFT1_30:
        case BTYP_SLIPPERY_LEFT2_30:
            obj->speed_y = new_spd_y;
            obj->speed_x = new_spd_x;
            break;
        case BTYP_SOLID_RIGHT1_30:
        case BTYP_SOLID_RIGHT2_30:
        case BTYP_SLIPPERY_RIGHT1_30:
        case BTYP_SLIPPERY_RIGHT2_30:
            obj->speed_y = new_spd_y;
            obj->speed_x = -new_spd_x;
            break;
        case BTYP_SOLID_LEFT_45:
        case BTYP_SLIPPERY_LEFT_45:
            obj->speed_y = new_spd_y + 2;
            obj->speed_x = new_spd_x + 16;
            break;
        case BTYP_SOLID_RIGHT_45:
        case BTYP_SLIPPERY_RIGHT_45:
            obj->speed_y = new_spd_y + 2;
            obj->speed_x = -16 - new_spd_x;
            break;
        case BTYP_SOLID_PASSTHROUGH:
        case BTYP_SOLID:
        case BTYP_SLIPPERY:
            obj->speed_y = new_spd_y;
            break;
        case BTYP_RESSORT:
            obj->speed_y = new_spd_y - 3;
            break;
    }
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 2;
}

//56CD8
void DO_THROWN_BOMB_REBOND(obj_t* obj, s16 pesanteur, s16 a3) {
    /* 2CBC4 801513C4 -O2 -msoft-float */
    ObjType old_type;
    u8 under;
    u8 btyp;
    u8 anim_speed;
    s32 mul_45 = 5;
    s32 mul_30 = 3;
    u8 div_45 = 1;
    u8 div_30 = 1;

    if (obj->main_etat == 2) {
        if (obj->sub_etat == 1) {
            DO_STONE_EXPLOSION(obj, -1);
            return;
        }
        if (obj->sub_etat == 0) {
            old_type = (ObjType)obj->type;
            obj->type = TYPE_STONEBOMB2;
            DO_STONEBOMB_REBOND(obj);
            obj->type = old_type;
            return;
        }
    }

    if (obj->speed_y >= 0) {
        under = underSlope(obj);
        if (under)
            btyp = obj->btypes[3];
        else
            btyp = obj->btypes[0];

        switch (btyp) {
            case BTYP_NONE:
            case BTYP_SLIPPERY:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SLIPPERY_RIGHT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_LEFT_45:
            case BTYP_SLIPPERY_LEFT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_RESSORT:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                break;
        }
        if (a3 > 0) {
            if (obj->speed_y >= 2) {
                obj->speed_y = a3 - obj->speed_y;
                if (pesanteur)
                    obj->speed_y++;
            }
            else
                obj->speed_y = 0;
        }
        else
            obj->speed_y = -3;

        if (under) {
            while (BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4) == obj->btypes[0]) {
                obj->y--;
            }

            recale_position(obj);
            calc_btyp(obj);
        } else if ((block_flags[obj->btypes[0]] & 1) && prof_in_bloc(obj) >= 4 && obj->speed_y < 3) {
            if (obj->speed_y == 0)
                obj->speed_y = 1;
        }
        else
            recale_position(obj);
    }
    obj->gravity_value_1 = 0;
    anim_speed = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;
    if (!(anim_speed == 10 || anim_speed == 11))
        obj->gravity_value_2 = 0;
}

//56F00
void DO_FRUIT_REBOND(obj_t* obj, s16 pesanteur, s16 a3) {
    /* 2D008 80151808 -O2 -msoft-float */
    u8 under;
    u8 btyp;
    ObjType obj_type;
    u8 anim_speed;
    s16 accel_x;
    s16 speed_x_bnd;
    s32 mul_45;
    s32 mul_30;
    u8 div_45;
    u8 div_30;

    if (obj->type == TYPE_GRAINE) {
        accel_x = 1;
        speed_x_bnd = 1;
        mul_45 = 1;
        mul_30 = 1;
        div_45 = 1;
        div_30 = 2;
    }
    else {
        accel_x = 5;
        speed_x_bnd = 4;
        mul_45 = 5;
        mul_30 = 3;
        div_45 = 1;
        div_30 = 1;
    }

    if (obj->speed_y >= 0) {
        if (obj->speed_x > 0)
            obj->speed_x -= accel_x;
        else if (obj->speed_x < 0)
            obj->speed_x += accel_x;

        if (obj->speed_x >= -speed_x_bnd && obj->speed_x <= speed_x_bnd)
            obj->speed_x = 0;

        under = underSlope(obj);
        if (under)
            btyp = obj->btypes[3];
        else
            btyp = obj->btypes[0];

        switch (btyp) {
            case BTYP_NONE:
            case BTYP_SLIPPERY:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SLIPPERY_RIGHT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_LEFT_45:
            case BTYP_SLIPPERY_LEFT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_RESSORT:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                break;
        }

        if (a3 > 0) {
            if (obj->speed_y >= 2) {
                obj->speed_y = a3 - obj->speed_y;
                if (pesanteur)
                    obj->speed_y++;
            } else {
                obj->speed_y = 0;
                obj->hit_points = 2;
            }
        } else {
            obj_type = (ObjType)obj->type;
            if (obj_type == TYPE_FALLING_OBJ || obj_type == TYPE_FALLING_OBJ2 || obj_type == TYPE_FALLING_OBJ3 ||
                obj_type == TYPE_FALLING_YING || obj_type == TYPE_FALLING_YING_OUYE
            ) {
                if (ray.follow_id == obj->id) {
                    obj->speed_y = RayEvts.tiny ? -4 : -3;
                }
                else
                    obj->speed_y = -5;

                if (obj->type == TYPE_FALLING_YING_OUYE)
                    obj->speed_y++;
            }
            else
                obj->speed_y = -3;
        }

        if (under) {
            while (BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4) == obj->btypes[0]) {
                obj->y--;
            }

            calc_btyp(obj);
            recale_position(obj);
        } else if ((block_flags[obj->btypes[0]] & 1) && prof_in_bloc(obj) >= 4 && obj->speed_y < 3) {
            if (obj->speed_y == 0)
                obj->speed_y = 1;
        }
        else
            recale_position(obj);
    }

    obj->gravity_value_1 = 0;
    anim_speed = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;
    if (!(anim_speed == 10 || anim_speed == 11))
        obj->gravity_value_2 = 0;
}

//5720C
void Drop_Atter(obj_t* obj) {
    set_main_and_sub_etat(obj, 2, 3);
    obj->speed_y = 0;
    obj->speed_x = 0;
}

//57230
void BadGuyAtter(obj_t* obj) {
    recale_position(obj);
    if (obj->hit_points == obj->init_hit_points) {
        set_main_and_sub_etat(obj, 1, 0);
        obj->flags.read_commands = 1;
    } else {
        set_main_and_sub_etat(obj, 1, (get_eta(obj)->flags & 0x40) ? 2 : 11);
    }
    calc_obj_dir(obj);
    skipToLabel(obj, obj->flags.flip_x ? 3 : 2, 1);
}

//572C4
void MiteAtter(obj_t* obj) {
    if (obj->speed_y > 0) {
        recale_position(obj);
        skipToLabel(obj, 2, 1);
        if (obj->follow_id > 500) {
            obj->follow_id = 0;
        }
    }
}

//572F8
void LidolPinkAtter(obj_t* obj) {
    if (gerbe != 0 && obj->sub_etat == 1) {
        do_boing(obj, 1, 0);
    } else {
        skipToLabel(obj, 1, 1);
        obj->speed_y = 0;
        recale_position(obj);
    }
}

//57344
void stoneDogAtter(obj_t* obj) {
    if (obj->sub_etat == 2) {
        skipToLabel(obj, 4, 1);
        recale_position(obj);
    }
}

//5736C
void stoneDogBounces(obj_t* obj) {
    /* 2D7D0 80151FD0 -O2 -msoft-float */
    s32 x = obj->x + obj->offset_bx;
    s32 y = obj->y + obj->offset_by;
    s32 block_l = mp.map[((x - 2) >> 4) + ((y >> 4) * mp.width)].tile_type;
    s32 block_r = mp.map[((x + 2) >> 4) + ((y >> 4) * mp.width)].tile_type;

    if (obj->main_etat == 2 && !(block_flags[obj->btypes[0]] & 2) &&
            ((block_flags[block_l] & 1 && !(obj->flags.flip_x)) || (block_flags[block_r] & 1 && obj->flags.flip_x))
    ) {
        if (obj->speed_y < 0) {
            if (obj->flags.flip_x)
                skipToLabel(obj, 6, true);
            else
                skipToLabel(obj, 7, true);
        } else {
            obj->x += obj->speed_x / 16;
            skipToLabel(obj, 4, true);
            obj->flags.flip_x ^= 1;
        }
    }
}

//574B8
void Spider_Atter(obj_t* obj) {
    set_main_and_sub_etat(obj, 1, 0);
    if (obj->flags.flip_x) {
        skipToLabel(obj, 3, 1);
    } else {
        skipToLabel(obj, 1, 1);
    }
}

//574F4
void NormalAtter(obj_t* obj) {
    set_main_and_sub_etat(obj, obj->init_main_etat, obj->init_sub_etat);
}

//57530
void OBJ_IN_THE_AIR(obj_t* obj) {
    /* 2DA58 80152258 -O2 -msoft-float */
    s16 pesanteur;
    s16 spd_y;
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    u8 anim_speed_div = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;

    if (!(anim_speed_div == 10 || anim_speed_div == 11))
    {
        obj->gravity_value_1++;
        if (obj->gravity_value_1 > 2)
            obj->gravity_value_1 = 0;

        obj->gravity_value_2++;
        if (obj->gravity_value_2 > 3)
            obj->gravity_value_2 = 0;
    }

    pesanteur = DO_PESANTEUR(obj);
    if (obj->type == TYPE_LIDOLPINK)
    {
        if (obj->speed_y >= 0 && obj->sub_etat == 0)
            set_sub_etat(obj, 1);

        if (!gerbe && obj->speed_y > 2 && obj->sub_etat == 1)
        {
            set_sub_etat(obj, 2);
            obj->speed_x = 0;
        }
    }
    else if (obj->type == TYPE_STONEDOG || obj->type == TYPE_STONEDOG2)
    {
        stoneDogBounces(obj);
        if (obj->main_etat == 2)
        {
            spd_y = obj->speed_y;
            if (spd_y > 0)
            {
                if (obj->sub_etat != 2)
                    set_sub_etat(obj, 2);
            }
            else if (spd_y < 0 && (block_flags[(u8) calc_typ_trav(obj, 1)] & 1))
                obj->speed_y = 0;
        }
    }
    else if (obj->type == TYPE_NOTE1)
    {
        if (--obj->iframes_timer == 0)
            DO_EXPLOSE_NOTE1(obj);
    }

    if ((flags[obj->type] & flags3_4_stop_moving_up_when_hit_block) && obj->speed_y < 0)
    {
        if (obj->flags.follow_enabled)
        {
            GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
            spr_x = obj->x + obj->offset_bx;
            spr_y = spr_y + obj->offset_hy;
        }
        else
        {
            spr_x = obj->x + obj->offset_bx;
            spr_y = obj->y + obj->offset_hy;
        }

        if (block_flags[BTYP(spr_x >> 4, (spr_y + obj->speed_y) >> 4)] & 0x10)
            obj->speed_y = 0;
    }
    if ((flags[obj->type] & flags2_1_check_tile_type) && (block_flags[obj->btypes[0]] & 2))
    {
        switch (obj->type)
        {
            case TYPE_TROMPETTE:
                trompetteAtter(obj);
                break;
            case TYPE_HYBRIDE_MOSAMS:
                calc_obj_dir(obj);
                recale_position(obj);
                skipToLabel(obj, 3, true);
                if (ray.main_etat != 2)
                {
                    allocateLandingSmoke(obj);
                    set_main_and_sub_etat(&ray, 0, 0);
                    button_released = 1;
                    ray_jump();
                    ray.speed_y = -8;
                }
                screen_trembling = 1;
                break;
            case TYPE_BLACKTOON1:
                switch (obj->follow_sprite)
                {
                    case 3:
                    case 5:
                    case 6:
                        break;
                    case 7:
                        obj->config = 2;
                        calc_obj_dir(obj);
                    case 1:
                    case 4:
                        set_main_and_sub_etat(obj, 1, 0);
                        if (obj->flags.flip_x)
                            skipToLabel(obj, 3, true);
                        else
                            skipToLabel(obj, 2, true);

                        recale_position(obj);
                        obj->speed_y = 0;
                        break;
                    case 2:
                        set_main_and_sub_etat(obj, 0, 0);
                        recale_position(obj);
                        obj->speed_y = 0;
                        break;
                }
                break;
            case TYPE_WASHING_MACHINE:
                if (obj->speed_y > 0)
                {
                    recale_position(obj);
                    obj->speed_y = -obj->speed_y;
                }
                break;
            case TYPE_DROP:
                Drop_Atter(obj);
                break;
            case TYPE_MITE:
            case TYPE_MITE2:
                MiteAtter(obj);
                break;
            case TYPE_STONEWOMAN:
                if (obj->speed_y >= 0)
                {
                    skipToLabel(obj, 16, true);
                    obj->config = 0;
                }
                break;
            case TYPE_TNT_BOMB:
                PlaySnd(187, obj->id);
                BombExplosion(obj);
                break;
            case TYPE_BADGUY1:
            case TYPE_BADGUY2:
            case TYPE_BADGUY3:
                BadGuyAtter(obj);
                break;
            case TYPE_STONEBOMB2:
            case TYPE_STONEBOMB3:
                if (obj->speed_y != 0)
                    PlaySnd(200, obj->id);
                DO_STONEBOMB_REBOND(obj);
                break;
            case TYPE_STONEBOMB:
                if (obj->speed_y != 0)
                    PlaySnd(200, obj->id);
                DO_THROWN_BOMB_REBOND(obj, pesanteur, 1);
                break;
            case TYPE_STONEDOG:
            case TYPE_STONEDOG2:
                stoneDogAtter(obj);
                break;
            case TYPE_PIRATE_BOMB:
                DO_FRUIT_REBOND(obj, pesanteur, 1);
                break;
            case TYPE_MST_SHAKY_FRUIT:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 6;
                    DO_FRUIT_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_MST_FRUIT1:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 7;
                    DO_FRUIT_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_MST_FRUIT2:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 6;
                    DO_THROWN_BOMB_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_FALLING_OBJ:
            case TYPE_FALLING_OBJ2:
            case TYPE_FALLING_OBJ3:
            case TYPE_FALLING_YING:
            case TYPE_FALLING_YING_OUYE:
            case TYPE_GRAINE:
            case TYPE_BLACKTOON_EYES:
                switch (obj->type)
                {
                    case TYPE_FALLING_OBJ:
                    case TYPE_FALLING_OBJ3:
                    case TYPE_FALLING_OBJ2:
                        PlaySnd(45, obj->id);
                        break;
                    case TYPE_FALLING_YING:
                    case TYPE_FALLING_YING_OUYE:
                        PlaySnd(140, obj->id);
                        break;
                    case TYPE_GRAINE:
                    case TYPE_BLACKTOON_EYES:
                        PlaySnd(0, obj->id);
                        break;
                }
                DO_FRUIT_REBOND(obj, pesanteur, 0);
                break;
            case TYPE_CAGE:
                set_main_and_sub_etat(obj, 0, 11);
                obj->speed_y = 0;
                break;
            case TYPE_LIDOLPINK:
                LidolPinkAtter(obj);
                break;
            case TYPE_PIRATE_GUETTEUR:
            case TYPE_PIRATE_GUETTEUR2:
                if (obj->sub_etat == 2)
                    set_main_and_sub_etat(obj, 0, 7);
                else if (obj->speed_y >= 0)
                    set_main_and_sub_etat(obj, 0, 10);
                recale_position(obj);
                obj->speed_y = 0;
                obj->speed_x = 0;
                break;
            case TYPE_PIRATE_NGAWE:
                recale_position(obj);
                set_main_and_sub_etat(obj, 0, 5);
                obj->speed_y = 0;
                obj->speed_x = 0;
                break;
            case TYPE_SUPERHELICO:
                obj->speed_x = 0;
                obj->speed_y = 0;
                break;
            case TYPE_NOTE0:
            case TYPE_NOTE1:
            case TYPE_NOTE2:
            case TYPE_BONNE_NOTE:
            case TYPE_NOTE3:
                DO_NOTE_REBOND(obj);
                break;
            case TYPE_BBL:
                DO_BBL_REBOND(obj);
                break;
            case TYPE_BIG_CLOWN:
            case TYPE_WAT_CLOWN:
                Clown_Music_Atter(obj);
                break;
            case TYPE_SPIDER:
                Spider_Atter(obj);
                break;
            case TYPE_BB1:
                if (obj->iframes_timer < 10)
                    DO_BBMONT_ATTER(obj);
                break;
            case TYPE_BB12:
                DO_BBMONT2_ATTER(obj);
                break;
            case TYPE_BB13:
                DO_BBMONT3_ATTER(obj);
                break;
            case TYPE_SAXO2:
                DO_SAXO2_ATTER(obj);
                break;
            case TYPE_SAXO:
                DO_SAXO_ATTER(obj);
                break;
            case TYPE_MAMA_PIRATE:
                DO_PMA_ATTER(obj); //TODO
                break;
            case TYPE_COUTEAU:
                DO_COU_ATTER(obj); //TODO
                break;
            case TYPE_BOUT_TOTEM:
                DO_TOTBT_REBOND(obj);
                break;
            case TYPE_SPIDER_PLAFOND:
                if (obj->main_etat == 2 && obj->speed_y > 0)
                {
                    set_main_and_sub_etat(obj, 0, 29);
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    s16 flip_x = ray.x > obj->x;
                    obj->flags.flip_x = flip_x;
                }
                break;
            case TYPE_BALLE1:
            case TYPE_BALLE2:
            case TYPE_BIG_BOING_PLAT:
            case TYPE_BLACK_FIST:
            case TYPE_BLACK_RAY:
            case TYPE_BNOTE:
            case TYPE_BOING_PLAT:
            case TYPE_BULLET:
            case TYPE_CORDEBAS:
            case TYPE_DARK_PHASE2:
            case TYPE_DARK:
            case TYPE_MARACAS_BAS:
            case TYPE_MOSKITO:
            case TYPE_MOSKITO2:
            case TYPE_MST_SCROLL:
            case TYPE_PI_BOUM:
            case TYPE_POI1:
            case TYPE_POI2:
            case TYPE_POI3:
            case TYPE_RING:
            case TYPE_SCORPION:
            case TYPE_SPACE_MAMA:
            case TYPE_SPACE_MAMA2:
            case TYPE_TAMBOUR1:
            case TYPE_TAMBOUR2:
                break;
            default:
                NormalAtter(obj);
                break;
        }
    }
    if (flags[obj->type] & flags1_0x20_move_y)
    {
        MIN_2(obj->speed_y, 96);
        MAX_2(obj->speed_y, -128)
    }
    else
    {
        MIN_2(obj->speed_y, 6);
        MAX_2(obj->speed_y, -8)
    }
}

//57FEC
void test_fall_in_water(obj_t* obj) {
    if (obj->btypes[0] == BTYP_WATER && !(obj->type == TYPE_23_RAYMAN && ray.main_etat == 6)) {
        if (obj->type == TYPE_52_MST_FRUIT2) {
            obj->flags.alive = 0;
        } else {
            if (flags[obj->type] & flags2_2_fall_in_water) {
                if (!obj->flags.flag_0x40) {
                    allocate_splash(obj);
                    obj->speed_y = 0;
                    obj->speed_x = 0;
                }
                if (flags[obj->type] & flags3_2_die_in_water) {
                    obj->is_active = 0;
                    obj->y = ymap + 484;
                }
            }
        }
    }
}

//58084
void MOVE_OBJECT(obj_t* obj) {
    if (obj->speed_x != 0 || obj->speed_y != 0) {
        if ((flags[obj->type] & flags2_0x80_increase_speed_x) && obj->config == 1 && horloge[20] == 0) {
            obj->speed_x = obj->speed_x <= 0 ? obj->speed_x + 1 : obj->speed_x - 1;
            if (obj->speed_x == 0) {
                obj->config = 0;
            }
        }
        bool need_set_flag_0x100;
        if (obj->type == TYPE_94_POING) {
            poing.y_16 += ashl16(obj->speed_y, 4);
            obj->y = poing.y_16 >> 4;
            obj->x += obj->speed_x;
            need_set_flag_0x100 = false;
        } else {
            s32 dx = 0;
            s32 dy = 0;

            if (flags[obj->type] & flags1_0x20_move_y) {
                dy = instantSpeed(obj->speed_y);
            } else {
                dy = obj->speed_y;
            }

            if (flags[obj->type] & flags1_0x10_move_x) {
                dx = instantSpeed(obj->speed_x);
            } else {
                dx = obj->speed_x;
            }

            if (flags[obj->type] & flags2_0x40_uturn_on_block) {
                s32 x1 = obj->x + obj->offset_bx;
                s32 x2 = x1 + dx;
                s32 y = obj->y + obj->offset_by - 8;
                if ((!(block_flags[BTYP(x1 >> 4, y >> 4)] & 0x10) && (block_flags[BTYP(x2 >> 4, y >> 4)] & 0x10)) ||
                        x2 < 0 || (x2 > xmapmax + SCREEN_WIDTH - 16)) {
                    dx = -dx;
                    obj->speed_x = -obj->speed_x;
                }
            }

            obj->x += dx;
            obj->y += dy;
            need_set_flag_0x100 = (dx != 0) || (dy != 0);
        }

        obj->flags.flag_0x100 = need_set_flag_0x100;
        test_fall_in_water(obj);
    } else {
        obj->flags.flag_0x100 = false;
    }
}

//58270
void DO_RAY_IN_ZONE(obj_t* obj) {
    if (OBJ_IN_ZONE(obj)) {
        ObjectsFonctions[obj->type].rayman_in_zone(obj);
    }
}

//5829C
void DoRaymanInZDDDefault(obj_t* obj) {
    //nullsub
}

//582A0
void DO_ONE_OBJECT(obj_t* obj) {
    if (flags[ot] & flags0_2_balle) {
        DO_BALLE(obj);
    }
    if (flags[ot] & flags0_0x20_has_detect_zone) {
        SET_DETECT_ZONE_FLAG(obj);
    }
    if (flags[ot] & flags2_1_check_tile_type) {
        calc_btyp(obj);
    }
    if (flags[ot] & flags1_0x80_read_cmd) {
        GET_OBJ_CMD(obj);
//        if (obj->id == debug_obj_id) {
//            printf("obj %d [%02d]: cmd %d\n", obj->id, obj->cmd_offset, obj->cmd);
//        }
    } else {
        obj->cmd = GO_NOP;
    }
    ObjectsFonctions[ot].command(obj);
    if (obj->main_etat == 2) {
        OBJ_IN_THE_AIR(obj); //TODO
    }
    if (obj->flags.follow_enabled) {
        SET_RAY_DIST(obj);
    } else {
        obj->ray_dist = 10000;
    }
    if (flags[ot] & flags1_0x40_special_platform) {
        DO_SPECIAL_PLATFORM(obj);
    }
    if (flags[ot] & flags0_0x20_has_detect_zone) {
        DO_RAY_IN_ZONE(obj);
    }
}

//58388
void build_active_table(void) {
    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    s32 screen_center_x = xmap + 160;
    s32 screen_center_y = ymap + 100;
    for (s32 i = 0; i < level_obj.nb_objects; ++i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        obj->field_1C = 0;
    }

    for (s32 i = level_obj.nb_objects - 1; i >= 0; --i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        if (obj->field_1C == 0) {
            if ((Abs(obj->x - screen_center_x) < 660 && Abs(obj->y - screen_center_y) < 600) || (flags[obj->type] & flags2_0x10_do_not_check_ray_collision)) {
                SET_ACTIVE_FLAG(obj->x - xmap + 8, obj->y - ymap, obj);
            } else {
                obj->is_active = 0;
            }
        }
    }

    for (s32 i = level_obj.nb_objects - 1; i >= 0; --i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        if (obj->is_active) {
            obj->screen_x = obj->x - xmap + 8;
            obj->screen_y = obj->y - ymap;
            actobj.objects[actobj.num_active_objects++] = obj->id;
            if (actobj.num_active_objects > COUNT(actobj.objects)) {
                FatalError("Error On MAX_ACTOBJ.");
            }
        } else {
            if (flags[obj->type] & flags0_1_always) {
                del_alwobj(obj->id);
            }
        }
    }
    if (actobj.num_active_objects < COUNT(actobj.objects)) {
        actobj.objects[actobj.num_active_objects] = -1;
    }
}

//58644
void Add_One_RAY_lives(void) {
    ++status_bar.lives;
    if (status_bar.lives > 99) {
        status_bar.lives = 99;
    }
    if (status_bar.max_hitp == 4) {
        ray.hit_points = status_bar.max_hitp;
    } else {
        ray.hit_points = 2;
    }
}

//58680
void DO_CLING_ANIMS(void) {
    if (id_Cling_1up != -1) {
        obj_t* obj = level.objects + id_Cling_1up;
        if (obj->timer != 0) {
            --obj->timer;
            obj->anim_frame = 0;
        }
        DO_ANIM(obj);
        if (EOA(obj)) {
            id_Cling_1up = -1;
            if (ray_mode != MODE_3_MORT_DE_RAYMAN && ray_mode != MODE_4_MORT_DE_RAYMAN_ON_MS) {
                Add_One_RAY_lives();
            }
            obj->flags.alive = 0;
        }
    }

    if (id_Cling_Pow != -1) {
        obj_t* obj = level.objects + id_Cling_Pow;
        if (obj->timer != 0) {
            --obj->timer;
            obj->anim_frame = 0;
        }
        DO_ANIM(obj);
        if (EOA(obj)) {
            id_Cling_Pow = -1;
            obj->flags.alive = 0;
        }
    }
}

//58754
void DO_OBJECTS_ANIMS(void) {
    for (s32 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = level.objects + actobj.objects[i];
        if (obj->type != TYPE_204_BLACK_RAY && obj->type != TYPE_205_BLACK_FIST) {
            DO_ANIM(obj);
        }
    }
}

//587C8
void DO_OBJECTS(void) {
    if (id_Cling_1up != -1 || id_Cling_Pow != -1) {
        DO_CLING_ANIMS();
    }
    if (lidol_to_allocate != 0) {
        allocate_toons(lidol_source_obj, 7);
        lidol_to_allocate = 0;
    }
    for (s32 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = level.objects + actobj.objects[i];
        ot = obj->type;
        if (debug_obj_id == obj->id && obj->x <= debug_obj_xmin) {
            BREAKPOINT; // added for debugging
        }
        if ((flags[ot] & flags3_8_switch_off) ||
            (ot == TYPE_161_WIZ && obj->sub_etat == 23) ||
            (ot == TYPE_83_EXPLOSION && obj->sub_etat == 1) ||
            (ot == TYPE_33_DARK2_SORT && obj->sub_etat == 35)
        ) {
            switchOff(obj);
        }
        if (ot != TYPE_161_WIZ && obj->is_active) {
            DO_ONE_OBJECT(obj); //TODO
            if (!(flags[ot] & flags0_0x80_boss) && obj->hit_points == 0 && (get_eta(obj)->flags & 8)) {
                ++obj->hit_points;
            }
        }
    }
}

//58908
void MOVE_OBJECTS(void) {
    for (s32 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = level.objects + actobj.objects[i];
        if (debug_obj_id == obj->id && obj->x <= debug_obj_xmin) {
            BREAKPOINT; // added for debugging
        }
        MOVE_OBJECT(obj);
    }
}

//58964
void RECALE_ALL_OBJECTS(void) {
    /* 30930 80155130 -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;
    s16 i = 0;
    obj_t* cur_obj = &level.objects[actobj.objects[0]];

    while (i < actobj.num_active_objects)
    {
        calc_obj_pos(cur_obj);
        i++;
        cur_obj = &level.objects[actobj.objects[i]];
    }

    if (ray.follow_id != -1)
    {
        cur_obj = &level.objects[ray.follow_id];
        if (cur_obj->is_active)
        {
            GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &x, &y, &w, &h);
            ray.y = y + cur_obj->offset_hy - ray.offset_by;
            calc_obj_pos(&ray);
        }
    }
}

//58AAC
u8 RayCoince(s16 dir) {
    /* 30A64 80155264 -O2 -msoft-float */
    s16 x_pos;
    s16 y_pos;
    s32 pos_to_check;
    s32 map_ind;
    u8 res = false;

    if (ray.flags.flip_x)
        x_pos = ray.x + 70;
    else
        x_pos = ray.x + 58;
    y_pos = ray.y + 32;

    switch (dir)
    {
        case 2:
            pos_to_check = y_pos - (y_pos / 16 * 16);
            if ((s16) pos_to_check < 3)
            {
                map_ind = (x_pos >> 4) + (mp.width * (y_pos >> 4));
                if (block_flags[mp.map[map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
            }
            break;
        case 3:
            pos_to_check = y_pos - (y_pos / 16 * 16);
            if ((s16) pos_to_check > 12)
            {
                map_ind = (x_pos >> 4) + (mp.width * ((y_pos + 16 * 4) >> 4));
                if (block_flags[mp.map[map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
            }
            break;
        case 0:
            pos_to_check = x_pos - (x_pos / 16 * 16);
            if ((s16) pos_to_check < 3)
            {
                map_ind = ((x_pos >> 4) + (mp.width * ((y_pos + 16) >> 4))) - 1;
                if (block_flags[mp.map[map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
            }
            break;
        case 1:
            pos_to_check = x_pos - (x_pos / 16 * 16);
            if ((s16) pos_to_check > 12)
            {
                map_ind = ((x_pos >> 4) + (mp.width * ((y_pos + 16) >> 4))) + 3;
                if (block_flags[mp.map[map_ind].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] >> BLOCK_FULLY_SOLID & 1)
                    res = true;
            }
            break;
    }

    return res;
}

//58DE8
void move_up_ray(void) {
    /* 30E34 80155634 -O2 -msoft-float */
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(2)) {
        ray.speed_y = 0;
    }

    s16 unk_y_1 = ((get_eta(&ray)->flags & 0x40) || RayEvts.tiny) ? 40 : 25;
    s16 unk_y_2 = 20;

    ray.y += ray.speed_y;
    if (ray.y < scroll_start_y - (unk_y_1 + unk_y_2)) {
        ray.y = scroll_start_y - (unk_y_1 + unk_y_2);
        ray.speed_y = 0;
        if (ray.follow_id != -1) {
            RAY_HIT(ray.iframes_timer == -1, NULL);
        }
    }
    calc_obj_pos(&ray);
    v_scroll_speed = 0;
}

//58EC8
void move_down_ray(void) {
    /* 30E34 80155634 -O2 -msoft-float */

    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(3)) {
        ray.speed_y = 0;
    }

    ray.y += ray.speed_y;
    calc_obj_pos(&ray);
    if (ray.main_etat == 2) {
        if ((scroll_end_y - ymap > ray.speed_y) && ray.screen_y >= 100) {
            v_scroll_speed = ray.speed_y;
        } else {
            v_scroll_speed = 0;
        }
    }
}

//58F70
void recale_ray_pos(void) {
    /* 3103C 8015583C -O2 -msoft-float */
    //NOTE: is this function really called RecaleRayPosInJumelle in the PS1 version?? maybe not right?
    if (ray.main_etat == 3 && ray.sub_etat == 23) {
        return;
    }
    if (scroll_y == -1) {
        s16 unk_y_1 = ((ray_zdc_h >> 1) - ray.offset_by) + 114; // 136 in the PS1 version
        if ((ray.main_etat == 2 && !(ray.sub_etat == 15 && ray_Suphelico_bis)) || screen_trembling) {
            if ((ymap != scroll_end_y && (unk_y_1 < ray.screen_y - ray.speed_y) && ray.speed_y > 0) ||
                (ymap != scroll_start_y && (-ray.offset_hy - 16 >= ray.screen_y) && ray.speed_y < 0))
            {
                if (ray.speed_y <= 16)
                    v_scroll_speed = ray.speed_y;
                else
                    v_scroll_speed = 0;
            }
        } else {
            if (v_scroll_speed != 255 || decalage_en_cours != 0)
            {
                s16 unk_y_2 = ray.screen_y - unk_y_1;
                s32 v_scr_temp = v_scroll_speed = ashr16(unk_y_2, 2);
                s32 spd_y_abs_1 = Abs(ray.speed_y);
                if (Abs(v_scr_temp) >= spd_y_abs_1) {
                    MAX_2(spd_y_abs_1, 3);
                    s16 spd_y_abs_2 = spd_y_abs_1;
                    if (v_scr_temp > 0) {
                        MIN_2(v_scroll_speed, (s16) spd_y_abs_1);
                    } else if (v_scr_temp < 0) {
                        MAX_2(v_scr_temp, -spd_y_abs_2);
                        v_scroll_speed = v_scr_temp;
                    } else {
                        if (unk_y_2 > 0)
                            v_scroll_speed = 1;
                        else if (unk_y_2 < 0)
                            v_scroll_speed = -1;
                    }
                }
            } else if (ray.main_etat != 1) {
                if (ray.screen_y < unk_y_1 + 48) {
                    v_scroll_speed = -4;
                } else {
                    v_scroll_speed = 0;
                }

            }
        }
    }

    if (scroll_x == -1) {
        s16 unk_x_1 = (112 - ray.offset_bx) - special_ray_mov_win_x_left;
        s16 unk_x_2 = special_ray_mov_win_x_right - (ray.offset_bx - 208);
        if (decalage_en_cours > 0 || ray.speed_x > 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - unk_x_1, 2);
            if (unk_x_3 >= dhspeed) {
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                }
            } else {
                dhspeed--;
            }
        } else if (decalage_en_cours < 0 || ray.speed_x < 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - unk_x_2, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else {
            if (ray.flags.flip_x) {
                s16 unk_x_3 = ashr16(ray.screen_x - unk_x_1, 2);
                if (unk_x_3 >= dhspeed) {
                    if (unk_x_3 > dhspeed) {
                        dhspeed++;
                    }
                } else {
                    dhspeed--;
                }
            } else {
                s16 unk_x_3 = ashr16(ray.screen_x - unk_x_2, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            }
        }

        if (dans_la_map_monde) {
            if (Abs(dhspeed) > 4) {
                dhspeed = dhspeed > 0 ? 4 : -4;
            }
        }

        h_scroll_speed += ashr16(dhspeed, 2);
        if ((unk_x_2 > ray.screen_x && ray.speed_x < 0) || (unk_x_1 < ray.screen_x && ray.speed_x > 0)) {
            h_scroll_speed += ray.speed_x;
        }
    }

    if (scroll_y == -1 && ray.follow_id != -1)
    {
        obj_t* other_obj = &level.objects[ray.follow_id];
        if (flags[other_obj->type] & flags1_0x20_move_y) {
            v_scroll_speed += instantSpeed(other_obj->speed_y);
        } else {
            v_scroll_speed += other_obj->speed_y;
        }
        v_scroll_speed += other_obj->follow_y;
    }
}

//59418
void RAY_TO_THE_RIGHT(void) {
    /* 31690 80155E90 -O2 -msoft-float */
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(1)) {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    ray.x += ray.speed_x;
    if (RIGHT_MAP_BORDER < ray.x) {
        ray.x = RIGHT_MAP_BORDER;
        ray.speed_x = 0;
        decalage_en_cours = 0;
        if (ray.main_etat != 2) {
            ray.speed_y = 0;
        }
    }
    calc_obj_pos(&ray);
}

//5949C
void RAY_TO_THE_LEFT(void) {
    /* 3176C 80155F6C -O2 -msoft-float */
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(0)) {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    ray.x += ray.speed_x;
    if (scroll_x == -1 && ray.x < LEFT_MAP_BORDER) {
        ray.x = LEFT_MAP_BORDER;
        ray.speed_x = 0;
        decalage_en_cours = 0;
        if (ray.main_etat != 2) {
            ray.speed_y = 0;
        }
    }

    calc_obj_pos(&ray); //NOTE: missing in all versions? A bug, or not needed?
}

//59518
void DO_FIXE(void) {
    if (status_bar.max_hitp == 2) {
        status_bar.hp_sprites[1] = ray.hit_points;
        status_bar.hp_sprites[0] = 0;
    } else {
        if (ray.hit_points > 2) {
            status_bar.hp_sprites[1] = 2;
            status_bar.hp_sprites[0] = ray.hit_points - 2;
        } else {
            status_bar.hp_sprites[1] = ray.hit_points;
            status_bar.hp_sprites[0] = 0;
        }
    }
    if (status_bar.lives == -1) {
        status_bar.lives_digits[0] = 0;
        status_bar.lives_digits[1] = 0;
    } else {
        status_bar.lives_digits[0] = status_bar.lives / 10;
        status_bar.lives_digits[1] = status_bar.lives % 10;
    }
    status_bar.num_wiz = MIN(status_bar.num_wiz, 99);
    status_bar.wiz_digits[0] = status_bar.num_wiz / 10;
    status_bar.wiz_digits[1] = status_bar.num_wiz % 10;
}

//595E8
void deactivate_ship_links(void) {
    /* 31D3C 8015653C -O2 -msoft-float */
    obj_t *bateau_obj;
    u8 linked;

    if (bateau_obj_id != -1) {
        bateau_obj = &level.objects[bateau_obj_id];
        if (bateau_obj->link_has_gendoor) {
            linked = link_init[bateau_obj->id];
            while (linked != bateau_obj->id) {
                level.objects[linked].flags.alive = 0;
                linked = link_init[linked];
            }
        }
    }
}

//5965C
u8 linkListHoldsAGendoor(obj_t* obj) {
    /* 31DEC 801565EC -O2 -msoft-float */
    s32 res = false;
    u8 linked = link_init[obj->id];

    while (linked != obj->id)
    {
        if (level.objects[linked].type == TYPE_GENERATING_DOOR)
        {
            res = true;
            break;
        }
        linked = link_init[linked];
    }
    return res;
}

//596C0
void correct_gendoor_link(u8 a1) {
    for (s16 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (obj->type == TYPE_164_GENERATING_DOOR) {
            s16 link = link_init[i];
            s16 last_link = i;

            // NOTE: I'm not too sure about this part, might need checking
            while (link != i && link != last_link) {
                if (obj->flags.alive && obj->type == TYPE_164_GENERATING_DOOR) {
                    level.objects[link].flags.alive = 0;
                }
                last_link = link;
                link = link_init[link];
            }

            if (link == i) {
                link_init[last_link] = link_init[i];
                if (last_link == link_init[i]) {
                    level.objects[last_link].link_has_gendoor = 0;
                }
                obj->link = link_init[i];
                link_init[i] = i;
                obj->link_has_gendoor = 0;
            }

            if (a1 && !obj->flags.alive) {
                s16 init_link = obj->link;
                s16 linked_obj_id;
                do {
                    linked_obj_id = link_init[init_link];
                    suppressFromLinkList(level.objects + linked_obj_id);
                } while (init_link != linked_obj_id);
            }

        }
    }
    detect_and_remove_invalid_link_cycles(); // NOTE: added as a precaution. TODO: remove if no longer needed?
}

//597FC
void suppressFromLinkList(obj_t* obj) {
    s16 id = obj->id;
    s16 linked_id = link_init[id];
    if (id != linked_id) {
        do {
            id = linked_id;
            linked_id = link_init[id];
        } while (linked_id != obj->id);
    }
    link_init[id] = link_init[obj->id];
    if (link_init[id] == id) {
        level.objects[link_init[id]].link_has_gendoor = 0;
    }
    link_init[obj->id] = obj->id;
    obj->link_has_gendoor = 0;
}

//59888
void correct_link(void) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (link_init[i] == i) {
            obj->link_has_gendoor = 0;
        } else {
            if ((flags[obj->type] & flags3_0x40_no_link) ||
                    ((flags[obj->type] & flags3_0x20_link_requires_gendoor) && !linkListHoldsAGendoor(obj))) {
                suppressFromLinkList(obj);
            } else {
                obj->link_has_gendoor = 1;
            }
        }
    }
}

//59900
void INIT_RAY_BEGIN(void) {
    RayEvts.super_helico = 0;
    RayEvts.magicseed = 0;
    RayEvts.tiny = 0;
    status_bar.max_hitp = 2;
    status_bar.num_wiz = 0;
    fin_continue = 0;
    ray.flags.hurtbyfist = false;
}

//59948
void INIT_RAY(u8 new_lvl) {
    gele = 0;
    compteur_attente = 0;
    ray_mode = (ray.main_etat == 6) ? MODE_2_RAY_ON_MS : MODE_1_RAYMAN;
    if (RayEvts.tiny) {
        rms.hit_points = ray.hit_points;
        ray = rms;
    }
    xmap = 0;
    ymap = 0;
    ray.type = TYPE_23_RAYMAN;
    ray.screen_x = 0;
    ray.screen_y = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;
    ray.x = 100;
    ray.offset_hy = 20;
    ray.y = 10;
    ray.anim_index = 0;
    ray.anim_frame = 0;
    ray.main_etat = 2;
    ray.sub_etat = 2;
    ray.config = 0;
    ray.timer = 0;
    ray.offset_bx = 80;
    ray.offset_by = 78;
    ray.follow_id = -1;
    ray.link = -1;
    ray.iframes_timer = -1;
    ray.is_active = 1;
    ray.flags.alive = true;
    ray.flags.flip_x = true;
    ray.flags.flag_0x40 = false;
    ray_stack_is_full = 0;
    no_ray_landing_smoke = 0;
    in_air_because_hit = 0;
    remoteRayXToReach = -32000;
    ray_last_ground_btyp = 1;
    ray_pos_in_stack = 0;
    ray_se_noie = 0;
    ray_in_fee_zone = 0;
    fin_dark = 0;
    for (s32 i = 0; i < 10; ++i) {
        pos_stack[i] = ray.x;
    }
    poing.is_active = 0;
    poing.is_charging = 0;
    if (new_lvl) {
        RayEvts.super_helico = 0;
    }
    if (!(num_world == world_1_jungle && num_level == 9) && !ModeDemo && !record.is_recording) {
        RayEvts.magicseed = 0;
    }
    if (RayEvts.squashed) {
        RayEvts.tiny = RayEvts.squashed;
        Ray_RayEcrase(); //TODO
    }
    RayEvts.tiny = 0;
    RayEvts.force_run = 0;
    if (save1.rayevts_reverse && save1.save_obj_id != -1) {
        RayEvts.reverse = 0;
        if (num_world == world_6_cake && num_level == 3) {
            RAY_REVERSE_COMMANDS(); //TODO
        }
    } else {
        //TODO: figure out what is happening here (seems to be wrong right now)
//        RayEvts.reverse = 1;
//        RAY_REVERSE_COMMANDS(); //TODO
    }

    if (level.objects && level.nb_objects > 0) {

        obj_t* obj = level.objects;
        bool found = false;
        for (s32 i = 0; i < level.nb_objects; ++i) {
            if (obj->type == TYPE_99_RAY_POS) {
                found = true;
                break;
            }
            ++obj;
        }
        if (found) {
            if (!fin_continue) {
                ray.flags.alive = false;
                ray.is_active = 0;
            }
            xmap = obj->x + obj->offset_bx - 160;
            ymap = obj->y - 10;
            if (GameModeVideo != MODE_NORMAL) {
                if (xmap >= xmapmax) {
                    xmap = xmapmax;
                }
                if (ymap >= ymapmax) {
                    ymap = ymapmax;
                }
                if (xmap < 0) {
                    xmap = 0;
                }
                if (ymap < 0) {
                    ymap = 0;
                }
            } else {
                set_xymap();
            }
            ray.x = obj->x + obj->offset_bx - ray.offset_bx;
            ray.y = obj->y + obj->offset_by - ray.offset_by;
        }
    }

    if (RaymanDansUneMapDuJeu) {
        calc_btyp(&ray);
    } else {
        for (s32 i = 0; i < 5; ++i) {
            ray.btypes[i] = 0;
        }
    }
}

//59CF4
u8 is_icy_pente(u8 btyp) {
    u8 result = (block_flags[btyp] & 0x40) != 0 && (block_flags[btyp] & 8) != 0;
    return result;
}

//59D18
void STOPPE_RAY_EN_XY(void) {
    if (ray.main_etat != 7) {
        s16 x = ray.offset_bx + ray.x;
        s16 y = ray.offset_by + ray.y;
        if (block_flags[ray.btypes[0]] & 0x40) {
            y -= 8;
        }

        x += ray.speed_x * 2;
        u8 btyp_1 = BTYP(x >> 4, (y - 8) >> 4);
        u8 btyp_2 = BTYP(x >> 4, (y - 24) >> 4);
        u8 btyp_3 = BTYP(x >> 4, (y - 40) >> 4);
        u8 btyp_4 = BTYP(x >> 4, (y - 56) >> 4);

        s16 stop = false;
        if (block_flags[btyp_1] & 0x10)
            stop = !is_icy_pente(btyp_2);
        if (!(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40) && !RayEvts.tiny) {
            if (!stop && (((block_flags[btyp_2] & 0x10) && !is_icy_pente(btyp_3)) ||
                     ((block_flags[btyp_3] & 0x10) && !is_icy_pente(btyp_4)))
            ) {
                stop = true;
            }
        }

        if (stop)
        {
            ray.speed_x = 0;
            decalage_en_cours = 0;
            ray.nb_cmd = 0;
            Reset_air_speed(true);
            Reset_air_speed(false);
        }
    }


}

//59E9C
void DO_PLACE_RAY(void) {
    h_scroll_speed = 0;
    v_scroll_speed = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;

    if (rightjoy()) {
        ray.speed_x += RAY_MODE_SPEED;
    } else if (leftjoy()) {
        ray.speed_x -= RAY_MODE_SPEED;
    }
    if (downjoy()) {
        ray.speed_y += RAY_MODE_SPEED>>1;
    } else if (upjoy()) {
        ray.speed_y -= RAY_MODE_SPEED>>1;
    }

    if (ray.speed_y < 0) {
        move_up_ray();
    } else if (ray.speed_y > 0) {
        move_down_ray();
    }
    if (ray.speed_x > 0) {
        RAY_TO_THE_RIGHT();
    } else if (ray.speed_x < 0) {
        RAY_TO_THE_LEFT();
    }
    calc_obj_pos(&ray);
    h_scroll_speed = ray.speed_x;
    v_scroll_speed = ray.speed_y;
    SHOW_COLLISIONS_ZONES();
}

//59FB8
void DO_AUTO_SCROLL(void) {
    /* 340A4 801588A4 -O2 -msoft-float */
    s16 i;
    s32 eau_id;
    obj_t* scr_obj;
    s16 scr_x;
    s16 ray_x = ray.x;
    s16 ray_y = ray.y;

    scroll_x = 0;
    scroll_y = -1;
    for (i = 0; i < NumScrollObj; i++) {
        eau_id = eau_obj_id;
        scr_obj = &level.objects[scroll_obj_id[i]];
        if (scr_obj->type != TYPE_64_SCROLL)
            break;

        switch (scr_obj->hit_points) {
            case 0:
                if (scr_obj->x <= ray_x)
                {
                    scroll_x++;
                    scr_obj->x = scr_obj->init_x - 200;
                }
                break;
            case 1:
                if (scr_obj->x <= ray_x)
                {
                    scroll_x--;
                    scr_obj->x = scr_obj->init_x - 200;
                }
                break;
            case 2:
                if (scr_obj->y <= ray_y)
                    scroll_y = -scroll_y;
                else
                    scr_obj->y = 10000;
                break;
            case 3:
                if (eau_id == -1 || (level.objects[eau_id].hit_points == 0 && level.objects[eau_id].param == 0)) {
                    if (scr_obj->y <= ray_y)
                    {
                        if (Abs(scroll_y) == 1)
                            scroll_y = 2;
                        else
                            scroll_y = -scroll_y;
                    }
                    else
                        scr_obj->y = 10000;

                    if (eau_id != -1 && scroll_y != 2)
                        level.objects[eau_id].iframes_timer = 2;
                }
                break;
        }
    }

    if (scroll_x <= 0)
        scroll_x--;

    scr_x = scroll_x;
    if (scr_x > 0)
        h_scroll_speed = scr_x;
    else if (scr_x < -1)
        h_scroll_speed = scr_x + 1;

    if (scroll_y == 1)
        v_scroll_speed = -1;
    else
    {
        if (scroll_y == 2)
        {
            if (horloge[2] != 0)
                v_scroll_speed = -1;
            else
                v_scroll_speed = 0;
        }
        if (scroll_y == -2)
            scroll_y = -1;
    }
}

//5A1E0
void INIT_MOTEUR(u8 new_lvl) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        level.objects[i].field_1C = 0;
    }

    memset(pix_gerbe, 0, sizeof(pix_gerbe));

    if (!fin_continue) {
        init_flocons();
    }
    if (num_world == world_1_jungle && num_level == 9) {
        set_SNSEQ_list(11);
    }
    scrollLocked = 0;
    loop_time = 0;
    dans_la_map_monde = 0;
    sko_rayon_on = 0;
    rubis_list_calculated = 0;
    Vignet_To_Display = 0;
    sko_last_action = 0;
    sko_enfonce_enable = 0;
    joe_exp_probleme = finBosslevel.helped_joe_2;
    pixels_enfonce = 0;
    if (num_world == world_5_cave && num_level == 10) {
        mp.height = 32;
        scroll_end_y = 312;
        ymapmax = 312;
    }
    in_pause = 0;
    map_time = 0;
    left_time = -2;
    INIT_HORLOGES();
    ray.id = -1;
    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    xmap = 0;
    ymap = 0;

    if (GameModeVideo != MODE_NORMAL) {
        ymapmax = 16 * mp.height - 200;
        xmapmax = 16 * mp.width - 305;
        if (xmapmax < 0) {
            xmapmax = 0;
        }
        if (ymapmax < 0) {
            ymapmax = 0;
        }
        scroll_start_x = 0;
        scroll_end_x = xmapmax;
        scroll_start_y = 0;
        scroll_end_y = ymapmax;
    } else {
        set_xymapini();
    }
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    dvspeed = 0;
    dhspeed = 0;
    jump_time = 0;
    gerbe = 0;
    decalage_en_cours = 0;
    ray_wind_force = 0;
    screen_trembling = 0;
    helico_time = -1;
    weather_wind = 0;
    lidol_to_allocate = 0;
    fin_boss = 0;
    boss_mort = 0;
    TextDark2_Affiche = 0;
    if (!fin_du_jeu) {
        dead_time = 64;
    }
    INIT_RAY(new_lvl);
    if (!bonus_map && departlevel && save1.is_just_saved) {
        s16 saved_tings = status_bar.num_wiz;
        restoreGameState(&save1);
        status_bar.num_wiz = saved_tings;
    } else {
        actualize_palette(0); // NOTE: Added to prevent wrong palette being used at start of level (is there a bug elsewhere?)
    }
    if (departlevel) {
        nb_wiz_collected = 0;
    } else {
        save1.is_just_saved = save_save;
        ray.x = ray_X_main_pos_init;
        ray.y = ray_Y_main_pos_init;
        ray.is_active = 1;
        ray.flags.alive = 1;
        current_pal_id = pal_id_at_wizard + 1;
        actualize_palette(pal_id_at_wizard);
        xmap = xmapsave;
        ymap = ymapsave;
    }
    INIT_OBJECTS(new_lvl);
    correct_link();
    deactivate_ship_links();
    special_flags_init();
    if (num_world_choice == world_5_cave && num_level_choice == 4 && finBosslevel.helped_joe_1) {
        RayEvts.firefly = 1;
    } else {
        RayEvts.firefly = 0;
    }
    if (RayEvts.firefly) {
        INIT_LUCIOLE(); //TODO
    }
    DO_OBJECTS(); //TODO
    if (mp.width <= 20 && (SizeScreen == 0 || GameModeVideo == MODE_X)) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    if (type_fndscrX != 0) {
        scroll_end_x = scroll_start_x;
        xmap = scroll_start_x;
    }
}

//5A5B4
void INIT_MOTEUR_BEGIN(void) {
    init_allowed_time();
    init_bonus_perfect();
    init_calcbloc_func();
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hit_points = 2;
    gele = 0;
    departlevel = 1;
    poing.sub_etat = 1;
    nb_continue = 9;
    You_Win = 0;
    set_proj_center(160, 170);
    INIT_RAY_BEGIN();
    scroll_x = -1;
    scroll_y = -1;
    special_ray_mov_win_x_left = 0;
    special_ray_mov_win_x_right = 0;
    fin_de_rayman = 0;
    NumDemo = myRand(5);
    if (DemoRecordWorld[NumDemo] == 5 && DemoRecordMap[NumDemo] == 10) {
        ++NumDemo;
        if (NumDemo == COUNT(DemoRecordWorld)) {
            NumDemo = 0;
        }
    }
    First_Hist = 1;
    First_Menu = 1;
    left_time = -2;
    life_becoz_wiz = 0;
    dontdoit = 0;
    RunTimeDemo = 1800;
}

//5A6D8
void INIT_MOTEUR_WORLD(void) {
    new_world = 0;
    new_level = 1;
}

//5A6F4
void INIT_MOTEUR_LEVEL(s16 a1) {
    Bloc_lim_W1_Aff = Bloc_lim_W1;
    Bloc_lim_W2_Aff = Bloc_lim_W2;
    Bloc_lim_H1_Aff = Bloc_lim_H1;
    Bloc_lim_H2_Aff = Bloc_lim_H2;
    if (!fin_du_jeu) {
        bonus_map = (get_allowed_time() != -2);
        bool need_save_game_state = (!bonus_map && departlevel && !fin_continue);
        if (need_save_game_state) {
            initGameSave();
            MapAvecPluieOuNeige = 0;
        }
        INIT_MOTEUR(a1); //TODO
        if (need_save_game_state) {
            saveGameState(NULL, &save1);
        }
        if (departlevel) {
            nb_wiz_collected = 0;
        } else  {
            departlevel = 1;
            restore_objects_flags();
        }
        correct_gendoor_link(0);
        ray_mode = MODE_1_RAYMAN;
        RAY_MODE_SPEED = 16; // This is Rayman's move speed when the free movement cheat is enabled.
        new_level = 0;
        build_active_table();
    }
}

//5A7DC
void restore_gendoor_link(void) {
    /* 34E30 80159630 -O2 -msoft-float */ //NB: nonmatching
    obj_t* temp_v1;
    obj_t* var_v0_2;
    s16 var_v0_1;
    s32 temp_a0_3;
    s32 var_a0;
    s16 temp_a0_1;
    u16 temp_a0_2;
    s16 var_a1;
    u32 var_v1;
    u8 temp_a1;
    s16 test_1;

    test_1 = level.nb_objects;
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (obj->type == TYPE_164_GENERATING_DOOR)
        {
            if (obj->flags.alive) {
                var_a1 = obj->link;
                do
                {
                    temp_a0_1 = var_a1;
                    var_a1 = link_init[var_a1];
                } while (var_a1 != obj->link);
                link_init[obj->id] = var_a1;
                link_init[temp_a0_1] = obj->id;
                obj->link_has_gendoor = 1;
                level.objects[temp_a0_1].link_has_gendoor = 1;
                continue;
            }
            temp_a0_2 = (u16) obj->id;
            var_a0 = temp_a0_2 << 0x10;
            if ((s16) temp_a0_2 != save1.link_init[temp_a0_2])
            {
                do
                {
                    temp_a0_3 = var_a0 >> 0x10;
                    temp_a1 = save1.link_init[temp_a0_3];
                    link_init[temp_a0_3] = temp_a1;
                    if (temp_a0_3 != temp_a1)
                    {
                        level.objects[temp_a0_3].link_has_gendoor = 1;
                    }
                    else
                    {
                        level.objects[temp_a0_3].link_has_gendoor = 0;
                    }
                    var_a0 = temp_a1 << 0x10;
                } while (obj->id != (s16) temp_a1);
            }
        }
    }
}

//5A8E4
void DONE_MOTEUR_LEVEL(void) {
    /* 34FEC 801597EC -O2 -msoft-float */
    restore_gendoor_link(); //TODO
    if (!bonus_map && departlevel && get_next_bonus_level(num_level) == 0 && !fin_continue) {
        doneGameSave();
    }
    if (rayman_obj_id != -1 && (new_level || new_world)) {
        raytmp = ray;
        ray = rms;
        INIT_RAY(1);
        ray.hit_points = raytmp.hit_points;
        ray.init_hit_points = raytmp.init_hit_points;
        ray.hit_sprite = raytmp.hit_sprite;
        set_main_and_sub_etat(&ray, 0, 0);
        ray_mode = MODE_0_NONE;
    }

    if (first_boss_meet) {
        Change_Wait_Anim();
    }

    if (fee_obj_id != -1) {
        record.is_playing = false;
    }
}

//5A9D8
void INIT_MOTEUR_DEAD(void) {
    if (dead_time == 0) {
        restore_gendoor_link(); //TODO
        INIT_MOTEUR(false);
        correct_gendoor_link(0);
    }
    if (!bonus_map && departlevel) {
        saveGameState(NULL, &save2);
        life_becoz_wiz = 0;
    }
}

//5AA20
void INIT_RAY_ON_MS(void) {
    if (NewMs) {
        if (rayman_obj_id == -1) {
            if (rayOnMsWasThere) {
                INIT_RAY(true);
                NewMs = 0;
            }
        } else {
            rms = ray;
            ray = level.objects[rayman_obj_id];
            INIT_RAY(true);
            ray.hit_points = rms.hit_points;
            ray.init_hit_points = rms.init_hit_points;
            ray.hit_sprite = rms.hit_sprite;
            set_main_and_sub_etat(&ray, 6, 0);
            ray_mode = MODE_2_RAY_ON_MS;
            NewMs = 0;
        }
    }
}

//5AAE8
void DO_MOTEUR(void) {
    if (ModeDemo != 0) {
        bool has_joystick_input = false; // TODO: check input buttons
        if (ModeDemo == 3 || ModeDemo == 4) {
            new_world = 1;
            DO_FADE_OUT();
            ModeDemo = 2;
        }
    }
    PC_do_cheats();
    horloges(1);
    calc_left_time();
    ray_old_main_etat = ray.main_etat;
    ray_old_sub_etat = ray.sub_etat;
    xmap_old = xmap;
    ymap_old = ymap;

    if (ModeVideoActuel == MODE_X) {
        RIGHT_MAP_BORDER = xmap + 320 - ray.offset_bx - 36;
        LEFT_MAP_BORDER = xmap + 20 - ray.offset_bx;
    } else {
        RIGHT_MAP_BORDER = Bloc_lim_W2 + xmap - ray.offset_bx - 36;
        LEFT_MAP_BORDER = Bloc_lim_W1 + xmap + 20 - ray.offset_bx;
    }

    if (ray.param != -1) {
        level.objects[ray.param].display_prio = oldPrio;
    }

    if (RayEvts.firefly) {
        DO_LUCIOLE(); //TODO
    }

    if (num_world == world_3_mountain) {
        ray_wind_force = 0;
    } else {
        ray_wind_force = weather_wind;
    }

    DO_OBJECTS(); //TODO
}

//5AC70
void DO_MOTEUR2(void) {
    DO_WIZ_AFTER_BONUS_MAP(); //TODO
    DO_PERFECT_BONUS_MAP(); //TODO
    DO_OBJECTS_ANIMS();
    if (ray.flags.alive) {
        if (dead_time != 64) {
            ray.follow_id = -1;
        }
        switch(ray_mode) {
            case MODE_1_RAYMAN: {
                DO_RAYMAN();
            } break;
            case MODE_2_RAY_ON_MS: {
                if (ray.main_etat != 6) {
                    set_main_and_sub_etat(&ray, 6, 0);
                }
                DO_RAY_ON_MS(); //TODO
            } break;
            case MODE_3_MORT_DE_RAYMAN:
            case MODE_4_MORT_DE_RAYMAN_ON_MS: {
                DO_MORT_DE_RAY();
            } break;
            case MODE_5_CASSE_BRIQUE: {
                DO_RAY_CASSE_BRIQUE(); //TODO
            } break;
            default: {
                // NOTE: ray_mode is set to negative when the free movement cheat is enabled.
                DO_PLACE_RAY(); //TODO
            } break;
        }
        if (NumScrollObj <= 0 || ray_mode <= MODE_0_NONE) {
            scroll_x = -1;
            scroll_y = -1;
        } else {
            DO_AUTO_SCROLL(); //TODO: fix scroll speed in map with flat stone with ropes
        }
        MOVE_OBJECTS();
        calc_obj_pos(&ray);
        if (scroll_y != -1 && v_scroll_speed == 255) {
            v_scroll_speed = 0;
        }
        if (ray_mode > MODE_0_NONE) {
            recale_ray_pos();
        }
        if (ray_mode == MODE_2_RAY_ON_MS) {
            if (scroll_x != -1)
            {
                scroll_x = ray.speed_x;
                ray.speed_x = h_scroll_speed;
                if (h_scroll_speed > 0)
                    RAY_TO_THE_RIGHT();
                else if (h_scroll_speed < 0)
                    RAY_TO_THE_LEFT();
                ray.speed_x = scroll_x;
            }
            if (ray.sub_etat != 14)
            {
                switch (ray.sub_etat)
                {
                    case 0:
                        if (Abs(ray.speed_x) >= 2 || Abs(h_scroll_speed) >= 2)
                            set_sub_etat(&ray, 6);
                        break;
                    case 1:
                        if (Abs(ray.speed_x) < 2 && Abs(h_scroll_speed) < 2)
                            set_sub_etat(&ray, 7);
                        else
                        {
                            if (Abs(h_scroll_speed) >= 5 && (xmap < xmapmax - 100))
                            {
                                set_sub_etat(&ray, 2);
                                start_cd_gros_rayman(); // change_audio_track_moskito_acc();
                            }
                        }
                        break;
                    case 5:
                        if (Abs(h_scroll_speed) < 5)
                        {
                            set_sub_etat(&ray, 3);
                            start_cd_gros_rayman(); // change_audio_track_moskito_dec();
                        }
                        break;
                }
                if (xmap == xmapmax)
                    scroll_start_x = xmap;
            }
            poing_obj->x += h_scroll_speed;
            poing_obj->y += v_scroll_speed;
        }
        DO_SCROLL(&h_scroll_speed, &v_scroll_speed);
        build_active_table();
        RECALE_ALL_OBJECTS();
        if (ray.follow_id != -1) {
            obj_t* v20 = level.objects + ray.follow_id;
            oldPrio = v20->display_prio;
            if (oldPrio > 3) {
                v20->display_prio = 3;
            }
        }
    } else {
        MOVE_OBJECTS();
        scroll_y = -1;
        build_active_table();
        if (dead_time != 0) {
            // TODO: solve ray.flags.alive set to false at start of game?
            --dead_time;
        }
    }
    DO_FIXE();
    if (MapAvecPluieOuNeige) {
        do_flocons(xmap, ymap, xmap_old, ymap_old);
        DO_SNOW_SEQUENCE();
    }
    do_pix_gerbes();
    if (RayEvts.tiny) {
        set_zoom_mode(0);
        if (RayEvts.squashed && (ray.iframes_timer == -1 || ray.scale & ~0x100)) {
            if (ray.main_etat != 5) {
                if (NbTramesEcrase != 0) {
                    --NbTramesEcrase;
                } else {
                    ray.scale = MAX(0, ray.scale - 5);
                }
            }
        } else {
            ray.scale = 256;
            NbTramesEcrase = 200;
        }
        if (ray.flags.flip_x) {
            set_proj_center(ray.screen_x - 8 + ray.offset_bx, ray.screen_y + ray.offset_by);
        } else {
            set_proj_center(ray.screen_x + ray.offset_bx, ray.screen_y + ray.offset_by);
        }
    }
}

//5B0A4
void RAY_REVERSE_COMMANDS(void) {
    print_once("Not implemented: RAY_REVERSE_COMMANDS"); //stub
}

//5B154
void RAY_DEMIRAY(void) {
    RayEvts.tiny = !RayEvts.tiny;
    if (RayEvts.tiny) {
        rms = ray;
        ray = level.objects[reduced_rayman_id];
        ray.animations = rms.animations;
        PlaySnd(206, -1);
    } else {
        raytmp = rms;
        rms = ray;
        ray = raytmp;
        PlaySnd(207, -1);
    }
    ray.type = TYPE_RAYMAN;
    ray.screen_x = rms.screen_x;
    ray.screen_y = rms.screen_y;
    ray.speed_x = rms.speed_x;
    ray.speed_y = rms.speed_y;
    ray.x = rms.x;
    ray.y = rms.y;
    ray.offset_bx = rms.offset_bx;
    ray.offset_by = rms.offset_by;
    ray.offset_hy = rms.offset_hy;
    ray.anim_index = rms.anim_index;
    ray.anim_frame = rms.anim_frame;
    ray.flags.flip_x = rms.flags.flip_x;
    ray.param = rms.param;
    ray.main_etat = rms.main_etat;
    ray.sub_etat = rms.sub_etat;
    ray.follow_id = rms.follow_id;
    ray.flags.alive = 1;
    ray.is_active = 1;
    ray.link = rms.link;
    ray.iframes_timer = rms.iframes_timer;
    ray.config = rms.config;
    ray.timer = rms.timer;
    ray.hit_points = rms.hit_points;
    ray.init_hit_points = rms.init_hit_points;
    ray.hit_sprite = rms.hit_sprite;

    if (block_flags[calc_typ_trav(&ray, 2)] & 0x10) {
        set_main_and_sub_etat(&ray, 0, 15);
    }
}

//5B378
void Ray_RayEcrase(void) {
    print_once("Not implemented: Ray_RayEcrase"); //stub
}

//5B668
void DO_MOTEUR_GELE(void) {
    if (gele != 3) {
        if (gele == 2) {
            horloges(1);
            TEST_SIGNPOST();
            h_scroll_speed = 0;
            v_scroll_speed = 0;
            DO_ANIM(&ray);
        } else {
            DO_MEDAILLON_TOON_GELE();
        }
        if (MapAvecPluieOuNeige) {
            do_flocons(xmap, ymap, xmap_old, ymap_old);
            DO_SNOW_SEQUENCE();
            ProchainEclair = 1;
            numero_palette_special = 0;
        }
        for (s32 i = 0; i < actobj.num_active_objects; ++i) {
            obj_t* obj = level.objects + actobj.objects[i];
            if ((flags[obj->type] & flags2_8_can_jump) || (obj->type == TYPE_161_WIZ && obj->sub_etat == 23) ||
                    (obj->type == TYPE_83_EXPLOSION && obj->sub_etat == 1) ||
                    (obj->type == TYPE_33_DARK2_SORT && obj->sub_etat != 35)) {
                DO_ANIM(obj);
                switchOff(obj);
            }
        }
    }
    build_active_table();
}
