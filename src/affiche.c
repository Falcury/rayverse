
//18CE8
void display2(obj_t* obj) {
    anim_t* anim = obj->animations + obj->anim_index;
    u16 layers_per_frame = anim->layers_per_frame & 0x3FFF;
    anim_layer_t* layer = anim->layers + (obj->anim_frame * layers_per_frame);
    for (s32 layer_index = 0; layer_index < layers_per_frame; ++layer_index) {
        s32 proj_y = get_proj_y(obj->scale, layer->y + obj->screen_y);
        if (layer->sprite_index != 0) {
            sprite_t* sprite = obj->sprites + layer->sprite_index;
            if (sprite->id != 0) {
                s32 x;
                if (obj->flags.flip_x) {
                    if (obj->scale == 256 && layer_index == 5 && obj->anim_index >= 14 && obj->anim_index <= 16) {
                        x = -16;
                    } else {
                        x = 0;
                    }
                    x += obj->offset_bx * 2 - layer->x - sprite->outer_width + obj->screen_x;
                } else {
                    x = layer->x + obj->screen_x;
                }

                draw_func_t* draw_func = NULL;
                if (obj->flags.flip_x ^ layer->mirrored) {
                    draw_func = DrawSpriteFlipNormalEtX;
                } else {
                    draw_func = DrawSpriteNormalEtX;
                }

                s32 proj_x = get_proj_x(obj->scale, x);
                s32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
                s32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
                vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
                u8 sprite_field_A = sprite->color >> 4;

                u8* image_data = obj->img_buffer + sprite->offset_in_atlas;

                draw_func(proj_x /*eax*/, sprite_field_A /*edx*/, proj_y /*ebx*/, proj_size /*ecx*/, /*edi*/ draw_buffer, image_data /*esi*/);

            }
        }

        ++layer;
    }

}


//18EB0
void display_sprite(obj_t* obj, u8 sprite_index, s16 x, s16 y, u8 flipped) {
    sprite_t* sprite = obj->sprites + sprite_index;
    draw_func_t* draw_func;
    if (((sprite->flags & 4) != 0) == flipped) {
        draw_func = DrawSpriteFlipNormalEtX;
    } else {
        draw_func = DrawSpriteNormalEtX;
    }
    s32 proj_x = get_proj_x(obj->scale, x);
    s32 proj_y = get_proj_y(obj->scale, y);
    s32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
    s32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
    vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
    u8* image_data = obj->img_buffer + sprite->offset_in_atlas;
    draw_func(proj_x, sprite->color, proj_y, proj_size, draw_buffer, image_data);
}

//18F7C
void display_sprite_NoClip(obj_t* obj, u8 sprite_index, s16 x, s16 y, u8 flipped) {
    sprite_t* sprite = obj->sprites + sprite_index;
    draw_func_t* draw_func;
    if (((sprite->flags & 4) != 0) == flipped) {
        draw_func = DrawSpriteFlipNoClipNormalEtX;
    } else {
        draw_func = DrawSpriteNoClipNormalEtX;
    }
    s32 proj_x = get_proj_x(obj->scale, x);
    s32 proj_y = get_proj_y(obj->scale, y);
    s32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
    s32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
    vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
    u8* image_data = obj->img_buffer + sprite->offset_in_atlas;
    draw_func(proj_x, sprite->color, proj_y, proj_size, draw_buffer, image_data);
}

//19048
void DISPLAY_POING(void) {
    if (poing.is_active) {
        display2(poing_obj);
    }
}

//19060
void DISPLAY_CLING(void) {
    /* 15FB8 8013A7B8 -O2 -msoft-float */
    obj_t *c_1up;
    obj_t *c_pow;
    s32 xmin, xmax, ymin, ymax;

    // NOTE: there are some differences here from the PS1 version
    s16 extra_y_offset;
    if (GameModeVideo == MODE_X && P486 == 1) {
        extra_y_offset = 16;
    } else {
        extra_y_offset = 0;
    };

    if (id_Cling_1up != -1) {
        c_1up = &level.objects[id_Cling_1up];
        if (c_1up->timer == 0) {
            c_1up->screen_x = 61 - c_1up->offset_bx;
            c_1up->screen_y = extra_y_offset + 5 - c_1up->offset_hy;
            get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
            sprite_clipping(0, 100, 0, 100);
            display2(c_1up);
            sprite_clipping(xmin, xmax, ymin, ymax);
        }
    }
    if (id_Cling_Pow != -1) {
        c_pow = &level.objects[id_Cling_Pow];
        if (c_pow->timer == 0) {
            c_pow->screen_x = 66 - c_pow->offset_bx;
            c_pow->screen_y = extra_y_offset + 14 - c_pow->offset_hy;
            get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
            sprite_clipping(0, 100, 0, 100);
            display2(c_pow);
            sprite_clipping(xmin, xmax, ymin, ymax);
        }
    }
}

//191A8
void display_bar_boss(obj_t* boss_obj) {
    /* 16080 8013A880 -O2 -msoft-float */
    s16 extra_x_offset, extra_y_offset;
    if (GameModeVideo == MODE_X) {
        extra_x_offset = 8;
        if (P486 == 1) {
            extra_y_offset = 8;
        } else {
            extra_y_offset = 0;
        }
    } else {
        extra_x_offset = 4;
        extra_y_offset = 0;
    };


    u8 hp;
    u8 init_hp;
    s32 unk_1;
    s32 unk_2;
    s32 unk_3;
    s32 disp_mode;

    if (scrollLocked) {
        hp = boss_obj->hit_points;
        if (hp != 0) {
            init_hp = boss_obj->init_hit_points;
            unk_1 = 6;
            unk_2 = (29 - init_hp) * unk_1;
            unk_3 = 20 - unk_2;
            disp_mode = 1;
            display_sprite(poing_obj, 61,
                           extra_x_offset + Bloc_lim_W1 - 4 + unk_3 - (init_hp - hp) * unk_1,
                           (Bloc_lim_H2 - 19 - extra_y_offset),
                           disp_mode);
            display_sprite(poing_obj, 60,
                           extra_x_offset + Bloc_lim_W1 - 4 + 20 - unk_2 + 1,
                           (Bloc_lim_H2 - 20 - extra_y_offset), disp_mode);

            u8 sprite = 0;
            switch (boss_obj->type) {
                case TYPE_MOSKITO:
                case TYPE_MOSKITO2:
                    sprite = 44;
                    break;
                case TYPE_BB1:
                case TYPE_BB12:
                    sprite = 108;
                    break;
                case TYPE_SPACE_MAMA:
                case TYPE_SPACE_MAMA2:
                    sprite = 110;
                    break;
                case TYPE_MAMA_PIRATE:
                    sprite = 113;
                    break;
                case TYPE_SCORPION:
                    sprite = 109;
                    break;
                case TYPE_HYB_BBF2_D:
                case TYPE_HYBRIDE_STOSKO:
                case TYPE_HYBRIDE_MOSAMS:
                case TYPE_DARK:
                    sprite = 112;
                    break;
                case TYPE_SAXO:
                case TYPE_SAXO2:
                    sprite = 111;
                    break;
            }
            s16 display_y = Bloc_lim_H2 - 30 - extra_y_offset;
            sprite = sprite_of_ref(poing_obj, sprite);
            display_sprite(poing_obj, sprite, extra_x_offset + Bloc_lim_W1 - 4, display_y, disp_mode);

        }
    }
}

//193A8
void DisplayCrackers(void) {
    display_text("/limited version/", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0);
    display_text("/install rayman with cdrom/", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 2, 1);
}

//193E4
void DisplayProgrammerMessage(void) {
    display_text("/hi master/", 160, 100, 0, 0);
    display_text("/happy to see you/", 160, 150, 2, 0);
}

//19420
void DISPLAY_FIXE(s32 time) {
    if (ray_mode == MODE_5_CASSE_BRIQUE) {
        if (cb_ball_obj_id != -1) {
            DISPLAY_FIXE_CB(level.objects + cb_ball_obj_id);
        }
    } else {
        DO_DARK2_AFFICHE_TEXT();
        if (dead_time != 64 && !fixontemp) {
            fixontemp = 300;
        }
        if (fixontemp > 0) {
            if (fixontemp == 1 && GameModeVideo == MODE_NORMAL) {
//                draw_buffer = display_buffer; //TODO: extra buffer indirection?
//                ClearBorder(Bloc_lim_H1, Bloc_lim_H2, Bloc_lim_W1, Bloc_lim_W2);
                draw_buffer = DrawBufferNormal;
                ClearBorder(Bloc_lim_H1, Bloc_lim_H2, Bloc_lim_W1, Bloc_lim_W2);
            }
            --fixontemp;
        }
        if (fixon || fixontemp != 0 || time == 0) {
            s16 height = 35;
            s16 width = 77;
            if (GameModeVideo == MODE_NORMAL && (Bloc_lim_W1 > 16 || Bloc_lim_H1 > 5)) {
                if (id_Cling_Old) {
                    height = 45;
                    width = 85;
                }
                if (Bloc_lim_H1 - 5 >= height) {
                    DISPLAY_BLACKBOX(13, 0, width, height, -1, 0);
                } else {
                    DISPLAY_BLACKBOX(13, 0, width, Bloc_lim_H1, -1, 0);
                    DISPLAY_BLACKBOX(13, Bloc_lim_H1, Bloc_lim_W1 - 15, height + 5 - Bloc_lim_H1, -1, 0);
                }
                if (Bloc_lim_H1 - 5 >= 23) {
                    DISPLAY_BLACKBOX(241, 4, 68, 23, -1, 0);
                } else {
                    DISPLAY_BLACKBOX(241, 4, 68, Bloc_lim_H1 - 4, -1, 0);
                    DISPLAY_BLACKBOX(Bloc_lim_W2, Bloc_lim_H1, 312 - Bloc_lim_W2, 28 - Bloc_lim_H1, -1, 0);
                }
            }
            obj_t* obj = level.objects + sbar_obj_id;
            if (time == -2) {
                s32 draw_y = (GameModeVideo == MODE_NORMAL && P486 == 1) ? 16 : 0;
                display_sprite_NoClip(obj, 27, 16, draw_y + 5, 1);
                display_sprite_NoClip(obj, 56, 244, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.lives_digits[0], 55, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.lives_digits[1], 70, draw_y + 5, 1);
                if (ray.hit_points != -1 && !fin_du_jeu) {
                    display_sprite_NoClip(obj, 17 + status_bar.hp_sprites[1], 35, draw_y + 27, 1);
                    if (status_bar.max_hitp == 4) {
                        display_sprite_NoClip(obj, 20 + status_bar.hp_sprites[0], 59, draw_y + 27, 1);
                    }
                }
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[0], 276, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[1], 290, draw_y + 5, 1);
            } else {
                display_sprite_NoClip(obj, 56, Bloc_lim_W1 + 14, Bloc_lim_H2 - 32, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[0], Bloc_lim_W1 + 46, Bloc_lim_H2 - 32, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[1], Bloc_lim_W1 + 60, Bloc_lim_H2 - 32, 1);
            }
            if (id_Cling_1up != -1 || id_Cling_Pow != -1) {
                DISPLAY_CLING();
            }
        }
    }
}

//19864
void DISPLAY_POINT(s16 x, s16 y) {
    DrawWldPointPlan2Normal(PLAN2BIT, x, y - 2);
}

//1987C
void DISPLAY_PTS_TO(s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 a5) {
    //stub
}

//198C4
void DISPLAY_PTS_TO_PLAN2(s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 percent) {
    //TODO: figure out why too many points are being drawn
    percent = MIN(100, percent); // this fixes the too many points issue but is not present in the original code.
    Bresenham(DISPLAY_POINT, origin_x + 8, origin_y, dest_x + 8, dest_y, 7, percent);
}

//198F4
void DISPLAY_CYMBALE(obj_t* obj, u8* draw_buf, u8 a3) {
    /* 179B4 8013C1B4 -O2 -msoft-float */
    u16 begin; u16 end; u16 i;
    sprite_t* sprite;
    s16 x; s16 y;
    anim_t* anim = &obj->animations[obj->anim_index];
    anim_layer_t* layer = &anim->layers[(anim->layers_per_frame & 0x3FFF) * obj->anim_frame];

    if (!a3) {
        end = 5;
        begin = 3;
    } else {
        end = 2;
        begin = 0;
    }

    for (i = begin; i <= end; i++) {
        sprite = &obj->sprites[layer[i].sprite_index];

        y = layer[i].y + obj->screen_y;
        if (sprite->id != 0) {
            x = layer[i].x + obj->screen_x;
            s32 proj_x = get_proj_x(obj->scale, x);
            s32 proj_y = get_proj_y(obj->scale, y);
            s32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
            s32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
            vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
            u8* image_data = obj->img_buffer + sprite->offset_in_atlas;
            DrawSpriteNormalEtX(proj_x, sprite->color, proj_y, proj_size, draw_buffer, image_data);
        }
    }
}

// For debugging: display the object ID above each object
void debug_display_obj_id(obj_t* obj) {
    char text[16];
    snprintf(text, sizeof(text), "%d", obj->id);
//    display_text(text, obj->screen_x + obj->offset_bx, obj->screen_y + obj->offset_hy, 2, 1);
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    GET_ANIM_POS(obj, &spr_x, &spr_y, &spr_w, &spr_h);
    s32 x = spr_x + spr_w / 2;
    s32 y = spr_y + spr_h / 2;
    s32 scr_x = x - xmap;
    s32 scr_y = y - ymap;
    display_text(text, scr_x, scr_y - 20, 2, obj->id == debug_obj_id ? 1 : 0);

    // mouse click to select debug object
    if (debug_clicked) {
        float dx = debug_click_x - (float)scr_x;
        float dy = debug_click_y - (float)scr_y;
        float dist_sq = dx*dx + dy*dy;
        if (dist_sq < debug_click_obj_dist_sq) {
            debug_click_obj_dist_sq = dist_sq;
            debug_obj_id = obj->id;
        }
    }
}

//19A2C
void DISPLAY_ALL_OBJECTS(void) {
    for (s32 current_display_prio = 7; current_display_prio >= 1; --current_display_prio) {
        for (s32 i = 0; i < actobj.num_active_objects; ++i) {
            obj_t* obj = level.objects + actobj.objects[i];
            if (obj->id == debug_obj_id) {
                BREAKPOINT;
            }
            if (current_display_prio == 1 && (flags[obj->type] & flags0_0x80_boss)) {
                display_bar_boss(obj);
            }
            if (obj->display_prio != current_display_prio) {
                continue;
            }
            if (obj->type == TYPE_157_EAU && num_world == world_5_cave && num_level == 8) {
                // Eat at Joe's: water buoys level
                if (obj->x >= 300) {
                    display2(obj);
                    continue;
                }
            } else {
                switch(obj->type) {
                    case TYPE_81_CYMBALE: {
                        DISPLAY_CYMBALE(obj, draw_buffer, 1);
                    } break;
                    case TYPE_254_SLOPEY_PLAT: {
                        //stub
                    } break;
                    case TYPE_252_VAGUE_DERRIERE:
                    case TYPE_251_VAGUE_DEVANT:
                    case TYPE_173_BATEAU: {
                        s32 xmin, xmax, ymin, ymax;
                        get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
                        sprite_clipping(xmin, Bloc_lim_H2 - 40, ymin, ymax);
                        display2(obj);
                        sprite_clipping(xmin, xmax, ymin, ymax);
                    } break;
                    // The cases below are added for debugging (these objects are usually not supposed to be drawn).
                    // In the PS1 version they are also explicitly excluded in DISPLAY_ALL_OBJECTS() but in the
                    // PC version they are excluded from drawing by assigning display priority 0, set in Prio()
                    case TYPE_141_NEIGE:
                    case TYPE_158_PALETTE_SWAPPER:
                    case TYPE_164_GENERATING_DOOR:
                    case TYPE_181_SCROLL_SAX:
                    case TYPE_199_BB1_VIT: {
                        if (is_debug_mode) {
                            display2(obj);
                            debug_display_obj_id(obj);
                        }
                    } break;
                    default: {
                        display2(obj);

                        // Added for debugging
                        if (is_debug_mode) {
                            debug_display_obj_id(obj);
                        }
                    } break;
                }
            }
        }

        if (current_display_prio == 3) {
            if ((gele || (ray.iframes_timer % (2 * display_mode + 2) <= display_mode) || (ray.iframes_timer > 90 && RayEvts.squashed))
                    && ray.flags.alive && ray.is_active) {
                display2(&ray);
                DISPLAY_POING();
            }
        } else if (current_display_prio == 2 && nb_cymbal_in_map != 0) {
            for (s32 j = 0; j < nb_cymbal_in_map; ++j) {
                obj_t* cymbale = level.objects + cymbal_obj_id[j];
                if (cymbale->is_active) {
                    DISPLAY_CYMBALE(cymbale, draw_buffer, 1);
                }
            }
        }
    }

    for (s32 i = 0; i < actobj.num_active_objects; ++i) {
        obj_t* obj = level.objects + actobj.objects[i];
        if (obj->display_prio == 0 && obj->type == TYPE_245_DUNE) {
            display2(obj);
            break;
        }
    }
}

//19D2C
void display_flocons_behind(void) {
    //stub
}

//19F08
void display_flocons_before(void) {
    //stub
}

//1A110
void display_pix_gerbes(void) {
    /* 17740 8013BF40 -O2 -msoft-float */
    u8 color;
    for (s32 i = 0; i < LEN(pix_gerbe); ++i) {
        if (pix_gerbe[i].is_active) {
            pix_gerbe_item_t* cur_item = &pix_gerbe[i].items[0];
            for (s32 j = 0; j < LEN(pix_gerbe[i].items); ++j) {
                u8 unk_1 = cur_item->unk_1;
                if (unk_1 > 127 && cur_item->y_pos > 0) {
                    s16 spd_y = (u8) cur_item->speed_y; /* using abs on android? */
                    if (spd_y > 127)
                        color = 88;
                    else
                        color = (unk_1 & 127) + ((spd_y >> 5) + 4);

                    fplotNormalETX(draw_buffer, cur_item->x_pos >> 6, cur_item->y_pos >> 6, color);
                }
                cur_item++;
            }
        }
    }
}

//1A1C4
void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, s16 font_size, u8 is_fond) {
    s32 clip_height = height;
    s32 clip_width = width;
    s32 clip_x = x;
    s32 clip_y = y;
    if (x < 0) {
        clip_x = 0;
        clip_width = x + width;
    } else if (x > 320) {
        clip_x = 320;
    }
    if (clip_width + clip_x > 320) {
        clip_width = 320 - clip_x;
    }
    if (y < 0) {
        clip_y = 0;
        clip_height = y + height;
    } else if (y > 200) {
        clip_y = 200;
    }
    if (clip_height + clip_y > 200) {
        clip_height = 200 - clip_y;
    }

    if (clip_width > 0 && clip_height > 0) {
        if (font_size == -1) {
            if (ModeVideoActuel == MODE_X) {
                //draw_borderbox();
            } else {
                if (is_fond == 0) {
                    DrawBlackBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width);
                } else {
                    DrawFondBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, is_fond);
                }
            }
        }
        if (is_fond == 1) {
            if (ModeVideoActuel != MODE_X) {
                DrawBorderBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, 0x2026);
            }
        } else if (is_fond == 2) {
            if (ModeVideoActuel != MODE_X) {
                DrawBorderBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, 0x2020);
            } else {
                //draw_borderbox();
            }
        }
    }
}

//1A3F0
void display_text(const char* text, s16 x, s16 y, u8 font_size, s8 color) {
    s32 line_height = 0;
    s16 char_spacing = 0;
    s16 space_width = 0;
    s32 num_let = 0;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }

    s32 current_x = x;

    sprite_t* sprite = NULL;

    if (text) {
        for (s32 i = 0; i < strlen(text); ++i) {
            const char* pos = text + i;
            char c = *pos;
            if (c == '/') {
                current_x = x - calc_largmax_text(text, i, space_width, char_spacing, font_size) / 2;
                if (i > 1) {
                    y += line_height; // next row
                }
                num_let = 0;
            } else if (c == ' ') {
                num_let = 0;
                current_x += space_width;
            } else {
                num_let = deter_num_let(c, pos + 1);
            }
            if (num_let != 0) {
                s32 let_width = calc_let_Width(font_size, num_let);
                if (num_let < 1000) {
                    if (font_size <= 1) {
                        if (font_size == 1) {
                            num_let += 41;
                        }
                        ASSERT(num_let < alpha2_sprite_count);
                        if (num_let < alpha2_sprite_count) { // bounds check added
                            sprite = alpha2->sprites + num_let;
                            vec2b_t size = {let_width, sprite->outer_height};
                            DrawSpriteColorNormalEtX(current_x, color, y - sprite->outer_height, size, draw_buffer, alpha2->img_buffer + sprite->offset_in_atlas);
                        }

                    } else if (font_size == 2) {
                        ASSERT(num_let < alpha_sprite_count);
                        if (num_let < alpha_sprite_count) { // bounds check added
                            sprite = alpha->sprites + num_let;
                            vec2b_t size = {let_width, sprite->outer_height};
                            DrawSpriteColorNormalEtX(current_x, color, y - sprite->outer_height, size, draw_buffer, alpha->img_buffer + sprite->offset_in_atlas);
                        }
                    }
                } else {
                    sprite = alpha_numbers->sprites + (num_let - 1000);
                    vec2b_t size = {let_width, sprite->outer_height};
                    DrawSpriteColorNormalEtX(current_x, color, y - sprite->outer_height, size, draw_buffer, alpha_numbers->img_buffer + sprite->offset_in_atlas);
                    i += 3;
                }
                current_x += (sprite->sprite_pos & 0xF) + sprite->inner_width - char_spacing;
            }

        }
    }
}

//1A68C
void display_deform_text(const char* text, s16 x, s16 y, u8 font_size, s8 color, s16 rotation, s16 enx, s16 eny) {
    display_text(text, x, y, font_size, color); // TODO: implement deform effect
    //stub
}

//1A8DC
void display_text_sin(const char* text, s16 x, s16 y, u8 font_size, s8 color, u8 a6) {
    //stub
}

//1ABE0
void display_box_text(display_item_t* box) {
    s16 blackbox_x = (s16)(box->centered_x_pos - 3);
    s16 blackbox_y = (s16)(box->centered_y_pos - 3);
    s16 blackbox_width = (s16)(box->width + 6);
    s16 blackbox_height = (s16)(box->height + 6);
    if (box->field_D5) {
        DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, -1, box->is_fond);
        if (box->is_fond == 1) {
            display_text(box->text, box->xpos - 1, box->ypos - 1, box->font_size, box->color);
        } else if (box->is_fond == 2) {
            display_text(box->text, box->xpos + 1, box->ypos + 1, box->font_size, box->color);
        } else {
            display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
        }
    } else {
        if (box->is_fond != 0) {
            DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, -2, box->is_fond);
        }
        display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
    }
}

//1AD00
void CLRSCR(void) {
    if (ModeVideoActuel == MODE_X) {
        display_emptypicture();
    } else {
        memset(DrawBufferNormal, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
}

//1AD38
void display_etoile(s16 in_x, s16 in_y) {
    /* 19A6C 8013E26C -O2 */
    u8 loc_star_spr[8] = {21, 22, 23, 24, 21, 25, 22, 21};

    grp_star_t* temp = &grp_stars[current_star];
    grp_star_t* star = temp;
    star->timer++;
    if (star->timer > star->length)
    {
        u8 rand_res = myRand(7);
        star->timer = 0;
        star->length = myRand(5);
        star->dist = rand_res;
        star->sprite_table_index = rand_res;
    }
    s16 draw_x = star->dist + (in_x - xmap) - 8;
    s16 draw_y = star->dist + (in_y - ymap);
    if (draw_x > 0 && SCREEN_WIDTH - draw_x > 0 && draw_y > 0 && SCREEN_HEIGHT - draw_y > 0) {
        sprite_t* sprite = poing_obj->sprites + loc_star_spr[star->sprite_table_index];
        vec2b_t proj_size = {(u8)sprite->outer_width, (u8)sprite->outer_height};
        DrawSpriteNormalEtX(draw_x, sprite->color >> 4, draw_y, proj_size, draw_buffer, poing_obj->img_buffer + sprite->offset_in_atlas);
    }

    if (current_star < COUNT(grp_stars)) {
        current_star++;
    }
}

//1AE50
void display_grp_stars(void) {
    obj_t* grapped;
    s16 x; s16 y; s16 w; s16 h;
    s16 cen_x;
    s16 grapped_x;

    if (ray.main_etat == 7 && ray.flags.alive) {
        current_star = 0;
        grapped = &level.objects[id_obj_grapped];
        GET_SPRITE_POS(&ray, 1, &x, &y, &w, &h);

        cen_x = x + (w >> 1);
        grapped_x = grapped->offset_bx + grapped->x;
        Bresenham(
                display_etoile,
                cen_x, (s16) (y + (h >> 1) - 6),
                grapped_x, (s16) (grapped->offset_hy + grapped->y),
                5,
                100 // NOTE: 128 in PC version?
        );
    }
}

//1AF1C
void DISPLAY_TEXT_FEE(void) {
    /* 19D20 8013E520 -O2 -msoft-float */
    u8 txt_fee;
    obj_t* obj;
    display_item_t ttd;
    s16 cen_x;
    s16 obj_x;
    s16 marg_x;

    // TODO: fix black bars not working on Betilla levels
    s16 black_bar_height = ((SCREEN_HEIGHT * 2) / (45 * 2)) * 10 ; // PS1: 50   PC/mobile: 40
    DISPLAY_BLACKBOX(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 12, -1, false);
    DISPLAY_BLACKBOX(0, SCREEN_HEIGHT - black_bar_height, SCREEN_WIDTH, black_bar_height, -1, false);
    txt_fee = display_txt_fee;
    if (txt_fee != 255)
    {
        obj = &level.objects[png_or_fee_id];
        memcpy(&ttd, &text_to_display[txt_fee], sizeof(ttd));
        if (ttd.text[0] != '\0')
        {
            if (obj->flags.alive)
            {
                s16 y_fee = (SCREEN_HEIGHT < 240) ? SCREEN_HEIGHT - 10 : SCREEN_HEIGHT - 20; // added to account for PC/PS1 difference
                obj->screen_y = y_fee - obj->offset_by;
                cen_x = ttd.centered_x_pos;
                obj_x = obj->offset_bx + obj->screen_x;
                marg_x = 8;
                if (cen_x < obj_x - marg_x)
                {
                    if (cen_x + ttd.width > obj_x)
                    {
                        ttd.text[(s16) ((obj_x - (cen_x + marg_x)) / 7)] = '\0';
                        display_box_text(&ttd);
                    }
                    else
                        display_box_text(&ttd);
                }
                else if (cen_x + ttd.width <= obj_x)
                    display_box_text(&ttd);

                DISPLAY_BLACKBOX(obj_x, obj->screen_y + obj->offset_by - 20, 30, 18, -1, 0);
                display2(obj);
            }
            else
                display_box_text(&ttd);
        }
    }

    txt_fee = old_txt_fee;
    if (txt_fee != 255) {
        memcpy(&ttd, &text_to_display[txt_fee], sizeof(ttd));
        if (ttd.text[0] != '\0')
            display_box_text(&ttd);
    }
}

//1B0E0
void DISPLAY_SAVE_SPRITES(s16 x, s16 save_index) {
    s16 y = save_index * (ecarty + 23) + debut_options - 23;
    loadinforay_t* loadinfo = LoadInfoRay + save_index;
    s32 sprite_index = 27;
    display_sprite(&div_obj, 27, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;

    sprite_index = (loadinfo->lives / 10) % 10 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;

    sprite_index = loadinfo->lives % 10 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    // The original code always uses sprite 28 here (I assume this doesn't matter as all the digits are the same width)?
    x += div_obj.sprites[28].outer_width + 10;

    sprite_index = 57;
    display_sprite(&mapobj[0], sprite_index, x, y, 1);
    x += mapobj[0].sprites[sprite_index].outer_width;

    sprite_index = loadinfo->continues % 10 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    x += div_obj.sprites[37].outer_width + 10;

    sprite_index = loadinfo->cages / 100 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;

    sprite_index = (loadinfo->cages / 10) % 10 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;

    sprite_index = loadinfo->cages % 10 + 28;
    display_sprite(&div_obj, sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;

    display_text("%", x, debut_options + save_index * (ecarty + 23), 1, 2);
}

//1B424
void DISPLAY_SAVE_POING(void) {
    display_sprite(&mapobj[0], 2, 10, (ecarty + 23) * (positiony - 1) + debut_options - 23, 0);
    if (fichier_a_copier) {
        display_sprite(&mapobj[0], 1, 10, (ecarty + 23) * (fichier_a_copier - 1) + debut_options - 23, 0);
    }
}

//1B4AC
void DISPLAY_OPTIONS_POING(void) {
    print_once("Not implemented: DISPLAY_OPTIONS_POING"); //stub
}

//1B53C
void DISPLAY_YESNO_POING(void) {
    print_once("Not implemented: DISPLAY_YESNO_POING"); //stub
}

//1B56C
void display_time(s32 a1) {
    print_once("Not implemented: display_time"); //stub
}

//1B79C
void DISPLAY_CONTINUE_SPR(void) {
    print_once("Not implemented: DISPLAY_CONTINUE_SPR"); //stub
}

//1B944
void AFFICHE_SPRITE_DEFORME(void* a1, s32 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7, s16 a8, s16 a9, s16 a10) {
    print_once("Not implemented: AFFICHE_SPRITE_DEFORME"); //stub
}

//1BBC4
void DISPLAY_DEFORMATION(obj_t* obj, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: DISPLAY_DEFORMATION"); //stub
}

//1BEA8
void DISPLAY_DEFORM_SPRITE(void* a1, s32 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    print_once("Not implemented: DISPLAY_DEFORM_SPRITE"); //stub
}

//1C030
void CALCUL_DEFORMATION(s16* a1, s16* a2, s16 a3, s16 a4, s16 a5) {
    print_once("Not implemented: CALCUL_DEFORMATION"); //stub
}

