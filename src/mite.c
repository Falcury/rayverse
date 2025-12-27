
//52D30
void mite_esquive_poing(obj_t* mit_obj, s16* out_diff_x) {
    /* 4A85C 8016F05C -O2 -msoft-float */
    s16 unk_x_1; s16 unk_y_1;
    s16 unk_y_2;

    if (!poing_obj->is_active) {
        unk_x_1 = ray.x + ray.offset_bx;
        unk_y_1 = ray.y + ((ray.offset_by + ray.offset_hy) >> 1) - 10;
        poing_obj->speed_x = 0;
        poing_obj->speed_y = 0;
    } else {
        unk_x_1 = poing_obj->x + poing_obj->offset_bx;
        unk_y_1 = poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1) - 3;
    }
    unk_y_2 = mit_obj->y + ((mit_obj->offset_hy + mit_obj->offset_by) >> 1) - unk_y_1;
    *out_diff_x = unk_x_1 - mit_obj->x - mit_obj->offset_bx;

    if (unk_y_2 < 0) {
        if (poing_obj->speed_y > 0) {
            if (unk_y_2 > -30)
                mit_obj->speed_y += 2;
            else if (unk_y_2 > -48)
                mit_obj->speed_y += 1;
        } else {
            if (unk_y_2 > -30)
                mit_obj->speed_y -= 2;
            else if (unk_y_2 > -48)
                mit_obj->speed_y -= 1;
        }
    } else {
        if (poing_obj->speed_y >= 0) {
            if (unk_y_2 < 30)
                mit_obj->speed_y += 2;
            else if (unk_y_2 < 48)
                mit_obj->speed_y += 1;
        } else {
            if (unk_y_2 < 30)
                mit_obj->speed_y -= 2;
            else if (unk_y_2 < 48)
                mit_obj->speed_y -= 1;
        }
    }
}

//52E64
u8 HAS_MIT_JUMP(obj_t* obj) {
    /* 4AA1C 8016F21C -O2 -msoft-float */
    u8 res = false;

    if (obj->main_etat != 2) {
        if (obj->eta[obj->main_etat][obj->sub_etat].flags & 1)
            skipToLabel(obj, 1, true);
    }
    else
    {
        if (obj->sub_etat == 1 && obj->anim_frame == 0)
        {
            obj->speed_y = -5;
            obj->gravity_value_1 = 0;
            obj->gravity_value_2 = 0;
            obj->y -= 14;
        }
        else
            res = true;
    }

    return res;
}

//52ED0
u8 ray_over_mit(obj_t* obj, s16 a2) {
    /* 4AACC 8016F2CC -O2 -msoft-float */
    return (!(obj->flags.flip_x) && a2 > 0) || (obj->flags.flip_x && a2 < 0);
}

//52EF4
void fix_mit_Xspeed(obj_t* obj, s16 a2) {
    /* 4AB0C 8016F30C -O2 -msoft-float */
    s16 spd_x;
    eta_t* eta;
    s32 unk_1;

    if (obj->main_etat == 1 || obj->main_etat == 2) {
        if (ray_over_mit(obj, a2)) {
            if (obj->main_etat == 2 && obj->speed_y < -2 && obj->sub_etat != 0)
                obj->speed_y++;
            else
                HAS_MIT_JUMP(obj);

            if (!obj->flags.flip_x)
                spd_x = 4;
            else
                spd_x = -4;
            obj->speed_x = spd_x;
        } else {
            obj->speed_x = -ray.speed_x;
            if (obj->x < obj->init_x && obj->flags.flip_x) {
                if (obj->speed_x < 0)
                    obj->speed_x = 0;
                else
                    obj->speed_x *= 2;
            } else if ((obj->x > obj->init_x) && !obj->flags.flip_x) {
                if (obj->speed_x > 0)
                    obj->speed_x = 0;
                else
                    obj->speed_x *= 2;
            }
        }

        if (obj->main_etat == 1) {
            eta = &obj->eta[obj->main_etat][obj->sub_etat];
            unk_1 =
                    (!(obj->flags.flip_x) && obj->speed_x > 0) ||
                    (obj->flags.flip_x && obj->speed_x < 0);
            eta->flags = (eta->flags & ~0x10) | (unk_1 << 4);
        }

        if (obj->speed_x > 4)
            obj->speed_x = 4;
        else if (obj->speed_x < -4)
            obj->speed_x = -4;
    } else {
        SET_X_SPEED(obj);
    }
}

//53068
void DO_MIT_ATTAK(obj_t* obj) {
    /* 4ACFC 8016F4FC -O2 -msoft-float */

    // TODO(Falcury): needs clean-up and checking

    s16 temp_a0_1;
    s16 temp_a1;
    s16 temp_v0_1;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v0_6;
    s16 temp_v1_1;
    s16 temp_v1_2;
    s16 var_v0_1;
    s16 var_v0_2;
    s16 var_v0_4;
    s16 var_v1_1;
    s16 var_v1_2;
    s32 var_v0_3;
    s32 var_v0_5;
    s16 new_var;

    if ((obj->main_etat == 0) && (obj->sub_etat == 10)) {
        temp_v0_1 = obj->cmd_arg_2;
        if (temp_v0_1 > 50) {
            obj->cmd_arg_2 = temp_v0_1 - 50;
        }
    }
    else if (HAS_MIT_JUMP(obj))
    {
        if (obj->sub_etat != 1 && obj->cmd_arg_2 == 500) {
            obj->configuration = 140;
            obj->speed_y = 0;
            obj->follow_x = obj->x;
            ++obj->cmd_arg_2;
            set_sub_etat(obj, 3);
        } else if (obj->sub_etat == 3) {
            if (obj->gravity_value_2 != 0) {
                if (obj->speed_y >= 0) {
                    --obj->speed_y;
                    return;
                }
            }
        } else if (obj->sub_etat == 4) {
            temp_v0_3 = obj->x;
            temp_v1_1 = obj->follow_x;
            if ((temp_v0_3 - temp_v1_1) >= 0)
            {
                var_v0_1 = temp_v0_3 - temp_v1_1;
            }
            else
            {
                var_v0_1 = temp_v1_1 - temp_v0_3;
            }
            temp_a0_1 = obj->follow_x;
            if (((((ray.x + ray.offset_bx) - temp_a0_1) - obj->offset_bx) - 10) >= 0)
            {
                var_v1_1 = (((ray.x + ray.offset_bx) - temp_a0_1) - obj->offset_bx) - 10;
            }
            else
            {
                new_var = ray.x; /* flip order of ray.x and ray.offset_bx? see DO_STONEWOMAN_COMMAND */
                var_v1_1 = 10 - (((ray.offset_bx + new_var) - temp_a0_1) - obj->offset_bx);
            }
            if (obj->configuration < var_v0_1 || ((var_v1_1 < var_v0_1) && obj->configuration == 140))
            {
                if ((obj->configuration == 140) && (var_v1_1 < 140))
                {
                    obj->configuration = (u8) var_v1_1;
                }
                if (obj->gravity_value_2 == 1)
                {
                    if (!obj->flags.flip_x)
                    {
                        var_v0_2 = (u16) obj->speed_x + 1;
                    }
                    else
                    {
                        var_v0_2 = (u16) obj->speed_x - 1;
                    }
                    obj->speed_x = var_v0_2;
                }
                if (obj->gravity_value_1 == 1)
                {
                    temp_v0_4 = obj->speed_y;
                    if (temp_v0_4 >= -1)
                    {
                        obj->speed_y = temp_v0_4 - 1;
                    }
                }
            }
            else
            {
                temp_v1_2 = ((obj->offset_by + (u16) obj->y) - ray.y) - ray.offset_by;
                /*
                if
                (obj->speed_x <= 0 && obj->flags.flip_x && obj->speed_x >= 0)
                with obj->speed_x = -4: obj->speed_x <= 0 && !(obj->flags.flip_x)
                (obj->speed_x > 0 && obj->flags.flip_x)

                else
                (obj->speed_x <= 0 && obj->flags.flip_x && obj->speed_x < 0)
                (obj->speed_x > 0 && !(obj->flags.flip_x)) ||

                similar with DO_CLOWN_TNT3_COMMAND?
                */
                if (obj->speed_x <= 0)
                {
                    if (obj->flags.flip_x)
                    {
                        if (obj->speed_x < 0)
                        {
                            goto block_47;
                        }
                        else
                            goto block_37;
                    }
                    else
                    {
                        obj->speed_x = -4;
                        goto block_40;
                    }
                }
                block_37:
                if (obj->flags.flip_x)
                {
                    obj->speed_x = 4;
                    block_40:
                    if (temp_v1_2 >= 0xB)
                    {
                        obj->speed_y = -1;
                    }
                    else if (temp_v1_2 < -0x14)
                    {
                        obj->speed_y = 2;
                    }
                    else if (temp_v1_2 < -10)
                    {
                        obj->speed_y = 1;
                    }
                    else
                    {
                        obj->speed_y = 0;
                    }
                }
                else
                {
                    block_47:
                    if (((temp_v1_2) >= -9) && (ray.main_etat == 0))
                    {
                        obj->speed_y = 0;
                    }
                    else
                    {
                        obj->speed_y = 1;
                    }
                    if (
                            (var_v0_1 < 8 && ((!(obj->flags.flip_x)))) ||
                            ((var_v0_1 > 8) && (obj->flags.flip_x))
                            )
                    {
                        if (--obj->speed_x < -4)
                        {
                            obj->speed_x = -4;
                        }
                    }
                    else
                    {
                        if (++obj->speed_x > 4)
                        {
                            obj->speed_x = 4;
                        }
                    }
                    if ((obj->speed_x == 0) && (var_v0_1 < 8))
                    {
                        obj->cmd_arg_2 = 0;
                        set_sub_etat(obj, 0U);
                    }
                }
            }
            if (block_flags[calc_typ_travd(obj, 0)] & 1)
            {
                var_v1_2 = 1;
                if (obj->speed_x > 0)
                {
                    var_v1_2 = -1;
                }
                obj->speed_x = var_v1_2;
            }
        }

    }
}

//53320
u8 IS_MIT_PAF(obj_t* obj) {
    /* 4B154 8016F954 -O2 -msoft-float */
    s16 unk_x_1;
    s16 unk_x_2;
    s16 unk_x_3;
    s16 spd_x;
    s32 res = false;

    if (obj->main_etat == 0 && obj->sub_etat == 3)
    {
        obj->speed_y = -4;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        res = true;
    }
    else if (
            (obj->main_etat == 1 && obj->sub_etat == 2) ||
            (obj->main_etat == 0 && (obj->sub_etat == 2 || obj->sub_etat == 4)) ||
            (obj->main_etat == 2 && obj->sub_etat == 2)
            )
    {
        res = true;
        if (EOA(obj))
            obj->cmd_arg_2 = 0;
        else
        {
            if (obj->main_etat != 2)
                SET_X_SPEED(obj);
            else
            {
                obj->speed_y = 0;
                if (obj->cmd_arg_2 >= 500)
                {
                    unk_x_1 = obj->x;
                    unk_x_2 = obj->follow_x;
                    if (unk_x_1 - unk_x_2 >= 0)
                        unk_x_3 = unk_x_1 - unk_x_2;
                    else
                        unk_x_3 = unk_x_2 - unk_x_1;

                    if (unk_x_3 < 8) {
                        if (!obj->flags.flip_x)
                            obj->speed_x--;
                        else
                            obj->speed_x++;
                    } else {
                        if (!obj->flags.flip_x)
                            spd_x = 4;
                        else
                            spd_x = -4;
                        obj->speed_x = spd_x;
                    }
                }
            }
        }
    }

    return res;
}

//5342C
void DO_MIT_COMMAND(obj_t* obj) {
    /* 4B3A0 8016FBA0 -O2 -msoft-float */
    s16 diff_x_1;
    s16 diff_x_2;
    s16 diff_y_1;
    s16 esquive_x;
    s32 prev_flip_x;
    s32 new_var;
    eta_t* obj_eta = &obj->eta[obj->main_etat][obj->sub_etat];

    if (!IS_MIT_PAF(obj)) {
        if (obj->cmd_arg_2 >= 500) {
            DO_MIT_ATTAK(obj);
            return;
        }

        if (!(obj->main_etat == 0 && obj->sub_etat == 7)) {
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else if (obj->cmd == GO_RIGHT)
                obj->flags.flip_x = 1;

            if (obj->detect_zone_flag != 0)
            {
                diff_x_1 = (ray.x + ray.offset_bx) - obj->x - obj->offset_bx;
                diff_x_2 = Abs(diff_x_1) >> 1;
                diff_y_1 = (obj->y + obj->offset_by) - ray.y - ray.offset_by;
                obj->cmd_arg_2++;
                if (ray_over_mit(obj, diff_x_1))
                {
                    if (obj->main_etat == 0 && obj->sub_etat == 10)
                        set_sub_etat(obj, 12);
                    else
                        HAS_MIT_JUMP(obj);
                }
                else if (poing.is_active || poing.is_charging)
                {
                    mite_esquive_poing(obj, &esquive_x);
                    /* TODO: ??? readable ??? */
                    if (
                            !(obj->flags.flip_x) ?
                            (diff_x_1 < -10 && (-diff_x_2 < esquive_x || poing_obj->speed_x >= 0)) :
                            (diff_x_1 > 10 && (diff_x_2 > esquive_x || poing_obj->speed_x <= 0))
                            )
                    {
                        if (obj->speed_y < 0)
                            HAS_MIT_JUMP(obj);
                        else if (obj_eta->flags & 4)
                            set_main_and_sub_etat(obj, 0, 9);
                    }
                    else
                    {
                        if (obj->main_etat == 0 && obj->sub_etat == 10)
                            set_sub_etat(obj, 12);
                    }

                    MAX_2(obj->speed_y, -3);
                    MIN_2(obj->speed_y, 3);
                }
                else
                {
                    switch (ray.main_etat)
                    {
                        case 0:
                        case 3:
                            if (
                                    obj->main_etat == 1 ||
                                    (obj->main_etat == 0 && obj->sub_etat == 10)
                                    )
                                set_main_and_sub_etat(obj, 0, 8);

                            obj->speed_x = 0;
                            break;
                        case 1:
                            /* TODO: ??? */
                            if (
                                    !(
                                            obj->main_etat == 2 ||
                                            (obj->main_etat == 0 && (obj->sub_etat == 9 || (obj->sub_etat == (new_var = 0x0A)) || obj->sub_etat == 0x0B))
                                    )
                                    )
                                set_main_and_sub_etat(obj, 1, 0);
                            break;
                        case 2:
                            if (HAS_MIT_JUMP(obj))
                            {
                                if (diff_y_1 > 0)
                                {
                                    if (obj->gravity_value_1 == 1)
                                    {
                                        obj->speed_y--;
                                        MAX_2(obj->speed_y, -3);
                                    }
                                }
                                else
                                MIN_2(obj->speed_y, 1);
                            }
                            break;
                    }
                }

                if (
                        diff_x_2 < 40 && diff_y_1 < 10 &&
                        obj->link == 0 && (obj_eta->flags & 2)
                        )
                {
                    if (obj->main_etat == 1)
                        set_main_etat(obj, 0);

                    set_sub_etat(obj, 11);
                    obj->link = 1;
                    obj->timer = 0;
                }

                if (obj->link == 1)
                {
                    obj->timer++;
                    if (obj->timer > 40)
                        obj->link = 0;
                }

                if (obj->y + obj->offset_hy < ymap - 50)
                    obj->speed_y += 2;

                fix_mit_Xspeed(obj, diff_x_1);
            }
            else
            {
                obj->speed_x = 0;
                if (obj->main_etat != 2)
                {
                    if (obj->main_etat == 1)
                    {
                        set_main_and_sub_etat(obj, 0, 8);
                        skipToLabel(obj, 7, true);
                    }

                    prev_flip_x = obj->flags.flip_x;
                    calc_obj_dir(obj);
                    if (obj->flags.flip_x != prev_flip_x)
                        skipToLabel(obj, 3, true);
                }
            }

            if (block_flags[obj->btypes[3]] >> BLOCK_FULLY_SOLID & 1)
                obj->speed_y = 0;

            if (block_flags[calc_typ_travd(obj, false)] >> BLOCK_FULLY_SOLID & 1)
                obj->speed_x = 0;

            if (obj->main_etat != 2)
                CALC_MOV_ON_BLOC(obj);
        }
    }
}

//53838
void DoMitePoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 1) {
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            skipToLabel(obj, 4, true);
        } else {
            set_main_and_sub_etat(obj, 0, 3);
            obj->flags.read_commands = 0;
        }
    }
}

//5387C
void DO_MITE2_COMMAND(obj_t* obj) {
    /* 4BA08 80170208 -O2 -msoft-float */
    s16 unk_1;

    if (obj->main_etat == 0 && obj->sub_etat == 3)
    {
        obj->speed_x = 0;
        obj->speed_y = -8;
        obj->flags.read_commands = 0;
    }
    else
    {
        DO_MITE2_ESQUIVE(obj);
        if (obj->cmd_arg_2 != 0 && HAS_MIT_JUMP(obj))
        {
            if (!obj->flags.flip_x)
                obj->speed_x = -2;
            else
                obj->speed_x = 2;
        }

        if (obj->main_etat != 2)
        {
            if (on_block_chdir(obj, obj->offset_bx, obj->offset_by - 16))
            {
                obj->cmd_arg_2 = 502;
                obj->speed_x = 0;
            }
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
        }

        unk_1 = calc_typ_travd(obj, false);
        if (obj->main_etat == 2)
        {
            if (
                    block_flags[
                            BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_hy) >> 4)
                    ] >> BLOCK_FULLY_SOLID & 1 &&
                    obj->speed_y < 0
                    )
                obj->speed_y = 1;
        }
        else
        {
            if (!(block_flags[obj->btypes[4]] >> BLOCK_SOLID & 1))
            {
                set_main_and_sub_etat(obj, 2, 0);
                skipToLabel(obj, 7, true);
            }
        }

        if (block_flags[unk_1] & 1)
        {
            if (obj->main_etat == 1)
                makeUturn(obj);
            else
            {
                obj->flags.flip_x = 1 - obj->flags.flip_x;
                obj->speed_x = -obj->speed_x;
            }
        }
    }
}

//53A04
void DoMite2PoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 1) {
        if (poing_obj->speed_x > 0) {
            obj->flags.flip_x = 0;
        } else if (poing_obj->speed_x < 0) {
            obj->flags.flip_x = 1;
        }
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            skipToLabel(obj, 4, true);
        } else {
            set_main_and_sub_etat(obj, 0, 3);
            obj->flags.read_commands = 0;
        }
    }
}

//53A68
void DO_MITE2_ESQUIVE(obj_t* mit2_obj) {
    /* 3C96C 8016116C -O2 -msoft-float */
    s16 diff_x; s16 diff_y; s16 unk_1;

    if (poing.is_active || poing.is_charging) {
        calc_esquive_poing(mit2_obj, &diff_x, &diff_y, &unk_1);
        if (Abs(diff_x) <= unk_1 && diff_y < 35 && mit2_obj->main_etat == 1 &&
                (
                        (!(mit2_obj->flags.flip_x) && diff_x < 0) ||
                        (mit2_obj->flags.flip_x && diff_x > 0)
                ) &&
                sgn(mit2_obj->speed_x) == -sgn(poing_obj->speed_x)
        ) {
            mit2_obj->speed_x = 0;
            mit2_obj->speed_y = 0;
            if (ray.main_etat != 2) {
                mit2_obj->cmd_arg_2 = 501;
            } else {
                if (mit2_obj->eta[mit2_obj->main_etat][mit2_obj->sub_etat].flags & 4) {
                    set_main_and_sub_etat(mit2_obj, 0, 9);
                    skipToLabel(mit2_obj, 7, true);
                }
            }
        }
    } else {
        if (mit2_obj->main_etat == 0 && mit2_obj->sub_etat == 10) {
            mit2_obj->cmd_arg_2 = 0;
            set_sub_etat(mit2_obj, 12);
            skipToLabel(mit2_obj, 8, true);
        }
    }
}

