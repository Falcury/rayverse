
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
    //stub
}

//2DBAC
void RAY_KO(obj_t* obj) {
    //stub
}

//2DBDC
void RAY_HIT(bool put_above_solid_tiles, obj_t* other_obj) {
    if (put_above_solid_tiles) {
        RAY_HURT();
        i32 tile_x = (ray.x + ray.offset_bx) / 16;
        for (;;) {
            i32 tile_y = (ray.y + ray.offset_by) / 16;
            if (BTYP(tile_x, tile_y) != 10) {
                --ray.y;
            } else {
                break;
            }
        }
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
            if (ray.main_etat == 0 && ray.sub_etat == 61) {
                ray.speed_x = ray.flags.flip_x ? -2 : 2;
                ray.speed_y = -3;
            } else {
                if (other_obj != NULL) {
                    i32 bump_direction = -1;
                    if (other_obj->type == TYPE_180_SAXO2) {
                        //bump_direction = sub_778CC();
                    } else if (other_obj->type == TYPE_150_SCORPION) {
                        //bump_direction = sub_79688();
                    } else if (other_obj->type == TYPE_198_BB12) {
                        bump_direction = 1;
                    } else if (other_obj->type == TYPE_200_BB13) {
                        bump_direction = -1;
                    } else if (other_obj->type == TYPE_100_MITE) {
                        bump_direction = other_obj->flags.flip_x ? 1 : -1;
                    } else if (other_obj->type == TYPE_120_BATTEUR_FOU) {
                        // bump_direction = sub_2008C(other_event);
                    } else if (other_obj->type == TYPE_187_MAMA_PIRATE) {
                        // bump_direction = sub_6628C(other_event);
                    } else if (other_obj->type == TYPE_209_FIRE_LEFT) {
                        bump_direction = 1;
                    } else if (other_obj->type == TYPE_210_FIRE_RIGHT) {
                        bump_direction = -1;
                    } else {
                        i32 xspeed_delta = other_obj->speed_x - ray.speed_x;
                        if (xspeed_delta == 0) {
                            bump_direction = ray.flags.flip_x ? 1 : -1;
                        } else {
                            bump_direction = xspeed_delta > 0 ? 1 : -1;
                        }
                    }
                    i32 bump_speed;
                    if (flags[other_obj->type] & flags1_4_fast_bump) {
                        bump_speed = 5;
                    } else if (other_obj->type == TYPE_180_SAXO2) {
                        bump_speed = 4;
                    } else {
                        bump_speed = 2;
                    }
                    ray.speed_x = bump_speed * bump_direction; // Note: can it be a different value except 1 or -1?
                    ray.speed_y = -bump_speed;

                }
            }

            // stub

        }
    }
}

//2DF34
void standard_frontZone(obj_t* obj, i16* a2, i16* a3) {
    //stub
}

//2DF88
void SET_DETECT_ZONE_FLAG(obj_t* obj) {
    //stub
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
    //stub
}
