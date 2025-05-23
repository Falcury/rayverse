
//67100
void DO_POISSON_VERT_CMD(obj_t* obj) {
    //stub
}

//671F8
void DO_POISSON_BLEU_CMD(obj_t* obj) {
    //stub
}

//67380
void DoPoissonBleuPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//673B4
bool can_free_fish(obj_t* piranha) {
    // stub
    bool result = true;
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* event = level.objects + i;
        if (event->type == TYPE_10_FISH && event->init_x == piranha->init_x && !event->is_active && event->timer >= 100) {
            result = false;
            break;
        }
    }
    return result;
}


//67410
void DESACTIVE_FISH_COLLIS(obj_t* obj) {
    //stub
}

//674C8
void DO_PYRANHA(obj_t* obj) {
    DO_ONE_CMD(obj);
    bool respawn = false;
    if (obj->main_etat == 0) {
        i32 y_offset_by = obj->y + obj->offset_by;
        if (y_offset_by + 20 < ymap + 200) {
            ++obj->timer;
        }
        if (obj->sub_etat == 3) {
            obj->speed_y = 0;
        } else if (obj->sub_etat == 9) {
            obj->speed_y = 6;
            if ((obj->y + obj->offset_hy > ymap + 200) || (y_offset_by + 14 > mp.height * 16)) {
                if (y_offset_by + 14 > mp.height * 16) {
                    allocate_splash(obj);
                }
                obj->y = ymap + 200;
                obj->is_active = 0;
                obj->flags.alive = false;
                obj->timer = 0;
                respawn = can_free_fish(obj);
            }
        } else {
            if (y_offset_by < ymap) { //?
                obj->y = ymap + 200;
                obj->is_active = 0;
                obj->flags.alive = false;
                respawn = can_free_fish(obj);
            }
        }

        if (obj->timer == 100) {
            respawn = can_free_fish(obj);
        }
        if (respawn) {
            // stub
        }
    }
}

//676EC
void DoFishPoingCollision(obj_t* obj, i16 a2) {
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

