
//5E160 (adapted from PS1 decomp)
u8 Prio(obj_t* obj) {
    s32 res;

    switch (obj->type)
    {
        case TYPE_EAU:
        case TYPE_MEDAILLON_TOON:
            res = 1;
            break;
        case TYPE_AUDIOSTART:
        case TYPE_SPACE_MAMA:
        case TYPE_BLACK_RAY:
        case TYPE_SPACE_MAMA2:
        case TYPE_ANNULE_SORT_DARK:
        case TYPE_POING_FEE:
        case TYPE_DUNE:
            res = 0;
            break;
        case TYPE_MST_SCROLL:
            res = (obj->hitp == 0) ? 2 : 0;
            break;
        case TYPE_BOUM:
        case TYPE_SPLASH:
        case TYPE_PHOTOGRAPHE:
        case TYPE_OUYE:
        case TYPE_MOVE_OUYE:
        case TYPE_FLAMME2:
        case TYPE_STALAG:
        case TYPE_CAGE:
        case TYPE_CAGE2:
        case TYPE_STONECHIP:
        case TYPE_EXPLOSION:
        case TYPE_NOTE0:
        case TYPE_NOTE1:
        case TYPE_NOTE2:
        case TYPE_BONNE_NOTE:
        case TYPE_BNOTE:
        case TYPE_PI_BOUM:
        case TYPE_ECLAIR:
        case TYPE_BLACKTOON1:
        case TYPE_ETINC:
        case TYPE_NOVA2:
        case TYPE_FLASH:
        case TYPE_CLE_SOL:
        case TYPE_SCORPION:
        case TYPE_WIZ:
        case TYPE_CYMBAL1:
        case TYPE_RAYON:
        case TYPE_PIERREACORDE:
        case TYPE_CFUMEE:
        case TYPE_STOSKO_PINCE:
        case TYPE_LAVE:
        case TYPE_SKO_PINCE:
            res = 2;
            break;
        case TYPE_ONOFF_PLAT:
        case TYPE_CLASH:
        case TYPE_BB1:
        case TYPE_DARD:
        case TYPE_PIRATE_NGAWE:
        case TYPE_RING:
        case TYPE_SAXO:
        case TYPE_PIRATE_GUETTEUR:
        case TYPE_PIRATE_BOMB:
        case TYPE_MARACAS:
        case TYPE_BBL:
        case TYPE_TNT_BOMB:
        case TYPE_SUPERHELICO:
        case TYPE_ROULETTE:
        case TYPE_ROULETTE2:
        case TYPE_ROULETTE3:
        case TYPE_SAXO2:
        case TYPE_SAXO3:
        case TYPE_MAMA_PIRATE:
        case TYPE_COUTEAU:
        case TYPE_BB12:
        case TYPE_BB13:
        case TYPE_BB14:
        case TYPE_SMA_WEAPON:
        case TYPE_BOUT_TOTEM:
        case TYPE_PIRATE_GUETTEUR2:
        case TYPE_RIDEAU:
            res = 3;
            break;
        case TYPE_PLANCHES:
            res = (bateau_obj_id != -1) ? 7 : 4;
            break;
        case TYPE_MORNINGSTAR:
        case TYPE_GENEBADGUY:
        case TYPE_PI:
        case TYPE_PI_MUS:
        case TYPE_WASHING_MACHINE:
        case TYPE_VAGUE_DEVANT:
            res = 5;
            break;
        case TYPE_POWERUP:
        case TYPE_MUS_WAIT:
        case TYPE_JAUGEUP:
        case TYPE_POING_POWERUP:
        case TYPE_REDUCTEUR:
        case TYPE_ONEUP:
        case TYPE_GRAP_BONUS:
        case TYPE_BATEAU:
            res = 6;
            break;
        case TYPE_NEUTRAL:
        case TYPE_SIGNPOST:
        case TYPE_TAMBOUR2:
        case TYPE_VAGUE_DERRIERE:
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
    //stub
}

//5E9BC (adapted from the PS1 decomp, with cross-checks with PC/Android versions)
void obj_init(obj_t* obj) {
    s16 unk_1;

    obj->nb_cmd = 0;
    obj->is_active = 0;
    obj->display_prio = Prio(obj);
    obj->zdc_meta = type_zdc[obj->type];
    if (obj->type != TYPE_163_INDICATOR) {
        obj->main_etat = obj->spawn_etat;
        obj->sub_etat = obj->spawn_sub_etat;
    }
    obj->anim_frame = 0;
    obj->xspeed = 0;
    obj->yspeed = 0;
    obj->xpos = obj->spawn_x;
    obj->ypos = obj->spawn_y;
    obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
    obj->flags &= ~obj_flags_8_flipped;
    obj->change_anim_mode = 0; //ANIMMODE_NONE;
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
    obj->detect_zone = 0;
    obj->iframes_timer = -1;
    obj->command_par2 = -1;
    obj->cmd_offset = -1;
    obj->flags = (obj->flags & ~(obj_flags_0x10 | obj_flags_8_flipped)) | (obj_flags_0x10 * (obj->cmds != NULL));
    memset(obj->coll_btype, 0, 5);
    GET_OBJ_CMD(obj); //TODO
    obj->flags &= (obj_flags_0x80_anim_changed | obj_flags_0x40);
    obj->test_block_index = 0;
    obj->cmd_context_depth = -1;
    obj->active_flag = 1; //ACTIVE_DEAD;
    obj->field_30 = 1;
    obj->timer = 0;
    obj->configuration = 0;
    obj->hitp = obj->spawn_hitp;
    obj->field_82 &= ~1;

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
                obj->spawn_sub_etat = 2;
                skipToLabel(obj, 1, true);
                break;
            }
            break;
        case TYPE_251_VAGUE_DEVANT:
        case TYPE_252_VAGUE_DERRIERE:
            obj->xpos = -15;
            obj->spawn_x = -15;
            break;
        case TYPE_218_HYBRIDE_MOSAMS:
            bossXToReach = obj->xpos;
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
            obj->anim_frame = obj->hitp;
            break;
        case TYPE_175_PUNAISE1:
        case TYPE_229_PUNAISE2:
        case TYPE_230_PUNAISE3:
        case TYPE_250_PUNAISE5:
            obj->flags = (obj->flags & ~obj_flags_8_flipped) | (obj->hitp & 1) * obj_flags_8_flipped;
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
            obj->timer = -76;
            saveBossEncounter = -1;
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
            saveBossEncounter = -1;
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
            saveBossEncounter = -1;
            currentBossAction = 0;
            currentBossActionIsOver = false;
            break;
        case TYPE_0_BADGUY1:
            obj->offset_hy = 30;
            obj->flags &= ~obj_flags_0x20_follow_enabled;
            break;
        case TYPE_144_LIDOLPINK2:
            obj->iframes_timer = 0;
            break;
        case TYPE_123_BLACKTOON1:
            switch (obj->follow_sprite)
            {
                // TODO: figure out discrepancy between PS1 and PC/Android object state layouts.
                // In the switch statement below, the PS1 decomp has:  obj->detect_zone
                // In the PC/Android decomps: obj->active_flag (?)
                case 1:
                    obj->configuration = 2;
                    obj->detect_zone = 0; //TODO: check if correct
                    break;
                case 2:
                    obj->configuration = 8;
                    obj->detect_zone = 60; //TODO: check if correct
                    break;
                case 3:
                    obj->configuration = 24;
                    obj->detect_zone = 170; //TODO: check if correct
                    break;
                case 4:
                    obj->configuration = 10;
                    obj->detect_zone = 100; //TODO: check if correct
                    break;
                case 5:
                    obj->command_par2 = 0;
                    obj->configuration = 1;
                    obj->detect_zone = 0; //TODO: check if correct
                    break;
                case 6:
                    obj->configuration = 4;
                    obj->detect_zone = 60; //TODO: check if correct
                    break;
                case 7:
                    obj->configuration = 36;
                    obj->detect_zone = 120; //TODO: check if correct
                    break;
            }
            break;
        case TYPE_145_KILLING_EYES:
            obj->iframes_timer = 40;
            obj->cmd_arg_1 = 40;
            break;
        case TYPE_72_PIRATE_NGAWE:
            obj->configuration = 1;
            break;
        case TYPE_77_PIRATE_GUETTEUR:
        case TYPE_239_PIRATE_GUETTEUR2:
            swapGuetteurCollZones(obj, obj->spawn_sub_etat == 14); //TODO
            obj->iframes_timer = 0;
            obj->detect_zone = 80;
            break;
        case TYPE_28_ONOFF_PLAT:
            obj->iframes_timer = 100;
            obj->command_par2 = 100;
            obj->flags |= obj_flags_4_triggered;
            break;
        case TYPE_26_CRUMBLE_PLAT:
            if (num_world != 1) {
                i16 unk_2 = (i16)(vblToEOA(obj, 1) - 2);
                obj->cmd_arg_1 = unk_2;
                obj->iframes_timer = unk_2;
            } else {
                obj->cmd_arg_1 = 40;
                obj->iframes_timer = 20;
            }
            obj->phase = 20;
            obj->flags |= obj_flags_4_triggered | obj_flags_0x20_follow_enabled;
            break;
        case TYPE_25_INST_PLAT:
            obj->iframes_timer = 15;
            obj->command_par2 = 15;
            obj->flags |= obj_flags_4_triggered;
            break;
        case TYPE_29_AUTOJUMP_PLAT:
        case TYPE_34_MOVE_AUTOJUMP_PLAT:
        case TYPE_152_CAISSE_CLAIRE:
        case TYPE_237_GOMME:
        case TYPE_243_MARK_AUTOJUMP_PLAT:
            obj->iframes_timer = 1;
            obj->command_par2 = 1;
            obj->flags |= obj_flags_4_triggered;
            break;
        case TYPE_116_CLOWN_TNT:
        case TYPE_117_CLOWN_TNT2:
        case TYPE_118_CLOWN_TNT3:
            obj->configuration = 0; // this line is missing in the Android version
            obj->phase = 0;
            break;
        case TYPE_168_CYMBAL1:
        case TYPE_169_CYMBAL2:
        case TYPE_178_HERSE_BAS:
        case TYPE_241_HERSE_HAUT:
            obj->phase = 0;
            break;
        case TYPE_150_SCORPION:
            obj->detect_zone = 160; //TODO: check if correct
            sko_phase = 0;
            break;
        case TYPE_7_MORNINGSTAR:
        case TYPE_21_PHOTOGRAPHE:
            obj->flags |= obj_flags_4_triggered;
            break;
        case TYPE_184_PIRATE_POELLE_D:
        case TYPE_226_PIRATE_P_D_45:
            obj->flags |= obj_flags_8_flipped;
            break;
        case TYPE_212_DARK:
            obj->cmd_arg_1 = 0;
            break;
        case TYPE_217_MITE2:
            obj->eta[1][0].interaction_flags &= ~0x10;
            obj->eta[1][2].interaction_flags &= ~0x10;
            obj->command_par2 = 0;
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
            obj->flags &= ~obj_flags_4_triggered;
            obj->is_active = 0;
            break;
    }
}

//5F1E8
void init_struct_level(void) {
    //stub
}

//5F2D4
void INIT_OBJECTS(u8 a1) {
    //stub
}

//60288
void REINIT_OBJECT(obj_t* obj) {
    //stub
}

//602C4
void special_flags_init(void) {
    //stub
}

