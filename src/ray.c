
//6BFD0
void allocateRayLandingSmoke(void) {
    //stub
}

//6C004
void recale_ray_on_liane(void) {
    //stub
}

//6C088
void calc_bhand_typ(void) {
    //stub
}

//6C138
void IS_RAY_ON_LIANE(void) {
    //stub
}

//6C224
void rayMayLandOnAnObject(u8* a1, i16 obj_id) {
    //stub
}

//6CA70
void set_air_speed(u8 a1, u8 a2, i16 a3, i32 a4) {
    //stub
}

//6CAF8
void Reset_air_speed(u8 a1) {
    //stub
}

//6CCE0
void determineRayAirInertia(void) {
    //stub
}

//6CD40
void ray_jump(void) {
    //stub
}

//6D020
void ray_inertia_speed(u8 a1, u8 a2, i16 a3, i16 a4) {
    //stub
}

//6D2E8
void RAY_SWIP(void) {
    //stub
}

//6D724
void RAY_STOP(void) {
    //stub
}

//6D7D4
void RAY_HELICO(void) {
    //stub
}

//6DB1C
void Make_Ray_Hang(i16 a1, i16 a2) {
    //stub
}

//6DC24
bool AIR(i32 a1) {
    return false; //stub
}

//6DC4C
bool MUR(i32 a1) {
    return false; //stub
}

//6DC78
void CAN_RAY_HANG_BLOC(void) {
    //stub
}

//6DF28
void RAY_TOMBE(void) {
    //stub
}

//6E030
void RAY_RESPOND_TO_DOWN(void) {
    //stub
}

//6E370
void RAY_RESPOND_TO_UP(void) {
    //stub
}

//6E548
void RAY_RESPOND_TO_DIR(void) {
    //stub
}

//6EB40
void RAY_RESPOND_TO_NOTHING(void) {
    //stub
}

//6F0B0
void RAY_RESPOND_TO_BUTTON3(void) {
    //stub
}

//6F1C8
void RAY_RESPOND_TO_FIRE0(void) {
    //stub
}

//6F208
void RAY_RESPOND_TO_FIRE1(void) {
    //stub
}

//6F24C
void RAY_BALANCE_ANIM(i16 a1) {
    //stub
}

//6F29C
void SET_RAY_BALANCE(void) {
    //stub
}

//6F328
void RAY_GOING_BALANCE(obj_t* obj) {
    //stub
}

//6F500
void RAY_BALANCE(void) {
    //stub
}

//6F7D0
void RAY_FIN_BALANCE(void) {
    //stub
}

//6F870
void RayTestBlocSH(void) {
    /* 621FC 801869FC -O2 -msoft-float */
    s16 x_1;
    s16 y_1;
    s16 temp_v1_2;
    s16 var_a1;
    s32 var_v0;
    s32 temp_v1;
    s32 var_a0;
    u32 var_v0_2;

    x_1 = ray.x + ray.offset_bx;
    var_a1 = x_1;
    y_1 = ray.y + ray.offset_hy;
    var_v0 = x_1;
    if (x_1 < 0)
        var_v0 = x_1 + 15;
    temp_v1 = x_1 - (var_v0 >> 4 << 4);
    if (decalage_en_cours > 0 || (decalage_en_cours == 0 && ray.flags.flip_x)) {
        var_a0 = 16;
        if ((u8) temp_v1 >= 14)
            var_a1 = x_1 + 16;
    } else {
        var_a0 = -16;
        if ((u8) temp_v1 < 4)
            var_a1 -= 16;
    }
    temp_v1_2 = var_a1 + var_a0;
    if (
            decalage_en_cours != 0 &&
            (MURDUR(temp_v1_2, y_1 + 16) ||
             MURDUR(temp_v1_2, y_1 + 32) ||
             MURDUR(temp_v1_2, y_1 + 48))
            )
    {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    if (ray.speed_y < 0 && MURDUR(x_1, y_1 + 16))
        ray.speed_y = 0;
}

//6F95C
void remoteControlRay(void) {
    i32 diff_x = ray.x - remoteRayXToReach;
    if (diff_x > 1) {
        ray.flags.flip_x = 0;
        set_sub_etat(&ray, 21);
    } else if (diff_x < -1) {
        ray.flags.flip_x = 1;
        set_sub_etat(&ray, 21);
    } else {
        ray.speed_x = 0;
        set_sub_etat(&ray, 20);
        remoteRayXToReach = (i16)ray.x;
    }
}

//6F9E0
void STOPPE_RAY_CONTRE_PAROIS(u8 block) {
    if (ray_mode != 3 && (block_flags[block] & 0x10)) {

    }
}

//6FA44
void RAY_IN_THE_AIR(void) {
    if (ray_wind_force) {
        ray.nb_cmd = 1;
    }
    memmove(&pos_stack[1], &pos_stack[0], sizeof(pos_stack) - sizeof(i16));
    pos_stack[0] = (i16)ray.x;

    if (ray.sub_etat == 7) {
        ray.cmd_arg_2 = -1;
    }

    bool may_land_obj = true;
    if (helico_time != -1) {
        --helico_time;
    }
    if (ray_between_clic != -1) {
        --ray_between_clic;
    }
    ++jump_time;

    u8 anim_speed = get_eta(&ray)->anim_speed >> 4;
    if (anim_speed != 10 && anim_speed != 11) {
        ++ray.gravity_value_1;
        if (ray.gravity_value_1 >= 3) {
            ray.gravity_value_1 = 0;
        }
        ++ray.gravity_value_2;
        if (ray.gravity_value_2 >= 4) {
            ray.gravity_value_2 = 0;
        }
    }

    if (jump_time == 23) {
        ++ray.speed_y;
    }
    if (!options_jeu.test_fire1() || jump_time > 12 || in_air_because_hit || ray.timer != 0 || ray.sub_etat == 2) {
        DO_PESANTEUR(&ray);
        if ((button_released % 2) == 0 && !options_jeu.test_fire1()) {
            ++button_released;
        }
    }
    if (jump_time == 6) {
        if (ray.sub_etat == 24 || ray.sub_etat == 33) {
            set_sub_etat(&ray, 2);
        } else if (ray.sub_etat == 32) {
            set_sub_etat(&ray, 17);
        }
    }

    if (ray.link != -1) {
        ray.link += ray.speed_y;
    }
    u8 block = calc_typ_trav(&ray, 2);
    if (ray.sub_etat != 8 && ray.sub_etat != 31) {
        RAY_HELICO(); //TODO
        if (ray.sub_etat == 15) {
            RayTestBlocSH();
        }
    }
    STOPPE_RAY_CONTRE_PAROIS(block);

    //stub
}

//7008C
void terminateFistWhenRayDies(void) {
    if (poing.is_active) {
        DO_NOVA(poing_obj);
        switch_off_fist();
    }
}

//700A4
void snifRayIsDead(obj_t* obj) {
    //stub
}

//7010C
void rayfallsinwater(void) {
    /* 62FA4 801877A4 -O2 -msoft-float */
    set_main_and_sub_etat(&ray, 3, 22);
    decalage_en_cours = 0;
    ray.anim_frame = 0;
    ray.speed_y = 0;
    ray.speed_x = 0;
    decalage_en_cours = 0;
    if (ray_on_poelle) {
        ray.x -= h_scroll_speed;
    }
    h_scroll_speed = 0;
    dead_time = 1;
    ray.iframes_timer = 120;
    ray_se_noie = true;
    allocate_splash(&ray);
    terminateFistWhenRayDies();
}

//701A8
u8 RAY_DEAD(void) {
    if (ray.x < LEFT_MAP_BORDER - 10 ||
            (ray.screen_y + ray.offset_by < -20 && scroll_y == -1) ||
            ray.x > RIGHT_MAP_BORDER + 10 ||
            ray.screen_y > SCREEN_HEIGHT - 20
    ) {
        ray.hit_points = 0;
        RAY_HIT(1, 0); //TODO
    } else {
        if (!(ray.main_etat == 2 && ray.sub_etat == 9) &&
                !(ray.main_etat == 3 && ray.sub_etat == 22) &&
                ray.flags.alive
        ) {
            if (ray.coll_btype[0] == BTYP_WATER && ray.cmd_arg_2 == -1 && ray.main_etat != 6) {
                rayfallsinwater();
            } else if (ray.y + ray.offset_by > (mp.height + 1) * 16) {
                set_main_and_sub_etat(&ray, 2, 9);
                if (ray.speed_y < 0) {
                    ray.speed_y = 0;
                }
            }
        }
        if ((ray.main_etat == 2 && ray.sub_etat == 9) ||
                (ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32) && EOA(&ray))
        ) {
            --dead_time;
            if (dead_time != 0 || fin_du_jeu) {
                ray.cmd_arg_2 = -1;
            } else {
                ray.hit_points = 2;
                if (ray.is_active) {
                    ray.is_active = 0;
                    ray.flags.alive = 0;
                    dead_time = 64;
                    snifRayIsDead(&ray); //TODO
                    if (status_bar.lives < 0) {
                        ray.hit_points = 0;
                        fin_du_jeu = 1;
                        status_bar.lives = 0;
                    }
                }
                status_bar.max_hitp = ray.hit_points;
                ray.iframes_timer = 90;
            }
        }
    }
    bool alive = ray.is_active && ray.flags.alive && ray_mode != 3 && ray_mode != 4;
    return alive;
}

//70408
void RAY_HURT(void) {
    //stub
}

//704B0
void RepousseRay(void) {
    //stub
}

//70598
u8 RayEstIlBloque(void) {
    return 0; //stub
}

//7089C
void stackRay(void) {
    //stub
}

//70964
void RAY_SURF(void) {
    //stub
}

//70A8C
void DO_SURF_CHANGE_HAIRS(void) {
    //stub
}

//70AE4
void DO_PIEDS_RAYMAN(obj_t* obj) {
    //stub
}

//70AE8
void DO_MORT_DE_RAY(void) {
    //stub
}

//70C20
void TEST_FIN_FOLLOW(void) {
    //stub
}

//70D58
void RAY_FOLLOW(void) {
    //stub
}

//70E14
void RAY_RESPOND_TO_ALL_DIRS(void) {
    //stub
}

//70FF8
void DO_RAY_ON_MS(void) {
    //stub
}

//7133C
void DO_RAYMAN(void) {
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    setvol(-1);
    if (RAY_DEAD()) {
        ray.ray_dist = ((ray.y + ray.offset_by) >> 4) * mp.width + ((ray.x + ray.offset_bx) >> 4);
        COLL_RAY_BLK_MORTEL();
        if (scroll_y != -1 && ray.screen_y + ray.offset_bx < 0) {
            ray.speed_y = 0;
        }
        if (ray.iframes_timer == -1 && get_eta(&ray)->flags & 8) {
            if (COLL_RAY_PIC()) { // TODO
                RAY_HIT(1, 0);
            }
        }

        if (RayEvts.squashed && ray.iframes_timer == -1 && ray.scale == 0) {
            Ray_RayEcrase(); // TODO
        }

        joy_done = 0;
        calc_obj_pos(&ray);
        if (ray.cmd_arg_2 == -1) {
            calc_btyp(&ray);
        } else {
            ray.speed_y = 0;
            ray.speed_x = 0;
        }

        if (!(remoteRayXToReach == -32000 || get_eta(&ray)->flags & 0x40)) {
            if ((ray.main_etat == 0 && !(ray.sub_etat == 4 || ray.sub_etat == 5 || ray.sub_etat == 6 ||
                                         ray.sub_etat == 7 || ray.sub_etat == 9 || ray.sub_etat == 10)) ||
                (ray.main_etat == 1 && !(ray.sub_etat == 1)) ||
                (ray.main_etat == 3 &&
                 !(ray.sub_etat == 1 || (ray.sub_etat == 2 || ray.sub_etat == 3) || ray.sub_etat == 4))
                    ) {
                if (remoteRayXToReach != 0) {
                    set_main_and_sub_etat(&ray, 3, 21);
                } else {
                    set_main_and_sub_etat(&ray, 3, 20);
                }
            }
        }

        if ((ray.main_etat == 2 && ray.sub_etat == 8) || (ray.main_etat == 2 && ray.sub_etat == 31)) {
            RAY_IN_THE_AIR(); //TODO
        } else if (ray.main_etat == 3) {
            if (ray.sub_etat == 16) {
                DO_GROWING_PLATFORM(); //TODO
                if (eau_obj_id != -1) {
                    level.objects[eau_obj_id].iframes_timer = 1;
                }
            } else if (ray.sub_etat == 21 || ray.sub_etat == 20) {
                remoteControlRay();
            }

            if (!(ray.sub_etat == 22 || ray.sub_etat == 23 || ray.sub_etat == 32)) {
                RAY_SWIP(); //TODO
            }
        } else {
            if (options_jeu.test_fire1()) {
                RAY_RESPOND_TO_FIRE1(); // jump //TODO
            }
            if (options_jeu.test_fire0()) {
                RAY_RESPOND_TO_FIRE0(); // weapon //TODO
            }
            if (options_jeu.test_button3()) {
                RAY_RESPOND_TO_BUTTON3(); // action //TODO
            }
            if (options_jeu.test_button4()) {
                //RAY_RESPOND_TO_BUTTON4(); //unused; NOTE: nullsub present in the Android version optimized out?
            }

            if (record.is_playing && butX1pressed() && butX0pressed()) {
                ChangeLevel();
            }

            if (poing.is_charging) {
                if (ray.sub_etat == 12) {
                    RAY_GROW_FIST();
                }
                if (!options_jeu.test_fire0()) {
                    RAY_THROW_FIST(); //TODO
                }
            }

            if (ray.main_etat == 7) {
                RAY_BALANCE(); //TODO
            } else if (ray.main_etat == 2) {
                RAY_IN_THE_AIR(); //TODO
            } else if (!options_jeu.test_fire1()) {
                button_released = 1;
            }
            RAY_RESPOND_TO_ALL_DIRS(); //TODO
        }

        if (RayEvts.reverse != 0) {
            i16 ray_x, ray_y, ray_w, ray_h;
            GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h);
            // TODO: initialize star_ray_der and star_ray_dey
            star_ray_der->x = star_ray_dev->x = ray_x + (ray_w >> 1) - star_ray_dev->offset_bx;
            star_ray_der->y = star_ray_dev->y = ray_y - star_ray_dev->offset_hy;

            if (!star_ray_der->is_active) {
                add_alwobj(star_ray_der);
            }
            if (!star_ray_dev->is_active) {
                add_alwobj(star_ray_dev);
            }
        } else if (RayEvts.force_run != 0) {
            if (RayEvts.force_run == 1) {
                RayEvts.force_run = 2;
                RayEvts.run = 1;
                DO_NOVA(&ray); //TODO
                set_main_and_sub_etat(&ray, 1, 3);
            } else if (!((ray.main_etat == 1 && (ray.sub_etat == 3 || ray.sub_etat == 7)) || ray.main_etat == 2)) {
                RayEvts.run = 1;
                set_main_and_sub_etat(&ray, 1, 3);
            } else if (RayEvts.force_run == 3 && (ray.main_etat != 2)) {
                RayEvts.force_run = 0;
            }
        }

        if (ray.cmd_arg_2 != -1) {
            RAY_FOLLOW(); //TODO
        }

        if (RayEstIlBloque()) { //TODO
            if (ray.main_etat == 7) {
                RAY_FIN_BALANCE(); //TODO
            }
            ray.speed_x = 0;
            ray.nb_cmd = 0;
            decalage_en_cours = 0;
        }

        if (pierreAcorde_obj_id != -1) {
            RepousseRay(); //TODO
        }

        RAY_SURF(); //TODO
        STOPPE_RAY_EN_XY(); //TODO

        if (ray.speed_y <= 0) {
            if (ray.speed_y < 0) {
                move_up_ray(); //TODO
            }
        } else {
            move_down_ray(); //TODO
        }
        if (ray.speed_x >= 0) {
            if (ray.speed_x > 0) {
                RAY_TO_THE_RIGHT(); //TODO
            }
        } else {
            RAY_TO_THE_LEFT(); //TODO
        }

        if (ray.flags.alive && ray.main_etat != 7) {
            DO_ANIM(&ray);
            DO_SURF_CHANGE_HAIRS(); //TODO
        }

        GET_RAY_ZDC(&ray, &ray_zdc_x, &ray_zdc_y, &ray_zdc_w, &ray_zdc_h); //TODO
        DO_COLLISIONS(); //TODO
        if (!fin_boss) {
            TEST_SIGNPOST(); //TODO
        }
    }

    stackRay(); //TODO
}

//71920
void INIT_PC(void) {
    init_memory(&main_mem_tmp, 0x2EE00);
}

//71934
void FIN_PC(void) {
    free(main_mem_tmp);
    main_mem_tmp = NULL;
}

//71940
void updateLogo(i32 fade_duration, i32 a2, i32 a3) {
    start_fade_in(2);
    WaitNSynchro(5);
    memcpy(DrawBufferNormal, PLAN2BIT, 320*200);
    //DisplayBufferModeNormal(DrawBufferNormal, display_buffer, current_rvb);
    for (i32 i = 0; i < fade_duration; ++i) {
        do_fade(&rvb_pres, &current_rvb);
        advance_frame();
        readinput();
        if (TOUCHE(SC_SPACE) || but0pressed() || but1pressed()) {
            goto end;
        }
    }
    // TODO: wait for fixed duration if CD music not available
    while (is_ogg_playing) {
        advance_frame();
        readinput();
        if (TOUCHE(SC_SPACE) || but0pressed() || but1pressed()) {
            goto end;
        }
    }
    end:
    fade_out(2, &rvb_pres);
    WaitNSynchro(1);
}

//71A70
void LOAD_SCREEN(void) {
    LoadPlan2InVignet(main_mem_tmp, 29);
}

//71A84
void sub_71A84(void) {
    LoadPlan2InVignet(main_mem_tmp, 31);
}

//71A98
void sub_71A98(void) {
    //stub
}

//71B34
void DO_UBI_LOGO(void) {
    INIT_PC();
    SetCompteurTrameAudio();
    current_pal_id = 0;
    LOAD_SCREEN();
    INIT_FADE_IN();
    play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
    updateLogo(60, -1, 8);
    FIN_PC();
    stop_cd();
}




