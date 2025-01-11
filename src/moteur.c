
//55480
void INIT_HORLOGES(void) {
    //stub
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
    //stub
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
        if (obj->flags & obj_flags_8_flipped) {
            xspeed = eta->right_speed * horloge_index;
        } else {
            xspeed = eta->left_speed * horloge_index;
        }
        if (obj->type == obj_23_rayman && RayEvts.tiny) {
            xspeed /= 2;
        }
    }
    obj->xspeed = (i16)xspeed;
}

//55F9C
void make_active2(obj_t* obj, u8 a2) {
    //stub
}


//55FBC
void make_active(obj_t* obj, u8 a2) {
    //stub
}

//55FE4
void in_action_zone(i16 a1, i16 a2, obj_t* obj, u8 a4) {
    //stub
}

//56310
void kill_obj(obj_t* obj) {
    //stub
}

//56338
void SET_ACTIVE_FLAG(i16 a1, i16 a2, obj_t* obj) {
    //stub
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
        if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
            --obj->anim_frame;
        } else {
            ++obj->anim_frame;
        }
    }
    obj->anim_index = eta->anim_index;
    anim = obj->animations + obj->anim_index;
    if ((obj->change_anim_mode == 1 && obj->anim_index != prev_anim_index) || obj->change_anim_mode == 2) {
        if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->obj_index);
        }
    }
    if (obj->anim_frame >= anim->frame_count || obj->anim_frame == 255) {
        // animation ended
        obj->main_etat = eta->next_etat;
        obj->sub_etat = eta->next_subetat;
        eta = get_eta(obj);
        obj->anim_index = eta->anim_index;
        anim = obj->animations + obj->anim_index;
        if ((obj->type == obj_23_rayman && (ray_old_etat == 2 || ray_old_etat == 6)) ||
            (obj->sub_etat == 61 && ray_old_subetat == 61 && ray_old_etat == 0)) {
            if (ray.timer > 60 && !RayEvts.squashed) {
                ray.timer = 60;
            }
        }
        if (eta->interaction_flags & eta_flags_0x10_anim_reverse) {
            obj->anim_frame = anim->frame_count - 1;
        } else {
            obj->anim_frame = 0;
        }
        if (obj->is_active != 0) {
            PlaySnd(eta->sound_index, obj->obj_index);
        }
    }
    obj->change_anim_mode = 0;
    if (obj->flags & obj_flags_0x20_follow_enabled) {
        //calc_follow_sprite_speed(); //STUB
    }
    u8 anim_changed_bit = 0;
    if (obj->anim_frame != prev_anim_frame || obj->anim_index != prev_anim_index) {
        anim_changed_bit = 1;
    }
    obj->flags &= ~obj_flags_0x80_anim_changed;
    obj->flags |= (anim_changed_bit << 7);

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
    //stub
}

//58388
void build_active_table(void) {
    //stub
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
    //stub
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
    //stub
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
    ray_max_hitp = 2;
    status_bar.num_wiz = 0;
    fin_continue = 0;
    ray.flags &= ~obj_flags_1;
}

//59948
void INIT_RAY(u8 a1) {
    //stub
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
void INIT_MOTEUR(u8 a1) {
    //stub
}

//5A5B4
void INIT_MOTEUR_BEGIN(void) {
    init_allowed_time();
    init_bonus_perfect();
    init_calcbloc_func();
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hitp = 2;
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
    time_left = -2;
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
    //stub
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
    //stub
}

//5AA20
void INIT_RAY_ON_MS(void) {
    //stub
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
    //stub
}
