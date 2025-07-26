
//52D30
void mite_esquive_poing(obj_t* obj, i16* a2) {
    //stub
}

//52E64
void HAS_MIT_JUMP(obj_t* obj) {
    //stub
}

//52ED0
void ray_over_mit(obj_t* obj, i16 a2) {
    //stub
}

//52EF4
void fix_mit_Xspeed(obj_t* obj, i16 a2) {
    //stub
}

//53068
void DO_MIT_ATTAK(obj_t* obj) {
    //stub
}

//53320
void IS_MIT_PAF(obj_t* obj) {
    //stub
}

//5342C
void DO_MIT_COMMAND(obj_t* obj) {
    //stub
}

//53838
void DoMitePoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//5387C
void DO_MITE2_COMMAND(obj_t* obj) {
    //stub
}

//53A04
void DoMite2PoingCollision(obj_t* obj, i16 a2) {
    //stub
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

