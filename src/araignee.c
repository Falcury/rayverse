
//1C0C0
void get_spi_zdc(obj_t* obj, i16* x, i16* y, i16* w, i16* h) {
    /* 537C0 80177FC0 -O2 -msoft-float */
    u8 eta_flags = obj->eta[obj->main_etat][obj->sub_etat].flags;

    if (eta_flags & 0x40)
    {
        *x = obj->x + 69;
        *y = obj->y + 38;
        *w = 21;
        *h = 30;
    }
    else if (eta_flags & 0x80)
    {
        *x = obj->x + 69;
        *y = obj->y + 86;
        *w = 34;
        *h = 10;
    }
    else
    {
        *x = obj->x + 71;
        *y = obj->y + 71;
        *w = 29;
        *h = 19;
    }
}

//1C160
void DO_SPIDER_PLAFOND(obj_t* obj) {
    print_once("Not implemented: DO_SPIDER_PLAFOND"); //stub
}

//1C588
void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, i16 sprite) {
    print_once("Not implemented: DO_SPIDER_PLAFOND_POING_COLLISION"); //stub
}

//1C6A0
void SPIDER_PLAFOND_REACT(obj_t* obj) {
    print_once("Not implemented: SPIDER_PLAFOND_REACT"); //stub
}

//1C75C
void DO_DARD_PLAFOND_ALWAYS(obj_t* obj) {
    print_once("Not implemented: DO_DARD_PLAFOND_ALWAYS"); //stub
}

//1CAC4
void DO_SPIDER_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_SPIDER_COMMAND"); //stub
}

//1CC24
void DO_SPIDER_TIR(obj_t* obj) {
    print_once("Not implemented: DO_SPIDER_TIR"); //stub
}

//1CC50
void allocateDard(obj_t* obj) {
    /* 395B0 8015DDB0 -O2 -msoft-float */
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_66_DARD && !cur_obj->is_active) {
            cur_obj->flags.flip_x = obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (cur_obj->flags.flip_x) {
                cur_obj->speed_x = -3;
            } else {
                cur_obj->speed_x = 3;
            }

            i16 spi_x; s16 spi_y; s16 spi_w; s16 spi_h;
            GET_SPRITE_POS(obj, 3, &spi_x, &spi_y, &spi_w, &spi_h);
            cur_obj->x = spi_x + (spi_w >> 1) - cur_obj->offset_bx;
            cur_obj->y = spi_y + (spi_h >> 1) - cur_obj->offset_by;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            cur_obj->display_prio = 4;
            cur_obj->main_etat = 0;
            cur_obj->sub_etat = 9;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            cur_obj->flags.flag_0x40 = 0;
            add_alwobj(cur_obj);
            cur_obj->anim_frame = 9;
            break;
        }
    }
}

//1CDB0
void DoSpiderPoingCollision(obj_t* obj, i16 sprite) {
    print_once("Not implemented: DoSpiderPoingCollision"); //stub
}

//1CE70
void DoSpiderRaymanZDD(obj_t* obj) {
    print_once("Not implemented: DoSpiderRaymanZDD"); //stub
}

