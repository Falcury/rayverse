
//7A340
void new_level_init(void) {
    ray_X_main_pos_init = ray.x;
    ray_Y_main_pos_init = ray.y;
    xmapsave = xmap;
    ymapsave = ymap;
    departlevel = 1;
}

//7A474
void fix_numlevel(obj_t* obj) {
    bool handledLevelChoice = false;

    if (departlevel && (new_level_init(), obj->type != TYPE_WIZARD1))
    {
        switch(num_world)
        {
            // Jungle
            case 1:
                // Betila: fist
                if (num_level == 2 && (RayEvts.poing) != 0)
                    num_level_choice++;

                // Boss: Bzzit
                if (num_level == 5 && (finBosslevel.bzzit) != 0)
                    num_level_choice++;

                // Betilla: hang
                if (num_level == 7 && (RayEvts.hang) != 0)
                    new_world = 1;

                // Boss: Moskito chase
                if (num_level == 13 && (finBosslevel.moskito) != 0)
                    num_level_choice++;

                // Boss: Moskito
                if (num_level == 15 && (finBosslevel.moskito) != 0)
                    new_world = 1;

                // Betilla: grab
                if (num_level == 16 && (RayEvts.grab) != 0)
                    new_world = 1;
                break;

                // Music
            case 2:
                // Boss: Red Drummer (unused)
                if (num_level == 5 && (finBosslevel.crazy_drummer) != 0)
                    new_world = 1;

                // Boss: Sax encounter
                if (num_level == 9 && (finBosslevel.mr_sax) != 0)
                    new_world = 1;

                // Betilla: helico
                if (num_level == 10 && (RayEvts.helico) != 0)
                    new_world = 1;

                // Boss: Sax
                if (num_level == 14 && (finBosslevel.mr_sax) != 0)
                    new_world = 1;
                break;

                // Mountain
            case 3:
                // Boss: Stone chase
                if (num_level == 1 && (finBosslevel.mr_stone) != 0)
                    new_world = 1;

                // Boss: Stone
                if ((num_level == 9) && (finBosslevel.mr_stone) != 0)
                    new_world = 1;

                // Betilla: run
                if (num_level == 10 && (RayEvts.run) != 0)
                    new_world = 1;
                break;

                // Image
            case 4:
                // Boss: Pirate Mama
                if (num_level == 3 && (finBosslevel.viking_mama) != 0)
                    new_world = 1;

                // Boss: Space Mama
                if (num_level == 10 && (finBosslevel.space_mama) != 0)
                    new_world = 1;
                break;

                // Cave
            case 5:
                // Boss: Skops
                if (num_level == 9 && (finBosslevel.mr_skops) != 0)
                    new_world = 1;
                break;

                // Cake
            case 6:
                // Boss: Dark (unused)
                if (num_level == 1 && (finBosslevel.mr_dark) != 0)
                    new_world = 1;
                break;
        }
    }

    switch(num_world)
    {
        // Jungle
        case 1:
            switch(num_level)
            {
                // Bonus: Pink Plant Woods 2
                case 2:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 21;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End & bonus: Pink Plant Woods 4
                case 4:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 20;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Anguish Lagoon and save point
                        if ((t_world_info[1].state & 1) == 0)
                        {
                            t_world_info[1].state |= 4;
                            t_world_info[18].state |= 4;
                        }
                    }
                    break;

                    // End: Anguish Lagoon
                case 8:
                    new_world = 1;

                    // Unlock The Swamps of Forgetfulness
                    if ((t_world_info[2].state & 1) == 0)
                        t_world_info[2].state |= 4;

                    // Unlock Bongo Hills
                    if ((t_world_info[4].state & 1) == 0)
                        t_world_info[4].state |= 4;
                    break;

                    // End & bonus: The Swamps of Forgetfulness 3
                case 11:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 18;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Moskito's Nest
                        if ((t_world_info[3].state & 1) == 0)
                            t_world_info[3].state |= 4;
                    }
                    break;

                    // Bonus: Moskito's Nest 1
                case 12:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 19;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Moskito's Nest
                case 17:
                    new_world = 1;
                    break;

                    // End: Bonus 1
                case 18:
                    num_level_choice = 11;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 19:
                    num_level_choice = 12;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 3
                case 20:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 4
                case 21:
                    num_level_choice = 2;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Music
        case 2:
            switch(num_level)
            {
                // Bonus: Bongo Hills 4
                case 4:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 17;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Bongo Hills
                case 6:
                    new_world = 1;

                    // Unlock Allegro Presto and save point
                    if ((t_world_info[5].state & 1) == 0)
                    {
                        t_world_info[5].state |= 4;
                        t_world_info[19].state |= 4;
                    }
                    break;

                    // Bonus: Allegro Presto 3
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 18;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Allegro Presto
                case 11:
                    new_world = 1;

                    // Unlock Gong Heights
                    if ((t_world_info[6].state & 1) == 0)
                        t_world_info[6].state |= 4;

                    // Unlock Twilight Gulch
                    if ((t_world_info[8].state & 1) == 0)
                        t_world_info[8].state |= 4;
                    break;

                    // End: Gong Heights
                case 13:
                    new_world = 1;

                    // Unlock Mr Sax's Hullaballoo and save point
                    if ((t_world_info[7].state & 1) == 0)
                    {
                        t_world_info[7].state |= 4;
                        t_world_info[20].state |= 4;
                    }
                    break;

                    // End: Mr Sax's Hullaballoo
                case 16:
                    new_world = 1;
                    break;

                    // End: Bonus 1
                case 17:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 18:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Mountain
        case 3:
            switch(num_level)
            {
                // End: Twilight Gulch
                case 2:
                    new_world = 1;

                    // Unlock The Hard Rocks
                    if ((t_world_info[9].state & 1) == 0)
                        t_world_info[9].state |= 4;
                    break;

                    // Bonus: The Hard Rocks 2
                case 4:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: The Hard Rocks
                case 5:
                    new_world = 1;

                    // Unlock Mr Stone's Peaks and save point
                    if ((t_world_info[10].state & 1) == 0)
                    {
                        t_world_info[10].state |= 4;
                        t_world_info[21].state |= 4;
                    }
                    break;

                    // Bonus: Mr Stone's Peaks 4
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 13;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Mr Stone's Peaks
                case 11:
                    new_world = 1;

                    // Unlock Eraser Plains
                    if ((t_world_info[11].state & 1) == 0)
                        t_world_info[11].state |= 4;
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 13:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Image
        case 4:
            switch(num_level)
            {
                // Bonus: Eraser Plains 3
                case 3:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Eraser Plains
                case 4:
                    new_world = 1;

                    // Unlock Pencil Pentathlon
                    if ((t_world_info[12].state & 1) == 0)
                        t_world_info[12].state |= 4;
                    break;

                    // End: Pencil Pentathlon
                case 7:
                    new_world = 1;

                    // Unlock Space Mama's Crater
                    if ((t_world_info[13].state & 1) == 0)
                        t_world_info[13].state |= 4;
                    break;

                    // Bonus: Space Mama's Crater 2
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 13;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Space Mama's Crater
                case 11:
                    new_world = 1;

                    // Unlock Crystal Palace and save point
                    if ((t_world_info[14].state & 1) == 0)
                    {
                        t_world_info[14].state |= 4;
                        t_world_info[22].state |= 4;

                        // Show cutscene
                        Vignet_To_Display = 1;
                    }
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 3;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 13:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Cave
        case 5:
            switch (num_level)
            {
                // End & bonus: Crystal Palace 2
                case 2:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Eat at Joe's
                        if ((t_world_info[15].state & 1) == 0)
                            t_world_info[15].state |= 4;
                    }
                    break;

                    // End: Eat at Joe's
                case 8:
                    new_world = 1;

                    // Unlock Mr Skops' Stalactites and save point
                    if ((t_world_info[16].state & 1) == 0)
                    {
                        t_world_info[16].state |= 4;
                        t_world_info[23].state |= 4;
                    }
                    break;

                    // End: Mr Skops' Stalactites
                case 11:
                    new_world = 1;

                    // Show cutscene
                    Vignet_To_Display = 1;
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 2;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Cake
        case 6:
            // End: Mr Dark's Dare
            if (num_level == 4)
            {
                new_world = 1;
                You_Win = 1;
                fin_du_jeu = 1;
                fin_dark = 1;
            }
            break;
    }

    // Unlock Mr Dark's Dare
    if ((t_world_info[17].state & 1) == 0 && Nb_total_cages > 101)
        t_world_info[17].state |= 4;

    // If we're in the same world and the level choice has not already been
    // handled then we increment the level by 1 to go to the next map
    if (!new_world && !handledLevelChoice)
        num_level_choice++;

    new_level = 1;
}

//7AD20
void allocate_splash(obj_t* obj) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_19_SPLASH && !cur_obj->is_active) {
            cur_obj->flags.alive = 1;
            make_active(cur_obj, 0);
            if (cur_obj->is_active) {
                add_alwobj(cur_obj);
            }
            cur_obj->x = obj->x + obj->offset_bx - cur_obj->offset_bx;
            if (obj->btypes[0] == BTYP_WATER) {
                cur_obj->y = obj->offset_by + obj->y - cur_obj->offset_by;
            } else {
                i16 offset_by;
                i16 height;
                if (!(block_flags[obj->btypes[0]] & 2)) {
                    offset_by = cur_obj->offset_by;
                    height = (*(u16 *) &mp.height - 1) * 16;
                } else {
                    height = (*(u16 *) &mp.height - 1) * 16;
                    offset_by = cur_obj->offset_by - 10;
                }
                cur_obj->y = height - offset_by;
            }
            calc_obj_pos(cur_obj);
            set_main_and_sub_etat(cur_obj, 0, 0);
            cur_obj->anim_frame = 0;
            obj->flags.flag_0x40 = 1;
            break;
        }
    }
}

//7AE1C
void DO_BALLE(obj_t* obj) {
    eta_t* eta = get_eta(obj);
    if (obj->type == TYPE_15_BALLE2) {
        eta->speed_x_left = -1;
        eta->speed_x_right = 1;
        eta->anim_speed = (eta->anim_speed & 0xF) | 0xA0; // TODO: change to bitfield?
    } else if (obj->type == TYPE_13_BALLE1) {
        eta->speed_x_left = -2;
        eta->speed_x_right = 2;
        eta->anim_speed = (eta->anim_speed & 0xF);
    }
}

//7AE78
void DO_TIR(obj_t* obj) {
    /* 38608 8015CE08 -O2 -msoft-float */
    //TODO: non-matching, check
    obj_t* cur_obj;
    obj_t* var_s0;
    s16 temp_a0;
    s16 temp_a1;
    s16 temp_v0_1;
    s16 temp_v0_4;
    s16 i;
    s16 var_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    u16 var_v1;
    s16 var_a2;
    u8 temp_v1_1;
    u8 var_a1;
    s16 test_1;
    s32 test_2;
    u32 test_3;
    u32 test_4;

    if (obj->main_etat == 0) {
        if (obj->sub_etat == 11) {
            if (obj->link == 0) {
                cur_obj = level.objects;
                i = 0;
                test_1 = level.nb_objects;
                while (i < test_1) {
                    if (((cur_obj->type == TYPE_13_BALLE1) || (cur_obj->type == TYPE_15_BALLE2)) && !cur_obj->is_active) {
                        cur_obj->flags.flip_x = obj->flags.flip_x;
                        cur_obj->flags.alive = 1;
                        cur_obj->is_active = 1;
                        add_alwobj(cur_obj);
                        set_main_and_sub_etat(cur_obj, 2, 2);
                        cur_obj->anim_frame = 0;
                        cur_obj->anim_index = (u8) cur_obj->eta[2][2].anim_index;
                        temp_v1_4 = (obj->x + obj->offset_bx) - cur_obj->offset_bx;
                        if (cur_obj->flags.flip_x) {
                            temp_v1_4 = temp_v1_4 + 64;
                        } else {
                            temp_v1_4 = temp_v1_4 - 64;
                        }
                        cur_obj->x = temp_v1_4;
                        cur_obj->y = obj->y;
                        cur_obj->init_x = cur_obj->x;
                        cur_obj->init_y = cur_obj->y;
                        calc_obj_pos(cur_obj);
                        PlaySnd(24, cur_obj->id);
                        if (cur_obj->flags.flip_x) {
                            skipToLabel(cur_obj, 3, 1);
                        } else {
                            skipToLabel(cur_obj, 1, 1);
                        }

                        if (obj->type == TYPE_14_CHASSEUR2) {
                            if (obj->x - ray.x < 0) {
                                var_a2 = (ray.x - obj->x) >> 5;
                            } else {
                                var_a2 = (obj->x - ray.x) >> 5;
                            }
                            cur_obj->type = TYPE_15_BALLE2;
                            temp_v1_6 = (ray.y - obj->y) * 2;
                            test_3 = (s16) var_a2;
                            temp_v0_2 = obj->x - ray.x;

                            if (temp_v0_2 >= 0) {
                                var_v0 = (temp_v1_6 / temp_v0_2) - test_3;
                            } else {
                                temp_v0_3 = ray.x - obj->x;
                                var_v0 = (temp_v1_6 / temp_v0_3) - test_3;
                            }
                            cur_obj->speed_y = var_v0;
                            cur_obj->gravity_value_2 = 10;
                        } else {
                            cur_obj->type = TYPE_13_BALLE1;
                            cur_obj->speed_y = 0;
                        }
                        calc_obj_pos(cur_obj);
                        break;
                    }
                    cur_obj += 1;
                    i = i + 1;
                }
                obj->link = 1;
                return;
            }
        } else if (obj->sub_etat == 2) {
            obj->link = 0;
        }
    }
}

//7B060
void DO_INTERACT_PLAT(obj_t* obj) {
    //stub
}

//7B0E0
void oldest_planted(void) {
    //stub
}

//7B1A0
void DO_GROWING_PLATFORM(void) {
    //stub
}

//7B29C
obj_t* allocateExplosion(obj_t* obj) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_83_EXPLOSION && !cur_obj->is_active) {
            set_main_and_sub_etat(cur_obj, 0, 0);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->x = obj->x + obj->offset_bx - cur_obj->offset_bx;
            cur_obj->y = obj->y + obj->offset_by - cur_obj->offset_by + 10;
            calc_obj_pos(cur_obj);
            cur_obj->anim_index = get_eta(cur_obj)->anim_index;
            cur_obj->anim_frame = 0;
            cur_obj->change_anim_mode = 2;
            return cur_obj;
        }
    }
    return NULL;
}

//7B370
void BombExplosion(obj_t* obj) {
    obj_t* explosion_obj = allocateExplosion(obj);
    obj->flags.alive = 0;
    obj->is_active = 0;
}

//7B390
void MARACAS_GO(obj_t* obj) {
    //stub
}

//7B470
void ANGLE_RAYMAN(obj_t* obj) {
    //stub
}

//7B56C
obj_t* allocateNOVA(void) {
    for (i32 i = 0; i < level_alw.nb_objects; ++i) {
        obj_t* obj = level.objects + level_alw.obj_ids[i];
        if (obj->type == TYPE_143_NOVA2 && !obj->is_active) {
            set_main_and_sub_etat(obj, 5, 20);
            obj->flags.alive = 1;
            add_alwobj(obj);
            obj->anim_index = get_eta(obj)->anim_index;
            obj->anim_frame = 0;
            return obj;
        }
    }
    return NULL;
}

//7B628
void DO_NOVA(obj_t* obj) {
    obj_t* nova = allocateNOVA();
    if (nova) {
        nova->x = obj->x;
        nova->y = obj->y;
        i16 nova_x, nova_y, nova_w, nova_h;
        i16 obj_x, obj_y, obj_w, obj_h;
        GET_ANIM_POS(nova, &nova_x, &nova_y, &nova_w, &nova_h);
        GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
        nova->is_active = 1;
        nova->display_prio = 1;
        nova->cmd_arg_1 = 0;
        nova->x += (obj_x + (obj_w >> 1)) - (nova_x + (nova_w >> 1));
        nova->y += (obj_y + (obj_h >> 1)) - (nova_y + (nova_h >> 1));
        calc_obj_pos(nova);
    }
}

//7B708
void DO_NOVA2(obj_t* obj) {
    /* 3B034 8015F834 -O2 -msoft-float */
    s16 i;
    obj_t* nova_obj;
    s16 nova_x; s16 nova_y; s16 nova_w; s16 nova_h;
    s16 in_x; s16 in_y; s16 in_w; s16 in_h;
    s16 new_x; s16 new_y;

    for (i = 0; i < 5; i++) {
        nova_obj = allocateNOVA();
        if (nova_obj)
        {
            nova_obj->x = obj->x;
            nova_obj->y = obj->y;
            GET_ANIM_POS(nova_obj, &nova_x, &nova_y, &nova_w, &nova_h);
            GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);
            new_x = in_x - (nova_x + (nova_w >> 1));
            new_y = in_y - (nova_y + (nova_h >> 1));
            nova_obj->is_active = 1;
            switch (i)
            {
                case 0:
                    break;
                case 1:
                    new_x += in_w;
                    break;
                case 2:
                    new_x += in_w;
                    new_y += in_h;
                    break;
                case 3:
                    new_y += in_h;
                    break;
                case 4:
                    new_x += in_w >> 1;
                    new_y += in_h >> 1;
                    break;
            }
            nova_obj->display_prio = 0;
            nova_obj->cmd_arg_1 = i * 5;
            nova_obj->x += new_x;
            nova_obj->y += new_y;
            calc_obj_pos(nova_obj);
        }
    }
}

//7B838
i16 NOVA_STATUS_BAR(void) {
    obj_t* obj = allocateNOVA();
    if (!obj) {
        return -1;
    }
    obj->display_prio = 0;
    obj->timer = 10;
    obj->cmd_arg_1 = 255;
    obj->is_active = 1;
    obj->x = xmap + obj->offset_bx;
    obj->y = ymap + obj->offset_by;
    return obj->id;
}

//7B920
void DO_REDEYE_FIRE(i16 a1, i16 a2, i16 a3) {
    //stub
}

//7BC60
void calc_esquive_poing(obj_t* obj, i16* a2, i16* a3, i16* a4) {
    //stub
}

//7BD04
void allocate_gerbe(void) {
    //stub
}

//7BD40
void start_pix_gerbe(i16 a1, i16 a2) {
    //stub
}

//7BE44
void do_pix_gerbes(void) {
    //stub
}

//7BF40
void START_2_PARTS_CYMBAL_ACTION(obj_t* obj) {
    //stub
}

//7BFB0
void ChangeLevel(void) {
    fix_numlevel(&ray);
    new_level = 1;
}

//7BFCC
void init_moustique(void) {
    if (!fin_du_jeu) {
        NewMs = rayman_obj_id != -1;
        rayOnMsWasThere = 0;
    }
}

//7BFF4
void DO_CORDE_CASSE(obj_t* obj) {
    //stub
}

//7C0A4
void DO_FUMEE_CORDE(obj_t* obj) {
    //stub
}

//7C10C
i32 GetY(i16 a1) {
    return 0; //stub
}

//7C1C4
void allocateSupHelico(obj_t* obj) {
    //stub
}

//7C274
void allocatePaillette(obj_t* obj) {
    obj_t* pailette_obj = NULL;
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_75_PAILLETTE && !cur_obj->is_active) {
            pailette_obj = cur_obj;
            break;
        }
    }
    if (pailette_obj) {
        add_alwobj(pailette_obj);
    } else {
        pailette_obj = findfirstObject(TYPE_75_PAILLETTE);
        // NOTE: added the below just to be sure
        del_alwobj(pailette_obj->id);
        add_alwobj(pailette_obj);
    }
    pailette_obj->is_active = 1;
    pailette_obj->flags.alive = 1;
    pailette_obj->active_timer = 0;
    pailette_obj->flags.flag_0x40 = 0;
    pailette_obj->x = obj->x - 40;
    pailette_obj->y = obj->y - 20;
    pailette_obj->anim_index = 0;
    pailette_obj->anim_frame = 1;
    pailette_obj->main_etat = 0;
    pailette_obj->init_main_etat = 0;
    pailette_obj->init_sub_etat = 0;
    pailette_obj->sub_etat = 0;
    calc_obj_pos(obj);
}

//7C328
void test_fin_cling(void) {
    /* 3F0B8 801638B8 -O2 -msoft-float */
    //NOTE: bug in original game: alive flag is set after setting the id to -1: level.objects[-1].flags.alive = 0;
    if (id_Cling_1up != -1) {
        Add_One_RAY_lives();
        level.objects[id_Cling_1up].flags.alive = 0;
        id_Cling_1up = -1;
    }

    if (id_Cling_Pow != -1) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
        level.objects[id_Cling_Pow].flags.alive = 0;
        id_Cling_Pow = -1;
    }
}
