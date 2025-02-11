
//60AD0
void popCmdContext(obj_t* obj) {
    //stub
}

//60AE4
i16 char2short(u8 x) {
    i16 result = x;
    if (x > 0x7F) {
        result = (i16)(x - 256);
    }
    return result;
}

//60AFC
u8 readNoArg(obj_t* obj) {
    return 0;
}

//60AFC
u8 skipNoArg(obj_t* obj) {
    return 0;
}

//60B00
u8 readOneArg(obj_t* obj) {
    ++obj->cmd_offset;
    obj->nb_cmd = obj->cmds[obj->cmd_offset];
    return 0;
}

//60B20
u8 readTestArgs(obj_t* obj) {
    ++obj->cmd_offset;
    obj->nb_cmd = obj->cmds[obj->cmd_offset];
    if (obj->nb_cmd <= 4) {
        ++obj->cmd_offset;
        obj->cmd_arg_1 = obj->cmds[obj->cmd_offset];
    }
    return 0;
}

//60B5C
u8 readGoXYargs(obj_t* obj) {
    ++obj->cmd_offset;
    obj->nb_cmd = char2short(obj->cmds[obj->cmd_offset]);

    ++obj->cmd_offset;
    obj->phase = char2short(obj->cmds[obj->cmd_offset]);
    return 0;
}

//60BA8
u8 readSpeedArgs(obj_t* obj) {
    ++obj->cmd_offset;
    obj->nb_cmd = obj->cmds[obj->cmd_offset];

    ++obj->cmd_offset;
    obj->iframes_timer = char2short(obj->cmds[obj->cmd_offset]);

    ++obj->cmd_offset;
    obj->cmd_arg_2 = char2short(obj->cmds[obj->cmd_offset]);
    return 0;
}

//60C14
u8 readInvalidArg(obj_t* obj) {
    --obj->cmd_offset;
    readOneCommand(obj);
    return 1;
}

//60C24
u8 skipOneArg(obj_t* obj) {
    ++obj->cmd_offset;
    return 0;
}

//60C2C
u8 skipTestArgs(obj_t* obj) {
    ++obj->cmd_offset;
    if (obj->cmds[obj->cmd_offset] <= 4) {
        ++obj->cmd_offset;
    }
    return 0;
}

//60C4C
u8 skipGoXYArgs(obj_t* obj) {
    obj->cmd_offset += 2;
    return 0;
}

//60C54
u8 skipSpeedArgs(obj_t* obj) {
    obj->cmd_offset += 3;
    return 0;
}

//60C5C
void handle_SELF_HANDLED(obj_t* obj) {
    //stub
}

//60C60
u8 skipInvalidArg(obj_t* obj) {
    --obj->cmd_offset;
    return skipOneCommand(obj);
}

//60C6C
void handle_11_GO_LABEL(obj_t* obj) {
    //stub
}

//60C70
void handle_19_GO_WAITSTATE(obj_t* obj) {
    //stub
}

//60C90
void handle_25_RESERVED_GO_GOSUB(obj_t* obj) {
    //stub
}

//60CBC
void handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO(obj_t* obj) {
    //stub
}

//60CDC
void handle_26_RESERVED_GO_BRANCHTRUE(obj_t* obj) {
    //stub
}

//60D04
void handle_27_RESERVED_GO_BRANCHFALSE(obj_t* obj) {
    //stub
}

//60D2C
void handle_28_RESERVED_GO_SKIPTRUE(obj_t* obj) {
    //stub
}

//60D54
void handle_29_RESERVED_GO_SKIPFALSE(obj_t* obj) {
    //stub
}

//60D7C
void handle_21_GO_X(obj_t* obj) {
    //stub
}

//60DA8
void handle_22_GO_Y(obj_t* obj) {
    //stub
}

//60DD4
void handle_12_GO_GOTO(obj_t* obj) {
    //stub
}

//60DEC
void handle_8_GO_ETAT(obj_t* obj) {
    //stub
}

//60E0C
void handle_5_GO_SUBETAT(obj_t* obj) {
    //stub
}

//60E2C
void handle_6_GO_SKIP(obj_t* obj) {
    //stub
}

//60E50
void handle_9_GO_PREPARELOOP(obj_t* obj) {
    //stub
}

//60E60
void handle_13_GO_GOSUB(obj_t* obj) {
    //stub
}

//60E88
void handle_14_GO_RETURN(obj_t* obj) {
    //stub
}

//60E90
void handle_10_GO_DOLOOP(obj_t* obj) {
    //stub
}

//60EC4
void handle_33_INVALID_CMD(obj_t* obj) {
    //stub
}

//60ED0
void handle_15_GO_BRANCHTRUE(obj_t* obj) {
    //stub
}

//60EF4
void handle_16_GO_BRANCHFALSE(obj_t* obj) {
    //stub
}

//60F18
void handle_31_GO_SKIPTRUE(obj_t* obj) {
    //stub
}

//60F44
void handle_32_GO_SKIPFALSE(obj_t* obj) {
    //stub
}

//60F70
void handle_18_GO_SETTEST(obj_t* obj) {
    //stub
}

//60F98
void handle_17_GO_TEST(obj_t* obj) {
    //stub
}

//61154
void readOneCommand(obj_t* obj) {
    //stub
}

//61180
u8 skipOneCommand(obj_t* obj) {
    return 0; //stub
}

//611AC
void GET_OBJ_CMD(obj_t* obj) {
    //stub
}

//611F4
void pushCmdContext(obj_t* obj, u8 count) {
    //stub
}

//61220
void skipToLabel(obj_t* obj, u8 a2, u8 a3) {
    //stub
}

//612C0
void pushToLabel(obj_t* obj, u8 a2, u8 a3) {
    //stub
}

//612F0
void ObjectUTurnDefault(obj_t* obj) {
    obj->flags.flip_x = !obj->flags.flip_x;
}

//61320
void DO_WIZARD(obj_t* obj) {
    //stub
}

//614B4
void get_next_bonus_level(obj_t* obj) {
    //stub
}

//6154C
void TEST_WIZARD(obj_t* obj) {
    //stub
}

//61894
void DO_TARZAN(obj_t* obj) {
    //stub
}

//61968
void DoTarzanPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//61980
void allocate_badguy(obj_t* tentacle_obj, i16 which_enemy, i16 xspeed, i16 yspeed) {
    // stub
    for (i32 event_index = 0; event_index < level.nb_objects; ++event_index) {
        obj_t* spawned = level.objects + event_index;
        if (((which_enemy == 1 && spawned->type == TYPE_0_BADGUY1) || (which_enemy == 2 && spawned->type == TYPE_9_BADGUY2))
            && spawned->init_x <= 0 && spawned->is_active == 0
                ) {
            spawned->flags.alive = false;
            spawned->flags.flag_0x40 = false;
            spawned->is_active = 1;
            spawned->flags.alive = true; // Note: this does not make sense, this bit was cleared just before!
            spawned->active_timer = 0;
            spawned->active_flag = 1;
            set_main_and_sub_etat(spawned, 2, 2); // thrown up in air
            spawned->x = tentacle_obj->x + tentacle_obj->offset_bx - spawned->offset_bx;
            spawned->y = tentacle_obj->y - (spawned->offset_by / 4);
            calc_obj_dir(spawned);
            spawned->speed_y = yspeed;
            spawned->speed_x = spawned->flags.flip_x ? xspeed : -xspeed;
            calc_obj_pos(spawned);
            spawned->flags.read_commands = false;
            return;
        }
    }
}

//61A90
void DO_PTI_ESQUIVE(obj_t* obj) {
    //stub
}

//61BB0
void DoPrisePoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//61C08
void DO_PETIT_COUTEAU_COMMAND(obj_t* obj) {
    //stub
}

//61C20
void DoPetitCouteauPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//61C34
void DO_TIRE_BOUCHON_COMMAND(obj_t* obj) {
    //stub
}

//61C4C
void DoOneUpPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//61C84
void DoOneUpRaymanCollision(obj_t* obj) {
    //stub
}

//61D10
void DoMorningStarPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//61D54
void DoGrapBonusRaymanCollision(obj_t* obj) {
    //stub
}

//61DB0
void DoPoingPowerupRaymanCollision(obj_t* obj) {
    //stub
}

//61EA0
void DoPowerupRaymanCollision(obj_t* obj) {
    DO_NOVA(obj);
    ray.hit_points += obj->hit_points;
    if (ray.hit_points > ray_max_hitp) {
        ray.hit_points = ray_max_hitp;
    }
    obj->flags.alive = false;
    PlaySnd(8, obj->obj_index);

}

//61EE4
void DoSuperHelicoRaymanCollision(obj_t* obj) {
    //stub
}

//61F20
void DoGraineRaymanCollision(obj_t* obj) {
    //stub
}

//61F50
void DoWizRaymanCollision(obj_t* obj) {
    //stub
}

//62024
void DoJaugeUpRaymanCollision(obj_t* obj) {
    //stub
}

//620C4
void DoGeneratingDoorRaymanCollision(obj_t* obj) {
    //stub
}

//620E4
void DO_REDUCTEUR(obj_t* obj) {
    //stub
}

//62104
void DoReducteurRaymanCollision(obj_t* obj) {
    //stub
}

//62140
void DoSignPostRaymanCollision(obj_t* obj) {
    //stub
}

//62150
void TEST_SIGNPOST(obj_t* obj) {
    //stub
}

//6252C
void DoPancarteRaymanCollision(obj_t* obj) {
    //stub
}

//625E8
void DO_MUSICIEN(obj_t* obj) {
    //stub
}

//62644
void DoMusicienRaymanInZDD(obj_t* obj) {
    //stub
}

//62658
// sub_62658
void DO_TEN_COMMAND(obj_t* obj) {
    if (obj->cmd == cmd_0_left) {
        obj_set_not_flipped(obj);
    } else if (obj->cmd == cmd_1_right) {
        obj_set_flipped(obj);
    }
    if (obj->main_etat == 0 && obj->sub_etat == 11) {
        if (obj->anim_frame >= 5 && obj->timer == 0) {
            ++obj->timer;
            ++obj->configuration; // alternate between 1=livingstone and 2=small livingstone
            if (obj->configuration > 2) {
                obj->configuration = 1;
            }
            if (obj->hit_points == 1) {
                allocate_badguy(obj, 2 /*small livingstone*/, 1, -2);
                allocate_badguy(obj, 1 /*livingstone*/      , 2, -1);
            } else {
                allocate_badguy(obj, obj->configuration   , 1, -1);
            }
        } else {
            if (obj->anim_frame > 2) {
                obj->timer = 0;
            }
        }
    } else if (obj->main_etat == 0 && obj->sub_etat == 21) {
        if (obj->anim_frame >= 5 && obj->timer == 0) {
            // last hit: spawn 4 enemies
            ++obj->timer;
            i16 xspeed = -4;
            for (i32 i = 0; i < 4; ++i) {
                i16 which_enemy = (i % 2) + 1;
                i16 yspeed = -which_enemy;
                ++xspeed;
                allocate_badguy(obj, which_enemy, xspeed, yspeed);
                if (xspeed == -2) xspeed = 3;
            }
        } else {
            if (obj->anim_frame > 2) {
                obj->timer = 0;
            }
        }
    } else if (obj->main_etat == 0 && obj->sub_etat == 3) {
        ++obj->timer;
        if (!(obj->configuration == 0 || obj->timer < -3)) {
            u8 saved_configuration = obj->configuration;
            //DO_INTERACT_PLAT(obj); // TODO: fix this
            obj->configuration = saved_configuration;
        }
        obj->speed_x = 0;
        obj->speed_y = 0;
        if (obj->anim_frame > 12) {
            obj->anim_frame = 3;
        } else if (obj->anim_frame <= 1){
            obj->timer = 0;
        }
    } else if (obj->main_etat == 1) {
        if (obj->phase != 0) ++obj->timer;
        if (obj->timer >= -2) {
            skipToLabel(obj, 2, 1);
        }
        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
    }
}

//62854
void DoGeneBadGuyPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//628B8
void DoGeneBadGuyRaymanZDD(obj_t* obj) {
    obj->phase = 1;
}

//628C0
void DoChasseurPoingCollision(obj_t* obj, i16 a2) {
    if (get_eta(obj)->interaction_flags & 1) {
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            obj->speed_x = 0;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 1); // startled
        } else {
            set_main_and_sub_etat(obj, 0, 3); // dead
        }
        obj->cmd = obj->flags.flip_x ? cmd_0_left : cmd_1_right;
    }
}

//62928
void DoChasseurRaymanZDD(obj_t* obj) {
    if (obj->main_etat == 0 && (obj->sub_etat == 0 || obj->sub_etat == 2)) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 2);
        obj->flags.read_commands = false;
        calc_obj_dir(obj);
        obj->cmd = obj->flags.flip_x ? cmd_1_right : cmd_0_left;
    }
}

//62988
void DO_CHASSEUR_COMMAND(obj_t* obj) {
    //stub
}

//629D8
void DoBadGuy23PoingCollision(obj_t* obj, i16 a2) {
    if (a2 == 255 || !(get_eta(obj)->interaction_flags & 1)) {
        obj_hurt(obj);
        if (obj->hit_points == 0) {
            set_main_and_sub_etat(obj, 0, 3);
            skipToLabel(obj, 2, 1);
            obj->y -= 2;
            obj->flags.read_commands = false;
        } else {
            //stub
        }
    }
}

//62AD4
void DoBadGuy23RaymanZDD(obj_t* obj) {
    if (obj->main_etat == 1 && obj->sub_etat == 11) {
        set_sub_etat(obj, 14);
    } else if ((obj->main_etat == 0 && obj->sub_etat == 0) || (obj->main_etat == 1 && obj->sub_etat == 0)) {
        set_main_and_sub_etat(obj, 1, 11);
        obj->flags.read_commands = false;
        skipToLabel(obj, obj->flags.flip_x ? 3 : 2, 1);
    }
}


//62B54
// sub_62B54
void DoBadGuy1PoingCollision(obj_t* obj, i16 a2) {
    obj_hurt(obj);
    if (poing_obj->speed_x > 0) {
        skipToLabel(obj, 3, 1);
    } else if (poing_obj->speed_x < 0) {
        skipToLabel(obj, 2, 1);
    }
    obj->speed_x = 0;
    obj->y -= 2;
    if (obj->hit_points != 0) {
        obj->speed_y = -4;
        set_main_and_sub_etat(obj, 2, (get_eta(obj)->interaction_flags & 0x40) ? 10 : 2);
        PlaySnd(28, obj->obj_index);
    } else {
        obj->speed_y = -8;
        set_main_and_sub_etat(obj, 0, (get_eta(obj)->interaction_flags & 0x40) ? 6 : 3);
    }

}

//62C40
void DoBadGuy1RaymanZDD(obj_t* obj) {
    if (obj->flags.flip_x != ray.flags.flip_x) {
        if ((obj->main_etat == 1 && obj->sub_etat == 0) || (obj->main_etat == 0 && obj->sub_etat == 0)) {
            if (ray.main_etat == 0 && ray.sub_etat == 18 /* stick out tongue */) {
                obj->speed_x = 0;
                obj->speed_y = 0;
                set_main_and_sub_etat(obj, 0, 2);
                skipToLabel(obj, obj->flags.flip_x ? 8 : 7, 1);
            }
        }
    }
}

//62CD4
void DO_CCL_COMMAND(obj_t* obj) {
    //stub
}

//62D68
void DoCaisseClairePoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//62DC4
void DoCaisseClaireRaymanZDD(obj_t* obj) {
    //stub
}

//62E10
void DoStalagRaymanZDD(obj_t* obj) {
    //stub
}

//62E2C
void DO_ENSEIGNE_COMMAND(obj_t* obj) {
    //stub
}

//62ED0
void DO_JOE_COMMAND(obj_t* obj) {
    //stub
}

//62FCC
void DoJoeRaymanZDD(obj_t* obj) {
    //stub
}

//63030
void DO_BOUEE_JOE_COMMAND(obj_t* obj) {
    //stub
}

//6309C
void DO_PHOTOGRAPHE_CMD(obj_t* obj) {
    //stub
}

//6323C
void DoAudioStartRaymanCollision(obj_t* obj) {
    //stub
}

//63244
void DO_SLOPEY_PLAT_CMD(obj_t* obj) {
    //stub
}

//63340
void DO_PIRATE_POELLE(obj_t* obj) {
    //stub
}

//63610
void DO_PIRATE_POELLE_POING_COLLISION(obj_t* obj, i16 a2) {
    //stub
}

//6374C
void PIRATE_POELLE_REACT(obj_t* obj) {
    //stub
}

//63800
void DO_SPECIAL_PLATFORM(obj_t* obj) {
    //stub
}

//643BC
void DoPlatformPoingCollision(obj_t* obj, i16 a2) {
    if (obj->hit_sprite == a2) {
        if (poing_obj->speed_x > 0) {
            set_sub_etat(obj, 25);
        } else if (poing_obj->speed_x < 0) {
            set_sub_etat(obj, 26);
        }
    }
}

