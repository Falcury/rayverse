
//24E90
u8 MURDUR(s16 x, s16 y) {
    bool result = BTYP(x >> 4, y >> 4) == BTYP_SOLID;
    return result;
}

//24EAC
s32 dist_to_bloc_floor(s16 btype, s16 x, s16 y) {
    s32 result = calcblocfloor[btype](x, y) - y;
    return result;
}

//24EC8
u8 bloc_floor(s16 btype, s16 x, s16 y) {
    if (dist_to_bloc_floor(btype, x, y) > 0) {
        return 0;
    } else {
        return btype;
    }
}

//24F60
u8 calc_typ_trav(obj_t* obj, u8 a2) {
    /* 6E710 80192F10 -O2 -msoft-float */
    s16 temp_v0_0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1_0;
    s16 var_a0_2;
    s32 temp_a0;
    s32 temp_s0;
    s32 var_a0_0;
    s32 var_s0_1;
    s32 var_s0_2;
    s32 var_s2;
    u8 var_s3_2;
    s32 var_v0_1;
    s32 var_v0_2;
    s16 temp_s1;
    s32 temp_v1_2;
    u8 var_s3_1;
    s16 test_3;

    var_s3_1 = a2;
    var_v0_1 = 0;
    var_s0_1 = 0;
    if ((obj->type == TYPE_23_RAYMAN) && RayEvts.tiny) {
        var_s3_1 = ashr16(a2, 1);
        if (var_s3_1 == 0 && !(get_eta(&ray)->flags & 0x40)) {
            var_s3_1 = 1;
        }
    }
    temp_v1_0 = obj->offset_bx + (u16) obj->x;
    temp_v0_0 = obj->offset_by + (u16) obj->y;
    var_v0_2 = 0 << 0x10;
    switch (BTYP((s32) temp_v1_0 >> 4, (s32) temp_v0_0 >> 4))
    {
        case BTYP_NONE:
        case BTYP_CHDIR:
        case BTYP_HURT:
        case BTYP_WATER:
        case BTYP_SPIKES:
        case BTYP_CLIFF:
            var_s0_1 += 8 - (u16) obj->speed_y;
            break;
        case BTYP_SOLID_RIGHT_45:
        case BTYP_SLIPPERY_RIGHT_45:
            if (obj->speed_x >= 0)
            {
                var_s0_1 += 0x11;
            }
            break;
        case BTYP_SOLID_LEFT_45:
        case BTYP_SLIPPERY_LEFT_45:
            if (obj->speed_x <= 0)
            {
                var_s0_1 += 0x11;
            }
            break;
        case BTYP_SOLID_RIGHT1_30:
        case BTYP_SOLID_RIGHT2_30:
        case BTYP_SLIPPERY_RIGHT1_30:
        case BTYP_SLIPPERY_RIGHT2_30:
            if (obj->speed_x >= 0)
            {
                var_s0_1 += 0x11;
            }
            else
            {
                var_s0_1 += 2;
            }
            break;
        case BTYP_SOLID_LEFT1_30:
        case BTYP_SOLID_LEFT2_30:
        case BTYP_SLIPPERY_LEFT1_30:
        case BTYP_SLIPPERY_LEFT2_30:
            if (obj->speed_x <= 0)
            {
                var_s0_1 += 0x11;
            }
            else
            {
                var_s0_1 += 2;
            }
            break;
        case BTYP_RESSORT:
        case BTYP_SOLID_PASSTHROUGH:
        case BTYP_SOLID:
        case BTYP_SLIPPERY:
            temp_v1_2 = obj->btypes[0];
            if ((s32) temp_v1_2 >= 2)
            {
                if ((s32) temp_v1_2 >= 4)
                {
                    if ((s32) temp_v1_2 < 0x14)
                    {
                        if ((s32) temp_v1_2 >= 0x12)
                        {
                            var_s0_1 += 8;
                        }
                    }
                }
                else
                {
                    var_s0_1 += 8;
                }
            }
            switch (BTYP(temp_v1_0 >> 4, (temp_v0_0 - 0x10) >> 4))
            {
                case BTYP_NONE:
                case BTYP_CHDIR:
                case BTYP_HURT:
                case BTYP_WATER:
                case BTYP_SPIKES:
                case BTYP_CLIFF:
                    var_s0_1 += 9;
                    break;
                case BTYP_SOLID_RIGHT_45:
                case BTYP_SLIPPERY_RIGHT_45:
                    if (obj->speed_x >= 0)
                    {
                        var_s0_1 += 0x19;
                    }
                    break;
                case BTYP_SOLID_LEFT_45:
                case BTYP_SLIPPERY_LEFT_45:
                    if (obj->speed_x <= 0)
                    {
                        var_s0_1 += 0x19;
                    }
                    break;
                case BTYP_SOLID_RIGHT1_30:
                case BTYP_SOLID_RIGHT2_30:
                case BTYP_SLIPPERY_RIGHT1_30:
                case BTYP_SLIPPERY_RIGHT2_30:
                    if (obj->speed_x >= 0)
                    {
                        var_s0_1 += 0x11;
                    }
                    else
                    {
                        var_s0_1 += 2;
                    }
                    break;
                case BTYP_SOLID_LEFT1_30:
                case BTYP_SOLID_LEFT2_30:
                case BTYP_SLIPPERY_LEFT1_30:
                case BTYP_SLIPPERY_LEFT2_30:
                    if (obj->speed_x <= 0)
                    {
                        var_s0_1 += 0x11;
                    }
                    else
                    {
                        var_s0_1 += 2;
                    }
                    break;
                case BTYP_LIANE:
                    var_s0_1 += 1;
                    break;
                case 9:
                case 11:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                default:
                    var_s0_1 += 8;
                    break;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 16:
        case 17:
        case 26:
        case 27:
        case 28:
        case 29:
        default:
            /* on android default case has 0 */
            break;
    }
    temp_s0 = (s16) var_s0_1;
    var_s3_1 = var_s3_1 - ashr16(temp_s0, 4U);
    if ((var_s3_1) >= 0x81U)
    {
        var_s3_1 = 0;
    }
    ashr16(temp_v1_0, 4U);
    ashr16(temp_v0_0 - temp_s0, 4U);
    var_s0_2 = 0;
    var_s2 = 0;
    while (var_s3_1 >= var_s0_2)
    {

        if ((MURDUR(temp_v1_0, (temp_v0_0 - temp_s0) - ((var_s3_1 - var_s0_2) * 0x10))) != 0)
        {
            var_s2 += 1;
        }
        var_s0_2 += 1;
    }

    if (var_s2 & 0xFF)
    {
        var_v0_1 = 0x0000000F;
    }
    return var_v0_1;
}

//251F8
u8 calc_typ_travd(obj_t* obj, u8 a2) {
    s16 spd_x_sgn = obj->speed_x; /* ??? */
    s16 x_offs_1;
    s32 unk_1;
    s16 x_saved;
    s16 x_offs_2;
    s16 foll_x; s16 foll_y; s16 foll_w; s16 foll_h;
    u8 unk_2;
    s32 res;

    /* sgn() on android */
    if (obj->speed_x >= 0)
        spd_x_sgn = obj->speed_x > 0;
    else
        spd_x_sgn = -1;

    if (spd_x_sgn != 0)
    {
        x_offs_1 = 14;
        if (obj->type == TYPE_RAYMAN)
        {
            unk_1 = 4;
            if (ray.scale != 0)
            {
                x_offs_1 = 8;
                unk_1 = 2;
            }

            if (ray.param != -1)
                x_offs_1 -= unk_1;
        }
        x_offs_1 *= spd_x_sgn;
        if (a2)
            x_offs_1 = -x_offs_1;
        x_saved = obj->x;
        obj->x += x_offs_1;
        if (!a2)
        {
            x_offs_2 = obj->speed_x;
            if (flags[obj->type] & flags1_0x10_move_x)
                x_offs_2 /= 16;
            obj->x -= x_offs_2;
        }

        if (obj->flags.follow_enabled)
            GET_SPRITE_POS(obj, obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h);
        else
            foll_y = 0;

        unk_2 = ashr16(-(foll_y + obj->offset_hy) + obj->offset_by, 4);
        if (obj->type == TYPE_RAYMAN && ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
            unk_2 = ashr16(unk_2, 1);
        if (unk_2 != 0)
            unk_2--;
        res = calc_typ_trav(obj, unk_2);
        obj->x = x_saved;
    }
    else
        res = 0;

    return res;
}

//25388
void TEST_FIN_BLOC(obj_t* obj) {
    /* 6EC34 80193434 -O2 -msoft-float */
    if (!(block_flags[obj->btypes[0]] & 2))
    {
        switch (obj->type)
        {
            case TYPE_RAYMAN:
                if (ray.main_etat == 1 && ray.sub_etat == 1) {
                    set_main_and_sub_etat(obj, 2, 6);
                } else if (ray.main_etat == 1 && ray.sub_etat == 6) {
                    set_main_and_sub_etat(obj, 2, 23);
                } else {
                    IS_RAY_ON_LIANE();
                    if (
                            !(hand_btyp == BTYP_LIANE || hand_btypd == BTYP_LIANE || hand_btypg == BTYP_LIANE ||
                              ray.main_etat == 5)
                            )
                    {
                        if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                        {
                            ray.speed_y = 0;
                            ray.y += 16;
                            ray.screen_y += 16;
                        }

                        if (ray_on_poelle)
                        {
                            if (ray.main_etat == 0 && ray.sub_etat == 40)
                                set_main_and_sub_etat(obj, 2, 26);
                            else
                                set_main_and_sub_etat(obj, 2, 28);
                        }
                        else
                        {
                            if (ray.main_etat == 1 && (ray.sub_etat == 9 || ray.sub_etat == 11)) {
                                ray.flags.flip_x ^= 1;
                            }
                            if (Abs(ray.speed_x) < 3)
                            {
                                Reset_air_speed(false);
                                if (!(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40))
                                    set_main_and_sub_etat(obj, 2, 24);
                                else
                                    set_main_and_sub_etat(obj, 2, 33);
                            }
                            else
                            {
                                Reset_air_speed(true);
                                set_main_and_sub_etat(obj, 2, 32);
                            }
                        }
                        jump_time = 0;
                        if (ray_last_ground_btyp == false || ray_wind_force != 0)
                            ray.nb_cmd = 1;
                        else
                            ray.nb_cmd = 0;
                    }
                }
                break;
            case TYPE_LIDOLPINK:
                set_main_and_sub_etat(obj, 2, 2);
                skipToLabel(obj, 2, true);
                break;
            case TYPE_BADGUY1:
                set_main_etat(obj, 2);
                if (obj->sub_etat != 2)
                    set_sub_etat(obj, 2);
                else
                    set_sub_etat(obj, 10);
                break;
            case TYPE_BADGUY2:
            case TYPE_BADGUY3:
                set_main_and_sub_etat(obj, 2, 2);
                break;
            case TYPE_STONEDOG:
            case TYPE_STONEDOG2:
                skipToLabel(obj, 2, true);
                obj->gravity_value_2 = 7;
                obj->speed_y = 0;
                break;
            case TYPE_MITE:
                skipToLabel(obj, 0, true);
                break;
            case TYPE_SPIDER:
                obj->cmd_context_depth = -1;
                set_main_and_sub_etat(obj, 2, 0);
                skipToLabel(obj, 8, true);
                obj->speed_x = 0;
                obj->speed_y = 0;
                break;
            case TYPE_STONEWOMAN:
                set_main_and_sub_etat(obj, 2, 4);
                skipToLabel(obj, 15, false);
                break;
        }
        obj->link = 0;
        obj->gravity_value_1 = 0;
        if (!(obj->type == TYPE_STONEDOG || obj->type == TYPE_STONEDOG2))
            obj->gravity_value_2 = 0;
    }
}

//256D4
u8 TEST_IS_ON_RESSORT_BLOC(obj_t* obj) {
    if (obj->btypes[0] == BTYP_RESSORT && obj->speed_y >= 0) {
        if (obj->type == TYPE_23_RAYMAN) {
            return true;
        } else {
            return (flags[obj->type] & flags2_8_can_jump) != 0;
        }
    } else {
        return false;
    }
}

//25708
u8 IS_ON_RESSORT_BLOC(obj_t* obj) {
    u8 result = false;
    if (obj->btypes[0] == BTYP_RESSORT && obj->speed_y >= 0) {
        if (obj->type == TYPE_23_RAYMAN) {
            button_released = 1;
            set_main_and_sub_etat(&ray, 0, 0);
            ray_jump();
            PlaySnd(249, -1);
            ray.speed_y -= 3;
            jump_time = 12;
            allocatePaillette(&ray);
            result = true;
        } else if ((flags[obj->type] & flags2_8_can_jump) != 0) {
            obj_jump(obj);
            result = true;
        }
    }
    return result;
}

//25820
void CALC_MOV_ON_BLOC(obj_t* obj) {
    s16 temp_s0_1;
    s16 temp_s0_3;
    s32 temp_s1_1;
    s16 temp_s1_2;
    u8 temp_v0_1;
    s16 temp_v0_2;
    s16 temp_v1_6;
    s32 var_s1;
    s16 var_v0_2;
    s32 temp_a0;
    s32 temp_s0_2;
    s32 temp_s4;
    s32 x_in_tile;
    s32 var_s2_1;
    s32 var_s2_2;
    s32 y_in_tile;
    s32 speed_x;
    s32 var_v0;
    u32 temp_v1_4;
    u32 temp_v1_5;
    u8 temp_v1_2;
    u8 temp_v1_3;

    /*var_s2_1 = saved_reg_s2;*/
    temp_a0 = obj->type * 4;
    if ((flags[obj->type] & flags2_1_check_tile_type) || obj->type == TYPE_23_RAYMAN) {
        if ((flags[obj->type] & flags1_0x10_move_x) && obj->type != TYPE_23_RAYMAN) {
            speed_x = instantSpeed(obj->speed_x);
        } else {
            speed_x = obj->speed_x;
        }
        x_in_tile = speed_x + ((obj->offset_bx + obj->x) & 0xF);
        y_in_tile = (obj->offset_by + obj->y) & 0xF;
        if ((obj->type == TYPE_23_RAYMAN) && (ray.param != -1)) {
            if (speed_x > 0)
            {
                x_in_tile = 15;
            }
            else if (speed_x < 0)
            {
                x_in_tile = 0;
            }
            temp_v1_2 = obj->btypes[0];
            var_s2_1 = calcbloc1[obj->btypes[0]](x_in_tile, y_in_tile);
            y_in_tile = var_s2_1;
        }
        else
        {
            var_s2_1 = calcbloc1[obj->btypes[0]](x_in_tile, y_in_tile);

        }
        var_s1 = var_s2_1 - y_in_tile;
        x_in_tile = speed_x + (obj->x + obj->offset_bx);
        temp_s1_1 = var_s1 + (obj->y + obj->offset_by);
        temp_v0_1 = BTYP(x_in_tile >> 4, temp_s1_1 >> 4);
        x_in_tile = x_in_tile & 0xF;
        y_in_tile = temp_s1_1 & 0xF;
        switch (temp_v0_1)
        {
            case 0:
            case 1:
            case 8:
            case 10:
            case 24:
            case 25:
                temp_s1_2 = speed_x + (obj->x + obj->offset_bx);
                temp_s0_3 = var_s1 + (obj->y + obj->offset_by) + 16;
                temp_v0_2 = dist_to_bloc_floor(BTYP(temp_s1_2 >> 4, temp_s0_3 >> 4), temp_s1_2 & 0xF, temp_s0_3 | ~0xF);
                if (temp_v0_2 >= 3)
                {
                    case 12:
                        var_s2_1 = y_in_tile;
                }
                else
                {
                    var_s2_1 = y_in_tile + temp_v0_2;
                }
                break;
            case 2:
            case 18:
                var_s2_1 = 15 - x_in_tile;
                break;
            case 3:
            case 19:
                var_s2_1 = x_in_tile;
                break;
            case 4:
            case 20:
                var_s2_1 = 15 - (x_in_tile >> 1);
                break;
            case 5:
            case 21:
                var_s2_1 = 7 - (x_in_tile >> 1);
                break;
            case 6:
            case 22:
                var_s2_1 = ((x_in_tile) >> 1);
                break;
            case 7:
            case 23:
                var_s2_1 = ((x_in_tile) >> 1) + 8;
                break;
            case 9:
            case 14:
            case 15:
            case 30:
                temp_v1_5 = BTYP((obj->x + obj->offset_bx + speed_x) >> 4, (obj->y + obj->offset_by + var_s1 - 16) >> 4);
                var_s2_2 = calcbloc2[temp_v1_5](x_in_tile, y_in_tile);
                var_s2_1 = (var_s2_2 - 16);
                var_s2_1 = y_in_tile + var_s2_1;
                break;
        }
        if (!IS_ON_RESSORT_BLOC(obj)) {
            if (obj->type == TYPE_23_RAYMAN) {
                if ((ray.follow_id == -1 && ray.main_etat != 2) && Abs(obj->speed_y) > 2 && Abs(obj->speed_y) < 16 &&
                    !(block_flags[temp_v0_1] & 2))
                {
                    s16 saved_speed_y = obj->speed_y;
                    set_main_and_sub_etat(&ray, 0, 0);
                    button_released = 1;
                    ray_jump();
                    obj->speed_y = saved_speed_y;
                }
                else
                {
                    if (
                            ((ray.main_etat != 3) || (((ray.sub_etat != 32)) &&
//                                                     (ray.sub_etat != 0x26) && //NOTE: this subetat only in PS1
                                                     ((ray.sub_etat != 22))))
                            )
                    {
                        obj->speed_y = (var_s1 + var_s2_1) - y_in_tile;
                        TEST_FIN_BLOC(obj);
                    }
                }
            }
            else {
                obj->speed_y = (var_s1 + var_s2_1) - y_in_tile;
                TEST_FIN_BLOC(obj);
            }
        }
    }
}

//25B30
void recale_position(obj_t* obj) {
    s16 y = (obj->offset_by + obj->y) & ~0xF;
    s16 x = (obj->offset_bx + obj->x) & 0xF;
    s16 btyp = BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4);

    s32 v5 = calcblocrecal[btyp](x, 0);

    if (obj->type != TYPE_23_RAYMAN) {
        obj->y = y + v5 - obj->offset_by;
    } else {
        obj->speed_y = y + v5 - obj->offset_by - obj->y;
    }
}

//25BB0
s32 blocs1_empty(s32 x, s32 y) {
    return y + 1;
}

//25BB4
s32 blocs1_right_45(s32 x, s32 y) {
    return 15 - x;
}

//25BC0
s32 blocs1_left_45(s32 x, s32 y) {
    return x;
}

//25BC4
s32 blocs1_right1_30(s32 x, s32 y) {
    return 15 - (x >> 1);
}

//25BD4
s32 blocs1_right2_30(s32 x, s32 y) {
    return 7 - (x >> 1);
}

//25BE4
s32 blocs1_left1_30(s32 x, s32 y) {
    return x >> 1;
}

//25BE8
s32 blocs1_left2_30(s32 x, s32 y) {
    return (x >> 1) + 8;
}

//25BF0
s32 blocs3_empty(s32 x, s32 y) {
    return y;
}

//25BF0 (identical to blocs3_empty)
s32 blocs1_liane(s32 x, s32 y) {
    return y;
}

//25BF4
s32 blocs4_empty(s32 x, s32 y) {
    return 0;
}

//25BF4 (identical to blocs4_empty)
s32 blocs1_hor(s32 x, s32 y) {
    return 0;
}

//25BF8
s32 blocs2_empty(s32 x, s32 y) {
    return 16;
}

//25C7C
void init_calcbloc_func(void) {
    for (s32 i = 0; i < 31; ++i) {
        switch(i) {
            default: {
                calcbloc1[i] = blocs1_empty;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            } break;
            case 2:
            case 18: {
                calcbloc1[i] = blocs1_right_45;
                calcbloc2[i] = blocs1_right_45;
                calcblocfloor[i] = blocs1_right_45;
                calcblocrecal[i] = blocs1_right_45;
            } break;
            case 3:
            case 19: {
                calcbloc1[i] = blocs1_left_45;
                calcbloc2[i] = blocs1_left_45;
                calcblocfloor[i] = blocs1_left_45;
                calcblocrecal[i] = blocs1_left_45;
            } break;
            case 4:
            case 20: {
                calcbloc1[i] = blocs1_right1_30;
                calcbloc2[i] = blocs1_right1_30;
                calcblocfloor[i] = blocs1_right1_30;
                calcblocrecal[i] = blocs1_right1_30;
            } break;
            case 5:
            case 21: {
                calcbloc1[i] = blocs1_right2_30;
                calcbloc2[i] = blocs1_right2_30;
                calcblocfloor[i] = blocs1_right2_30;
                calcblocrecal[i] = blocs1_right2_30;
            } break;
            case 6:
            case 22: {
                calcbloc1[i] = blocs1_left1_30;
                calcbloc2[i] = blocs1_left1_30;
                calcblocfloor[i] = blocs1_left1_30;
                calcblocrecal[i] = blocs1_left1_30;
            } break;
            case 7:
            case 23: {
                calcbloc1[i] = blocs1_left2_30;
                calcbloc2[i] = blocs1_left2_30;
                calcblocfloor[i] = blocs1_left2_30;
                calcblocrecal[i] = blocs1_left2_30;
            } break;
            case 9:
            case 14:
            case 15:
            case 30: {
                calcbloc1[i] = blocs1_hor;
                calcbloc2[i] = blocs1_hor;
                calcblocfloor[i] = blocs1_hor;
                calcblocrecal[i] = blocs1_hor;
            } break;
            case 12: {
                calcbloc1[i] = blocs1_liane;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            }
        }
    }
}

