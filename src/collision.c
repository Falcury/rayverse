
//2B2D0
i32 get_nb_zdc(obj_t* obj) {
    return obj->zdc >> 11;
}

//2B2E0
i32 get_zdc_index(obj_t* obj) {
    return (i32)(obj->zdc & 7);
}

//2B2E8
zdc_t* get_zdc(obj_t* obj, i32 a2) {
    return zdc_tab + get_zdc_index(obj) + a2;
}

//2B300
u16 get_ZDCPTR(void) {
    return ZDCPTR;
}

//2B308
i32 in_coll_sprite_list(obj_t* obj, i16 a2) {
    return 0; //stub
}

//2B3B8
bool box_inter_v_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    return false; //stub
}

//2B3F4
bool box_inter_h_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    return false; //stub
}

//2B430
i32 inter_box(i16 box1_x, i16 box1_y, i16 box1_width, i16 box1_height, i16 box2_x, i16 box2_y, i16 box2_width, i16 box2_height) {
    i32 temp1 = box1_x - box2_width;
    i32 temp2 = box1_y - box2_height;
    if (box2_x < temp1 || box2_y < temp2) {
        return 0;
    }
    i32 temp3 = temp1 + (box1_width + box2_width);
    if (box2_x <= temp3 && box2_width <= (box1_height + box2_height) + temp2) {
        return 1;
    } else {
        return 0;
    }
}

//2B48C
void GET_OBJ_ZDC(obj_t* obj) {
    switch(obj->type) {
        case TYPE_2_POWERUP:
        case TYPE_8_FALLING_OBJ:
        case TYPE_134_FALLING_OBJ2:
        case TYPE_167_FALLING_OBJ3: {
            //GET_ANIM_POS(obj, ..)
        } break;

            //stub
    }
}

//2C028
void GET_SPRITE_ZDC(obj_t* obj, i16 a2, i16* a3, i16* a4, i16* a5, i16* a6) {
    //stub
}

//2C33C
void BOX_HIT_SPECIAL_ZDC(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj) {
    //stub
}

//2C670
void BOX_IN_COLL_ZONES(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, obj_t* obj) {
    //stub
}

//2CA84
void COLL_BOX_SPRITE(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj) {
    //stub
}

//2CB1C
void CHECK_BOX_COLLISION(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, obj_t* obj) {
    //stub
}

//2CB9C
void possible_sprite(obj_t* obj, i16 a2) {
    //stub
}

//2CE34
void setToleranceDist(i16 a1, i16 a2, i16 a3) {
    //stub
}

//2CF14
void SET_RAY_DIST_SLOPEY_PLAT(obj_t* obj) {
    //stub
}

//2D0D4
void SET_RAY_DIST_MULTISPR_CANTCHANGE(obj_t* obj) {
    //stub
}

//2D334
void SET_RAY_DIST_PI(obj_t* obj) {
    //stub
}

//2D3EC
void SET_RAY_DIST_BAG(obj_t* obj) {
    //stub
}

//2D4D0
void SET_RAY_DIST(obj_t* obj) {
    //stub
}

//2D5CC
void do_boum(void) {
    //stub
}

//2D5FC
void DO_POING_COLLISION(void) {
    //stub
}

//2D858
void DoPoingCollisionDefault(obj_t* obj, i16 a2) {
    //stub
}

//2D85C
void COLL_BOX_ALL_SPRITES(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj) {
    //stub
}

//2D914
bool COLL_RAY_PIC(void) {
    return false; //stub
}

//2DA98
void COLL_RAY_BLK_MORTEL(void) {
    if ((ray.y + ray.offset_by) < (mp.height * 16)) {
        i16 btyp = mp.map[ray.ray_dist].tile_type;
        if (btyp == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            ray.speed_y = 0;
            ray.speed_x = 0;
            ray.y += 5;
        } else if (ray.coll_btype[3] == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            ray.speed_y = 0;
            ray.speed_x = 0;
        } else if (btyp == BTYP_CLIFF) {
            set_main_and_sub_etat(&ray, 2, 31);
            scroll_end_y = ymap;
            scroll_start_y = ymap;
        }
    }
}

//2DBAC
void RAY_KO(obj_t* obj) {
    //stub
}

//2DBDC
void RAY_HIT(bool hurt, obj_t* obj) {
    if (hurt) {
        RAY_HURT();
        i32 tile_x = (ray.x + ray.offset_bx) / 16;
        while (BTYP(tile_x, (ray.y + ray.offset_by) / 16) == BTYP_WATER) {
            --ray.y;
        }
        ray.coll_btype[0] = BTYP_NONE;
    }
    if (ray.main_etat == 6) {
        set_main_and_sub_etat(&ray, 6, 8);
        ray.speed_y = 0;
        ray.speed_x = 0;
        poing.is_charging = 0;
    } else if (ray.flags.alive) {
        if (!(ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32)) &&
            !(ray.main_etat == 2 && ray.sub_etat == 31)
        ) {
            if ((get_eta(&ray)->flags & 0x40) && (block_flags[calc_typ_trav(&ray, 2)] & 0x10)) {
                set_main_and_sub_etat(&ray, 0, 61);
            } else {
                set_main_and_sub_etat(&ray, 2, 8);
            }
            ray_speed_inv = 0;
            if (obj == NULL && !(ray.main_etat == 0 && ray.sub_etat == 61)) {
                ray.speed_x = ray.flags.flip_x ? -2 : 2;
                ray.speed_y = -3;
            } else if (obj && !(ray.main_etat == 0 && ray.sub_etat == 61)){
                i32 eject_sens = -1;
                if (obj->type == TYPE_180_SAXO2) {
                    eject_sens = saxo2_get_eject_sens();
                } else if (obj->type == TYPE_150_SCORPION) {
                    eject_sens = sko_get_eject_sens();
                } else if (obj->type == TYPE_198_BB12) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_200_BB13) {
                    eject_sens = -1;
                } else if (obj->type == TYPE_100_MITE) {
                    eject_sens = obj->flags.flip_x ? 1 : -1;
                } else if (obj->type == TYPE_120_BATTEUR_FOU) {
                    eject_sens = bat_get_eject_sens(obj);
                } else if (obj->type == TYPE_187_MAMA_PIRATE) {
                    eject_sens = pma_get_eject_sens(obj);
                } else if (obj->type == TYPE_209_FIRE_LEFT) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_210_FIRE_RIGHT) {
                    eject_sens = -1;
                } else {
                    i32 xspeed_delta = obj->speed_x - ray.speed_x;
                    if (xspeed_delta == 0) {
                        eject_sens = ray.flags.flip_x ? 1 : -1;
                    } else {
                        eject_sens = xspeed_delta > 0 ? 1 : -1;
                    }
                }
                i32 bump_speed;
                if (flags[obj->type] & flags1_4_fast_bump) {
                    bump_speed = 5;
                } else if (obj->type == TYPE_180_SAXO2) {
                    bump_speed = 4;
                } else {
                    bump_speed = 2;
                }
                ray.speed_x = bump_speed * eject_sens; // Note: eject_sens is always 1 or -1
                ray.speed_y = ~bump_speed;
            }

        }
    }
}

//2DF34
void standard_frontZone(obj_t* obj, i16* x, i16* w) {
    /* 1FA54 80144254 -O2 -msoft-float */
    if (!obj->flags.flip_x)
        *x -= obj->detect_zone;
    else
        *x += (*w >> 1);
    *w = obj->detect_zone + (*w >> 1);
}

//2DF88
void SET_DETECT_ZONE_FLAG(obj_t* obj) {
    /* 1FAB4 801442B4 -O2 -msoft-float */

    s16 obj_x; s16 obj_y; s16 obj_w; s16 obj_h;
    s32 unk_1;
    u16 det_zn_flg;

    GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
    switch (obj->type)
    {
        case TYPE_FEE:
            obj_x += (obj_h >> 1) - (obj->detect_zone >> 1);
            obj_y -= 16;
            obj_w = obj->detect_zone;
            obj_h += 64;
            break;
        case TYPE_MST_SHAKY_FRUIT:
            obj_y += obj_h;
            obj_h += 240;
            break;
        case TYPE_PIRATE_GUETTEUR:
        case TYPE_PIRATE_GUETTEUR2:
            standard_frontZone(obj, &obj_x, &obj_w);
            if (obj->main_etat == 0 && obj->sub_etat == 4)
            {
                obj_y += (obj_h >> 1);
                obj_h += (obj->detect_zone << 2) + (obj_h >> 1);
            }
            break;
        case TYPE_BLACKTOON1:
            unk_1 = -1;
            switch (obj->follow_sprite)
            {
                case 3:
                    if (obj->main_etat == 0 && obj->sub_etat == 0)
                    {
                        obj_h += obj->detect_zone;
                        obj_y -= obj->detect_zone;
                        obj_w += obj->detect_zone;
                        obj_x -= (obj->detect_zone >> 1);
                    }
                    break;
                case 7:
                    if (obj->main_etat == 1 && obj->sub_etat == 1)
                    {
                        obj_x = obj_x + unk_1 + (obj_w >> 1);
                        obj_y += (obj_h >> 1);
                        obj_w = 20;
                        obj_h += obj->detect_zone;
                        if (!obj->flags.flip_x)
                            obj_x -= obj_w;
                    }
                    break;
                case 4:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_x -= (obj_w >> 1);
                    obj_w += obj_w;
                    obj_h += obj->detect_zone;
                    break;
                case 2:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_h = obj->detect_zone + obj_h;
                    obj_w += obj->detect_zone * 2;
                    obj_x -= obj->detect_zone;
                    break;
            }
            break;
        case TYPE_POI3:
            obj_x += (obj->flags.flip_x) ? obj_w : -obj_w;
            obj_y -= 50;
            obj_h += 50;
            break;
        case TYPE_MST_SCROLL:
            if (obj->hit_points != 0)
            {
                obj_y = 0;
                obj_h = mp.height << 4;
            }
            break;
        case TYPE_SCROLL_SAX:
        case TYPE_BB1_VIT:
            obj_x = obj->x + obj->offset_bx;
            obj_w = 16;
            obj_y = 0;
            obj_h = mp.height << 4;
            break;
        case TYPE_WAT_CLOWN:
            obj_h += (obj->detect_zone >> 1);
            obj_y -= (obj->detect_zone >> 1);
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
        case TYPE_CHASSEUR2:
            obj_h = obj->detect_zone;
            obj_y -= (obj->detect_zone >> 1);
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_WIZARD1:
        case TYPE_CHASSEUR1:
        case TYPE_GENEBADGUY:
        case TYPE_CYMBALE:
        case TYPE_CYMBAL1:
        case TYPE_CYMBAL2:
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_CLOWN_TNT:
            if (!obj->flags.flip_x)
                obj_x += (obj_w >> 1) - (obj->detect_zone >> 1);
            else
                obj_x += (obj_w >> 1);

            obj_w = (obj->detect_zone >> 1);
            obj_y += (obj_h >> 1);
            obj_h = (obj_h >> 1) + obj->detect_zone;
            break;
        case TYPE_STONEWOMAN:
        case TYPE_STONEWOMAN2:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = (obj_h << 1);
            break;
        case TYPE_MITE:
            unk_1 = 150;
            if (!obj->flags.flip_x)
                obj_x -= obj->detect_zone;
            else
                obj_x = obj_x + -unk_1 + (obj_w >> 1);

            obj_w = obj->detect_zone + unk_1;
            obj_y -= ((obj->detect_zone - obj_h) >> 1);
            obj_h = obj->detect_zone;
            break;
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_P_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            break;
        case TYPE_PIRATE_POELLE_D:
        case TYPE_PIRATE_P_D_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            obj_x += 70;
            obj_y += 50;
            break;
        case TYPE_SPIDER_PLAFOND:
            if (
                    (
                            obj->main_etat == 0 &&
                            (obj->sub_etat == 24 || obj->sub_etat == 30 || obj->sub_etat == 11)
                    ) ||
                    (obj->main_etat == 1 && obj->sub_etat == 2)
                    )
            {
                obj_h = 10;
                obj_x -= 40;
                obj_w += 80;
            }
            else
            {
                obj_h = 200;
                obj_x -= 130;
                obj_w += 260;
            }
            break;
        case TYPE_DARK:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 250;
            break;
        case TYPE_JOE:
            obj_h += 20;
            break;
        case TYPE_PHOTOGRAPHE:
            break;
        default:
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
    }

    if (inter_box(obj_x, obj_y, obj_w, obj_h, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h))
    {
        det_zn_flg = obj->detect_zone_flag;
        if (det_zn_flg == 0 || det_zn_flg == 1)
        {
            if (det_zn_flg == 0)
                obj->detect_zone_flag = 1;
            else if (det_zn_flg == 1)
                obj->detect_zone_flag = 2;
        }
        else
        {
            obj->detect_zone_flag++;
            if (obj->detect_zone_flag == 20)
                obj->detect_zone_flag = 2;
        }
    }
    else
    {
        obj->detect_zone_flag = 0;
        if (obj->type == TYPE_PHOTOGRAPHE)
            obj->flags.flag_1 = 0;
    }
}

//2E738
void goToRay(obj_t* obj) {
    //stub
}

//2E998
void unleashMonsterHost(obj_t* obj) {
    //stub
}

//2EA2C
void SHOW_COLLISIONS_ZONES(void) {
    //stub
}

//2EAE8
void DO_COLLISIONS(void) {
    //stub
}

//2ED74
void DoRaymanCollisionDefault(obj_t* obj) {
    if ((flags[obj->type] & flags2_0x10_do_not_check_ray_collision) && get_eta(obj)->flags & 0x20) {
        if (!(ray.main_etat == 3 && ray.sub_etat == 32)) {
            i16 command_par3 = ray.iframes_timer;
            if (command_par3 >= 60 || command_par3 == -1) {
                RAY_HIT(0, obj);
                ray.iframes_timer = command_par3;
            } else {
                RAY_HIT(1, obj);
                if (!(ray_mode == 3 || ray_mode == 4)) {
                    ray.iframes_timer = 60;
                }
            }
        }
    }
}

//2EE20
void DO_OBJ_COLLISIONS(obj_t* obj, i16 a2) {
    //stub
}

//2F1E0
void test_allowed(obj_t* obj, i16 center_x, i16 center_y) {
    //stub
}

//2F224
void obj_jump(obj_t* obj) {
    //stub
}

//2F2FC
void DO_MOVING_PLATFORM_COMMAND(obj_t* obj) {
    //stub
}

//2F378
void DoPTGRAPPINPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//2F44C
void DO_ONE_CMD_WAIT(obj_t* obj) {
    if (obj->main_etat == 1) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 0);
    } else {
        if (!(obj->type == TYPE_10_FISH && obj->main_etat == 0 && obj->sub_etat == 0)) {
            obj->speed_y = 0;
        }
    }
}

//2F488
void DO_ONE_CMD_LR_ATTENTE(obj_t* obj) {
    //stub
}

//2F594
void DO_ONE_CMD_UPDOWN(obj_t* obj) {
    if (obj->type == TYPE_1_PLATFORM) {
        if (obj->cmd == GO_UP) {
            obj->speed_y = -2;
        } else if (obj->cmd == GO_DOWN) {
            obj->speed_y = 2;
        }
    } else if (obj->type == TYPE_10_FISH) {
        if (obj->main_etat == 0 && obj->sub_etat == 0) {
            if (obj->cmd == GO_UP) {
                obj->speed_y = -2;
            } else {
                obj->speed_y = 2;
            }
        }
    } else if (obj->type == TYPE_24_INTERACTPLT) {
        // This procedure is only called for commands 3 and 4, so, not sure why we are checking for command 2 here?
        if (obj->cmd != GO_WAIT) {
            --obj->nb_cmd;
            if (obj->nb_cmd <= 0) {
                if (obj->cmd == GO_UP) {
                    obj->speed_y = -1;
                } else if (obj->cmd == GO_DOWN) {
                    obj->speed_y = 1;
                }
            } else {
                obj->speed_y = 0;
                obj->nb_cmd = 0;
                obj->cmd = GO_WAIT;
                obj->init_y = obj->y;
            }

        }
    }
}

//2F63C
void special_pour_liv(obj_t* event) {
    if (event->type == TYPE_0_BADGUY1 && event->main_etat == 1 && event->sub_etat == 11) {
        event->flags.read_commands = false;
    }
}

//2F658
void DO_ONE_CMD(obj_t* obj) {
    special_pour_liv(obj);
    u8 cmd = obj->cmd;
    u8 etat = obj->main_etat;
    if (cmd == GO_LEFT || cmd == GO_RIGHT) {
        if (cmd == GO_LEFT) {
            obj->flags.flip_x = false;
        } else {
            obj->flags.flip_x = true;
        }
        if (etat == 1) {
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
        } else if (etat == 2) {
            SET_X_SPEED(obj);
        } else if (etat == 0) {
            DO_ONE_CMD_LR_ATTENTE(obj);
        }
    } else if (cmd == GO_WAIT) {
        DO_ONE_CMD_WAIT(obj);
    } else if (cmd == GO_UP || cmd == GO_DOWN) {
        DO_ONE_CMD_WAIT(obj);
    } else if (cmd == GO_SPEED) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->cmd_arg_2;
    }
}

//2F714
void DO_ROLL_EYES(obj_t* obj) {
    //stub
}

//2F978
void DoKillingEyesPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//2F9B8
void YaUnBloc(obj_t* obj) {
    //stub
}

//2FBF4
void DO_BAG1_COMMAND(obj_t* obj) {
    //stub
}

//2FC64
void DoRaymanBag1Collision(obj_t* obj) {
    //stub
}

//2FCB0
void MarCoince(obj_t* obj, i16 a2) {
    //stub
}

//2FFDC
void DO_MOVE_MARACAS_COMMAND(obj_t* obj) {
    //stub
}

//301A4
void DO_FLASH_COMMAND(obj_t* obj) {
    //stub
}

//30260
void DO_WLKNOT_COMMAND(obj_t* obj) {
    //stub
}

//30318
void ACTIVE_L_EAU(obj_t* obj) {
    //stub
}

//30490
void DO_EAU_QUI_MONTE(obj_t* obj) {
    //stub
}

//3076C
void allocateOtherPosts(obj_t* obj) {
    //stub
}

//3088C
void doHerseCommand(obj_t* obj) {
    //stub
}

//30908
void doBlackRaymanCommand(obj_t* obj) {
    //stub
}

//30BE4
void doBlKRayRaymanCollision(obj_t* obj) {
    //stub
}

//30BF8
void DO_POELLE_COMMAND(obj_t* obj) {
    //stub
}

//30EA8
void DO_CORDE_COMMAND(obj_t* obj) {
    //stub
}

//31060
void DO_PAC_COMMAND(obj_t* obj) {
    //stub
}

//310F8
void DO_CFUMEE_COMMAND(obj_t* obj) {
    //stub
}

//31190
void DO_NOVA2_COMMAND(obj_t* obj) {
    //stub
}

//31208
void doShipCommand(obj_t* obj) {
    //stub
}

//31400
void DO_PROP_COMMAND(obj_t* obj) {
    //stub
}

//314A4
void move_fruit_in_water(obj_t* obj) {
    //stub
}

//315F0
void DO_FALLING_OBJ_CMD(obj_t* obj) {
    //stub
}

//3167C
void DoFallingObjPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//3171C
void DO_BLKTOON_EYES_CMD(obj_t* obj) {
    //stub
}

//31768
void DO_RAY_POS_CMD(obj_t* obj) {
    if (obj->timer != 255) {
        if (obj->timer == 0) {
            if (obj->main_etat != 0) {
                if (obj->main_etat == 5) {
                    ray.is_active = 1;
                    ray.flags.alive = 1;
                    set_main_and_sub_etat(&ray, 3, 40);
                }
            } else {
                ray.is_active = 1;
                ray.flags.alive = 1;
            }
            obj->flags.alive = 0;
            del_alwobj(obj->id);
            dead_time = 64;
            ray.x = obj->x + obj->offset_bx - ray.offset_bx;
            ray.y = obj->y + obj->offset_by - ray.offset_by;
        }
        --obj->timer;
    }
}
