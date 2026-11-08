
//26A20
u8 get_casse_brique_ON(void) {
    return CasseBriqueON;
}

//26A28
void set_casse_brique_ON(u8 on) {
    CasseBriqueON = on;
}

//26A30
u8 get_casse_brique_active(void) {
    return casse_brique_active;
}

//26A38
void set_casse_brique_active(void) {
    casse_brique_active = 1;
}

//26A40
void enter_casse_brique(void) {
    if (casse_brique_active) {
        old_num_level_choice = num_level_choice;
        num_level_choice = 22;
        old_num_world_choice = num_world_choice;
        num_world_choice = 1;
        num_world = 1;
        CasseBriqueON = 1;
    }
}

//26A90
void enter_casse_brique2(void) {
    cbout = 0;
}

//26A9C
void exit_casse_brique(void) {
    num_level_choice = old_num_level_choice;
    num_world_choice = old_num_world_choice;
}

//26AB8
void exit_casse_brique2(void) {
    num_world = 7; // Force the saved world to reload on return.
    casse_brique_active = 0;
}

//26AD0
void get_speed_ball(void) {
    if (speed_ball_angle < 0) {
        speed_ball_angle += 72;
    } else if (speed_ball_angle > 72) {
        speed_ball_angle -= 72;
    }
    s16 angle = speed_ball_angle;
    s16 row;
    if (angle <= 18) {
        row = angle;
    } else if (angle <= 36) {
        row = 36 - angle;
    } else if (angle <= 54) {
        row = angle - 36;
    } else {
        row = 72 - angle;
    }
    speed_ball_x = CBdata[row][1 + 2 * speed_ball_level];
    speed_ball_y = CBdata[row][2 + 2 * speed_ball_level];
    if (angle > 18 && angle <= 54) {
        speed_ball_x = -speed_ball_x;
    }
    if (angle > 36) {
        speed_ball_y = -speed_ball_y;
    }
}

//26C00
void speed_ball_null(obj_t* obj) {
    obj->speed_x = obj->speed_y = 0;
    speed_ball_angle = speed_ball_level = 0;
    speed_ball_x = speed_ball_y = 0;
}

//26C30
void speed_ball_init(obj_t* obj) {
    obj->speed_x = obj->speed_y = 0;
    speed_ball_angle = 18;
    speed_ball_level = 0;
    get_speed_ball();
}

//26C5C
void speed_up_ball(obj_t* obj) {
    obj->speed_x = obj->speed_y = 0;
    speed_ball_level = MIN(speed_ball_level + 1, 7);
    get_speed_ball();
}

//26C8C
void invert_speed_ball_x(obj_t* obj) {
    obj->speed_x = obj->speed_y = 0;
    speed_ball_angle = 36 - speed_ball_angle;
    get_speed_ball();
}

//26CB8
void invert_speed_ball_y(obj_t* obj) {
    obj->speed_x = obj->speed_y = 0;
    speed_ball_angle = 72 - speed_ball_angle;
    get_speed_ball();
}

//26CE4
void set_ball_pos(obj_t* obj, s16 x, s16 y) {
    obj->x = x;
    obj->y = y;
    ball_x = x * 256;
    ball_y = y * 256;
}

//26D0C
s16 angle_direction(s16 dx, s16 dy) {
    s16 angle;
    if (dx != 0) {
        s16 slope = (s16)((dy * 512) / dx);
        if (slope < 0) slope = (s16)-slope;
        angle = 17;
        // Uncalled on PC. Preserve its zero-extension of the signed tangent
        // word before negation, even though that makes ordinary slopes pick 17.
        // Android sign-extends that word instead; its result is not used here.
        while (slope < -(s32)(u16)CBdata[angle][0] && angle > 0) --angle;
    } else {
        angle = dy != 0 ? 18 : 0;
    }
    if (dx < 0) angle = 36 - angle;
    if (dy > 0) angle = 72 - angle;
    return angle;
}

//26D88
void move_ball(obj_t* obj) {
    s16 jitter_x = 0, jitter_y = 0;
    if (undestroy > 5 || upscreen) {
        if (undestroy > 5) undestroy = 0;
        jitter_x = myRand(257) - 128;
        jitter_y = myRand(257) - 128;
        upscreen = 0;
    }
    ball_x += speed_ball_x + jitter_x;
    ball_y += speed_ball_y + jitter_y;
    // PC uses arithmetic shifts and truncates the resulting coordinates to words.
    obj->x = (s16)(ball_x >> 8);
    obj->y = (s16)(ball_y >> 8);
}

static obj_t* cb_brick_obj(u32 index) {
    if (index >= COUNT(CasseBrique)) {
        return NULL;
    }
    s16 id = CasseBrique[index].obj_id;
    return id >= 0 && id < level.nb_objects ? &level.objects[id] : NULL;
}

//26E4C
void get_brik_bonus(void) {
    u8 start = myRand(19);
    for (u8 i = 0; i < 20; ++i) {
        u8 index = (start + i) % 20;
        obj_t* brick = cb_brick_obj(index);
        if (brick && index != BonusBrik && brick->param >= 1 && brick->param <= 3) {
            brick->hit_points = 10;
            BonusBrik = index;
            BonusMode = 2;
            BonusBrikType = brick->param;
            brick->param = 6;
            return;
        }
    }
}

//26ED0
void do_bonus(void) {
    if (NbBrikTouched >= 5) {
        if (BonusMode == 1) {
            get_brik_bonus();
        } else if (BonusMode != 2) {
            BonusMode = 1;
        }
    }
}

//26F2C
bool get_brik_info(obj_t* obj, s16 index, s16* x, s16* y, u8* type) {
    anim_t* layout = &obj->animations[2];
    s32 layer_count = layout->layers_per_frame & 0x3FFF;
    if (NoCB < 0 || NoCB >= layout->frames_count || index < 0 || index >= layer_count) {
        return false;
    }
    anim_layer_t* layer = &layout->layers[NoCB * layer_count + index];
    sprite_t* sprite = &obj->sprites[layer->sprite_index];
    if (layer->sprite_index == 0 || sprite->id == 0) {
        return false;
    }
    *x = layer->x;
    *y = layer->y;
    switch (sprite->id) {
        case 39: *type = 1; break;
        case 40: *type = 2; break;
        case 41: *type = 3; break;
        case 42: *type = 5; break;
        case 43: *type = 4; break;
        case 50: *type = 7; break;
        default: *type = 0; break;
    }
    return true;
}

//2701C
void frame_briques(obj_t* obj) {
    switch (obj->param) {
        case 0: obj->is_active = 0; obj->flags.alive = 0; break;
        case 1: obj->anim_frame = 1; break;
        case 2: obj->anim_frame = 0; break;
        case 3: obj->anim_frame = 2; break;
        case 4: obj->anim_frame = obj->hit_points <= 5 ? 7 : 3; break;
        case 5: obj->anim_frame = 4; break;
        case 6:
            if (obj->anim_frame == obj->animations[1].frames_count - 1) {
                obj->anim_frame = 0;
                if (--obj->hit_points == 0) {
                    obj->param = BonusBrikType;
                    get_brik_bonus();
                }
            } else if (horloge[4] == 1) {
                ++obj->anim_frame;
            }
            break;
        case 7: obj->anim_frame = 5; break;
    }
    if (obj->param != 0 && (u16)obj->iframes_timer < COUNT(CasseBrique)) {
        cb_brick_t* brick = &CasseBrique[obj->iframes_timer];
        obj->x = xmap + CBoffsetX + brick->x - obj->offset_bx;
        obj->y = ymap + CBoffsetY + brick->y - obj->offset_by;
    }
}

//27140
void AllocateCasseBriqueBoum(obj_t* obj) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* effect = &level.objects[i];
        if (effect->type == TYPE_11_BOUM && !effect->is_active) {
            s16 x, y, w, h;
            GET_ANIM_POS(obj, &x, &y, &w, &h);
            effect->anim_frame = 0;
            effect->x = x + (w >> 1) - effect->offset_bx;
            effect->y = y + h - effect->offset_by;
            effect->is_active = 1;
            effect->flags.alive = 1;
            add_alwobj(effect);
            break;
        }
    }
}

//271F8
void AllocateBonus(obj_t* obj, s16 type, u8 main_etat, u8 sub_etat) {
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* bonus = &level.objects[i];
        if (bonus->type == type && !bonus->is_active) {
            bonus->x = obj->x + obj->offset_bx - bonus->offset_bx;
            bonus->y = obj->y + obj->offset_by - bonus->offset_by;
            set_main_and_sub_etat(bonus, main_etat, sub_etat);
            bonus->is_active = 1;
            bonus->flags.alive = 1;
            bonus->speed_x = 0;
            // The PC checks offset_bx even on this vertical comparison.
            bonus->speed_y = bonus->y + bonus->offset_bx - ymap < 100 ? 2 : 1;
            bonus->display_prio = 1;
            bonus->active_timer = 0;
            bonus->active_flag = 0;
            break;
        }
    }
}

//272D8
void bonus_fall(obj_t* obj) {
    BonusMode = 1;
    if (status_bar.max_hitp != 4) {
        AllocateBonus(obj, TYPE_82_JAUGEUP, 6, 2);
    } else if (poing_obj->init_sub_etat == 1 || poing_obj->init_sub_etat == 3 || poing_obj->init_sub_etat == 5) {
        AllocateBonus(obj, TYPE_95_POING_POWERUP, 5, 7);
    } else if (poing_obj->init_sub_etat != 12) {
        AllocateBonus(obj, TYPE_95_POING_POWERUP, 5, 14);
    } else {
        AllocateBonus(obj, TYPE_161_WIZ, 5, myRand(7) + 24);
    }
}

//2737C
u8 collision_briques(obj_t* obj) {
    s16 ball_cx = obj->x - xmap + obj->offset_bx + 8;
    s16 ball_cy = obj->y - ymap + obj->offset_by;
    nb_collis = 0;
    for (u8 i = 0; i < 20; ++i) {
        obj_t* brick = cb_brick_obj(i);
        if (!brick || brick->param == 0 || i == LastTouched) {
            continue;
        }
        s16 dx = ball_cx - (s16)(brick->x - xmap + brick->offset_bx + 8);
        s16 dy = ball_cy - (s16)(brick->y - ymap + brick->offset_by);
        s16 ax = Abs(dx), ay = Abs(dy);
        if (ax > 21 || dx * dx + dy * dy > dist2) {
            continue;
        }
        LastTouched = i;
        switch (brick->param) {
            case 1: case 2:
                ++brick->param;
                undestroy = 0;
                ++NbBrikTouched;
                no_sound = 193;
                break;
            case 3: case 6:
                DO_NOVA(brick);
                ++NbBrikTouched;
                --NbClassicBrik;
                if (brick->param == 6) {
                    bonus_fall(brick);
                }
                brick->param = 0;
                undestroy = 0;
                no_sound = 14;
                break;
            case 4:
                no_sound = 214;
                ++undestroy;
                if (--brick->hit_points == 0) {
                    DO_NOVA(brick);
                    brick->param = 0;
                    undestroy = 0;
                    no_sound = 14;
                }
                break;
            case 5:
                speed_up_ball(obj);
                ++undestroy;
                no_sound = 54;
                break;
            case 7:
                tempo = 1;
                CBoffsetY -= 5;
                ++undestroy;
                no_sound = 54;
                break;
        }
        cb_collision_t* collision = &CBCollis[0];
        collision->dx = dx;
        collision->dy = dy;
        collision->brick_index = i;
        collision->flip_x = collision->flip_y = 0;
        // Preserve the PC's two independent comparisons at a 45-degree contact.
        if (ay <= ax) {
            if (SGN(speed_ball_x) == SGN(dx)) collision->flip_y = 1;
            else collision->flip_x = 1;
        }
        if (ax <= ay) {
            if (SGN(speed_ball_y) == SGN(dy)) collision->flip_x = 1;
            else collision->flip_y = 1;
        }
        nb_collis = 1;
        nb_max_collis = MAX(nb_max_collis, nb_collis);
        if (collision->flip_x) invert_speed_ball_x(obj);
        if (collision->flip_y) invert_speed_ball_y(obj);
        PlaySnd_old(no_sound);
        break; // PC deliberately handles only the first brick hit this update.
    }
    return nb_collis;
}

//27798
void collision_rayman(obj_t* obj) {
    calc_obj_pos(obj);
    s16 x = obj->screen_x + obj->offset_bx - 8;
    s16 y = obj->screen_y + obj->offset_by - 8;
    s16 width = 46;
    calc_obj_pos(&ray);
    s16 paddle_x = ray.screen_x + ray.offset_bx;
    s16 paddle_y = ray.screen_y;
    u8 touched = 0;
    if ((ray.main_etat == 0 && ray.sub_etat == 4) || (ray.main_etat == 1 && ray.sub_etat == 1)) {
        paddle_x -= 22;
        paddle_y += 32;
        if (LastTouched != 25) {
            touched = 25;
        }
    } else if ((ray.main_etat == 0 && ray.sub_etat == 20) ||
               (ray.main_etat == 3 && (ray.sub_etat == 17 || ray.sub_etat == 18))) {
        width += 32;
        paddle_x -= 38;
        paddle_y += 74;
        if (LastTouched != 26) touched = 26;
    }
    if (touched && inter_box(x, y, 16, 16, paddle_x, paddle_y, width, 2) && speed_ball_y >= 0) {
        speed_ball_level = NbClassicBrik <= 5 && NoCB != 3 ? 1 : 0;
        s32 direction = -120 * (x + 16 - paddle_x) / (width + 15);
        speed_ball_angle = (s16)((150.0 + direction) * 0.2);
        get_speed_ball();
        AllocateCasseBriqueBoum(obj);
        PlaySnd_old(44);
        if (touched == 25) {
            set_main_and_sub_etat(&ray, 3, 50);
        }
        LastTouched = touched;
        undestroy = 0;
    }
}

//279BC
void move_briques(obj_t* obj) {
    (void)obj;
    // Called once per active brick, not once per game frame: retain PC timing.
    if (InvadersMode == 1 && --tempo == 0) {
        CBoffsetY += Ydir;
        tempo = 500;
        if (CBoffsetY >= CBoffsetYMax) {
            CBoffsetY = CBoffsetYMax;
        } else {
            if (CBoffsetY <= CBoffsetYMin) CBoffsetY = CBoffsetYMin;
            Ydir = 1;
        }
        Xdir = 0;
    }
}

//27A68
void init_briques(obj_t* obj) {
    s16 max_x = -SCREEN_WIDTH, max_y = -SCREEN_HEIGHT;
    s16 min_x = SCREEN_WIDTH, min_y = SCREEN_HEIGHT;
    InvadersMode = BonusMode = 0;
    BonusBrik = 20;
    memset(CasseBrique, 0, sizeof(CasseBrique));
    for (s32 i = 0; i < 20; ++i) {
        CasseBrique[i].obj_id = -1;
    }
    obj->iframes_timer = 100;
    s16 count = 0;
    for (s16 i = 0; i < level.nb_objects && count < 20; ++i) {
        if (level.objects[i].type != TYPE_256_BREAKOUTBRICK_20) {
            continue;
        }
        s16 x, y;
        u8 type;
        if (!get_brik_info(obj, count, &x, &y, &type)) {
            x = 128;
            y = 0;
            type = 0;
        }
        cb_brick_t* brick = &CasseBrique[count++];
        brick->x = x + 42;
        brick->y = y + 9;
        brick->type = type;
        brick->obj_id = i;
        // Keep the original else-if extrema updates, including empty layout slots.
        if (brick->x > max_x) {
            max_x = brick->x;
        } else if (brick->x < min_x) {
            min_x = brick->x;
        }
        if (brick->y > max_y) {
            max_y = brick->y;
        } else if (brick->y < min_y) {
            min_y = brick->y;
        }
    }
    CBoffsetXMin = MIN(0, 12 - min_x);
    CBoffsetXMax = MAX(0, SCREEN_WIDTH - 28 - max_x);
    CBoffsetYMin = MIN(0, 8 - min_y);
    CBoffsetYMax = MAX(0, 120 - max_y);
    CBoffsetY = 0;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    if (GameModeVideo == MODE_X) {
        CBoffsetX -= 8;
        if (P486 == 1) {
            CBoffsetY = CasseBriqueOffsetX[NoCB];
        }
    }
    NbClassicBrik = 0;
    for (s16 i = 0; i < 20; ++i) {
        obj_t* brick = cb_brick_obj(i);
        if (!brick) {
            continue;
        }
        brick->x = xmap + CasseBrique[i].x - brick->offset_bx + CBoffsetX;
        brick->y = ymap + CasseBrique[i].y - brick->offset_by + CBoffsetY;
        brick->is_active = 1;
        brick->flags.alive = 1;
        brick->param = CasseBrique[i].type;
        brick->iframes_timer = i;
        add_alwobj(brick);
        brick->display_prio = 4;
        brick->speed_x = brick->speed_y = 0;
        if (brick->param >= 1 && brick->param <= 3) {
            ++NbClassicBrik;
        } else if (brick->param == 7) {
            InvadersMode = 2;
        } else if (brick->param == 4) {
            brick->hit_points = 20;
        }
        frame_briques(brick);
    }
    NbBrikTouched = upscreen = 0;
}

//27DC8
void reset_briques(void) {
    for (s32 i = 0; i < 20; ++i) {
        obj_t* brick = cb_brick_obj(i);
        if (brick) {
            brick->flags.alive = 0;
            brick->is_active = 0;
        }
    }
}

//27E14
void casse_brique_init(obj_t* obj) {
    ray_mode = MODE_5_CASSE_BRIQUE;
    exit_casse_brique();
    enter_casse_brique2();
    xmap = GameModeVideo == MODE_NORMAL ? 0 : -5;
    scroll_end_x = scroll_start_x = xmap;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    CBoffsetY = YOFFSETCB = 0;
    XOFFSETRAY = 56;
    LIMITE_GAUCHE = 56 - ray.offset_bx - 8;
    LIMITE_DROITE = SCREEN_WIDTH - 56 - ray.offset_bx - 8;
    ymap = scroll_end_y = scroll_start_y = 24;
    XOFFSETCBLEFT = XOFFSETCBRIGHT = 32;
    if (GameModeVideo == MODE_NORMAL) {
        xmap += 8;
    } else {
        CBoffsetX -= 8;
        if (P486 == 1) {
            CBoffsetY = CasseBriqueOffsetY[NoCB];
            YOFFSETCB = 10;
        }
        XOFFSETRAY = -8;
        XOFFSETCBLEFT -= 8;
        XOFFSETCBRIGHT += 8;
        LIMITE_GAUCHE -= 8;
        LIMITE_DROITE -= 8;
    }
    obj->display_prio = 0;
    speed_ball_null(obj);
    ray.y = ymap + SCREEN_HEIGHT - 8 - ray.offset_by;
    if (map_time == 2) {
        NoCB = NoCBInit = myRand(obj->animations[2].frames_count - 1);
        obj->hit_points = 3;
        init_briques(obj);
        set_main_and_sub_etat(&ray, 3, 1);
    } else if (map_time == 50) {
        set_main_and_sub_etat(obj, 0, 1);
    }
}

//28004
void casse_brique_start(obj_t* obj) {
    Xdir = Ydir = 0;
    if (!((ray.main_etat == 0 && ray.sub_etat == 4) || (ray.main_etat == 1 && ray.sub_etat == 1))) {
        return;
    }
    speed_ball_null(obj);
    s16 x, y, w, h;
    GET_SPRITE_POS(&ray, 1, &x, &y, &w, &h);
    x += (ray.flags.flip_x ? 0 : w - 2) - obj->offset_by;
    y -= obj->offset_by + 5;
    set_ball_pos(obj, x, y);
    if (obj->display_prio == 0) {
        DO_NOVA(obj);
        PlaySnd_old(194);
    }
    obj->display_prio = 2;
    obj->is_active = 1;
    obj->flags.alive = 1;
    if (but0pressed()) {
        speed_ball_init(obj);
        set_main_and_sub_etat(obj, 0, 2);
        set_main_and_sub_etat(&ray, 3, 50);
        if (InvadersMode == 2) {
            InvadersMode = 1;
            Xdir = 0;
            Ydir = 1;
        } else {
            InvadersMode = 0;
        }
        LastTouched = 20;
    }
}

//28170
bool ray_face_brik(s16 x) {
    s16 dx = x - (ray.screen_x + ray.offset_bx);
    return ray.flags.flip_x ? dx >= 0 : dx <= 0;
}

//281AC
void casse_brique_play(obj_t* obj) {
    if (NbClassicBrik == 0) {
        obj->display_prio = 0;
        --obj->iframes_timer;
        speed_ball_null(obj);
        if (obj->iframes_timer == 0) {
            reset_briques();
            if (++NoCB >= obj->animations[2].frames_count) {
                NoCB = 0;
            }
            if (NoCB == NoCBInit) {
                obj->iframes_timer = 100;
                set_main_and_sub_etat(obj, 0, 4);
                PlaySnd_old(107);
            } else {
                init_briques(obj);
                set_main_and_sub_etat(obj, 0, 1);
                PlaySnd_old(105);
            }
        }
        return;
    }
    s16 x = obj->x - xmap + obj->offset_bx + 8;
    s16 y = obj->y - ymap + obj->offset_by;
    if (x + 8 >= SCREEN_WIDTH - XOFFSETCBRIGHT) {
        if (LastTouched != 22) {
            invert_speed_ball_x(obj);
            LastTouched = 22;
            undestroy = 0;
            PlaySnd_old(45);
        }
    } else if (x - 8 <= XOFFSETCBLEFT) {
        if (LastTouched != 21) {
            invert_speed_ball_x(obj);
            LastTouched = 21;
            undestroy = 0;
            PlaySnd_old(45);
        }
    } else if (y - 8 <= YOFFSETCB) {
        if (LastTouched != 23) {
            invert_speed_ball_y(obj);
            LastTouched = 23;
            if (undestroy) {
                ++undestroy;
            }
            PlaySnd_old(45);
            upscreen = 1;
        }
    } else if (y + 8 > ray.screen_y + 64) {
        if (ray.speed_x != 0 && ray_face_brik(x) &&
            ((ray.main_etat == 0 && ray.sub_etat == 4) || (ray.main_etat == 1 && ray.sub_etat == 1))) {
            set_main_and_sub_etat(&ray, 3, 17);
        } else if (y >= SCREEN_HEIGHT - YOFFSETCB) {
            set_main_and_sub_etat(obj, 0, 3);
            obj->display_prio = 0;
            speed_ball_null(obj);
            if (InvadersMode == 1) {
                InvadersMode = 2;
            }
        } else collision_rayman(obj);
    } else if (!collision_briques(obj)) {
        collision_rayman(obj);
    }
    move_ball(obj);
    do_bonus();
}

//28484
void casse_brique_end(obj_t* obj) {
    if (BonusMode == 2) {
        obj_t* brick = cb_brick_obj(BonusBrik);
        if (brick) {
            brick->param = BonusBrikType;
        }
    }
    BonusMode = 0;
    if (--obj->hit_points == 0) {
        obj->iframes_timer = 100;
        set_main_and_sub_etat(obj, 0, 4);
        PlaySnd_old(22);
    } else {
        set_main_and_sub_etat(obj, 0, 1);
        PlaySnd_old(15);
    }
}

//28504
void casse_brique_done(obj_t* obj) {
    obj->display_prio = 0;
    speed_ball_null(obj);
    Xdir = Ydir = 0;
    if (--obj->iframes_timer == 0) {
        if (obj->hit_points != 0) {
            Add_One_RAY_lives();
        }
        ray_mode = MODE_1_RAYMAN;
        reset_briques();
        obj->is_active = 0;
        obj->flags.alive = 0;
        set_main_and_sub_etat(&ray, 3, 2);
        new_world = 1;
        exit_casse_brique();
        exit_casse_brique2();
        DO_FADE_OUT();
    }
}

//28590
void DISPLAY_FIXE_CB(obj_t* obj) {
    // Keep PC sprite counters/text; Android additionally drives its Lua HUD.
    s16 width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    if (GameModeVideo == MODE_X && P486 == 1) {
        width -= 3;
        height -= 8;
    }
    for (u8 i = 1; i < obj->hit_points; ++i) {
        display_sprite(obj, 11, width - 28, height - 20 * i, 1);
    }
    if (NbClassicBrik <= 20) {
        s16 x = 16, y = SCREEN_HEIGHT - 25;
        if (GameModeVideo == MODE_X && P486 == 1) {
            x = 12;
            y -= 6;
        }
        obj_t* status = &level.objects[sbar_obj_id];
        display_sprite(status, 57, x - 2, y, 1);
        display_sprite(status, 28 + NbClassicBrik / 10, x + 39, y, 1);
        display_sprite(status, 28 + NbClassicBrik % 10, x + 54, y, 1);
    }
    if (obj->main_etat == 0) {
        if (obj->sub_etat == 1 && (map_time & 63) < 32) {
            char message[256];
            snprintf(message, sizeof(message), "/%s/", language_txt[188]);
            display_text(message, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0);
            display_text(language_txt[180], SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 2, 0);
        } else if (obj->sub_etat == 4) {
            display_text(language_txt[cbout || obj->hit_points == 0 ? 128 : 157],
                         SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0);
        }
    }
}

//287D4
void DO_CB_BALL_COMMAND(obj_t* obj) {
    if (TOUCHE(SC_ESCAPE) && !cbout) {
        obj->hit_points = 0;
        cbout = 1;
        obj->iframes_timer = 100;
        set_main_and_sub_etat(obj, 0, 4);
        PlaySnd_old(22);
    }
    if (obj->main_etat == 0) {
        switch (obj->sub_etat) {
            case 0: casse_brique_init(obj); break;
            case 1: casse_brique_start(obj); break;
            case 2: casse_brique_play(obj); break;
            case 3: casse_brique_end(obj); break;
            case 4: casse_brique_done(obj); break;
        }
    }
}

//28874
void DO_CB_BRIK_COMMAND(obj_t* obj) {
    move_briques(obj);
    frame_briques(obj);
}

//28888
void DO_RAY_CASSE_BRIQUE(void) {
    if ((ray.main_etat == 0 && ray.sub_etat == 4) || (ray.main_etat == 1 && ray.sub_etat == 1)) {
        if (rightjoy()) {
            if (!ray.flags.flip_x) {
                ray.flags.flip_x = 1;
                ray.speed_x = -ray.speed_x;
            }
            if (ray.main_etat == 0 && ray.sub_etat == 4) {
                set_main_and_sub_etat(&ray, 1, 1);
                ray.speed_x = 16;
            } else {
                ray.speed_x = MIN(47, ray.speed_x + 1);
            }
        } else if (leftjoy()) {
            if (ray.flags.flip_x) {
                ray.flags.flip_x = 0;
                ray.speed_x = -ray.speed_x;
            }
            if (ray.main_etat == 0 && ray.sub_etat == 4) {
                set_main_and_sub_etat(&ray, 1, 1);
                ray.speed_x = -16;
            } else {
                ray.speed_x = MAX(-47, ray.speed_x - 1);
            }
        } else {
            if (ray.speed_x > 0) {
                --ray.speed_x;
            } else if (ray.speed_x < 0) {
                ++ray.speed_x;
            }
            if (Abs(ray.speed_x) < 16) {
                set_main_and_sub_etat(&ray, 0, 4);
            }
        }
    } else if ((ray.main_etat == 3 && ray.sub_etat == 18) || (ray.main_etat == 0 && ray.sub_etat == 20)) {
        SET_X_SPEED(&ray);
        if (but0pressed()) {
            set_main_and_sub_etat(&ray, 0, 4);
        }
    } else if (ray.main_etat == 3 && ray.sub_etat == 17) {
        SET_X_SPEED(&ray);
    }
    if (ray.x <= LIMITE_GAUCHE && ray.speed_x < 0) {
        ray.x = LIMITE_GAUCHE;
        ray.speed_x = 0;
    } else if (ray.x >= LIMITE_DROITE && ray.speed_x > 0) {
        ray.x = LIMITE_DROITE;
        ray.speed_x = 0;
    } else {
        ray.x += instantSpeed(ray.speed_x);
    }
    GET_RAY_ZDC(&ray, &ray_zdc_x, &ray_zdc_y, &ray_zdc_w, &ray_zdc_h);
    DO_COLLISIONS();
    DO_ANIM(&ray);
}
