
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
}

//678E8
void DISPLAY_PTS_WAY(void) {
    for (i16 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + i;
        world_info->state &= ~2;
    }
    for (i16 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + i;
        if (world_info->state & 1) {
            TEST_DISPLAY_PTS_WAY(i, world_info->index_up, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(i, world_info->index_down, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(i, world_info->index_left, world_info->xpos, world_info->ypos);
            TEST_DISPLAY_PTS_WAY(i, world_info->index_right, world_info->xpos, world_info->ypos);
            world_info->state |= 2;
        }
    }
}

//6799C
void DISPLAY_PLAT_WAY(void) {
    for (i32 i = 0; i < 24; ++i) {
        world_info_t* world_info = t_world_info + i;
        obj_t* obj = mapobj + i;
        if ((world_info->state & 1) || i == 17) {

            display2(obj); // seems to be DISPLAY_PLATEAU() inlined

            // NOTE: disabling clipping for now
            /*i32 x = obj->screen_x + obj->offset_bx + 8;
            if (x > -8 && x < LargeurJumelle + 14) {
                i32 y = obj->screen_y + obj->offset_by;
                if (y > -10 && y < HauteurJumelle + 12) {
                    display2(obj); // seems to be DISPLAY_PLATEAU() inlined
                }
            }*/
            if (chemin_percent != 0 && i != 17) {
                world_info_t* wi_up = t_world_info + world_info->index_up;
                world_info_t* wi_down = t_world_info + world_info->index_down;
                world_info_t* wi_left = t_world_info + world_info->index_left;
                world_info_t* wi_right = t_world_info + world_info->index_right;
                if (wi_up->state & 4) {
                    DISPLAY_PTS_TO_PLAN2(world_info->xpos, world_info->ypos, wi_up->xpos, wi_up->ypos, chemin_percent);
                }
                if (wi_down->state & 4) {
                    DISPLAY_PTS_TO_PLAN2(world_info->xpos, world_info->ypos, wi_down->xpos, wi_down->ypos, chemin_percent);
                }
                if (wi_left->state & 4) {
                    DISPLAY_PTS_TO_PLAN2(world_info->xpos, world_info->ypos, wi_left->xpos, wi_left->ypos, chemin_percent);
                }
                if (wi_right->state & 4) {
                    DISPLAY_PTS_TO_PLAN2(world_info->xpos, world_info->ypos, wi_right->xpos, wi_right->ypos, chemin_percent);
                }
            }
        }
    }
}

//67B2C
void DO_MEDAILLONS(void) {
    bool unlocked = false;
    if (chemin_percent < 128 && (horloge[2] != 0 || ALL_WORLD)) {
        ++chemin_percent;
    }
    for (i32 i = 0; i < COUNT(t_world_info); ++i) {
        world_info_t* world_info = t_world_info + i;
        obj_t* obj = mapobj + i;
        if ((world_info->state & 4) || (world_info->state & 2)) {
            CalcObjPosInWorldMap(obj);

            if (chemin_percent == 128 && (world_info->state & 4) &&
                ((t_world_info[world_info->index_right].state & 2) ||
                (t_world_info[world_info->index_left].state & 2) ||
                (t_world_info[world_info->index_up].state & 2) ||
                (t_world_info[world_info->index_down].state & 2))
            ) {
                // set unlocked
                world_info->state = (world_info->state & ~7) | 1;
                set_sub_etat(obj, 46);
                unlocked = true;
            }

            if (i == 17 && !(obj->sub_etat == 59 || obj->sub_etat == 46) && (world_info->state & 1)) {
                set_sub_etat(obj, 59);
            } else if ((i == 18 || i == 19 || i == 20 || i == 21 || i == 22 || i == 23) &&
                    !(obj->sub_etat == 58 || obj->sub_etat == 46) &&
                    (world_info->state & 1)
            ) {
                set_sub_etat(obj, 58);
            } else {
                u8 nb_cages = world_info->nb_cages;
                if (nb_cages > 5 && obj->sub_etat == 51 && EOA(obj)) {
                    set_sub_etat(obj, 52);
                } else if (nb_cages > 4 && obj->sub_etat == 50 && EOA(obj)) {
                    set_sub_etat(obj, 51);
                } else if (nb_cages > 3 && obj->sub_etat == 49 && EOA(obj)) {
                    set_sub_etat(obj, 50);
                } else if (nb_cages > 2 && obj->sub_etat == 48 && EOA(obj)) {
                    set_sub_etat(obj, 49);
                } else if (nb_cages > 1 && obj->sub_etat == 47 && EOA(obj)) {
                    set_sub_etat(obj, 48);
                }
            }

            DO_ANIM(obj);
        }
    }
    if (unlocked) {
        chemin_percent = 0;
        for (i32 i = 0; i < COUNT(t_world_info); ++i) {
            world_info_t* world_info = t_world_info + i;
            if (!(world_info->state & 2) && (world_info->state & 1)) {
                world_info->state |= 2;
            }
        }
    }
}

//67D50
void INIT_LEVEL_STAGE_NAME(void) {
    memcpy(text_to_display[0].text, t_world_info[num_world_choice].text, 48);
    text_to_display[0].xpos = 450;
    text_to_display[0].ypos = 193;
    text_to_display[0].font_size = 2;
    text_to_display[0].is_fond = 0;
    text_to_display[0].field_D5 = 0;
    INIT_TXT_BOX(&text_to_display[0]);
    text_to_display[0].centered_y_pos -= 6;
    text_to_display[0].width += 10;
    text_to_display[0].height += 6;
    text_to_display[0].color = t_world_info[num_world_choice].color;
}

//67E40
void INIT_WORLD_STAGE_NAME(void) {
    i8 new_color = 0;
    switch (t_world_info[num_world_choice].world) {
        case 1: {
            strcpy(text_to_display[2].text, language_txt[24]); // /the dream forest/
            new_color = 5;
        } break;
        case 2: {
            strcpy(text_to_display[2].text, language_txt[25]); // /band land/
            new_color = 3;
        } break;
        case 3: {
            strcpy(text_to_display[2].text, language_txt[26]); // /blue mountains/
            new_color = 0;
        } break;
        case 4: {
            strcpy(text_to_display[2].text, language_txt[27]); // /picture city/
            new_color = 2;
        } break;
        case 5: {
            strcpy(text_to_display[2].text, language_txt[28]); // /the caves of skops/
            new_color = 1;
        } break;
        case 6: {
            strcpy(text_to_display[2].text, language_txt[29]); // /candy chateau/
            new_color = 1;
        } break;
        case 7: {
            new_color = t_world_info[num_world_choice].color;
            switch(fichier_selectionne) {
                case 0: {
                    strcpy(text_to_display[2].text, language_txt[161]); // /password/
                } break;
                case 1: {
                    strncpy(text_to_display[2].text, save_ray[0], 4);
                } break;
                case 2: {
                    strncpy(text_to_display[2].text, save_ray[1], 4);
                } break;
                case 3: {
                    strncpy(text_to_display[2].text, save_ray[2], 4);
                } break;
                default: break;
            }
        } break;
        default: break;
    }
    text_to_display[2].color = new_color;
    text_to_display[2].font_size = 1;
    text_to_display[2].xpos = 450;
    text_to_display[2].ypos = 30;
    text_to_display[2].is_fond = 0;
    text_to_display[2].field_D5 = 0;
    INIT_TXT_BOX(&text_to_display[2]);
    text_to_display[2].width += 10;
    text_to_display[2].height += 2;
}

//68120
void INIT_STAGE_NAME(void) {
    INIT_TEXT_TO_DISPLAY();
    INIT_LEVEL_STAGE_NAME();
    INIT_WORLD_STAGE_NAME();
    text_to_display[1].xpos = 160;
    text_to_display[1].ypos = 250;
    text_to_display[1].field_D5 = 0;
    text_to_display[1].is_fond = 0;
    text_to_display[3].xpos = 150;
    text_to_display[3].ypos = -50;
    text_to_display[3].field_D5 = 0;
    text_to_display[3].is_fond = 0;
}

//6817C
void CHANGE_STAGE_NAMES(void) {
    text_to_display[1] = text_to_display[0];
    INIT_LEVEL_STAGE_NAME();
    if (t_world_info[num_world_choice].world != t_world_info[old_num_world].world) {
        text_to_display[3] = text_to_display[2];
        INIT_WORLD_STAGE_NAME();
    }
}

//68208
void DO_STAGE_NAMES(void) {
    if (ModeAutoJumelle) {
        // World map idle animation with letters bouncing everywhere
        //stub
    } else {
        if (text_to_display[0].xpos > 160) {
            text_to_display[0].xpos -= 6;
            text_to_display[0].centered_x_pos -= 6;
        }
        if (text_to_display[2].xpos > 160) {
            text_to_display[2].xpos -= 6;
            text_to_display[2].centered_x_pos -= 6;
        }
        if (text_to_display[1].ypos < 293) {
            text_to_display[1].ypos += 2;
            text_to_display[1].centered_y_pos += 2;
        }
        if (text_to_display[3].ypos > -70) {
            text_to_display[3].ypos -= 2;
            text_to_display[3].centered_y_pos -= 2;
        }
    }

    switch(PositionStageNameCalcule) {
        case 1: {
            //stub
        } break;

    }

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
    ray.sprites = raytmp.sprites;
    ray.img_buffer = raytmp.img_buffer;
    ray.scale = 0;
    ray.nb_sprites = raytmp.nb_sprites;
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
    ray.x = initial_world_info->xpos - ray.offset_bx + 4;
    ray.y = initial_world_info->ypos - ray.offset_by + 8;
    ray.speed_x = 0;
    ray.speed_y = 0;
    set_zoom_mode(0);
    chemin_percent = 0;
    Nb_total_cages = 0;
    for (i32 i = 0; i < 24; ++i) {
        obj_t* medaillon = mapobj + i;
        world_info_t* world_info = t_world_info + i;
        medaillon->type = TYPE_54_MEDAILLON;
        medaillon->init_x = world_info->xpos - 78;
        medaillon->init_y = world_info->ypos - 64;
        medaillon->init_main_etat = 5;
        if (world_info->state & 4) {
            medaillon->init_sub_etat = 46;
        } else {
            i32 cages = world_info->nb_cages;
            if (cages != 0) {
                Nb_total_cages += cages;
                if (cages == 6) {
                    medaillon->init_sub_etat = 52;
                } else {
                    medaillon->init_sub_etat = 47;
                }
            } else if (i == 17) {
                medaillon->sub_etat = 59;
                medaillon->init_sub_etat = 59;
            } else {
                medaillon->init_sub_etat = 39;
            }
        }
        medaillon->scale = 0;
        medaillon->offset_bx = 80;
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
    ray.flags.flip_x = dir_on_wldmap & 1;

    // If it's a new game, create the new save file on disk
    if (nouvelle_partie) {
        if (NBRE_SAVE != 0) {
            SaveGameOnDisk(fichier_selectionne);
        }
        nouvelle_partie = 0;
    }

    INIT_PASTILLES_SAUVE();
    INIT_STAGE_NAME();

    // Added: unlock first world if flag is set incorrectly
    if (!(t_world_info[17].state & 1)) {
        t_world_info[17].state |= 1;
    }
}

//68F38
void RESPOND_TO_UP(void) {
    num_world_choice = t_world_info[num_world_choice].index_up;
    ChangeJumelleSizeOK &= ~4;
    if (PositionStageNameCalcule == 1) {
        PositionStageNameCalcule = 2;
    }
}

//68F88
void RESPOND_TO_DOWN(void) {
    num_world_choice = t_world_info[num_world_choice].index_down;
    ChangeJumelleSizeOK &= ~4;
    if (PositionStageNameCalcule == 1) {
        PositionStageNameCalcule = 2;
    }
}

//68FD8
void RESPOND_TO_RIGHT(void) {
    num_world_choice = t_world_info[num_world_choice].index_right;
    ChangeJumelleSizeOK &= ~4;
    if (PositionStageNameCalcule == 1) {
        PositionStageNameCalcule = 2;
    }
}

//69028
void RESPOND_TO_LEFT(void) {
    num_world_choice = t_world_info[num_world_choice].index_left;
    ChangeJumelleSizeOK &= ~4;
    if (PositionStageNameCalcule == 1) {
        PositionStageNameCalcule = 2;
    }
}

//69078
void DO_RAYMAN_IN_WLD_MAP(void) {
    if (old_num_world == num_world_choice) {
        if (ValidButPressed()) {
            PositionStageNameCalcule = 0;
            ModeAutoJumelle = 0;
            if (num_world_choice == 18 || num_world_choice == 19 || num_world_choice == 20 || num_world_choice == 21 ||
                num_world_choice == 22 || num_world_choice == 23
            ) {
                if (t_world_info[num_world_choice].text == language_txt[150] /* /save game/ */ &&
                    NBRE_SAVE != 0 &&
                    strcmp(text_to_display[2].text, language_txt[161] /* /password/ */) != 0
                ) {
                    // Erase previously saved game?
                    if (confirmation_msg(3)) {
                        SaveGameOnDisk(fichier_selectionne);
                        PASTILLES_SAUVE_SAVED(num_world_choice);
                        CHANGE_STAGE_NAMES();
                    }
                }
            } else {
                new_world = 1;
            }
            ChangeJumelleSizeOK &= ~4;
        } else if (rightjoy()) {
            RESPOND_TO_RIGHT();
        } else if (leftjoy()) {
            RESPOND_TO_LEFT();
        } else if (downjoy()) {
            RESPOND_TO_DOWN();
        } else if (upjoy()) {
            RESPOND_TO_UP();
        }

        if (num_world_choice != old_num_world) {
            if (t_world_info[num_world_choice].state & 1) {
                ray.timer = 0;
                set_main_and_sub_etat(&ray, 1, 0);
                CHANGE_STAGE_NAMES();
            } else {
                num_world_choice = old_num_world;
            }
        }
    } else {
        i16 diff_x = t_world_info[num_world_choice].xpos - t_world_info[old_num_world].xpos;
        i16 diff_y = t_world_info[num_world_choice].ypos - t_world_info[old_num_world].ypos;
        if (diff_x < 0) {
            ray.flags.flip_x = false;
        } else {
            ray.flags.flip_x = true;
        }
        i16 xspeed = 0; // speed x?
        i16 yspeed = 0; // speed y?
        if (diff_x != 0 && diff_y != 0) {
            if (Abs(diff_y) > Abs(diff_x)) {
                xspeed = Abs(ray.timer * diff_x / diff_y);
                yspeed = ray.timer;
            } else {
                xspeed = ray.timer;
                yspeed = Abs(ray.timer * diff_y / diff_x);
            }
        } else if (diff_x == 0) {
            yspeed = ray.timer;
        } else /*if (diff_y == 0)*/ {
            xspeed = ray.timer;
        }
        ++ray.timer;

        ray.speed_x = t_world_info[old_num_world].xpos + xspeed * sgn(diff_x) - ray.offset_bx - ray.x;
        ray.speed_y = t_world_info[old_num_world].ypos + yspeed * sgn(diff_y) - ray.offset_by - (ray.y - 8);

        if (Abs(xspeed) >= Abs(diff_x) && Abs(yspeed) >= Abs(diff_y)) {
            // arrived at new location
            old_num_world = num_world_choice;
            if (ray.main_etat != 0) {
                set_main_and_sub_etat(&ray, 0, 0);
                ray.speed_x = 0;
                ray.speed_y = 0;
                ray.x = t_world_info[num_world_choice].xpos - ray.offset_bx;
                ray.y = t_world_info[num_world_choice].ypos - ray.offset_by + 8;
            }
        }
    }
}

//6947C
void DO_CHEMIN(void) {
    horloges(1);
    RecaleRayPosInJumelle();
    DoScrollInWorldMap(h_scroll_speed, v_scroll_speed);
    MoveRayInWorldMap();
    set_proj_center(ray.screen_x + ray.offset_bx, ray.screen_y + ray.offset_by);
    DO_ANIM(&ray);
    DO_MEDAILLONS();
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
void PASTILLES_SAUVE_SAVED(i16 world) {
    INIT_PASTILLES_SAUVE();
    t_world_info[world].text = language_txt[151]; // /game saved/
}

//69570
void FIN_WORLD_CHOICE(void) {
    DO_FADE_OUT();
    xwldmapsave = xmap;
    ywldmapsave = ymap;
    xmap = xmapinit;
    ymap = ymapinit;
    dir_on_wldmap = ray.flags.flip_x;
    RESTORE_RAY();
    INIT_PASTILLES_SAUVE();
    PROC_EXIT = 0;
    if (ray.hit_points == -1) {
        ray.hit_points = 0;
    }
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
    ray.hit_points = 2;
    fin_du_jeu = 0;
    nb_continue = 9;
    ray.flags.flag_1 = false;
    poing.sub_etat = 1;
    departlevel = 1;
    memset(wi_save_zone, 0, sizeof(wi_save_zone));
    memset(save_zone, 0, sizeof(save_zone));
    memset(&RayEvts, 0, sizeof(RayEvts)); // added; TODO: figure out why RayEvts doesn't 'leak' into a new game?
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
        LoadInfoGame(i + 1);
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
    to_display->field_D5 = 0;
    to_display->ypos = debut_sortie;
    to_display->color = 5;
    ecartx = 21;
    ecarty = ecart_options;
}

//69DCC
void SAISIE_NOM(void) {
    TestCompteur();
    if (!fichier_existant && action != 1) {
        *(u32*)(save_ray[positiony-1]) = *(u32*)"aaa";
        fichier_existant = 1;
    }
    if (affiche_bon_ecran) {
        fin_saisie_nom = 1;
    }

    if (ValidButPressed() && button_released) {
        char* pos = save_ray[fichier_selectionne - 1] + positionx - 1;
        if (*pos == '~') {
            if (positionx != 1) {
                *pos = 'a';
                --positionx;
                PlaySnd_old(68);
            }
        } else if (positionx >= 3) {
            PlaySnd_old(68);
            affiche_bon_ecran = 1;
        } else {
            ++positionx;
            PlaySnd_old(68);
        }
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
    }

    if (upjoy() && !rightjoy() && !leftjoy() && (button_released || (compteur % 5 == 0))) {
        char* pos = save_ray[fichier_selectionne - 1] + positionx - 1;
        switch (*pos) {
            case 'z': {
                *pos = ' ';
            } break;
            case ' ': {
                *pos = '~';
            } break;
            case '~': {
                *pos  = 'a';
            } break;
            default: {
                *pos += 1;
            } break;
        }
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
        PlaySnd_old(68);
    }

    if (downjoy() && !rightjoy() && !leftjoy() && (button_released || (compteur % 5 == 0))) {
        char* pos = save_ray[fichier_selectionne - 1] + positionx - 1;
        switch (*pos) {
            case 'a': {
                *pos = '~';
            } break;
            case '~': {
                *pos = ' ';
            } break;
            case ' ': {
                *pos  = 'z';
            } break;
            default: {
                *pos -= 1;
            } break;
        }
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
        PlaySnd_old(68);
    }

    if (!upjoy() && !downjoy() && !rightjoy() && !leftjoy() && !affiche_bon_ecran && !fin_saisie_nom) {
        clignotement = 1;
    }

    if (SelectButPressed()) {
        MENU_RETURN = 1;
        PlaySnd_old(77);
    }
    TestButtonReleased();
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
            if (save_ray[positiony-1][0] == '\0') {
                break;
            }
            if (confirmation_msg(1)) {
                fichier_selectionne = positiony;
                REMOVE_FICHIER();
                save_ray[fichier_selectionne-1][0] = '\0';
            }
            realisation_effectuee = 1;
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
    bigray.cmd = 0;
    bigray.flags.flip_x = false;
}

//6B258
void DO_LOADER_ANIM(void) {
    eta_t* eta = get_eta(&bigray);
    bigray.speed_x = 0;
    if ((eta->anim_speed & 15) != 0 && (horloge[eta->anim_speed] == 0)) {
        SET_X_SPEED(&bigray);
    }
    bigray.screen_x += bigray.speed_x;
    if (PROC_EXIT == 1) {
        PROC_EXIT = 0;
        bigray.cmd = 5;
        set_main_and_sub_etat(&bigray, 0, 2);
        bigray.timer = 2;
        bigray.flags.flip_x = false;
        bigray.screen_x = 160 - bigray.offset_bx - 16;
    }

    if (bigray.cmd <= 5) {
        switch(bigray.cmd) {
            default: break;
            case 0: {
                if (bigray.screen_x + bigray.offset_bx < -100) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 1);
                    bigray.screen_x = (-bigray.offset_bx) - 60;
                    bigray.flags.flip_x = true;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 1: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 2);
                    bigray.anim_frame = 0;
                    bigray.screen_x = 350 - bigray.offset_bx;
                    bigray.flags.flip_x = false;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 2: {
                if (bigray.main_etat == 0 && bigray.sub_etat == 0) {
                    ++bigray.cmd;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 3: {
                if (EOA(&bigray)) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 3);
                    bigray.screen_x = -60 - bigray.offset_bx;
                    bigray.flags.flip_x = true;
                    DO_ANIM(&bigray);
                } else {
                    DO_ANIM(&bigray);
                }
            } break;
            case 4: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 0, 1);
                    bigray.screen_x = 160 - bigray.offset_bx - 16;
                    bigray.flags.flip_x = false;
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
    anim_sequence = NULL;
    switch (num_world) {
        case world_1_jungle: {
            if (num_level == 16 && (new_level == 1 || new_world == 1)) {
                anim_sequence = car_sequence;
            }
            if (num_level == 7 && (new_level == 1 || new_world == 1)) {
                anim_sequence = moust2_sequence;
                dontdoit = 0;
            } else if (num_level == 7 && !dontdoit) {
                anim_sequence = moust1_sequence;
                if (!fin_du_jeu) {
                    dontdoit = 1;
                }
            }
        } break;
        case world_2_music: {
            if (num_level == 16 && new_world == 1) {
                anim_sequence = pst1_sequence;
            }
        } break;
        case world_3_mountain: {
            if (num_level == 10 && new_level == 1) {
                anim_sequence = pst1_sequence;
            }
        } break;
        case world_4_image: {
            if (num_level == 11 && (new_level == 1 || new_world == 1)) {
                anim_sequence = pst2_sequence;
            }
        } break;
        case world_5_cave: {
            if (num_level == 11 && (new_level == 1 || new_world == 1)) {
                anim_sequence = worry_sequence;
            }
        } break;
        default: break;
    }
    PROC_EXIT = 0;
    if (anim_sequence && !fin_du_jeu) {
        raytmp = ray;
        ray.cmd = GO_LEFT;
        ray.speed_x = 0;
        ray.screen_y = 168 - ray.offset_by;
        anim_func = DO_LEVEL_ANIM;
    } else {
        PROC_EXIT = 1;
    }
}

//6B73C
void DO_LEVEL_ANIM(void) {
    //stub
}

//6BA34
void FIN_LEVEL_ANIM(void) {
    PROC_EXIT = 0;
    ray = raytmp;
    DO_FADE_OUT();
    INIT_FADE_IN();
}


