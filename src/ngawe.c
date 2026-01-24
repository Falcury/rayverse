
//5D630
void ngaweFollowsShip(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (bateau_obj_id != -1) {
        snapToSprite(obj, &level.objects[bateau_obj_id], 2, 21, -51);
    }
}

//5D670
void ngaweTriesToGrabShip(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 ship_x; s16 ship_y; s16 ship_w; s16 ship_h;

    if (bateau_obj_id != -1) {
        GET_SPRITE_POS(&level.objects[bateau_obj_id], 2, &ship_x, &ship_y, &ship_w, &ship_h);
        obj->x = ship_x + 21;
        ship_h = ship_y - 67;
        if ((obj->y >= ship_h - obj->speed_y) && (obj->y <= ship_h + obj->speed_y))
            set_main_and_sub_etat(obj, 2, 2);
    }
}

//5D714
void allocatePirateNgaweRing(obj_t* ngw_obj, s16 spd_y, u8 param_3) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 ngw_x; s16 ngw_y; s16 ngw_w; s16 ngw_h;
    s16 unk_1;
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_RING && !cur_obj->is_active) {
            if (param_3)
                cur_obj->flags.flip_x = ngw_obj->flags.flip_x;
            else
                cur_obj->flags.flip_x = ngw_obj->flags.flip_x ^ 1;

            cur_obj->speed_y = spd_y;

            GET_SPRITE_POS(ngw_obj, 6, &ngw_x, &ngw_y, &ngw_w, &ngw_h);
            cur_obj->x = ngw_x - cur_obj->offset_bx;
            if (ngw_obj->flags.flip_x)
                cur_obj->x += ngw_w;

            cur_obj->y = ngw_y - ((cur_obj->offset_hy + cur_obj->offset_by) >> 1);

            skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
#ifdef PC
            add_alwobj(cur_obj);
#endif
            cur_obj->flags.flag_0x40 = 0;

            /* see FLIP_X_SGN macro? */
            unk_1 =
                    (cur_obj->flags.flip_x ? 1 : -1) *
                    (s16) (ray.x + ray.offset_bx - cur_obj->x - cur_obj->offset_bx);
            unk_1 = MAX(unk_1, 0) + 15;
            cur_obj->link = unk_1;

            cur_obj->flags.hurtbyfist = 0;
            cur_obj->flags.command_test = 0;
            cur_obj->param = ngw_obj->id;
            ngw_obj->config--;
            break;
        }
        cur_obj++;
        i++;
    }
}

//5D8CC
void DO_NGW_TIR(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->anim_frame < 26)
        obj->link = 0;

    if (obj->link == 0 && obj->anim_frame == 26 && horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0) {
        allocatePirateNgaweRing(obj, 0, true);
        obj->link = 1;
    }
}

//5D934
void NGW_REACT_TO_RAY_IN_ZONE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if ((
            (obj->main_etat == 0 && obj->sub_etat == 0) ||
            (obj->main_etat == 1 && obj->sub_etat == 0)
        ) && obj->config != 0
    ) {
        set_main_and_sub_etat(obj, 0, 2);
    }
}

//5D964
void DO_ONE_NGW_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s32 prev_flip_x = obj->flags.flip_x;

    calc_obj_dir(obj);
    switch (obj->main_etat * 0x100 + obj->sub_etat)
    {
        case 0x3:
            obj->speed_y = -8;
            obj->speed_x = 0;
            break;
        case 0x205:
        case 0x5:
            obj->speed_x = 0;
            break;
        case 0x0:
            obj->speed_x = 0;
            if (!poing.is_active) {
                if (obj->config == 1) {
                    if (obj->flags.flip_x != prev_flip_x) {
                        set_sub_etat(obj, 6);
                    } else {
                        set_main_and_sub_etat(obj, 1, 0);
                    }
                } else {
                    obj->flags.flip_x = prev_flip_x;
                }
            }
            else
                set_main_and_sub_etat(obj, 0, 7);
            break;
        case 0x2:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (!poing.is_active) {
                DO_NGW_TIR(obj);
            } else {
                set_main_and_sub_etat(obj, 0, 7);
            }
            break;
        case 0x100:
            if (!poing.is_active) {
                if (obj->config == 1) {
                    if ((block_flags[calc_typ_travd(obj, true)] >> BLOCK_FULLY_SOLID & 1) ||
                        !((u8) block_flags[
                                BTYP((obj->x + obj->offset_bx + obj->speed_x) >> 4, (obj->y + obj->offset_by) >> 4)
                        ] >> BLOCK_SOLID & 1)
                    ) {
                        if (prev_flip_x == obj->flags.flip_x) {
                            prev_flip_x = obj->flags.flip_x ^ 1;
                        }
                    }

                    if (obj->flags.flip_x != prev_flip_x) {
                        set_main_and_sub_etat(obj, 0, 6);
                    }

                    SET_X_SPEED(obj);
                    CALC_MOV_ON_BLOC(obj);
                } else {
                    set_main_and_sub_etat(obj, 0, 0);
                }
            } else {
                set_main_and_sub_etat(obj, 0, 7);
            }
            break;
        case 0x207:
            ngaweTriesToGrabShip(obj);
            break;
        case 0x202:
        case 0x203:
        case 0x204:
            ngaweFollowsShip(obj);
            break;
        case 0x6:
        case 0x4:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (poing.is_active) {
                set_main_and_sub_etat(obj, 0, 7);
            }
            break;
        case 0x8:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (!poing.is_active)
                set_sub_etat(obj, 4);
            break;
        case 0x7:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            break;
        case 0x1:
            obj->timer = 100;
            obj->speed_x = 0;
            break;
    }

    if (obj->timer != 0) {
        obj->timer--;
    }
}

//5DC90
void DO_NGW_POING_COLLISION(obj_t* ngw_obj, s16 sprite) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
#ifdef PS1
    obj_t *poing_obj = &level.objects[poing_obj_id];
#endif

    if (ngw_obj->timer == 0) {
        poing.damage = 1;
        obj_hurt(ngw_obj);
        if (ngw_obj->hit_points == 0) {
            set_main_and_sub_etat(ngw_obj, 0, 3);
            if (bateau_obj_id != -1) {
                skipOneCommand(&level.objects[bateau_obj_id]);
                level.objects[bateau_obj_id].nb_cmd = 0;
            }
        } else {
            if (poing_obj->speed_x > 0)
                ngw_obj->flags.flip_x = 0;
            else if ((poing_obj->speed_x >= 0) && poing_obj->flags.flip_x)
                ngw_obj->flags.flip_x = 0;
            else
                ngw_obj->flags.flip_x = 1;
            set_main_and_sub_etat(ngw_obj, 0, 1);
        }
    }
}

//5DD58
void DO_ONE_NGW_RING_COMMAND(obj_t* ring_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    obj_t *unk_obj = &level.objects[ring_obj->param];

    if (!(unk_obj->is_active)) {
        ring_obj->flags.alive = 0;
        DO_NOVA(ring_obj);
        return;
    }

    if (ring_obj->link <= 0) {
        if (ring_obj->flags.hurtbyfist) {
            ring_obj->is_active = 0;
            ring_obj->flags.alive = 0;
            unk_obj->config++;
        } else {
            ring_obj->flags.hurtbyfist = 1;
            ring_obj->flags.command_test = 1;
            ring_obj->link = Abs((s16) ((ring_obj->offset_bx + ring_obj->x) - unk_obj->x - unk_obj->offset_bx));
        }
        return;
    }

    ring_obj->flags.command_test = 0;
    if (ring_obj->cmd == GO_SPEED) {
        ring_obj->speed_x = ring_obj->iframes_timer;
        if (ring_obj->follow_id != 0) {
            ring_obj->speed_y = ring_obj->follow_id;
        } else {
            if (ring_obj->flags.hurtbyfist) {
                unk_obj = &level.objects[ring_obj->param];
                ring_obj->link = (unk_obj->offset_bx + unk_obj->x) - ring_obj->x - ring_obj->offset_bx;
                ring_obj->speed_y =
                        (
                                unk_obj->y + ((unk_obj->offset_by + unk_obj->offset_hy) >> 1) -
                                (ring_obj->y + ((ring_obj->offset_by + ring_obj->offset_hy) >> 1))
                        ) * ring_obj->speed_x;
                if (ring_obj->link != 0) {
                    ring_obj->speed_y /= ring_obj->link;
                }

                ring_obj->link = Abs(ring_obj->link);
            }
            ring_obj->link -= instantSpeed(Abs(ring_obj->speed_x));
        }
    }
}

