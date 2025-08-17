
//67100
void DO_POISSON_VERT_CMD(obj_t* obj) {
    //stub
}

//671F8
void DO_POISSON_BLEU_CMD(obj_t* obj) {
    //stub
}

//67380
void DoPoissonBleuPoingCollision(obj_t* obj, i16 sprite) {
    //stub
}

//673B4
bool can_free_fish(obj_t* fish) {
    bool result = true;
    for (i32 i = 0; i < level.nb_objects; ++i) {
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
    /* 37CB8 8015C4B8 -O2 -msoft-float */
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
    /* 37DA4 8015C5A4 -O2 -msoft-float */
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
void DoFishPoingCollision(obj_t* obj, i16 sprite) {
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
    //stub
}

