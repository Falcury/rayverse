
//24E90
u8 MURDUR(i16 x, i16 y) {
    bool result = BTYP(x >> 4, y >> 4) == BTYP_SOLID;
    return result;
}

//24EAC
i32 dist_to_bloc_floor(i16 btype, i16 x, i16 y) {
    i32 result = calcblocfloor[btype](x, y) - y;
    return result;
}

//24EC8
u8 bloc_floor(i16 btype, i16 x, i16 y) {
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
            temp_v1_2 = obj->coll_btype[0];
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
    return 0; //stub
}

//25388
void TEST_FIN_BLOC(obj_t* obj) {
    //stub
}

//256D4
void TEST_IS_ON_RESSORT_BLOC(obj_t* obj) {
    //stub
}

//25708
void IS_ON_RESSORT_BLOC(obj_t* obj) {
    //stub
}

//25820
void CALC_MOV_ON_BLOC(obj_t* obj) {
    //stub
}

//25B30
void recale_position(obj_t* obj) {
    i16 y = (obj->offset_by + obj->y) & ~0xF;
    i16 x = (obj->offset_bx + obj->x) & 0xF;
    i16 btyp = BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4);

    i32 v5 = calcblocrecal[btyp](x, 0);

    if (obj->type != TYPE_23_RAYMAN) {
        obj->y = y + v5 - obj->offset_by;
    } else {
        obj->speed_y = y + v5 - obj->offset_by - obj->y;
    }
}

//25BB0
i32 blocs1_empty(i32 a1, i32 a2) {
    return a2 + 1;
}

//25BB4
i32 blocs1_right_45(i32 a1, i32 a2) {
    return 15 - a1;
}

//25BC0
i32 blocs1_left_45(i32 a1, i32 a2) {
    return a1;
}

//25BC4
i32 blocs1_right1_30(i32 a1, i32 a2) {
    return 15 - (a1 >> 1);
}

//25BD4
i32 blocs1_right2_30(i32 a1, i32 a2) {
    return 7 - (a1 >> 1);
}

//25BE4
i32 blocs1_left1_30(i32 a1, i32 a2) {
    return a1 >> 1;
}

//25BE8
i32 blocs1_left2_30(i32 a1, i32 a2) {
    return (a1 >> 1) + 8;
}

//25BF0
i32 blocs3_empty(i32 a1, i32 a2) {
    return a2;
}
#define blocs1_liane blocs3_empty

//25BF4
i32 blocs4_empty(i32 a1, i32 a2) {
    return 0;
}
#define blocs1_hor blocs4_empty

//25BF8
i32 blocs2_empty(i32 a1, i32 a2) {
    return 16;
}

//25C7C
void init_calcbloc_func(void) {
    for (i32 i = 0; i < 31; ++i) {
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

