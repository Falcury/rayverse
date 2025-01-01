
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
    //stub
}

//6F95C
void remoteControlRay(void) {
    //stub
}

//6F9E0
void STOPPE_RAY_CONTRE_PAROIS(u8 a1) {
    //stub
}

//6FA44
void RAY_IN_THE_AIR(void) {
    //stub
}

//7008C
void terminateFistWhenRayDies(void) {
    //stub
}

//700A4
void snifRayIsDead(i32 a1) {
    //stub
}

//7010C
void rayfallsinwater(void) {
    //stub
}

//701A8
void RAY_DEAD(void) {
    //stub
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
void RayEstIlBloque(void) {
    //stub
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
void DO_PIEDS_RAYMAN(void) {
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
    //stub
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
void updateLogo(i32 a1, i32 a2, i32 a3) {
    start_fade_in(2);
    WaitNSynchro(5);
    for (i32 i = 0; i < a1; ++i) {
        do_fade(&fade_source_palette, global_game->draw_buffer.pal);
        advance_frame();
    }
    while (is_ogg_playing) {
        readinput();
        if (TOUCHE(SC_SPACE) || but0pressed() || but1pressed() || a3 == -1) {
            break;
        }
        advance_frame();
    }
    fade_out(2, &fade_source_palette);
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
    image_t ubisoft_logo = load_vignet_pcx(29);
    copy_full_image_to_draw_buffer(&ubisoft_logo);
    fade_source_palette = *ubisoft_logo.pal;
    destroy_image(&ubisoft_logo);
    INIT_FADE_IN();
    play_cd_track(12); // CD track 12: Intro music - "Ubisoft Presents"
    updateLogo(60, -1, 8);
    FIN_PC();
    stop_cd();
}




