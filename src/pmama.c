
//643F0
void pmamaFollowsShip(obj_t* obj) {
    print_once("Not implemented: pmamaFollowsShip"); //stub
}

//64440
void init_couteaux(void) {
    print_once("Not implemented: init_couteaux"); //stub
}

//64500
void reset_couteax(void) {
    print_once("Not implemented: reset_couteax"); //stub
}

//6451C
void check_couteaux(void) {
    print_once("Not implemented: check_couteaux"); //stub
}

//64568
i8 find_couteau(obj_t* obj) {
    print_once("Not implemented: find_couteau");
    return -1; //stub
}

//64574
i32 x_pos(u8 a1, u8 a2) {
    print_once("Not implemented: x_pos");
    return 0; //stub
}

//645CC
i32 y_pos(i16 a1, i16 a2) {
    print_once("Not implemented: y_pos");
    return 0; //stub
}

//645E0
i32 y_floor(i16 a1, i16 a2) {
    print_once("Not implemented: y_floor");
    return 0; //stub
}

//64650
void init_move_couteau(obj_t* obj) {
    print_once("Not implemented: init_move_couteau"); //stub
}

//647CC
void init_lance_couteau(u8 a1) {
    print_once("Not implemented: init_lance_couteau"); //stub
}

//64A80
void couteau_frame(i16 a1, i16 a2) {
    print_once("Not implemented: couteau_frame"); //stub
}

//64B14
void update_couteau(obj_t* obj) {
    print_once("Not implemented: update_couteau"); //stub
}

//64B70
void get_cou_zdc(obj_t* obj, i16 *x, i16 *y, i16 *w, i16 *h) {
    /* 268E0 8014B0E0 -O2 -msoft-float */
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    u8 sub_etat;

    GET_SPRITE_POS(obj, 0, &spr_x, &spr_y, &spr_w, &spr_h);
    *w = 6;
    *h = 6;
    if (obj->main_etat == 2)
    {
        sub_etat = obj->sub_etat;
        if (sub_etat - 10 <= 1U)
        {
            switch (obj->anim_frame)
            {
                case 0:
                    *x = spr_x + ((spr_w - *w) >> 1);
                    *y = spr_y + spr_h - *h;
                    return;
                case 1:
                case 2:
                case 3:
                    *x = spr_x + spr_w - *w;
                    *y = spr_y + spr_h - *h;
                    return;
                case 4:
                    *w = spr_w;
                    *x = spr_x + spr_w - *w;
                    *y = spr_y + ((spr_h - *h) >> 1);
                    return;
                case 5:
                case 6:
                case 7:
                    *x = spr_x + spr_w - *w;
                    *y = spr_y;
                    return;
                case 8:
                    *x = spr_x + ((spr_w - *w) >> 1);
                    *y = spr_y;
                    return;
                case 9:
                case 10:
                case 11:
                    *x = spr_x;
                    *y = spr_y;
                    return;
                case 12:
                    *w = spr_w;
                    *x = spr_x;
                    *y = spr_y + ((spr_h - *h) >> 1);
                    return;
                case 13:
                case 14:
                case 15:
                    *x = spr_x;
                    *y = spr_y + spr_h - *h;
                    return;
                default:
                    *x = spr_x + ((spr_w - *w) >> 1);
                    *y = spr_y + spr_h - *h;
                    return;
            }
        }
        else
        {
            if (obj->sub_etat == 14 && !(obj->eta[2][obj->sub_etat].anim_speed & 0xf))
            {
                *h = spr_h;
                *x = spr_x + ((spr_w - *w) >> 1);
                *y = spr_y;
                return;
            }
            if (obj->main_etat == 2)
            {
                if (
                        (obj->sub_etat == 13) ||
                        (obj->sub_etat == 14 && obj->eta[2][obj->sub_etat].anim_speed & 0xf)
                        )
                {
                    *x = -((*w >> 1) + -0x80) + obj->x;
                    *y = -((*h >> 1) + -0x60) + obj->y;
                    return;
                }
                if (obj->main_etat == 2 && obj->sub_etat == 12)
                {
                    *x = spr_x + ((spr_w - *w) >> 1);
                    *y = spr_y + spr_h - *h;
                    return;
                }
            }
        }
    }
    *x = -*w;
    *y = -*h;
}

//64EA4
void pma_attaque_suivante(void) {
    print_once("Not implemented: pma_attaque_suivante"); //stub
}

//64FF8
i16 convertspeed(i16 x) {
    return ashl16(x, 4);
}

//65000
void lance_couteau_parabolique(obj_t* obj) {
    print_once("Not implemented: lance_couteau_parabolique"); //stub
}

//65120
void lance_couteau_droit(obj_t* obj) {
    print_once("Not implemented: lance_couteau_droit"); //stub
}

//652EC
void retour_couteau(obj_t* obj) {
    print_once("Not implemented: retour_couteau"); //stub
}

//654F8
void lance_couteau_lineaire(obj_t* obj) {
    print_once("Not implemented: lance_couteau_lineaire"); //stub
}

//65B10
void DO_COU_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_COU_COMMAND"); //stub
}

//65D00
void calc_pma_dir(obj_t* obj) {
    print_once("Not implemented: calc_pma_dir"); //stub
}

//65D90
void DO_PMA_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_PMA_COMMAND"); //stub
}

//66170
void init_mama_pirate(obj_t* obj) {
    print_once("Not implemented: init_mama_pirate"); //stub
}

//66218
void PMA_SORT_DU_CANON(obj_t* obj) {
    print_once("Not implemented: PMA_SORT_DU_CANON"); //stub
}

//66244
void DO_PMA_POING_COLLISION(obj_t* obj, i16 sprite) {
    print_once("Not implemented: DO_PMA_POING_COLLISION"); //stub
}

//6628C
i32 pma_get_eject_sens(obj_t* obj) {
    /* 29118 8014D918 -O2 -msoft-float */
    ray.iframes_timer = 40;
    return -1;
}

//662A0
void DO_COU_ATTER(obj_t* obj) {
    print_once("Not implemented: DO_COU_ATTER"); //stub
}

//663B0
void DO_PMA_ATTER(obj_t* obj) {
    print_once("Not implemented: DO_PMA_ATTER"); //stub
}
