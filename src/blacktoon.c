
//23030
void allocateBlacktoonEyes(obj_t* obj) {
    for (s32 i = 0; i < level_alw.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + level_alw.obj_ids[i];
        if (cur_obj->type == TYPE_130_BLACKTOON_EYES && !cur_obj->is_active) {
            cur_obj->anim_frame = 0;
            cur_obj->x = obj->x;
            cur_obj->y = obj->y;
            cur_obj->speed_y = 0;
            cur_obj->speed_x = 0;
            set_main_and_sub_etat(cur_obj, cur_obj->init_main_etat, cur_obj->init_sub_etat);
            calc_obj_pos(cur_obj);
            calc_obj_dir(cur_obj);
            cur_obj->cmd_offset = -1;
            cur_obj->nb_cmd = 0;
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->flags.flag_0x40 = 0;
        }
    }
}

//2311C
void DO_BLK_SPEED_COMMAND(obj_t* obj) {
    /* 6721C 8018BA1C -O2 -msoft-float */
    s16 cen_x; s16 cen_y;

    if (obj->config & 4) {
        cen_x = get_center_x(obj);
        cen_y = get_center_y(obj);
        if (on_block_chdir(obj, cen_x, cen_y) && test_allowed(obj, cen_x, cen_y))
            skipToLabel(obj, 99, true);

        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
        if (obj->speed_x > 0)
            obj->flags.flip_x = 1;
        else if (obj->speed_x < 0)
            obj->flags.flip_x = 0;
    }
    if (obj->config & 8) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
    }
}

//231BC
void blkUTurn(obj_t* obj) {
    if (obj->cmd == GO_LEFT) {
        obj->flags.flip_x = 1;
        skipToLabel(obj, 3, 1);
    } else if (obj->cmd == GO_RIGHT) {
        obj->flags.flip_x = 0;
        skipToLabel(obj, 2, 1);
    }
}

//2320C
void DO_BLK_LR_COMMAND(obj_t* obj) {
    /* 6732C 8018BB2C -O2 -msoft-float */
    s16 cen_x; s16 cen_y;

    if ((obj->main_etat == 0) && (obj->sub_etat == 4)) {
        SET_X_SPEED(obj);
        obj->speed_x = -obj->speed_x;
    } else {
        if (obj->cmd == GO_LEFT)
            obj->flags.flip_x = 0;
        else
            obj->flags.flip_x = 1;

        if (obj->config & 2) {
            SET_X_SPEED(obj);
            if (block_flags[calc_typ_travd(obj, false)] & 1) {
                if (obj->cmd == GO_LEFT)
                    skipToLabel(obj, 3, true);
                else
                    skipToLabel(obj, 2, true);
            }
            CALC_MOV_ON_BLOC(obj);
        }
        if (obj->config & 1) {
            obj->speed_y = sinYspeed(obj, 48, 40, &obj->follow_id);
            cen_x = get_center_x(obj);
            cen_y = get_center_y(obj);
            if (on_block_chdir(obj, cen_x, cen_y) && test_allowed(obj, cen_x, cen_y))
            {
                obj->x -= obj->speed_x;
                if (obj->cmd == GO_LEFT) {
                    skipToLabel(obj, 3, true);
                    obj->flags.flip_x = 1;
                } else {
                    skipToLabel(obj, 2, true);
                    obj->flags.flip_x = 0;
                }
            }
            SET_X_SPEED(obj);
        }
        if (obj->config & 0x10)
        {
            SET_X_SPEED(obj);
            obj->speed_y = 0;
        }
    }
}

//23350
void DO_BLK_NOP_COMMAND(obj_t* obj) {
    /* 6753C 8018BD3C -O2 -msoft-float */
    if (obj->config & 0x10 &&
        ((obj->main_etat == 0 && obj->sub_etat == 2) || (obj->main_etat == 2 && obj->sub_etat == 3))
    ) {
        if (obj->timer == 255) {
            if (((obj->y + obj->speed_y <= obj->param) && obj->y >= obj->param) || obj->speed_y == 0) {
                obj->timer = 60;
            }
        } else if (obj->timer == 0) {
            set_main_and_sub_etat(obj, 2, 0);
            if (obj->flags.flip_x)
                skipToLabel(obj, 3, true);
            else
                skipToLabel(obj, 2, true);

            obj->speed_y = 0;
        } else {
            obj->speed_y = 0;
            obj->timer--;
        }
    }

    if ((obj->follow_sprite == 4) && (
            (obj->main_etat == 0 && obj->sub_etat == 2) || (obj->main_etat == 2 && obj->sub_etat == 3))
    ) {
        if (obj->detect_zone_flag != 0) {
            if ((obj->y + obj->speed_y <= obj->param) && obj->y >= obj->param) {
                set_main_and_sub_etat(obj, 0, 3);
                obj->speed_y = 0;
            }
        } else {
            set_main_and_sub_etat(obj, 2, 1);
        }
    }

    if (obj->config & 2 && obj->speed_x != 0 && block_flags[calc_typ_travd(obj, false)] & 1) {
        if (obj->cmd == GO_LEFT)
            skipToLabel(obj, 3, true);
        else
            skipToLabel(obj, 2, true);
    }
}

//234CC
void DO_BLKTOON_COMMAND(obj_t* obj) {
    /* 71A0 8012B9A0 -O2 -msoft-float */
    switch (obj->cmd) {
        case GO_SPEED:
            DO_BLK_SPEED_COMMAND(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            DO_BLK_LR_COMMAND(obj);
            break;
        case GO_NOP:
            DO_BLK_NOP_COMMAND(obj);
        case GO_WAIT:
        default:
            break;
    }
}

//234FC
void DoBlackToonPoingCollision(obj_t* obj, s16 sprite) {
    obj_hurt(obj);
    set_main_and_sub_etat(obj, 0, 4);
    obj->speed_y = 0;
    if (poing_obj->speed_x >= 0 && (poing_obj->speed_x != 0 || poing_obj->flags.flip_x)) {
        obj->flags.flip_x = 0;
    } else {
        obj->flags.flip_x = 1;
    }
    allocateBlacktoonEyes(obj);
    if (obj->flags.flip_x) {
        skipToLabel(obj, 2, 1);
    } else {
        skipToLabel(obj, 3, 1);
    }

}

//235A0
void DoBlackToonRaymanZDD(obj_t* obj) {
    switch (obj->follow_sprite) {
        case 7:
            if (obj->main_etat == 1 && obj->sub_etat == 1 && ray.main_etat != 2) {
                skipToLabel(obj, 5, true);
            }
            break;
        case 4:
            if (ray.main_etat != 2) {
                s16 blkt_x, blkt_y, blkt_w, blkt_h;
                GET_ANIM_POS(obj, &blkt_x, &blkt_y, &blkt_w, &blkt_h);
                blkt_y -= obj->detect_zone + blkt_h;
                blkt_x -= blkt_w >> 1;
                blkt_w += blkt_w;
                blkt_h += obj->detect_zone;
                if (obj->main_etat == 1 && obj->sub_etat == 0
                        && inter_box(blkt_x, blkt_y, blkt_w, blkt_h, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h)
                ) {
                    calc_obj_dir(obj);
                    obj->param = ray_zdc_y - obj->offset_hy;
                    if (obj->param < obj->y - 16) {
                        skipToLabel(obj, 4, true);
                        obj->gravity_value_1 = 0;
                        obj->gravity_value_2 = 6;
                        obj->y--;
                    }
                }
            }
            break;
        case 3:
            if (obj->main_etat == 0 && obj->sub_etat == 0 && ray.main_etat != 2) {
                calc_obj_dir(obj);
                obj->param = ray.y + ray.offset_hy - obj->offset_hy;
                if (obj->param < obj->y - 16) {
                    skipToLabel(obj, 4, true);
                    obj->gravity_value_2 = 6;
                    obj->gravity_value_1 = 0;
                    obj->timer = 255;
                    obj->y--;
                }
            }
            break;
        case 2:
            if (obj->main_etat == 0 && obj->sub_etat == 0 && ray.main_etat != 2) {
                calc_obj_dir(obj);
                skipToLabel(obj, 4, true);
                obj->gravity_value_1 = 0;
                obj->gravity_value_2 = 6;
                obj->y--;
            }
            break;
    }
}
