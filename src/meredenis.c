
//50B60
void findMereDenisWeapon(void) {
    mereDenis_weapon_id = -1;
    machine_obj_id = -1;

    if (!level.objects || level.nb_objects <= 0) return;

    for (s16 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur = &level.objects[i];
        if (!cur) continue;
        if (cur->type == TYPE_203_SMA_WEAPON) {
            mereDenis_weapon_id = i;
        } else if (cur->type == TYPE_112_WASHING_MACHINE) {
            machine_obj_id = i;
        }
    }
}


//50BB8
u8 mereDenisCanAttak(obj_t* obj) {
    if (scrollLocked) {
        u8 t = obj->timer;
        if (t) {
            if (horloge[2]) {
                if (scroll_start_y >= ymap)
                    return 0;
            } else {
                obj->timer = (u8)(t - 1);
                if (scroll_start_y >= ymap)
                    goto LABEL_6;
            }
        } else {
            if (scroll_start_y >= ymap)
                return 1;
        }
        scroll_start_y = ymap;
    } else {
        u8 is_in_zone_x = (xmap >= bossScrollStartX - h_scroll_speed) && (xmap <= bossScrollEndX + h_scroll_speed);
        u8 is_in_zone_y = (ymap >= bossScrollStartY - v_scroll_speed) && (ymap <= bossScrollEndY + v_scroll_speed);

        if (!is_in_zone_x || !is_in_zone_y) {
            scrollLocked = 0;
            return 0;
        }

        scrollLocked = 1;
        scroll_end_x = xmap;
        scroll_start_x = xmap;
        scroll_start_y = bossScrollStartY;
        scroll_end_y = bossScrollEndY;
    }

LABEL_6:
    if (obj->timer)
        return 0;
    return 1;
}



//50CC8
void setMereDenisAtScrollBorder(obj_t* obj, u8 flip_x) {
    if (flip_x) {
        if (flip_x == 1) {
            obj->flags.flip_x = 0;
            obj->x = scroll_end_x + 304 - obj->offset_bx;
        } else {
            
            s16 center = (scroll_start_x + scroll_end_x + 320) / 2;
            s16 obj_center = obj->x + obj->offset_bx;
            
            if (obj_center - center <= 0) {
                
                obj->x = scroll_start_x - obj->offset_bx;
                obj->flags.flip_x = 1;
            } else {
                
                obj->flags.flip_x = 0;
                obj->x = scroll_end_x + 304 - obj->offset_bx;
            }
        }
    } else {
        
        obj->flags.flip_x = 1;
        obj->x = scroll_start_x - obj->offset_bx;
    }
}

//50DA4
void setCirclePointToReach(obj_t* obj) {
    bossXToReach = circle_x + (sinus(circle_index) * 100) / 512;
    bossYToReach = circle_y + (cosinus(circle_index) * 60) / 512;
    circle_index += 4;
    if (circle_index < 512) {circle_index += 4;
    } else {
    circle_index = -32000; }
}

//50E5C
void allocateMereDenisBombChips(obj_t* obj) {
    u8 i;
    u8 unk_1;
    u8 se1;
    u8 se2;

    for (i = 0; i <= 2; i += 2) {
        for (s16 j = 0; j < level.nb_objects; ++j) {
            obj_t* cur = level.objects + j;

        
            unk_1 = 0;
            switch (obj->sub_etat) {
                case 0:
                case 1:
                    se1 = 0;
                    se2 = 1;
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    if (obj->flags.flip_x) {
                        se1 = 4;
                        se2 = 5;
                    } else {
                        se1 = 2;
                        se2 = 3;
                    }
                    break;
                case 6:
                case 7:
                    if (obj->flags.flip_x)
                        unk_1 = 1;
                    se1 = 6;
                    se2 = 7;
                    break;
                default:
                    if (obj->sub_etat == 1) { se1 = 0; se2 = 1; }
                    else { se1 = 2; se2 = 3; }
                    break;
            }

            if (cur->type == TYPE_194_SMA_BOMB_CHIP &&
                !cur->is_active &&
                (cur->sub_etat == se1 || cur->sub_etat == se2))
            {
                cur->flags.flip_x = obj->flags.flip_x;
                cur->flags.alive = 1;
                cur->is_active = 1;
                cur->x = obj->x;
                cur->y = obj->y;
                add_alwobj(cur); 
                set_main_and_sub_etat(cur, 1, (i == 0 ? se1 : se2));
                calc_obj_pos(cur);

                
                skipToLabel(cur, (unk_1 ? i == 0 : i != 0), true);
                break;
            }
        }
    }

    allocateExplosion(obj);
    PlaySnd(220, obj->id);
    obj->is_active = 0;
    obj->flags.alive = 0;
}

//51010
void mereDenisExplodeBombs(obj_t* obj) {
    for (int t = 0; t < 8; t++) {
        for (int v = 0; v < 8; v++) {
            s16 id = droppedBombIds[t * 8 + v];
            if (id != -1 && id < level.nb_objects) {
                level.objects[id].timer = 70; 
                droppedBombIds[t * 8 + v] = -1;
            }
        }
    }
}

//5108C
void mereDenisDropBomb(obj_t *smama_obj)
{
    s16 ray_x;
    s16 smama_x_1; s16 smama_x_2;
    obj_t *cur_obj;
    s16 j; u8 i;
    s16 nb_objs;
    u8 new_sub_etat;
    u8 new_flip_x;

    if (currentBombSequence == 0xfe && lastDroppedBombXCenterPos == -32000)
    {
        ray_x = ray.x + ray.offset_bx;
        smama_x_1 = smama_obj->x + smama_obj->offset_bx;
        if ((ray_x >= smama_x_1 - 8) && (ray_x <= smama_x_1 + 8))
        {
            cur_obj = level.objects; 
            j = 0;
            nb_objs = level.nb_objects;
            while (j < nb_objs)
            {
                if (cur_obj->type == TYPE_193_SMA_BOMB && cur_obj->is_active == 0)
                {
                    cur_obj->sub_etat = 0;
                    cur_obj->flags.flip_x = 0;
                    cur_obj->x = smama_x_1 - cur_obj->offset_bx;
#ifdef PC
                    cur_obj->y = smama_obj->y + smama_obj->offset_by - cur_obj->offset_by - 3 * ((Bloc_lim_H2 - Bloc_lim_H1) / 18);
#else
                    cur_obj->y = smama_obj->y + smama_obj->offset_by - cur_obj->offset_by - 3 * 11;
#endif
                    cur_obj->timer = 50;
                    cur_obj->flags.alive = 1;
                    cur_obj->is_active = 1;
                    add_alwobj(cur_obj);
                    break;
                }
                cur_obj++;
                j++;
            }
            currentBombSequence = 0xff;
            lastDroppedBombXCenterPos = smama_x_1;
        }
    }
    else
    {
        for (i = 0; i < sizeof(lastDroppedBombIdInSequence); i++)
        {
            if (lastDroppedBombIdInSequence[i] == 7) {
                currentBombSequence = 0xff;
                lastDroppedBombIdInSequence[i] = 0xff;
                lastDroppedBombXCenterPos = -32000;
            }
            else
            {
                smama_x_2 = smama_obj->x + smama_obj->offset_bx;
                if (i == 0) {
#ifdef PC
                    if (Abs(smama_x_2 - lastDroppedBombXCenterPos) < (Bloc_lim_W2 - Bloc_lim_W1) / 9) {
                        return;
                    }
#else
                    if (Abs(smama_x_2 - lastDroppedBombXCenterPos) < 0x25) {
                        return;
                    }
#endif
                }
                lastDroppedBombIdInSequence[i]++;
                switch (bombSequences[currentBombSequence][i][lastDroppedBombIdInSequence[i]])
                {
                case 0:
                case 4:
                    new_sub_etat = 0;
                    new_flip_x = 0;
                    break;
                case 1:
                case 5:
                    new_sub_etat = 2;
                    new_flip_x = smama_obj->flags.flip_x;
                    break;
                case 2:
                    new_sub_etat = 6;
                    new_flip_x = smama_obj->flags.flip_x;
                    break;
                case 6:
                    new_sub_etat = 6;
                    new_flip_x = !smama_obj->flags.flip_x;
                    break;
                case 3:
                case 7:
                    new_sub_etat = 4;
                    new_flip_x = !smama_obj->flags.flip_x;
                    break;
                case 8:
                    smama_x_2 = 0;
                    droppedBombIds[i * 8 + lastDroppedBombIdInSequence[i]] = -1;
                    break;
                default:
                    break;
                }

                if (smama_x_2 != 0)
                {
                    cur_obj = level.objects;
                    j = 0;
                    nb_objs = level.nb_objects;
                    while (j < nb_objs)
                    {
                        if (cur_obj->type == TYPE_193_SMA_BOMB && cur_obj->is_active == 0)
                        {
                            cur_obj->sub_etat = new_sub_etat;
                            cur_obj->flags.flip_x = new_flip_x;
                            droppedBombIds[i * 8 + lastDroppedBombIdInSequence[i]] = j;
                            cur_obj->anim_frame = 0;
                            cur_obj->x = smama_x_2 - cur_obj->offset_bx;
#ifdef PC
                            cur_obj->y = (smama_obj->y + smama_obj->offset_by - cur_obj->offset_by) + (i - 6) * ((Bloc_lim_H2 - Bloc_lim_H1) / 18);
#else
                            cur_obj->y = (smama_obj->y + smama_obj->offset_by - cur_obj->offset_by) + (i - 6) * 11;
#endif
                            cur_obj->flags.alive = 1;
                            cur_obj->is_active = 1;
                            add_alwobj(cur_obj);
                            break;
                        }
                        cur_obj++;
                        j++;
                    }
                    if (j >= nb_objs)
                        droppedBombIds[i * 8 + lastDroppedBombIdInSequence[i]] = -1;
                }
                lastDroppedBombXCenterPos = smama_obj->x + smama_obj->offset_bx;
            }
        }
    }
}



//51470
void swapWeaponAnimState(obj_t* obj, u8 new_sub_etat) {
        set_main_and_sub_etat(obj, 0, new_sub_etat);
        obj->anim_frame = 0;
        obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
    
}

//5149C
void swapMereDenisCollZones(obj_t* obj, u8 smama2) {
    if (!smama2)
        obj->type = TYPE_98_SPACE_MAMA;
    else
        obj->type = TYPE_213_SPACE_MAMA2; // corresponds to the washing machine space mama
obj->zdc = type_zdc[obj->type];

}

//515AC

void prepareNewMereDenisAttack(obj_t* smama_obj) {
    u8 cur_act = 0xFF;
    obj_t* wep_obj = (mereDenis_weapon_id != -1) ? &level.objects[mereDenis_weapon_id] : NULL;
    obj_t* mach_obj = (machine_obj_id != -1) ? &level.objects[machine_obj_id] : NULL;
    obj_t* poing_obj;
    u8 old_flip_x;
    s16 i;

    if (circle_index == -32000) 
    {
        while (currentBossActionIsOver) 
        {
            cur_act = mereDenisActionSequences[bossEncounter][currentBossAction++];
            switch (cur_act) {
            case 0:
                bossEncounter++;
            case 1:
                currentBossAction = 0;
                currentBossActionIsOver = 1;
                break;

            case 2:
                currentBossAction = saveCurrentBossAction;
                bossEncounter = saveBossEncounter;
                if (currentBossAction == 0xFF)
                    currentBossAction = saveCurrentBossAction + 1;
                currentBossActionIsOver = 1;
                saveBossEncounter = 0xFF;
                break;

            case 46:
                remoteRayXToReach = -32000;
                fin_boss = true;
                smama_obj->nb_cmd = vblToEOA(smama_obj, 1) - 4;
                currentBossActionIsOver = 0;
                break;

            case 42:
                set_main_and_sub_etat(smama_obj, 0, 3);
                remoteRayXToReach = ray.x;
                skipToLabel(smama_obj, 0, true);
                smama_obj->nb_cmd = 0;
                fin_boss = true;
                currentBossActionIsOver = 1;
                break;

            case 44:
                if (mach_obj) {
                    switch (mach_obj->sub_etat) {
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                        set_main_and_sub_etat(smama_obj, 0, 16);
                        swapWeaponAnimState(wep_obj, 43);
                        set_main_and_sub_etat(mach_obj, mach_obj->main_etat, mach_obj->sub_etat + 1);
                        skipToLabel(smama_obj, 0, true);
                        mach_obj->x = smama_obj->x;
                        mach_obj->y = smama_obj->y;
                        mach_obj->anim_frame = 0;
                        currentBossActionIsOver = 0;
                        break;

                    case 42:
                        wep_obj->flags.alive = 0;
                        currentBossAction = 0;
                        bossEncounter = 6;
                        currentBossActionIsOver = 1;
                        
                        break;
                    }
                }
                laserSourceSprNumInAnim = -1;
                break;

            case 40:
                    poing_obj = &level.objects[poing_obj_id];
                    set_main_and_sub_etat(smama_obj, 0, 2);
                    bossYToReach = smama_obj->y;
                    if (poing_obj->speed_x != 0) {
                        skipToLabel(smama_obj, 0, true);
                        bossXToReach = smama_obj->eta[0][2].speed_x_right * smama_obj->nb_cmd / 16;
                        if (poing_obj->speed_x < 0)
                            bossXToReach = smama_obj->x + bossXToReach;
                        else if (poing_obj->speed_x > 0)
                            bossXToReach = smama_obj->x - bossXToReach;

                        bossReachingAccuracyX = 0xFF;
                        bossReachingAccuracyY = 0xFF;
                        bossReachingTimer = 1;
                        setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                        skipToLabel(smama_obj, 1, true);
                    } else {
                        skipToLabel(smama_obj, 0, true);
                    }

                currentBossActionIsOver = 0;
                break;

            case 5:
                fix_numlevel(smama_obj);
                new_level = true;
                currentBossActionIsOver = 0;
                finBosslevel.space_mama = true;
                break;

            case 41:
                old_flip_x = smama_obj->flags.flip_x;
                calc_obj_dir(smama_obj);
                if (smama_obj->flags.flip_x != old_flip_x) {
                    set_main_and_sub_etat(smama_obj, 0, 4);
                    skipToLabel(smama_obj, 0, true);
                    currentBossActionIsOver = 0;
                } else {
                    currentBossActionIsOver = 1;
                }
                break;

            case 7:
                stepsForward = 0;
                currentPhaseHitCounter = 0;
                smama_obj->display_prio = 3;
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 0, 44);
                setMereDenisAtScrollBorder(smama_obj, 0);
                smama_obj->flags.flip_x = 0;
                smama_obj->x = smama_obj->x - 144;
                smama_obj->y = (floorLine - smama_obj->offset_by) - 32;
                bossXToReach = (scroll_start_x - smama_obj->offset_bx) + 50;
                bossYToReach = smama_obj->y;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 8:
                swapMereDenisCollZones(smama_obj, 1);
                circle_index = 4;
                circle_x = smama_obj->x;
                circle_y = smama_obj->y - 60;
                setCirclePointToReach(smama_obj);
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 12:
                swapMereDenisCollZones(smama_obj, 1);
                bossXToReach = smama_obj->x + 110;
                bossYToReach = (floorLine - smama_obj->offset_by) - 124;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 38:
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 2, 1);
                bossYToReach = floorLine - smama_obj->offset_by;
                bossXToReach = smama_obj->x;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 13:
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 0, 10);
                wep_obj->x = smama_obj->x;
                wep_obj->y = smama_obj->y;
                mach_obj->x = smama_obj->x;
                mach_obj->y = smama_obj->y;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 14:
                swapMereDenisCollZones(smama_obj, 0);
                if (mach_obj) {
                    set_main_and_sub_etat(mach_obj, 0, 37);
                    mach_obj->flags.alive = 1;
                    mach_obj->is_active = 1;
#ifdef PC
                    add_alwobj(mach_obj);
#endif
                    calc_obj_pos(mach_obj);
                }
                currentBossActionIsOver = 1;
                break;

            case 36: 
                swapMereDenisCollZones(smama_obj, 1);
                if (mach_obj) {
                    if (!mach_obj->flags.alive)
                        add_alwobj(mach_obj);
                    mach_obj->flags.alive = 1;
                    mach_obj->is_active = 1;
                }
                smama_obj->display_prio = 6;
                if (wep_obj) {
                    if (!wep_obj->flags.alive)
                        add_alwobj(wep_obj);
                    wep_obj->flags.alive = 1;
                    wep_obj->is_active = 1;
                    swapWeaponAnimState(wep_obj, 36);
                }
                set_main_and_sub_etat(smama_obj, 0, 19);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 15:
                swapMereDenisCollZones(smama_obj, 0);
                if ((s16)myRand(1))
                    set_main_and_sub_etat(smama_obj, 0, 0);
                else
                    set_main_and_sub_etat(smama_obj, 0, 9);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 16:
                swapMereDenisCollZones(smama_obj, 0);
                stepsForward++;
                set_main_and_sub_etat(smama_obj, 1, 1);
                skipToLabel(smama_obj, 0, true);
                bossYToReach = smama_obj->y;
                bossXToReach = smama_obj->eta[1][1].speed_x_right * smama_obj->nb_cmd / 16;
                if (smama_obj->flags.flip_x)
                    bossXToReach = smama_obj->x + bossXToReach;
                else
                    bossXToReach = smama_obj->x - bossXToReach;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 26:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 5);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 18:
                swapMereDenisCollZones(smama_obj, 0);
                laserSourceSprNumInAnim = 5;
                currentLaserSize = 4;
                set_main_and_sub_etat(smama_obj, 0, 7);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 34:
                switch (smama_obj->sub_etat) {
                case 34:
                    set_main_and_sub_etat(smama_obj, smama_obj->main_etat, 31);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 33);
                    break;
                case 21:
                case 31:
                case 19:
                    set_main_and_sub_etat(smama_obj, smama_obj->main_etat, 24);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 25);
                    break;
                }
                goto block_123;

            case 35:
                switch (smama_obj->sub_etat) {
                case 22:
                    set_main_and_sub_etat(smama_obj, smama_obj->main_etat, 23);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 26);
                    break;
                case 21:
                case 23:
                case 19:
                    set_main_and_sub_etat(smama_obj, smama_obj->main_etat, 30);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 32);
                    break;
                }
block_123:
                swapMereDenisCollZones(smama_obj, 1);
                if (wep_obj)
                    wep_obj->flags.alive = 1;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 32:
            case 33:
            case 43:
                swapMereDenisCollZones(smama_obj, 1);
                laserSourceSprNumInAnim = 8;
                currentLaserSize = mereDenisActionSequences[bossEncounter][currentBossAction++];
                switch (cur_act) {
                case 33:
                    set_main_and_sub_etat(smama_obj, 0, 22);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 18);
                    break;
                case 32:
                    set_main_and_sub_etat(smama_obj, 0, 21);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 17);
                    break;
                case 43:
                    set_main_and_sub_etat(smama_obj, 0, 34);
                    if (wep_obj)
                        swapWeaponAnimState(wep_obj, 35);
                    break;
                }
                if (wep_obj)
                    wep_obj->flags.alive = 1;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 19:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 8);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 21:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 11);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 39:
                swapMereDenisCollZones(smama_obj, 0);
                mach_obj->flags.alive = 1;
                mach_obj->is_active = 1;
#ifdef PC
                add_alwobj(mach_obj);
#endif
                smama_obj->display_prio = 3;
                mach_obj->speed_y = -6;
                set_main_and_sub_etat(mach_obj, 2, 1);
                mach_obj->speed_x = mach_obj->eta[mach_obj->main_etat][mach_obj->sub_etat].speed_x_left;
                set_main_and_sub_etat(smama_obj, 0, 14);
                smama_obj->x += 100;
                goto block_90;

            case 22:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 13);
block_90:
                bossXToReach = smama_obj->x;
                bossYToReach = scroll_end_y - smama_obj->offset_by - 16;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                if (cur_act == 22) {
                    bossReachingAccuracyX = 32;
                    bossReachingAccuracyY = 32;
                    bossReachingTimer = 2;
                    if (smama_obj->flags.flip_x)
                        bossXToReach += 80;
                    else
                        bossXToReach -= 80;
                }
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 30:
                smama_obj->flags.flip_x ^= 1;
                goto block_98;
            case 23:
            case 25:
                smama_obj->flags.flip_x = 1;
                goto block_98;
            case 45:
block_98:
                mere_denis_wait_time = 60;
                swapMereDenisCollZones(smama_obj, false);
                setMereDenisAtScrollBorder(smama_obj, smama_obj->flags.flip_x);
                if (cur_act == 45)
                {
                    bossXToReach = ray.offset_bx + ray.x - smama_obj->offset_bx;
                    smama_obj->x = bossXToReach;
                }
                else if (cur_act == 25)
                {
                    bossXToReach = level.objects[machine_obj_id].x + 100;
                    smama_obj->x = bossXToReach + 80;
                }
                else /* cur_act == 30 || cur_act == 23 */
                {
                   if (smama_obj->flags.flip_x)
                    {
                        bossXToReach = smama_obj->x + 48;
                        smama_obj->x = bossXToReach + 80;
                    }
                    else
                    {
                        bossXToReach = smama_obj->x - 160;
                        smama_obj->x = bossXToReach - 80;
                    }
                }
                smama_obj->y = scroll_end_y - smama_obj->offset_by - 16;
                bossYToReach = smama_obj->y;
                bossReachingAccuracyX = 24;
                bossReachingAccuracyY = 24;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                bossYToReach = floorLine - smama_obj->offset_by;
                set_main_and_sub_etat(smama_obj, 0, 14);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 24:
                calc_obj_dir(smama_obj);
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 1);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 37:
                swapMereDenisCollZones(smama_obj, 1);
                if (wep_obj)
                    wep_obj->flags.alive = 0;
                if (mach_obj)
                    mach_obj->flags.alive = 0;
                set_main_and_sub_etat(smama_obj, 0, 6);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 31:
                swapMereDenisCollZones(smama_obj, 1);
                smama_obj->x -= 100;
                if (mach_obj)
                    mach_obj->flags.alive = 0;
                set_main_and_sub_etat(smama_obj, 0, 20);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 27:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 15);
                bossXToReach = smama_obj->x;
                bossYToReach = scroll_end_y - smama_obj->offset_by - 16;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                currentBombSequence = 0xFF;
                break;

            case 28:
                swapMereDenisCollZones(smama_obj, 0);
                setMereDenisAtScrollBorder(smama_obj, (smama_obj->flags.flip_x ^ 1) & 1);
#ifdef PC
                if (smama_obj->flags.flip_x) {
                    smama_obj->x -= 144 - Bloc_lim_H1;
                    bossXToReach = scroll_end_x - smama_obj->offset_bx + SCREEN_WIDTH + 144;
                } else {
                    smama_obj->x += 136;
                    bossXToReach = scroll_start_x - smama_obj->offset_bx - 144;
                }
#else
                if (smama_obj->flags.flip_x) {
                    smama_obj->x -= 144;
                    bossXToReach = scroll_end_x - smama_obj->offset_bx + SCREEN_WIDTH + 144;
                } else {
                    smama_obj->x += 144;
                    bossXToReach = scroll_start_x - smama_obj->offset_bx - 144;
                }
#endif
                smama_obj->y =
                    floorLine - smama_obj->offset_by -
                    (mereDenisActionSequences[bossEncounter][currentBossAction++] + 32);
                bossYToReach = smama_obj->y;
                set_main_and_sub_etat(smama_obj, 0, 15);
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                timerBeforeFirstBomb = 41;
                currentBombSequence = mereDenisActionSequences[bossEncounter][currentBossAction++];

                if (currentBombSequence == 0xFF) {
                    do {
                        do {
                            currentBombSequence = myRand(6);
                        } while (currentBombSequence == 1);
                    } while (currentBombSequence == 6);
                }

                for (i = 0; i < (s16)sizeof(lastDroppedBombIdInSequence); i++)
                    lastDroppedBombIdInSequence[i] = 0xFF;

                lastDroppedBombXCenterPos = -32000;
                currentBossActionIsOver = 0;
                break;

            case 29:
                swapMereDenisCollZones(smama_obj, 0);
                smama_obj->flags.flip_x = !smama_obj->flags.flip_x;
                set_main_and_sub_etat(smama_obj, 0, 0);
                skipToLabel(smama_obj, 0, true);
                mereDenisExplodeBombs(smama_obj);
                currentBossActionIsOver = 0;
                break;
            }
        }
    }
}

//525FC
void snapLaserToWeapon(obj_t *laser_obj, u8 param_2)
{
    obj_t *unk_obj = &level.objects[laser_obj->follow_id];
    s16 x; s16 y; s16 w; s16 h;
    u8 sub_etat;
    eta_t *eta;

    GET_SPRITE_POS(unk_obj, laser_obj->follow_sprite, &x, &y, &w, &h);
    if (param_2)
    {
        sub_etat = unk_obj->sub_etat;
        eta = &unk_obj->eta[unk_obj->main_etat][sub_etat];
        switch (sub_etat)
        {
        case 17:
            eta->anim_index = 47;
            break;
        case 18:
            eta->anim_index = 46;
            break;
        case 35:
            eta->anim_index = 48;
            break;
        }
        unk_obj->anim_index = eta->anim_index;
        laser_obj->x = x - laser_obj->offset_bx;
        if (unk_obj->flags.flip_x)
        {
            laser_obj->cmd = GO_LEFT; //for some reason it's inverted compared to the PS1 decomp? 
            laser_obj->x += w - 5;
        }
        else
        {
            laser_obj->cmd = GO_RIGHT;
            laser_obj->x += 5;
        }
    }
    laser_obj->speed_y = 0;
    laser_obj->y = y + (h >> 1) - ((laser_obj->offset_by + laser_obj->offset_hy) / 2) + 2;
}

//52718
void allocateSpaceMamaLaser(obj_t* smama_obj)
{
    u8 sub_etat = 0;
    u16 obj_type = 0;
    
    switch (currentLaserSize) {
    case 1: sub_etat = 3; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 2: sub_etat = 4; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 3: sub_etat = 2; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 4: sub_etat = 4; obj_type = TYPE_244_SMA_PETIT_LASER; break;
            default:
            laserSourceSprNumInAnim = -1;
            currentLaserSize = 0;
            return;
    }

    for (s16 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == obj_type && cur_obj->is_active == 0) {
            obj_t* source = smama_obj;
            if (smama_obj->type == TYPE_213_SPACE_MAMA2 && mereDenis_weapon_id != -1) {
                source = &level.objects[mereDenis_weapon_id];
            }

            cur_obj->follow_id = source->id;
            cur_obj->follow_sprite = laserSourceSprNumInAnim;
            cur_obj->flags.read_commands = 0;
            cur_obj->flags.flip_x = source->flags.flip_x;
            
            cur_obj->anim_frame = 0;
            cur_obj->sub_etat = sub_etat;
            source->follow_id = cur_obj->id; 
            
            snapLaserToWeapon(cur_obj, 1); 
            
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj); 
            calc_obj_pos(cur_obj);
            break;
        }
    }
    laserSourceSprNumInAnim = -1;
    currentLaserSize = 0;
}

//52870
void doMereDenisCommand(obj_t* obj) {
    if (mere_denis_wait_time != 0) {
        mere_denis_wait_time--;
    } else { 
        if (mereDenisCanAttak(obj)) { 
            if (bossSafeTimer != 0) bossSafeTimer--;
            
            if (!(obj->flags.hurtbyfist)) { 
                 if (obj->cmd == GO_WAIT) {
                     if (laserSourceSprNumInAnim != -1) {
                         allocateSpaceMamaLaser(obj);
                     }
                     bossXToReach = -32000;
                     bossYToReach = -32000;
                     circle_index = -32000;
                     obj->speed_x = 0;
                     obj->speed_y = 0;
                } else {
                    laserSourceSprNumInAnim = -1;
                    if (circle_index != -32000) {
                        setCirclePointToReach(obj);
                    } else if (timerBeforeFirstBomb == 0 && currentBombSequence != 0xFF &&
                               obj->main_etat == 0 && obj->sub_etat == 15) {
                        mereDenisDropBomb(obj);
                    } else if (timerBeforeFirstBomb != 0) {
                        timerBeforeFirstBomb--;
                    }
                    setBossReachingSpeeds(obj, bossReachingTimer, bossReachingAccuracyX, bossReachingAccuracyY);
                }
                testActionEnd(obj);
            }
            prepareNewMereDenisAttack(obj);
        }
    }
    obj->flags.hurtbyfist = 0; 
}

//529A8
void changeMereDenisPhase(obj_t* obj) {
   u8 old_encounter;
    u8 *encounter;
    u8 *action;

    if (bossEncounter == 8 || bossEncounter == 9)
    {
        encounter = &saveBossEncounter;
        action = &saveCurrentBossAction;
    }
    else
    {
        encounter = &bossEncounter;
        action = &currentBossAction;
    }
    old_encounter = *encounter;

    switch (*encounter)
    {
    case 1:
        if (currentPhaseHitCounter > 1 && stepsForward > 2)
        {
            *encounter = 2;
            stepsForward = 0;
        }
        break;
    case 2:
        if (currentPhaseHitCounter > 2)
            *encounter = 3;
        break;
    case 3:
    case 4:
    case 5:
    case 6: /* set to this at some point in prepareNewMereDenisAttack, not sure if max for space_mama */
        break;
    }

    if (old_encounter != *encounter)
    {
        *action = 0;
        currentPhaseHitCounter = 0;
    }

}

//52A28
void fitSaveCurrentAction(obj_t* obj)
{
    (void)obj;

    u8 enc  = (u8)bossEncounter;
    u8 act  = (u8)currentBossAction;
    const u8* seq = (const u8*)mereDenisActionSequences[enc];
    u8 code = seq[act];

    for (;;)
    {
        switch (code)
        {
            case 0:
                ++enc;
                currentBossAction = 0;
                bossEncounter = enc;

                seq = (const u8*)mereDenisActionSequences[enc];
                act = 0;             
                code = seq[0];         
                continue;

            case 1:
                currentBossAction = 0;
                act = 0;
                code = seq[0];        
                continue;

            case 0x0F:
            case 0x12:
            case 0x13:
            case 0x18:
            case 0x1A:
            case 0x22:
            case 0x23:
            case 0x24:
                goto LABEL_4;

            case 0x16:
                if (act == 0)
                    goto LABEL_5;
                currentBossAction = --act;
                code = seq[act];
                continue;

            case 0x1C:
                act += 2;
                currentBossAction = act;
                code = seq[act];
                continue;

            case 0x1D:
                continue;

            case 0x21:
            case 0x2B:
                ++act;

LABEL_4:
                currentBossAction = ++act;
                code = seq[act];
                if (code > 0x2B)
                    goto LABEL_5;
                continue;

            default:
LABEL_5:
                saveCurrentBossAction = act;
                saveBossEncounter = enc;
                return;
        }
    }
}


//52AF4
void doMereDenisHit(obj_t* obj, s16 sprite) {
    if (!(obj->eta[obj->main_etat][obj->sub_etat].flags & 1))
        return;

    if (bossSafeTimer != 0)
        return;

    if ((obj->eta[obj->main_etat][obj->sub_etat].flags & 1) && bossSafeTimer == 0) {
        s16 unk_1;
        switch (obj->main_etat * 0x100 + obj->sub_etat) {
            case 0x013:
            case 0x015:
            case 0x016:
            case 0x017:
            case 0x018:
            case 0x01e:
            case 0x01f:
            case 0x022:
                unk_1 = 255;
                break;
            case 0x002:
            case 0x006:
            case 0x00a:
            case 0x00b:
            case 0x00d:
            case 0x00f:
            case 0x025:
            case 0x02c:
                unk_1 = -1;
                break;
            default:
                unk_1 = 1;
                break;
            }
        if (unk_1 == sprite)
        {
            if (unk_1 == 1)
                {
                    poing.damage = 1;
                    obj_hurt(obj);
                    currentPhaseHitCounter++;
                    changeMereDenisPhase(obj); 
                }
            if (obj->hit_points != 0)
            {
                if (!(bossEncounter == 8 || bossEncounter == 9))
                {
                    fitSaveCurrentAction(obj);
                    if (obj->type == TYPE_98_SPACE_MAMA)
                        bossEncounter = 8;
                    else
                        bossEncounter = 9;
                }
            }
            else {
             // PC version does triggerAction(15, 1);
                bossEncounter = 10;
                boss_mort = 1; // PC
            }

            currentBossAction = 0;
            obj->flags.hurtbyfist = 1;
            bossSafeTimer = 0xFF;
            currentBossActionIsOver = true;
            laserSourceSprNumInAnim = -1;
            currentLaserSize = 0;
        
        }


    }
    
}

//52C54
void mereDenisBigLaserCommand(obj_t *laser_obj)
{
    u8 max_frame;
    obj_t *unk_obj;
    eta_t *unk_eta;

    switch (laser_obj->sub_etat)
    {
    case 3:
    case 5:
        max_frame = 0;
        break;
    case 4:
        max_frame = 7;
        break;
    case 2:
        max_frame = 14;
        break;
    }

    if (laser_obj->anim_frame <= max_frame)
        snapLaserToWeapon(laser_obj, false);
    else
    {
        unk_obj = &level.objects[laser_obj->follow_id];
        if (unk_obj->follow_id == laser_obj->id) // field 4A? 
        {
            unk_eta = &unk_obj->eta[unk_obj->main_etat][unk_obj->sub_etat];
            switch (unk_obj->anim_index)
            {
            case 46:
                unk_eta->anim_index = 1;
                break;
            case 47:
                unk_eta->anim_index = 0;
                break;
            case 48:
                unk_eta->anim_index = 28;
                break;
            }
        }
    }
    SET_X_SPEED(laser_obj);
}


//52D10
void mereDenisBombCommand(obj_t* obj) {
    if (obj->timer != 0) {
        obj->timer--;
        if (obj->timer == 0) {
            allocateMereDenisBombChips(obj);
        }
    }
}
