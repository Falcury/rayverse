
//6BFD0
void allocateRayLandingSmoke(void) {
    //stub
}

//6C004
void recale_ray_on_liane(void) {
    //stub
}

//6C088
void calc_bhand_typ(void) {
    //stub
}

//6C138
void IS_RAY_ON_LIANE(void) {
    //stub
}

//6C224
void rayMayLandOnAnObject(u8* param_1, i16 obj_id) {
    /* 5D518 80181D18 -O2 -msoft-float */
    s16 var_a1_1;

    s16 i;
    u8 cur_type;
    s16 unk_y;


    i16 unk_1 = RayEvts.tiny ? 8 : 4;
    ray.cmd_arg_2 = -1;

    i = 0;
    obj_t* cur_obj = &level.objects[actobj.objects[i]];
    while (i < actobj.num_active_objects) {
        if (cur_obj->flags.follow_enabled && cur_obj->id != obj_id) {
            if (flags[cur_obj->type] & flags1_0x20_move_y)
                unk_y = instantSpeed(cur_obj->speed_y);
            else
                unk_y = cur_obj->speed_y;

            unk_y = abs((s16) (unk_y + (cur_obj->follow_y - ray.speed_y))) + 2;
            MAX_2(unk_y, unk_1);

            cur_type = cur_obj->type;
            if (cur_type == TYPE_PI || cur_type == TYPE_HERSE_HAUT || cur_type == TYPE_HERSE_BAS_NEXT) {
                unk_y += 8;
            }

            if (abs(cur_obj->ray_dist) < unk_y && !(block_flags[(u8) calc_typ_trav(&ray, 2)] & 0x10)) {
                ray.cmd_arg_2 = actobj.objects[i];
                if (left_time == -2 && (map_time % 2 != 0)) {
                    ++map_time;
                }

                ray.cmd_arg_1 = -1;
                ray_last_ground_btyp = 1;
                if (ray.main_etat == 2)
                {
                    switch (cur_obj->type)
                    {
                        case TYPE_TIBETAIN_2:
                            PlaySnd(110, cur_obj->id);
                            break;
                        case TYPE_PLATFORM:
                            if (num_world == 1 || num_world == 4)
                                PlaySnd(19, -1);
                            else
                                PlaySnd(242, -1);
                            break;
                        case TYPE_FALLPLAT:
                        case TYPE_LIFTPLAT:
                            if (num_world == 4 || num_world == 5)
                                PlaySnd(19, -1);
                            else
                                PlaySnd(242, -1);
                            break;
                        case TYPE_MOVE_PLAT:
                        case TYPE_CRUMBLE_PLAT:
                        case TYPE_BOING_PLAT:
                        case TYPE_ONOFF_PLAT:
                        case TYPE_MOVE_START_NUA:
                            if (num_world == 4)
                                PlaySnd(19, -1);
                            else
                                PlaySnd(242, -1);
                            break;
                        case TYPE_TIBETAIN:
                        case TYPE_MARACAS:
                        case TYPE_TAMBOUR1:
                        case TYPE_TAMBOUR2:
                        case TYPE_TIBETAIN_6:
                        case TYPE_HERSE_BAS:
                        case TYPE_HERSE_BAS_NEXT:
                            break;
                        case TYPE_GOMME:
                            PlaySnd(248, -1);
                            break;
                        case TYPE_MARK_AUTOJUMP_PLAT:
                            PlaySnd(52, -1);
                            break;
                        default:
                            PlaySnd(19, -1);
                            break;
                    }
                }

                switch (cur_obj->type)
                {
                    case TYPE_CYMBALE:
                        cur_obj->link = 0;
                        break;
                    case TYPE_FALLING_OBJ:
                    case TYPE_FALLING_OBJ2:
                    case TYPE_FALLING_OBJ3:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 13)
                            set_main_and_sub_etat(cur_obj, 0, 15);
                        break;
                    case TYPE_BAG3:
                        if (cur_obj->sub_etat == 3)
                            skipToLabel(cur_obj, 0, true);
                        break;
                    case TYPE_MARACAS:
                        if (cur_obj->sub_etat == 4)
                        {
                            /*
                            alternative to do{}while(0);
                            possible up to commit fed31ab
                            */
                            if (/*var_a1_1 = */ cur_obj->hit_points == 0)
                                set_sub_etat(cur_obj, 5);
                            else
                                MARACAS_GO(cur_obj); //TODO
                        }
                        break;
                    case TYPE_TAMBOUR1:
                        cur_obj->configuration = 0x10;
                        if (cur_obj->main_etat == 0)
                        {
                            if (cur_obj->sub_etat == 7)
                            {
                                set_main_and_sub_etat(cur_obj, 2, 0);
                                PlaySnd(110, cur_obj->id);
                            }
                            else if (cur_obj->sub_etat == 9)
                            {
                                set_main_and_sub_etat(cur_obj, 2, 2);
                                PlaySnd(112, cur_obj->id);
                            }
                        }
                        break;
                    case TYPE_TAMBOUR2:
                        cur_obj->configuration = 0x10;
                        set_main_and_sub_etat(cur_obj, 2, 1);
                        PlaySnd(111, cur_obj->id);
                        break;
                    case TYPE_CYMBAL2:
                        /* TODO: macroooooooooooooooooo */
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 13)
                            START_2_PARTS_CYMBAL_ACTION(cur_obj); //TODO
                        break;
                    case TYPE_ROULETTE:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 10)
                            skipToLabel(cur_obj, 1, true);
                        break;
                    case TYPE_ROULETTE2:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 13)
                            skipToLabel(cur_obj, 1, true);
                        break;
                    case TYPE_ROULETTE3:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 15)
                            skipToLabel(cur_obj, 1, true);
                        break;
                    case TYPE_MOVE_START_PLAT:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 0)
                            skipToLabel(cur_obj, 0, true);
                        break;
                    case TYPE_MOVE_START_NUA:
                        if (!(cur_obj->main_etat == 0 && cur_obj->sub_etat == 10))
                            skipToLabel(cur_obj, 0, true);
                        break;
                    case TYPE_FALLPLAT:
                    case TYPE_LIFTPLAT:
                        skipToLabel(cur_obj, 0, true);
                        break;
                    case TYPE_UFO_IDC:
                        if (finBosslevel.helped_joe_2)
                            START_UFO(cur_obj); //TODO
                        break;
                    case TYPE_POELLE:
                        if (!(cur_obj->main_etat == 0 && cur_obj->sub_etat == 1)) {
                            set_main_and_sub_etat(cur_obj, 0, 1);
                            cur_obj->flags.follow_enabled = 0;
                            ray_on_poelle = 1;
                            ray.cmd_arg_2 = -1;
                            ray.speed_x = 1;
                            SauveRayEvts = RayEvts;
                            memset(&RayEvts, 0, sizeof(RayEvts));
                            RayEvts.poing = 1;
                            RayEvts.hang = 0;
                            RayEvts.helico = 0;
                            RayEvts.super_helico = 0;
                            RayEvts.handstand_dash = 0;
                            RayEvts.handstand = 0;
                            RayEvts.magicseed = 0;
                            RayEvts.grab = 0;
                            RayEvts.run = 0;
                            RayEvts.tiny = 0;
                            RayEvts.firefly = 0;
                            RayEvts.force_run = 0;
                            RayEvts.reverse = 0;
                            RayEvts.squashed = 0;
                        }
                        break;
                    case TYPE_SWING_PLAT:
                        if (cur_obj->main_etat == 0 && cur_obj->sub_etat == 2)
                            set_sub_etat(cur_obj, 9);
                        break;
                    case TYPE_BOING_PLAT:
                    case TYPE_BIG_BOING_PLAT:
                        cur_obj->cmd_arg_2 = MAX(ray.speed_y, 2);
                        break;
                }

                if (ray.main_etat == 2) {
                    u8 new_me = 1;
                    u8 new_se = 3;
                    if (abs(ray.speed_x) >= ashr16(ray.eta[new_me][new_se].right_speed, 4) && RayEvts.run) {
                        set_main_and_sub_etat(&ray, new_me, new_se);
                    } else {
                        set_main_etat(&ray, 0);
                        set_sub_etat(&ray, 8);
                        if (ray.link > 200) {
                            allocateRayLandingSmoke(); //TODO
                        }
                    }
                }

                s16 foll_x; s16 foll_y; s16 foll_w; s16 foll_h;
                GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h);
                ray.speed_y = cur_obj->offset_hy + foll_y - ray.offset_by - ray.y;
                *param_1 = 0;
                ray.link = -1;
                helico_time = -1;
                break;
            }
        }

        i++;
        cur_obj = &level.objects[actobj.objects[i]];
    }

    if (obj_id != -1 && ray.cmd_arg_2 == -1)
        ray.cmd_arg_2 = obj_id;
    else if (ray.cmd_arg_2 != -1)
        level.objects[ray.cmd_arg_2].ray_dist = 0;
}

//6CA70
void set_air_speed(u8 a1, u8 a2, i16 a3, i32 a4) {
    //stub
}

//6CAF8
void Reset_air_speed(u8 a1) {
    //stub
}

//6CCE0
void determineRayAirInertia(void) {
    //stub
}

//6CD40
void ray_jump(void) {
    //stub
}

//6D020
void ray_inertia_speed(u8 a1, u8 a2, i16 prev_speed_x, i16 a4) {
    /* 5E4BC 80182CBC -O2 -msoft-float */
    s16 unk_1;
    s16 unk_2;
    s16 unk_3;
    s16 unk_4;
    s16 unk_5;
    s16 unk_6;

    if (a1 == 0)
    {
        decalage_en_cours = prev_speed_x;
        unk_1 = 0;
    }
    else
    {
        unk_2 = ashr32(prev_speed_x * a1, 8);
        unk_1 = ashr16(a1, 3);
        unk_1 = unk_1 != 0 ? unk_1 : 1;
        unk_3 = ashl16(a2, 2);
        unk_4 = ashl16(a4, 3) + (unk_2 + unk_3);

        switch ((s16) (num_world - 1))
        {
            case 0:
            case 1:
            case 3:
            case 5:
                unk_5 = 6;
                break;
            case 2:
                unk_5 = 3;
                break;
        }

        /* ??? duplication ??? */
        if (unk_4 > 0)
        {
            unk_6 =
                    (unk_2 > 0 ? prev_speed_x : 0) +
                    (unk_3 > 0 ? (ashl16(a2 + unk_5, 8) / unk_1) : 0) +
                    ashl16(a4, 8);

            MAX_3(unk_6, decalage_en_cours);
        }
        else if (unk_4 < 0)
        {
            unk_6 =
                    (unk_2 < 0 ? prev_speed_x : 0) +
                    (unk_3 < 0 ? (ashl16(a2 - unk_5, 8)) / unk_1 : 0) +
                    ashl16(a4, 8);

            MIN_3(unk_6, decalage_en_cours);
        }
        else
            unk_6 = decalage_en_cours;

        if (unk_4 != 0)
        {
            if (unk_6 > 0)
            {
                if (decalage_en_cours < unk_6)
                    decalage_en_cours += unk_4;
                if (decalage_en_cours > unk_6)
                    decalage_en_cours = unk_6;
            }
            else
            {
                if (decalage_en_cours > unk_6)
                    decalage_en_cours += unk_4;
                if (decalage_en_cours < unk_6)
                    decalage_en_cours = unk_6;
            }
        }
    }

    if (decalage_en_cours != 0)
    {
        ray.speed_x = instantSpeed(ashr16(decalage_en_cours, 4));
        if (block_flags[calc_typ_travd(&ray, false)] & 0x10 && ray.main_etat != 2)
        {
            ray.speed_x = 0;
            decalage_en_cours = 0;
            ray.nb_cmd = 0;
        }
    }
    else
        ray.speed_x = 0;

    if ((ray.main_etat == 0 || ray.main_etat == 1 || ray.main_etat == 3) && ray.cmd_arg_2 == -1)
        CALC_MOV_ON_BLOC(&ray);

    if (ray.main_etat == 2 && ray.sub_etat == 15)
        unk_1 += 2;

    if (decalage_en_cours >= unk_1)
        decalage_en_cours -= unk_1;
    else if (decalage_en_cours <= -unk_1)
        decalage_en_cours += unk_1;
    else
        decalage_en_cours = 0;
}

//6D2E8
void RAY_SWIP(void) {
    //stub
}

//6D724
void RAY_STOP(void) {
    //stub
}

//6D7D4
void RAY_HELICO(void) {
    //stub
}

//6DB1C
void Make_Ray_Hang(i16 a1, i16 a2) {
    //stub
}

//6DC24
bool AIR(i32 a1) {
    return false; //stub
}

//6DC4C
bool MUR(i32 a1) {
    return false; //stub
}

//6DC78
void CAN_RAY_HANG_BLOC(void) {
    //stub
}

//6DF28
void RAY_TOMBE(void) {
    //stub
}

//6E030
void RAY_RESPOND_TO_DOWN(void) {
    //stub
}

//6E370
void RAY_RESPOND_TO_UP(void) {
    //stub
}

//6E548
void RAY_RESPOND_TO_DIR(i16 dir) {
    //stub
}

//6EB40
void RAY_RESPOND_TO_NOTHING(void) {
    /* 60A58 80185258 -O2 -msoft-float */
    eta_t *sel_eta_1;
    eta_t *sel_eta_2;
    s32 unk_1;
    anim_t *sel_anim;

    switch (ray.main_etat)
    {
        case 1:
            if (
                    !(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40) && // TODO: check: this part not in the PC version?
                    !(ray.sub_etat == 4 || ray.sub_etat == 5) && RayEvts.force_run == 0
                    )
            {
                set_main_etat(&ray, 0);
                if (ray.sub_etat == 8 || ray.sub_etat == 10)
                    set_sub_etat(&ray, 47);
                else if (ray.sub_etat == 9 || ray.sub_etat == 11)
                    set_sub_etat(&ray, 48);
                else if (ray.sub_etat == 3 || ray.sub_etat == 7)
                    set_sub_etat(&ray, 36);
                else
                    set_sub_etat(&ray, 0);
            }
            RAY_SWIP();
            break;
        case 0:
            ray.speed_x = 0;
            if (ray.cmd_arg_1 != -1 && ray.sub_etat == 0)
            {
                if (ray.cmd_arg_1 == 1)
                    set_sub_etat(&ray, 49);
                else
                    set_sub_etat(&ray, 3);
            }
            if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
            {
                if (!(block_flags[(u8) calc_typ_trav(&ray, 2)] & 0x10))
                {
                    if (!(ray.main_etat == 0 && ray.sub_etat == 60))
                        set_main_and_sub_etat(&ray, 0, 60);
                }
                else if (!(ray.main_etat == 0 && (ray.sub_etat == 15 || ray.sub_etat == 61)))
                {
                    sel_anim = &ray.animations[ray.anim_index];
                    set_main_and_sub_etat(&ray, 0, 15);
                    ray.anim_frame = sel_anim->frame_count - 1;
                }
            }
            else
            {
                if (ray.sub_etat == 37 && abs(decalage_en_cours) <= 128)
                    set_sub_etat(&ray, 38);
                else if (ray.sub_etat == 20 && EOA(&ray))
                {
                    if (block_flags[(u8) calc_typ_trav(&ray, 2)] & 0x10)
                    {
                        sel_eta_1 = &ray.eta[ray.main_etat][ray.sub_etat];
                        unk_1 = ((sel_eta_1->flags >> 4 ^ 1) & 1) * 0x10;
                        sel_eta_1->flags = (sel_eta_1->flags & 0xEF) | unk_1;
                        freezeAnim(&ray, 1);
                        sel_eta_2 = &ray.eta[ray.main_etat][ray.sub_etat];
                        unk_1 = ((sel_eta_2->flags >> 4 ^ 1) & 1) * 0x10;
                        sel_eta_2->flags = (sel_eta_2->flags & 0xEF) | unk_1;
                    }
                }
                else if (ray.sub_etat == 59 || ray.sub_etat == 62 || ray.sub_etat == 63)
                {
                    if (EOA(&ray))
                    {
                        compteur_attente = 0;
                        set_sub_etat(&ray, 0);
                    }
                }
                else if (
                        (ray.sub_etat == 0 || ray.sub_etat == 1 || ray.sub_etat == 2) &&
                        ++compteur_attente >= 500
                        ) {
                    set_sub_etat(&ray, 59);
                    //TODO: add animation_attente
                }
            }
            RAY_SWIP();
            break;
        case 4:
            if (ray.sub_etat == 2 || ray.sub_etat == 3)
            {
                set_sub_etat(&ray, 0);
                ray.speed_y = 0;
            }
            break;
        case 2:
            compteur_attente = 0;
            animation_attente = 0;
            if (ray.sub_etat != 8)
                ray.speed_x = 0;
            if (ray.sub_etat != 15 && ray.nb_cmd == 0)
                ray_inertia_speed(0, 0, 0, ray_wind_force);
            else
                ray_inertia_speed(8, 0, 0, ray_wind_force);
            break;
        case 6:
            decalage_en_cours = 0;
            joy_done++;
            ray.flags.flip_x = 1;
            if (ray.gravity_value_1 == 0)
            {
                if (ray.speed_x > 0)
                    ray.speed_x--;
                else if (ray.speed_x < 0)
                    ray.speed_x++;

                if (ray.speed_y > 0)
                    ray.speed_y--;
                else if (ray.speed_y < 0)
                    ray.speed_y++;
            }
            break;
    }
}

//6F0B0
void RAY_RESPOND_TO_BUTTON3(void) {
    //stub
}

//6F1C8
void RAY_RESPOND_TO_FIRE0(void) {
    //stub
}

//6F208
void RAY_RESPOND_TO_FIRE1(void) {
    //stub
}

//6F24C
void RAY_BALANCE_ANIM(i16 a1) {
    //stub
}

//6F29C
void SET_RAY_BALANCE(void) {
    //stub
}

//6F328
void RAY_GOING_BALANCE(obj_t* obj) {
    //stub
}

//6F500
void RAY_BALANCE(void) {
    //stub
}

//6F7D0
void RAY_FIN_BALANCE(void) {
    //stub
}

//6F870
void RayTestBlocSH(void) {
    /* 621FC 801869FC -O2 -msoft-float */
    s16 x_1;
    s16 y_1;
    s16 temp_v1_2;
    s16 var_a1;
    s32 var_v0;
    s32 temp_v1;
    s32 var_a0;
    u32 var_v0_2;

    x_1 = ray.x + ray.offset_bx;
    var_a1 = x_1;
    y_1 = ray.y + ray.offset_hy;
    var_v0 = x_1;
    if (x_1 < 0)
        var_v0 = x_1 + 15;
    temp_v1 = x_1 - (var_v0 >> 4 << 4);
    if (decalage_en_cours > 0 || (decalage_en_cours == 0 && ray.flags.flip_x)) {
        var_a0 = 16;
        if ((u8) temp_v1 >= 14)
            var_a1 = x_1 + 16;
    } else {
        var_a0 = -16;
        if ((u8) temp_v1 < 4)
            var_a1 -= 16;
    }
    temp_v1_2 = var_a1 + var_a0;
    if (
            decalage_en_cours != 0 &&
            (MURDUR(temp_v1_2, y_1 + 16) ||
             MURDUR(temp_v1_2, y_1 + 32) ||
             MURDUR(temp_v1_2, y_1 + 48))
            )
    {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    if (ray.speed_y < 0 && MURDUR(x_1, y_1 + 16))
        ray.speed_y = 0;
}

//6F95C
void remoteControlRay(void) {
    i32 diff_x = ray.x - remoteRayXToReach;
    if (diff_x > 1) {
        ray.flags.flip_x = 0;
        set_sub_etat(&ray, 21);
    } else if (diff_x < -1) {
        ray.flags.flip_x = 1;
        set_sub_etat(&ray, 21);
    } else {
        ray.speed_x = 0;
        set_sub_etat(&ray, 20);
        remoteRayXToReach = (i16)ray.x;
    }
}

//6F9E0
void STOPPE_RAY_CONTRE_PAROIS(u8 block) {
    if (ray_mode != 3 && (block_flags[block] & 0x10) && ray.sub_etat != 7 && ray.sub_etat != 9 && ray.speed_y <= 0) {
        if (ray.sub_etat != 8) {
            set_sub_etat(&ray, 1);
            button_released = 0;
        }
        ray.link = 0; // landing speed
        ray.speed_y = 0;
    }
}

//6FA44
void RAY_IN_THE_AIR(void) {
    if (ray_wind_force) {
        ray.nb_cmd = 1;
    }
    memmove(&pos_stack[1], &pos_stack[0], sizeof(pos_stack) - sizeof(i16));
    pos_stack[0] = (i16)ray.x;

    if (ray.sub_etat == 7) {
        ray.cmd_arg_2 = -1;
    }

    u8 may_land_obj = true;
    if (helico_time != -1) {
        --helico_time;
    }
    if (ray_between_clic != -1) {
        --ray_between_clic;
    }
    ++jump_time;

    u8 anim_speed = get_eta(&ray)->anim_speed >> 4;
    if (anim_speed != 10 && anim_speed != 11) {
        ++ray.gravity_value_1;
        if (ray.gravity_value_1 >= 3) {
            ray.gravity_value_1 = 0;
        }
        ++ray.gravity_value_2;
        if (ray.gravity_value_2 >= 4) {
            ray.gravity_value_2 = 0;
        }
    }

    if (jump_time == 23) {
        ++ray.speed_y;
    }
    if (!options_jeu.test_fire1() || jump_time > 12 || in_air_because_hit || ray.timer != 0 || ray.sub_etat == 2) {
        DO_PESANTEUR(&ray);
        if ((button_released % 2) == 0 && !options_jeu.test_fire1()) {
            ++button_released;
        }
    }
    if (jump_time == 6) {
        if (ray.sub_etat == 24 || ray.sub_etat == 33) {
            set_sub_etat(&ray, 2);
        } else if (ray.sub_etat == 32) {
            set_sub_etat(&ray, 17);
        }
    }

    if (ray.link != -1) {
        ray.link += ray.speed_y; // landing speed
    }
    u8 block = calc_typ_trav(&ray, 2);
    if (ray.sub_etat != 8 && ray.sub_etat != 31) {
        RAY_HELICO(); //TODO
        if (ray.sub_etat == 15) {
            RayTestBlocSH();
        }
    }
    STOPPE_RAY_CONTRE_PAROIS(block);
    if (ray.speed_y < -3 && ray.sub_etat == 15 && anim_speed == 11) {
        eta_t* ray_eta = get_eta(&ray);
        ray_eta->anim_speed = (ray_eta->anim_speed & 0xF) | 0x10;
        ray.gravity_value_1 = 0;
        ray.gravity_value_2 = 0;
    }
    if (ray.speed_y >= 0) {
        if (ray.sub_etat == 0) {
            set_sub_etat(&ray, 1);
            ray.link = 0; // landing speed
        } else if (ray.sub_etat == 17 || ray.sub_etat == 18) {
            set_sub_etat(&ray, 19);
            ray.link = 0; // landing speed
        }
    }
    if ((ray.sub_etat != 17 && ray.sub_etat != 0 && ray.sub_etat != 9 && ray.sub_etat != 31) &&
            (ray.sub_etat != 13 || ray.speed_y >= 0) && jump_time >= 3 && ray_mode != 3
    ) {
        u8 bf = block_flags[ray.coll_btype[0]];
        if (bf & 2) {
            // Rayman lands on a block
            if (bf & 0x40) {
                if (bf & 0x80) {
                    decalage_en_cours += 16 * ray.speed_y;
                } else {
                    decalage_en_cours -= 16 * ray.speed_y;
                }
            }
            recale_position(&ray);
            ray.y += ray.speed_y;
            ray.speed_y = 0;
            ray.timer = 0;
            in_air_because_hit = 0;

            if (!(TEST_IS_ON_RESSORT_BLOC(&ray))) { //TODO
                u16 snd = (num_world == 6 && num_level == 1) ? 242 : 19;
                PlaySnd(snd, -1);
            }

            if (ray.nb_cmd == 0) {
                i32 i;
                for (i = 1; i < 10; i++) {
                    if (pos_stack[i - 1] != pos_stack[i]) {
                        break;
                    }
                }
                if (i != 10) {
                    i16 new_decalage;
                    if (pos_stack[i - 1] < pos_stack[i]) {
                        new_decalage = MIN(decalage_en_cours, -256);
                    } else {
                        new_decalage = MAX(decalage_en_cours, 256);
                    }
                    decalage_en_cours = new_decalage;
                } else {
                    decalage_en_cours = 0;
                }
            }
            helico_time = -1;
            if (ray.sub_etat == 3) {
                if (ray.anim_frame == 0) {
                    button_released = 1;
                }
            } else if (ray.sub_etat == 8) {
                i16 new_timer = MIN(ray.iframes_timer, 90);
                ray.iframes_timer = new_timer;
            }
            if (ray.sub_etat == 17 || ray.sub_etat == 18 || ray.sub_etat == 19) {
                if (RayEvts.run && abs(ray.speed_x) >= ashr16(ray.eta[1][3].right_speed, 4)) {
                    set_main_and_sub_etat(&ray, 1, 7);
                } else {
                    set_main_and_sub_etat(&ray, 0, 43);
                }
            } else if (ray.sub_etat == 25 || ray.sub_etat == 26) {
                set_main_and_sub_etat(&ray, 0, 52);
            } else if (ray.sub_etat == 27 || ray.sub_etat == 28) {
                set_main_and_sub_etat(&ray, 0, 53);
            } else {
                set_main_and_sub_etat(&ray, 0, 8);
            }
            if (ray.link >= 200) { // landing speed
                allocateRayLandingSmoke(); //TODO
            }
            ray.link = -1; // landing speed
        } else {
            if (ray.sub_etat != 7) {
                if (ray.sub_etat == 8) {
                    if (ray.speed_y > -1) {
                        rayMayLandOnAnObject(&may_land_obj, -1);
                    }
                    if (ray.main_etat != 2 || ray.sub_etat != 8) {
                        ray.iframes_timer = 90;
                    }
                } else {
                    rayMayLandOnAnObject(&may_land_obj, -1);
                }
            }
        }
    }
    if (ray_mode != 3 && ray.main_etat == 2 && ray.sub_etat != 31) {
        IS_RAY_ON_LIANE(); //TODO
        CAN_RAY_HANG_BLOC(); //TODO
    }

    if (may_land_obj == true) {
        if (ray.speed_y > 4) {
            ray.speed_y = 4;
        }
        if (ray.speed_y < -10) {
            ray.speed_y = -10;
        }
    }
    if (num_world == world_2_music && num_level == 15 && (xmapmax < ray.x + 150) && (mp.height * 16 - 100 < ray.y) && ray.hit_points != -1) {
        ChangeLevel();
        DO_FADE_OUT();
    }
}

//7008C
void terminateFistWhenRayDies(void) {
    if (poing.is_active) {
        DO_NOVA(poing_obj); //TODO
        switch_off_fist();
    }
}

//700A4
void snifRayIsDead(obj_t* obj) {
    --status_bar.lives;
    RayEvts.super_helico = 0;
    status_bar.num_wiz = 0;
    if (!ray_se_noie && !(ray.main_etat == 3 && ray.sub_etat == 32)) {
        lidol_to_allocate = 5;
        lidol_source_obj = obj;
        gerbe = true;
    }
    terminateFistWhenRayDies();
}

//7010C
void rayfallsinwater(void) {
    /* 62FA4 801877A4 -O2 -msoft-float */
    set_main_and_sub_etat(&ray, 3, 22);
    decalage_en_cours = 0;
    ray.anim_frame = 0;
    ray.speed_y = 0;
    ray.speed_x = 0;
    decalage_en_cours = 0;
    if (ray_on_poelle) {
        ray.x -= h_scroll_speed;
    }
    h_scroll_speed = 0;
    dead_time = 1;
    ray.iframes_timer = 120;
    ray_se_noie = true;
    allocate_splash(&ray);
    terminateFistWhenRayDies();
}

//701A8
u8 RAY_DEAD(void) {
    if (ray.x < LEFT_MAP_BORDER - 10 ||
            (ray.screen_y + ray.offset_by < -20 && scroll_y == -1) ||
            ray.x > RIGHT_MAP_BORDER + 10 ||
            ray.screen_y > SCREEN_HEIGHT - 20
    ) {
        ray.hit_points = 0;
        RAY_HIT(1, 0);
    } else {
        if (!(ray.main_etat == 2 && ray.sub_etat == 9) &&
                !(ray.main_etat == 3 && ray.sub_etat == 22) &&
                ray.flags.alive
        ) {
            if (ray.coll_btype[0] == BTYP_WATER && ray.cmd_arg_2 == -1 && ray.main_etat != 6) {
                rayfallsinwater();
            } else if (ray.y + ray.offset_by > (mp.height + 1) * 16) {
                set_main_and_sub_etat(&ray, 2, 9);
                if (ray.speed_y < 0) {
                    ray.speed_y = 0;
                }
            }
        }
        if ((ray.main_etat == 2 && ray.sub_etat == 9) ||
                (ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32) && EOA(&ray))
        ) {
            --dead_time;
            if (dead_time != 0 || fin_du_jeu) {
                ray.cmd_arg_2 = -1;
            } else {
                ray.hit_points = 2;
                if (ray.is_active) {
                    ray.is_active = 0;
                    ray.flags.alive = 0;
                    dead_time = 64;
                    snifRayIsDead(&ray);
                    if (status_bar.lives < 0) {
                        ray.hit_points = 0;
                        fin_du_jeu = 1;
                        status_bar.lives = 0;
                    }
                }
                status_bar.max_hitp = ray.hit_points;
                ray.iframes_timer = 90;
            }
        }
    }
    bool alive = ray.is_active && ray.flags.alive && ray_mode != 3 && ray_mode != 4;
    return alive;
}

//70408
void RAY_HURT(void) {
    /* 6346C 80187C6C -O2 -msoft-float */
    test_fin_cling();
    --ray.hit_points;
    if (ray.iframes_timer == -1) {
        u8 flag_set = get_eta(&ray)->flags & 0x40;
        if ((flag_set && !(block_flags[(u8) calc_typ_trav(&ray, 2)] & 0x10)) || !flag_set) {
            ray.iframes_timer = 120;
        } else {
            ray.iframes_timer = 60;
        }
    }

    if (ray.hit_points == -1) {
        if (ray_mode == 2) {
            ray_mode = 4;
        } else {
            ray_mode = 3;
        }
        jump_time = 0;
    }
}

//704B0
void RepousseRay(void) {
    //stub
}

//70598
u8 RayEstIlBloque(void) {
    return 0; //stub
}

//7089C
void stackRay(void) {
    //stub
}

//70964
void RAY_SURF(void) {
    //stub
}

//70A8C
void DO_SURF_CHANGE_HAIRS(void) {
    //stub
}

//70AE4
void DO_PIEDS_RAYMAN(obj_t* obj) {
    //stub
}

//70AE8
void DO_MORT_DE_RAY(void) {
    /* 63E30 80188630 -O2 -msoft-float */
    ray.iframes_timer = -1;
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    calc_obj_pos(&ray);
    calc_btyp(&ray);
    RAY_IN_THE_AIR();
    RAY_SWIP();
    STOPPE_RAY_EN_XY();
    if (ray.speed_y > 0) {
        move_down_ray();
    } else if (ray.speed_y < 0) {
        move_up_ray();
    }
    if (ray.speed_x < 0) {
        RAY_TO_THE_LEFT();
    } else if (ray.speed_x > 0) {
        RAY_TO_THE_RIGHT();
    }

    fin_poing_follow(0); //TODO

    if (ray_on_poelle == 1) {
        RayEvts = SauveRayEvts;
        ray_on_poelle = 0;
    }
    if (EOA(&ray) || !(ray.main_etat == 2 && ray.sub_etat == 8)) {
        snifRayIsDead(&ray);
        dead_time = 128;
        gerbe = 1;
        ray.flags.alive = 0;
        if (status_bar.lives >= 0) {
            ray.hit_points = 2;
            status_bar.max_hitp = 2;
        } else {
            ray.hit_points = 0;
            status_bar.lives = 0;
            fin_du_jeu = 1;
        }
    }
    DO_ANIM(&ray);
    stackRay();
}

//70C20
void TEST_FIN_FOLLOW(void) {
    //stub
}

//70D58
void RAY_FOLLOW(void) {
    //stub
}

//70E14
void RAY_RESPOND_TO_ALL_DIRS(void) {
    /* 32AF8 801572F8 -O2 -msoft-float */
    if (ray_on_poelle == true) {
        if (decalage_en_cours == 0 && ray.speed_y == 0)
        {
            if (!rightjoy() || ray.flags.flip_x)
            {
                if (leftjoy() && ray.flags.flip_x)
                    RAY_RESPOND_TO_DIR(0);
            }
            else
                RAY_RESPOND_TO_DIR(1);
        }
        else if (decalage_en_cours > 0 && !(ray.flags.flip_x))
            RAY_RESPOND_TO_DIR(1);
        else if (decalage_en_cours < 0 && ray.flags.flip_x)
            RAY_RESPOND_TO_DIR(0);

        if (joy_done == 0)
        {
            RAY_RESPOND_TO_NOTHING();
            return;
        }
        else
        {
            compteur_attente = 0;
            return;
        }
    }

    if (RayEvts.reverse == 0) {
        if (rightjoy() || RayEvts.force_run != 0)
            RAY_RESPOND_TO_DIR(1);
        if (leftjoy() && RayEvts.force_run == 0)
            RAY_RESPOND_TO_DIR(0);
        if (downjoy() && joy_done == 0 && RayEvts.force_run == 0)
        {
            RAY_RESPOND_TO_DOWN();
            joy_done++;
        }
        if (upjoy())
        {
            if (joy_done == 0)
            {
                if (RayEvts.force_run == 0)
                {
                    RAY_RESPOND_TO_UP();
                    joy_done++;
                }
            }
            else
            {
                compteur_attente = 0;
                animation_attente = 0;
                return;
            }
        }
        if (joy_done != 0)
        {
            compteur_attente = 0;
            animation_attente = 0;
            return;
        }
        if (RayEvts.force_run == 0) {
            RAY_RESPOND_TO_NOTHING();
            return;
        } else {
            compteur_attente = 0;
            animation_attente = 0;
            return;
        }
    }
    if (rightjoy())
        RAY_RESPOND_TO_DIR(0);
    if (leftjoy())
        RAY_RESPOND_TO_DIR(1);
    if (downjoy() && joy_done == 0)
    {
        RAY_RESPOND_TO_DOWN();
        joy_done++;
    }
    if (upjoy())
    {
        if (joy_done == 0)
        {
            RAY_RESPOND_TO_UP();
            joy_done++;
        }
        else
        {
            compteur_attente = 0;
            animation_attente = 0;
            return;
        }
    }

    if (joy_done == 0) {
        RAY_RESPOND_TO_NOTHING();
    } else {
        compteur_attente = 0;
        animation_attente = 0;
    }
}

//70FF8
void DO_RAY_ON_MS(void) {
    //stub
}

//7133C
void DO_RAYMAN(void) {
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    setvol(-1);
    if (RAY_DEAD()) {
        ray.ray_dist = ((ray.y + ray.offset_by) >> 4) * mp.width + ((ray.x + ray.offset_bx) >> 4);
        COLL_RAY_BLK_MORTEL();
        if (scroll_y != -1 && ray.screen_y + ray.offset_bx < 0) {
            ray.speed_y = 0;
        }
        if (ray.iframes_timer == -1 && get_eta(&ray)->flags & 8) {
            if (COLL_RAY_PIC()) { // TODO
                RAY_HIT(1, 0);
            }
        }

        if (RayEvts.squashed && ray.iframes_timer == -1 && ray.scale == 0) {
            Ray_RayEcrase(); // TODO
        }

        joy_done = 0;
        calc_obj_pos(&ray);
        if (ray.cmd_arg_2 == -1) {
            calc_btyp(&ray);
        } else {
            ray.speed_y = 0;
            ray.speed_x = 0;
        }

        if (!(remoteRayXToReach == -32000 || get_eta(&ray)->flags & 0x40)) {
            if ((ray.main_etat == 0 && !(ray.sub_etat == 4 || ray.sub_etat == 5 || ray.sub_etat == 6 ||
                                         ray.sub_etat == 7 || ray.sub_etat == 9 || ray.sub_etat == 10)) ||
                (ray.main_etat == 1 && !(ray.sub_etat == 1)) ||
                (ray.main_etat == 3 &&
                 !(ray.sub_etat == 1 || (ray.sub_etat == 2 || ray.sub_etat == 3) || ray.sub_etat == 4))
                    ) {
                if (remoteRayXToReach != 0) {
                    set_main_and_sub_etat(&ray, 3, 21);
                } else {
                    set_main_and_sub_etat(&ray, 3, 20);
                }
            }
        }

        if ((ray.main_etat == 2 && ray.sub_etat == 8) || (ray.main_etat == 2 && ray.sub_etat == 31)) {
            RAY_IN_THE_AIR(); //TODO
        } else if (ray.main_etat == 3) {
            if (ray.sub_etat == 16) {
                DO_GROWING_PLATFORM(); //TODO
                if (eau_obj_id != -1) {
                    level.objects[eau_obj_id].iframes_timer = 1;
                }
            } else if (ray.sub_etat == 21 || ray.sub_etat == 20) {
                remoteControlRay();
            }

            if (!(ray.sub_etat == 22 || ray.sub_etat == 23 || ray.sub_etat == 32)) {
                RAY_SWIP(); //TODO
            }
        } else {
            if (options_jeu.test_fire1()) {
                RAY_RESPOND_TO_FIRE1(); // jump //TODO
            }
            if (options_jeu.test_fire0()) {
                RAY_RESPOND_TO_FIRE0(); // weapon //TODO
            }
            if (options_jeu.test_button3()) {
                RAY_RESPOND_TO_BUTTON3(); // action //TODO
            }
            if (options_jeu.test_button4()) {
                //RAY_RESPOND_TO_BUTTON4(); //unused; NOTE: nullsub present in the Android version optimized out?
            }

            if (record.is_playing && butX1pressed() && butX0pressed()) {
                ChangeLevel();
            }

            if (poing.is_charging) {
                if (ray.sub_etat == 12) {
                    RAY_GROW_FIST();
                }
                if (!options_jeu.test_fire0()) {
                    RAY_THROW_FIST(); //TODO
                }
            }

            if (ray.main_etat == 7) {
                RAY_BALANCE(); //TODO
            } else if (ray.main_etat == 2) {
                RAY_IN_THE_AIR(); //TODO
            } else if (!options_jeu.test_fire1()) {
                button_released = 1;
            }
            RAY_RESPOND_TO_ALL_DIRS(); //TODO
        }

        // NOTE: added null check for star_ray_der and star_ray_dev
        // This seems to occur if RayEvts.reverse is set but the TYPE_202_RAY_ETOILES is missing from the level?
        // In that case, star_ray_der and star_ray_dev are not set in obj_init
        // TODO: maybe be even more robust?
        if (!(star_ray_der && star_ray_dev)) {
            RayEvts.reverse = 0;
        }

        if (RayEvts.reverse != 0) {
            i16 ray_x, ray_y, ray_w, ray_h;
            GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h);
            star_ray_der->x = star_ray_dev->x = ray_x + (ray_w >> 1) - star_ray_dev->offset_bx;
            star_ray_der->y = star_ray_dev->y = ray_y - star_ray_dev->offset_hy;

            if (!star_ray_der->is_active) {
                add_alwobj(star_ray_der);
            }
            if (!star_ray_dev->is_active) {
                add_alwobj(star_ray_dev);
            }
        } else if (RayEvts.force_run != 0) {
            if (RayEvts.force_run == 1) {
                RayEvts.force_run = 2;
                RayEvts.run = 1;
                DO_NOVA(&ray); //TODO
                set_main_and_sub_etat(&ray, 1, 3);
            } else if (!((ray.main_etat == 1 && (ray.sub_etat == 3 || ray.sub_etat == 7)) || ray.main_etat == 2)) {
                RayEvts.run = 1;
                set_main_and_sub_etat(&ray, 1, 3);
            } else if (RayEvts.force_run == 3 && (ray.main_etat != 2)) {
                RayEvts.force_run = 0;
            }
        }

        if (ray.cmd_arg_2 != -1) {
            RAY_FOLLOW(); //TODO
        }

        if (RayEstIlBloque()) { //TODO
            if (ray.main_etat == 7) {
                RAY_FIN_BALANCE(); //TODO
            }
            ray.speed_x = 0;
            ray.nb_cmd = 0;
            decalage_en_cours = 0;
        }

        if (pierreAcorde_obj_id != -1) {
            RepousseRay(); //TODO
        }

        RAY_SURF(); //TODO
        STOPPE_RAY_EN_XY(); //TODO

        if (ray.speed_y <= 0) {
            if (ray.speed_y < 0) {
                move_up_ray(); //TODO
            }
        } else {
            move_down_ray(); //TODO
        }
        if (ray.speed_x >= 0) {
            if (ray.speed_x > 0) {
                RAY_TO_THE_RIGHT(); //TODO
            }
        } else {
            RAY_TO_THE_LEFT(); //TODO
        }

        if (ray.flags.alive && ray.main_etat != 7) {
            DO_ANIM(&ray);
            DO_SURF_CHANGE_HAIRS(); //TODO
        }

        GET_RAY_ZDC(&ray, &ray_zdc_x, &ray_zdc_y, &ray_zdc_w, &ray_zdc_h); //TODO
        DO_COLLISIONS(); //TODO
        if (!fin_boss) {
            TEST_SIGNPOST(); //TODO
        }
    }

    stackRay(); //TODO
}

//71920
void INIT_PC(void) {
    init_memory(&main_mem_tmp, 0x2EE00);
}

//71934
void FIN_PC(void) {
    free(main_mem_tmp);
    main_mem_tmp = NULL;
}

//71940
void updateLogo(i32 fade_duration, i32 a2, i32 a3) {
    start_fade_in(2);
    WaitNSynchro(5);
    memcpy(DrawBufferNormal, PLAN2BIT, 320*200);
    //DisplayBufferModeNormal(DrawBufferNormal, display_buffer, current_rvb);
    for (i32 i = 0; i < fade_duration; ++i) {
        do_fade(&rvb_pres, &current_rvb);
        advance_frame();
        readinput();
        if (TOUCHE(SC_SPACE) || but0pressed() || but1pressed()) {
            goto end;
        }
    }
    // TODO: wait for fixed duration if CD music not available
    while (is_ogg_playing) {
        advance_frame();
        readinput();
        if (TOUCHE(SC_SPACE) || but0pressed() || but1pressed()) {
            goto end;
        }
    }
    end:
    fade_out(2, &rvb_pres);
    WaitNSynchro(1);
}

//71A70
void LOAD_SCREEN(void) {
    LoadPlan2InVignet(main_mem_tmp, 29);
}

//71A84
void sub_71A84(void) {
    LoadPlan2InVignet(main_mem_tmp, 31);
}

//71A98
void sub_71A98(void) {
    //stub
}

//71B34
void DO_UBI_LOGO(void) {
    INIT_PC();
    SetCompteurTrameAudio();
    current_pal_id = 0;
    LOAD_SCREEN();
    INIT_FADE_IN();
    play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
    updateLogo(60, -1, 8);
    FIN_PC();
    stop_cd();
}




