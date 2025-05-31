

static inline void set_def_bande(i32 index, i16 field_0, u8 field_2, u8 field_4, u8 field_6) {
    def_bande_t def = {field_0, field_2, field_4, field_6};
    Def_Bande[index] = def;
}

static void init_def_bande(void) {
    Def_Bande = (def_bande_t*)calloc(18000, sizeof(def_bande_t));
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

    i16 cumulative_length = 0;
    for (i32 i = 0; i < NbBande; ++i) {
        def_bande_t* def_bande = Def_Bande + 3000 * (num_world - 1) + 300 * Num_Fond + i;
        bande_t* bande = Bande + i;
        bande->length = def_bande->length;
        bande->field_8 = def_bande->field_2;
        bande->field_9 = def_bande->field_4;
        bande->field_4 = def_bande->field_6;
        bande->field_6 = 0;
        bande->field_14 = 0;
        bande->offset = cumulative_length;
        u8 type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
        if (type_scroll == 0 || type_scroll == 2) {
            bande->source_buffer_pos = source_buf + cumulative_length * width;
            bande->draw_buffer_pos = dest_buf + cumulative_length * 320;
        } else {
            bande->source_buffer_pos = source_buf + cumulative_length;
            bande->draw_buffer_pos = dest_buf + cumulative_length;
        }

        cumulative_length += bande->length;
    }

    NbSprite = Tab_NbSprite[10 * (num_world - 1) + Num_Fond];
    for (i32 i = 0; i < NbSprite; ++i) {
        def_sprite_t* def_sprite = Def_Sprite + 200 * (num_world - 1) + 20 * Num_Fond + i;
        def_sprite_t* sprite = Sprite + i;
        sprite->x = def_sprite->x;
        sprite->y = def_sprite->y;
        sprite->bande_index = def_sprite->bande_index;
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
                if (h1 < rideau->y - 110 - (SCREEN_HEIGHT - h2)) {
                    y = rideau->y - 110 - (SCREEN_HEIGHT - h2);
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
            v68 = SCREEN_HEIGHT - h2 + (ymap >> 1) + 11;
        } else {
            v68 = Val_Add_Scroll_Y + ymap / Val_Div_Scroll_Y;
        }
        if (v68 > SCREEN_HEIGHT - h2 + (plan_height - SCREEN_HEIGHT)) {
            v68 = plan_height - h2;
        }
        for (i32 i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            u8* source_buffer_pos = bande->source_buffer_pos + bande->field_6 + w1;
            u8* dest_buffer_pos = bande->draw_buffer_pos - SCREEN_WIDTH * v68 + w1;
            i32 draw_height = bande->length;
            i32 v34 = bande->offset - v68;
            i32 v36 = bande->length + v34;
            if (v34 < 0 && v36 > 0 && bande->length != 0) {
                draw_height = v36;
                dest_buffer_pos -= SCREEN_WIDTH * v34;
                source_buffer_pos -= plan_width * v34;
                v34 = 0;
            }
            if (y < v36 && v34 < h2) {
                if (y <= v34) {
                    if (h2 < v36) {
                        draw_height = h2 - v34;
                    }
                    Copy_Plan0_To_Buf(source_buffer_pos - BufferNormalDeplt, dest_buffer_pos, plan_width, draw_height, w2 - w1);
                } else {
                    Copy_Plan0_To_Buf(source_buffer_pos + (y - v34) * plan_width - BufferNormalDeplt, dest_buffer_pos + SCREEN_WIDTH * (y - v34), plan_width, draw_height - (y - v34), w2 - w1);
                }
            }
        }
        plan_width /= 2;
        for (i32 i = 0; i < NbSprite; ++i) {
            def_sprite_t* sprite = Sprite + i;
            bande_t* bande = Bande + sprite->bande_index;
            if (bande->length != 0) {
                i32 source_x = sprite->x - bande->field_6;
                if (source_x > SCREEN_WIDTH) {
                    source_x -= plan_width;
                }
                sprite_t* scroll_diff_sprite = ScrollDiffSprites->sprites + (i + 1);
                if (source_x + scroll_diff_sprite->outer_width > w1) {
                    vec2b_t size = {scroll_diff_sprite->outer_width, scroll_diff_sprite->outer_height};
                    DrawSpriteDiffNormal(source_x, sprite->y - v68, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                // Draw a second sprite further to the right if >1 fit on screen
                if (source_x + plan_width < w2) {
                    vec2b_t size = {scroll_diff_sprite->outer_width, scroll_diff_sprite->outer_height};
                    DrawSpriteDiffNormal(source_x + plan_width, sprite->y - v68, size, Scroll_Masque[i + 1],
                                         draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }

    } else {
        // type_scroll == 1
        for (i32 i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            i16 v24_4 = plan_height - bande->field_6 - h1;
            if (bande->offset + bande->length > w1 && bande->offset < w2) {
                if (bande->length != 0) {
                    i32 v67 = bande->length;
                    u8* source_buffer_pos = bande->source_buffer_pos + bande->field_6 * plan_width + h1 * plan_width;
                    u8* dest_buffer_pos = bande->draw_buffer_pos + SCREEN_WIDTH * h1;
                    if (bande->offset + bande->length <= w2) {
                        if (bande->offset < w1) {
                            source_buffer_pos += w1 - bande->offset;
                            dest_buffer_pos += w1 - bande->offset;
                            v67 = bande->length + bande->offset - w1;
                        }
                    } else if (bande->offset >= w1) {
                        v67 = w2 - bande->offset;
                    } else {
                        source_buffer_pos += w1 - bande->offset;
                        dest_buffer_pos += w1 - bande->offset;
                        v67 = w2 - w1;
                    }
                    i32 v14 = h2 - h1;
                    i32 v15 = plan_height - bande->field_6 - h1;
                    if (v15 > v14) {
                        v15 = v14;
                    }
                    i32 v18;
                    if (v15 <= 0) {
                        source_buffer_pos -= plan_width * v15;
                        v18 = v14;
                    } else {
                        Copy_Plan0_To_Buf(source_buffer_pos, dest_buffer_pos + BufferNormalDeplt, plan_width, v15, v67);
                        source_buffer_pos = bande->source_buffer_pos;
                        dest_buffer_pos = bande->draw_buffer_pos + SCREEN_WIDTH * h1 + SCREEN_WIDTH * v15;
                        v18 = v14 - v15;
                    }
                    if (bande->offset < w1) {
                        source_buffer_pos += w1 - bande->offset;
                        dest_buffer_pos += w1 - bande->offset;
                    }
                    if (v14 < v18) {
                        v18 = v14;
                    }
                    if (v18 > 0) {
                        Copy_Plan0_To_Buf(source_buffer_pos, dest_buffer_pos + BufferNormalDeplt, plan_width, v18, v67);
                    }
                }
            }
        }
        for (i32 i = 0; i < NbSprite; ++i) {
            def_sprite_t* sprite = Sprite + i;
            bande_t* bande = Bande + sprite->bande_index;
            if (bande->length != 0) {
                i32 source_y = sprite->y - bande->field_6;
                if (source_y > SCREEN_HEIGHT) {
                    source_y -= plan_height;
                }
                sprite_t* scroll_diff_sprite = ScrollDiffSprites->sprites + (i + 1);
                if (source_y + scroll_diff_sprite->outer_height > h1) {
                    vec2b_t size = {scroll_diff_sprite->outer_width, scroll_diff_sprite->outer_height};
                    DrawSpriteDiffNormal(sprite->x, source_y, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                // Draw a second sprite further to the right if >1 fit on screen
                if (source_y + plan_height < h2) {
                    vec2b_t size = {scroll_diff_sprite->outer_width, scroll_diff_sprite->outer_height};
                    DrawSpriteDiffNormal(sprite->x, source_y + plan_height, size, Scroll_Masque[i + 1],
                                         draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }
    }
}

//784A8
void Display_Sprite_On_Front(i16 plan_width, i16 plan_height, i16 w1, i16 h1, i16 w2, i16 h2) {
    //stub
}

//787E8
void Calcul_Deplacement_Bande(i16 x, i16 plan_width, i16 plan_height) {
    i32 v17 = 0; // y something?
    u8 type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
    if (type_scroll != 0) {
        if (type_scroll == 1) {
            for (i32 i = 0; i < NbBande; ++i) {
                bande_t* bande = Bande + i;
                if (bande->length != 0) {
                    if (bande->field_4 != 0) {
                        if (bande->field_4 == 1 || bande->field_4 == 4) {
                            bande->field_14 = (bande->field_9 + bande->field_14) % (16 * plan_height);
                            if (bande->field_4 != 1) {
                                bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_height;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        } else if (bande->field_4 == 2 || bande->field_4 == 5) {
                            if (bande->field_9 > bande->field_14) {
                                bande->field_14 += 16 * plan_height;
                            }
                            bande->field_14 -= bande->field_9;
                            if (bande->field_4 != 2) {
                                bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_height;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        }
                    } else {
                        bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                    }
                } else {
                    bande->field_6 = (2 * ymap) % plan_height;
                }
            }
        }
    } else {
        // type_scroll == 0
        plan_width /= 2;
        if (NbBande != 0) {
            for (i32 i = 0; i < NbBande; ++i) {
                bande_t* bande = Bande + i;
                if (bande->length != 0) {
                    if (bande->field_4 != 0) {
                        if (bande->field_4 == 1 || bande->field_4 == 4) {
                            bande->field_14 = (bande->field_9 + bande->field_14) % (16 * plan_width);
                            if (bande->field_4 != 1) {
                                bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_width;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        } else if (bande->field_4 == 2 || bande->field_4 == 5) {
                            if (bande->field_9 > bande->field_14) {
                                bande->field_14 += 16 * plan_width;
                            }
                            bande->field_14 -= bande->field_9;
                            if (bande->field_4 != 2) {
                                bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_width;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        }
                    } else {
                        bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                    }
                } else {
                    bande->field_6 = (2 * x) % plan_width;
                }
            }
        }
        if (num_world == world_5_cave && num_level == 10) {
            Do_Effet_Chaleur(plan_width, plan_height);
        }
    }
}

//78C14
void Init_Effet_Chaleur(i16 width, i16 height, u8* source_buf, u8* dest_buf) {
    //stub
}

//78CA8
void Do_Effet_Chaleur(i16 a1, i16 a2) {
    //stub
}

