
//18CE8
void display2(obj_t* obj) {
    anim_t* anim = obj->animations + obj->anim_index;
    u16 layers_per_frame = anim->layers_per_frame & 0x3FFF;
    anim_layer_t* layer = anim->layers + (obj->anim_frame * layers_per_frame);
    for (i32 layer_index = 0; layer_index < layers_per_frame; ++layer_index) {
        i32 proj_y = get_proj_y(obj->scale, layer->y + obj->screen_y);
        if (layer->sprite_index != 0) {
            sprite_t* sprite = obj->sprites + layer->sprite_index;
            if (sprite->unk_index != 0) {
                i32 x;
                if (obj->flags & obj_flags_8_flipped) {
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
                if ((obj->flags & obj_flags_8_flipped) ^ layer->mirrored) {
                    draw_func = DrawSpriteFlipNormalEtX;
                } else {
                    draw_func = DrawSpriteNormalEtX;
                }

                i32 proj_x = get_proj_x(obj->scale, x);
                i32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
                i32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
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
void display_sprite(obj_t* obj, u8 sprite_index, i16 x, i16 y, u8 flipped) {
    sprite_t* sprite = obj->sprites + sprite_index;
    draw_func_t* draw_func;
    if (((sprite->flags & 4) != 0) == flipped) {
        draw_func = DrawSpriteFlipNormalEtX;
    } else {
        draw_func = DrawSpriteNormalEtX;
    }
    i32 proj_x = get_proj_x(obj->scale, x);
    i32 proj_y = get_proj_y(obj->scale, y);
    i32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
    i32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
    vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
    u8* image_data = obj->img_buffer + sprite->offset_in_atlas;
    draw_func(proj_x, sprite->color, proj_y, proj_size, draw_buffer, image_data);
}

//18F7C
void display_sprite_NoClip(obj_t* obj, u8 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//19048
void DISPLAY_POING(void) {
    if (poing.is_active) {
        display2(poing_obj);
    }
}

//19060
void DISPLAY_CLING(obj_t* obj) {
    //stub
}

//191A8
void display_bar_boss(obj_t* obj) {
    //stub
}

//193A8
void DisplayCrackers(void) {
    //stub
}

//193E4
void DisplayProgrammerMessage(void) {
    //stub
}

//19420
void DISPLAY_FIXE(i32 a1) {
    //stub
}

//19864
void DISPLAY_POINT(i16 x, i16 y) {
    DrawWldPointPlan2Normal(PLAN2BIT, x, y - 2);
}

//1987C
void DISPLAY_PTS_TO(i16 origin_x, i16 origin_y, i16 dest_x, i16 dest_y, i16 a5) {
    //stub
}

//198C4
void DISPLAY_PTS_TO_PLAN2(i16 origin_x, i16 origin_y, i16 dest_x, i16 dest_y, i16 percent) {
    Bresenham(DISPLAY_POINT, origin_x + 8, origin_y, dest_x + 8, dest_y, 7, percent);
}

//198F4
void DISPLAY_CYMBALE(obj_t* obj) {
    //stub
}

//19A2C
void DISPLAY_ALL_OBJECTS(void) {
    //stub
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
    //stub
}

//1A1C4
void DISPLAY_BLACKBOX(i16 x, i16 y, i16 width, i16 height, i16 font_size, i8 color) {
    i32 clip_height = height;
    i32 clip_width = width;
    i32 clip_x = x;
    i32 clip_y = y;
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
            if (ModeVideoActuel == 1) {
                //stub
            } else {
                if (color == 0) {
                    DrawBlackBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width);
                } else {
                    DrawFondBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width);
                }
            }
        }
    }

    //stub
}

//1A3F0
void display_text(const char* text, i16 x, i16 y, u8 font_size, i8 color) {
    i32 line_height = 0;
    i16 char_spacing = 0;
    i16 space_width = 0;
    i32 num_let = 0;

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

    i32 current_x = x;

    sprite_t* sprite = NULL;

    if (text) {
        for (i32 i = 0; i < strlen(text); ++i) {
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
                i32 let_width = calc_let_Width(font_size, num_let);
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
                current_x += (sprite->field_9 & 0xF) + sprite->inner_width - char_spacing;
            }

        }
    }
}

//1A68C
void display_deform_text(const char* text, i16 x, i16 y, u8 font_size, i8 color, i16 a6, i16 a7, i16 a8) {
    //stub
}

//1A8DC
void display_text_sin(const char* text, i16 x, i16 y, u8 font_size, i8 color, u8 a6) {
    //stub
}

//1ABE0
void display_box_text(display_item_t* box) {
    i16 blackbox_x = (i16)(box->centered_x_pos - 3);
    i16 blackbox_y = (i16)(box->centered_y_pos - 3);
    i16 blackbox_width = (i16)(box->width + 6);
    i16 blackbox_height = (i16)(box->height + 6);
    if (box->field_D5) {
        DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, -1, box->color);
        if (box->is_fond == 1) {
            display_text(box->text, box->xpos - 1, box->ypos - 1, box->font_size, box->color);
        } else if (box->is_fond == 2) {
            display_text(box->text, box->xpos + 1, box->ypos + 1, box->font_size, box->color);
        } else {
            display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
        }
    } else {
        if (box->is_fond != 0) {
            DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, -2, box->color);
        }
        display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
    }
}

//1AD00
void CLRSCR(void) {
    //stub
}

//1AD38
void display_etoile(i16 a1, i16 a2) {
    //stub
}

//1AE50
void display_grp_stars(void) {
    //stub
}

//1AF1C
void DISPLAY_TEXT_FEE(void) {
    //stub
}

//1B0E0
void DISPLAY_SAVE_SPRITES(i16 x, i16 save_index) {
    i16 y = save_index * (ecarty + 23) + debut_options - 23;
    loadinforay_t* loadinfo = LoadInfoRay + save_index;
    i32 sprite_index = 27;
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
    //stub
}

//1B53C
void DISPLAY_YESNO_POING(void) {
    //stub
}

//1B56C
void display_time(i32 a1) {
    //stub
}

//1B79C
void DISPLAY_CONTINUE_SPR(void) {
    //stub
}

//1B944
void AFFICHE_SPRITE_DEFORME(void* a1, i32 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7, i16 a8, i16 a9, i16 a10) {
    //stub
}

//1BBC4
void DISPLAY_DEFORMATION(obj_t* obj, i16 a2, i16 a3, i16 a4) {
    //stub
}

//1BEA8
void DISPLAY_DEFORM_SPRITE(void* a1, i32 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    //stub
}

//1C030
void CALCUL_DEFORMATION(i16* a1, i16* a2, i16 a3, i16 a4, i16 a5) {
    //stub
}

