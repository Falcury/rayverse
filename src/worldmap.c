
// NOTE: unsure about the source filename

//67860
void TEST_DISPLAY_PTS_WAY(i16 world, i16 connected_world, i16 xpos, i16 ypos) {
    if (world != connected_world) {
        world_info_t* connected_world_info = t_world_info + connected_world;
        if ((connected_world_info->state & 2) == 0) {
            if (connected_world_info->state & 1) {
                DISPLAY_PTS_TO_PLAN2(xpos, ypos, connected_world_info->xpos, connected_world_info->ypos, 128);
            } else if (connected_world_info->state & 4) {
                DISPLAY_PTS_TO_PLAN2(xpos, ypos, connected_world_info->xpos, connected_world_info->ypos, chemin_percent);
            }
        }
    }
    //stub
}

//678E8
void DISPLAY_PTS_WAY(void) {
    for (i32 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + i;
        world_info->state &= ~2;
    }
    for (i32 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + i;
        if (world_info->state & 1) {
            TEST_DISPLAY_PTS_WAY(world_index, world_info->index_up, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(world_index, world_info->index_down, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(world_index, world_info->index_left, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(world_index, world_info->index_right, world_info->xpos, world_info->ypos);
            world_info->state |= 2;
        }
    }
}

//6799C
void DISPLAY_PLAT_WAY(void) {
    //stub
}

//67B2C
void DO_MEDAILLONS(void) {
    //stub
}

//67D50
void INIT_LEVEL_STAGE_NAME(void) {
    //stub
}

//67E40
void INIT_WORLD_STAGE_NAME(void) {
    //stub
}

//68120
void INIT_STAGE_NAME(void) {
    //stub
}

//6817C
void CHANGE_STAGE_NAMES(void) {
    //stub
}

//68208
void DO_STAGE_NAMES(void) {
    //stub
}

//68BC0
void INIT_WORLD_INFO(void) {
    for (i32 i = 0; i < 24; ++i) {
        t_world_info[i].text = language_txt[i];
    }
    num_world = 0;
    num_level = 0;
    new_world = 1;
    new_level = 1;
    world_index = 0;
    xwldmapsave = 0;
    ywldmapsave = 0;
    dir_on_wldmap = 1;
    You_Win = 0;
    fin_du_jeu = 0;
    INIT_PASTILLES_SAUVE();
    MENU_RETURN = 0;
}

//68C44
void INIT_LITTLE_RAY(void) {
    raytmp = ray;
    ray.sprites = raylittle.sprites;
    ray.img_buffer = raylittle.img_buffer;
    ray.scale = 256;
}

//68C8C
void RESTORE_RAY(void) {
    //stub
}

//68CB8
void INIT_CHEMIN(void) {
    PROC_EXIT = 0;
    dans_la_map_monde = 1;
    gele = 0;
    if (!ModeDemo) {
        new_world = 0;
        num_world_choice = world_index;
    }
    old_num_world = num_world_choice;
    INIT_LITTLE_RAY();
    set_main_and_sub_etat(&ray, 0, 0);
    world_info_t* initial_world_info = t_world_info + num_world_choice;
    ray.xpos = initial_world_info->xpos - ray.offset_bx + 4;
    ray.ypos = initial_world_info->ypos - ray.offset_by + 8;
    ray.xspeed = 0;
    ray.yspeed = 0;
    set_zoom_mode(0);
    chemin_percent = 0;
    Nb_total_cages = 0;
    for (i32 i = 0; i < 24; ++i) {
        obj_t* medaillon = mapobj + i;
        world_info_t* world_info = t_world_info + i;
        medaillon->type = TYPE_54_MEDAILLON;
        medaillon->spawn_x = world_info->xpos - 78;
        medaillon->spawn_y = world_info->ypos - 64;
        medaillon->spawn_etat = 5;
        if (world_info->state & 4) {
            medaillon->spawn_sub_etat = 46;
        } else {
            i32 cages = world_info->nb_cages;
            if (cages != 0) {
                Nb_total_cages += cages;
                if (cages == 6) {
                    medaillon->spawn_sub_etat = 52;
                } else {
                    medaillon->spawn_sub_etat = 47;
                }
            } else if (i == 17) {
                medaillon->sub_etat = 59;
                medaillon->spawn_sub_etat = 59;
            } else {
                medaillon->spawn_sub_etat = 39;
            }
        }
        medaillon->scale = 0;
        medaillon->offset_bx = 0;
        medaillon->offset_by = 64;
        obj_init(medaillon);
        CalcObjPosInWorldMap(medaillon);
        medaillon->anim_frame = i % medaillon->animations[get_eta(medaillon)->anim_index].frame_count;
    }

    // unlock Mr Dark's Dare
    if ((t_world_info[17].state & 1) == 0 && Nb_total_cages >= 102) {
        t_world_info[17].state |= 4;
    }

    xmapinit = xmap;
    ymapinit = ymap;
    xmap = xwldmapsave;
    ymap = ywldmapsave;
    ChangeJumelleVariable();
    special_ray_mov_win_x_left = 30;
    special_ray_mov_win_x_right = 30;
    scroll_x = -1;
    scroll_y = -1;
    ray.flags = (ray.flags & ~obj_flags_8_flipped) | ((dir_on_wldmap & 1) * obj_flags_8_flipped);

    // If it's a new game, create the new save file on disk
    if (nouvelle_partie) {
        if (NBRE_SAVE != 0) {
            SaveGameOnDisk(fichier_selectionne);
        }
        nouvelle_partie = 0;
    }

    INIT_PASTILLES_SAUVE();
    INIT_STAGE_NAME(); //TODO
}

//68F38
void RESPOND_TO_UP(void) {
    //stub
}

//68F88
void RESPOND_TO_DOWN(void) {
    //stub
}

//68FD8
void RESPOND_TO_RIGHT(void) {
    //stub
}

//69028
void RESPOND_TO_LEFT(void) {
    //stub
}

//69078
void DO_RAYMAN_IN_WLD_MAP(void) {
    //stub
}

//6947C
void DO_CHEMIN(void) {
    //stub
}

//69518
void INIT_PASTILLES_SAUVE(void) {
    char* text = (NBRE_SAVE != 0) ? language_txt[150] /*/save game/*/: language_txt[152]; /*/no save available/*/
    t_world_info[18].text = text;
    t_world_info[19].text = text;
    t_world_info[20].text = text;
    t_world_info[21].text = text;
    t_world_info[22].text = text;
    t_world_info[23].text = text;
}

//6954C
void PASTILLES_SAUVE_SAVED(void) {
    //stub
}

//69570
void FIN_WORLD_CHOICE(void) {
    //stub
}

//695DC
void DETER_WORLD_AND_LEVEL(void) {
    world_index = num_world_choice;
    if (!ModeDemo) {
        world_info_t* world_info = t_world_info + num_world_choice;
        num_level_choice = world_info->level;
        num_world_choice = world_info->world;
        enter_casse_brique();
    }
    RayEvts.firefly = 0;
    if (num_world_choice == world_5_cave && num_level_choice == 3 && finBosslevel.helped_joe_1) {
        RayEvts.firefly = 1;
        ++num_level_choice;
    } else if (num_world_choice == world_5_cave && num_level_choice == 4 && finBosslevel.helped_joe_1 && ModeDemo) {
        RayEvts.firefly = 1;
    } else if (num_world_choice == world_6_cake && finBosslevel.mr_dark) {
        You_Win = 1;
        fin_du_jeu = 1;
        new_world = 1;
        fin_dark = 1;
    }
}

//696CC
void INIT_NEW_GAME(void) {
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hitp = 2;
    fin_du_jeu = 0;
    nb_continue = 9;
    ray.flags &= ~obj_flags_1;
    poing.sub_etat = 1;
    departlevel = 1;
    memset(wi_save_zone, 0, sizeof(wi_save_zone));
    memset(save_zone, 0, sizeof(save_zone));
    INIT_RAY_BEGIN();
    u8 first_world_is_accessible_flag = (t_world_info[0].state | 1);
    for (i32 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + 1;
        world_info->nb_cages = 0;
        world_info->state &= ~7;
    }
    t_world_info[0].state |= first_world_is_accessible_flag;
    i32 save_index = positiony - 1;
    loadinforay_t* loadinfo = LoadInfoRay + save_index;
    loadinfo->lives = 3;
    loadinfo->tings = 0;
    loadinfo->cages = 0;
    loadinfo->continues = 0;
    for (i32 i = 0; i < 23; ++i) { // NOTE: the loop only goes up to 23 here, not 24
        bonus_perfect[i] = 0;
    }
    init_finBossLevel();
    INIT_WORLD_INFO();
}

//697DC
void DO_NOT_SAVE_CHOICE(void) {
    //stub
}

//6980C
void INIT_SAVE_CHOICE(void) {
    rotationtxt = 0;
    ktxtenx = 4096;
    ktxteny = 4096;
    coeffktxt = 1;
    nbre_options = 3;
    positiony = 1;
    positionx2 = 3;
    positiony2 = NBRE_SAVE + 1;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    nouvelle_partie = 0;
    delai_repetition = 100;
    repetition = 50;
    basex = 40;
    sortie_save = 0;
    for (i32 i = 0; i < NBRE_SAVE; ++i) {
        LoadInfoGame(i);
    }
}

//698F0
void INIT_SAVE_CONTINUE(void) {
    selection_effectuee = 0;
    realisation_effectuee = 0;
    fichier_selectionne = 0;
    fichier_a_copier = 0;
    action = 0;
    fin_saisie_nom = 0;
    affiche_bon_ecran = 0;
    clignotement = 0;
    compteur_clignote = 0;
    positionx = 1;
}

//69944
void DO_COMMANDE_SAVE(void) {
    if (rightjoy() && !upjoy() && !downjoy() && !fichier_a_copier && (button_released || (delai_repetition < compteur && (compteur % repetition == 0)))) {
        positionx2 = (positionx2 == 3) ? 1 : positionx2 + 1;
        TempsDemo = 0;
        PlaySnd_old(68);
    }
    if (leftjoy() && !upjoy() && !downjoy() && !fichier_a_copier && (button_released || (delai_repetition < compteur && (compteur % repetition == 0)))) {
        positionx2 = (positionx2 == 1) ? 3 : positionx2 - 1;
        TempsDemo = 0;
        PlaySnd_old(68);
    }

    if (upjoy() && !leftjoy() && !rightjoy() && !fichier_a_copier && (button_released || (delai_repetition < compteur && (compteur % repetition == 0)))) {
        positiony = (positiony <= 1) ? NBRE_SAVE : positiony - 1;
        TempsDemo = 0;
        PlaySnd_old(68);
    }
    if (downjoy() && !leftjoy() && !rightjoy() && !fichier_a_copier && (button_released || (delai_repetition < compteur && (compteur % repetition == 0)))) {
        positiony = (positiony == NBRE_SAVE) ? 1 : positiony + 1;
        TempsDemo = 0;
        PlaySnd_old(68);
    }

    if (rightjoy() || leftjoy() || downjoy() || upjoy()) {
        button_released = 0;
    } else {
        compteur = 0;
        button_released = 1;
    }

    if (SelectButPressed() && button_released) {
        MENU_RETURN = 1;
        PlaySnd_old(77);
    }
}

//69C1C
void SELECTION_SAVE_OPTION(void) {
    TestCompteur();
    if (positionx2 == 1 && ValidButPressed()) {
        if (button_released || (delai_repetition < compteur && compteur % repetition == 0)) {
            action = 1;
            PlaySnd_old(69);
        }
    } else if (positionx2 == 2 && ValidButPressed()) {
        if (button_released || (delai_repetition < compteur && compteur % repetition == 0)) {
            action = 2;
            PlaySnd_old(69);
        }
    } else if (positionx2 == 3 && ValidButPressed()) {
        if (button_released || (delai_repetition < compteur && compteur % repetition == 0)) {
            action = 3;
            PlaySnd_old(69);
            WaitNSynchro(20);
        }
    }
    TestButtonReleased();
}

//69D54
void INIT_AFFICHE_ECRAN_SAVE(void) {
    nbre_options = 4;
    general_init_screen(20, 65, 5);
    basex = 50;
    display_item_t* to_display = menu_to_display;
    to_display->font_size = 1;
    to_display->is_fond = 1;
    to_display->field_D5 = 1;
    to_display->ypos = debut_sortie;
    to_display->color = 5;
    ecartx = 21;
    ecarty = ecart_options;
}

//69DCC
void SAISIE_NOM(void) {
    //stub
}

//6A0F4
void REALISATION_ACTION(void) {
    switch(action) {
        default: break;
        case 1: {
            // Copy
            if (!fichier_a_copier) {
                //stub
            }
        } break;
        case 2: {
            // Erase
            //stub
        } break;
        case 3: {
            // Start
            fichier_selectionne = positiony;
            if (save_ray[fichier_selectionne-1][0] == '\0') {
                fichier_existant = 0;
                nouvelle_partie = 1;
                INIT_NEW_GAME();
            } else {
                fichier_existant = 1;
                LoadGameOnDisk(fichier_selectionne);
                sortie_save = 1;
                new_world = 1;
            }
        } break;
    }
}

//6A3A0
void DISPLAY_SYMBOLE(i16 a1, i16 a2, i16 a3, u8 a4) {
    //stub
}

//6A3EC
void INIT_CONTINUE(void) {
    //stub
}

//6A5E8
void CHEAT_MODE_CONTINUE(void) {
    //stub
}

//6A798
void MAIN_CONTINUE_PRG(void) {
    //stub
}

//6AAA8
void FIN_CONTINUE_PRG(void) {
    //stub
}

//6AB24
void INIT_NO_MORE_CONTINUE(void) {
    //stub
}

//6AB98
void MAIN_NO_MORE_CONTINUE_PRG(void) {
    //stub
}

//6ABEC
void INIT_VIGNET(void) {
    //stub
}

//6AF40
void INIT_CREDITS(void) {
    //stub
}

//6B000
void DO_CREDITS(void) {
    //stub
}

//6B138
void INIT_ANIM_VICTOIRE(void) {
    //stub
}

//6B178
void DO_RAYMAN_WINS(void) {
    //stub
}

//6B1EC
void INIT_LOADER_ANIM(void) {
    bigray.offset_bx = 200;
    bigray.offset_by = 240;
    bigray.screen_y = -72;
    bigray.screen_x = 120;
    bigray.main_etat = 1;
    bigray.sub_etat = 0;
    bigray.command = 0;
    bigray.flags &= ~obj_flags_8_flipped;
}

//6B258
void DO_LOADER_ANIM(void) {
    eta_t* eta = get_eta(&bigray);
    bigray.xspeed = 0;
    if ((eta->anim_speed & 15) != 0 && (horloge[eta->anim_speed] == 0)) {
        SET_X_SPEED(&bigray);
    }
    bigray.screen_x += bigray.xspeed;
    if (PROC_EXIT == 1) {
        PROC_EXIT = 0;
        bigray.command = 5;
        set_main_and_sub_etat(&bigray, 0, 2);
        bigray.timer = 2;
        bigray.flags &= ~obj_flags_8_flipped;
        bigray.screen_x = 160 - bigray.offset_bx - 16;
    }

    if (bigray.command <= 5) {
        switch(bigray.command) {
            default: break;
            case 0: {
                if (bigray.screen_x + bigray.offset_bx < -100) {
                    ++bigray.command;
                    set_main_and_sub_etat(&bigray, 1, 1);
                    bigray.screen_x = (-bigray.offset_bx) - 60;
                    bigray.flags |= obj_flags_8_flipped;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 1: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.command;
                    set_main_and_sub_etat(&bigray, 1, 2);
                    bigray.anim_frame = 0;
                    bigray.screen_x = 350 - bigray.offset_bx;
                    bigray.flags &= ~obj_flags_8_flipped;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 2: {
                if (bigray.main_etat == 0 && bigray.sub_etat == 0) {
                    ++bigray.command;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 3: {
                if (EOA(&bigray)) {
                    ++bigray.command;
                    set_main_and_sub_etat(&bigray, 1, 3);
                    bigray.screen_x = -60 - bigray.offset_bx;
                    bigray.flags |= obj_flags_8_flipped;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 4: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.command;
                    set_main_and_sub_etat(&bigray, 0, 1);
                    bigray.screen_x = 160 - bigray.offset_bx - 16;
                    bigray.flags &= ~obj_flags_8_flipped;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 5: {
                if (bigray.main_etat == 0 && bigray.sub_etat == 2) {
                    if (bigray.timer == 0) {
                        PROC_EXIT = 1;
                    } else {
                        --bigray.timer;
                    }
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
        }
    }
}

//6B550
void SPECIAL_INIT(void) {
    dark_phase = 0;
    if (ray_on_poelle == 1) {
        RayEvts = SauveRayEvts;
        ray_on_poelle = 0;
    }
}

//6B590
void INIT_LEVEL_ANIM(void) {
    //stub
}

//6B73C
void DO_LEVEL_ANIM(void) {
    //stub
}

//6BA34
void FIN_LEVEL_ANIM(void) {
    //stub
}


