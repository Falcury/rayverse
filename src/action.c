
//18890
void setBossReachingSpeeds(obj_t* obj, u8 timer, u8 accuracyX, u8 accuracyY) {
    s16 xDiff;
    s16 yDiff;
    s32 xDist;
    s32 yDist;
    s32 speed;
    s16 maxDist;
    s32 dir;
    s32 newSpeedX;
    s32 newSpeedY;

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
        xDiff = (bossXToReach - obj->x) * 16;
        yDiff = (bossYToReach - obj->y) * 16;
        
        if (!(xDiff == 0 && yDiff == 0) && speed != 0) {
            // Get the distance to reach
            xDist = Abs(xDiff);
            yDist = Abs(yDiff);
            maxDist = MAX(xDist, yDist);

            // Make sure speed isn't greater than the distance to move
            speed = MIN(maxDist, speed);

            // Get the direction as an integer
            dir = ashl32(maxDist, 4) / speed;

            // Get the new speed for x and y as integers
            newSpeedX = ashl32(xDiff, 4) / dir;
            newSpeedY = ashl32(yDiff, 4) / dir;

            // Check the frame timer and change to new speed, using lerp based on accuracy value (0-255)
            if (horloge[timer] == 0) {
                obj->speed_x = LERP(obj->speed_x, newSpeedX, accuracyX, U8_MAX) / U8_MAX;

                // If the object is in the air then optionally force full accuracy
                if (obj->main_etat == OBJ_MAIN_ETAT_IN_AIR && speed >= yDiff) {
	                accuracyY = U8_MAX;
                }

                obj->speed_y = LERP(obj->speed_y, newSpeedY, accuracyY, U8_MAX) / U8_MAX;
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
i16 testActionEnd(obj_t* obj) {
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
i16 firstFloorBelow(obj_t* obj) {
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
    i16 new_bossScrollStartX = bossScrollStartX;
    i16 new_bossScrollEndX = bossScrollEndX;
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
