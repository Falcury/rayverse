
//75840
void saxoCanAttak(void) {
    if (scrollLocked) {
        if (scroll_start_y < ymap) {
            scroll_start_y = ymap;
        }
    } else {
        bool lock = false;
        if (xmap >= bossScrollStartX - h_scroll_speed && xmap <= h_scroll_speed + bossScrollEndX) {
            if (ymap >= bossScrollStartY - v_scroll_speed && ymap <= v_scroll_speed + bossScrollEndY) {
                lock = true;
            }
        }
        scrollLocked = lock;
        if (lock) {
            scroll_end_x = xmap;
            scroll_start_x = xmap;
            scroll_start_y = bossScrollStartY;
            scroll_end_y = bossScrollEndY;
        }
    }
}

//75928
void INIT_SAXO(obj_t* sax_obj) {
    /* 50CDC 801754DC -O2 -msoft-float */
    sax_obj->y = firstFloorBelow(sax_obj) - sax_obj->offset_by;
    calc_obj_pos(sax_obj);
    sax_obj->is_active = 0;
    sax_obj->speed_x = 0;
    sax_obj->speed_y = 0;
    Phase = 0;
    IsBossThere = false;
    IndexSerie = 0;
    sax_obj->flags.alive = 1;
    if (sax_obj->type == TYPE_74_SAXO) {
        sax_obj->hit_points = Sax.saved_hp;
        setBossScrollLimits(sax_obj); // added in PC version
    }
    Sax.coup = 0;
    SaxMarche = 0;
    Sax.field10_0x10 = 0;
}

//759B4
void allocateNote2(obj_t* note_obj, s16 a2) {
    /* 50D7C 8017557C -O2 -msoft-float */
    obj_t* cur_obj;
    s16 i;
    u8 nb_objs;
    u8 offs_half;
    u16 offs_x;

    if (note_obj->iframes_timer > 0) {
        cur_obj = level.objects;
        i = 0;
        nb_objs = level.nb_objects;
        if (nb_objs != 0) {
            do {
                if (cur_obj->type == TYPE_NOTE2 && !cur_obj->is_active) {
                    cur_obj->flags.flip_x = 0;
                    cur_obj->speed_y = 0;
                    cur_obj->speed_x = 0;

                    offs_half = note_obj->offset_bx / 2;
                    offs_x = offs_half;
                    if (a2 != 0)
                        offs_x = -offs_half;
                    cur_obj->x = offs_x + note_obj->x;
                    cur_obj->y = note_obj->y;

                    cur_obj->main_etat = 0;
                    cur_obj->sub_etat = a2 + 2;
                    cur_obj->init_x = cur_obj->x;
                    cur_obj->init_y = cur_obj->y;
                    skipToLabel(cur_obj, 1, true);
                    calc_obj_pos(cur_obj);
                    cur_obj->flags.alive = 1;
                    cur_obj->flags.flag_0x40 = 0;
                    cur_obj->is_active = 1;
                    add_alwobj(cur_obj);
                    cur_obj->gravity_value_1 = 0;
                    cur_obj->iframes_timer = note_obj->iframes_timer - 1;
                    calc_obj_pos(cur_obj);
                    cur_obj->cmd_arg_1 = 10;
                    allocateExplosion(cur_obj);
                    break;
                }
                cur_obj++;
                i++;
            } while (i < nb_objs);
        }
    }
}

//75AC0
void DO_EXPLOSE_NOTE2(obj_t* obj) {
    /* 50EE0 801756E0 -O2 -msoft-float */
    obj->speed_x = 0;
    obj->speed_y = 0;
    calc_obj_pos(obj);
    set_main_and_sub_etat(obj, 0, 1);
    PlaySnd(246, obj->id);
    allocateExplosion(obj);
    obj->cmd_arg_1 = 10;
    obj->iframes_timer = 3;
}

//75B0C
void DO_NOTE_CMD(obj_t* obj) {
    /* 50F40 80175740 -O2 -msoft-float */
    s16 calc_x;
    s32 offs_x;
    u8 var_v1;
    u8 temp_v0_2;

    switch (obj->type) {
        case TYPE_182_NOTE3:
            if (obj->speed_x == 0) {
                calc_x = obj->offset_bx + obj->x - Sax.note_box_coll_x;
                if (calc_x < 20) {
                    set_main_and_sub_etat(obj, 2, 0);
                    obj->speed_x = 3;
                    obj->flags.flip_x = 1;
                }
                else if (calc_x < 200) {
                    /* vibrate back and forth the closer sax gets */
                    var_v1 = (calc_x >> 3) - 2;
                    offs_x = 1;
                    if (var_v1 < 2)
                        var_v1 = 2;
                    temp_v0_2 = var_v1;
                    if (calc_x < 50)
                        offs_x = 2;

                    if (horloge[temp_v0_2] < (temp_v0_2 / 2))
                        obj->x = obj->init_x + offs_x;
                    else
                        obj->x = obj->init_x;
                }
            }
            break;
        case TYPE_92_NOTE2:
            if (obj->main_etat == 0) {
                obj->cmd_arg_1--;
                if (obj->cmd_arg_1 == 0)
                {
                    switch (obj->sub_etat)
                    {
                        case 1:
                            allocateNote2(obj, 1);
                            /* fall through */
                        case 2:
                            allocateNote2(obj, 0);
                            obj->is_active = 0;
                            obj->flags.alive = 0;
                            break;
                        case 3:
                            allocateNote2(obj, 1);
                            obj->is_active = 0;
                            obj->flags.alive = 0;
                            break;
                    }
                }
            }
            break;
        case TYPE_BONNE_NOTE:
            if (inter_box(obj->x + obj->offset_bx - 8, obj->y + obj->offset_by - 8, 16, 16,
                          Sax.note_box_coll_x, Sax.note_box_coll_y, 20, 140)
            ) {
                if (Sax.coup == 0)
                    Sax.coup = 1;
                obj->is_active = 0;
                obj->flags.alive = 0;
                obj->y += 30;
                PlaySnd(243, obj->id);
                allocateExplosion(obj);
            }
            break;
    }
}

//75D20
void Cree_Eclat_Note(obj_t* bnote, obj_t* note1, s16 index) {
    /* 511C4 801759C4 -O2 -msoft-float */
    s16 speed_x;

    if (bnote->type == TYPE_102_BNOTE)
    {
        if (bnote->is_active)
        {
            do
            {
                bnote++;
                if (bnote->type != TYPE_102_BNOTE)
                    return;
            } while (bnote->is_active);
        }
    }

    if (bnote->type == TYPE_102_BNOTE)
    {
        if (index < 4)
        {
            bnote->flags.flip_x = (index % 2) & 1;
            if (index < 2)
                bnote->speed_y = -1;
            else
                bnote->speed_y = 1;

            if (bnote->flags.flip_x)
                speed_x = 1;
            else
                speed_x = -1;
            bnote->speed_x = speed_x;
        }
        else
        {
            bnote->flags.flip_x = (index % 2) & 1;
            switch (index)
            {
                case 4:
                    bnote->speed_y = 0;
                    bnote->speed_x = -2;
                    break;
                case 5:
                    bnote->speed_x = 2;
                    bnote->speed_y = 0;
                    break;
                case 6:
                    bnote->speed_x = 0;
                    bnote->speed_y = -2;
                    break;
                case 7:
                    bnote->speed_x = 0;
                    bnote->speed_y = 2;
                    break;
            }
        }
        bnote->x = note1->x;
        bnote->y = note1->y;
        bnote->main_etat = 2;
        bnote->sub_etat = 5;
        skipToLabel(bnote, 1, true);
        calc_obj_pos(bnote);
        bnote->flags.alive = 1;
        bnote->is_active = 1;
        add_alwobj(bnote);
        calc_obj_pos(bnote);
    }
}

//75E88
void DO_EXPLOSE_NOTE1(obj_t* obj) {
    obj_t *cur_obj;
    s16 i;
    u8 nb_objs_1;
    u8 nb_objs_2;

    cur_obj = level.objects;
    i = 0;
    nb_objs_1 = level.nb_objects;
    if (nb_objs_1 != 0)
    {
        nb_objs_2 = nb_objs_1;
        do
        {
            if (cur_obj->type == TYPE_BNOTE && !cur_obj->is_active)
            {
                PlaySnd(244, obj->id);
                allocateExplosion(obj);
                Cree_Eclat_Note(cur_obj++, obj, 0);
                Cree_Eclat_Note(cur_obj++, obj, 1);
                Cree_Eclat_Note(cur_obj++, obj, 2);
                Cree_Eclat_Note(cur_obj++, obj, 3);
                Cree_Eclat_Note(cur_obj++, obj, 4);
                Cree_Eclat_Note(cur_obj++, obj, 5);
                Cree_Eclat_Note(cur_obj++, obj, 6);
                Cree_Eclat_Note(cur_obj, obj, 7);
                obj->flags.alive = 0;
                obj->is_active = 0;
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs_2);
    }
}

//75F98
void BonneNote(obj_t* orig_obj) {
    /* 514F4 80175CF4 -O2 -msoft-float */
    obj_t* cur_obj;
    s16 i;
    u8 nb_objs;
    s16 speed_x;

    cur_obj = level.objects;
    i = 0;
    nb_objs = level.nb_objects;
    if (nb_objs != 0) {
        do {
            if (cur_obj->type == TYPE_BONNE_NOTE && !cur_obj->is_active) {
                if (orig_obj->speed_x == 0) {
                    cur_obj->flags.flip_x = 0;
                    cur_obj->speed_x = -1;
                    cur_obj->speed_y = -4;
                    cur_obj->gravity_value_2 = 10;
                    cur_obj->cmd_arg_1 = 2;
                } else {
                    speed_x = 4;
                    if (poing_obj->speed_x < 0)
                        speed_x = -4;
                    cur_obj->speed_x = speed_x;
                    if (speed_x < 0)
                        cur_obj->flags.flip_x = 0;
                    else
                        cur_obj->flags.flip_x = 1;
                    cur_obj->gravity_value_2 = 255;
                    cur_obj->cmd_arg_1 = 1;
                    cur_obj->speed_y = -1;
                }
                cur_obj->x = orig_obj->x;
                cur_obj->y = orig_obj->y;
                cur_obj->main_etat = 2;
                cur_obj->sub_etat = 3;
                cur_obj->init_x = cur_obj->x;
                skipToLabel(cur_obj, 1, true);
                calc_obj_pos(cur_obj);
                cur_obj->gravity_value_1 = 0;
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                add_alwobj(cur_obj);
                cur_obj->iframes_timer = 200;
                orig_obj->is_active = 0;
                orig_obj->flags.alive = 0;
                calc_obj_pos(cur_obj);
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
    }
}

//760CC
void DO_NOTE_TOUCHEE(obj_t* obj, s16 sprite) {
    /* 516B4 80175EB4 -O2 -msoft-float */
    switch (obj->type) {
        case TYPE_90_NOTE0:
        case TYPE_182_NOTE3:
            BonneNote(obj);
            break;
        case TYPE_91_NOTE1:
            DO_EXPLOSE_NOTE1(obj);
            break;
        case TYPE_93_BONNE_NOTE:
            obj->speed_x += poing_obj->speed_x;
            obj->speed_y = -1;
            obj->gravity_value_2 = 255;
            obj->cmd_arg_1 = 1;
            break;
    }
}

//76124
void DO_NOTE_REBOND(obj_t* obj) {
    /* 51774 80175F74 -O2 -msoft-float */
    switch (obj->type) {
        case TYPE_92_NOTE2:
            DO_EXPLOSE_NOTE2(obj);
            break;
        case TYPE_90_NOTE0:
        case TYPE_182_NOTE3:
        case TYPE_93_BONNE_NOTE:
            obj->speed_y = -Abs(obj->speed_y);
            if (obj->speed_y > -3)
                obj->speed_y--;

            obj->gravity_value_1 = 10 - obj->gravity_value_1;
            calc_obj_pos(obj);
            break;
    }
}

//7619C
void allocateNote(obj_t* obj) {
    /* 51828 80176028 -O2 -msoft-float */
    obj_t* note_obj;
    s16 i;
    u8 nb_objs_1;
    u8 nb_objs_2;

    if (NextNote > 6) {
        NextNote = 6; // added in PC version (maybe moved from SAXO_TIRE())
    }

    note_obj = level.objects;
    i = 0;
    nb_objs_1 = level.nb_objects;
    if (nb_objs_1 != 0)
    {
        nb_objs_2 = nb_objs_1;
        do {
            if ((note_obj->type == atak[NextNote].type + TYPE_NOTE0)) {
                if (!note_obj->is_active) {
                    note_obj->flags.flip_x = obj->flags.flip_x;
                    note_obj->speed_y = atak[NextNote].speed_y;
                    if (note_obj->flags.flip_x) {
                        note_obj->speed_x = atak[NextNote].speed_x;
                        note_obj->x = Sax.sprite2_x + 23 - note_obj->offset_bx;
                    } else {
                        note_obj->speed_x = -atak[NextNote].speed_x;
                        note_obj->x = Sax.sprite2_x + 9 - note_obj->offset_bx;
                    }
                    note_obj->y = (Sax.sprite2_y - note_obj->offset_by) + 25;
                    note_obj->main_etat = 2;
                    note_obj->sub_etat = atak[NextNote].type;
                    skipToLabel(note_obj, 1, true);
                    calc_obj_pos(note_obj);
                    note_obj->flags.alive = 1;
                    note_obj->flags.flag_0x40 = 0;
                    note_obj->is_active = 1;
                    add_alwobj(note_obj);
                    note_obj->gravity_value_1 = 0;
                    note_obj->gravity_value_2 = 10;
                    note_obj->iframes_timer = atak[NextNote].initial_iframes;
                    calc_obj_pos(note_obj);
                    break;
                }
            }
            note_obj++;
            i++;
        } while (i < nb_objs_2);
    }
}

//76334
u8 PrepareAtak(void) {
    /* 51A30 80176230 -O2 -msoft-float */
    attaque = SerieDatak[IndexSerie][IndexAtak];
    if (attaque.end) {
        IndexAtak = 0;
    } else {
        IndexAtak++;
    }

    return attaque.next_note;
}

//76388
void SAXO_TIRE(obj_t* obj) {
    /* 51AE0 801762E0 -O2 -msoft-float */
    if (obj->type == TYPE_74_SAXO) {
        allocateNote(obj);
        WaitForFinAtan = attaque.time + 1;
        NextNote = PrepareAtak();
    } else {
        NextNote = 0;
        allocateNote(obj);
    }
}

//763BC
void DO_SAXO_COUP(obj_t* obj) {
    /* 51B80 80176380 -O2 -msoft-float */
    u8 hp;

    if (Phase != 100) {
        hp = --obj->hit_points;
        if (hp != 0) {
            if (obj->main_etat != 2) {
                Sax.field8_0xe = obj->main_etat;
                Sax.field9_0xf = obj->sub_etat;
                set_main_and_sub_etat(obj, 0, 3);
                Sax.coup = 2;
            } else {
                Sax.coup = 0;
            }

            IndexSerie++;
            if (IndexSerie >= 4) {
                IndexSerie = 3;
            } else {
                IndexAtak = 0;
            }
            NextNote = PrepareAtak();
        }
        else
            Sax.coup = 0;
    }
}

//76450
void DO_SAXO2_COUP(obj_t* obj) {
    /* 51C7C 8017647C -O2 -msoft-float */
    switch (Phase) {
        case 1:
            obj->hit_points--;
            if (obj->init_hit_points - obj->hit_points >= 3)
                Phase = 2;
            FinAnim = false;
            set_sub_etat(obj, 3);
            Sax.coup = 2;
            break;
        case 2:
            obj->hit_points--;
            Sax.coup = 0;
            break;
        case 3:
            set_main_and_sub_etat(obj, 0, 3);
            obj->speed_x = 0;
            obj->hit_points--;
            FinAnim = false;
            Sax.coup = 2;
            break;
        default: break;
    }
}

//764F0
void SetSaxoEventPos(obj_t* obj) {
    s16 w = 0, h = 0;
    GET_SPRITE_POS(obj, 2, &Sax.sprite2_x, &Sax.sprite2_y, &w, &h);
    if (obj->flags.flip_x) {
        Sax.sprite2_x += w - 32;
    }
    Sax.sprite2_x += 12;
    Sax.sprite2_y += 12;
}

//76564
void SetSaxoCollNoteBox(obj_t* obj) {
    s16 sprite2_x, sprite2_y, sprite2_w, sprite2_h;
    s16 sprite5_x, sprite5_y, sprite5_w, sprite5_h;
    GET_SPRITE_POS(obj, 2, &sprite2_x, &sprite2_y, &sprite2_w, &sprite2_h);
    GET_SPRITE_POS(obj, 5, &sprite5_x, &sprite5_y, &sprite5_w, &sprite5_h);
    Sax.note_box_coll_y = sprite5_y + 20;
    Sax.note_box_coll_x = sprite2_x + ((sprite2_w - 20) >> 1);
}

//76614
void DO_SAXO_COMMAND(obj_t* obj) {
    /* 51E0C 8017660C -O2 -msoft-float */
    s16 sp18;
    s16 sp1A;
    s16 temp_a0;
    s16 temp_v0_2;
    s16 temp_v1_7;
    s16 new_speed_x;
    s16 var_a0_2;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 temp_lo_1;
    s32 temp_lo_2;
    s32 prev_dir;
    s32 temp_v1_9;
    u16 temp_v0;
    u16 temp_v0_3;
    s16 temp_v1_1;
    u32 temp_a3;
    s16 temp_v1_8;
    u8 temp_a1;
    s16 temp_s2;
    s16 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;
    u8 temp_v1_5;
    u8 temp_v1_6;
    int new_var;
    s16 *speed_y;

    saxoCanAttak();
    SetSaxoEventPos(obj);
//    new_var = 0x20;
//    scrollLocked = 1;
//    Sax.x_pos = obj->x;
//    Sax.y_pos = obj->y;
    SetSaxoCollNoteBox(obj);
    if (obj->hit_points == 0 && Phase != 100) {
        Sax.coup = 0;
        if ((block_flags[mp.map[(((obj->x + obj->offset_bx) >> 4) + (mp.width * ((obj->y + obj->offset_by) >> 4)))].tile_type
                 ] >> BLOCK_SOLID & 1)
        ) {
            obj->anim_frame = 255;
            set_main_and_sub_etat(obj, 0, 4);
            skipToLabel(obj, 0, 1);
            Phase = 100;
            Sax.field10_0x10 = 420;
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    }
    if (Sax.coup == 1) {
        DO_SAXO_COUP(obj);
        FinAnim = 0;
    } else {
        if (
                obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1) &&
                horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
                )
        {
            FinAnim = 1;
            WaitForAnim = 0;
        }
        else
        {
            FinAnim = 0;
        }
    }

    temp_v1_1 = obj->x;
    switch (Phase)
    {
        case 0:
            if ((obj->main_etat != 0) || (obj->sub_etat != 0))
            {
                if ((ray.x < (temp_v1_1 + 220)) && (temp_v1_1 < (ray.x + 110)))
                {
                    WaitForAnim = 1;
                }
                else
                {
                    set_main_and_sub_etat(obj, 0, 2);
                }
                if ((WaitForAnim != 0) && (FinAnim != 0))
                {
                    Phase = 1;
                    NiveauDansPhase = 1;
                    IndexAtak = 0;
                    IndexSerie = 0;
                    NextNote = PrepareAtak();
                    set_main_and_sub_etat(obj, 0, 1);
                }
            }
            break;
        case 1:
            prev_dir = obj->flags.flip_x;
            calc_obj_dir(obj);
            if (obj->flags.flip_x != prev_dir) {
                set_main_and_sub_etat(obj, 0, 0);
                obj->speed_x = 0;
                FinAnim = false;
            }
            switch (obj->sub_etat) {
                case 1:
                    if (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0) {
                        if (((obj->anim_frame == 24) ||
                            (obj->anim_frame == 28 && WaitForFinAtan == 0) ||
                            ((obj->anim_frame == 32) && (WaitForFinAtan == 0)))
                        ) {
                            SAXO_TIRE(obj);
                        }
                    }
                    break;
                case 3:
                    if (FinAnim) {
                        Sax.coup = 0;
                    }
                    break;
                case 2:
                    if (FinAnim) {
                        if (WaitForFinAtan < 2) {
                            switch (attaque.next_note) {
                                case 0:
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                case 6:
                                    set_sub_etat(obj, 1);
                                    break;
                                case 7:
                                    set_main_and_sub_etat(obj, 0, 10);
                                    Phase = 2;
                                    Sax.field10_0x10 = 3;
                                    break;
                                case 8:
                                    set_main_and_sub_etat(obj, 0, 10);
                                    Phase = 3;
                                    Sax.field10_0x10 = 2;
                                    break;
                                case 9:
                                    set_main_and_sub_etat(obj, 0, 10);
                                    Phase = 4;
                                    break;
                            }
                        } else {
                            WaitForFinAtan--;
                        }
                    }
                    break;
                case 0:
                    if (FinAnim) {
                        set_sub_etat(obj, 1);
                        NextNote = PrepareAtak();
                    }
                    break;
            }
            break;
        case 2:
            if (FinAnim && obj->main_etat == 0 && obj->sub_etat == 3) {
                set_main_and_sub_etat(obj, Sax.field8_0xe, Sax.field9_0xf);
                Sax.coup = 0;
                FinAnim = 0;
            }

            if (obj->main_etat == 0) {
                if (!(obj->flags.flip_x) ? obj->x + obj->offset_bx < 0 : obj->x + obj->offset_bx > xmapmax + SCREEN_WIDTH) {
                    obj->speed_x = 0;
                }

                if (FinAnim) {
                    switch (obj->sub_etat) {
                        case 10:
                            obj->gravity_value_2 = 5;
                            obj->gravity_value_1 = 0;
                            obj->speed_y = -6;
                            obj->y += obj->speed_y;
                            new_speed_x = -1;
                            if (obj->flags.flip_x) {
                                new_speed_x = 1;
                            }
                            obj->speed_x = new_speed_x;
                            break;
                        case 11:
                            if (--Sax.field10_0x10 > 0) {
                                set_main_and_sub_etat(obj, 0, 10);
                                obj->speed_x = 0;
                            } else {
                                set_main_and_sub_etat(obj, 1, 0);
                                obj->speed_x = obj->flags.flip_x ? 2 : -2;
                            }
                            break;
                    }
                }
            }
            else if (obj->main_etat == 1)
            {
                block_666:
                if (
                        !(obj->flags.flip_x) ?
                        (obj->x + 0x40) < 0 :
                        (obj->x + 0xC0) > (xmapmax + 0x140)
                        )
                {
                    obj->flags.flip_x = 1 - obj->flags.flip_x;
                    set_main_and_sub_etat(obj, 0, 0);
                    obj->speed_x = 0;
                    Phase = 1;
                }
            }
            break;
        case 3:
            if (FinAnim != 0 && obj->main_etat == 0 && obj->sub_etat == 3)
            {
                set_main_and_sub_etat(obj, Sax.field8_0xe, Sax.field9_0xf);
                Sax.coup = 0;
                FinAnim = 0;
            }

            if (obj->main_etat == 0)
            {
                if (
                        !(obj->flags.flip_x) ?
                        obj->x + obj->offset_bx < 0 :
                        obj->x + obj->offset_bx > xmapmax + SCREEN_WIDTH
                        )
                    obj->speed_x = 0;

                if (FinAnim != 0)
                {
                    switch (obj->sub_etat)
                    {
                        case 10:
                            obj->gravity_value_2 = 5;
                            obj->gravity_value_1 = 0;
                            obj->speed_y = -6;
                            obj->y += obj->speed_y;
                            obj->speed_x = obj->flags.flip_x ? 1 : -1;
                            break;
                        case 11:
                            Sax.field10_0x10 -= 1;
                            set_main_and_sub_etat(obj, 1, 0);
                            obj->speed_x = obj->flags.flip_x ? 2 : -2;
                            obj->iframes_timer = 30;
                            break;
                    }
                }
                break;
            }
            else if (obj->main_etat == 1)
            {
                if (Sax.field10_0x10 != 1)
                {
                    goto block_666;
                }
                if (--obj->iframes_timer <= 0)
                {
                    set_main_and_sub_etat(obj, 0, 10);
                    obj->speed_x = 0;
                }
            }
            break;
        case 4:
            if (FinAnim != 0 && obj->main_etat == 0 && obj->sub_etat == 3)
            {
                set_main_and_sub_etat(obj, Sax.field8_0xe, Sax.field9_0xf);
                Sax.coup = 0;
                FinAnim = 0;
            }

            if (obj->main_etat == 0)
            {
                if (
                        !(obj->flags.flip_x) ?
                        obj->x + obj->offset_bx < 0 :
                        obj->x + obj->offset_bx > xmapmax + SCREEN_WIDTH
                        )
                    obj->speed_x = 0;

                if (FinAnim != 0)
                {
                    switch (obj->sub_etat)
                    {
                        case 10:
                            obj->gravity_value_2 = 5;
                            obj->gravity_value_1 = 0;
                            temp_v1_8 = (ray.x - 0x30);
                            temp_v1_8 = obj->x - temp_v1_8;
                            var_a0_2 = temp_v1_8;
                            if (temp_v1_8 < 0)
                            {
                                var_a0_2 = -temp_v1_8;
                            }
                            if (var_a0_2 >= 0x65)
                            {
                                obj->speed_y = var_a0_2 / 20;
                                if (obj->speed_y > 9)
                                {
                                    obj->speed_y = 9;
                                }
                                else if (obj->speed_y < 4)
                                {
                                    obj->speed_y = 4;
                                }
                                var_v1_3 = -2;
                                if (obj->flags.flip_x)
                                {
                                    var_v1_3 = 2;
                                }
                                obj->speed_x = var_v1_3;
                            }
                            else
                            {
                                obj->speed_y = var_a0_2 / (s32) 10;
                                if (obj->speed_y > 9)
                                {
                                    obj->speed_y = 9;
                                }
                                else if (obj->speed_y < 4)
                                {
                                    obj->speed_y = 4;
                                }
                                var_v1_3 = -1;
                                if (obj->flags.flip_x)
                                {
                                    var_v1_3 = 1;
                                }
                                obj->speed_x = var_v1_3;
                            }

                            obj->y = obj->y - obj->speed_y;
                            obj->speed_y = -obj->speed_y;
                            break;
                        case 11:
                            if (!(obj->flags.flip_x))
                            {
                                goto block_140;
                            }
                            if ((obj->x + 0x30) < ray.x)
                            {
                                goto block_142;
                            }
                            goto block_145;
                        block_140:
                            if (ray.x < (obj->x + 0x30))
                            {
                                goto block_142;
                            }
                            goto block_143;
                        block_142:
                            set_main_and_sub_etat(obj, 0, 10);
                            break;
                        block_143:
                            if ((obj->x + 0x40) < 0)
                            {
                                goto block_155;
                            }
                            goto block_147;
                        block_145:
                            if ((obj->x + 0xC0) > (xmapmax + 0x140))
                            {
                                goto block_155;
                            }
                            goto block_147;
                        block_147:
                            set_main_and_sub_etat(obj, 1, 0);
                            var_v1_3 = -2;
                            if (obj->flags.flip_x)
                            {
                                var_v1_3 = 2;
                            }
                            obj->speed_x = var_v1_3;
                            break;
                    }
                }
            }
            else if (obj->main_etat == 1)
            {
                if (
                        !(obj->flags.flip_x) ?
                        (obj->x + 0x40) < 0 :
                        (obj->x + 0xC0) > (xmapmax + 0x140)
                        )
                {
                    block_155:
                    obj->flags.flip_x = 1 - obj->flags.flip_x;
                    set_main_and_sub_etat(obj, 0, 0);
                    obj->speed_x = 0;
                    Phase = 1;
                }
            }
            break;
        case 100:
            if (--Sax.field10_0x10 <= 0)
            {
                Sax.field10_0x10 = 0x03E7;
                fin_boss = 1;
                finBosslevel.mr_sax = true;
            }
            break;
    }

    temp_v1_9 = obj->x + obj->offset_bx + obj->speed_x;
    if ((temp_v1_9 < 0) || ((xmapmax + 303) < temp_v1_9)) // NOTE: 319 changed to 303 in PC version
    {
        obj->speed_x = 0;
    }
}

//77134
void DO_SAXO_ATTER(obj_t* obj) {
    /* 52BC4 801773C4 -O2 -msoft-float */
    if (obj->speed_y > 0) {
        CALC_MOV_ON_BLOC(obj);
        screen_trembling = 1;
        set_main_and_sub_etat(obj, 0, 11);
        obj->speed_x = 0;
        obj->speed_y = 0;
        allocateLandingSmoke(obj);
    }
}

//771B8
void DO_SAXO2_COMMAND(obj_t* obj) {
    /* 52C28 80177428 -O2 -msoft-float */
    s16 sprite_w;
    s16 sprite_h;
    u16 temp_x;
    s16 x_pos;
    u8 anim_frame;
    s32 main_etat;
    s32 offs_x;
    u8 should_dec;
    s32 next_x;

    scrollLocked = true;
    SetSaxoEventPos(obj);
    Sax.x_pos = obj->x;
    Sax.y_pos = obj->y;
    SetSaxoCollNoteBox(obj);
    if (Sax.coup == 1)
        DO_SAXO2_COUP(obj);

    if (
            obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1) &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
            )
    {
        FinAnim = true;
        WaitForAnim = false;
    }
    else
        FinAnim = false;

    x_pos = obj->x;
    if (xmapmax < ray.x + 150 && (mp.height * 16 - 110) < ray.y) {
        Sax.saved_hp = obj->hit_points;
    }

    switch (Phase) {
        default: break;
        case 0:
            CALC_MOV_ON_BLOC(obj);
            if (ray.x < x_pos + 220 && ray.x + 220 > x_pos)
                WaitForAnim = true;

            if (WaitForAnim && FinAnim) {
                Phase = 1;
                WaitForFinAtan = 2;
                NiveauDansPhase = 0;
                set_main_and_sub_etat(obj, 0, 1);
            }
            break;
        case 1:
            switch (obj->sub_etat) {
                case 1:
                    if (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0) {
                        anim_frame = obj->anim_frame;
                        if (anim_frame == 24 || (anim_frame == 28 && NiveauDansPhase >= 2) || (anim_frame == 32 && NiveauDansPhase >= 4)) {
                            SAXO_TIRE(obj);
                            NiveauDansPhase++;
                        }
                    }
                    break;
                case 2:
                    if (FinAnim) {
                        if (WaitForFinAtan < 2)
                            set_sub_etat(obj, 1);
                        else
                            WaitForFinAtan--;
                    }
                    break;
                case 3:
                    if (FinAnim)
                        Sax.coup = 0;
                    break;
            }
            break;
        case 2:
            switch (obj->sub_etat) {
                case 3:
                    if (FinAnim) {
                        Sax.coup = 0;
                        set_main_and_sub_etat(obj, 0, 10);
                    }
                    break;
                case 10:
                    if (FinAnim) {
                        obj->gravity_value_2 = 5;
                        obj->gravity_value_1 = 0;
                        obj->speed_y = -6;
                        obj->speed_x = -1;
                        obj->y -= 6;
                        Sax.field10_0x10 = 2;
                    }
                    break;
                case 11:
                    if (FinAnim)
                    {
                        if (obj->x < ray.x - 50) {
                            obj->flags.flip_x = 1 - obj->flags.flip_x;
                            set_sub_etat(obj, 0);
                            Phase = 3;
                        } else {
                            set_main_and_sub_etat(obj, 0, 10);
                        }
                    }
                    break;
            }
            break;
        case 3:
            /* TODO: m2c output was switch */
            main_etat = obj->main_etat;
            if (main_etat != 1) {
                if (main_etat < 2 && main_etat == 0 && FinAnim)
                {
                    switch (obj->sub_etat)
                    {
                        case 0:
                        case 2:
                        case 3:
                        case 11:
                            set_main_and_sub_etat(obj, 1, 0);
                            obj->speed_x = Sax.field10_0x10;
                            Sax.coup = 0;
                            break;
                        case 10:
                            set_main_and_sub_etat(obj, 2, 1);
                            obj->anim_frame = obj->gravity_value_2;
                            obj->gravity_value_2 = 5;
                            obj->gravity_value_1 = 0;
                            obj->speed_y = -7;
                            obj->speed_x = 2;
                            obj->y -= 7;
                            break;
                    }
                }
            }
            else
            {
                offs_x = obj->offset_bx + 50;
                /* TODO: shifts/div? */
                if (!(block_flags[mp.map[(((obj->x + offs_x) >> 4) + (mp.width * ((obj->y + obj->offset_by + 8) >> 4)))].tile_type
                          ] >> BLOCK_SOLID & 1)
                ) {
                    set_main_and_sub_etat(obj, 0, 0xA);
                    obj->speed_x = 0;
                }
                else if (xmap < obj->x + 150)
                {
                    if (Sax.field10_0x10 == 2)
                    {
                        if (horloge[2] != 0)
                        {
                            obj->speed_x = 1;
                            should_dec = horloge[4] < 2;
                        }
                        else
                        {
                            obj->speed_x = 2;
                            break;
                        }
                    }
                    else
                    {
                        obj->speed_x = 1;
                        should_dec = horloge[2];
                    }

                    if (should_dec != 0)
                        obj->anim_frame--;
                }
                else
                    obj->speed_x = 2;
                break;
            }
    }
    next_x = obj->x + obj->offset_bx + obj->speed_x;
    if (next_x < 0 || (xmapmax + SCREEN_WIDTH - 1) < next_x)
        obj->speed_x = 0;
}

//776A0
void DO_SAXO2_ATTER(obj_t* obj) {
    /* 533C8 80177BC8 -O2 -msoft-float */
    switch (Phase) {
        default: break;
        case 2:
        case 3:
            if (obj->speed_y > 0) {
                CALC_MOV_ON_BLOC(obj);
                screen_trembling3 = 1;
                set_main_and_sub_etat(obj, 0, 11);
                obj->speed_x = 0;
                obj->speed_y = 0;
                allocateLandingSmoke(obj);
            }
            break;
    }
}

//776F4
void DO_SAXO_MARCHE(obj_t* obj) {
    Sax.field10_0x10 = obj->hit_points + 1;
}

//77704
void DO_SAXO3_COMMAND(obj_t* obj) {
    /* 5345C 80177C5C -O2 -msoft-float */
    s16 sprite_w;
    s16 sprite_h;
    u16 temp_x;
    s32 temp_flags;

    SetSaxoEventPos(obj);
    Sax.x_pos = obj->x;
    Sax.y_pos = obj->y;
    SetSaxoCollNoteBox(obj);
    if (Phase == 2 && obj->main_etat == 0 && obj->sub_etat != 0)
        Phase = 0;
    if (
            obj->anim_frame == (obj->animations[obj->anim_index].frame_count - 1) &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
            )
    {
        FinAnim = true;
        WaitForAnim = false;
    }
    else
        FinAnim = false;

    switch (Phase) {
        case 0:
            break;
        case 1:
            switch (obj->sub_etat)
            {
                case 1:
                    if ((obj->anim_frame == 24) && (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0))
                        SAXO_TIRE(obj);
                    if (FinAnim)
                    {
                        obj->flags.flip_x = 1 - obj->flags.flip_x;
                        set_sub_etat(obj, 0);
                        Phase++;
                    }
                    break;
                case 3:
                    if (FinAnim)
                        Sax.coup = 0;
                    break;
            }
            break;
        case 2:
            if (FinAnim && obj->sub_etat == 0)
            {
                obj->speed_x = 2;
                set_main_and_sub_etat(obj, 1, 0);
            }
            break;
    }
}

//778A4
void DO_SAXO3_DEBUT(obj_t* obj) {
    /* 53758 80177F58 -O2 -msoft-float */
    if (Phase == 0) {
        Phase = 1;
        WaitForFinAtan = 1;
        set_main_and_sub_etat(obj, 0, 1);
    }
}

//778CC
s32 saxo2_get_eject_sens(void) {
    return Phase < 2 ? -1 : 1;
}




