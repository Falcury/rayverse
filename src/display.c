
//34108
void display_inter_anim(void) {
    //stub
}

//34174
void continue_fonction(void) {
    //stub
}

//341B0
void DO_CONTINUE(void) {
    //stub
}

//34240
void saisie_nom_prg(void) {
    //stub
}

//34334
void selection_save_option_prg(void) {
    //stub
}

//34490
void DO_SAVE_CHOICE(void) {
    //stub
}

//3452C
void AFFICHE_ECRAN_SAVE(void) {
    //stub
}

//34A80
void select_level_prg(void) {
    //stub
}

//34BB4
void SELECT_LEVEL(i16 a1) {
    //stub
}

//34C00
void CLEAR_STAGE_NAMES(void) {
    //stub
}

//34D2C
void DISPLAY_STAGE_NAMES(void) {
    //stub
}

//35028
void WORLD_CHOICE(void) {
    //stub
}

//3514C
void DO_WORLD_MAP(void) {
    //stub
}

//35370
void DISPLAY_TXT_VIGNET(void) {
    //stub
}

//353C8
void display_vignet_prg(void) {
    //stub
}

//354A4
void DISPLAY_GAME_VIGNET(void) {
    //stub
}

//355BC
void DISPLAY_TXT_CREDITS(void) {
    //stub
}

//3561C
void display_credits_prg(void) {
    //stub
}

//35680
void display_anim_victoire(void) {
    //stub
}

//356B8
void DISPLAY_CREDITS(void) {
    //stub
}

//3572C
void DISPLAY_CREDITS_MENU(void) {
    //stub
}

//357E4
void DISPLAY_PROTOON_BACK(void) {
    //stub
}

//35850
void DO_ANIM_VICTOIRE(void) {
    //stub
}

//35878
void DO_VICTOIRE(void) {
    //stub
}

//35908
void InitDemoJeu(void) {
    //stub
}

//35B78
void FinDemoJeu(void) {
    //stub
}

//35C64
i16 loader_anim_prg(u32 a1) {
    readinput();
    if (ValidButPressed() || StartButPressed()) {
        PROC_EXIT = 1;
    }
    horloges(1);
    DoCdRap();
    DISPLAY_FOND3();
    display2(&bigray);
    DO_LOADER_ANIM();
    if (PROC_EXIT && nb_fade == 0) {
        return 1;
    } else {
        return 0;
    }
}

//35CC4
void START_LOADER_ANIM(void) {
    LOAD_BIG_RAYMAN(main_mem_tmp);
    LOAD_SAVE_SCREEN(main_mem_tmp);
    SAVE_PALETTE(&rvb_plan3);
    start_cd_gros_rayman();
    INIT_FADE_IN();
    INIT_LOADER_ANIM();
    DrawSpriteNormalEtX = DrawSpriteNormal256;
    DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal256;
    SYNCHRO_LOOP(loader_anim_prg);
    DrawSpriteNormalEtX = DrawSpriteNormal;
    DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
}

//35D30
void END_LOADER_ANIM(void) {
    DO_FADE_OUT();
    RESTORE_PALETTE();
}

//35D3C
void DO_GROS_RAYMAN(void) {
    init_memory(&main_mem_tmp, 0x41A00);
    SetCompteurTrameAudio();
    START_LOADER_ANIM();
    free(main_mem_tmp);
    PROC_EXIT = 0;
    LOAD_ALL_FIX();
    END_LOADER_ANIM();
}

//35D74
void START_LEVEL_ANIM(void) {
    //stub
}

//35DB4
void PouvoirsParMap(void) {
    //stub
}

//35F70
void sub_35F70(void) {
    //stub
}

//3609C
void sub_3609C(void) {
    //stub
}

//36150
void sub_36150(void) {
    //stub
}

//3622C
void FIRST_INIT(void) {
    init_memory(&temp_mem_buf, TailleMainMemTmp); // NOTE: need to figure out when (if ever) this gets freed?
    InitTextMode();
    freeze = 0;
    record.is_recording = 0;
    record.is_playing = 0;
    gele = 0;
    JoystickPresent();
    word_E0CD0 = 3; // inlined sub_4212C()
    DrawBufferNormal = global_app_state.game.draw_buffer.memory; //(u8*)malloc(64064);
    EffetBufferNormal = (u8*)malloc(128000);
    if (!DrawBufferNormal || !EffetBufferNormal) {
        fatal_error();
    }
    InitMatriceJumelle();
    if (Frequence == 70) {
        VGA_FREQ = Frequence;
    } else {
        VGA_FREQ = 60;
    }
    InitModeNormalWithFrequency(VGA_FREQ);
    use_sync = 1;
    set_speaker_on();
    //sub_35F70();
    path_has_changed = 0;
}

//362E4
void init_divers_level_PC(u8* a1) {
    if (fin_du_jeu) {
        return;
    }
    *a1 = 255;
    readinput();
    if (record.current_offset != 0 || record.is_playing || record.is_recording) {
        return;
    }
    //sub_42490(&record);
}

//3631C
void file_level(void) {
    //stub
}

//36394
void world_level(i32 world, char* filename) {
    snprintf(filename, 20, "RAY%d.WLD", world);
}

//3640C
void LoadPlan3InVignet(mem_t* mem, i32 resource_id) {
    // The original code calls LoadPlan2InVignet() after saving the plan2 related stuff into temporary variables.
    // This seems unnecessary though, so I simplified it a bit.
    i16 width = 0;
    i16 height = 0;
    image_t image = LoadPcxInVignet(mem, resource_id, &width, &height, &current_rvb);
    //LoadPcxPaletteInVignet(resource_id, &current_rvb); // we already loaded the palette in LoadPcxInVignet(), so we can skip this
    rvb_plan3 = current_rvb;
    plan3bit_length = width * height;
    plan3bit_nb_bytes = width >> 2;
    PLAN3BIT = image.memory;
}

//364E8
void DISPLAY_AND_FADE_PLAN3(void) {
    //stub
}

//36554
void LOAD_VIGNET_PC(i32 a1, i16 a2) {
    //stub
}

//365E0
void LOAD_SAVE_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 12);
    DISPLAY_FOND3();
}

//365F0
void LOAD_PERFTIME_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 73);
}

//36600
void LOAD_PERFECT_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 71);
}

//36610
void LOAD_CONTINUE_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 11);
    //stub: there is a DRM check here
}

//36644
void DISPLAY_FOND_CONTINUE(void) {
    //stub
}

//36678
void SwapPlan2PlanVignInVignet(i32 a1, i32 a2, i16 a3) {
    //stub
}

//36788
void LOAD_VIGNET_GAME(i32 a1) {
    //stub
}

//3681C
void LOAD_CREDITS_VIGNET(i32 a1, i32 a2, i16 a3) {
    //stub
}

//36880
void DEPART_WORLD(void) {
    if (MusicCdActive) {
        stop_cd();
    }
    if (!NormalModeAutorise) {
        GameModeVideo = 1;
    }
    if (FondAutorise == 2) {
        P486 = 1;
    }
    if (num_world_choice == num_world) {
        if (get_casse_brique_active()) {
            num_level = 0;
            new_world = 1;
        } else {
            new_world = 0;
        }
        new_level = 1;
    } else {
        num_level = 0;
        new_world = 1;
        new_level = 1;
    }
    if (You_Win == 1 && num_world_choice == world_6_cake) {
        //DO_VICTOIRE();
    } else if (!fin_du_jeu && new_world != 0) {
        //START_WORLD_VIGNET();
        block_free(main_mem_world);
        block_free(main_mem_sprite);
        world_level(num_world_choice, world_filename);
        load_world(main_mem_world, main_mem_sprite, world_filename);
        SpriteWorldOffset(main_mem_sprite);
        INIT_MOTEUR_WORLD();
        //END_WORLD_VIGNET();
    }
}

//36998
void DEPART_LEVEL(void) {
    //stub
}

//36B0C
void REMOVE_FICHIER(void) {
    //stub
}

//36BBC
i32 TestSave(void) {
    return 3;
}

//36BC4
void test_EXIT(void) {
    //stub
}

//36BF8
void test_Keyb_on_wldmap(void) {
    //stub
}

//36C44
void Keyflush(void) {
    //stub
}

//36C54
void LOAD_GENERAL_SCREEN(void) {
    //stub
}

//36C88
void LOAD_OPTIONS_SCREEN(void) {
    //stub
}

//36CBC
void HANDLE_KEY(u8* a1) {
    //stub
}

//36D30
void DEPART_INIT_LOOP(void) {
    //nullsub
}

//36D30
void DEPART_DEAD_LOOP(void) {
    //nullsub
}

//36D34
void FIN_GAME_LOOP(void) {
    if (!fin_du_jeu) {
        DEPART_INIT_LOOP();
        return;
    }
    doneGameSave();
    block_free(main_mem_world);
    block_free(main_mem_sprite);
}
//36D58
void FIN_MAP_LOOP(void) {
    //stub
}

//36D8C
void FIN_DEAD_LOOP(void) {
    //nullsub
}

//36D90
void END_GAME(void) {
    //stub
}

//36E1C
void INIT_EXTENSIONS(void) {
    //nullsub
}

//36E20
void DISPLAY_LOADING(void) {
    //stub
}

//36E60
void START_WORLD_VIGNET(void) {
    //stub
}

//36F48
void END_WORLD_VIGNET(void) {
    //stub
}

//36FAC
void sub_36FAC(void) {
    //stub
}

//36FBB
void sub_36FBB(void) {
    //stub
}

//370E8
void sub_370E8(void) {
    //stub
}

//37134
void sub_37134(void) {
    //stub
}

//37150
bool LOAD_BIG_RAYMAN(mem_t* mem) {
    load_big_ray(mem);
    return true;
}

//3715C
bool ValidButPressed(void) {
    bool result = false;
    if (nb_fade == 0) {
        result = (joy_buttonA1 == 1 || (input_mode == 1 && (TOUCHE(SC_ENTER) || TOUCHE(SC_SPACE))));
    }
    return result; //stub
}

//371A4
bool StartButPressed(void) {
    if (input_mode == 1) {
        return TOUCHE(SC_ENTER);
    } else {
        return but1pressed();
    }
}

//371D4
bool PauseButPressed(void) {
    return false; //stub
}

//37210
bool ExitButPressed(void) {
    return false; //stub
}

//37248
bool CancelButPressed(void) {
    return false; //stub
}

//37280
void SelectButPressed(void) {
    //stub
}

//372B8
void ToDoAtExit(void) {
    Reset_Clavier();
//    set_speaker_off();
    if (MusicCdActive) {
        DoneMusic();
    }
    if (ModeVideoActuel != 255) {
        InitTextMode();
    }
    printf("\nThank you for playing Rayman.\n");
    //stub
}
