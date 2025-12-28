
//200B0
void BB2_MARCHE(obj_t* obj) {
    print_once("Not implemented: BB2_MARCHE"); //stub
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
    /* 58784 8017CF84 -O2 -msoft-float */
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
    obj->flags.alive = 0;
    obj->is_active = 0;
    bb1.field1_0x2 = -1;
    bb1.speed_x = 0;
    bb1.saved_cmd = 0;
    bb1.field2_0x4 = 0;
    bb1.field8_0xe = 1;
}

//20CF8
void DO_BBL_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_BBL_COMMAND"); //stub
}

//20F38
void BBMONT_ECLAIR(obj_t* obj) {
    print_once("Not implemented: BBMONT_ECLAIR"); //stub
}

//21098
void BBMONT_ETINCELLES(obj_t* obj) {
    print_once("Not implemented: BBMONT_ETINCELLES"); //stub
}

//212C4
void BBMONT_TIRE(obj_t* obj) {
    print_once("Not implemented: BBMONT_TIRE"); //stub
}

//212D4
void Cree_BBL(obj_t* obj) {
    print_once("Not implemented: Cree_BBL"); //stub
}

//21300
void BB_Attaque(obj_t* obj) {
    print_once("Not implemented: BB_Attaque"); //stub
}

//21400
void Fin_BB_Attaque(obj_t* obj) {
    print_once("Not implemented: Fin_BB_Attaque"); //stub
}

//214B8
void BB_Atan(obj_t* obj) {
    print_once("Not implemented: BB_Atan"); //stub
}

//214F8
void DO_BBMONT_ATTER(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT_ATTER"); //stub
}

//21568
void DO_BBMONT_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT_COMMAND"); //stub
}

//22050
void DO_BBMONT_TOUCHE(obj_t* obj, s16 sprite) {
    print_once("Not implemented: DO_BBMONT_TOUCHE"); //stub
}

//22094
void DO_BBMONT2_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT2_COMMAND"); //stub
}

//22A7C
void DO_BBMONT2_MARCHE(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT2_MARCHE"); //stub
}

//22A98
void DO_BBMONT2_ATTER(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT2_ATTER"); //stub
}

//22BAC
void DO_BBMONT3_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT3_COMMAND"); //stub
}

//22D9C
void DO_BBMONT3_ATTER(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT3_ATTER"); //stub
}

//22DD0
void DO_BBMONT4_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_BBMONT4_COMMAND"); //stub
}

//22EAC
void DO_ECLAIR_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_ECLAIR_COMMAND"); //stub
}

//2300C
void DO_BB1_PLAT_CMD(obj_t* obj) {
    print_once("Not implemented: DO_BB1_PLAT_CMD"); //stub
}
