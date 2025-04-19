
//75840
void saxoCanAttak(void) {
    //stub
}

//75928
void INIT_SAXO(obj_t* obj) {
    //stub
}

//759B4
void allocateNote2(obj_t* obj, i16 a2) {
    //stub
}

//75AC0
void DO_EXPLOSE_NOTE2(obj_t* obj) {
    //stub
}

//75B0C
void DO_NOTE_CMD(obj_t* obj) {
    //stub
}

//75D20
void Cree_Eclat_Note(obj_t* bnote, obj_t* note1, i16 index) {
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
void BonneNote(obj_t* obj) {
    //stub
}

//760CC
void DO_NOTE_TOUCHEE(obj_t* obj, i16 a2) {
    //stub
}

//76124
void DO_NOTE_REBOND(obj_t* obj) {
    //stub
}

//7619C
void allocateNote(obj_t* obj) {
    //stub
}

//76334
void PrepareAtak(obj_t* obj) {
    //stub
}

//76388
void SAXO_TIRE(obj_t* obj) {
    //stub
}

//763BC
void DO_SAXO_COUP(obj_t* obj) {
    //stub
}

//76450
void DO_SAXO2_COUP(obj_t* obj) {
    //stub
}

//764F0
void SetSaxoEventPos(obj_t* obj) {
    //stub
}

//76564
void SetSaxoCollNoteBox(obj_t* obj) {
    //stub
}

//76614
void DO_SAXO_COMMAND(obj_t* obj) {
    //stub
}

//77134
void DO_SAXO_ATTER(obj_t* obj) {
    //stub
}

//771B8
void DO_SAXO2_COMMAND(obj_t* obj) {
    //stub
}

//776A0
void DO_SAXO2_ATTER(obj_t* obj) {
    //stub
}

//776F4
void DO_SAXO_MARCHE(obj_t* obj) {
    //stub
}

//77704
void DO_SAXO3_COMMAND(obj_t* obj) {
    //stub
}

//778A4
void DO_SAXO3_DEBUT(obj_t* obj) {
    //stub
}

//778CC
i32 saxo2_get_eject_sens(void) {
    return Phase < 2 ? -1 : 1;
}




