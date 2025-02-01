
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
        if (event->type == TYPE_10_FISH && event->spawn_x == piranha->spawn_x && !event->is_active && event->timer >= 100) {
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
        i32 y_offset_by = obj->ypos + obj->offset_by;
        if (y_offset_by + 20 < ymap + 200) {
            ++obj->timer;
        }
        if (obj->sub_etat == 3) {
            obj->yspeed = 0;
        } else if (obj->sub_etat == 9) {
            obj->yspeed = 6;
            if ((obj->ypos + obj->offset_hy > ymap + 200) || (y_offset_by + 14 > mp.height * 16)) {
                if (y_offset_by + 14 > mp.height * 16) {
                    //allocate_splash(obj); // TODO: fix this
                }
                obj->ypos = ymap + 200;
                obj->is_active = 0;
                obj->flags &= ~obj_flags_4_triggered;
                obj->timer = 0;
                respawn = can_free_fish(obj);
            }
        } else {
            if (y_offset_by < ymap) { //?
                obj->ypos = ymap + 200;
                obj->is_active = 0;
                obj->flags &= ~obj_flags_4_triggered;
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
    if (obj->hitp != 0) {
        obj->xspeed = 0;
        obj->yspeed = 0;
        set_main_and_sub_etat(obj, 0, 1);
    } else {
        set_main_and_sub_etat(obj, 0, 3);
    }
    DESACTIVE_FISH_COLLIS(obj);
    obj->command = obj_flipped(obj) ? cmd_1_right : cmd_0_left;
}

//6773C
void DoPoissonBleuRaymanZDD(obj_t* obj) {
    //stub
}

