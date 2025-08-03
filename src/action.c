
//18890
void setBossReachingSpeeds(obj_t* obj, u8 horloge_ind, u8 unk_x, u8 unk_y) {
    /* 66BE8 8018B3E8 -O2 -msoft-float */
    s16 diff_x_1;
    s16 diff_y_1;
    s32 diff_x_abs;
    s32 diff_y_abs;
    s32 unk_1;
    s16 unk_2;
    s32 unk_3;
    s32 unk_4;
    s32 unk_5;

    if ((bossXToReach != -32000) && (bossYToReach != -32000))
    {
        unk_1 = obj->eta[obj->main_etat][obj->sub_etat].speed_x_right * (bossSpeedFactor + alternateBossSpeedFactor);
        if (unk_1 < 0)
            unk_1 += 0x3FFF;
        unk_1 = unk_1 >> 0xE;
        diff_x_1 = (bossXToReach - obj->x) * 16;
        diff_y_1 = (bossYToReach - obj->y) * 16;
        if (!(diff_x_1 == 0 && diff_y_1 == 0) && unk_1 != 0)
        {
            diff_x_abs = Abs(diff_x_1);
            diff_y_abs = Abs(diff_y_1);
            unk_2 = MAX(diff_x_abs, diff_y_abs);
            unk_1 = MIN(unk_2, unk_1);
            unk_3 = ashl32(unk_2, 4) / unk_1;
            unk_4 = ashl32(diff_x_1, 4) / unk_3;
            unk_5 = ashl32(diff_y_1, 4) / unk_3;
            if (horloge[horloge_ind] == 0)
            {
                obj->speed_x = (obj->speed_x * (0xFF - unk_x) + (unk_4 * unk_x)) / 255;
                if (obj->main_etat == 2)
                {
                    if (unk_1 >= diff_y_1)
                        unk_y = 0xFF;
                }
                obj->speed_y = (obj->speed_y * (0xFF - unk_y) + (unk_5 * unk_y)) / 255;
            }
        }
        else
        {
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    }
    else
    {
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
    floorLine = firstFloorBelow(obj); //TODO
    bossScrollStartY = Bloc_lim_H1 - Bloc_lim_H2 + floorLine - 16;
    bossScrollEndY = ymapmax;
    adjustBossScrollLocker(); //TODO
}
