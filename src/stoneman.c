

//7CA90
void DO_STONEMAN_COMMAND(obj_t* obj) {
    //stub
}

//7CB48
void DO_STONEDOG_COMMAND(obj_t* obj) {
    /* 49090 8016D890 -O2 -msoft-float */
    if (obj->sub_etat == 3) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    switch (obj->cmd) {
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->cmd_arg_2;
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;

            SET_X_SPEED(obj);
            if (obj->main_etat == 0)
                set_main_and_sub_etat(obj, 1, 0);

            if (block_flags[calc_typ_travd(obj, false)] & 1) {
                if (obj->cmd == GO_LEFT)
                    obj->flags.flip_x = 1;
                else
                    obj->flags.flip_x = 0;

                skipToLabel(obj, 2, true);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
            }
            else if (
                    on_block_chdir(obj, obj->offset_bx, obj->offset_by - 16) &&
                    test_allowed(obj, obj->offset_bx, obj->offset_by - 16) &&
                    obj->type == TYPE_STONEDOG
                    )
            {
                if (obj->cmd == GO_LEFT)
                    obj->flags.flip_x = 1;
                else
                    obj->flags.flip_x = 0;

                if (obj->main_etat == 2)
                    skipToLabel(obj, 9, true);
                else
                    skipToLabel(obj, 8, true);

                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
            }
            if (
                    obj->y + obj->offset_hy < 0 ||
                    block_flags[obj->btypes[3]] & 0x10)
                obj->speed_y++;

            SET_X_SPEED(obj);
            if (obj->main_etat != 2)
                CALC_MOV_ON_BLOC(obj);
            else {
                if (obj->sub_etat == 1 || obj->sub_etat == 3)
                    obj->flags.flip_x ^= 1;
            }
            break;
        default:
            break;
    }
}

//7CCF0
void DoStoneDogPoingCollision(obj_t* obj, i16 a2) {
    obj_hurt(obj);
    if (obj->hit_points != 0) {
        skipToLabel(obj, 5, true);
        obj->gravity_value_2 = 7;
        obj->change_anim_mode = 2;
        if (poing_obj->speed_x > 0)
            obj->flags.flip_x = 0;
        else if (poing_obj->speed_x < 0)
            obj->flags.flip_x = 1;
    } else {
        set_main_and_sub_etat(obj, 0, 3);
        obj->flags.read_commands = 0;
    }
}

//7CD54
void DO_STONEBOMB_COMMAND(obj_t* obj) {
    /* 49A74 8016E274 -O2 -msoft-float */
    u8 timer_0 = false;

    if (obj->type == TYPE_36_STONEBOMB) {
        if (obj->timer == 0) {
            DO_STONE_EXPLOSION(obj, 0);
            timer_0 = true;
        } else {
            if (horloge[3] == 0) {
                obj->timer--;
            }

            if (obj->timer == 0) {
                DO_STONE_EXPLOSION(obj, 0);
                timer_0 = true;
            }
        }
    }

    if (!timer_0) {
        if (block_flags[calc_typ_travd(obj, false)] >> BLOCK_FULLY_SOLID & 1) {
            DO_STONE_EXPLOSION(obj, -1);
        } else {
            if (obj->main_etat == 2 && obj->sub_etat == 1) {
                Projectil_to_RM(obj, &obj->speed_x, &obj->speed_y, 30, 3);
            }
        }
    }
}

//7CDE0
void IS_STONEWOMAN_WAIT(obj_t* obj) {
    //stub
}

//7CE04
void DO_STONEWOMAN_COMMAND(obj_t* obj) {
    //stub
}

//7D070
void DO_NOMOVE_STONEWOMAN_COMMAND(obj_t* obj) {
    //stub
}

//7D0F0
void DO_ONE_STONECHIP_COMMAND(obj_t* obj) {
    /* 4995C 8016E15C -O2 -msoft-float */
    switch (obj->cmd) {
        case GO_LEFT:
            obj->flags.flip_x = 0;
            SET_X_SPEED(obj);
            switch (obj->sub_etat)
            {
                case 9:
                    obj->speed_y = obj->speed_x;
                    break;
                case 11:
                    obj->speed_y = 0;
                    break;
                case 13:
                    obj->speed_y = -obj->speed_x;
                    break;
            }
            break;
        case GO_RIGHT:
            obj->flags.flip_x = 1;
            SET_X_SPEED(obj);
            obj->flags.flip_x = 0;
            switch (obj->sub_etat)
            {
                case 10:
                    obj->speed_y = -obj->speed_x;
                    break;
                case 12:
                    obj->speed_y = 0;
                    break;
                case 14:
                    obj->speed_y = obj->speed_x;
                    break;
            }
            break;
        default:
            obj->speed_x = 0;
            obj->speed_y = 0;
            break;
    }
}

//7D1C8
void setStoneChipPos(obj_t* obj1, obj_t* obj2, u8* param_3) {
    /* 38118 8015C918 -O2 -msoft-float */
    if (!*param_3) {
        obj2->x = (obj1->x + obj1->offset_bx) - obj2->offset_bx;
        obj2->y = (obj1->y + obj1->offset_by) - obj2->offset_by;
        switch (obj2->sub_etat) {
            case 9:
                obj2->x -= (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y -= obj2->offset_by + obj2->offset_by / 3;
                break;
            case 10:
                obj2->x += (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y -= obj2->offset_by + obj2->offset_by / 3;
                break;
            case 11:
                obj2->x -= (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y -= obj2->offset_by;
                break;
            case 12:
                obj2->x += (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y -= obj2->offset_by;
                break;
            case 13:
                obj2->x -= (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y += obj2->offset_by / 3 - obj2->offset_by;
                break;
            case 14:
                obj2->x += (obj2->offset_bx >> 1) - obj2->offset_bx;
                obj2->y += obj2->offset_by / 3 - obj2->offset_by;
                break;
        }
        calc_obj_pos(obj2);
        obj2->flags.alive = 1;
        obj2->is_active = 1;
        add_alwobj(obj2);
        obj2->flags.flag_0x40 = 0;
        *param_3 = true;
    }
}

//7D37C
void allocateStoneChips(obj_t* obj) {
    u8 se_1;
    u8 se_2;
    u8 se_3;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    u8 sub_etat;
    u8 unk_par_3 = false;
    u8 unk_par_2 = false;
    u8 unk_par_1 = false;

    switch (obj->speed_x > 0) {
        case false:
            se_1 = 10;
            se_2 = 12;
            se_3 = 14;
            break;
        case true:
            se_1 = 9;
            se_2 = 11;
            se_3 = 13;
            break;
    }

    i = 0;
    cur_obj = &level.objects[i];
    nb_objs = level.nb_objects;
    while (i < nb_objs)
    {
        if (cur_obj->type == TYPE_STONECHIP && !(cur_obj->is_active))
        {
            sub_etat = cur_obj->sub_etat;
            if (sub_etat == se_1)
                setStoneChipPos(obj, cur_obj, &unk_par_1);
            else if (sub_etat == se_2)
                setStoneChipPos(obj, cur_obj, &unk_par_2);
            else if (sub_etat == se_3)
                setStoneChipPos(obj, cur_obj, &unk_par_3);
        }
        cur_obj++;
        i++;
    }
}

//7D43C
void DO_STONE_EXPLOSION(obj_t* obj, i16 a2) {
    /* 38514 8015CD14 -O2 -msoft-float */
    if (a2 != -1) {
        set_sub_etat(obj, 1); // added in PC/mobile versions
    }
    PlaySnd_old(84);
    if (obj->is_active) {
        if (obj->main_etat == 2 && obj->sub_etat == 1) {
            obj->x += 20;
            obj->speed_x = -1;
            allocateStoneChips(obj);
            obj->x -= 20;
            obj->speed_x = 1;
            allocateStoneChips(obj);
            obj->x += 10;
        }
        else
            allocateStoneChips(obj);

        obj->is_active = 0;
        if (obj->type == TYPE_STONEBOMB)
            obj->flags.alive = 0;
        else
            obj->y = ymap + 484;

        allocateExplosion(obj);
    }
}

//7D4E4
void allocateStonemanStone(obj_t* obj) {
    //stub
}

//7D83C
void DO_STONEMAN1_TIR(obj_t* obj) {
    //stub
}

//7D908
void DO_STONEMAN2_TIR(obj_t* obj) {
    //stub
}

//7D9C0
void allocateStonewomanStone(obj_t* obj) {
    //stub
}

//7DC5C
void DO_STONEWOMAN_TIR(obj_t* obj) {
    //stub
}

//7DD78
void allocateEclatPS(obj_t* obj) {
    //stub
}

//7DE68
void DO_PI_EXPLOSION(obj_t* obj) {
    //stub
}

//7E000
void DoPiMusPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//7E120
void DoPiPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//7E190
void DO_PNG_COLL_STONEMAN(obj_t* obj, i16 a2) {
    //stub
}

//7E258
void DO_PNG_COLL_STONEWOMAN(obj_t* obj, i16 a2) {
    //stub
}

//7E2FC
void DoStoneMan1et2RaymanZDD(obj_t* obj) {
    //stub
}

//7E33C
void DoStoneDogRaymanZDD(obj_t* obj) {
    //stub
}
