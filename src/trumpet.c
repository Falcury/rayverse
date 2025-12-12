
//7E420
void DO_TRP_COMMAND(obj_t* obj) {
    /* 493E0 8016DBE0 -O2 -msoft-float */
    if (obj->main_etat == 0 && obj->sub_etat == 1)
    {
        obj->speed_x = 0;
        obj->speed_y = -8;
    }
    else
    {
        switch (obj->cmd)
        {
            case GO_SPEED:
                obj->speed_x = obj->iframes_timer;
                obj->speed_y = obj->cmd_arg_2;
                break;
            case GO_LEFT:
            case GO_RIGHT:
                if (obj->cmd == GO_LEFT)
                    obj->flags.flip_x = 0;
                else
                    obj->flags.flip_x = 1;

                if (
                        (obj->main_etat == 0 && obj->sub_etat == 0) ||
                        (obj->main_etat == 1 && obj->sub_etat == 0)
                        )
                {
                    set_main_and_sub_etat(obj, 1, 0);
                    SET_X_SPEED(obj);
                    CALC_MOV_ON_BLOC(obj);
                }
                break;
            case GO_WAIT:
                CALC_MOV_ON_BLOC(obj);
                break;
            case GO_UP:
            case GO_DOWN:
            case GO_SUBSTATE:
            case GO_SKIP:
            case GO_ADD:
            case GO_STATE:
            case GO_PREPARELOOP:
            case GO_DOLOOP:
            case GO_LABEL:
            case GO_GOTO:
            case GO_GOSUB:
            case GO_RETURN:
            case GO_BRANCHTRUE:
            case GO_BRANCHFALSE:
            case GO_TEST:
            case GO_SETTEST:
            case GO_WAITSTATE:
            case GO_X:
            case GO_Y:
            case RESERVED_GO_SKIP:
            case RESERVED_GO_GOTO:
            case RESERVED_GO_GOSUB:
            case RESERVED_GO_GOTOT:
            case RESERVED_GO_GOTOF:
            case RESERVED_GO_SKIPT:
            case RESERVED_GO_SKIPF:
            case GO_NOP:
            default:
                break;
        }
    }

    DO_TRP_ATTAK(obj);
}

//7E4C8
void trompetteWind(obj_t* obj, u8 a2) {
    /* 3A004 8015E804 -O2 -msoft-float */
    s16 trp_x; s16 trp_y; s16 trp_w; s16 trp_h;
    s16 unk_1;

    obj->timer = 100;
    GET_ANIM_POS(obj, &trp_x, &trp_y, &trp_w, &trp_h);
    if (obj->flags.flip_x)
        trp_x -= 16 - trp_w;
    else
        trp_x -= 76 + trp_w;

    trp_w += 92;
    trp_h = ashr16(trp_h, 1) + 2;
    trp_y += ashr16(trp_h, 2);
    if (inter_box(ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, trp_x, trp_y, trp_w, trp_h)) {
        ashr16(ray_zdc_w, 1);
        ashr16(ray_zdc_h, 1);
        unk_1 = 7;
        if ((!(obj->flags.flip_x) && a2 == false) || (obj->flags.flip_x && a2 == true))
            unk_1 = -unk_1;
    }
    else
        unk_1 = 0;

    if (unk_1 != 0) {
        ray_wind_force += unk_1;
        if (ray.main_etat != 5)
            decalage_en_cours = ashl16(ray_wind_force, 8);
    }
}

//7E650
void DO_TRP_ATTAK(obj_t* obj) {
    /* 3A1CC 8015E9CC -O2 -msoft-float */
    u8 sub_etat;

    if (obj->timer != 0)
        obj->timer--;

    if (obj->main_etat == 0) {
        sub_etat = obj->sub_etat;
        if ((sub_etat == 2 && obj->anim_frame >= 10) || sub_etat == 4)
            trompetteWind(obj, false);
        else if ((sub_etat == 6 && obj->anim_frame >= 10) || sub_etat == 7)
            trompetteWind(obj, true);
    }
}

//7E69C
void trompetteAtter(obj_t* obj) {
    /* 2D9EC 801521EC -O2 -msoft-float */
    skipToLabel(obj, 8, true);
    recale_position(obj);
}

//7E6BC
void DoTrumpetPoingCollision(obj_t* obj, i16 sprite) {
    obj_hurt(obj);
    if (poing_obj->speed_x > 0)
        obj->flags.flip_x = 0;
    else if (poing_obj->speed_x < 0)
        obj->flags.flip_x = 1;

    if (obj->hit_points == 0) {
        set_main_and_sub_etat(obj, 0, 1);
        obj->flags.read_commands = 0;
        obj->cmd = GO_WAIT;
    } else {
        obj->y -= 2;
        skipToLabel(obj, 4, true);
    }
}

//7E72C
void DoTrompetteRaymanZDD(obj_t* obj) {
    if (obj->timer == 0 && ((obj->main_etat == 0 && obj->sub_etat == 0) || (obj->main_etat == 1 && obj->sub_etat == 0))) {
        skipToLabel(obj, 5, true);
    }
}
