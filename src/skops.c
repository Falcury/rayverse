//78DB0
void swap(s32 index_1, s32 index_2) {
    u8 temp = ecroule_rubis_list[index_1];
    ecroule_rubis_list[index_1] = ecroule_rubis_list[index_2];
    ecroule_rubis_list[index_2] = temp;
}

//78DD0
void set_rubis_list(void) {
    s32 rubis_count = 0;

    rubis_list_calculated = true;
    sko_nb_lave = 0;

    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = &level.objects[i];

        if (obj->type == TYPE_188_RUBIS) {
            ecroule_rubis_list[rubis_count++] = obj->id;
        } else if (obj->type == TYPE_248_SKO_PINCE) {
            sko_pince_obj_id = i;
        } else if (obj->type == TYPE_234_LAVE) {
            sko_lave_obj[sko_nb_lave++] = i;
        }
    }

    /* The encounter data always contains eight ruby platforms. */
    bool swapped;
    do {
        swapped = false;
        for (s32 i = 0; i < 7; ++i) {
            if (level.objects[ecroule_rubis_list[i]].x >
                level.objects[ecroule_rubis_list[i + 1]].x) {
                swap(i, i + 1);
                swapped = true;
            }
        }
    } while (swapped);

    /* PS1 checks demos only; PC also makes recordings deterministic. */
    if (ModeDemo != 0 || record.is_recording) {
        ecroule_plat_index = 8;
    } else {
        ecroule_plat_index = (myRand(177) % 3) << 3;
    }

    sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];
    sko_enfonce_enable = 0;
    mp.height = 32;

    /*
     * PC uses a 200-line output (limit 312), while the PS1 port uses a
     * 240-line output (limit 272). Derive the limit from the viewport.
     */
    scroll_end_y = mp.height * 16 - SCREEN_HEIGHT;
    ymapmax = mp.height * 16 - SCREEN_HEIGHT;
    sko_screen_tremble = 0;

    for (s32 i = 0; i < sko_nb_lave; ++i) {
        obj_t* lava = &level.objects[sko_lave_obj[i]];
        lava->init_y = mp.height * 16 - 60;
        lava->y = lava->init_y;
        /* PC exposes this position to its heat-haze pass; PS1 does not. */
        PosLave_Y = lava->init_y;
    }
}

//78F94
void allocate_rayon(s16 x, s16 y) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* ray_obj = &level.objects[i];
        if (ray_obj->type == TYPE_170_RAYON && !ray_obj->is_active) {
            ray_obj->flags.alive = true;
            ray_obj->is_active = true;
            add_alwobj(ray_obj);
            ray_obj->flags.flag_0x40 = false;
            ray_obj->x = x;
            ray_obj->y = y;
            ray_obj->speed_x = 0;
            ray_obj->speed_y = 0;
            set_main_etat(ray_obj, 3);
            set_sub_etat(ray_obj, 0);
            break;
        }
    }
}

//7903C
void allocate_8_petits_rayons(s16 x, s16 y) {
    obj_t* nova = allocateNOVA();
    if (nova != NULL) {
        nova->x = x + 64;
        nova->y = y + 64;
        nova->is_active = true;
        /* allocateNOVA() already registers it; the PC's repeat insertion is redundant here. */
        nova->display_prio = 1;
        nova->param = 0;
        calc_obj_pos(nova);
    }

    start_pix_gerbe(x + 140, y + 32);

    obj_t* first_ray = findfirstObject(TYPE_170_RAYON);
    if (first_ray >= level.objects + level.nb_objects) {
        return;
    }

    obj_t* ray_obj = first_ray + 10;
    for (s32 i = 0; i < 8; ++i, ++ray_obj) {
        if (!ray_obj->is_active) {
            add_alwobj(ray_obj);
        }
        ray_obj->is_active = true;
        ray_obj->flags.alive = true;
        ray_obj->x = x + 8;
        ray_obj->y = y;
        ray_obj->main_etat = 3;
        ray_obj->sub_etat = 2;
        ray_obj->speed_x = skops_ray_speed_x[i];
        ray_obj->speed_y = skops_ray_speed_y[i];
        ray_obj->anim_frame = 2;
    }
}

//79114
void do_sko_rayon(void) {
    if (sko_rayon_on == 0) {
        return;
    }

    sko_beam_speed = 3;
    --sko_rayon_on;

    obj_t* platform = &level.objects[sko_ecroule_plat];

    s16 diff_x = sko_final_x - sko_rayon_x;
    s16 diff_y = sko_final_y - sko_rayon_y;
    s16 distance = Abs(diff_x) + Abs(diff_y);

    if (distance < 8) {
        set_main_etat(platform, 2);
        set_sub_etat(platform, 2);
    }

    if (distance > 0) {
        diff_x = (diff_x * (1 << sko_beam_speed)) / distance;
        diff_y = (diff_y * (1 << sko_beam_speed)) / distance;
    }

    if (horloge[4] == 0 && sko_rayon_on < 50) {
        sko_beam_dx += sgn(diff_x - sko_beam_dx);
        sko_beam_dy += sgn(diff_y - sko_beam_dy);
    }

    sko_rayon_x += sko_beam_dx;
    sko_rayon_y += sko_beam_dy;

    if (horloge[3] == 0 && distance != 0) {
        allocate_rayon(sko_rayon_x, sko_rayon_y);
    }
}

//792AC
void do_sko_rayon2(void) {
    if (sko_rayon_on == 0) {
        return;
    }

    --sko_rayon_on;
    sko_beam_speed = 3;

    bool steer_now;
    if (poing.is_active) {
        obj_t* fist = poing_obj;
        sko_final_x = fist->x + fist->offset_bx - 104;
        sko_final_y = fist->y + fist->offset_hy - 120;
        steer_now = horloge[2] == 0;
    } else {
        sko_final_x = ray.x + ray.offset_bx - 120;
        sko_final_y = ray.y + ray.offset_by - 140;
        steer_now = horloge[8] == 0;
    }

    s16 diff_x = sko_final_x - sko_rayon_x;
    s16 diff_y = sko_final_y - sko_rayon_y;
    s16 distance = Abs(diff_x) + Abs(diff_y);

    if (distance > 0) {
        diff_x = (diff_x * (1 << sko_beam_speed)) / distance;
        diff_y = (diff_y * (1 << sko_beam_speed)) / distance;
    }

    if (steer_now) {
        sko_beam_dx += sgn(diff_x - sko_beam_dx);
        sko_beam_dy += sgn(diff_y - sko_beam_dy);
    }

    sko_rayon_x += sko_beam_dx;
    sko_rayon_y += sko_beam_dy;

    if (sko_rayon_x < -150) {
        sko_beam_dx = -sko_beam_dx;
        sko_rayon_x += sko_beam_dx * 2;
    }

    if (horloge[3] == 0 && distance != 0) {
        allocate_rayon(sko_rayon_x, sko_rayon_y);
    }

    if (sko_rayon_on == 0) {
        sko_rayon_x = OBJ_INVALID_XY;
        sko_rayon_y = OBJ_INVALID_XY;
    }
}

//794D4
void start_sko_rayon(s16 obj_x, s16 obj_y) {
    ++ecroule_plat_index;
    sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];

    obj_t* platform = &level.objects[sko_ecroule_plat];
    if (sko_rayon_on != 0xFF) {
        sko_rayon_on = 60;
    }

    sko_rayon_x = obj_x - 80;
    sko_beam_dx = -4;
    sko_rayon_y = obj_y - 10;
    sko_beam_dy = 4;
    sko_final_x = platform->x + platform->offset_bx - 120;
    sko_final_y = platform->y + platform->offset_hy - 120;
    allocate_rayon(sko_rayon_x, sko_rayon_y);
}

//795A0
void start_sko_rayon2(s16 obj_x, s16 obj_y) {
    obj_t* target = poing.is_active ? poing_obj : &ray;

    sko_rayon_on = 120;
    sko_rayon_x = obj_x - 80;
    sko_beam_dx = -4;
    sko_rayon_y = obj_y - 10;
    sko_beam_dy = 2;
    sko_final_x = target->x + target->offset_bx - 120;
    sko_final_y = target->y + target->offset_hy - 120;
    allocate_rayon(sko_rayon_x, sko_rayon_y);
}

//79638
void lance_pince(obj_t* skops_obj) {
    obj_t* claw = &level.objects[sko_pince_obj_id];
    claw->flags.alive = true;
    claw->x = skops_obj->x;
    claw->y = skops_obj->y;
    claw->speed_x = -128;
    claw->speed_y = 0;
    calc_obj_pos(claw);
    /* PC re-adds the claw to its always-object list; the PS1 path does not. */
    add_alwobj(claw);
}

//79688
s32 sko_get_eject_sens(void) {
    ray.iframes_timer = 40;
    return -1;
}

//7969C
void DO_SOL_ENFONCE(void) {
    s16 new_ymapmax = ymapmax;
    s16 new_lava_y = PosLave_Y;
    s32 tremble = sko_screen_tremble;

    /*
     * PC treats state 3/7 as a death/reset state here. The PS1 routine uses
     * state 3/52 instead, reflecting a platform-specific Rayman state table.
     */
    if (ray_mode == 3 ||
        (ray.main_etat == 2 && ray.sub_etat == 9) ||
        (ray.main_etat == 3 && (ray.sub_etat == 23 || ray.sub_etat == 7))) {
        sko_enfonce_enable = 0;
        pixels_enfonce = 0;
    }

    if (tremble != 0) {
        --tremble;
        xmap += horloge[4] >= 2 ? 2 : -2;
    }

    bool sink_ground =
        (pixels_enfonce < 96 && sko_enfonce_enable == 1 && horloge[2] == 0) ||
        (pixels_enfonce < 196 && sko_enfonce_enable == 2 && horloge[4] == 0);

    if (sink_ground) {
        xmap += horloge[4] >= 2 ? 3 : -3;
        --new_ymapmax;
        /* PS1 decrements unconditionally; PC prevents the limits crossing. */
        if (scroll_end_y > scroll_start_y) {
            --scroll_end_y;
        }
        ++pixels_enfonce;

        if ((pixels_enfonce & 0xF) == 0) {
            --mp.height;
        }

        for (s32 i = 0; i < sko_nb_lave; ++i) {
            obj_t* lava = &level.objects[sko_lave_obj[i]];
            --lava->y;
            --lava->init_y;

            if (sko_enfonce_enable == 1 && horloge[10] == 0) {
                --lava->y;
                --lava->init_y;
            }

            /* PC clamps lava to the viewport; this clamp is absent on PS1. */
            if (lava->y < new_ymapmax + 120) {
                lava->y = new_ymapmax + 120;
            }
            new_lava_y = lava->y;
        }
    }

    if ((pixels_enfonce == 96 && sko_enfonce_enable == 1) ||
        (pixels_enfonce == 196 && sko_enfonce_enable == 2)) {
        pixels_enfonce = 0;
        sko_enfonce_enable = 0;
        tremble = 60;
    }

    sko_screen_tremble = tremble;
    PosLave_Y = new_lava_y;
    ymapmax = new_ymapmax;
}

//79904
void DO_SKO_PHASE_0(obj_t* obj) {
    u8 sub_etat = obj->sub_etat;
    obj_t* platform = &level.objects[sko_ecroule_plat];

    if (obj->main_etat != 0 || sub_etat < 2) {
        return;
    }

    if (sub_etat == 2 || sub_etat == 3) {
        if (sko_last_action == 4) {
            set_sub_etat(obj, 8);
            sko_last_action = 8;
        } else {
            set_sub_etat(obj, 4);
            skipToLabel(obj, 3, true);
            sko_last_action = 4;
        }
    } else if (sub_etat == 4 && obj->anim_frame == 27 && screen_trembling == 0) {
        screen_trembling = 1;
        ++sko_nb_frap;

        if (platform->sub_etat == 7) {
            set_main_etat(platform, 2);
            set_sub_etat(platform, 2);
            ++ecroule_plat_index;
            sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];
        }

        if (sko_nb_frap == 4) {
            skipToLabel(obj, 4, true);
        }
        if (sko_nb_frap == 5) {
            sko_nb_frap = 0;
            sko_nb_hit = 0;
            ++sko_phase;
            skipToLabel(obj, 5, true);
            --ecroule_plat_index;
        }
    }
}

//79A90
void DO_SKO_PHASE_1(obj_t* obj) {
    if (obj->main_etat == 0 && obj->sub_etat == 4 &&
        obj->anim_frame == 27 && screen_trembling == 0) {
        screen_trembling = 1;
        ++sko_nb_frap;

        if (ray.main_etat == 5) {
            set_main_etat(&ray, 2);
            set_sub_etat(&ray, 2);
        }

        if (sko_nb_frap == 3) {
            sko_rayon_on = 0;
            ++sko_phase;
            skipToLabel(obj, 6, true);
            sko_nb_frap = 0;
            obj->anim_frame = 0;
        }
    }
}

//79B5C
void DO_SKO_PHASE_2(obj_t* obj) {
    u8 main_etat = obj->main_etat;
    u8 sub_etat = obj->sub_etat;

    do_sko_rayon();

    if (main_etat == 0) {
        switch (sub_etat) {
            case 4:
                if (sko_nb_frap == 0 && obj->anim_frame == 27) {
                    ++sko_nb_frap;
                    sko_enfonce_enable = 1;
                } else if (sko_nb_frap == 1 && obj->anim_frame == 27) {
                    ++sko_nb_frap;
                    screen_trembling = 1;
                }
                break;

            case 2:
            case 3:
                set_sub_etat(obj, 5);
                break;

            case 7:
                if (obj->anim_frame == 9 && sko_rayon_on == 0) {
                    start_sko_rayon(obj->x, obj->y);
                    if (++sko_nb_frap == 5) {
                        skipToLabel(obj, 7, true);
                        sko_nb_frap = 0;
                        ++sko_phase;
                    }
                }
                break;
        }
    } else if (main_etat == 1 && sub_etat == 15 && obj->nb_cmd == sko_nb_hit) {
        obj->nb_cmd = 0;
    }
}

//79C98
void DO_SKO_PHASE_3(obj_t* obj) {
    u8 main_etat = obj->main_etat;
    u8 sub_etat = obj->sub_etat;

    do_sko_rayon();

    if (main_etat == 0) {
        if (sub_etat == 4 && obj->anim_frame == 27 && sko_nb_frap == 1) {
            ++sko_nb_frap;
            sko_enfonce_enable = 2;
        } else if (sub_etat == 2) {
            if (sko_nb_frap == 0) {
                ++sko_nb_frap;
                skipToLabel(obj, 11, true);
            } else if (sko_nb_frap == 2) {
                set_main_etat(obj, 1);
                set_sub_etat(obj, 15);
                obj->speed_x = 3;
            }
        }
    }
}

//79D48
void DO_SKO_PINCE(obj_t* obj) {
    if (obj->main_etat != 0) {
        return;
    }

    if (obj->sub_etat == 8 && obj->anim_frame == 65) {
        lance_pince(obj);
        set_sub_etat(obj, 9);
        skipToLabel(obj, 9, true);
    } else if (obj->sub_etat == 9) {
        obj_t* claw = &level.objects[sko_pince_obj_id];
        claw->speed_x += 2;
        if (claw->speed_x == 110) {
            set_sub_etat(obj, 10);
        }
    } else if (obj->sub_etat == 10 && obj->anim_frame == 7) {
        obj_t* claw = &level.objects[sko_pince_obj_id];
        claw->flags.alive = false;
        claw->x = OBJ_INVALID_XY;
        claw->y = OBJ_INVALID_XY;
    }
}

//79E18
void DO_SCORPION_COLLISION(obj_t* obj) {
    s16 collision = -1;

    if (!(obj->main_etat == 0 && (obj->sub_etat == 11 || obj->sub_etat == 12))) {
        collision = BOX_IN_COLL_ZONES(
            TYPE_150_SCORPION,
            sko_rayon_x + 120,
            sko_rayon_y + 120,
            16,
            16,
            obj
        );
    }

    if (collision != -1) {
        set_sub_etat(obj, 11);
        allocate_8_petits_rayons(sko_rayon_x, sko_rayon_y);
        sko_rayon_x = OBJ_INVALID_XY;
        sko_rayon_y = OBJ_INVALID_XY;
        sko_rayon_on = 0;
        --obj->hit_points;

        if (obj->hit_points == 1) {
            skipToLabel(obj, 13, true);
        }
        if (obj->hit_points == 0) {
            set_sub_etat(obj, 12);
            /* Present in the PC executable; omitted by the PS1 implementation. */
            boss_mort = true;
        }
    }
}

//79F24
void DO_SCORPION_MORT(obj_t* obj) {
    finBosslevel.mr_skops = true;
    if (obj->main_etat == 0 && obj->sub_etat == 12 && obj->anim_frame >= 127) {
        obj->anim_frame = 129;
        fin_boss = true;
        TEST_SIGNPOST();
    }
}

//79F64
void DO_SKO(obj_t* obj) {
    u8 main_etat = obj->main_etat;
    u8 sub_etat = obj->sub_etat;

    if (main_etat == 0 && sub_etat == 1) {
        if (obj->anim_frame == 0) {
            PlaySnd(172, obj->id);
        }
        if (obj->anim_frame == 50) {
            PlaySnd(173, obj->id);
        }
        if (obj->anim_frame == 100) {
            PlaySnd(177, obj->id);
        }
    }

    if (num_level == 10) {
        DO_ONE_CMD(obj);
        if (!rubis_list_calculated) {
            set_rubis_list();
        }
        DO_SKO_PINCE(obj);
        DO_SOL_ENFONCE();

        if (obj->x + obj->offset_bx < ray.x && ray_mode != 3) {
            RAY_HIT(true, obj);
        }

        switch (sko_phase) {
            case 0:
                DO_SKO_PHASE_0(obj);
                break;
            case 1:
                DO_SKO_PHASE_1(obj);
                break;
            case 2:
                DO_SKO_PHASE_2(obj);
                break;
            case 3:
                DO_SKO_PHASE_3(obj);
                break;
        }
    } else if (num_level == 11) {
        if (!rubis_list_calculated) {
            scrollLocked = true;
            obj->hit_points = obj->init_hit_points;

            for (s32 i = 0; i < level.nb_objects; ++i) {
                if (level.objects[i].type == TYPE_248_SKO_PINCE) {
                    sko_pince_obj_id = i;
                    break;
                }
            }

            skipToLabel(obj, 12, true);
            set_main_etat(obj, 0);
            set_sub_etat(obj, 5);
            rubis_list_calculated = true;
            sko_phase = 4;
        }

        DO_ONE_CMD(obj);
        do_sko_rayon2();
        DO_SKO_PINCE(obj);
        DO_SCORPION_COLLISION(obj);
        DO_SCORPION_MORT(obj);

        /* Deliberately use the state captured before DO_ONE_CMD, as the PC does. */
        if (main_etat == 0) {
            if (sub_etat == 7 && obj->anim_frame == 9 && sko_rayon_on == 0) {
                sko_rayon_on = 0xFF;
                start_sko_rayon2(obj->x, obj->y);
            } else if (sub_etat == 2 || sub_etat == 3) {
                if (sko_last_action == 5) {
                    set_sub_etat(obj, 8);
                    sko_last_action = 8;
                } else {
                    set_sub_etat(obj, 5);
                    sko_last_action = 5;
                }
            }
        }
    }
}

//7A1DC
void SKO_ray_in_zone(obj_t* obj) {
    u8 main_etat = obj->main_etat;
    u8 sub_etat = obj->sub_etat;

    if (sko_phase == 0 && main_etat == 0) {
        if (sub_etat == 0) {
            set_sub_etat(obj, 1);
            skipToLabel(obj, 2, true);
            sko_nb_frap = 0;
        } else if (sub_etat == 2 || sub_etat == 3) {
            set_sub_etat(obj, 4);
            skipToLabel(obj, 3, true);
        }
    }
}

//7A264
void DO_SKO_HIT(obj_t* obj, s16 sprite) {
    (void)sprite;

    u8 main_etat = obj->main_etat;
    u8 sub_etat = obj->sub_etat;

    if (sko_phase == 0 && main_etat == 0) {
        switch (sub_etat) {
            case 0:
                set_sub_etat(obj, 1);
                skipToLabel(obj, 2, true);
                obj->change_anim_mode = ANIMMODE_NONE;
                obj->anim_frame = 81;
                sko_nb_frap = 0;
                break;

            case 1:
                obj->anim_frame = MAX(obj->anim_frame, 81);
                break;

            case 2:
            case 3:
                set_sub_etat(obj, 4);
                skipToLabel(obj, 3, true);
                break;
        }
    } else if (sko_phase == 1 && main_etat == 0) {
        obj->x += 2;
        ++sko_nb_hit;
        if (sub_etat == 2 || sub_etat == 3) {
            obj->nb_cmd = 0;
        }
    }
}

//7A338
s16 Get_PosLave_Y(void) {
    return PosLave_Y;
}
