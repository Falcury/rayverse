
//200B0
void BB2_MARCHE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem identical. The function is inlined in the PS1 version.
    switch (bb1.speed_x) {
        case 2:
            set_main_and_sub_etat(obj, 1, 1);
            break;
        case 3:
            set_main_and_sub_etat(obj, 1, 2);
            break;
        case 0:
        default:
            set_main_and_sub_etat(obj, 1, 0);
            break;
    }
}

//200F0
void DO_TOTEM_TOUCHE(obj_t* obj, s16 sprite) {
    /* 5783C 8017C03C -O2 -msoft-float */
    s16 x;

    if (sprite == 6 && obj->param == 0) {
        if (poing_obj->speed_x > 0) {
            set_sub_etat(obj, 4);
            x = obj->init_x + 71;
        } else {
            set_sub_etat(obj, 5);
            x = obj->init_x - 85;
        }
        obj->x = x;
        obj->iframes_timer = 0;
        obj->param = 255;
        obj->anim_frame = 255;
    } else if (sprite >= 2) {
        if (poing_obj->speed_x > 0)
            set_sub_etat(obj, 7);
        else
            set_sub_etat(obj, 6);
    }
}

//2018C
void DO_TOTEM_COMMAND(obj_t* tot_obj) {
    /* 57950 8017C150 -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;
    u8 nb_objs;
    s16 i;
    s16 j;
    obj_t *cur_obj;
    s16 new_spd_x;
    u8 unk_1;
    s16 new_x;

    tot_obj->param--;
    if (tot_obj->param < 0)
        tot_obj->param = 0;
    if (bb1.field2_0x4 == 3) {
        tot_obj->is_active = 0;
        tot_obj->flags.alive = 0;
        nb_objs = level.nb_objects;
        i = 0;
        cur_obj = level.objects;
        if (nb_objs != 0) {
            do {
                if (cur_obj->type == TYPE_BOUT_TOTEM) {
                    for (j = 0; j < 6; cur_obj++, j++) {
                        GET_SPRITE_POS(tot_obj, j, &x, &y, &w, &h);
                        cur_obj->x = x;
                        cur_obj->y = y;
                        cur_obj->is_active = 1;
                        cur_obj->flags.alive = 1;
                        if (j == 0) {
                            cur_obj->speed_x = 0;
                            cur_obj->speed_y = 0;
                        } else {
                            new_spd_x = 2 - myRand(4);
                            cur_obj->speed_x = new_spd_x;
                            if (new_spd_x == 0)
                                cur_obj->speed_x = 1;
                            cur_obj->speed_y = -4 - myRand(3);
                        }
                        cur_obj->gravity_value_1 = 0;
                        cur_obj->gravity_value_2 = 5;
                        switch (j) {
                            case 1:
                                set_main_and_sub_etat(cur_obj, 2, 8);
                                cur_obj->offset_by = 43;
                                cur_obj->offset_bx = 25;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 31;
                                break;
                            case 2:
                                set_main_and_sub_etat(cur_obj, 2, 10);
                                cur_obj->offset_by = 17;
                                cur_obj->offset_bx = 12;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 33;
                                break;
                            case 3:
                                set_main_and_sub_etat(cur_obj, 2, 11);
                                cur_obj->offset_by = 7;
                                cur_obj->offset_bx = 7;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 34;
                                break;
                            case 4:
                                set_main_and_sub_etat(cur_obj, 2, 9);
                                cur_obj->offset_by = 28;
                                cur_obj->offset_bx = 17;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 32;
                                break;
                            case 5:
                                set_main_and_sub_etat(cur_obj, 2, 10);
                                cur_obj->offset_by = 17;
                                cur_obj->offset_bx = 12;
                                cur_obj->flags.flip_x = 1;
                                cur_obj->anim_index = 33;
                                break;
                            case 0:
                                set_main_and_sub_etat(cur_obj, 0, 21);
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 37;
                                break;
                        }
                    }
                    break;
                }
                i++;
                cur_obj++;
            } while (i < nb_objs);
        }
    } else if (!(tot_obj->sub_etat == 6 || tot_obj->sub_etat == 7 || tot_obj->sub_etat == 3)) {
        GET_SPRITE_POS(tot_obj, 6, &x, &y, &w, &h);
        x += 5; y += 5; w -= 10; h -= 10;
        if (inter_box(x, y, w, h, bb1.sprite6_x, bb1.sprite6_y, 32, 32) && bb1.field2_0x4 == 0) {
            bb1.field2_0x4 = 1;
            tot_obj->iframes_timer = 4;
            IndSerie++;
            if (IndSerie > 5)
                IndSerie--;
            IndAtak = 0;
        }

        if (tot_obj->anim_frame == (tot_obj->animations[tot_obj->anim_index].frame_count - 1))
            unk_1 = horloge[get_eta(tot_obj)->anim_speed & 0xf] == 0;
        else
            unk_1 = false;

        switch (tot_obj->sub_etat) {
            case 3:
            case 6:
            case 7:
                break;
            case 4:
                if (unk_1) {
                    if (++tot_obj->iframes_timer < 4) {
                        set_sub_etat(tot_obj, 5);
                        tot_obj->x = tot_obj->init_x - 85;
                    } else {
                        set_sub_etat(tot_obj, 6);
                        tot_obj->x = tot_obj->init_x;
                    }
                }
                break;
            case 5:
                if (unk_1) {
                    if (++tot_obj->iframes_timer < 4) {
                        set_sub_etat(tot_obj, 4);
                        tot_obj->x = tot_obj->init_x + 71;
                    } else {
                        set_sub_etat(tot_obj, 7);
                        tot_obj->x = tot_obj->init_x;
                    }
                }
                break;
        }
    }
}

//205C4
void DO_TOTBT_REBOND(obj_t* obj) {
    /* 57EC4 8017C6C4 -O2 -msoft-float */
    if (obj->speed_y > 0 && obj->y + obj->offset_by >= 265) {
        obj->speed_x /= 2;
        obj->speed_y = -obj->speed_y / 2;
        if (obj->speed_y == 0)
            obj->gravity_value_2 = 0xFF;
        obj->gravity_value_1 = 5 - obj->gravity_value_1;
    }
}

//20620
void DO_PI_EXPLOSION2(obj_t* obj) {
    /* 57F48 8017C748 -O2 -msoft-float */
    PlaySnd_old(84);
    obj->speed_x = -1;
    obj->x += 20;
    allocateStoneChips(obj);
    obj->speed_x = 1;
    obj->x -= 40;
    allocateStoneChips(obj);
    obj->x += 20;
    allocateExplosion(obj);
}

//20674
void DO_BBL_REBOND(obj_t* obj) {
    /* 57FBC 8017C7BC -O2 -msoft-float */
    s32 x_pos;
    s16 x_pierre;

    PlaySnd(200, obj->id);
    switch (obj->param) {
        case 0:
            obj->speed_y = -5;
            obj->gravity_value_1 = 3 - obj->gravity_value_1;
            break;
        case 2:
            obj->speed_y = -5;
            obj->gravity_value_1 = 3 - obj->gravity_value_1;
            x_pos = obj->x;
            if (obj->flags.flip_x)
                x_pierre = x_pos - 130;
            else
                x_pierre = x_pos + 25;
            PosPierre = x_pierre;
            break;
        case 4:
            DO_PI_EXPLOSION2(obj);
            obj->is_active = 0;
            obj->flags.alive = 0;
            if (ray.follow_id == obj->id) {
                ray.follow_id = -1;
                obj->ray_dist = 1000;
                set_main_and_sub_etat(&ray, 2, 2);
            }
//            PS1_AlwaysObjectsCount -= 1;
            break;
    }
    recale_position(obj);
}

//20748
void allocateDog(obj_t* bb1_obj) {
    /* 580F8 8017C8F8 -O2 -msoft-float */
    s16 new_x;
    s16 nb_objs = level.nb_objects;
    obj_t *cur = level.objects;
    s16 i = 0;

    if (nb_objs != 0) {
        do {
            if (cur->type == TYPE_STONEDOG2 && !cur->is_active) {
                if (!((bb1_obj->flags.flip_x + YaDesChiens) & 1)) {
                    new_x = -20;
                    cur->flags.flip_x = 1;
                } else {
                    new_x = 260;
                    cur->flags.flip_x = 0;
                }
                obj_init(cur);
                cur->cmd_offset = -1;
                cur->x = new_x;
                cur->y = bb1_obj->y - 20;
                cur->is_active = 1;
                cur->flags.alive = 1;
                add_alwobj(cur);
                cur->speed_x = 0;
                cur->speed_y = 0;
                cur->main_etat = 2;
                cur->flags.flag_0x40 = 0;
                cur->sub_etat = 2;
                if (cur->flags.flip_x)
                    skipToLabel(cur, 1, true);
                else
                    skipToLabel(cur, 0, true);
                calc_obj_pos(cur);
                if (niveau == 0)
                    cur->hit_points = 1;
                break;
            }
            cur++;
            i++;
        } while (i < nb_objs);
    }
}

//20868
void allocateTir(obj_t* bb1_obj, s16 param_2) {
    /* 58278 8017CA78 -O2 -msoft-float */
    s32 sprite_ind;
    s16 x; s16 y; s16 w; s16 h;
    s32 new_x_1;
    s32 bb1_x;
    s32 new_spd_x;
    s32 new_x_2;
    u8 nb_objs = level.nb_objects;
    s16 i = 0;
    obj_t *cur_obj = level.objects;

    if (nb_objs != 0) {
        do {
            if (cur_obj->type == TYPE_BBL && !cur_obj->is_active) {
                // NOTE: added in PC version
                if (param_2 == 4) {
                    cur_obj->flags.follow_enabled = 0;
                } else {
                    cur_obj->flags.follow_enabled = 1;
                }

                if (param_2 != 0)
                    sprite_ind = 8;
                else
                    sprite_ind = 9;
                cur_obj->flags.flip_x = 0;
                GET_SPRITE_POS(bb1_obj, sprite_ind, &x, &y, &w, &h);
                if (param_2 == 1) {
                    new_x_1 = x - cur_obj->offset_bx + (w >> 1);
                    if (bb1_obj->flags.flip_x)
                        new_x_1 += 64;
                    else
                        new_x_1 -= 64;
                    cur_obj->x = new_x_1;
                    cur_obj->y = y - 40;
                    if (bb1_obj->flags.flip_x)
                        new_spd_x = 12;
                    else
                        new_spd_x = -12;
                    cur_obj->speed_x = new_spd_x;
                    cur_obj->speed_y = 0;
                    cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                } else if (param_2 == 0) {
                    new_x_1 = (x - cur_obj->offset_bx) + (w >> 1);
                    if (bb1_obj->flags.flip_x)
                        new_x_1 += 9;
                    else
                        new_x_1 -= 9;
                    cur_obj->x = new_x_1;
                    cur_obj->y = y - 40;
                    if (bb1_obj->flags.flip_x)
                        new_spd_x = 2;
                    else
                        new_spd_x = -2;
                    cur_obj->speed_x = new_spd_x;
                    cur_obj->speed_y = -5;
                    cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                } else {
                    if (param_2 == 3) {
                        bb1_x = bb1_obj->x;
                        if (!(bb1_obj->flags.flip_x))
                            cur_obj->x = bb1_x - 36;
                        else
                            cur_obj->x = bb1_x + 120;
                        cur_obj->y = bb1_obj->y - 185;
                        cur_obj->speed_y = 4;
                    } else {
                        if (param_2 < 10) {
                            cur_obj->x = ray.x;
                            if (cur_obj->x < 40)
                                cur_obj->x = 40;
                            if (cur_obj->x > 200)
                                cur_obj->x = 200;
                        } else {
                            if (bb1_obj->flags.flip_x)
                                new_x_2 = 240 - bb1.field8_0xe * 50;
                            else {
                                new_x_2 = bb1.field8_0xe * 50 - 20;
                            }
                            cur_obj->x = new_x_2;
                        }
                        cur_obj->y = bb1_obj->y - 200;
                        cur_obj->speed_y = 0;
                    }
                    cur_obj->speed_x = 0;
                }
                cur_obj->iframes_timer = 38;
                if (param_2 != 3)
                    cur_obj->gravity_value_2 = 10;
                else
                    cur_obj->gravity_value_2 = 7;

                if (param_2 == 0)
                    cur_obj->gravity_value_2 = 3;
                cur_obj->main_etat = 2;
                cur_obj->sub_etat = 6;
                skipToLabel(cur_obj, 1, true);
                calc_obj_pos(cur_obj);
                cur_obj->gravity_value_1 = 0;
                cur_obj->is_active = 1;
                cur_obj->flags.alive = 1;
                add_alwobj(cur_obj);
                if (param_2 < 10)
                    cur_obj->param = param_2;
                else
                    cur_obj->param = 4;
//                if (param_2 != 3)
//                {
//                    PS1_AlwaysObjects[PS1_AlwaysObjectsCount] = cur_obj->id;
//                    PS1_AlwaysObjectsCount++;
//                }
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
    }
}

//20B7C
void CreateFirstBBL(void) {
    /* 58644 8017CE44 -O2 -msoft-float */
    obj_t *obj;
    s16 i;
    u8 nb_objs;

    obj = level.objects;
    i = 0;
    nb_objs = level.nb_objects;
    if (nb_objs != 0) {
        do {
            if (obj->type == TYPE_BBL && !obj->is_active)
            {
                obj->x = -40;
                PosPierre = -15;
                obj->y = 160;
                obj->speed_x = 0;
                obj->speed_y = 0;
                obj->iframes_timer = 38;
                obj->gravity_value_2 = 3;
                obj->main_etat = 2;
                obj->sub_etat = 6;
                obj->flags.flip_x = 0;
                skipToLabel(obj, 1, true);
                calc_obj_pos(obj);
                obj->gravity_value_1 = 0;
                obj->param = 2;
                obj->is_active = 1;
                obj->flags.alive = 1;
                obj->flags.follow_enabled = 1; // added in PC version
                add_alwobj(obj);
//                PS1_AlwaysObjects[PS1_AlwaysObjectsCount] = obj->id;
//                PS1_AlwaysObjectsCount++;
                break;
            }
            obj++;
            i++;
        } while (nb_objs > i);
    }
}

//20C4C
void INIT_BBMONT(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    calc_obj_pos(obj);
    obj->param = 0;
    obj->gravity_value_2 = 10;
    Phase = 0;
    WaitForAnim = false;
    WaitForFinAtan = 10;
    PosPierre = 9999;
    PierreDoitExploser = 0;
    IndSerie = 0;
    IndAtak = 0;
    RMrepere = 0;
    YaDesChiens = 0;
    screen_trembling2 = 0;
    obj->flags.alive = 1;
    obj->is_active = 0;
    bb1.field1_0x2 = -1;
    bb1.speed_x = 0;
    bb1.saved_cmd = 0;
    bb1.field2_0x4 = 0;
    bb1.field8_0xe = 1;
}

//20CF8
void DO_BBL_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    // There is a bug in the original game, though, causing a potential out-of-bounds access (see below).

    s16 new_spd_x;
    s32 spd_x_abs;
    s32 spd_x;
    s32 bb1_x;
    s16 unk_x;

    if (PierreDoitExploser) {
        DO_PI_EXPLOSION2(obj);
        obj->is_active = 0;
        obj->flags.alive = 0;
        if (ray.follow_id == obj->id) {
            ray.follow_id = -1;
            obj->ray_dist = 1000;
            set_main_and_sub_etat(&ray, 2, 2);
        }
        PierreDoitExploser = false;
        PosPierre = 9999;
#ifdef PS1
        PS1_AlwaysObjectsCount--;
#endif
    }

    if (obj->param == 3 && --obj->iframes_timer == 0) {
        obj->is_active = 0;
        obj->flags.alive = 0;
    }

    if (obj->speed_x < 0)
        unk_x = -20;
    else
        unk_x = 20;

    // NOTE: need to prevent out-of bounds if obj->y + 40 < 0 (this is a bug in the original game)
    s32 tile_x = MAX(0, obj->x + obj->offset_bx + unk_x) >> 4;
    s32 tile_y = MAX(0, obj->y + 40) >> 4;

    if (block_flags[mp.map[mp.width * tile_y + tile_x].tile_type] >> BLOCK_FLAG_4 & 1) {
        switch (obj->param) {
            case 1:
                PlaySnd(200, obj->id);
                spd_x = obj->speed_x;
                spd_x_abs = Abs(spd_x);
                if (spd_x_abs == 12) {
                    if (spd_x > 0)
                        new_spd_x = -13;
                    else
                        new_spd_x = 13;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (spd_x_abs == 13) {
                    if (spd_x > 0)
                        new_spd_x = -14;
                    else
                        new_spd_x = 14;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (spd_x_abs == 14) {
                    if (spd_x > 0)
                        new_spd_x = -15;
                    else
                        new_spd_x = 15;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (spd_x_abs == 15) {
                    DO_PI_EXPLOSION2(obj);
                    obj->is_active = 0;
                    obj->flags.alive = 0;
                    if (ray.follow_id == obj->id) {
                        ray.follow_id = -1;
                        obj->ray_dist = 1000;
                        set_main_and_sub_etat(&ray, 2, 2);
                    }
#ifdef PS1
                    PS1_AlwaysObjectsCount--;
#endif
                    return;
                }
                break;
            case 0:
                PlaySnd(200, obj->id);
                obj->speed_x = 0;
                obj->param = 2;
                break;
        }
    }
}

//20F38
void BBMONT_ECLAIR(obj_t* bb1_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 new_spd_x;
    s32 new_x;
    s16 x; s16 y; s16 w; s16 h;
    u8 nb_objs = level.nb_objects;
    s16 i = 0;
    obj_t* cur_obj = level.objects;

    if (nb_objs != 0) {
        do {
            if (cur_obj->type == TYPE_ECLAIR && !cur_obj->is_active) {
                cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                cur_obj->speed_y = 0;
                if (!cur_obj->flags.flip_x)
                    cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_left;
                else
                    cur_obj->speed_x = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].speed_x_right;
                GET_SPRITE_POS(bb1_obj, 10, &x, &y, &w, &h);
                new_x = (x + w) - cur_obj->offset_bx;
                if (bb1_obj->flags.flip_x)
                    new_x += 16;
                else
                    new_x -= 16;
                cur_obj->x = new_x;
                cur_obj->y = y;
                cur_obj->init_x = cur_obj->x;
                cur_obj->init_y = cur_obj->y;
                skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
                calc_obj_pos(cur_obj);
                cur_obj->is_active = 1;
                cur_obj->flags.alive = 1;
#ifdef PC
                add_alwobj(cur_obj);
#endif
                allocateExplosion(cur_obj);
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
    }
}

//21098
void BBMONT_ETINCELLES(obj_t* in_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 in_x; s16 in_y; s16 in_w; s16 in_h;
    s16 i = 0;
    obj_t *cur_obj = &level.objects[0];
    u8 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_ETINC && !cur_obj->is_active) {
            /* sprite 3 */
            cur_obj->flags.flip_x = in_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (!cur_obj->flags.flip_x)
                cur_obj->speed_x = 7;
            else
                cur_obj->speed_x = -7;

            GET_SPRITE_POS(in_obj, 3, &in_x, &in_y, &in_w, &in_h);
            if (!in_obj->flags.flip_x)
                in_w = 0;

            cur_obj->x = in_x + in_w - cur_obj->offset_bx;
            cur_obj->y = in_y - 8;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
#ifdef PC
            add_alwobj(cur_obj);
#endif

            /* sprite 2 */
            cur_obj++;
            cur_obj->flags.flip_x = in_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (!(cur_obj->flags.flip_x))
                cur_obj->speed_x = 7;
            else
                cur_obj->speed_x = -7;

            GET_SPRITE_POS(in_obj, 2, &in_x, &in_y, &in_w, &in_h);
            if (!in_obj->flags.flip_x)
                in_w = 0;

            cur_obj->x = in_x + in_w - cur_obj->offset_bx;
            cur_obj->y = in_y - 8;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, true);
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
#ifdef PC
            add_alwobj(cur_obj);
#endif

            allocateExplosion(cur_obj);
            break;
        }
        cur_obj++;
        i++;
    }
}

//212C4
void BBMONT_TIRE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions are identical
    allocateTir(obj, 1);
}

//212D4
void Cree_BBL(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions are identical
    allocateTir(obj, 0);
}

//21300
void BB_Attaque(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    switch (NextAtak) {
        case 0:
            set_main_and_sub_etat(obj, 0, 4);
            Phase = 1;
            break;
        case 1:
            set_main_and_sub_etat(obj, 0, 3);
            Phase = 2;
            break;
        case 2:
            set_main_and_sub_etat(obj, 0, 6);
            Phase = 3;
            break;
        case 3:
            set_main_and_sub_etat(obj, 0, 7);
            Phase = 4;
            break;
        case 4:
            set_main_and_sub_etat(obj, 0, 2);
            Phase = 5;
            break;
        case 5:
            set_main_and_sub_etat(obj, 0, 4);
            Phase = 12;
            break;
        case 6:
            set_main_and_sub_etat(obj, 2, 3);
            obj->iframes_timer = 35;
            Phase = 11;
            bb1.field8_0xe = 1;
            break;
        case 7:
            set_main_and_sub_etat(obj, 0, 22);
            Phase = 13;
        default: break;
    }
}

//21400
void Fin_BB_Attaque(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    IndAtak++;
    if (IndAtak < 7)
        NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
    else
        NextAtak = 0xff;

    if (NextAtak == 0xff) {
        IndAtak = 0;
        NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
    }
    WaitForFinAtan = SerieAtakBB[IndSerie][IndAtak].wait_for_fin_atan;
    BB_Attaque(obj);
}

//214B8
void BB_Atan(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    if (WaitForFinAtan != 0) {
        set_main_and_sub_etat(obj, 0, 0);
        skipToLabel(obj, 0, 1);
        Phase = 6;
    } else {
        Fin_BB_Attaque(obj);
    }
}

//214F8
void DO_BBMONT_ATTER(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    obj->speed_y = 0;
    obj->speed_x = 0;
    recale_position(obj);
    set_sub_etat(obj, 6);
    allocateLandingSmoke(obj);
    screen_trembling3 = 1;
}

//21568
void DO_BBMONT_COMMAND(obj_t* obj) {
    // NOTE(Falcury): I did not spot functional differences PS1 and PC versions at first glance
    s16 unk_1; s16 unk_2;
    s32 x_1;
    s32 x_2;
    s32 pos_pierre;
    u8 wait;
    s16 new_dogs;
    s16 new_spd_x_1;
    s16 new_spd_x_2;

    scrollLocked = true;
    if (YaDesChiens != 0 && --horlogeDog < 0) {
        horlogeDog = 50;
        allocateDog(obj);
        YaDesChiens--;
    }
    GET_SPRITE_POS(obj, 6, &bb1.sprite6_x, &bb1.sprite6_y, &unk_1, &unk_2);
    unk_1 = obj->x;
    unk_2 = obj->y;
    if (obj->anim_frame == obj->animations[obj->anim_index].frame_count - 1 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
    ) {
        FinAnim = true;
    } else {
        FinAnim = false;
    }

    if (bb1.field2_0x4 == 2 && FinAnim) {
        if (obj->hit_points == obj->init_hit_points - 1) {
            skipToLabel(obj, 4, true);
            Phase = 7;
        }
        set_main_and_sub_etat(obj, bb1.saved_main_etat, bb1.saved_sub_etat);
        WaitForFinAtan = 1;
        obj->cmd = bb1.saved_cmd;
    }
    if (bb1.field2_0x4 == 1) {
        if (!(obj->main_etat == 0 && obj->sub_etat == 9)) {
            bb1.saved_main_etat = obj->main_etat;
            bb1.saved_sub_etat = obj->sub_etat;
            set_main_and_sub_etat(obj, 0, 9);
            bb1.field2_0x4 = 2;
            FinAnim = false;
            obj->speed_x = 0;
            obj->hit_points--;
            bb1.saved_cmd = obj->cmd;
            if (obj->hit_points == 0) {
                bb1.field2_0x4 = 3;
                set_main_and_sub_etat(obj, 0, 20);
                obj->speed_x = 0;
                bb1.saved_cmd = 420;
            }
        } else {
            bb1.field2_0x4 = 0;
        }
    }

    if (bb1.field2_0x4 == 3) {
        if (--bb1.saved_cmd <= 0) {
            fin_boss = true;
            bb1.saved_cmd = 999;
            finBosslevel.mr_stone = true;
        }
    } else {
        switch (obj->cmd) {
            case GO_LEFT:
                obj->flags.flip_x = 0;
            case GO_RIGHT:
                if (obj->cmd == GO_RIGHT)
                    obj->flags.flip_x = 1;
                SET_X_SPEED(obj);
                if (block_flags[calc_typ_travd(obj, false)] >> BLOCK_FLAG_4 & 1) {
                    obj->flags.flip_x ^= 1;
                    skipToLabel(obj, 6, true);
                    if (IndSerie > 3)
                        WaitForFinAtan = 2;
                    Phase = 8;
                }

                if (Phase == 7) {
                    /* different way to write this, or is this the nicest??? */
                    pos_pierre = PosPierre;
                    x_1 = obj->x;
                    if ((obj->flags.flip_x && (x_1 < pos_pierre + 14)) || (!(obj->flags.flip_x) && (x_1 < pos_pierre - 6))) {
                        pos_pierre = PosPierre;
                        x_2 = obj->x;
                        if ((obj->flags.flip_x && (x_2 > pos_pierre + 6)) || (!(obj->flags.flip_x) && (x_2 > pos_pierre - 14))) {
                            set_main_and_sub_etat(obj, 0, 2);
                            skipToLabel(obj, 0, true);
                            Phase = 9;
                        }
                    }
                }
                CALC_MOV_ON_BLOC(obj);
                break;
            case GO_WAIT:
                switch (Phase) {
                    default: break;
                    case 0:
                        if (bb1.field8_0xe != 0) {
                            CreateFirstBBL();
                            WaitForAnim = true;
                            bb1.field8_0xe = 0;
                        }

                        if (FinAnim && WaitForAnim) {
                            obj->anim_frame = 0;
                            IndSerie = 0;
                            IndAtak = 0;
                            WaitForFinAtan = SerieAtakBB[IndSerie][IndAtak].wait_for_fin_atan;
                            NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
                            BB_Attaque(obj);
                        }
                        break;
                    case 1:
                        if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            screen_trembling2 == 0
                        ) {
                            screen_trembling2 = 1;
                            allocateTir(obj, 4);
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 2:
                        if (obj->sub_etat == 11 && obj->anim_frame == 23 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            allocateTir(obj, 1);
                        }
                        if (obj->sub_etat == 3 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            screen_trembling2 == 0
                        ) {
                            screen_trembling2 = 1;
                            allocateTir(obj, 3);
                            wait = WaitForFinAtan;
                            if (niveau == 0)
                                WaitForFinAtan = wait + 3;
                            else
                                WaitForFinAtan = wait + 1;
                        }
                        if (obj->sub_etat == 10 && FinAnim) {
                            if (--WaitForFinAtan != 0) {
                                obj->anim_frame = 0;
                                set_sub_etat(obj, 10);
                            }
                        }
                        if (obj->sub_etat == 11 && FinAnim) {
                            Phase = 11;
                            obj->iframes_timer = 35;
                        }
                        break;
                    case 3:
                        if (obj->sub_etat == 6 && obj->anim_frame == 64 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            if (niveau == 0)
                                new_dogs = 3;
                            else
                                new_dogs = 4;
                            YaDesChiens = new_dogs;
                            horlogeDog = 0;
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 4:
                        if (obj->sub_etat == 7 && obj->anim_frame == 44 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            BBMONT_ETINCELLES(obj);
                            PlaySnd(114, obj->id);
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 5:
                        if (obj->sub_etat == 2 && obj->anim_frame == 21 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            BBMONT_ECLAIR(obj);
                            PlaySnd(114, obj->id);
                        }

                        if (FinAnim) {
                            obj->anim_frame = 0;
                            set_sub_etat(obj, 1);
                            Phase = 10;
                        }
                        break;
                    case 6:
                        if (FinAnim && --WaitForFinAtan == 0)
                            Fin_BB_Attaque(obj);
                        break;
                    case 7:
                        if (obj->sub_etat == 8 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            allocateTir(obj, 0);
                        } else {
                            if (FinAnim) {
                                if (--WaitForFinAtan == 0) {
                                    set_main_and_sub_etat(obj, 1, 2);
                                    if (obj->flags.flip_x)
                                        skipToLabel(obj, 5, true);
                                    else
                                        skipToLabel(obj, 4, true);
                                } else {
                                    set_sub_etat(obj, 1);
                                }
                            }
                        }
                        break;
                    case 8:
                        if (FinAnim) {
                            if (obj->sub_etat != 9) {
                                if (WaitForFinAtan == 2) {
                                    set_main_and_sub_etat(obj, 0, 1);
                                    WaitForFinAtan--;
                                } else {
                                    if (bb1.field8_0xe == 0) {
                                        /* ... */
                                        WaitForFinAtan = SerieAtakBB[IndSerie][0].wait_for_fin_atan;
                                        NextAtak = SerieAtakBB[IndSerie][0].attack;
                                        IndAtak = 0;
                                        BB_Attaque(obj);
                                    } else {
                                        Fin_BB_Attaque(obj);
                                        bb1.field8_0xe = 0;
                                    }
                                }
                            }
                        }
                        break;
                    case 9:
                        if (obj->anim_frame == 20 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            obj->sub_etat != 9
                        ) {
                            PierreDoitExploser = true;
                        }

                        if (FinAnim && bb1.field2_0x4 != 2) {
                            set_main_and_sub_etat(obj, 1, 0);
                            if (obj->flags.flip_x)
                                skipToLabel(obj, 5, true);
                            else
                                skipToLabel(obj, 4, true);
                            PierreDoitExploser = false;
                        }
                        break;
                    case 10:
                        if (FinAnim) {
                            obj->anim_frame = 0;
                            set_main_and_sub_etat(obj, 1, 1);
                            if (obj->flags.flip_x)
                                skipToLabel(obj, 5, true);
                            else
                                skipToLabel(obj, 4, true);
                        }
                        break;
                    case 11:
                        switch (obj->sub_etat) {
                            case 0:
                            case 3:
                                if (--obj->iframes_timer > 1) {
                                    obj->speed_y = -8;
                                    obj->gravity_value_1 = 0;
                                    if (obj->flags.flip_x)
                                        new_spd_x_1 = 4;
                                    else
                                        new_spd_x_1 = -4;
                                    obj->speed_x = new_spd_x_1;
                                } else {
                                    set_sub_etat(obj, 4);
                                    if (obj->flags.flip_x)
                                        new_spd_x_2 = 2;
                                    else
                                        new_spd_x_2 = -2;
                                    obj->speed_x = new_spd_x_2;
                                }
                                break;
                            case 6:
                                if (FinAnim) {
                                    set_main_and_sub_etat(obj, 1, 0);
                                    if (obj->flags.flip_x)
                                        skipToLabel(obj, 5, true);
                                    else
                                        skipToLabel(obj, 4, true);
                                }
                                break;
                        }
                        break;
                    case 12:
                        if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            screen_trembling2 == 0
                        ) {
                            screen_trembling2 = 1;
                            allocateTir(obj, bb1.field8_0xe + 10);
                            bb1.field8_0xe++;
                        }

                        if (FinAnim && bb1.field8_0xe == 3) {
                            bb1.field8_0xe = 0;
                            BB_Atan(obj);
                        }
                        break;
                    case 13:
                        if (obj->sub_etat == 22) {
                            if (obj->anim_frame == 63 &&
                                horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                            ) {
                                allocateTir(obj, 1);
                            }
                            if (obj->anim_frame == 14 &&
                                horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                                screen_trembling2 == 0
                            ) {
                                screen_trembling2 = 1;
                                allocateTir(obj, 3);
                            }

                            if (FinAnim) {
                                Phase = 11;
                                obj->iframes_timer = 35;
                                bb1.field8_0xe = 1;
                            }
                        }
                }
                break;
        }

        if (FinAnim && bb1.field2_0x4 == 2) {
            bb1.field2_0x4 = 0;
        }
    }
}

//22050
void DO_BBMONT_TOUCHE(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 and PC versions are slightly different (but in the end functionally identical).
    // In the PS1 version the sprite check is in DO_POING_COLLISION, while in the PC version the check is moved here.
    if (sprite == 9) {
        set_sub_etat(obj, 8);
        obj->anim_frame = 0;
        Phase = 7;
        WaitForFinAtan = 2;
    }
}

//22094
void DO_BBMONT2_COMMAND(obj_t* obj) {
    //NOTE: DO_BBMONT2_COMMAND is still non-matching in the PS1 decomp!
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_v0_2;
    s16 temp_v0_3;
    s16 var_v0_2;
    s16 var_v1;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    s32 var_v0_6;
    s32 var_v1_2;
    u16 temp_v0_4;
    u16 var_v0;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    bb_data_t *test_1;
    s32 test_2;
    s32 test_3;
    u8 test_4;
    u8 test_5;
    u32 test_6;

    temp_s2 = (u16) obj->x + obj->offset_bx;
    temp_s3 = obj->offset_hy + (u16) obj->y + 0x40;
    FinAnim = ((obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1)) && (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0));
    if (((u8) Phase < 2) && (obj->main_etat != 2) && ((u32) ((ray_zdc_x - temp_s2) + 0x10) < 0x21) && (ray_zdc_y < (temp_s3 - 32))) {
        set_main_and_sub_etat(obj, 0, 16);
        obj->speed_x = 0;
        obj->speed_y = 0;
    }

    if (obj->main_etat == 1)
    {
        if (Phase == 0)
        {
            Phase = 1;
        }
        SET_X_SPEED(obj);
        if ((u8) Phase < 4U)
        {
            CALC_MOV_ON_BLOC(obj);
        }
        if ((FinAnim != 0) && (bb1.speed_x != (obj->sub_etat + 1)))
        {
            BB2_MARCHE(obj);
        }
    }
    if ((bb1.field1_0x2 < 8) && (Phase != 0) && (Phase != 5))
    {
        if (scroll_end_x == xmapmax)
        {
            scroll_end_x = xmap;
            if (xmap < temp_s2)
            {
                scroll_end_x = temp_s2;
            }
        }
        else if (scroll_end_x < temp_s2)
        {
            bb1.saved_cmd = 1;
        }
        if (bb1.saved_cmd != 0)
        {
            if (obj->main_etat == 1)
            {
                scroll_end_x = (u16) scroll_end_x + (obj->sub_etat + 1);
            }
            else
            {
                scroll_end_x = scroll_end_x + (u16) obj->speed_x;
            }
        }
    }
    else
    {
        scroll_end_x = xmapmax;
    }
    switch (Phase)
    {
        case 0:
            /* gotos-only does not fix this case */
            temp_v0 = ((u8) block_flags[obj->btypes[0]] >> BLOCK_SOLID) & 1;
            var_a0 = temp_v0 ? 40 : 60;
            if (((block_flags[obj->btypes[0]] >> BLOCK_SOLID) & 1) == 0)
            {
                if (((temp_s2 + var_a0) < ray_zdc_x) && (obj->main_etat == 0) && (bb1.speed_x != 0))
                {
                    obj->flags.flip_x = 1;
                    set_main_and_sub_etat(obj, 2, 1);
                    obj->speed_y = 2;
                    return;
                }
            }
            else if ((obj->main_etat == 0) && (obj->sub_etat == 0) && ((var_a0 + temp_s2) < ray_zdc_x))
            {
                Phase = 1;
                BB2_MARCHE(obj);
            }
            break;
        case 1:
            if ((obj->main_etat == 1) &&
                !(block_flags[mp.map[(((obj->x + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))].tile_type] >> BLOCK_SOLID & 1)
            ) {
                set_main_and_sub_etat(obj, 2U, 4U);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = -8;
                obj->speed_x = 3;
            }
            break;
        case 2:
            if (
                    (obj->main_etat == 1) &&
                    !(block_flags [
                              (u16) mp.map [
                                      (s16) (((obj->x + (obj->offset_bx - 8)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                              ].tile_type
                      ] >> 1 & 1)
                    )
            {
                set_main_and_sub_etat(obj, 2U, 1U);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = 0;
                obj->speed_x = 0;
                bb1.field1_0x2 = -1;
            }
            break;
        case 3:
            scroll_end_x = MAX(scroll_end_x, 5580);

            if (((temp_s2 + 32) < ray_zdc_x) && (bb1.field1_0x2 == -1) && (temp_s3 < ray_zdc_y))
            {
                if (bb1.speed_x != -1)
                {
                    BB2_MARCHE(obj);
                    bb1.field1_0x2 = 0;
                }
                else
                {
                    if (obj->param < 200)
                    {
                        if (++obj->param == 200)
                        {
                            bb1.speed_x = 1;
                            set_main_and_sub_etat(obj, 1, 0);
                            bb1.field1_0x2 = 0;
                            obj->param = 255;
                        }
                    }
                    scroll_end_x = xmap;
                    if (scroll_end_x < temp_s2)
                    {
                        test_2 = ray.speed_x;
                        if (test_2 < 1)
                            test_2 = 1;
                        scroll_end_x = scroll_end_x + test_2;
                    }
                }
            }
            if (FinAnim != 0)
            {
                if ((obj->main_etat == 0) && (obj->sub_etat == 16))
                {
                    BB2_MARCHE(obj);
                }
                if ((FinAnim != 0) && (obj->main_etat == 0) && (obj->sub_etat == 1))
                {
                    BB2_MARCHE(obj);
                }
            }
            if (obj->main_etat == 1)
            {
                if (!(block_flags[mp.map[(((obj->x + (obj->offset_bx - 8)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))].tile_type] >> BLOCK_SOLID & 1)) {
                    obj->speed_y = -6;
                    obj->speed_x = 3;
                    set_main_and_sub_etat(obj, 2, 4);
                    obj->gravity_value_2 = 5;
                    obj->gravity_value_1 = 0;
                }
                if ((obj->main_etat != 2) && ((u32) ((ray_zdc_x - temp_s2) + 0x10) < 0x21U))
                {
                    if (ray_zdc_y < (temp_s3 - 32))
                    {
                        set_main_and_sub_etat(obj, 0U, 16);
                        obj->speed_y = 0;
                    }
                    else
                    {
                        set_main_and_sub_etat(obj, 0U, 1);
                        obj->speed_y = 0;
                    }
                    obj->speed_x = 0;
                }
            }
            break;
        case 4:
            if (bb1.sprite6_y < scroll_end_y)
            {
                scroll_end_y -= 1;
            }
            if (FinAnim != 0)
            {
                if ((obj->main_etat == 2) && (obj->sub_etat == 2))
                {
                    bb1.speed_x = 1;
                    set_main_and_sub_etat(obj, 1, 0);
                }
                else if ((obj->main_etat == 0) && (obj->sub_etat == 5))
                {
                    if (++bb1.sprite6_x < 3)
                    {
                        bb1.speed_x = 1;
                        set_main_and_sub_etat(obj, 1, 0);
                    }
                    else
                    {
                        set_main_and_sub_etat(obj, 0, 2);
                        Phase = 5;
                        bb1.sprite6_y = ymap;
                    }
                }
            }
            if (obj->main_etat == 1)
            {
                if (!(block_flags[mp.map[(((obj->x + obj->offset_bx + (!obj->flags.flip_x ? -16 : 16)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))].tile_type] >> BLOCK_SOLID & 1)) {
                    obj->speed_y = 0;
                    obj->speed_x = 0;
                    /* see also: makeUturn() case TYPE_SPIDER_PLAFOND */
                    obj->flags.flip_x = 1 - obj->flags.flip_x;
                    set_main_and_sub_etat(obj, 0, 5);
                }
            }
            if ((temp_s2 - (ray_zdc_x + ((s16) ray_zdc_w >> 1))) >= 0)
            {
                if ((temp_s2 - (ray_zdc_x + ((s16) ray_zdc_w >> 1))) > 250)
                {
                    goto block_172;
                }
                return;
            }
            if (((ray_zdc_x + ((s16) ray_zdc_w >> 1)) - temp_s2) > 250)
            {
                block_172:
                obj->flags.alive = 0;
                IsBossThere = 0;
                Phase = 0;
                scroll_end_x = xmapmax;
                scroll_end_y = ymapmax;
                obj->is_active = 0;
            }
            break;
        case 5:
            if (bb1.field1_0x2 < 0)
            {
#ifdef PS1
                scroll_end_y = bb1.sprite6_y - horloge[5];
                bb1.field1_0x2 = bb1.field1_0x2 + 1;
#else
                ++bb1.field1_0x2;
                scroll_end_y = (bb1.field1_0x2 == -1) ? ymapmax : bb1.sprite6_y - horloge[5];
#endif
            }

            // NOTE: this part is fixed for the PC version. Not necessarily matching for the PS1 version.
            if (FinAnim && obj->main_etat == 0 && obj->sub_etat == 5) {
                bb1.speed_x = 3;
                BB2_MARCHE(obj);
            }
            if (obj->main_etat == 0 && obj->sub_etat == 2) {
                if (FinAnim) {
                    bb1.speed_x = 2;
                    BB2_MARCHE(obj);
                } else if (obj->anim_frame == 21 && (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0)) {
                    bb1.field1_0x2 = -30;
                }
            }

            if (obj->main_etat == 1) {
                if (!(block_flags[mp.map[
                        (((obj->x + obj->offset_bx + (!obj->flags.flip_x ? -16 : 16)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                    ].tile_type] >> BLOCK_SOLID & 1)
                ) {
                    if (bb1.speed_x == 3)
                    {
                        set_main_and_sub_etat(obj, 2, 4);
                        obj->gravity_value_2 = 0xFF;
                        obj->gravity_value_1 = 0;
                        obj->speed_y = -2;
                        obj->speed_x = 4;
                        scroll_end_y = ymapmax;
                        scroll_end_x = xmapmax;
                    }
                    else
                    {
                        obj->speed_y = 0;
                        obj->speed_x = 0;
                        obj->flags.flip_x = 1 - obj->flags.flip_x;
                        set_main_and_sub_etat(obj, 0, 5);
                    }
                }
            }
            if ((temp_s2 - (ray_zdc_x + ((s16) ray_zdc_w >> 1))) >= 0)
            {
                if ((temp_s2 - (ray_zdc_x + ((s16) ray_zdc_w >> 1))) > 400)
                {
                    goto block_173;
                }
                return;
            }
            if (((ray_zdc_x + ((s16) ray_zdc_w >> 1)) - temp_s2) > 400)
            {
                block_173:
                obj->flags.alive = 0;
                IsBossThere = 0;
                Phase = 0;
                scroll_end_x = xmapmax;
                scroll_end_y = ymapmax;
                obj->is_active = 0;
            }
            break;
    }
}

//22A7C
void DO_BBMONT2_MARCHE(obj_t* obj) {
    bb1.speed_x = obj->hit_points;
}

//22A98
void DO_BBMONT2_ATTER(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    if (obj->speed_y >= 2)
    {
        obj->speed_y = 0;
        obj->speed_x = 0;
        recale_position(obj);
        switch (Phase)
        {
            case 0:
                set_sub_etat(obj, 2);
                break;
            case 1:
                BB2_MARCHE(obj);
                Phase = 2;
                obj->speed_x = bb1.speed_x;
                break;
            case 2:
                set_sub_etat(obj, 2);
                Phase = 3;
                obj->x -= 32;
                bb1.speed_x = -1;
                obj->param = 0;
                break;
            case 3:
                bb1.field1_0x2 += 1;
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_x = 2;
                set_sub_etat(obj, 2);
                obj->speed_x = 0;
                bb1.sprite6_y = 691;
                bb1.sprite6_x = 0;
                scroll_end_y = ymap;
                if (ymap < 691) {
                    scroll_end_y = 691;
                }
                Phase = 4;
                obj->speed_y = 0;
                break;
        }
    }
}

//22BAC
void DO_BBMONT3_COMMAND(obj_t* obj) {
    // NOTE(Falcury): I did not spot functional differences PS1 and PC versions at first glance
    u8 eight;

    if (obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1) &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
    )
        FinAnim = true;
    else
        FinAnim = false;

    if (obj->main_etat == 1) {
        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
        if (FinAnim && (bb1.speed_x != obj->sub_etat + 1))
        {
            BB2_MARCHE(obj);
        }
    }
    switch (Phase)
    {
        case 0:
            obj->flags.flip_x = 0;
            if (obj->main_etat != 2)
                CALC_MOV_ON_BLOC(obj);

            if (FinAnim && obj->main_etat == 0)
            {
                bb1.speed_x = 2;
                set_main_and_sub_etat(obj, 1, 1);
            }
            eight = 8;
            if ((obj->main_etat == 1) &&
                ((block_flags[mp.map[
                        (((obj->x + (obj->offset_bx - eight)) >> 4) + mp.width * ((obj->y + obj->offset_by + eight) >> 4))
                ].tile_type] >> BLOCK_SOLID & 1) == 0)
            ) {
                set_main_and_sub_etat(obj, 2, 4);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = -8;
                obj->speed_x = -2;
            }
            break;
        case 1:
            if ((obj->main_etat == 1) &&
                ((block_flags[mp.map[
                        (((obj->x + (obj->offset_bx)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                ].tile_type] >> BLOCK_SOLID & 1) == 0)
            ) {
                set_main_and_sub_etat(obj, 0, 1);
                obj->speed_y = 0;
                obj->speed_x = 0;
                Phase = 2;
            }
            break;
    }
}

//22D9C
void DO_BBMONT3_ATTER(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    if (obj->speed_y >= 2) {
        obj->speed_y = 0;
        obj->speed_x = 0;
        CALC_MOV_ON_BLOC(obj);
        if (Phase == 0) {
            BB2_MARCHE(obj);
            Phase = 1;
        }
    }
}

//22DD0
void DO_BBMONT4_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    obj->flags.flip_x = 1;
    if (obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1) &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
    )
        FinAnim = true;
    else
        FinAnim = false;

    if (obj->main_etat == 0) {
        if (obj->sub_etat == 7 && obj->anim_frame == 44 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
        )
            BBMONT_ETINCELLES(obj);
        if (FinAnim && obj->sub_etat == 0)
            set_sub_etat(obj, 7);
    }
}

//22EAC
void DO_ECLAIR_COMMAND(obj_t* ecl_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;

    switch (ecl_obj->cmd) {
        case GO_LEFT:
            ecl_obj->flags.flip_x = 0;
        case GO_RIGHT:
            if (ecl_obj->cmd == GO_RIGHT)
                ecl_obj->flags.flip_x = 1;

            SET_X_SPEED(ecl_obj);
            ecl_obj->speed_y = 0;
            if (block_flags[mp.map[
                    (((ecl_obj->x + ecl_obj->offset_bx) >> 4) + mp.width * ((ecl_obj->y + ecl_obj->offset_by) >> 4))
                ].tile_type] >> BLOCK_CH_DIR & 1
            ) {
                i = 0;
                cur_obj = &level.objects[i];
                nb_objs = level.nb_objects;
                while (i < nb_objs) {
                    if (cur_obj->type == TYPE_BB1_PLAT && !cur_obj->is_active) {
                        cur_obj->flags.flip_x = 0;
                        cur_obj->speed_y = 0;
                        cur_obj->speed_x = 0;
                        cur_obj->x = ecl_obj->x - 32;
                        cur_obj->y = ecl_obj->y - 48;
                        cur_obj->init_x = cur_obj->x;
                        cur_obj->init_y = cur_obj->y;
                        calc_obj_pos(cur_obj);
                        cur_obj->iframes_timer = 255;
                        cur_obj->param = 30;
                        cur_obj->link = 30;
                        cur_obj->main_etat = 0;
                        cur_obj->sub_etat = 4;
                        cur_obj->anim_frame = 0;
                        cur_obj->cmd_offset = -1;
                        cur_obj->nb_cmd = 0;
                        cur_obj->flags.alive = 1;
                        cur_obj->is_active = 1;
#ifdef PC
                        add_alwobj(cur_obj);
#endif
                        break;
                    }
                    cur_obj++;
                    i++;
                }

                ecl_obj->is_active = 0;
                ecl_obj->flags.alive = 0;
            }
    }
}

//2300C
void DO_BB1_PLAT_CMD(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    switch (obj->cmd) {
        case GO_UP:
            obj->speed_y = -1;
            break;
        case GO_WAIT:
            obj->speed_y = 0;
            break;
    }
}
