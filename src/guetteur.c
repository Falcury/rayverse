
//3DB40
void swapGuetteurCollZones(obj_t* obj, u8 param_2) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (param_2)
        obj->type = TYPE_PIRATE_GUETTEUR;
    else
        obj->type = TYPE_PIRATE_GUETTEUR2;

    obj->zdc = type_zdc[obj->type];
}

//3DB68
void guetteurFollowsShip(obj_t* obj) {
    // NOTE(Falcury): very small difference between the PS1 and PC versions (see below).
    if (bateau_obj_id != -1) {
#ifdef PC
        snapToSprite(obj, &level.objects[bateau_obj_id], 1, -52, -63);
#else
        snapToSprite(obj, &level.objects[bateau_obj_id], 1, -51, -63);
#endif
    } else {
        obj->speed_y = 0;
        obj->speed_x = 0;
    }
}

//3DBB8
void DO_ONE_PAR_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s32 flip_x;

    if (obj->timer != 0)
        obj->timer--;

    flip_x = obj->flags.flip_x;
    if (obj->type == TYPE_PIRATE_GUETTEUR2)
        calc_obj_dir(obj);
    else
        guetteurFollowsShip(obj);

    switch (obj->main_etat * 0x100 + obj->sub_etat) {
        case 0x3:
            obj->speed_y = -8;
            obj->speed_x = 0;
            break;
        case 0x2:
            obj->flags.flip_x = flip_x;
        case 0x5:
        case 0xD:
            obj->timer = 50;
            DO_PAR_TIR(obj);
            break;
        case 0x0:
        case 0x11:
            if (poing.is_active)
                set_sub_etat(obj, 15);
            break;
        case 0x8:
        case 0xB:
        case 0x14:
            if (poing.is_active)
                set_sub_etat(obj, 18);
            break;
        case 0x10:
            if (!poing.is_active)
                set_sub_etat(obj, 17);
            break;
        case 0x13:
            if (!poing.is_active)
                set_sub_etat(obj, 20);
            break;
        case 0x6:
            obj->flags.flip_x = flip_x;
            if (EOA(obj)) {
                swapGuetteurCollZones(obj, 0);
                obj->speed_x = 0;
            }
            break;
        case 0x9:
            obj->flags.flip_x = flip_x;
            if (EOA(obj)) {
                SET_X_SPEED(obj);
                obj->speed_y = -3;
            }
            break;
        case 0x200:
            obj->flags.flip_x = flip_x;
            SET_X_SPEED(obj);
            break;
        case 0xA:
            obj->flags.flip_x = flip_x;
            break;
        case 0x1:
            obj->iframes_timer = 100;
            break;
    }

    if (obj->iframes_timer != 0) {
        obj->iframes_timer--;
    }
}

//3DE28
s32 hasGuetteurABomb(obj_t* obj, s32 param_2) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    anim_t *anim;
    anim_layer_t *unk_2;
    s16 unk_1 = param_2;

    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 4) {
        anim = &obj->animations[obj->anim_index];
        unk_1 = obj->type == TYPE_PIRATE_GUETTEUR ? 0 : 5;
        unk_2 = &anim->layers[(anim->layers_per_frame & 0x3fff) * obj->anim_frame];
        if (unk_2[unk_1].sprite_index == 0)
            unk_1 = -1;
    }

    return unk_1;
}

//3DEA0
void allocatePirateGuetteurBomb(obj_t* gue_obj, s32 param_2, u8 param_3, u8 param_4) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s32 unk_1;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    s16 gue_x; s16 gue_y; s16 gue_w; s16 gue_h;
    s16 unk_x;
    s16 new_spd_x;
    u8 mul_x_pos;
    eta_t *cur_eta_1;
    eta_t *cur_eta_2;
    eta_t *cur_eta_3;

#ifdef NUGGET
    /* reg s3 set to 2 at 801424dc. but can have other values? */
    mul_x_pos = 2;
#endif
    unk_1 = hasGuetteurABomb(gue_obj, param_2);
    if ((s16) unk_1 != -1) {
        i = 0;
        cur_obj = &level.objects[i];
        nb_objs = level.nb_objects;
        while (i < nb_objs) {
            if (cur_obj->type == TYPE_PIRATE_BOMB) {
                if (!cur_obj->is_active) {
                    if (param_3)
                        cur_obj->flags.flip_x = gue_obj->flags.flip_x;
                    else
                        cur_obj->flags.flip_x = gue_obj->flags.flip_x ^ 1;

                    cur_obj->speed_y = param_2;
                    GET_SPRITE_POS(gue_obj, (s16) unk_1, &gue_x, &gue_y, &gue_w, &gue_h);
                    cur_obj->x = gue_x - cur_obj->offset_bx + (gue_w >> 1);
                    cur_obj->y = gue_y + gue_h - cur_obj->offset_by;
                    switch (gue_obj->sub_etat) {
                        case 2:
                        case 0x12:
                            new_spd_x = 48;
                            mul_x_pos = 3;
                            cur_obj->y -= 1;
                            cur_eta_1 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_1->anim_speed = (cur_eta_1->anim_speed & 0xF) | 0xA0;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 10;
                            break;
                        case 5:
                            new_spd_x = 32;
                            mul_x_pos = 3;
                            cur_obj->y += 24;
                            cur_eta_2 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_2->anim_speed = (cur_eta_2->anim_speed & 0xF) | 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            break;
                        case 0x0D:
                            unk_x = 8 - myRand(32);
                            unk_x += 8 * SGN(unk_x);
                            new_spd_x = unk_x;
                            mul_x_pos = 0;
                            cur_eta_3 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_3->anim_speed = (cur_eta_3->anim_speed & 0xF) | 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            cur_obj->y -= 20;
                            break;
                    }
                    if (param_3 == 0)
                        new_spd_x = 16;

                    if (gue_obj->flags.flip_x) /* ternary-free zone */
                        cur_obj->speed_x = new_spd_x;
                    else
                        cur_obj->speed_x = -new_spd_x;

                    cur_obj->x += mul_x_pos * ashr16(cur_obj->speed_x, 4);
                    skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
                    calc_obj_pos(cur_obj);
                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
#ifdef PC
                    add_alwobj(cur_obj);
#endif
                    cur_obj->timer = param_4;
                    cur_obj->flags.flag_0x40 = 0;
                    break;
                }
            }
            cur_obj++;
            i++;
        }
    }
}

//3E1AC
void DO_PAR_TIR(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 sub_etat;

    if (obj->main_etat == 0) {
        sub_etat = obj->sub_etat;
        if (!(sub_etat == 2 && obj->anim_frame > 20) &&
            !(sub_etat == 5 && obj->anim_frame > 17) &&
            !(sub_etat == 13 && obj->anim_frame > 1)
        ) {
            obj->link = 0;
        }

        if (obj->link == 0) {
            sub_etat = obj->sub_etat;
            if ((sub_etat == 2 && obj->anim_frame == 21) ||
                (sub_etat == 5 && obj->anim_frame == 18) ||
                (sub_etat == 13 && obj->anim_frame == 2)
            ) {
                sub_etat = obj->sub_etat;
                if (horloge[obj->eta[obj->main_etat][sub_etat].anim_speed & 0xf] == 0) {
                    if (sub_etat == 5)
                        allocatePirateGuetteurBomb(obj, 2, 1, 40);
                    else if (sub_etat == 2)
                        allocatePirateGuetteurBomb(obj, -1, 1, 40);
                    else {
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x ^= 1;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x ^= 1;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x ^= 1;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x ^= 1;
                    }
                    obj->link = 1;
                }
            }
        }
    } else {
        obj->link = 0;
    }
}

//3E398
void DO_PAR_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 eta_flags = obj->eta[obj->main_etat][obj->sub_etat].flags;

    if ((eta_flags & 1) && obj->iframes_timer == 0 && (obj->type == TYPE_PIRATE_GUETTEUR || sprite == 3)) {
        if (obj->type == TYPE_PIRATE_GUETTEUR2 && (eta_flags & 4)) {
            allocatePirateGuetteurBomb(obj, 1, 0, 20);
        }

        poing.damage = 1;
        obj_hurt(obj);

        if (obj->hit_points == 0) {
            set_main_and_sub_etat(obj, 0, 3);
        } else {
            if (obj->type == TYPE_PIRATE_GUETTEUR) {
                if (obj->init_hit_points - obj->hit_points > 2) {
                    set_sub_etat(obj, 6);
                    if (bateau_obj_id != -1) {
                        skipOneCommand(&level.objects[bateau_obj_id]);
                        level.objects[bateau_obj_id].nb_cmd = 0;
                    }
                } else {
                    set_sub_etat(obj, 12);
                }
            } else {
                set_main_and_sub_etat(obj, 0, 1);
            }
        }
    }
}

//3E4B4
void PAR_REACT_TO_RAY_IN_ZONE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 sub_etat;

    if (obj->timer == 0 && obj->main_etat == 0) {
        sub_etat = obj->sub_etat;
        if (sub_etat == 11)
            set_sub_etat(obj, 2);
        else if (sub_etat == 0) {
            obj->sub_speed = (obj->x + obj->offset_bx) - (ray.x + ray.offset_bx);
            if (Abs(obj->sub_speed) > obj->detect_zone + 16)
                set_sub_etat(obj, 8);
            else
                set_sub_etat(obj, 9);
        }
        else if (sub_etat == 4)
            set_sub_etat(obj, 5);
    }
}

//3E550
void DO_PAR_BOMB_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->timer != 0) {
        if (horloge[2] != 0)
            obj->timer--;
    } else {
        PlaySnd_old(147);
        BombExplosion(obj);
    }
}
