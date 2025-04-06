

static inline void set_def_bande(i32 index, i16 field_0, u8 field_2, u8 field_4, u8 field_6) {
    def_bande_t def = {field_0, field_2, field_4, field_6};
    Def_Bande[index] = def;
}

static void init_def_bande(void) {
    Def_Bande = calloc(18000, sizeof(def_bande_t));
    for (i32 i = 0; i < 18000; i += 300) {
        set_def_bande(i, 288, 2, 0, 0);
    }
    set_def_bande(600, 320, 2, 0, 0);
    set_def_bande(1500, 88, 4, 4, 4);
    set_def_bande(1501, 110, 4, 0, 0);
    set_def_bande(1502, 15, 3, 0, 0);
    set_def_bande(1503, 75, 2, 0, 0);
    set_def_bande(1801, 64, 2, 0, 0);
    set_def_bande(1802, 192, 4, 0, 0);
    set_def_bande(1803, 64, 2, 0, 0);
    set_def_bande(4500, 168, 4, 4, 5);
    set_def_bande(4501, 70, 4, 0, 0);
    set_def_bande(4502, 50, 2, 0, 0);
    set_def_bande(6600, 320, 2, 0, 0);
    set_def_bande(6900, 288, 1, 0, 0);
    set_def_bande(7200, 159, 4, 0, 0);
    set_def_bande(7201, 129, 2, 0, 0);
    set_def_bande(7500, 12, 6, 0, 0);
    set_def_bande(7501, 20, 4, 0, 0);
    set_def_bande(7502, 64, 2, 0, 0);
    set_def_bande(7503, 20, 4, 0, 0);
    set_def_bande(7504, 56, 6, 0, 0);
    set_def_bande(7505, 20, 4, 0, 0);
    set_def_bande(7506, 64, 2, 0, 0);
    set_def_bande(7507, 20, 4, 0, 0);
    set_def_bande(7508, 28, 6, 0, 0);
    set_def_bande(7509, 16, 8, 0, 0);
    set_def_bande(8100, 84, 4, 0, 0);
    set_def_bande(8101, 204, 2, 0, 0);
    set_def_bande(10500, 177, 4, 0, 0);
    set_def_bande(10501, 111, 2, 0, 0);
    set_def_bande(10800, 179, 0, 0, 3);
    set_def_bande(10801, 20, 0, 16, 1);
    set_def_bande(10802, 1, 0, 32, 1);
    set_def_bande(10803, 40, 0, 0, 3);
    set_def_bande(11100, 183, 4, 4, 4);
    set_def_bande(11101, 5, 8, 0, 0);
    set_def_bande(11102, 21, 6, 0, 0);
    set_def_bande(11103, 78, 4, 0, 0);
    set_def_bande(11104, 1, 2, 0, 0);
    set_def_bande(13500, 109, 6, 0, 0);
    set_def_bande(13501, 178, 4, 0, 0);
    set_def_bande(13502, 1, 2, 0, 0);
    set_def_bande(15900, 213, 4, 0, 0);
    set_def_bande(15901, 75, 2, 0, 0);
    set_def_bande(16200, 200, 6, 0, 0);
    set_def_bande(16201, 87, 4, 0, 0);
    set_def_bande(16202, 1, 2, 0, 0);
}

//778F0
void Init_Bande(u8 fnd, i16 width, i16 height, u8* source_buf, u8* dest_buf) {

    // NB. Decided to initialize Def_Bande at runtime instead of compile-time because it's a very large and sparse array.
    init_def_bande();

    Num_Fond = fnd;

    if (height <= 200) {
        Val_Div_Scroll_Y = 1;
    } else {
        Val_Div_Scroll_Y = (16 * mp.height - 200) / (height - 200);
    }
    if (Val_Div_Scroll_Y != 0) {
        ++Val_Div_Scroll_Y;
    } else {
        Val_Div_Scroll_Y = 2;
    }
    Val_Add_Scroll_Y = height - 200 - (16 * mp.height - 200) / Val_Div_Scroll_Y - 1;

    if (num_world == world_5_cave && num_level == 10) {
        Init_Effet_Chaleur(width, height, source_buf, dest_buf); //TODO
        return;
    }

    NbBande = Tab_NbBande[10 * (num_world - 1) + fnd];

    i16 v7_field_0_cumulative = 0;
    for (i32 i = 0; i < NbBande; ++i) {
        def_bande_t* def_bande = Def_Bande + 3000 * (num_world - 1) + 300 * Num_Fond + i;
        bande_t* bande = Bande + i;
        bande->field_0 = def_bande->field_0;
        bande->field_8 = def_bande->field_2;
        bande->field_9 = def_bande->field_4;
        bande->field_4 = def_bande->field_6;
        bande->field_6 = 0;
        bande->field_14 = 0;
        bande->field_2 = v7_field_0_cumulative;
        u8 type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
        if (type_scroll == 0 || type_scroll == 2) {
            bande->source_buffer_pos = source_buf + v7_field_0_cumulative * width;
            bande->draw_buffer_pos = dest_buf + v7_field_0_cumulative * 320;
        } else {
            bande->source_buffer_pos = source_buf + v7_field_0_cumulative;
            bande->draw_buffer_pos = dest_buf + v7_field_0_cumulative;
        }

        v7_field_0_cumulative += bande->field_0;
    }

    NbSprite = Tab_NbSprite[10 * (num_world - 1) + Num_Fond];
    for (i32 i = 0; i < NbSprite; ++i) {
        def_sprite_t* def_sprite = Def_Sprite + 200 * (num_world - 1) + 20 * Num_Fond + i;
        def_sprite_t* sprite = Sprite + i;
        sprite->field_0 = def_sprite->field_0;
        sprite->field_2 = def_sprite->field_2;
        sprite->field_4 = def_sprite->field_4;
    }
}

//77BA0
void Display_Back_Screen(i16 plan_width, i16 plan_height, i16 w1, i16 h1, i16 w2, i16 h2) {
    i32 y = h1;
    u8 type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
    if (type_scroll == 0 || type_scroll == 2) {
        if (num_world == world_4_image && (num_level == 4 || num_level == 11)) {
            obj_t* rideau = level.objects + rideau_obj_id;
            if (rideau->is_active) {
                if (h1 < rideau->y - 110 - (200 - h2)) {
                    y = rideau->y - 110 - (200 - h2);
                    if (y < 0) {
                        y = 0;
                    }
                }
                if (y >= h2) {
                    y = h2 - 1;
                }
            }
        }
        i32 v68; //?
        if (type_scroll != 0) {
            v68 = plan_height - h2;
        } else if (num_world == world_3_mountain && num_level == 10) {
            v68 = 200 - h2 + (ymap >> 1) + 11;
        } else {
            v68 = Val_Add_Scroll_Y + ymap / Val_Div_Scroll_Y;
        }
        if (v68 > 200 - h2 + (plan_height - 200)) {
            v68 = plan_height - h2;
        }
        for (i32 i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            u8* v59 = bande->source_buffer_pos + bande->field_6 + w1;
            u8* v61 = bande->draw_buffer_pos - 320 * v68 + w1;
            i32 v33 = bande->field_0;
            i32 v34 = bande->field_2 - v68;
            i32 v36 = bande->field_0 + v34;
            if (v34 < 0 && v36 > 0 && bande->field_0 != 0) {
                v34 = 0;
            }
            //stub
        }

    } else {
        // type_scroll == 1
        for (i32 i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            i16 v24_4 = plan_height - bande->field_6 - h1;
            //stub
        }
        for (i32 i = 0; i < NbSprite; ++i) {
            //stub
        }
    }
    //stub
}

//784A8
void Display_Sprite_On_Front(i16 plan_width, i16 plan_height, i16 w1, i16 h1, i16 w2, i16 h2) {
    //stub
}

//787E8
void Calcul_Deplacement_Bande(i16 x, i16 plan_width, i16 plan_height) {
    //stub
}

//78C14
void Init_Effet_Chaleur(i16 width, i16 height, u8* source_buf, u8* dest_buf) {
    //stub
}

//78CA8
void Do_Effet_Chaleur(i16 a1, i16 a2) {
    //stub
}

