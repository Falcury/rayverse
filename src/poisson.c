
//67100
void DO_POISSON_VERT_CMD(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    DO_ONE_CMD(obj);
    if (
        (
            !(obj->flags.flip_x) &&
            on_block_chdir(obj, obj->offset_bx - 20, obj->offset_by - 3) &&
            (s16) test_allowed(obj, obj->offset_bx - 20, obj->offset_by - 3)
        ) ||
        (
            obj->flags.flip_x &&
            on_block_chdir(obj, obj->offset_bx + 20, obj->offset_by - 3) &&
            (s16) test_allowed(obj, obj->offset_bx + 20, obj->offset_by - 3)
        )
    ) {
        if (obj->flags.flip_x) {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 7, true);
        } else {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 8, true);
        }
    }
    DO_OBJ_COLLISIONS(obj, 4);
}

//671F8
void DO_POISSON_BLEU_CMD(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    DO_ONE_CMD(obj);
    if (
        (
            !(obj->flags.flip_x) &&
            on_block_chdir(obj, obj->offset_bx - 20, obj->offset_by - 3) &&
            (s16) test_allowed(obj, obj->offset_bx - 20, obj->offset_by - 3)
        ) ||
        (
            obj->flags.flip_x &&
            on_block_chdir(obj, obj->offset_bx + 20, obj->offset_by - 3) &&
            (s16) test_allowed(obj, obj->offset_bx + 20, obj->offset_by - 3)
        )
    ) {
        if (obj->flags.flip_x) {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 13, true);
        } else {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 14, true);
        }
        obj->y = obj->init_y;
    }

    if (obj->main_etat == 2 && obj->sub_etat == 17) {
        if (obj->init_y <= obj->y) {
            if ((obj->flags.flip_x))
                skipToLabel(obj, 10, true);
            else
                skipToLabel(obj, 9, true);
            obj->y = obj->init_y;
        }
    }
    if (obj->main_etat == 2 && obj->sub_etat == 18) {
        if (EOA(obj)) {
            if (obj->speed_y == 5) {
                obj->display_prio = 0;
                obj->is_active = 0;
            } else {
                obj->speed_x = 0;
                obj->speed_y = 5;
                skipToLabel(obj, 12, true);
            }
        }
    }
}

//67380
void DoPoissonBleuPoingCollision(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    if (!(obj->main_etat == 2 && obj->sub_etat == 18)) {
        skipToLabel(obj, 12, true);
        obj->speed_x = 0;
        obj->speed_y = 0;
        obj->hit_points = 0;
    }
}

//673B4
bool can_free_fish(obj_t* fish) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    bool result = true;
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_10_FISH && cur_obj->init_x == fish->init_x && cur_obj->is_active && cur_obj->timer < 100) {
            result = false;
            break;
        }
    }
    return result;
}


//67410
void DESACTIVE_FISH_COLLIS(obj_t* fish) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    u8 index = link_init[fish->id];
    obj_t* unk_obj = &level.objects[index];

    if (unk_obj->y == fish->y) {
        unk_obj->y = ymap + SCREEN_HEIGHT;
        unk_obj->flags.alive = 0;
        unk_obj->is_active = 0;
    }

    index = link_init[unk_obj->id];
    unk_obj = &level.objects[index];
    if (unk_obj->y == fish->y) {
        unk_obj->is_active = 0;
        unk_obj->y = ymap + SCREEN_HEIGHT;
        unk_obj->flags.alive = 0;
    }
}

//674C8
void DO_PYRANHA(obj_t* in_obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    u8 can_free;
    s32 in_y_pos;
    s32 in_y;
    obj_t* cur_obj;
    s16 i;
    u8 nb_obj;

    DO_ONE_CMD(in_obj);
    can_free = false;
    if (in_obj->main_etat == 0) {
        if (in_obj->y + in_obj->offset_by + 20 < ymap + SCREEN_HEIGHT)
            in_obj->timer++;
        if (in_obj->sub_etat == 3)
            in_obj->speed_y = 0;
        else if (in_obj->sub_etat == 9) {
            in_y_pos = in_obj->y;
            in_y = in_obj->y + in_obj->offset_hy;
            in_obj->speed_y = 6;
            if (ymap + SCREEN_HEIGHT < in_y || in_y_pos + in_obj->offset_by + 14 > mp.height * 16) {
                if (in_y_pos + in_obj->offset_by + 14 > mp.height * 16)
                    allocate_splash(in_obj);
                in_obj->timer = 0;
                in_obj->is_active = 0;
                in_obj->y = ymap + SCREEN_HEIGHT;
                in_obj->flags.alive = 0;
                can_free = can_free_fish(in_obj);
            }
        } else if (in_obj->y + in_obj->offset_by < ymap) {
            in_obj->is_active = 0;
            in_obj->y = ymap + SCREEN_HEIGHT;
            in_obj->flags.alive = 0;
            can_free = can_free_fish(in_obj);
        }
    }
    if (in_obj->timer == 100)
        can_free = can_free_fish(in_obj);
    if (can_free) {
        cur_obj = level.objects;
        nb_obj = level.nb_objects;
        for (i = 0; nb_obj > i; i++) {
            if (cur_obj->type == TYPE_FISH && cur_obj->init_x == in_obj->init_x && !(cur_obj->is_active)) {
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                cur_obj->y = ymap + SCREEN_HEIGHT;
                cur_obj->x = in_obj->init_x;
                if (cur_obj->y + cur_obj->offset_by > ((mp.height - 1) << 4)) {
                    cur_obj->y = -cur_obj->offset_by + ((mp.height - 1) << 4);
                    allocate_splash(cur_obj);
                }
                cur_obj->main_etat = in_obj->init_main_etat;
                cur_obj->sub_etat = in_obj->init_sub_etat;
                cur_obj->hit_points = in_obj->init_hit_points;
                cur_obj->timer = 0;
                cur_obj->flags.read_commands = 1;
                calc_obj_pos(cur_obj);
                skipToLabel(cur_obj, 1, true);
                break;
            }
            cur_obj++;
        }
    }
}

//676EC
void DoFishPoingCollision(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    obj_hurt(obj);
    if (obj->hit_points != 0) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 1);
    } else {
        set_main_and_sub_etat(obj, 0, 3);
    }
    DESACTIVE_FISH_COLLIS(obj);
    obj->cmd = obj->flags.flip_x ? GO_RIGHT : GO_LEFT;
}

//6773C
void DoPoissonBleuRaymanZDD(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_RAY_IN_ZONE(). Seems to be functionally identical.
    u8 sub_etat;
    if (
        !(
            obj->main_etat == 2 &&
            (obj->sub_etat == 16 || (sub_etat = obj->sub_etat, sub_etat == 17) || sub_etat == 18)
        ) &&
        (
            ray.x + ray.offset_bx - obj->x - obj->offset_bx >= 0
            ? obj->flags.flip_x
            : !(obj->flags.flip_x)
        )
    ) {
        if (obj->flags.flip_x)
            obj->speed_x = 3;
        else
            obj->speed_x = -3;

        obj->speed_y = -5;
        set_main_etat(obj, 2);
        set_sub_etat(obj, 16);
        skipToLabel(obj, 11, true);
    }
}

