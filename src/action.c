
//18890
void setBossReachingSpeeds(obj_t* obj, u8 timer, u8 accuracy_x, u8 accuracy_y) {
    s16 diff_x;
    s16 diff_y;
    s32 dist_x;
    s32 dist_y;
    s32 speed;
    s16 max_dist;
    s32 dir;
    s32 new_speed_x;
    s32 new_speed_y;

    // Check if x and y targets have been set
    if (bossXToReach != OBJ_INVALID_XY && bossYToReach != OBJ_INVALID_XY) {
        // Get speed as fixed-point Q14
        speed = OBJ_STATE(obj).speed_x_right * (bossSpeedFactor + alternateBossSpeedFactor);

        // Convert to integer, while truncating to 0
        if (speed < 0) {
            speed += 0x3FFF;
        }
        speed = speed >> 0xE;
        
        // Get x and y differences as fixed-point Q4
        diff_x = (bossXToReach - obj->x) * 16;
        diff_y = (bossYToReach - obj->y) * 16;
        
        if (!(diff_x == 0 && diff_y == 0) && speed != 0) {
            // Get the distance to reach
            dist_x = Abs(diff_x);
            dist_y = Abs(diff_y);
            max_dist = MAX(dist_x, dist_y);

            // Make sure speed isn't greater than the distance to move
            speed = MIN(max_dist, speed);

            // Get the direction as an integer
            dir = ashl32(max_dist, 4) / speed;

            // Get the new speed for x and y as integers
            new_speed_x = ashl32(diff_x, 4) / dir;
            new_speed_y = ashl32(diff_y, 4) / dir;

            // Check the frame timer and change to new speed, using lerp based on accuracy value (0-255)
            if (horloge[timer] == 0) {
                obj->speed_x = LERP(obj->speed_x, new_speed_x, accuracy_x, U8_MAX) / U8_MAX;

                // If the object is in the air then optionally force full accuracy
                if (obj->main_etat == OBJ_MAIN_ETAT_IN_AIR && speed >= diff_y) {
	                accuracy_y = U8_MAX;
                }

                obj->speed_y = LERP(obj->speed_y, new_speed_y, accuracy_y, U8_MAX) / U8_MAX;
            }
        }
        // If no movement to perform then reset speed
        else {
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    }
    // If no x and y targets then reset speed
    else {
        obj->speed_x = 0;
        obj->speed_y = 0;
    }
}

//18A64
s16 testActionEnd(obj_t* obj) {
    /* 66EFC 8018B6FC -O2 -msoft-float */
    s16 res = false;

    if (bossXToReach == -32000 && bossYToReach == -32000)
        res = obj->nb_cmd == 0;
    else if (obj->x == bossXToReach && obj->y == bossYToReach)
        res = true;
    if (res)
        currentBossActionIsOver = true;
    return res;
}

//18AD0
s16 firstFloorBelow(obj_t* obj) {
    /* 6703C 8018B83C -O2 -msoft-float */
    s16 x = obj->offset_bx + obj->x;
    s16 y = obj->offset_by + obj->y;
    u8 btyp = BTYP(x >> 4, y >> 4);

    while (!(block_flags[btyp] >> BLOCK_SOLID & 1) && (y <= ymapmax + SCREEN_HEIGHT))
    {
        y += 16;
        btyp = BTYP(x >> 4, y >> 4);
    }

    return dist_to_bloc_floor(btyp, x & 0xf, 0) + (y & ~0xf);
}

//18B4C
void adjustBossScrollLocker(void) {
    s16 new_bossScrollStartX = bossScrollStartX;
    s16 new_bossScrollEndX = bossScrollEndX;
    if (GameModeVideo != MODE_NORMAL) {
        if (bossScrollStartX < 0) {
            new_bossScrollStartX = 0;
        }
        if (new_bossScrollStartX > xmapmax) {
            new_bossScrollStartX = xmapmax;
        }
        if (bossScrollEndX < 0) {
            new_bossScrollEndX = 0;
        }
        if (new_bossScrollEndX > xmapmax) {
            new_bossScrollEndX = xmapmax;
        }
    } else {
        if (bossScrollStartX < (8 - Bloc_lim_W1)) {
            new_bossScrollStartX = (8 - Bloc_lim_W1);
        }
        if (new_bossScrollStartX > xmapmax + 8) {
            new_bossScrollStartX = xmapmax + 8;
        }
        if (bossScrollEndX < (8 - Bloc_lim_W1)) {
            new_bossScrollEndX = (8 - Bloc_lim_W1);
        }
        if (new_bossScrollEndX > xmapmax + 8) {
            new_bossScrollEndX = xmapmax + 8;
        }
    }
    if (bossScrollStartY < -Bloc_lim_H1) {
        bossScrollStartY = -Bloc_lim_H1;
    }
    if (bossScrollStartY > ymapmax) {
        bossScrollStartY = ymapmax;
    }
    if (bossScrollEndY < -Bloc_lim_H1) {
        bossScrollEndY = -Bloc_lim_H1;
    }
    if (bossScrollEndY > ymapmax) {
        bossScrollEndY = ymapmax;
    }

    bossScrollStartX = new_bossScrollStartX;
    bossScrollEndX = new_bossScrollEndX;
}

//18C8C
void setBossScrollLimits(obj_t* obj) {
    bossScrollStartX = scroll_start_x;
    bossScrollEndX = scroll_end_x;
    floorLine = firstFloorBelow(obj);
    bossScrollStartY = Bloc_lim_H1 - Bloc_lim_H2 + floorLine - 16;
    bossScrollEndY = ymapmax;
    adjustBossScrollLocker();
}
