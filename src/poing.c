
//666A0
void fist_U_turn(u8 a1) {
    /* 476F0 8016BEF0 -O2 -msoft-float */
    switch (poing_obj->sub_etat)
    {
        case 1:
            set_sub_etat(poing_obj, 2);
            break;
        case 3:
            set_sub_etat(poing_obj, 4);
            break;
        case 5:
            set_sub_etat(poing_obj, 6);
            break;
        case 8:
            set_sub_etat(poing_obj, 9);
            break;
        case 10:
            set_sub_etat(poing_obj, 11);
            break;
        case 12:
            set_sub_etat(poing_obj, 13);
            break;
    }

    if (a1)
        poing_obj->anim_frame = 7;
}

//66700
void CALC_FIST_POS(void) {
    poing_obj->screen_y = (poing.y_16 >> 4) - ymap;
    poing_obj->screen_x = poing_obj->x + 8 - xmap; // Note: in PS1 version, 8 is not added
}

//6676C
void RAY_THROW_FIST(void) {
    /* 47828 8016C028 -O2 -msoft-float */
    s32 unk_1;
    s16 unk_2;

    if (ray.main_etat == 2 && (ray.sub_etat == 25 || ray.sub_etat == 26))
        set_sub_etat(&ray, 29);
    else if (ray.main_etat == 2 && (ray.sub_etat == 27 || ray.sub_etat == 28))
        set_sub_etat(&ray, 30);
    else if (!(ray.main_etat == 2 && (ray.sub_etat == 17 || ray.sub_etat == 18 || ray.sub_etat == 19)))
        set_sub_etat(&ray, 13);

    if (ray.main_etat == 0) {
        if (ray.flags.flip_x)
            decalage_en_cours -= poing.charge * 16; /* TODO: fixed point? */
        else
            decalage_en_cours += poing.charge * 16;
    }
    poing.is_charging = false;
    poing.is_active = true;
    poing.is_boum = false;
    poing.is_returning = false;
    poing_obj->flags.flip_x = ray.flags.flip_x;
    poing_obj->flags.alive = 1;
    poing_obj->is_active = 1;
    del_alwobj(poing_obj_id);
    add_alwobj(poing_obj);
    poing_obj->speed_y = 0;
    poing_obj->anim_frame = 0;
    poing_obj->cmd_arg_2 = -1;
    set_main_and_sub_etat(poing_obj, poing_obj->init_main_etat, poing_obj->init_sub_etat);
    poing_obj->anim_index = poing_obj->eta[poing_obj->main_etat][poing_obj->sub_etat].anim_index;
    poing_obj->x = ray_zdc_x + (ray_zdc_w >> 1) - poing_obj->offset_bx;
    poing_obj->x -= 5 * (ray.flags.flip_x ? 1 : -1);
    poing.damage = ashr16(poing.charge, 4) + 1;

    switch (poing_obj->init_sub_etat)
    {
        case 12:
            poing.damage += 2;
        case 5:
            poing.speed_x = 11;
            break;
        case 10:
            poing.damage += 2;
        case 3:
            poing.speed_x = 8;
            break;
        case 8:
            poing.damage += 2;
        case 1:
            poing.speed_x = 5;
            break;
    }

    if (ray.flags.flip_x ? decalage_en_cours > 0 : decalage_en_cours < 0)
        poing.speed_x += MAX(Abs(ray.speed_x) - 5, 0);

    poing.speed_x = MIN(poing.speed_x, 16);

    if (poing_obj->flags.flip_x) {
        poing_obj->x += 11;
        poing_obj->speed_x = poing.speed_x;
    } else {
        poing_obj->x -= 11;
        poing_obj->speed_x = -poing.speed_x;
    }
    unk_1 = -40; /* tried applying pattern from fin_poing_follow */
    poing.field_A = (poing.charge * 130 / 64) - (poing.speed_x * 3 + unk_1);
    unk_2 = ashl16(poing.charge, 8);
    poing.charge = unk_2;
    poing.field_4 = unk_2;
    poing.field_A = unk_2 * poing.speed_x / poing.field_A;
    poing_obj->y = ray_zdc_y + (ray_zdc_h >> 1) - ((poing_obj->offset_by + poing_obj->offset_hy) >> 1);
    poing.y_16 = ashl16(poing_obj->y, 4);
    CALC_FIST_POS();
}

//66AF0
void RAY_PREPARE_FIST(void) {
    if (get_eta(&ray)->flags & 2) {
        if (ray.main_etat == 2) {
            eta_t* eta = ray.eta[ray.main_etat];
            u8 anim_speed = (ray.speed_y > 0) ? eta[2].anim_index : eta[0].anim_speed;
            eta[13].anim_speed = (eta[13].anim_speed & 0xF) | (anim_speed & 0xF0);
            poing.charge = 32;
            RAY_THROW_FIST();
            helico_time = -1;
            if (RayEvts.super_helico) {
                button_released = 0;
            }
        } else {
            RAY_STOP();
            set_sub_etat(&ray, 11);
            poing.charge = 5;
            poing.is_charging = 1;
            compteur_attente = 0;
        }
    }
    //stub
}

//66BD0
void RAY_GROW_FIST(void) {
    ++poing.charge;
    if (poing.charge >= 64) {
        poing.charge = 63;
    }
}

//66BF4
void fin_poing_follow(u8 a1) {
    if (poing_obj->cmd_arg_2 != -1) {
        obj_t* unk_obj = level.objects + poing_obj->cmd_arg_2;
        unk_obj->speed_x = 0;
        unk_obj->speed_y = 0;
        poing_obj->cmd_arg_2 = -1;
        if (a1 || !(poing_obj->flags.alive)) {
            unk_obj->x = ray.x + ray.offset_bx - unk_obj->offset_bx;
            i32 ray_y = ray_zdc_y + (ray_zdc_h >> 1);
            i32 unk_y = ((unk_obj->offset_by + unk_obj->offset_hy) >> 1) + 5;
            unk_obj->y = ray_y - unk_y;
        } else {
            unk_obj->x = unk_obj->init_x;
            unk_obj->y = unk_obj->init_y;
        }
    }
}

//66CA0
void POING_FOLLOW(obj_t* obj) {
    /* 47F1C 8016C71C -O2 -msoft-float */
    obj_t* follow_obj = &level.objects[poing_obj->cmd_arg_2];

    follow_obj->speed_x = poing_obj->speed_x;
    follow_obj->speed_y = poing_obj->speed_y;
    if (!(ray.iframes_timer == -1 && follow_obj->is_active)) {
        fin_poing_follow(0);
    }
}

//66CEC
void alter_fist_speed(obj_t* obj) {
    /* 47F9C 8016C79C -O2 -msoft-float */
    u8 accel_x;
    s32 abs_spd_x = Abs(obj->speed_x);

    if (abs_spd_x > 10)
        accel_x = 2;
    else if (abs_spd_x > 7)
        accel_x = 1;
    else
        accel_x = horloge[2] != 0;

    if (obj->flags.flip_x)
        obj->speed_x -= accel_x;
    else
        obj->speed_x += accel_x;
}

//66D54
void switch_off_fist(void) {
    poing.is_active = 0;
    poing.is_boum = 0;
    poing.charge = 5;
    poing.is_returning = 0;
    poing_obj->flags.alive = 0;
    poing_obj->is_active = 0;
    poing_obj->sub_etat = poing_obj->init_sub_etat;
    fin_poing_follow(0);
}

//66DA4
void DO_POING(obj_t* obj) {
    s32 ray_diff_x; s32 ray_diff_y;
    s16 unk_x_1; s32 unk_y_1;
    s32 unk_1;

    if (poing.is_active)
    {
        if (obj->speed_x != 0)
        {
            CALC_FIST_POS();
            obj->y = poing.y_16 >> 4;
            if (poing.charge > 0)
            {
                if (poing.charge < poing.field_A)
                    poing.charge = 0;
                else
                    poing.charge -= poing.field_A;

                if (poing.charge == 0)
                    fist_U_turn(false);
            } else if (poing.is_returning == 0) {
                alter_fist_speed(obj);
            } else if (poing.is_returning == 1) {
                if (Abs(obj->speed_x) < poing.speed_x) {
                    alter_fist_speed(obj);
                }

                ray_diff_x = ray_zdc_x + (ray_zdc_w >> 1) - obj->offset_bx;
                ray_diff_x += 11 * (obj->flags.flip_x ? 1 : -1);

                if (ray.main_etat == 4)
                    ray_diff_x += 11 * (obj->flags.flip_x ? 1 : -1);

                ray_diff_y =
                        (ray_zdc_y + (ray_zdc_h >> 1) -
                         ((obj->offset_by + obj->offset_hy) >> 1)) * 16;

                /* TODO: also a macro? see RAY_THROW_FIST */
                if (
                        obj->flags.flip_x ?
                        ray_diff_x >= obj->x :
                        ray_diff_x <= obj->x
                        )
                    switch_off_fist();
                else
                {
                    unk_x_1 = ray_diff_x - obj->x;
                    unk_y_1 = ray_diff_y - poing.y_16;
                    if (unk_x_1 != 0)
                        unk_y_1 = unk_y_1 / unk_x_1;
                    obj->speed_y = unk_y_1 * obj->speed_x;

                    unk_1 = obj->speed_y + poing.y_16;
                    if (
                            unk_1 >= MIN(poing.y_16, ray_diff_y) &&
                            unk_1 <= MAX(poing.y_16, ray_diff_y)
                            )
                        obj->speed_y >>= 4;
                    else
                        obj->speed_y = (ray_diff_y - poing.y_16) >> 4;
                }
            }

            // NOTE: this happens first instead of last in the PS1 version (maybe a possible bug fix?)
            if (!poing.is_boum && poing.is_active) {
                DO_POING_COLLISION();
            }
        }
        else
        {
            poing.is_returning = true;
            if (obj->flags.flip_x)
                obj->speed_x--;
            else
                obj->speed_x++;
        }

        if (obj->cmd_arg_2 != -1) {
            POING_FOLLOW(obj);
        }
    }
}

//66FF8
void allocatePoingBoum(void) {
    //stub
}

