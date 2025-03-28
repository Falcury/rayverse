
//55480
void INIT_HORLOGES(void) {
    memset(horloge, 0, sizeof(horloge));
}

//55498
void horloges(u8 ticks) {
    for (i32 i = 0; i < 25; ++i) {
        horloge[i] += 1;
        if (horloge[i] >= i) {
            horloge[i] = 0;
        }
    }
    map_time += ticks;
}

//554EC
void init_allowed_time(void) {
    for (i32 i = 0; i < 192; ++i) {
        allowed_time[i] = -2;
    }
    i16* allowed_time_world = allowed_time;
    allowed_time_world[17] = 30; //magician (swinging plums)
    allowed_time_world[18] = 45; //magician (flower platforms)
    allowed_time_world[19] = 25; //magician (grass platforms)
    allowed_time_world[20] = 35; //magician (low flower platforms)
    allowed_time_world += 32;
    allowed_time_world[16] = 40; //magician (slippery platforms)
    allowed_time_world[17] = 25; //magician (rotating platforms)
    allowed_time_world += 32;
    allowed_time_world[11] = 45; //magician (disappearing clouds)
    allowed_time_world[12] = 20; //magician (bouncing clouds)
    allowed_time_world += 32;
    allowed_time_world[11] = 35; //magician (parkour)
    allowed_time_world[12] = 50; //magician (bouncy erasers)
    allowed_time_world += 32;
    allowed_time_world[11] = 35; //magician (ring parkour)
}

//55AC8
void fades(void) {
    if (map_time == 0) {
        INIT_FADE_IN();
    }
    if (dead_time >= 32 && dead_time <= display_mode + 33) {
        if (fade == 65) {
            INIT_FADE_OUT();
        }
    }
    if ((fade & 0x40) == 0) {
        DO_FADE();
    }
}

//55B24
i16 frapsol(i16 a1) {
    return ashr16(expsin[a1], 7);
}

//55B48
void DO_SCREEN_TREMBLE(void) {
    //stub
}

//55B90
void DO_SCREEN_TREMBLE3(void) {
    //stub
}

//55BCC
void DO_SCREEN_TREMBLE2(void) {
    //stub
}

//55C08
void DO_SCROLL(i16* a1, i16* a2) {
    //stub
}

//55D78
void allocateLandingSmoke(obj_t* obj) {
    //stub
}

//55E60
void instantSpeed(i16 a1) {
    //stub
}

//55EE4
void SET_X_SPEED(obj_t* obj) {
    i32 xspeed = 0;
    anim_t* anim = obj->animations + obj->anim_index;
    u8 horloge_index = ((anim->layers_per_frame & 0xC000) >> 14) + 1;
    if (horloge[horloge_index] == 0) {
        eta_t* eta = get_eta(obj);
        if (obj->flags.flip_x) {
            xspeed = eta->right_speed * horloge_index;
        } else {
            xspeed = eta->left_speed * horloge_index;
        }
        if (obj->type == TYPE_23_RAYMAN && RayEvts.tiny) {
            xspeed /= 2;
        }
    }
    obj->speed_x = (i16)xspeed;
}

//55F9C
void make_active2(obj_t* obj, u8 do_nova) {
    obj->is_active = 0;
    obj->field_1C = 1;
    obj->active_flag = 0;
    if (do_nova) {
        DO_NOVA(obj);
    }
}


//55FBC
void make_active(obj_t* obj, u8 do_nova) {
    if (obj->flags.alive) {
        obj->is_active = 0;
        obj->field_1C = 1;
        obj->active_flag = 0;
        if (do_nova) {
            DO_NOVA(obj);
        }
    }
}

//55FE4
bool in_action_zone(i16 screen_x, i16 screen_y, obj_t* obj, u8 active) {
    i32 in_zone = 0;
    i16 zdiff_x = zonediffx[ot];
    i16 zdiff_y = zonediffy[ot];
    u8 offset_bx = obj->offset_bx;
    if (flags[ot] & flags0_0x10_keep_active) {
        if ((flags[ot] & flags0_0x80_boss) && IsBossThere) {
            in_zone = 1;
        } else {
            //TODO: check that this is right for the PC version, there seem to be some oddities here but PC/Android sadly don't decompile cleanly
            switch(ot) {
                case TYPE_153_FEE:
                case TYPE_94_POING:
                case TYPE_120_BATTEUR_FOU:
                case TYPE_204_BLACK_RAY:
                case TYPE_205_BLACK_FIST:
                case TYPE_190_COUTEAU:
                case TYPE_215_DARK_SORT:
                case TYPE_143_NOVA2:
                    in_zone = 1;
                    break;
                case TYPE_10_FISH:
                case TYPE_97_BBL:
                    if ((u16) (screen_x + 283) < 727)
                        in_zone = 1;
                    break;
                case TYPE_220_PIERREACORDE:
                    if (((s16) screen_y + zdiff_y > -50) && (u16) (screen_x + 283) < 783)
                    {
                        if (level.objects[pierreAcorde_obj_id].hit_points == 0)
                        {
                            level.objects[pierreAcorde_obj_id].hit_points = 10;
                            set_main_and_sub_etat(&level.objects[pierreAcorde_obj_id], 2, 4);
                        }
                    }
                case TYPE_219_CORDE:
                    if (level.objects[pierreAcorde_obj_id].hit_points != 0)
                        in_zone = 1;
                    break;
                case TYPE_157_EAU:
                    if ((u16) (screen_x + 283) < 783)
                        in_zone = 1;
                    break;
                case TYPE_122_STONEDOG2:
                    if (((u16) (screen_x + 283) < 727) && ((u16) (screen_y + 99) < 349))
                        in_zone = 1;
                    break;
                case TYPE_244_SMA_PETIT_LASER:
                case TYPE_192_SMA_GRAND_LASER:
                    offset_bx = 128;
                    break;
                // NOTE: the below cases only seems to be present in the PS1 version?
                /*case TYPE_75_PAILLETTE:
                case TYPE_76_DESTROYING_DOOR:
                    if (((u16) (obj->y - 50) > 320) || !IsBossThere)
                    {
                        if (ray.cmd_arg_2 == obj->id)
                        {
                            ray.cmd_arg_2 = -1;
                            obj->ray_dist = 1000;
                            set_main_and_sub_etat(&ray, 2, 2);
                        }
                        in_zone = 2;
                    }
                    else
                        in_zone = 1;
                    break;*/
                default: break;
            }
        }
    }

    if (in_zone == 0) {
        if (active) {
            zdiff_x += 60;
            zdiff_y += 60;
        }
        in_zone = 1;
        if (((s16) screen_x < -284 - zdiff_x) || ((s16) screen_x > zdiff_x + 444) ||
                ((s16) screen_y < -144 - zdiff_y) || ((s16) screen_y > zdiff_y + 304) ||
                (obj->y + obj->offset_by < -30) || (obj->y > mp.height * 16 + 30) ||
                (obj->x + offset_bx * 2 < 0) || (obj->x > mp.width * 16)
        ) {
            in_zone = 0;
        }
    }

    if (in_zone && (flags[ot] & flags0_0x80_boss)) {
        IsBossThere = true;
        if ((!first_boss_meet && obj->hit_points != 0 && scrollLocked) || (first_boss_meet && obj->hit_points == 0)) {
            Change_Wait_Anim();
        }
    }

    return in_zone;
}

//56310
void kill_obj(obj_t* obj) {
    obj->x = -32000;
    obj->y = -32000;
    obj->flags.alive = 0;
    obj->active_flag = 1;
}

//56338
void SET_ACTIVE_FLAG(i16 screen_x, i16 screen_y, obj_t* obj) {
    ot = obj->type;
    i32 was_active = obj->is_active;
    obj->is_active = 0;
    if (obj->flags.alive) {
        if (in_action_zone(screen_x, screen_y, obj, was_active)) {
            if (obj->active_timer != 0 ||
                    (!(flags[ot] & flags1_1_keep_linked_objects_active) &&
                    obj->active_flag >= 2 &&
                     (obj->active_flag == 2 || ot != TYPE_10_FISH))
            ) {
                if (obj->active_timer > 0) {
                    --obj->active_timer;
                }
            } else {
                obj->is_active = was_active;
                if ((flags[ot] & flags0_1_always) || (flags[ot] & flags3_0x40_no_link)) {
                    make_active2(obj, !obj->is_active && (flags[ot] & flags1_1_keep_linked_objects_active));
                } else {
                    i16 id = obj->id;
                    do {
                        obj_t* cur_obj = level.objects + id;
                        make_active(cur_obj, !cur_obj->is_active && (flags[obj->type] & flags1_1_keep_linked_objects_active));
                        id = link_init[cur_obj->id];
                    } while (id != obj->id);
                }
            }
        } else {
            if (obj->active_flag != 1) {
                obj->active_timer = 0;
                if (flags[ot] & flags2_0x20_kill_if_outside_active_zone) {
                    kill_obj(obj);
                } else if (obj->active_flag == 2) {
                    obj->active_flag = 1;
                } else if (obj->active_flag == 0 || obj->active_flag == 4) {
                    if (flags[ot] & flags0_1_always) {
                        REINIT_OBJECT(obj);
                    } else {
                        obj->active_flag = 4;
                        obj->is_active = 1;

                        bool dont_reinit_linked_objects = false;
                        if (obj->type != TYPE_10_FISH) {
                            i16 id = obj->id;
                            while (1) {
                                obj_t* cur_obj = level.objects + id;
                                if (cur_obj->active_flag != 4) {
                                    dont_reinit_linked_objects = true;
                                    break;
                                } else {
                                    id = link_init[cur_obj->id];
                                    if (id == obj->id) {
                                        break;
                                    }
                                }
                            }
                        }

                        if (obj->y + obj->offset_by < -30) {
                            obj->is_active = 0;
                        }

                        if (!dont_reinit_linked_objects) {
                            i16 id = obj->id;
                            do {
                                obj_t* cur_obj = level.objects + id;
                                REINIT_OBJECT(cur_obj);
                                id = link_init[cur_obj->id];
                            } while (id != obj->id);
                        }
                    }
                }
            }
        }
    }
}

//565D0
void DO_PESANTEUR(obj_t* obj) {
    //stub
}

//56734
void freezeAnim(obj_t* obj, u8 a2) {
    //stub
}

//567AC
void DO_ANIM(obj_t* obj) {
    i32 prev_anim_frame = obj->anim_frame;
    i32 prev_anim_index = obj->anim_index;
    eta_t* eta = get_eta(obj);
    anim_t* anim = obj->animations + obj->anim_index;
    u8 anim_speed = eta->anim_speed & 15;
    if (anim_speed != 0 && horloge[anim_speed] == 0) {
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            --obj->anim_frame;
        } else {
            ++obj->anim_frame;
        }
    }
    obj->anim_index = eta->anim_index;
    anim = obj->animations + obj->anim_index;
    if ((obj->change_anim_mode == 1 && obj->anim_index != prev_anim_index) || obj->change_anim_mode == 2) {
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->id);
        }
    }
    if (obj->anim_frame >= anim->frame_count || obj->anim_frame == 255) {
        // animation ended
        obj->main_etat = eta->next_etat;
        obj->sub_etat = eta->next_subetat;
        eta = get_eta(obj);
        obj->anim_index = eta->anim_index;
        anim = obj->animations + obj->anim_index;
        if ((obj->type == TYPE_23_RAYMAN && (ray_old_etat == 2 || ray_old_etat == 6)) ||
            (obj->sub_etat == 61 && ray_old_subetat == 61 && ray_old_etat == 0)) {
            if (ray.timer > 60 && !RayEvts.squashed) {
                ray.timer = 60;
            }
        }
        if (eta->flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->id);
        }
    }
    obj->change_anim_mode = 0;
    if (obj->flags.follow_enabled) {
        //calc_follow_sprite_speed(); //STUB
    }
    u8 anim_changed_bit = 0;
    if (obj->anim_frame != prev_anim_frame || obj->anim_index != prev_anim_index) {
        anim_changed_bit = 1;
    }
    obj->flags.anim_changed = anim_changed_bit;

}


//56A38
void prof_in_bloc(obj_t* obj) {
    //stub
}

//56A4C
void do_boing(obj_t* obj, u8 a2, u8 a3) {
    //stub
}

//56BC4
void underSlope(obj_t* obj) {
    //stub
}

//56BEC
void DO_STONEBOMB_REBOND(obj_t* obj) {
    //stub
}

//56CD8
void DO_THROWN_BOMD_REBOND(obj_t* obj, i16 a2, i16 a3) {
    //stub
}

//56F00
void DO_FRUIT_REBOND(obj_t* obj, i16 a2, i16 a3) {
    //stub
}

//5720C
void Drop_Atter(obj_t* obj) {
    //stub
}

//57230
void BadGuyAtter(obj_t* obj) {
    //stub
}

//572C4
void MiteAtter(obj_t* obj) {
    //stub
}

//572F8
void LidolPinkAtter(obj_t* obj) {
    //stub
}

//57344
void stoneDogAtter(obj_t* obj) {
    //stub
}

//5736C
void stoneDogBounces(obj_t* obj) {
    //stub
}

//574B8
void Spider_Atter(obj_t* obj) {
    //stub
}

//574F4
void NormalAtter(obj_t* obj) {
    //stub
}

//57530
void OBJ_IN_THE_AIR(obj_t* obj) {
    //stub
}

//57FEC
void test_fall_in_water(obj_t* obj) {
    //stub
}

//58084
void MOVE_OBJECT(obj_t* obj) {
    //stub
}

//58270
void DO_RAY_IN_ZONE(obj_t* obj) {
    //stub
}

//5829C
void DoRaymanInZDDDefault(obj_t* obj) {
    //nullsub
}

//582A0
void DO_ONE_OBJECT(obj_t* obj) {
    if (flags[ot] & flags0_2_balle) {
        DO_BALLE(obj); //TODO
    }
    if (flags[ot] & flags0_0x20_has_detect_zone) {
        SET_DETECT_ZONE_FLAG(obj); //TODO
    }
    if (flags[ot] & flags2_1_check_tile_type) {
        calc_btyp(obj);
    }
    if (flags[ot] & flags1_0x80_read_cmd) {
        GET_OBJ_CMD(obj);
    } else {
        obj->cmd = GO_NOP;
    }
    ObjectsFonctions[ot].command(obj);
    if (obj->main_etat == 2) {
        OBJ_IN_THE_AIR(obj); //TODO
    }
    if (obj->flags.follow_enabled) {
        SET_RAY_DIST(obj); //TODO
    } else {
        obj->ray_dist = 10000;
    }
    if (flags[ot] & flags1_0x40_special_platform) {
        DO_SPECIAL_PLATFORM(obj); //TODO
    }
    if (flags[ot] & flags2_0x20_kill_if_outside_active_zone) {
        DO_RAY_IN_ZONE(obj); //TODO
    }
}

//58388
void build_active_table(void) {
    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    i32 screen_center_x = xmap + 160;
    i32 screen_center_y = ymap + 100;
    for (i32 i = 0; i < level_obj.nb_objects; ++i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        obj->field_1C = 0;
    }

    for (i32 i = level_obj.nb_objects-1; i >= 0; --i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        if (obj->field_1C == 0) {
            if ((abs(obj->x - screen_center_x) < 660 && abs(obj->y - screen_center_y) < 600) || (flags[obj->type] & flags2_0x10_do_not_check_ray_collision)) {
                SET_ACTIVE_FLAG(obj->x - xmap + 8, obj->y - ymap, obj);
            } else {
                obj->is_active = 0;
            }
        }
    }

    for (i32 i = level_obj.nb_objects-1; i >= 0; --i) {
        obj_t* obj = &level.objects[level_obj.obj_ids[i]];
        if (obj->is_active) {
            obj->screen_x = obj->x - xmap + 8;
            obj->screen_y = obj->y - ymap;
            actobj.objects[actobj.num_active_objects++] = obj->id;
            if (actobj.num_active_objects > COUNT(actobj.objects)) {
                FatalError("Error On MAX_ACTOBJ.");
            }
        } else {
            if (flags[obj->type] & flags2_1_check_tile_type) {
                del_alwobj(obj->id);
            }
        }
    }
    if (actobj.num_active_objects < COUNT(actobj.objects)) {
        actobj.objects[actobj.num_active_objects] = -1;
    }
}

//58644
void Add_One_RAY_lives(obj_t* obj) {
    //stub
}

//58680
void DO_CLING_ANIMS(void) {
    //stub
}

//58754
void DO_OBJECTS_ANIMS(void) {
    //stub
}

//587C8
void DO_OBJECTS(void) {
    if (id_Cling_1up == -1 && id_Cling_Pow == -1) {
        DO_CLING_ANIMS(); //TODO
    }
    if (lidol_to_allocate != 0) {
        allocate_toons(lidol_source_obj, 7); //TODO
        lidol_to_allocate = 0;
    }
    for (i32 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = level.objects + actobj.objects[i];
        ot = obj->type;
        if ((flags[ot] & flags3_8_switch_off) ||
            (ot == TYPE_161_WIZ && obj->sub_etat == 2) ||
            (ot == TYPE_83_EXPLOSION && obj->sub_etat == 1) ||
            (ot == TYPE_33_DARK2_SORT && obj->sub_etat == 35)
        ) {
            switchOff(obj);
        }
        if (ot != TYPE_161_WIZ && obj->is_active) {
            DO_ONE_OBJECT(obj); //TODO
            if (!(flags[ot] & flags0_0x80_boss) && obj->hit_points == 0 && (get_eta(obj)->flags & 8)) {
                ++obj->hit_points;
            }
        }
    }
}

//58908
void MOVE_OBJECTS(void) {
    //stub
}

//58964
void RECALE_ALL_OBJECTS(void) {
    //stub
}

//58AAC
void RayCoince(i16 a1) {
    //stub
}

//58DE8
void move_up_ray(void) {
    //stub
}

//58EC8
void move_down_ray(void) {
    //stub
}

//58F70
void recale_ray_pos(void) {
    //stub
}

//59418
void RAY_TO_THE_RIGHT(void) {
    //stub
}

//5949C
void RAY_TO_THE_LEFT(void) {
    //stub
}

//59518
void DO_FIXE(void) {
    //stub
}

//595E8
void deactivate_ship_links(void) {
    //stub
}

//5965C
void linkListHoldsAGendoor(obj_t* obj) {
    //stub
}

//596C0
void correct_gendoor_link(u8 a1) {
    for (i16 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (obj->type == TYPE_164_GENERATING_DOOR) {
            i16 link = link_init[i];
            i16 last_link = i;

            // NOTE: I'm not too sure about this part, might need checking
            while (link != i && link != last_link) {
                if (obj->flags.alive && obj->type == TYPE_164_GENERATING_DOOR) {
                    level.objects[link].flags.alive = 0;
                }
                last_link = link;
                link = link_init[link];
            }

            if (link == i) {
                link_init[last_link] = link_init[i];
                if (last_link == link_init[i]) {
                    level.objects[last_link].link_has_gendoor = 0;
                }
                obj->link = link;
                obj->link_has_gendoor = 0;
            }

            if (a1 && !obj->flags.alive) {
                i16 init_link = obj->link;
                i16 linked_obj_id;
                do {
                    linked_obj_id = link_init[init_link];
                    suppressFromLinkList(level.objects + linked_obj_id); //TODO
                } while (init_link != linked_obj_id);
            }

        }
    }
}

//597FC
void suppressFromLinkList(obj_t* obj) {
    //stub
}

//59888
void correct_link(void) {
    //stub
}

//59900
void INIT_RAY_BEGIN(void) {
    RayEvts.super_helico = 0;
    RayEvts.magicseed = 0;
    RayEvts.tiny = 0;
    status_bar.max_hitp = 2;
    status_bar.num_wiz = 0;
    fin_continue = 0;
    ray.flags.flag_1 = false;
}

//59948
void INIT_RAY(u8 new_lvl) {
    gele = 0;
    compteur_attente = 0;
    ray_mode = (ray.main_etat == 6) + 1;
    if (RayEvts.tiny) {
        rms.hit_points = ray.hit_points;
        ray = rms;
    }
    xmap = 0;
    ymap = 0;
    ray.type = TYPE_23_RAYMAN;
    ray.screen_x = 0;
    ray.screen_y = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;
    ray.x = 100;
    ray.offset_hy = 20;
    ray.y = 10;
    ray.anim_index = 0;
    ray.anim_frame = 0;
    ray.main_etat = 2;
    ray.sub_etat = 2;
    ray.configuration = 0;
    ray.timer = 0;
    ray.offset_bx = 80;
    ray.offset_by = 78;
    ray.cmd_arg_2 = -1;
    ray.link = -1;
    ray.iframes_timer = -1;
    ray.is_active = 1;
    ray.flags.alive = true;
    ray.flags.flip_x = true;
    ray.flags.flag_0x40 = false;
    ray_stack_is_full = 0;
    no_ray_landing_smoke = 0;
    in_air_because_hit = 0;
    remoteRayXToReach = -32000;
    ray_last_ground_btyp = 1;
    ray_pos_in_stack = 0;
    ray_se_noie = 0;
    ray_in_fee_zone = 1;
    fin_dark = 0;
    for (i32 i = 0; i < 10; ++i) {
        pos_stack[i] = ray.x;
    }
    poing.is_active = 0;
    poing.is_charging = 0;
    if (new_lvl) {
        RayEvts.super_helico = 0;
    }
    if (!(num_world == world_1_jungle && num_level == 9) && !ModeDemo && !record.is_recording) {
        RayEvts.magicseed = 0;
    }
    if (RayEvts.squashed) {
        RayEvts.tiny = RayEvts.squashed;
        Ray_RayEcrase(); //TODO
    }
    RayEvts.tiny = 0;
    RayEvts.force_run = 0;
    if (save1.rayevts_reverse && save1.save_obj_id != -1) {
        RayEvts.reverse = 0;
        if (num_world == world_6_cake && num_level == 3) {
            RAY_REVERSE_COMMANDS(); //TODO
        }
    } else {
        RayEvts.reverse = 1;
        RAY_REVERSE_COMMANDS(); //TODO
    }

    if (level.objects && level.nb_objects > 0) {

        obj_t* obj = level.objects;
        for (i32 i = 0; i < level.nb_objects; ++i) {
            if (obj->type == TYPE_99_RAY_POS) {
                break;
            }
            ++obj;
        }
        // NOTE: if the rayman start position doesn't exist, the first object is used instead
        if (!fin_continue) {
            ray.flags.alive = false;
            ray.is_active = 0;
        }
        xmap = obj->x + obj->offset_bx - 160;
        ymap = obj->y - 10;
        if (GameModeVideo) {
            if (xmap >= xmapmax) {
                xmap = xmapmax;
            }
            if (ymap >= ymapmax) {
                ymap = ymapmax;
            }
            if (xmap < 0) {
                xmap = 0;
            }
            if (ymap < 0) {
                ymap = 0;
            }
        } else {
            set_xymap();
        }
        ray.x = obj->x + obj->offset_bx - ray.offset_bx;
        ray.y = obj->y + obj->offset_by - ray.offset_by;
    }

    if (RaymanDansUneMapDuJeu) {
        calc_btyp(&ray);
    } else {
        for (i32 i = 0; i < 5; ++i) {
            ray.coll_btype[i] = 0;
        }
    }
}

//59CF4
void is_icy_pente(u8 a1) {
    //stub
}

//59D18
void STOPPE_RAY_EN_XY(void) {
    //stub
}

//59E9C
void DO_PLACE_RAY(void) {
    //stub
}

//59FB8
void DO_AUTO_SCROLL(void) {
    //stub
}

//5A1E0
void INIT_MOTEUR(u8 new_lvl) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        level.objects[i].field_1C = 0;
    }

    memset(pix_gerbe, 0, sizeof(pix_gerbe));

    if (!fin_continue) {
        init_flocons();
    }
    if (num_world == world_1_jungle && num_level == 9) {
        set_SNSEQ_list(11);
    }
    scrollLocked = 0;
    loop_time = 0;
    dans_la_map_monde = 0;
    sko_rayon_on = 0;
    rubis_list_calculated = 0;
    Vignet_To_Display = 0;
    sko_last_action = 0;
    sko_enfonce_enable = 0;
    joe_exp_probleme = finBosslevel.helped_joe_2;
    pixels_enfonce = 0;
    if (num_world == world_5_cave && num_level == 10) {
        mp.height = 32;
        scroll_end_y = 312;
        ymapmax = 312;
    }
    in_pause = 0;
    map_time = 0;
    left_time = -2;
    INIT_HORLOGES();
    ray.id = -1;
    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    xmap = 0;
    ymap = 0;

    if (GameModeVideo != 0) {
        ymapmax = 16 * mp.height - 200;
        xmapmax = 16 * mp.width - 305;
        if (xmapmax < 0) {
            xmapmax = 0;
        }
        if (ymapmax < 0) {
            ymapmax = 0;
        }
        scroll_start_x = 0;
        scroll_end_x = xmapmax;
        scroll_start_y = 0;
        scroll_end_y = ymapmax;
    } else {
        set_xymapini();
    }
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    dvspeed = 0;
    dhspeed = 0;
    jump_time = 0;
    gerbe = 0;
    decalage_en_cours = 0;
    ray_wind_force = 0;
    screen_trembling = 0;
    helico_time = -1;
    weather_wind = 0;
    lidol_to_allocate = 0;
    fin_boss = 0;
    boss_mort = 0;
    TextDark2_Affiche = 0;
    if (!fin_du_jeu) {
        dead_time = 64;
    }
    INIT_RAY(new_lvl);
    if (!bonus_map && departlevel && save1.is_just_saved) {
        i16 saved_tings = status_bar.num_wiz;
        restoreGameState(&save1);
        status_bar.num_wiz = saved_tings;
    }
    if (departlevel) {
        nb_wiz_collected = 0;
    } else {
        save1.is_just_saved = save_save;
        ray.x = ray_X_main_pos_init;
        ray.y = ray_Y_main_pos_init;
        ray.is_active = 1;
        ray.flags.alive = 1;
        current_pal_id = pal_id_at_wizard + 1;
        actualize_palette(pal_id_at_wizard);
        xmap = xmapsave;
        ymap = ymapsave;
    }
    INIT_OBJECTS(new_lvl);
    correct_link(); //TODO
    deactivate_ship_links(); //TODO
    special_flags_init();
    if (num_world_choice == world_5_cave && num_level_choice == 4 && finBosslevel.helped_joe_1) {
        RayEvts.firefly = 1;
    } else {
        RayEvts.firefly = 0;
    }
    if (RayEvts.firefly) {
        INIT_LUCIOLE(); //TODO
    }
    DO_OBJECTS(); //TODO
    if (mp.width <= 20 && (SizeScreen == 0 || GameModeVideo == 1)) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    if (type_fndscrX != 0) {
        scroll_end_x = scroll_start_x;
        xmap = scroll_start_x;
    }
}

//5A5B4
void INIT_MOTEUR_BEGIN(void) {
    init_allowed_time();
    init_bonus_perfect();
    init_calcbloc_func();
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hit_points = 2;
    gele = 0;
    departlevel = 1;
    poing.sub_etat = 1;
    nb_continue = 9;
    You_Win = 0;
    set_proj_center(160, 170);
    INIT_RAY_BEGIN();
    scroll_x = -1;
    scroll_y = -1;
    special_ray_mov_win_x_left = 0;
    special_ray_mov_win_x_right = 0;
    fin_de_rayman = 0;
    NumDemo = myRand(5);
    if (DemoRecordWorld[NumDemo] == 5 && DemoRecordMap[NumDemo] == 10) {
        ++NumDemo;
        if (NumDemo == COUNT(DemoRecordWorld)) {
            NumDemo = 0;
        }
    }
    First_Hist = 1;
    First_Menu = 1;
    left_time = -2;
    life_becoz_wiz = 0;
    dontdoit = 0;
    RunTimeDemo = 1800;
}

//5A6D8
void INIT_MOTEUR_WORLD(void) {
    new_world = 0;
    new_level = 1;
}

//5A6F4
void INIT_MOTEUR_LEVEL(i16 a1) {
    Bloc_lim_W1_Aff = Bloc_lim_W1;
    Bloc_lim_W2_Aff = Bloc_lim_W2;
    Bloc_lim_H1_Aff = Bloc_lim_H1;
    Bloc_lim_H2_Aff = Bloc_lim_H2;
    if (!fin_du_jeu) {
        bonus_map = (get_allowed_time() != -2);
        bool need_save_game_state = (!bonus_map && departlevel && !fin_continue);
        if (need_save_game_state) {
            initGameSave();
            MapAvecPluieOuNeige = 0;
        }
        INIT_MOTEUR(a1); //TODO
        if (need_save_game_state) {
            saveGameState(NULL, &save1);
        }
        if (departlevel) {
            nb_wiz_collected = 0;
        } else  {
            departlevel = 1;
            restore_objects_flags();
        }
        correct_gendoor_link(0);
        ray_mode = 1;
        RAY_MODE_SPEED = 16;
        new_level = 0;
        build_active_table();
    }
}

//5A7DC
void restore_gendoor_link(void) {
    //stub
}

//5A8E4
void DONE_MOTEUR_LEVEL(void) {
    //stub
}

//5A9D8
void INIT_MOTEUR_DEAD(void) {
    if (dead_time == 0) {
        restore_gendoor_link(); //TODO
        INIT_MOTEUR(false);
        correct_gendoor_link(0);
    }
    if (!bonus_map && departlevel) {
        saveGameState(NULL, &save2);
        life_becoz_wiz = 0;
    }
}

//5AA20
void INIT_RAY_ON_MS(void) {
    if (NewMs) {
        if (rayman_obj_id == -1) {
            if (rayOnMsWasThere) {
                INIT_RAY(true);
                NewMs = 0;
            }
        } else {
            rms = ray;
            ray = level.objects[rayman_obj_id];
            INIT_RAY(true);
            ray.hit_points = rms.hit_points;
            ray.init_hit_points = rms.init_hit_points;
            ray.hit_sprite = rms.hit_sprite;
            set_main_and_sub_etat(&ray, 6, 0);
            ray_mode = 2; //MODE_RAY_ON_MS
            NewMs = 0;
        }
    }
}

//5AAE8
void DO_MOTEUR(void) {
    //stub
}

//5AC70
void DO_MOTEUR2(void) {
    //stub
}

//5B0A4
void RAY_REVERSE_COMMANDS(void) {
    //stub
}

//5B154
void RAY_DEMIRAY(void) {
    //stub
}

//5B378
void Ray_RayEcrase(void) {
    //stub
}

//5B668
void DO_MOTEUR_GELE(void) {
    if (gele != 3) {
        if (gele == 2) {
            horloges(1);
            TEST_SIGNPOST();
            // stub
        }
    }
    //stub
}
