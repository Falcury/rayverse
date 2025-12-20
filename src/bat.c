

//1FA40
void DO_BAT_FLASH(i16 in_x, i16 in_y) {
    /* 71680 80195E80 -O2 -msoft-float */
    s16 fire_par_3;
    s16 unk_1;
    s16 unk_x = in_x;
    s16 unk_y = in_y;
    s16 diff_x = ray_zdc_x + (ray_zdc_w >> 1) - unk_x;
    s16 diff_y = ray_zdc_y + (ray_zdc_h >> 1) - unk_y;
    u8 x_gt_0 = diff_x > 0;
    u8 y_gt_0 = diff_y > 0;

    if (!x_gt_0)
        diff_x = -diff_x;
    if (!y_gt_0)
        diff_y = -diff_y;

    if (diff_x > 200)
    {
        diff_x = ashr16(diff_x, 1);
        diff_y = ashr16(diff_y, 1);
    }

    if (diff_y == 0)
    {
        if (x_gt_0)
            fire_par_3 = 27;
        else
        {
            if (diff_x != 0)
                fire_par_3 = 9;
            else
                fire_par_3 = -1;
        }
    }
    else
    {
        unk_1 = diff_x * 45 / diff_y;
        if (x_gt_0)
        {
            if (y_gt_0)
            {
                if (unk_1 >= (s16) LEN(N_anim))
                    fire_par_3 = 27;
                else
                    fire_par_3 = 27 + N_anim[unk_1];
            }
            else
            {
                if (unk_1 >= (s16) LEN(N_anim))
                    fire_par_3 = 0;
                else
                    fire_par_3 = 0 + N_anim[unk_1];
            }
        }
        else
        {
            if (y_gt_0)
            {
                if (unk_1 >= (s16) LEN(N_anim))
                    fire_par_3 = 18;
                else
                    fire_par_3 = 18 + N_anim[unk_1];
            }
            else
            {
                if (unk_1 >= (s16) LEN(N_anim))
                    fire_par_3 = 9;
                else
                    fire_par_3 = 9 + N_anim[unk_1];
            }
        }
    }

    DO_REDEYE_FIRE(unk_x, unk_y, fire_par_3);
}

//1FBC0
void DO_BAT_LEFT_FLASH(obj_t* obj) {
    /* 718C4 801960C4 -O2 -msoft-float */
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

//1FC18
void DO_BAT_RIGHT_FLASH(obj_t* obj) {
    /* 71928 80196128 -O2 -msoft-float */
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

//1FC70
void DO_BAT_LEFT_RIGHT_FLASH(obj_t* obj) {
    /* 7198C 8019618C -O2 -msoft-float */
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;

    GET_SPRITE_POS(obj, 15, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

//1FD14
i16 bat_dir(obj_t* obj) {
    /* 71A5C 8019625C -O2 -msoft-float */
    return (ray.x + ray.offset_bx - obj->x - obj->offset_bx) > 0;
}

//1FD9C
void DO_BAT_COMMAND(obj_t* obj) {
    /* 71D48 80196548 -O2 -msoft-float */
    u8 offs_bx; u8 offs_by;
    s16 spd_y;
    u8 flag_set;
    s16 obj_sub_etat = obj->sub_etat;
    u8 horl_0 = horloge[obj->eta[obj->main_etat][obj_sub_etat].anim_speed & 0xF] == 0;

    switch (obj_sub_etat)
    {
        default: break;
        case 2:
            if (obj->anim_frame == 14 && horl_0)
                DO_BAT_LEFT_RIGHT_FLASH(obj);
            break;
        case 5:
            if (obj->anim_frame == 22)
            {
                if (obj->hit_points == 0)
                    skipToLabel(obj, 8, true);
                else if ((s16) myRand(1) == 0)
                    skipToLabel(obj, 2, true);
                else
                    skipToLabel(obj, 3, true);
                return;
            }
            break;
        case 3:
        case 4:
            if (obj->anim_frame == 19 && horl_0)
            {
                bat_nb_frap++;
                if (obj_sub_etat == 4)
                    DO_BAT_RIGHT_FLASH(obj);
                if (obj_sub_etat == 3)
                    DO_BAT_LEFT_FLASH(obj);
            }
            break;
        case 6:
        case 7:
            if (obj->hit_points == 0)
                skipToLabel(obj, 8, true);
            if (obj->anim_frame == 20)
            {
                if (obj->hit_points == 0)
                    skipToLabel(obj, 8, true);
                else
                {
                    switch (bat_nb_frap)
                    {
                        case 1:
                            set_sub_etat(obj, 11);
                            break;
                        case 3:
                            set_sub_etat(obj, 11);
                            bat_nb_frap = 0;
                            break;
                        default:
                            if (obj_sub_etat == 6)
                                skipToLabel(obj, 3, true);
                            else
                                skipToLabel(obj, 2, true);
                            break;
                    }
                }
            }
            break;
        case 8:
            if (bat_dir(obj))
                skipToLabel(obj, 4, true);
            else
                skipToLabel(obj, 5, true);
            break;
        case 11:
            if (bat_dir(obj))
                skipToLabel(obj, 6, true);
            else
                skipToLabel(obj, 7, true);
            break;
        case 9:
        case 10:
        case 12:
        case 13:
            if (obj->cmd == GO_SPEED)
            {
                switch (obj_sub_etat)
                {
                    case 10:
                    case 13:
                        obj->speed_x = -Abs(obj->iframes_timer);
                        break;
                    case 12:
                    case 9:
                        obj->speed_x = Abs(obj->iframes_timer);
                        break;
                }
                obj->speed_y = obj->cmd_arg_2;
            }

            offs_bx = obj->offset_bx;
            offs_by = obj->offset_by;
            if (test_allowed(obj, offs_bx, offs_by) && on_block_chdir(obj, offs_bx, offs_by)) {
                switch (obj_sub_etat)
                {
                    case 10:
                        obj->sub_etat = 9;
                        break;
                    case 9:
                        obj->sub_etat = 10;
                        break;
                    case 13:
                        obj->sub_etat = 12;
                        break;
                    case 12:
                        obj->sub_etat = 13;
                        break;
                }
            }
            break;
        // NOTE: subetats 14-16 are only present in the PS1 version, not in the PC version
        /*case 14:
            obj->speed_x = 0;
            obj->speed_y = 0;
            break;
        case 15:
            obj->speed_x = 0;
            if (obj->anim_frame < 29)
                spd_y = -4;
            else
                spd_y = -8;
            obj->speed_y = spd_y;
            break;
        case 16:
            obj->speed_y = -8;
            if (EOA(obj))
            {
                if (obj->y + obj->offset_by < ymap)
                {
                    obj->is_active = 0;
                    obj->flags.alive = 0;
                    fix_numlevel(&ray);
                    new_level = true;
                }
            }
            break;*/
    }
}

//20058
void BAT_ray_in_zone(obj_t* obj) {
    /* 72224 80196A24 -O2 -msoft-float */
    if (obj->sub_etat == 0)
    {
        bat_nb_frap = 0;
        obj->flags.flip_x = 0;
        skipToLabel(obj, 1, true);
    }
}

//20088
void DO_BAT_POING_COLLISION(obj_t* bat_obj, i16 sprite) {
    /* 7226C 80196A6C -O2 -msoft-float */
    // NOTE: this function is empty in the PC version
    /*s16 poing_x; s16 poing_y;
    s16 b_dir = bat_dir(bat_obj);

    if (
            bat_obj->sub_etat == 5 ||
            (bat_obj->sub_etat == 6 && b_dir == 0) ||
            (bat_obj->sub_etat == 7 && b_dir == 1)
            )
    {
        bat_obj->hit_points--;
        poing_x = poing_obj->screen_x + poing_obj->offset_bx;
        poing_y = poing_obj->screen_y + poing_obj->offset_by;
        DO_NOVA(poing_obj);
        start_pix_gerbe(poing_x, poing_y);
    }*/
}

//2008C
i32 bat_get_eject_sens(obj_t* obj) {
    /* 72344 80196B44 -O2 -msoft-float */
    ray.iframes_timer = 40;
    return bat_dir(obj) ? 1 : -1;
}

