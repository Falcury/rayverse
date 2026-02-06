
//32A70
void PlaceDarkPhase1et2(obj_t* obj) {
    print_once("Not implemented: PlaceDarkPhase1et2"); //stub
}

//32AA8
void dark_attaque_suivante(obj_t* obj) {
    print_once("Not implemented: dark_attaque_suivante"); //stub
}

//32AD8
void init_vitraux(void) {
    print_once("Not implemented: init_vitraux"); //stub
}

//32B90
void poing_face_obj(obj_t* obj) {
    print_once("Not implemented: poing_face_obj"); //stub
}

//32BD4
void DARK_phase1(obj_t* obj) {
    print_once("Not implemented: DARK_phase1"); //stub
}

//32FAC
void DARK_phase3(obj_t* obj) {
    print_once("Not implemented: DARK_phase3"); //stub
}

//331FC
void DO_DARK_COMMAND(obj_t* mr_drk_obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s16 dark_x; s16 dark_y; s16 dark_w; s16 dark_h;
    s16 sort_x; s16 sort_y;

    if (mr_drk_obj->main_etat == 0) {
        if (mr_drk_obj->sub_etat == 39) {
            switch (dark_phase) {
                case 0:
                case 1:
                    goto_phase1(mr_drk_obj);
                    break;
                case 2:
                    goto_phase2(mr_drk_obj);
                    break;
                case 3:
                    goto_phase3(mr_drk_obj);
                    break;
                case 5:
                    goto_phase5(mr_drk_obj);
                    break;
            }
        }

        if (mr_drk_obj->main_etat == 0) {
            if (mr_drk_obj->sub_etat == 6) {
                if (mr_drk_obj->anim_frame == 4) {
                    mr_drk_obj->speed_y = -4;
                } else {
                    if (dark_phase == 4) {
                        if (EOA(mr_drk_obj)) {
                            goto_phase5(mr_drk_obj);
                        }
                    } else {
                        if (EOA(mr_drk_obj)) {
                            mr_drk_obj->is_active = 0;
                            mr_drk_obj->flags.alive = 0;
                        }
                    }
                }
            }
            else if (
                (
                    mr_drk_obj->sub_etat == 8 || mr_drk_obj->sub_etat == 10 || mr_drk_obj->sub_etat == 12 ||
                    mr_drk_obj->sub_etat == 14 || mr_drk_obj->sub_etat == 22 || mr_drk_obj->sub_etat == 23 ||
                    mr_drk_obj->sub_etat == 24
                ) &&
                mr_drk_obj->anim_frame == 22
            ) {
                GET_SPRITE_POS(mr_drk_obj, 3, &dark_x, &dark_y, &dark_w, &dark_h);
                dark_rayon_dx = -5;
                dark_rayon_dy = 0;
                sort_x = dark_x + (dark_w >> 1) - 96;
                sort_y = dark_y + (dark_h >> 1) - 128;
                switch (mr_drk_obj->sub_etat) {
                    case 8:
                        allocate_DARK_SORT(sort_x, sort_y, 0, 1);
                        break;
                    case 10:
                        allocate_DARK_SORT(sort_x, sort_y, 1, 1);
                        break;
                    case 12:
                        allocate_DARK_SORT(sort_x, sort_y, 2, 1);
                        break;
                    case 14:
                        allocate_DARK_SORT(sort_x, sort_y, 3, 1);
                        break;
                    case 22:
                        byte_DF74C = 2;
                        allume_vitraux((u8(*)[5]) vitrail_clignotement[0]);
                        allocate_DARK_SORT(sort_x, sort_y, 18, 3);
                        allocate_DARK_SORT(sort_x, sort_y, 18, 5);
                        break;
                    case 23:
                        byte_DF74C = 2;
                        allume_vitraux((u8(*)[5]) vitrail_clignotement[1]);
                        allocate_DARK_SORT(sort_x, sort_y, 19, 1);
                        allocate_DARK_SORT(sort_x, sort_y, 19, 4);
                        break;
                    case 24:
                        byte_DF74C = 2;
                        allume_vitraux((u8(*)[5]) vitrail_clignotement[2]);
                        allocate_DARK_SORT(sort_x, sort_y, 20, 1);
                        allocate_DARK_SORT(sort_x, sort_y, 20, 2);
                        break;
                }
            } else {
                if (mr_drk_obj->main_etat == 0) {
                    if (mr_drk_obj->sub_etat == 15) {
                        if (mr_drk_obj->anim_frame == 0) {
                            if (horloge[mr_drk_obj->eta[mr_drk_obj->main_etat][mr_drk_obj->sub_etat].anim_speed & 0xF] == 0) {
                                DO_NOVA2(mr_drk_obj);
                                mr_drk_obj->flags.flip_x = 1;
                                mr_drk_obj->flags.alive = 1;
                                mr_drk_obj->is_active = 1;
                            }
                        } else if (mr_drk_obj->anim_frame == 22) {
                            GET_SPRITE_POS(mr_drk_obj, 3, &dark_x, &dark_y, &dark_w, &dark_h);
                            dark_rayon_dx = -2;
                            dark_rayon_dy = 0;
                            allocate_DARK_SORT((s16) (dark_x + (dark_w >> 1) - 96), (s16) (dark_y + (dark_h >> 1) - 128), 4, 1);
                        }
                    } else if (mr_drk_obj->sub_etat == 21) {
                        DARK_phase3(mr_drk_obj);
                    }
                    else if (
                        mr_drk_obj->sub_etat == 26 || mr_drk_obj->sub_etat == 27 ||
                        mr_drk_obj->sub_etat == 28 || mr_drk_obj->sub_etat == 29 ||
                        mr_drk_obj->sub_etat == 40
                    ) {
                        DARK_phase1(mr_drk_obj);
                    }
                }
            }
        }
    }
}

//33630
void DO_DARK_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (obj->main_etat == 0 && (obj->sub_etat == 16 || obj->sub_etat == 17) && obj->display_prio != 0) {
        obj->hit_points--;
        if (obj->hit_points == 0) {
            set_main_and_sub_etat(obj, 0, 6);
        }
    }
}

//33670
void DO_DARK_REACT(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (obj->main_etat == 0) {
        if (obj->sub_etat == 7)
            set_sub_etat(obj, 8);
        if (obj->sub_etat == 9)
            set_sub_etat(obj, 10);
        if (obj->sub_etat == 11)
            set_sub_etat(obj, 12);
        if (obj->sub_etat == 13)
            set_sub_etat(obj, 14);
    }
}

//336C8
void DO_DARK_SORT_COMMAND(obj_t* ds_obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    obj_t *br_obj;
    s16 diff_x; s16 diff_y;
    s16 dist;

    if (ds_obj->hit_points != 0) {
        if (ds_obj->sub_etat == 5) {
            if (fin_boss == true) {
                ds_obj->hit_points = 0;
            }
        }
        else if (ds_obj->sub_etat == 4) {
            br_obj = &level.objects[black_ray_obj_id];
            if (ds_obj->iframes_timer == 1) {
                ds_obj->iframes_timer = 0;
                br_obj->hit_points = 0xFE;
                br_obj->flags.flip_x = 0;
                set_main_and_sub_etat(br_obj, 0, 0);
                br_obj->x = ray.x; //0;
                br_obj->y = ray.y; //0;
                br_obj->init_x = ray.x;
                br_obj->init_y = ray.y;
                br_obj->display_prio = 0;
                ray_stack_is_full = false;
                ray_pos_in_stack = 0;
                black_pos_in_stack = 0;
                br_obj->flags.alive = 1;
                br_obj->is_active = 1;
#ifdef PC
                add_alwobj(br_obj);
#endif
            }

            diff_x = (br_obj->init_x + br_obj->offset_bx) - (ds_obj->x + ds_obj->offset_bx);
            diff_y = (br_obj->init_y + br_obj->offset_by) - (ds_obj->y + ds_obj->offset_by);
            if (ray_stack_is_full == true && black_pos_in_stack == 25) {
                ds_obj->hit_points = 0;
            }
        } else {
            ds_obj->iframes_timer = 0;
            diff_x = (ray.x + ray.offset_bx) - (ds_obj->x + ds_obj->offset_bx);
            if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40) {
                diff_y = (ray.y + ray.offset_by) - (ds_obj->y + ds_obj->offset_by);
            } else {
                diff_y = (ray.y + ray.offset_hy + ((ray.offset_by - ray.offset_hy) >> 1)) - (ds_obj->y + ds_obj->offset_by);
            }
        }

        dist = Abs(diff_x) + Abs(diff_y);
        if (dist > 0) {
            diff_x = ashl16(diff_x, 4) / dist;
            diff_y = ashl16(diff_y, 4) / dist;
        }

        if (horloge[2] == 0) {
            dark_rayon_dx += SGN(diff_x - dark_rayon_dx);
            dark_rayon_dy += SGN(diff_y - dark_rayon_dy);
            allocate_DARK_SORT(
                    ds_obj->x + dark_rayon_dx,
                    ds_obj->y + dark_rayon_dy,
                    ds_obj->sub_etat,
                    ds_obj->iframes_timer
            );
            ds_obj->hit_points = 0;
        }
    }
}

//33980
void DO_DARK_SORT_COLLISION(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (obj->hit_points != 0 && !(obj->main_etat == 0 && obj->sub_etat == 4)) {
        obj->hit_points = 0;
        switch (obj->sub_etat) {
            case 0:
                RayEvts.force_run = 1;
                break;
            case 1:
                DO_NOVA(&ray);
                RayEvts.reverse = 0;
                RAY_REVERSE_COMMANDS();
                break;
            case 2:
                DO_NOVA(&ray);
                RAY_DEMIRAY();
                break;
            case 3:
                DO_NOVA(&ray);
                RayEvts.poing = false;
                break;
        }
    }
}

//33A18
void allocate_DARK_SORT(s16 x, s16 y, s16 sub_etat, s16 iframes) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s16 unk_1 = 0;
    s16 unk_2;
    obj_t *cur_obj = &level.objects[0];
    s16 i = 0;
    s16 nb_objs = level.nb_objects;

    do {
        if (cur_obj->type == TYPE_DARK_SORT && !cur_obj->is_active)
            unk_1 = 1;
        cur_obj++;
        i++;
    } while (unk_1 == 0 && i < nb_objs);
    cur_obj--;
    if (unk_1 != 0)
    {
        cur_obj->x = x;
        cur_obj->y = y;
        cur_obj->hit_points = 1;
        cur_obj->iframes_timer = iframes;
        cur_obj->param = 0;
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
#ifdef PC
        add_alwobj(cur_obj);
#endif
        set_main_and_sub_etat(cur_obj, 0, sub_etat);
        cur_obj->anim_frame = 0;
    } else {
        unk_2 = 0;
        cur_obj = &level.objects[0];
        i = 0;
        unk_1 = 0;
        do {
            if (cur_obj->type == TYPE_DARK_SORT && unk_2 < cur_obj->anim_frame) {
                unk_1 = i;
                unk_2 = cur_obj->anim_frame;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
        cur_obj = &level.objects[unk_1];
        cur_obj->x = x;
        cur_obj->y = y;
        cur_obj->hit_points = 1;
        cur_obj->iframes_timer = iframes;
        cur_obj->param = 0;
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
#ifdef PC
        add_alwobj(cur_obj);
#endif
        set_main_and_sub_etat(cur_obj, 0, sub_etat);
        cur_obj->anim_frame = 0;
    }
}

//33B4C
void DoAnnuleDarkSortRaymanCollision(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_COLLISIONS(). Seems to be functionally identical.
    if (RayEvts.tiny || RayEvts.force_run != 0 || RayEvts.reverse != 0) {
        DO_NOVA(&ray);
        if (RayEvts.force_run != 0)
            RayEvts.force_run = 3;
        if (RayEvts.reverse != 0)
            RAY_REVERSE_COMMANDS();
        if (RayEvts.tiny)
            RAY_DEMIRAY();
        obj->flags.alive = 0;
        obj->is_active = 0;
    }
}

//33BBC
void corde_en_bas(obj_t* obj) {
    print_once("Not implemented: corde_en_bas"); //stub
}

//33C00
void corde_en_haut(obj_t* obj) {
    print_once("Not implemented: corde_en_haut"); //stub
}

//33CB4
void init_corde(obj_t* obj) {
    print_once("Not implemented: init_corde"); //stub
}

//33D28
void goto_phase1(obj_t* obj) {
    print_once("Not implemented: goto_phase1"); //stub
}

//33DD8
void goto_phase2(obj_t* obj) {
    print_once("Not implemented: goto_phase2"); //stub
}

//33E5C
void goto_phase3(obj_t* obj) {
    print_once("Not implemented: goto_phase3"); //stub
}

//33F28
void goto_phase5(obj_t* obj) {
    print_once("Not implemented: goto_phase5"); //stub
}

//34024
void DO_VITRAIL_COMMAND(obj_t* obj) {
    print_once("Not implemented: DO_VITRAIL_COMMAND"); //stub
}

//34094
void allume_vitraux(u8 (*param_1)[5]) {
    print_once("Not implemented: allume_vitraux"); //stub
}
