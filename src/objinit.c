
//5E160 (adapted from PS1 decomp)
u8 Prio(obj_t* obj) {
    s32 res;

    switch (obj->type)
    {
        case TYPE_149_CLE_SOL: // Changed from 2 to 1 in the PC version
        case TYPE_157_EAU:
        case TYPE_197_MEDAILLON_TOON:
            res = 1;
            break;
        case TYPE_30_AUDIOSTART:
        case TYPE_98_SPACE_MAMA:
        case TYPE_204_BLACK_RAY:
        case TYPE_213_SPACE_MAMA2:
        case TYPE_236_ANNULE_SORT_DARK:
        case TYPE_238_POING_FEE:
        case TYPE_245_DUNE:
            // NOTE: these below are added in the PC version.
            // In the PS1 version, they are explicitly not drawn in DISPLAY_ALL_OBJECTS() instead
        case TYPE_141_NEIGE:
        case TYPE_158_PALETTE_SWAPPER:
        case TYPE_164_GENERATING_DOOR:
        case TYPE_181_SCROLL_SAX:
        case TYPE_199_BB1_VIT:
            res = 0;
            break;
        case TYPE_147_MST_SCROLL:
            res = (obj->hit_points == 0) ? 2 : 0;
            break;
        case TYPE_11_BOUM:
        case TYPE_19_SPLASH:
        case TYPE_21_PHOTOGRAPHE:
        case TYPE_41_OUYE:
        case TYPE_45_MOVE_OUYE:
        case TYPE_48_FLAMME2:
        case TYPE_57_STALAG:
        case TYPE_78_PIRATE_BOMB: // Changed from 3 to 2 in the PC version
        case TYPE_79_STONECHIP:
        case TYPE_83_EXPLOSION:
        case TYPE_90_NOTE0:
        case TYPE_91_NOTE1:
        case TYPE_92_NOTE2:
        case TYPE_93_BONNE_NOTE:
        case TYPE_94_POING: // Added in PC version
        case TYPE_102_BNOTE:
        case TYPE_110_PI_BOUM:
        case TYPE_121_ECLAIR:
        case TYPE_135_ETINC:
        case TYPE_143_NOVA2:
        case TYPE_146_FLASH:
        case TYPE_150_SCORPION:
        case TYPE_161_WIZ:
        case TYPE_168_CYMBAL1:
        case TYPE_170_RAYON:
        case TYPE_220_PIERREACORDE:
        case TYPE_221_CFUMEE:
        case TYPE_224_STOSKO_PINCE:
        case TYPE_234_LAVE:
        case TYPE_248_SKO_PINCE:
            res = 2;
            break;
        case TYPE_123_BLACKTOON1:
            res = (num_world == 1 && num_level == 14) ? 3 : 2; // Changed from always 2 in the PC version
            break;
        case TYPE_28_ONOFF_PLAT:
        case TYPE_44_CLASH:
        case TYPE_46_BB1:
        case TYPE_58_CAGE: // Changed from 2 to 3 in the PC version
        case TYPE_59_CAGE2: // Changed from 2 to 3 in the PC version
        case TYPE_66_DARD:
        case TYPE_72_PIRATE_NGAWE:
        case TYPE_73_RING:
        case TYPE_74_SAXO:
        case TYPE_77_PIRATE_GUETTEUR:
        case TYPE_86_MARACAS:
        case TYPE_97_BBL:
        case TYPE_119_TNT_BOMB:
        case TYPE_133_SUPERHELICO:
        case TYPE_138_ROULETTE:
        case TYPE_154_ROULETTE2:
        case TYPE_155_ROULETTE3:
        case TYPE_180_SAXO2:
        case TYPE_183_SAXO3:
        case TYPE_187_MAMA_PIRATE:
        case TYPE_190_COUTEAU:
        case TYPE_198_BB12:
        case TYPE_200_BB13:
        case TYPE_201_BB14:
        case TYPE_203_SMA_WEAPON:
        case TYPE_211_BOUT_TOTEM:
        case TYPE_239_PIRATE_GUETTEUR2:
        case TYPE_249_RIDEAU:
            res = 3;
            break;
        case TYPE_253_PLANCHES:
            res = (bateau_obj_id != -1) ? 7 : 4;
            break;
        case TYPE_7_MORNINGSTAR:
        case TYPE_20_GENEBADGUY:
        case TYPE_96_TOTEM: // Added in PC version
        case TYPE_109_PI:
        case TYPE_111_PI_MUS:
        case TYPE_112_WASHING_MACHINE:
        case TYPE_246_CORDE_DARK: // Added in PC version
        case TYPE_251_VAGUE_DEVANT:
            res = 5;
            break;
        case TYPE_2_POWERUP:
        case TYPE_31_ONEUP_ALWAYS: // Added in PC version
        case TYPE_55_MUS_WAIT:
        case TYPE_82_JAUGEUP:
        case TYPE_95_POING_POWERUP:
        case TYPE_137_REDUCTEUR:
        case TYPE_142_ONEUP:
        case TYPE_148_GRAP_BONUS:
        case TYPE_173_BATEAU:
            res = 6;
            break;
        case TYPE_4_NEUTRAL:
        case TYPE_42_SIGNPOST:
        case TYPE_88_TAMBOUR2:
        case TYPE_252_VAGUE_DERRIERE:
            res = 7;
            break;
        default:
            res = 4;
            break;
    }

    return res;
}

//5E640
void first_obj_init(obj_t* obj) {
    s16 type = obj->type;
    if ((flags[type] & 1) != 0 || ((type == TYPE_9_BADGUY2 || type == TYPE_0_BADGUY1 ) && obj->x < 0)) {
        obj->x = -32000;
        obj->y = -32000;
    }
    obj->scale = 0;
    obj->init_x = obj->x;
    obj->init_y = obj->y;
    obj->init_main_etat = obj->main_etat;
    obj->init_sub_etat = obj->sub_etat;
    obj->init_hit_points = obj->hit_points;
    obj->active_flag = 1;

    switch(type) {
        case TYPE_5_WIZARD1:
        case TYPE_50_MOSKITO:
        case TYPE_65_SPIDER:
        case TYPE_227_MOSKITO2: {
            obj->detect_zone = 60;
            obj->link = 0;
        } break;
        case TYPE_12_CHASSEUR1:
        case TYPE_14_CHASSEUR2:
        case TYPE_20_GENEBADGUY:
        case TYPE_183_SAXO3: {
            obj->detect_zone = 180;
            obj->link = 0;
        } break;
        case TYPE_32_DARK_PHASE2:
        case TYPE_61_WAT_CLOWN:
        case TYPE_174_PIRATE_POELLE:
        case TYPE_187_MAMA_PIRATE:
        case TYPE_212_DARK:
        case TYPE_225_PIRATE_P_45: {
            obj->detect_zone = 150;
            obj->link = 0;
        }
        case TYPE_35_STONEMAN1:
        case TYPE_40_STONEDOG:
        case TYPE_43_STONEMAN2:
        case TYPE_55_MUS_WAIT:
        case TYPE_72_PIRATE_NGAWE:
        case TYPE_120_BATTEUR_FOU:
        case TYPE_122_STONEDOG2: {
            obj->detect_zone = 100;
            obj->link = 0;
        } break;
        case TYPE_56_STONEWOMAN2:
        case TYPE_60_BIG_CLOWN:
        case TYPE_100_MITE:
        case TYPE_172_STONEWOMAN: {
            obj->detect_zone = 220;
            obj->link = 0;
        } break;
        case TYPE_57_STALAG: {
            obj->detect_zone = 250;
            obj->link = 0;
        } break;
        case TYPE_70_TROMPETTE:
        case TYPE_150_SCORPION:
        case TYPE_217_MITE2: {
            obj->detect_zone = -96;
            obj->link = 0;
        } break;
        case TYPE_81_CYMBALE:
        case TYPE_168_CYMBAL1:
        case TYPE_169_CYMBAL2: {
            obj->detect_zone = 0;
            obj->link = 0;
        } break;
        case TYPE_98_SPACE_MAMA:
        case TYPE_213_SPACE_MAMA2: {
            findMereDenisWeapon(); //TODO
            obj->link = 0;
        } break;
        case TYPE_113_BAG1: {
            obj->param = 0;
            obj->link = 0;
        } break;
        case TYPE_116_CLOWN_TNT:
        case TYPE_153_FEE: {
            obj->detect_zone = 200;
            obj->link = 0;
        } break;
        case TYPE_184_PIRATE_POELLE_D:
        case TYPE_226_PIRATE_P_D_45: {
            obj->detect_zone = 50;
            obj->link = 0;
        } break;
        default: {
            obj->detect_zone = 40;
            obj->link = 0;
        }
    }
}

//5E9BC (adapted from the PS1 decomp, with cross-checks with PC/Android versions)
void obj_init(obj_t* obj) {
    s16 unk_1;

    obj->nb_cmd = 0;
    obj->is_active = 0;
    obj->display_prio = Prio(obj);
    obj->zdc = type_zdc[obj->type];
    if (obj->type != TYPE_163_INDICATOR) {
        obj->main_etat = obj->init_main_etat;
        obj->sub_etat = obj->init_sub_etat;
    }
    obj->anim_frame = 0;
    obj->speed_x = 0;
    obj->speed_y = 0;
    obj->x = obj->init_x;
    obj->y = obj->init_y;
    obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
    obj->change_anim_mode = ANIMMODE_NONE;
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
    obj->detect_zone_flag = 0;
    obj->iframes_timer = -1;
    obj->follow_id = -1;
    obj->cmd_offset = -1;
    obj->flags.flip_x = false;
    obj->flags.read_commands = (obj->cmds != NULL);
    memset(obj->btypes, 0, 5);
    GET_OBJ_CMD(obj);
    obj->flags.anim_changed = false;
    obj->flags.flag_0x40 = false;
    obj->flags.flag_0x100 = false;
    obj->test_block_index = 0;
    obj->cmd_context_depth = -1;
    obj->active_flag = 1; //ACTIVE_DEAD;
    obj->timer = 0;
    obj->config = 0;
    obj->hit_points = obj->init_hit_points;

    switch (obj->type)
    {
        case TYPE_74_SAXO:
        case TYPE_180_SAXO2:
        case TYPE_183_SAXO3:
            INIT_SAXO(obj);
            break;
        case TYPE_228_PRI:
            if (finBosslevel.helped_joe_2)
            {
                prise_branchee = true;
                set_main_and_sub_etat(obj, 0, 2);
                obj->init_sub_etat = 2;
                skipToLabel(obj, 1, true);
                break;
            }
            break;
        case TYPE_251_VAGUE_DEVANT:
        case TYPE_252_VAGUE_DERRIERE:
            obj->x = -15;
            obj->init_x = -15;
            break;
        case TYPE_218_HYBRIDE_MOSAMS:
            bossXToReach = obj->x;
            bossYToReach = firstFloorBelow(&ray) - obj->offset_by + 16; // TODO
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            break;
        case TYPE_176_CRAYON_BAS:
        case TYPE_240_CRAYON_HAUT:
            obj->anim_frame = myRand(obj->animations[obj->anim_index].frame_count - 1);
            break;
        case TYPE_191_FALLING_CRAYON:
        case TYPE_235_PUNAISE4:
            obj->anim_frame = obj->hit_points;
            break;
        case TYPE_175_PUNAISE1:
        case TYPE_229_PUNAISE2:
        case TYPE_230_PUNAISE3:
        case TYPE_250_PUNAISE5:
            obj->flags.flip_x = (obj->hit_points & 1);
            break;
        case TYPE_51_MST_FRUIT1:
        case TYPE_53_MST_SHAKY_FRUIT:
            obj->gravity_value_2 = 5;
            break;
        case TYPE_52_MST_FRUIT2:
            obj->gravity_value_2 = 10;
            break;
        case TYPE_98_SPACE_MAMA:
            obj->type = TYPE_213_SPACE_MAMA2;
        case TYPE_213_SPACE_MAMA2:
            swapMereDenisCollZones(obj, 1); // TODO
            setBossScrollLimits(obj); // TODO
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            laserSourceSprNumInAnim = -1;
            IsBossThere = false;
            scrollLocked = false;
            obj->timer = 180;
            saveBossEncounter = 255;
            currentBossAction = 0;
            currentBossActionIsOver = false;
            bossEncounter = 0;
            break;
        case TYPE_227_MOSKITO2:
            bossEncounter = 2;
            obj->scale = 255; // added in PC/Android
            setBossScrollLimits(obj);
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            IsBossThere = false;
            scrollLocked = false;
            obj->timer = 100;
            saveBossEncounter = 255;
            currentBossAction = 0;
            currentBossActionIsOver = false;
            break;
        case TYPE_50_MOSKITO:
            bossEncounter = 0;
            obj->scale = 255; // added in PC/Android
            setBossScrollLimits(obj);
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            IsBossThere = false;
            scrollLocked = false;
            obj->timer = 100;
            saveBossEncounter = 255;
            currentBossAction = 0;
            currentBossActionIsOver = false;
            break;
        case TYPE_0_BADGUY1:
            obj->offset_hy = 30;
            obj->flags.follow_enabled = false;
            break;
        case TYPE_144_LIDOLPINK2:
            obj->iframes_timer = 0;
            break;
        case TYPE_123_BLACKTOON1:
            switch (obj->follow_sprite)
            {
                case 1:
                    obj->config = 2;
                    obj->detect_zone = 0;
                    break;
                case 2:
                    obj->config = 8;
                    obj->detect_zone = 60;
                    break;
                case 3:
                    obj->config = 24;
                    obj->detect_zone = 170;
                    break;
                case 4:
                    obj->config = 10;
                    obj->detect_zone = 100;
                    break;
                case 5:
                    obj->follow_id = 0;
                    obj->config = 1;
                    obj->detect_zone = 0;
                    break;
                case 6:
                    obj->config = 4;
                    obj->detect_zone = 60;
                    break;
                case 7:
                    obj->config = 36;
                    obj->detect_zone = 120;
                    break;
            }
            break;
        case TYPE_145_KILLING_EYES:
            obj->iframes_timer = 40;
            obj->param = 40;
            break;
        case TYPE_72_PIRATE_NGAWE:
            obj->config = 1;
            break;
        case TYPE_77_PIRATE_GUETTEUR:
        case TYPE_239_PIRATE_GUETTEUR2:
            swapGuetteurCollZones(obj, obj->init_sub_etat == 14); //TODO
            obj->iframes_timer = 0;
            obj->detect_zone = 80;
            break;
        case TYPE_28_ONOFF_PLAT:
            obj->iframes_timer = 100;
            obj->follow_id = 100;
            obj->flags.alive = true;
            break;
        case TYPE_26_CRUMBLE_PLAT:
            if (num_world != 1) {
                s16 unk_2 = (s16)(vblToEOA(obj, 1) - 2);
                obj->param = unk_2;
                obj->iframes_timer = unk_2;
            } else {
                obj->param = 40;
                obj->iframes_timer = 20;
            }
            obj->link = 20;
            obj->flags.alive = true;
            obj->flags.follow_enabled = true;
            break;
        case TYPE_25_INST_PLAT:
            obj->iframes_timer = 15;
            obj->follow_id = 15;
            obj->flags.alive = true;
            break;
        case TYPE_29_AUTOJUMP_PLAT:
        case TYPE_34_MOVE_AUTOJUMP_PLAT:
        case TYPE_152_CAISSE_CLAIRE:
        case TYPE_237_GOMME:
        case TYPE_243_MARK_AUTOJUMP_PLAT:
            obj->iframes_timer = 1;
            obj->follow_id = 1;
            obj->flags.alive = true;
            break;
        case TYPE_116_CLOWN_TNT:
        case TYPE_117_CLOWN_TNT2:
        case TYPE_118_CLOWN_TNT3:
            obj->config = 0; // this line is missing in the Android version
            obj->link = 0;
            break;
        case TYPE_168_CYMBAL1:
        case TYPE_169_CYMBAL2:
        case TYPE_178_HERSE_BAS:
        case TYPE_241_HERSE_HAUT:
            obj->link = 0;
            break;
        case TYPE_150_SCORPION:
            obj->detect_zone = 160;
            sko_phase = 0;
            break;
        case TYPE_7_MORNINGSTAR:
        case TYPE_21_PHOTOGRAPHE:
            obj->flags.alive = true;
            break;
        case TYPE_184_PIRATE_POELLE_D:
        case TYPE_226_PIRATE_P_D_45:
            obj->flags.flip_x = true;
            break;
        case TYPE_212_DARK:
            obj->param = 0;
            break;
        case TYPE_217_MITE2:
            obj->eta[1][0].flags &= ~0x10;
            obj->eta[1][2].flags &= ~0x10;
            obj->follow_id = 0;
            break;
        case TYPE_202_RAY_ETOILES:
            if (obj->main_etat == 0 && obj->sub_etat == 57)
                star_ray_der = obj;
            else
                star_ray_dev = obj;
            obj->display_prio = 3;
            // fallthrough
        case TYPE_170_RAYON:
        case TYPE_197_MEDAILLON_TOON:
            obj->flags.alive = false;
            obj->is_active = 0;
            break;
    }
}

//5F1E8
void init_struct_level(void) {
    level_alw.nb_objects = 0;
    level_obj.nb_objects = 0;
    for (s32 i = 0; i < level.nb_objects; ++i) {
        level_alw.obj_ids[i] = 0;
        level_obj.obj_ids[i] = 0;
    }
    for (s16 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (bonus_map || (flags[obj->type] & flags1_2_is_collectible) == 0 || !bonus_taken(obj->id)) {
            if (flags[obj->type] & flags0_1_always) {
                level_alw.obj_ids[level_alw.nb_objects++] = i;
            } else {
                level_obj.obj_ids[level_obj.nb_objects++] = i;
            }
        }
    }
}

//5F2D4
void INIT_OBJECTS(u8 a1) {
    ray_on_poelle = 0;
    nb_cymbal_in_map = 0;
    IsBossThere = 0;
    Mus_obj_id = -1;
    NumScrollObj = 0;
    rayman_obj_id = -1;
    reduced_rayman_id = -1;
    id_Cling_1up = -1;
    id_Cling_Pow = -1;
    fee_obj_id = -1;
    eau_obj_id = -1;
    mst_scroll_obj_id = -1;
    bateau_obj_id = -1;
    mama_pirate_obj_id = -1;
    black_ray_obj_id = -1;
    pierreAcorde_obj_id = -1;
    rideau_obj_id = -1;
    cb_ball_obj_id = -1;
    stosko_obj_id = -1;
    moskitomama_droite_obj_id = -1;
    moskitomama_gauche_obj_id = -1;
    moskitosaxo_obj_id = -1;
    corde_dark_obj_id = -1;
    nb_wiz = 0;

    // NOTE: added null initialization for extra robustness (defend against TYPE_202_RAY_ETOILES missing from the level)
    star_ray_der = NULL;
    star_ray_dev = NULL;

    s16 found_bbf2_gauche = -1;
    s16 found_bbf2_droite = -1;

    for (s16 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        obj->id = i;
        if (a1) {
            first_obj_init(obj);
        }
        obj->active_timer = 0;
        if (obj->type == TYPE_163_INDICATOR) {
            obj->main_etat = obj->init_main_etat;
            obj->sub_etat = obj->init_sub_etat;
        }
        obj_init(obj);
        if (flags[obj->type] & flags0_1_always) {
            obj->x = -32000;
            obj->y = -32000;
            obj->flags.alive = 0;
        } else {
            obj->flags.alive = 1;
        }

        switch(obj->type) {
            case TYPE_3_LIDOLPINK: {
                obj->init_flag = 7;
                obj->flags.alive = 0;
            } break;
            case TYPE_9_BADGUY2:
            case TYPE_165_BADGUY3: {
                obj->eta[2][2].sound_index = 0;
            } break;
            case TYPE_10_FISH: {
                if (i > 0) {
                    obj_t* prev_obj = level.objects + i - 1;
                    obj->flags.alive = 1;
                    if (prev_obj->type == TYPE_10_FISH && prev_obj->x == obj->x) {
                        obj->flags.alive = 0;
                        obj->is_active = 0;
                    }
                }
            } break;
            case TYPE_11_BOUM: {
                sbar_obj_id = i;
                obj->init_flag = 7;
                obj->flags.alive = 0;
            } break;
            case TYPE_13_BALLE1:
            case TYPE_15_BALLE2:
            case TYPE_33_DARK2_SORT:
            case TYPE_36_STONEBOMB:
            case TYPE_44_CLASH:
            case TYPE_51_MST_FRUIT1:
            case TYPE_52_MST_FRUIT2:
            case TYPE_59_CAGE2:
            case TYPE_62_DROP:
            case TYPE_66_DARD:
            case TYPE_71_NOTE:
            case TYPE_73_RING:
            case TYPE_78_PIRATE_BOMB:
            case TYPE_79_STONECHIP:
            case TYPE_83_EXPLOSION:
            case TYPE_90_NOTE0:
            case TYPE_91_NOTE1:
            case TYPE_92_NOTE2:
            case TYPE_93_BONNE_NOTE:
            case TYPE_97_BBL:
            case TYPE_102_BNOTE:
            case TYPE_108_DARK2_PINK_FLY:
            case TYPE_110_PI_BOUM:
            case TYPE_112_WASHING_MACHINE:
            case TYPE_119_TNT_BOMB:
            case TYPE_121_ECLAIR:
            case TYPE_122_STONEDOG2:
            case TYPE_130_BLACKTOON_EYES:
            case TYPE_135_ETINC:
            case TYPE_139_MARACAS_BAS:
            case TYPE_143_NOVA2:
            case TYPE_146_FLASH:
            case TYPE_186_POELLE_ALWAYS:
            case TYPE_192_SMA_GRAND_LASER:
            case TYPE_193_SMA_BOMB:
            case TYPE_194_SMA_BOMB_CHIP:
            case TYPE_196_DARD_PLAFOND:
            case TYPE_197_MEDAILLON_TOON:
            case TYPE_206_PIEDS_RAYMAN:
            case TYPE_209_FIRE_LEFT:
            case TYPE_210_FIRE_RIGHT:
            case TYPE_215_DARK_SORT:
            case TYPE_233_HYB_BBF2_LAS:
            case TYPE_244_SMA_PETIT_LASER: {
                obj->flags.alive = 0;
                obj->is_active = 0;
                obj->init_flag = 7;
            } break;
            case TYPE_19_SPLASH:
            case TYPE_203_SMA_WEAPON:
            case TYPE_224_STOSKO_PINCE:
            case TYPE_238_POING_FEE: {
                obj->flags.alive = 0;
            } break;
            case TYPE_20_GENEBADGUY: {
                obj->init_flag = 7;
                obj->timer = 0;
            } break;
            case TYPE_21_PHOTOGRAPHE: {
                obj->timer = 0;
            } break;
            case TYPE_23_RAYMAN: {
                obj->flags.alive = 0;
                rayman_obj_id = i;
            } break;
            case TYPE_31_ONEUP_ALWAYS: {
                obj->flags.alive = 0;
                obj->init_flag = 7;
            } break;
            case TYPE_32_DARK_PHASE2: {
                obj->flags.alive = 0;
                obj->is_active = 0;
                obj->init_x = 0;
                obj->x = 0;
                obj->init_y = 0;
                obj->y = 0;
            } break;
            case TYPE_37_TARZAN: {
                obj->flags.alive = !RayEvts.magicseed;
            } break;
            case TYPE_38_GRAINE: {
                obj->flags.alive = 0;
            } break;
            case TYPE_39_NEN_GRAINE: {
                obj->iframes_timer = 0;
                obj->flags.alive = 0;
            } break;
            case TYPE_46_BB1:
            case TYPE_198_BB12:
            case TYPE_200_BB13: {
                INIT_BBMONT(obj);
            } break;
            case TYPE_55_MUS_WAIT: {
                obj->init_flag = 7;
                Mus_obj_id = obj->id;
                if (finBosslevel.helped_musician) {
                    obj->init_sub_etat = 4;
                    set_main_and_sub_etat(obj, 0, 4);
                }
            } break;
            case TYPE_64_SCROLL: {
                scroll_obj_id[NumScrollObj++] = i;
                obj->y = obj->init_y;
                obj->x = obj->init_x;
                if (NumScrollObj >= 50) {
                    FatalError("Augmenter MAX_NB_SCROLL_PER_MAP");
                }
                obj->flags.alive = 0;
            } break;
            case TYPE_65_SPIDER: {
                obj->link = 0;
            } break;
            case TYPE_81_CYMBALE: {
                cymbal_obj_id[nb_cymbal_in_map++] = i;
            } break;
            case TYPE_82_JAUGEUP:
            case TYPE_142_ONEUP:
            case TYPE_148_GRAP_BONUS: {
                obj->init_flag = 7;
            } break;
            case TYPE_94_POING: {
                obj->flags.alive = 0;
                obj->init_sub_etat = poing.sub_etat;
                obj->sub_etat = poing.sub_etat;
                poing_obj_id = i;
                poing_obj = obj;
                obj->init_flag = 7;
            } break;
            case TYPE_96_TOTEM: {
                obj->param = 0;
                obj->iframes_timer = 0;
            } break;
            case TYPE_99_RAY_POS: {
                if (save1.is_just_saved && !bonus_map) {
                    obj->flags.alive = 0;
                }
                if (obj->main_etat == 0) {
                    obj->timer = 0;
                    obj->is_active = 0;
                } else if (obj->main_etat == 5) {
                    obj->timer = 30;
                }
            } break;
            case TYPE_109_PI: {
                obj->init_flag = 7;
            } break;
            case TYPE_111_PI_MUS: {
                if (finBosslevel.helped_musician) {
                    set_sub_etat(obj, 10);
                    obj->init_sub_etat = 10;
                }
            } break;
            case TYPE_115_BB1_PLAT: {
                obj->flags.alive = 0;
                obj->is_active = 0;
                obj->init_flag = 7;
            } break;
            case TYPE_133_SUPERHELICO: {
                if (!RayEvts.super_helico) {
                    obj->flags.alive = finBosslevel.helped_musician;
                } else {
                    obj->flags.alive = 0;
                }
            } break;
            case TYPE_136_DEMI_RAYMAN: {
                obj->flags.alive = 0;
                obj->is_active = 0;
                reduced_rayman_id = i;
            } break;
            case TYPE_145_KILLING_EYES: {
                obj->iframes_timer = 0;
            } break;
            case TYPE_147_MST_SCROLL: {
                if (obj->hit_points == 0) {
                    obj->init_x = ray.x - 200;
                    obj->x = obj->init_x;
                    obj->y = ray.y - 50;
                    set_main_and_sub_etat(obj, 0, 9);
                    obj->speed_x = 2;
                    obj->speed_y = 0;
                    obj->is_active = 1;
                    obj->param = 0;
                    obj->link = 0;
                    obj->timer = 0;
                    mst_scroll_obj_id = i;
                    obj->flags.flip_x = 1;
                }
            }
            case TYPE_153_FEE: {
                fee_obj_id = i;
            } break;
            case TYPE_157_EAU: {
                if (obj->hit_points != 0) {
                    obj->x = xmap - obj->offset_bx;
                    if (num_world == world_1_jungle) {
                        if (RayEvts.magicseed) {
                            obj->hit_points = 1;
                        }
                        obj->y = ymapmax + SCREEN_HEIGHT;
                    } else {
                        obj->param = obj->x;
                        obj->hit_points = 0;
                        if (num_world == world_3_mountain && num_level == 7) {
                            obj->sub_speed = 0;
                        } else {
                            obj->y = ymapmax + (SCREEN_HEIGHT - 80);
                            if (num_world == world_5_cave && num_level == 8) {
                                obj->y = ymapmax + Bloc_lim_H2 - 80;
                            } else if (num_world == world_4_image) {
                                obj->y = ymapmax + (SCREEN_HEIGHT - 70);
                            }
                            obj->init_y = obj->y;
                        }
                    }
                    obj->is_active = 1;
                    eau_obj_id = i;
                    obj->flags.alive = 1;
                    obj->iframes_timer = 0;
                    calc_obj_pos(obj);
                } else {
                    if (eau_obj_id == -1 || num_world == world_1_jungle) {
                        obj->flags.alive = 0;
                        obj->is_active = 0;
                    } else {
                        obj->speed_y = 0;
                        obj->speed_x = 0;
                        obj->flags.flip_x = 0;
                        obj_t* eau = level.objects + eau_obj_id;
                        eau->param += 101;
                        obj->x = eau->param;
                        obj->y = eau->y;
                        if (num_world == world_3_mountain && num_level == 7) {
                            obj->init_y = obj->y;
                            obj->sub_speed = 0;
                        } else {
                            obj->init_y = ymapmax + (SCREEN_HEIGHT - 80);
                            if (num_world == world_4_image) {
                                obj->init_y = (SCREEN_HEIGHT - 70); //TODO: fix this (doesn't seem right)
                            }
                            if (num_world == world_5_cave && num_level == 8) {
                                obj->y = eau->y + eau_obj_id - obj->id; //TODO: fix this (doesn't seem right)
                                obj->init_y = obj->y;
                            }
                        }
                        calc_obj_pos(obj);
                        obj->flags.alive = 1;
                        obj->is_active = 1;
                        ++eau->hit_points;
                        obj->sub_etat = eau->hit_points;
                        if (obj->sub_etat > 3) {
                            obj->sub_etat -= 4;
                        }
                        obj->iframes_timer = 0;
                    }
                }
            } break;
            case TYPE_173_BATEAU: {
                obj->param = -1;
                obj->link = 0;
                obj->timer = 0;
                bateau_obj_id = i;
            } break;
            case TYPE_179_HERSE_BAS_NEXT:
            case TYPE_242_HERSE_HAUT_NEXT: {
                if (obj->field_1C == 0) {
                    obj->flags.alive = 0;
                    obj->is_active = 0;
                }
            } break;
            case TYPE_187_MAMA_PIRATE: {
                mama_pirate_obj_id = i;
            } break;
            case TYPE_190_COUTEAU: {
                obj->follow_id = 1;
                obj->is_active = 0;
                obj->init_flag = 7;
                obj->flags.alive = 0;
                obj->iframes_timer = 1;
            } break;
            case TYPE_204_BLACK_RAY: {
                obj->is_active = 0;
                obj->flags.alive = 0;
                obj->init_flag = 7;
                obj->animations = ray.animations;
                black_ray_obj_id = i;
            } break;
            case TYPE_205_BLACK_FIST: {
                obj->flags.alive = 0;
                black_fist_obj_id = obj->id;
            } break;
            case TYPE_218_HYBRIDE_MOSAMS: {
                obj->param = 0;
                moskitosaxo_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_219_CORDE: {
                obj->hit_points = obj->init_hit_points >> 2;
                if (pierreAcorde_obj_id != -1) {
                    // NOTE: this code does nothing, obj->y is overwritten by obj->init_y later (bug in the game?)
                    obj_t* pierre_a_corde = level.objects + pierreAcorde_obj_id;
                    if (obj->x < pierre_a_corde->x) {
                        obj->y = pierre_a_corde->x + 112;
                    } else {
                        obj->y = pierre_a_corde->x + 120;
                    }
                    obj->y = obj->init_y; // ?
                }
            } break;
            case TYPE_220_PIERREACORDE: {
                pierreAcorde_obj_id = i;
                obj->iframes_timer = 0;
            } break;
            case TYPE_223_HYBRIDE_STOSKO: {
                obj->param = 0;
                stosko_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_231_HYB_BBF2_D: {
                found_bbf2_droite = obj->id;
                obj->param = 0;
                moskitomama_droite_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_232_HYB_BBF2_G: {
                found_bbf2_gauche = obj->id;
                obj->param = 0;
                moskitomama_gauche_obj_id = obj->id;
                obj->flags.flip_x = 1;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_246_CORDE_DARK: {
                corde_dark_obj_id = obj->id;
            } break;
            case TYPE_249_RIDEAU: {
                if (obj->init_sub_etat == 2) {
                    rideau_obj_id = obj->id;
                }
            } break;
            case TYPE_257_BREAKOUT_GAME: {
                cb_ball_obj_id = obj->id;
            } break;
        }

        // Take away any leftover cages if nb_cages has been set to 6 (e.g. through a cheat?)
        if (t_world_info[world_index].nb_cages == 6 && obj->type == TYPE_58_CAGE) {
            obj->flags.alive = 0;
            obj->is_active = 0;
            take_bonus(obj->id);
        }

        // Disable bonuses (collectibles) that were already taken
        if (!bonus_map && (flags[obj->type] & flags1_2_is_collectible) && bonus_taken(obj->id)) {
            obj->flags.alive = 0;
            obj->is_active = 0;
        }

        // Count the tings in bonus stages
        if (bonus_map && obj->type == TYPE_161_WIZ) {
            ++nb_wiz;
        }
    }

    // Link moskitomama objects to each other
    if (found_bbf2_gauche != -1 && found_bbf2_droite != -1) {
        level.objects[found_bbf2_gauche].link = found_bbf2_droite;
        level.objects[found_bbf2_droite].link = found_bbf2_gauche;
    }

    if (bonus_map) {
        nb_wiz_save = status_bar.num_wiz;
        status_bar.num_wiz = 0;
    }

    init_struct_level();
}

//60288
void REINIT_OBJECT(obj_t* obj) {
    obj_init(obj);
    obj->flags.alive = 1;
    obj->active_flag = 2;
    calc_obj_pos(obj);
    if (obj->link_has_gendoor) {
        obj->active_timer = 0;
    } else {
        obj->active_timer = 120;
    }
    obj->is_active = 0;
}

//602C4
void special_flags_init(void) {
    for (s32 i = 0; i < COUNT(flags); ++i) {
        if (i == TYPE_0_BADGUY1) {
            if (num_level == 3) {
                zonediffx[i] = -120;
            } else {
                zonediffx[i] = 0;
            }
        }
        if (i == TYPE_17_LIFTPLAT || i == TYPE_16_FALLPLAT || i == TYPE_24_INTERACTPLT) {
            if (num_world == world_1_jungle) {
                flags[i] |= flags1_8_ray_dist_multispr_cantchange;
            } else {
                flags[i] &= ~flags1_8_ray_dist_multispr_cantchange;
            }
        }
    }
}

