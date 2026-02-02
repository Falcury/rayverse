
//2A940
void DO_CLOWN_TNT_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 diff_x;
    s32 unk_x;
    s32 offs_bx;

    if (obj->main_etat == 0 && obj->sub_etat == 2) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    switch (obj->cmd) {
        case GO_WAIT:
            break;
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;
            break;
        default:
            break;
    }

    diff_x = (obj->x + obj->offset_bx) - ray.x - ray.offset_bx - (ray.speed_x * 2);
    unk_x = 10;
    if (diff_x < -unk_x) {
        if (!(obj->flags.flip_x)) {
            if (obj->sub_etat != 6) {
                obj->flags.flip_x = 1;
                skipToLabel(obj, 4, true);
            }
        } else {
            obj->speed_x = 3;
        }
    } else if (diff_x > unk_x) {
        if (obj->flags.flip_x) {
            if (obj->sub_etat != 6) {
                obj->flags.flip_x = 0;
                skipToLabel(obj, 3, true);
            }
        } else {
            obj->speed_x = -3;
        }
    } else {
        obj->speed_x = 0;
    }

    offs_bx = obj->offset_bx;
    if (obj->flags.flip_x)
        offs_bx += 16;
    else
        offs_bx -= 16;

    if (on_block_chdir(obj, offs_bx, obj->offset_by))
        obj->speed_x = 0;

    if (obj->main_etat == 0) {
        if (obj->sub_etat == 3) {
            if (obj->y <= obj->init_y) {
                obj->speed_y = 0;
                obj->link = 0;
                obj->y = obj->init_y;
                skipToLabel(obj, 2, true);
            }
        } else if (obj->sub_etat == 6) {
            if (obj->anim_frame > 15 && obj->link == 1) {
                DO_CLOWN_TNT_ATTACK(obj);
                obj->link = 2;
            }
        }
    }

    if (obj->link != 0) {
        obj->timer++;
        if (obj->timer > 80) {
            obj->timer = 0;
            obj->link = 0;
        }
    }
}

//2AB2C
void DO_CLOWN_TNT2_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->main_etat == 0 && obj->sub_etat == 2) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    switch (obj->cmd) {
        case GO_WAIT:
            break;
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;
            break;
        default:
            break;
    }
    if (obj->main_etat == 0 && obj->sub_etat == 8) {
        if (obj->link == 0 && obj->anim_frame > 5) {
            DO_CLOWN_TNT_ATTACK(obj);
            obj->link = 1;
        } else if (obj->anim_frame == 1) {
            obj->link = 0;
        }
    }
}

//2ABC8
void DO_CLOWN_TNT3_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 diff_x; s16 diff_y;

    if (obj->main_etat == 0 && obj->sub_etat == 2) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    switch (obj->cmd) {
        case GO_WAIT:
            break;
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;
            SET_X_SPEED(obj);
            break;
    }
    diff_x = (obj->x + obj->offset_bx) - ray.x - ray.offset_bx;
    diff_y = (ray.y + ray.offset_by) - obj->y - obj->offset_by;
    if (obj->link == 0) {
        if (diff_y < 18)
            obj->speed_y = 0;
        else if (diff_y < 40)
            obj->speed_y = 1;
        else if (diff_y < 65)
            obj->speed_y = 2;
        else if (diff_y < 100)
            obj->speed_y = 3;
        else {
            if (Abs(diff_x) < 165)
                obj->speed_y = 4;
            else
                obj->speed_y = 0;
        }

        if ((obj->flags.flip_x && diff_x > 90) || (!(obj->flags.flip_x) && diff_x < -90)) {
            obj->link = 1;
        }
    } else {
        if (diff_y > 120)
            obj->speed_y = -2;
        else
            obj->speed_y = -1;
    }
}

//2AD1C
void DO_BIG_CLOWN_ATTAK(obj_t* bc_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (bc_obj->main_etat == 0 && bc_obj->sub_etat == 2 && bc_obj->link == 0 && bc_obj->anim_frame > 15) {
        bc_obj->link = 1;
        screen_trembling = 1;

        if (ray.main_etat == 0 || ray.main_etat == 1)
            ray.config = 1;
        else if (ray.main_etat == 4 || ray.main_etat == 5) {
            ray.main_etat = 2;
            ray.sub_etat = 1;
        }
    }
    else if (bc_obj->main_etat == 0 && bc_obj->sub_etat == 11)
        bc_obj->link = 0;
    else if (bc_obj->main_etat == 0 && bc_obj->sub_etat == 22)
        skipToLabel(bc_obj, 6, true);
}

//2ADB8
void allocatedrop(obj_t* watc_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_DROP && !cur_obj->is_active) {
            set_main_and_sub_etat(cur_obj, 2, 0);
            cur_obj->flags.flip_x = watc_obj->flags.flip_x;
            cur_obj->speed_y = -2;
            if (!cur_obj->flags.flip_x)
                cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_left;
            else
                cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_right;

            cur_obj->x = (watc_obj->x + watc_obj->offset_bx) - cur_obj->offset_bx;
            cur_obj->y = watc_obj->y - 25;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            cur_obj->gravity_value_2 = 12;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
            calc_obj_pos(cur_obj);
            cur_obj->flags.flag_0x40 = 0;
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
#ifdef PC
            add_alwobj(cur_obj);
#endif
            break;
        }
        cur_obj++;
        i++;
    }
}

//2AEE4
void DO_WAT_CLOWN_ATTAK(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->main_etat == 0 && obj->sub_etat == 2) {
        obj->link = 0;
    } else if (obj->main_etat == 0 && obj->sub_etat == 11 && obj->link == 0) {
        obj->link = 1;
        allocatedrop(obj);
    } else if (obj->main_etat == 0 && obj->sub_etat == 22) {
        skipToLabel(obj, 6, true);
    }
}

//2AF3C
void DO_CLOWN_TNT_ATTACK(obj_t* ct_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_TNT_BOMB && !cur_obj->is_active) {
            set_main_and_sub_etat(cur_obj, 2, 0);
            cur_obj->flags.flip_x = ct_obj->flags.flip_x;
            if (ct_obj->type == TYPE_CLOWN_TNT) {
                cur_obj->speed_y = 1;
                if (ray.speed_x < 2)
                    cur_obj->speed_x = 0;
                else if (cur_obj->flags.flip_x)
                    cur_obj->speed_x = 1;
                else
                    cur_obj->speed_x = -1;
            } else{
                cur_obj->speed_y = 0;
                if (!(cur_obj->flags.flip_x))
                    cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_left;
                else
                    cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_right;
            }

            if (ct_obj->type == TYPE_CLOWN_TNT) {
                GET_SPRITE_POS(ct_obj, 6, &spr_x, &spr_y, &spr_w, &spr_h);
            } else {
                GET_SPRITE_POS(ct_obj, 9, &spr_x, &spr_y, &spr_w, &spr_h);
                spr_w = 16;
                spr_y += 32;
                spr_x += 16;
            }

            cur_obj->x = spr_x - cur_obj->offset_bx + (spr_w >> 1);
            cur_obj->y = spr_y - cur_obj->offset_hy;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
#ifdef PC
            add_alwobj(cur_obj);
#endif
            cur_obj->anim_frame = 0;
            calc_obj_pos(ct_obj);
            break;
        }
        cur_obj++;
        i++;
    }
}

//2B0FC
void Clown_Music_Atter(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->hit_points != obj->init_hit_points) {
        if (obj->main_etat == 2 && obj->sub_etat == 1) {
            set_main_and_sub_etat(obj, 0, 2);
            obj->speed_x = 0;
            obj->flags.read_commands = 1;
        }
    } else {
        set_main_etat(obj, 1);
        set_sub_etat(obj, 0);
        obj->flags.read_commands = 1;
    }

    obj->speed_y = 0;
    if (!(obj->flags.flip_x)) {
        skipToLabel(obj, 2, true);
    } else {
        skipToLabel(obj, 3, true);
    }
}

//2B18C
void DoWatAndBigClownPoingCollision(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    obj_hurt(obj);
    if (poing_obj->speed_x > 0) {
        obj->flags.flip_x = 0;
    } else if (poing_obj->speed_x < 0) {
        obj->flags.flip_x = 1;
    }

    if (obj->hit_points != 0) {
        obj->y -= 2;
        skipToLabel(obj, 4, true);
    } else {
        set_main_and_sub_etat(obj, 0, 3);
        obj->flags.read_commands = 0;
        obj->cmd = GO_LEFT;
    }
}

//2B1FC
void DoTntClownPoingCollision(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    obj_hurt(obj);
    if (poing_obj->speed_x > 0) {
        obj->flags.flip_x = 0;
    } else if (poing_obj->speed_x < 0) {
        obj->flags.flip_x = 1;
    }

    if (obj->hit_points != 0) {
        if (!obj->flags.flip_x) {
            skipToLabel(obj, 6, true);
        } else {
            skipToLabel(obj, 5, true);
        }
    } else {
        set_main_and_sub_etat(obj, 0, 2);
        obj->flags.read_commands = 0;
    }
}

//2B274
void DoWatAndBigClownRaymanZDD(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_RAY_IN_ZONE(). Seems to be functionally identical.
    if ((obj->main_etat == 0 && obj->sub_etat == 0) || (obj->main_etat == 1 && obj->sub_etat == 0)) {
        skipToLabel(obj, 5, true);
    }
}

//2B2A0
void DoTntClownRaymanZDD(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_RAY_IN_ZONE(). Seems to be functionally identical.
    if (obj->link == 0) {
        skipToLabel(obj, 7, true);
        obj->timer = 0;
        obj->link = 1;
    }
}
