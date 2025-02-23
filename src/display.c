
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
i16 saisie_nom_prg(u32 a1) {
    readinput();
    button_released = ButtonReleasedSav3;
    SAISIE_NOM();
    ButtonReleasedSav3 = button_released;
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        endsynchro();
        synchro();
        InitPlasma(1);
        AFFICHE_ECRAN_SAVE();
        SWAP_BUFFERS();
    } else {
        AFFICHE_ECRAN_SAVE();
    }
    AFFICHE_ECRAN_SAVE();

    if (action == 3) {
        sortie_save = 1;
    }

    bool finished = fin_saisie_nom || MENU_RETURN;
    if (MENU_RETURN) {
        if (action == 1) {
            strncpy(save_ray[fichier_selectionne-1], save_ray_copy, 4);
        } else {
            save_ray[fichier_selectionne-1][0] = '\0';
        }
    }

    return finished;
}

//34334
i16 selection_save_option_prg(u32 a1) {
    readinput();
    DoCdRap();
    button_released = ButtonReleasedSav1;
    DO_COMMANDE_SAVE();
    ButtonReleasedSav1 = button_released;
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        endsynchro();
        synchro();
        InitPlasma(1);
        AFFICHE_ECRAN_SAVE();
        SWAP_BUFFERS();
    } else {
        AFFICHE_ECRAN_SAVE();
    }
    readinput();
    button_released = ButtonReleasedSav2;
    SELECTION_SAVE_OPTION();
    ButtonReleasedSav2 = button_released;
    REALISATION_ACTION();
    if ((action == 1 || (action == 3 && !fichier_existant)) && fichier_selectionne) {
        if (!fin_saisie_nom) {
            INIT_AFFICHE_ECRAN_SAVE();
            SYNCHRO_LOOP(saisie_nom_prg);
        }
        if (!MENU_RETURN) {
            SaveGameOnDisk(fichier_selectionne);
        }
    }
    if (realisation_effectuee) {
        INIT_SAVE_CONTINUE();
    }
    action = 0;
    ++TempsDemo;
    if (TempsDemo >= RunTimeDemo && !sortie_save && !fin_du_jeu) {
        InitDemoJeu();
    }
    return fin_du_jeu || sortie_save || ModeDemo || MENU_RETURN;
}

//34490
void DO_SAVE_CHOICE(void) {
    let_shadow = 1;
    LOAD_SAVE_SCREEN(main_mem_level);
    SAVE_PALETTE(&rvb_plan3);
    InitPlasma(0);
    INIT_FADE_IN();
    INIT_SAVE_CHOICE();
    INIT_SAVE_CONTINUE();
    TempsDemo = 0;
    INIT_AFFICHE_ECRAN_SAVE();
    ButtonReleasedSav1 = 1;
    ButtonReleasedSav2 = 1;
    ButtonReleasedSav3 = 0;
    max_compteur = 100;
    compteur = 0;
    xwldmapsave = 0;
    ywldmapsave = 0;
    SYNCHRO_LOOP(selection_save_option_prg);
    DO_FADE_OUT();
    RESTORE_PALETTE();
    block_free(main_mem_level);
}

//3452C
void AFFICHE_ECRAN_SAVE(void) {
    if (compteur >= max_compteur || button_released) {
        if (button_released) {
            compteur = 0;
        } else {
            compteur = delai_repetition + 1;
        }
    } else {
        ++compteur;
    }
    if (compteur_clignote >= 30) {
        compteur_clignote = 0;
    } else {
        ++compteur_clignote;
    }
    DISPLAY_FOND3();
    display_text(language_txt[96], 160, debut_titre, 1, 0); // /choose a game/

    i16 start_xpos;
    i16 erase_xpos;
    if (language == 1) {
        start_xpos = 255;
        erase_xpos = 145;
    } else {
        start_xpos = 265;
        erase_xpos = 159;
    }

    if (positionx2 == 1) {
        strncpy(menu_to_display->text, language_txt[97], 200); // /copy/
        menu_to_display->xpos = 53;
    } else {
        display_text(language_txt[97], 53, debut_sortie, 1, 5); // /copy/
    }

    if (positionx2 == 2) {
        strncpy(menu_to_display->text, language_txt[98], 200); // /erase/
        menu_to_display->xpos = erase_xpos;
    } else {
        display_text(language_txt[98], erase_xpos, debut_sortie, 1, 5); // /erase/
    }

    if (positionx2 == 3) {
        strncpy(menu_to_display->text, language_txt[99], 200); // /start/
        menu_to_display->xpos = start_xpos;
    } else {
        display_text(language_txt[99], start_xpos, debut_sortie, 1, 5); // /start/
    }

    INIT_TXT_BOX(menu_to_display);
    display_box_text_plasma(menu_to_display, 1);

    for (i32 save_index = 0; save_index < NBRE_SAVE; ++save_index) {
        if (save_ray[save_index][0] == '\0') {
            let_shadow = 1;
            display_text(language_txt[155], basex, debut_options + (ecarty + 23) * save_index, 1, 2);
        } else {
            for (i32 cur_x = 1; cur_x <= strlen(save_ray[save_index]); ++cur_x) {
                if (positionx == cur_x && positiony == save_index + 1 && clignotement) {
                    u32 len = strlen(save_ray[save_index]);
                    char temp[4] = {0};
                    strncpy(temp, save_ray[save_index], 4);
                    temp[cur_x] = '\0';

                    ktxtenx = 4096;
                    if (coeffktxt >= 512) {
                        coeffktxt = 0;
                    }
                    ktxteny = 8 * cosinus(coeffktxt);
                    coeffktxt += (cosinus(coeffktxt + 256) >> 7) + 5;

                    u8 c = temp[cur_x-1];
                    if (c == '~') {
                        display_deform_text("~", basex + ecartx * (cur_x - 1), debut_options + save_index * (ecarty + 23), 1, 2, rotationtxt, ktxtenx, ktxteny);
                    } else {
                        display_deform_text(temp + (cur_x - 1), basex + ecartx * (cur_x - 1), debut_options + save_index * (ecarty + 23), 1, colour, rotationtxt, ktxtenx, ktxteny);
                    }

                } else {
                    char temp[4] = {0};
                    strncpy(temp, save_ray[save_index], 4);
                    temp[cur_x] = '\0';
                    u8 c = temp[cur_x-1];
                    if (c == '~') {
                        display_deform_text("~", basex + ecartx * (cur_x - 1), debut_options + save_index * (ecarty + 23), 1, 2, rotationtxt, ktxtenx, ktxteny);
                    } else if (c == ' ') {
                        display_text("", basex + ecartx * (cur_x - 1), debut_options + save_index * (ecarty + 23), 1, colour);
                    } else {
                        display_text(temp + (cur_x - 1), basex + ecartx * (cur_x - 1), debut_options + save_index * (ecarty + 23), 1, colour);
                    }

                }
            }
            DISPLAY_SAVE_SPRITES(basex + 3 * (ecartx + 11) - 30, save_index);
        }
    }
    DISPLAY_SAVE_POING();

}

//34A80
i16 select_level_prg(u32 a1) {
    return 0; //stub
}

//34BB4
i16 SELECT_LEVEL(i16 original_level_choice) {
    // This seems to be a level selection screen.
    level_select = original_level_choice;
    inter_select = 0;
    LoadPlan2InVignet(main_mem_level, 50);
    SAVE_PALETTE(&rvb_pres);
    INIT_FADE_IN();
    SYNCHRO_LOOP(select_level_prg);
    RESTORE_PALETTE();
    return level_select;
}

//34C00
void CLEAR_STAGE_NAMES(void) {
    //stub
}

//34D2C
void DISPLAY_STAGE_NAMES(void) {
    DO_STAGE_NAMES(); //TODO
    sprite_clipping(0, 320, 0, 200);
    if (PositionStageNameCalcule != 1) {
        if (text_to_display[2].text[0] != '\0') {
            display_text(text_to_display[2].text, text_to_display[2].xpos, text_to_display[2].ypos, text_to_display[2].font_size, text_to_display[2].color);
        }
        if (text_to_display[1].text[0] != '\0') {
            display_text(text_to_display[1].text, text_to_display[1].xpos, text_to_display[1].ypos, text_to_display[1].font_size, text_to_display[1].color);
        }
        if (text_to_display[4].text[0] != '\0') {
            display_text(text_to_display[4].text, text_to_display[4].xpos, text_to_display[4].ypos, text_to_display[4].font_size, text_to_display[4].color);
        }
        if (text_to_display[3].text[0] != '\0') {
            display_text(text_to_display[3].text, text_to_display[3].xpos, text_to_display[3].ypos, text_to_display[3].font_size, text_to_display[3].color);
        }
    }
    if (PositionStageNameCalcule) {

    }
    //stub
}

//35028
i16 WORLD_CHOICE(u32 a1) {
    //map_screen_check_cheats();
    i32 next_clock_target = 0;
    if (need_timer) {
        next_clock_target = word_CF7EE + a1;
    }

    let_shadow = 0;
    memset(DrawBufferNormal, 0, 320*200);
    draw_buffer = EffetBufferNormal;

    DO_RAYMAN_IN_WLD_MAP();
    DO_CHEMIN();
    DisplayJumellesFondNormal();
    display2(&ray);
    DISPLAY_PLAT_WAY();

    i32 v0 = 3 * RayonJumelle + 3;
    if (v0 >= 0) {
        v0 = 3 * RayonJumelle;
    }
    DisplayJumellesNormal(JumellePosX, JumellePosY, RayonJumelle, v0 >> 2, EffetBufferNormal, DrawBufferNormal);
    draw_buffer = DrawBufferNormal;

    DISPLAY_STAGE_NAMES();
    readinput();
    DoCdRap();
    test_Keyb_on_wldmap();
    if (PROC_EXIT) {
        PlaySnd_old(77);
    }

    i16 result = new_world;
    if (new_world || PROC_EXIT || get_casse_brique_active()) {
        result = 1;
    }

    if (need_timer) {
        wait_until(next_clock_target);
        // if (?) { DO_RAYMAN_IN_WLD_MAP(); DO_CHEMIN(); }
    }
    return result;
}

//3514C
void DO_WORLD_MAP(void) {
    //clear_text_input_buffer();
    SetCompteurTrameAudio();
    EFFACE_VIDEO();
    INIT_RAY(new_level);
    readinput();
    map_time = 0;
    if (!ModeDemo) {
        LoadPlan2InVignet(main_mem_level, 48); //NOTE: 46 in the Android version
        WidthNworld = plan2_width;
        HeightNworld = plan2_height;
        chemin_percent = 1;
        DISPLAY_PTS_WAY();
    }
    SAVE_PALETTE(&rvb_pres);
    INIT_FADE_IN();
    INIT_CHEMIN();
    if (xwldmapsave == 0 && ywldmapsave == 0) {
        xmap = (scroll_end_x + scroll_start_x) / 2;
        ymap = (scroll_end_y + scroll_start_y) / 2;
        DefaultJumelleVariable();
    }
    if (!ModeDemo) {
        //InitPcSoundCard?
        SYNCHRO_LOOP(WORLD_CHOICE);
    }
    if (JeuCracker && num_world_choice > world_3_mountain) {
        fin_de_rayman = 1;
        new_world = 1;
        new_level = 1;
        fin_du_jeu = 1;
        PROC_EXIT = 1;
    }
    if (PROC_EXIT) {
        fin_du_jeu = 1;
        fichier_selectionne = 0;
        world_index = num_world_choice;
        menuEtape = 4;
        num_world_choice = num_world;
        num_level_choice = num_level;
        num_world = 0;
        PlaySnd_old(77);
    }
    for (;;) {
        // We seem to wait here until all buttons are released (why? can we skip this?)
        if (!but0pressed() && !but1pressed() && !but2pressed() && !but3pressed()) {
            break;
        }
        if (ModeDemo) {
            break;
        }
        readinput();
        break; // added // TODO: investigate if this code is needed
    }
    DETER_WORLD_AND_LEVEL();
    if (!get_casse_brique_active() && ALL_WORLD && !ModeDemo) {
        num_level_choice = SELECT_LEVEL(num_level_choice);
    }
    if (!ModeDemo) {
        PlaySnd_old(69);
        WaitNSynchro(5);
    }
    FIN_WORLD_CHOICE();
    stop_cd();
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
void file_level(i32 level_id, char* filename) {
    snprintf(filename, 20, "RAY%d.LEV", level_id);
}

//36394
void world_level(i32 world_id, char* filename) {
    snprintf(filename, 20, "RAY%d.WLD", world_id);
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
void LOAD_VIGNET_PC(mem_t* mem, i16 world) {
    if (get_casse_brique_active()) {
        LoadPlan3InVignet(mem, 64);
    } else {
        switch(world) {
            case 1: {
                LoadPlan3InVignet(mem, 68);
            } break;
            case 2: {
                LoadPlan3InVignet(mem, 70);
            } break;
            case 3: {
                LoadPlan3InVignet(mem, 69);
            } break;
            case 4: {
                LoadPlan3InVignet(mem, 66);
            } break;
            case 5: {
                LoadPlan3InVignet(mem, 63);
            } break;
            case 6: {
                LoadPlan3InVignet(mem, 62);
            } break;
            default: {
                LoadPlan3InVignet(mem, 61);
            } break;
        }
    }
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
void SwapPlan2PlanVignInVignet(mem_t* mem, i32 resource_id, i16 vig_index) {
    u8* saved_plan2bit = PLAN2BIT;
    i32 saved_plan2bit_length = plan2bit_length;
    i32 saved_plan2bit_nb_bytes = plan2bit_nb_bytes;
    rgb_palette_t saved_palette = rvb_pres;
    LoadPlan2InVignet(mem, resource_id);
    PLANVIGBIT[vig_index] = PLAN2BIT;
    planVigbit_length[vig_index] = plan2bit_length;
    planVigbit_nb_bytes[vig_index] = plan2bit_nb_bytes;
    rvb_Vig[vig_index] = rvb_pres;
    PLAN2BIT = saved_plan2bit;
    plan2bit_length = saved_plan2bit_length;
    plan2bit_nb_bytes = saved_plan2bit_nb_bytes;
    rvb_pres = saved_palette;
}

//36788
void LOAD_VIGNET_GAME(mem_t* mem) {
    i16 vignet = -1;
    switch(num_world) {
        case 1: {
            if (num_level == 9) {
                vignet = 74;
            }
        } break;
        case 3: {
            if (num_level == 6) {
                vignet = 60;
            }
        } break;
        case 4: {
            if (num_level == 11) {
                vignet = 72;
            }
        } break;
        case 5: {
            if (num_level == 3) {
                vignet = 67;
            } else if (num_level == 11) {
                vignet = 51;
            }
        } break;
        case 6: {
            if (num_level == 4) {
                vignet = 65;
            }
        } break;
    }
    if (vignet != -1) {
        SwapPlan2PlanVignInVignet(mem, vignet, 0);
    }
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
        DO_VICTOIRE();
    } else if (!fin_du_jeu && new_world != 0) {
        START_WORLD_VIGNET();
        block_free(main_mem_world);
        block_free(main_mem_sprite);
        world_level(num_world_choice, world_filename);
        load_world(main_mem_world, main_mem_sprite, world_filename);
        SpriteWorldOffset(main_mem_sprite);
        INIT_MOTEUR_WORLD();
        END_WORLD_VIGNET();
    }
}

//36998
void DEPART_LEVEL(void) {
    if (MusicCdActive) {
        stop_cd();
    }
    //sub_6BAE0();
    new_world = 0;
    num_world = num_world_choice;
    new_level = 1;
    num_level = num_level_choice;
    if (!fin_du_jeu) {

        if (byte_CFA2A) {
            i16 text_x = 160;
            i16 text_y = 190;
            if (get_casse_brique_active()) {
                text_x = 240;
                text_y = 160;
            }
            memset(draw_buffer, 0, 320*200);
            start_fade_in(2);
            display_text(language_txt[149], text_x, text_y, 2, 0); // /loading../
            synchro();
            endsynchro();
            SWAP_BUFFERS();
            do {
                synchro();
                endsynchro();
                do_fade(&rvb_plan3, &current_rvb);
            } while (nb_fade != 0);
        }

        if (MusicCdActive) {
            cdTime = 0;
        }

        file_level(num_level, level_filename);
        block_free(main_mem_level);
        SpriteWorldBlocksFree(main_mem_sprite);
        LOAD_VIGNET_GAME(main_mem_level);
        load_level(main_mem_level, num_world, level_filename);
        INIT_FND(); //TODO

    }
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
    i16 width = 0;
    i16 height = 0;
    LoadPcxOptions(EffetBufferNormal, 13, &width, &height, &rvb_plan3);
    //LoadPcxPaletteInVignet(13, &rvb_plan3); // opening twice no longer needed (we saved the palette in LoadPcxOptions)
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
    if (RayEvts.tiny) {
        RAY_DEMIRAY();
    }
    textmode();
    if (JeuCracker) {
        exit(0);
    }
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
    DISPLAY_FOND3();
    i16 text_x = 160;
    i16 text_y = 190;
    if (get_casse_brique_active()) {
        text_x = 240;
        text_y = 160;
    }
    display_text(language_txt[149], text_x, text_y, 2, 0); // /loading../
}

//36E60
void START_WORLD_VIGNET(void) {
    LOAD_VIGNET_PC(main_mem_level, num_world_choice);
    rvb_save = current_rvb;
    current_rvb = rvb_plan3;
    start_fade_in(2);
    synchro();
    endsynchro();
    SWAP_BUFFERS();
    DISPLAY_LOADING();

    // Do loading screen effect (TODO)
    if (sub_38208(9, 100, DrawBufferNormal, DrawBufferNormal)) {
        sub_38258();
        sub_38290();
        DISPLAY_LOADING();
    }

    endsynchro();
    synchro();
    SWAP_BUFFERS();
    while (nb_fade != 0) {
        endsynchro();
        synchro();
        do_fade(&rvb_plan3, &current_rvb);
    }
}

//36F48
void END_WORLD_VIGNET(void) {
    // Do loading screen effect (TODO)
    if (sub_38220(9, 100, display_buffer, DrawBufferNormal)) {
        sub_38258();
        sub_38290();
    }
    fade_out(2, &rvb_plan3);
    current_rvb = rvb_save;
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
bool SelectButPressed(void) {
    if (input_mode == 1) {
        return TOUCHE(SC_ESCAPE);
    } else {
        return 0; //TODO: stub
    }
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
