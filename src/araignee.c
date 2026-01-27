
//1C0C0
void get_spi_zdc(obj_t* obj, s16* x, s16* y, s16* w, s16* h) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    u8 eta_flags = obj->eta[obj->main_etat][obj->sub_etat].flags;

    if (eta_flags & 0x40) {
        *x = obj->x + 69;
        *y = obj->y + 38;
        *w = 21;
        *h = 30;
    } else if (eta_flags & 0x80) {
        *x = obj->x + 69;
        *y = obj->y + 86;
        *w = 34;
        *h = 10;
    } else {
        *x = obj->x + 71;
        *y = obj->y + 71;
        *w = 29;
        *h = 19;
    }
}

//1C160
void DO_SPIDER_PLAFOND(obj_t* spipl_obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    u8 sub_etat;
#ifdef PS1
    obj_t *poing_obj;
#endif
    s16 poing_x; s16 poing_y_1; s16 poing_y_2;
    s16 spipl_x; s16 spipl_y_1; s16 spipl_y_2;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    s16 spipl_spr_x; s16 spipl_spr_y; s16 spipl_spr_w; s16 spipl_spr_h;
    u8 new_flip;
    s16 cen_x = get_center_x(spipl_obj);
    s16 cen_y = get_center_y(spipl_obj);

    if (
        (
            spipl_obj->main_etat == 0 &&
            (sub_etat = spipl_obj->sub_etat, sub_etat == 27 || sub_etat == 24 || sub_etat == 30 || sub_etat == 11)
        ) ||
        (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2)
    ) {
        if (poing.is_active) {
#ifdef PS1
            poing_obj = &level.objects[poing_obj_id];
#endif
            poing_x = poing_obj->x + poing_obj->offset_bx;
            poing_y_1 = poing_obj->y + poing_obj->offset_by;
            poing_y_2 = poing_obj->y + poing_obj->offset_hy;

            spipl_x = spipl_obj->x + spipl_obj->offset_bx;
            spipl_y_1 = spipl_obj->y + spipl_obj->offset_by + 5;
            spipl_y_2 = spipl_obj->y + spipl_obj->offset_hy - 5;
            if (
                (poing_y_1 <= spipl_y_1 || poing_y_2 <= spipl_y_1) &&
                (poing_y_1 >= spipl_y_2 || poing_y_2 >= spipl_y_2) &&
                (poing_x <= spipl_x + 60) && (poing_x >= spipl_x - 60)
            ) {
                spipl_obj->speed_x = 0;
                spipl_obj->speed_y = 0;
                set_main_and_sub_etat(spipl_obj, 0, 25);
            }
        }
    } else if (spipl_obj->main_etat == 0 && spipl_obj->sub_etat == 26 && !poing.is_active) {
        set_sub_etat(spipl_obj, 27);
    }

    if (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2) {
        SET_X_SPEED(spipl_obj);
        if (block_flags[calc_typ_travd(spipl_obj, false)] >> BLOCK_FULLY_SOLID & 1) {
            spipl_obj->flags.flip_x = 1 - spipl_obj->flags.flip_x;
            set_main_and_sub_etat(spipl_obj, 0, 24);
            SET_X_SPEED(spipl_obj);
        }
        CALC_MOV_ON_BLOC(spipl_obj);
    }

    if (on_block_chdir(spipl_obj, cen_x, cen_y) && test_allowed(spipl_obj, cen_x, cen_y)) {
        if (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2) {
            set_main_and_sub_etat(spipl_obj, 0, 21);
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = 0;
            spipl_obj->flags.flip_x = 1 - spipl_obj->flags.flip_x;
        } else if (
            spipl_obj->main_etat == 2 &&
            (spipl_obj->sub_etat == 3 || spipl_obj->sub_etat == 1 || spipl_obj->sub_etat == 2)
        ) {
            set_main_and_sub_etat(spipl_obj, 0, 12);
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = 0;
        }
    } else {
        if (spipl_obj->main_etat == 2 && spipl_obj->sub_etat == 1 &&
            spipl_obj->anim_frame == 0
        ) {
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = -10;
        }
        else if (
            spipl_obj->main_etat == 0 && spipl_obj->sub_etat == 20 &&
            spipl_obj->anim_frame == 27
        ) {
            i = 0;
            cur_obj = &level.objects[i];
            nb_objs = level.nb_objects;
            while (i < nb_objs) {
                if ((cur_obj->type == TYPE_DARD_PLAFOND) && !cur_obj->is_active) {
                    GET_SPRITE_POS(spipl_obj, 2, &spipl_spr_x, &spipl_spr_y, &spipl_spr_w, &spipl_spr_h);
                    cur_obj->speed_x = 0;
                    cur_obj->speed_y = 32;
                    cur_obj->cmd_offset = -1;
                    cur_obj->flags.flip_x = 0;

                    cur_obj->init_x =
                    cur_obj->x =
                            spipl_spr_x - cur_obj->offset_bx + (spipl_spr_w >> 1);

                    cur_obj->init_y =
                    cur_obj->y =
                            spipl_spr_y - cur_obj->offset_by + (spipl_spr_h >> 1);

                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
#ifdef PC
                    add_alwobj(cur_obj);
#endif
                    cur_obj->flags.read_commands = 1;
                    cur_obj->nb_cmd = 0;
                    if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                        position_ray = true;
                    else
                        position_ray = false;

                    DO_DARD_PLAFOND_ALWAYS(cur_obj);
                    break;
                }
                cur_obj++;
                i++;
            }
        }
    }

    if (spipl_obj->main_etat == 0 &&
        (sub_etat = spipl_obj->sub_etat, sub_etat == 20 || sub_etat == 13 || sub_etat == 23)
    ) {
        new_flip = ray.x > spipl_obj->x;
        spipl_obj->flags.flip_x = new_flip;
    }
}

//1C588
void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s32 main_etat;

    if (--obj->hit_points == 0) {
        if (
            (
                obj->main_etat == 0 &&
                (obj->sub_etat == 12 || obj->sub_etat == 13 ||
                 obj->sub_etat == 23 || obj->sub_etat == 20)
            ) ||
            (
                obj->main_etat == 2 &&
                (obj->sub_etat == 2 || obj->sub_etat == 3 ||
                 obj->sub_etat == 28 || obj->sub_etat == 7)
            )
        ) {
            set_main_and_sub_etat(obj, 0, 19);
        } else {
            set_main_and_sub_etat(obj, 0, 18);
        }
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
    } else {
        main_etat = obj->main_etat;
        if (main_etat == 1 && obj->sub_etat == 2) {
            /* this, or goto? */
            set_main_and_sub_etat(obj, 0, 14);
            obj->speed_y = 0;
            obj->speed_x = 0;
        } else if (main_etat == 0) {
            /*
            using a switch would generate rodata
            using a var for obj->sub_etat doesn't seem to work either
            */
            if (obj->sub_etat == 11 || obj->sub_etat == 29 ||
                obj->sub_etat == 25 || obj->sub_etat == 26 ||
                obj->sub_etat == 27 || obj->sub_etat == 24 ||
                obj->sub_etat == 30
            ) {
                set_main_and_sub_etat(obj, 0, 14);
                obj->speed_y = 0;
                obj->speed_x = 0;
            } else if (
                    obj->sub_etat == 12 || obj->sub_etat == 13 ||
                    obj->sub_etat == 23 || obj->sub_etat == 20
            ) {
                if (obj->x < ray.x) {
                    obj->flags.flip_x = 1;
                } else {
                    obj->flags.flip_x = 0;
                }
                set_main_and_sub_etat(obj, 0, 17);
            }
        }
    }
}

//1C6A0
void SPIDER_PLAFOND_REACT(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s32 prev_flip;
    u8 new_flip;

    if (obj->main_etat == 0 && (obj->sub_etat == 12 || obj->sub_etat == 13))
        set_sub_etat(obj, 20);
    else if (
        (obj->main_etat == 0 && (obj->sub_etat == 11 || obj->sub_etat == 30)) ||
        (obj->main_etat == 1 && obj->sub_etat == 2)
    ) {
        prev_flip = obj->flags.flip_x;
        prev_flip &= 1;
        if ((ray.x > obj->x && prev_flip != true) || /* ??? */
            (ray.x <= obj->x && prev_flip != false)
        ) {
            new_flip = ray.x > obj->x;
            obj->flags.flip_x = new_flip;
            obj->speed_x = 0;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 24);
        }
    }
}

//1C75C
void DO_DARD_PLAFOND_ALWAYS(obj_t* obj) {
    // NOTE(Falcury): PS1 non-matching function, needing cleanup!!
    print_once("Not implemented: DO_DARD_PLAFOND_ALWAYS"); //stub
}

//1CAC4
void DO_SPIDER_COMMAND(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (obj->sub_etat == 7) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    if ((obj->main_etat == 0) && (obj->sub_etat == 0))
        obj->link = 0;

    switch (obj->cmd) {
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;

            SET_X_SPEED(obj);
            obj->param = 1;
            if (obj->main_etat == 0) {
                set_main_and_sub_etat(obj, 1, 0);
            }

            if (obj->main_etat == 1) {
                if (block_flags[calc_typ_travd(obj, false)] & 1) {
                    if (obj->cmd == GO_LEFT) {
                        obj->flags.flip_x = 1;
                        skipToLabel(obj, 2, true);
                    } else {
                        obj->flags.flip_x = 0;
                        skipToLabel(obj, 0, true);
                    }
                }
                SET_X_SPEED(obj);
                CALC_MOV_ON_BLOC(obj);
            }
            break;
        case GO_WAIT:
            switch (obj->main_etat) {
                case 0:
                    if (obj->sub_etat == 4)
                        obj->link = 0;

                    if (obj->sub_etat == 8)
                        TEST_FIN_BLOC(obj);
                    break;
                case 1:
                    if (!(block_flags[calc_typ_travd(obj, false)] >> BLOCK_FULLY_SOLID & 1))
                        SET_X_SPEED(obj);
                    else
                        obj->speed_x = 0;

                    CALC_MOV_ON_BLOC(obj);
            }
            break;
    }
}

//1CC24
void DO_SPIDER_TIR(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    DO_SPIDER_COMMAND(obj);
    if ((obj->main_etat == 0) && (obj->sub_etat == 3) && (obj->link == 0)) {
        allocateDard(obj);
        obj->link = 1;
    }
}

//1CC50
void allocateDard(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_66_DARD && !cur_obj->is_active) {
            cur_obj->flags.flip_x = obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (cur_obj->flags.flip_x) {
                cur_obj->speed_x = -3;
            } else {
                cur_obj->speed_x = 3;
            }

            s16 spi_x; s16 spi_y; s16 spi_w; s16 spi_h;
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
#ifdef PC
            add_alwobj(cur_obj);
#endif
            cur_obj->anim_frame = 9;
            break;
        }
    }
}

//1CDB0
void DoSpiderPoingCollision(obj_t* other_obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    obj_hurt(other_obj);
    if (other_obj->hit_points == 0) {
        set_main_and_sub_etat(other_obj, 0, 7);
        other_obj->anim_frame = 0;
        other_obj->flags.read_commands = 0;
    } else {
        if (other_obj->main_etat == 0 &&
            (other_obj->sub_etat == 2 || other_obj->sub_etat == 3 ||
             other_obj->sub_etat == 4 || other_obj->sub_etat == 5)
        ) {
            other_obj->cmd_context_depth = -1;
        }

        other_obj->param = 0;
        if (level.objects[poing_obj_id].speed_x > 0) {
            skipToLabel(other_obj, 3, true);
            other_obj->flags.flip_x = 1;
        } else {
            skipToLabel(other_obj, 1, true);
            other_obj->flags.flip_x = 0;
        }
        pushToLabel(other_obj, 6, true);
        other_obj->anim_frame = 0;
        pushToLabel(other_obj, 7, true);
    }
}

//1CE70
void DoSpiderRaymanZDD(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_RAY_IN_ZONE(). Seems to be functionally identical.
    if (obj->main_etat == 1 && obj->sub_etat == 0 && obj->link == 0) {
        obj->anim_frame = 0;
        if (!(obj->flags.flip_x)) {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 3, true);
        } else {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 1, true);
        }
        pushToLabel(obj, 5, true);
    }
}

