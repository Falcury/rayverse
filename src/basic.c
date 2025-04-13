
//1D074
bool EOA(obj_t* obj) {
    eta_t* eta = get_eta(obj);
    bool on_last_frame;
    if (eta->flags & eta_flags_0x10_anim_reverse) {
        on_last_frame = (obj->anim_frame == 0);
    } else {
        anim_t* anim = obj->animations + obj->anim_index;
        on_last_frame = (obj->anim_frame == anim->frame_count - 1);
    }
    if (on_last_frame && horloge[eta->anim_speed & 15] == 0) {
        return true;
    } else {
        return false;
    }
}

//1D0EC
void save_objects_flags(void) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        set2bits((u32*) &saveobj[i >> 4], i & 0xF, 2 * (obj->is_active & 1) + obj->flags.alive);
    }
}

//1D15C
void restore_objects_flags(void) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* obj = level.objects + i;
        if (obj->type != TYPE_58_CAGE) {
            u32 high_bit = 0;
            u32 low_bit = 0;
            read2bits((u32*) &saveobj[i >> 4], i & 0xF, &high_bit, &low_bit);
            obj->is_active = high_bit;
            obj->flags.alive = low_bit;
        }
    }
}

//1D204
void snapToSprite(obj_t* obj1, obj_t* obj2, u8 a3, i16 a4, i16 a5) {
    //stub
}

//1D4C4
void findfirstObject(i16 a1) {
    //stub
}

//1D4F4
void findfirstInactiveObject(i16 a1) {
    //stub
}

//1D528
void sinYspeed(obj_t* obj, i16 a2, i16 a3, i16* a4) {
    //stub
}

//1D560
void set_proj_center(i16 x, i16 y) {
    PROJ_CENTER_X = x;
    PROJ_CENTER_Y = y;
}

//1D570
i32 get_proj_dist(i16 scale, i16 outer_dim) {
    //NOTE: needs checking
    if (zoom_mode == 0) {
        return outer_dim;
    } else {
        i32 temp = ((256*256) / (scale + 256)) * (outer_dim);
        return (temp / 256);
    }
}

//1D5B0
i32 get_proj_dist2(i16 scale, i16 a2) {
    return (a2 * ((256*256) / (scale + 256))) >> 8;
}

//1D5D8
i32 get_proj_x(i16 scale, i16 a2) {
    //NOTE: needs checking
    i32 temp = ((256*256) / (scale + 256)) * (a2 - PROJ_CENTER_X);
    return ((temp / 256) + PROJ_CENTER_X);
}

//1D614
i32 get_proj_y(i16 scale, i16 a2) {
    //NOTE: needs checking
    i32 temp = ((256*256) / (scale + 256)) * (a2 - PROJ_CENTER_Y);
    return ((temp / 256) + PROJ_CENTER_Y);
}

//1D650
void set_zoom_mode(u8 mode) {
    zoom_mode = mode;
}

//1D658
i32 inverse_proj_x(i16 scale, i16 a2) {
    return PROJ_CENTER_X + ((a2 - PROJ_CENTER_X) << 8) / ((256*256) / (scale + 256));
}

//1D690
i32 inverse_proj_y(i16 scale, i16 a2) {
    return PROJ_CENTER_Y + ((a2 - PROJ_CENTER_Y) << 8) / ((256*256) / (scale + 256));
}

//1D6C8
i32 vblToEOA(obj_t* obj, u8 a2) {
    eta_t* eta = get_eta(obj);
    i32 anim_speed = eta->anim_speed & 15;
    i32 result = obj->animations[eta->anim_index].frame_count * anim_speed * a2 - horloge[anim_speed] + 1;
    return (result <= 0) ? 1 : result;
}

//1D738
void GET_ANIM_POS(obj_t* obj, i16* x, i16* y, i16* w, i16* h) {
    anim_t* anim = obj->animations + obj->anim_index;
    anim_frame_t* frame = anim->frames + obj->anim_frame;
    *w = frame->width;
    *h = frame->height;
    if (obj->flags.flip_x) {
        *x = obj->x + 2 * obj->offset_bx - frame->x - *w;
    } else {
        *x = obj->x + frame->x;
    }
    *y = obj->y + frame->y;
}

//1D7D8
void add_actobj(i16 a1) {
    //stub
}

// 1D808
void set_sub_etat(obj_t* obj, u8 sub_etat) {
    obj->change_anim_mode = 1;
    obj->sub_etat = sub_etat;
}

// 1D810
void set_main_etat(obj_t* obj, u8 etat) {
    obj->change_anim_mode = 1;
    obj->main_etat = etat;
}

// 1D818
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat) {
    obj->change_anim_mode = 1;
    obj->main_etat = etat;
    obj->sub_etat = sub_etat;
}

//1D824
void get_center_x(obj_t* obj) {
    //stub
}

//1D954
void get_center_y(obj_t* obj) {
    //stub
}

//1DAC8
void on_block_chdir(obj_t* obj, i16 center_x, i16 center_y) {
    //stub
}

//1DB04
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, i32 a2, i32 a3, i16 a4) {
    //stub
}

//1DBB4
void GET_SPRITE_POS(obj_t* obj, i16 a2, i16* a3, i16* a4, i16* a5, i16* a6) {
    //stub
}

//1DCFC
void GET_RAY_ZDC(obj_t* obj, i16* a2, i16* a4, i16* a5) {
    //stub
}

//1DEC0
void GET_BB1_ZDCs(obj_t* obj, i16* a2, i16* a3, i16* a4, i16* a5, i16* a6, i16* a7, i16* a8, i16* a9) {
    //stub
}

//1DFB8
void calc_obj_dir(obj_t* obj) {
    if (ray.x + ray.offset_bx > obj->x + obj->offset_bx) {
        obj->flags.flip_x = true;
    } else {
        obj->flags.flip_x = false;
    }
}

//1DFF4
u8 OBJ_IN_ZONE(obj_t* obj) {
    switch(obj->type) {
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
        case TYPE_LIDOLPINK:
        case TYPE_LIDOLPINK2:
            return obj->detect_zone_flag == 1;
        default:
            return obj->detect_zone_flag != 0;
    }
}

//1E030
void calc_obj_pos(obj_t* obj) {
    obj->screen_y = obj->y - ymap;
    obj->screen_x = obj->x - xmap + 8;
}

//1E05C
void makeUturn(obj_t* obj) {
    //stub
}

//1E588
u8 BTYP(i32 tile_x, i32 tile_y) {
    if (tile_x >= 0 && tile_x <= (mp.width - 1) && tile_y >= 0 && tile_y <= (mp.height - 1)) {
        return mp.map[tile_y * mp.width + tile_x].tile_type;
    } else {
        return BTYP_NONE;
    }
}

//1E608
void calc_btyp_square(obj_t* obj) {
    i16 x_offset;
    if (obj->type == TYPE_23_RAYMAN) {
        x_offset = RayEvts.tiny ? 4 : 8;
    } else {
        x_offset = 16;
    }

    i16 x = obj->x + obj->offset_bx;
    i16 y = obj->y + obj->offset_by;
    i16 tile_x = ashr16(y, 4);
    i16 tile_y = ashr16(x, 4);
    i16 tile_x_left = ashr16(x - x_offset, 4);
    i16 tile_x_right = ashr16(x + x_offset, 4);

    obj->coll_btype[3] = BTYP(tile_x, tile_y - 1); // up
    obj->coll_btype[1] = BTYP(tile_x_left, tile_y); // left

    // center
    if (obj->main_etat == 2) {
        obj->coll_btype[0] = bloc_floor(BTYP(tile_x, tile_y), x % 16, y % 16);
    } else {
        obj->coll_btype[0] = BTYP(tile_x, tile_y);
    }

    obj->coll_btype[2] = BTYP(tile_x_right, tile_y); // right
    obj->coll_btype[4] = BTYP(tile_x, tile_y + 1); // down
}

//1E76C
void DO_OBJ_REBOND_EN_X(obj_t* obj) {
    //stub
}

//1E790
u8 calc_btyp(obj_t* obj) {
    u8 btyp = BTYP_NONE;
    calc_btyp_square(obj);
    i32 x = obj->x + obj->offset_bx;
    i32 y = obj->y + obj->offset_by;

    if (obj->type == TYPE_RAYMAN) {
        ray.cmd_arg_1 = -1;
        switch (ray.coll_btype[0]) {
            case BTYP_NONE:
            case BTYP_CHDIR:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SOLID_LEFT_45:
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_LIANE:
            case BTYP_SOLID_PASSTHROUGH:
            case BTYP_SOLID:
                ray_last_ground_btyp = 1;
                break;
            case BTYP_SLIPPERY_RIGHT_45:
            case BTYP_SLIPPERY_LEFT_45:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
            case BTYP_SLIPPERY:
                ray_last_ground_btyp = 0;
                break;
        }
    }

    if (!(block_flags[obj->coll_btype[0]] & 2)) {
        if (obj->type == TYPE_23_RAYMAN) {
            btyp = mp.map[ray.ray_dist].tile_type;
        } else {
            btyp = BTYP(x >> 4, y >> 4);
        }

        if (obj->main_etat == 2 && !(block_flags[btyp] & 2)) {
            btyp = BTYP(x >> 4, (y + 16) >> 4);
        }

        if (!(block_flags[btyp] & 2)) {
            u8 left_and_right_solid = (block_flags[obj->coll_btype[2]] & 2) + ((block_flags[obj->coll_btype[1]] & 2) != 0);
            // left = 1, right = 2, both = 3
            if (left_and_right_solid == 3) {
                // decide whether the closest solid tile is to the left or right?
                if ((x % 16) < 8) {
                    left_and_right_solid = 1;
                } else {
                    left_and_right_solid = 2;
                }
            }

            if (left_and_right_solid != 0) {
                u8 left_or_right = left_and_right_solid - 1; // now 0 = left, 1 = right
                if (obj->type == TYPE_23_RAYMAN) {
                    if (ray_last_ground_btyp == 1) {
                        i32 ground_x = left_or_right ? x + 16 : x - 16;
                        if (!(block_flags[BTYP(ground_x >> 4, y >> 6)] & 2)) {
                            if (ray.main_etat != 2) {
                                obj->cmd_arg_1 = (left_or_right == ray.flags.flip_x);
                            }
                            ray.coll_btype[0] = ray.coll_btype[1 + (left_or_right != 0)];
                        }
                    }
                } else if (left_or_right != ray.flags.flip_x && obj->main_etat != 2) {
                    if (
                            (obj->type == TYPE_BADGUY1 && obj->flags.flip_x) ||
                            obj->type == TYPE_BADGUY2 || obj->type == TYPE_BADGUY3 || obj->type == TYPE_GENEBADGUY ||
                            obj->type == TYPE_STONEMAN1 || obj->type == TYPE_STONEMAN2 ||
                            obj->type == TYPE_BIG_CLOWN || obj->type == TYPE_WAT_CLOWN ||
                            (obj->type == TYPE_SPIDER && obj->cmd_arg_1 != 0) ||
                            obj->type == TYPE_TROMPETTE || obj->type == TYPE_MITE || obj->type == TYPE_MITE2 ||
                            obj->type == TYPE_CAISSE_CLAIRE || obj->type == TYPE_WALK_NOTE_1 || obj->type == TYPE_SPIDER_PLAFOND ||
                            (obj->type == TYPE_BLACKTOON1 &&
                             (obj->follow_sprite == 1 || obj->follow_sprite == 4 || (obj->follow_sprite == 7 && obj->configuration == 2)) &&
                             !(obj->main_etat == 0 && obj->sub_etat == 4)
                            )
                    ) {
                        makeUturn(obj);
                        obj->coll_btype[0] = BTYP_SOLID;
                    }
                }
            } else {
                btyp = obj->coll_btype[0];
            }
        }
    } else {
        btyp = obj->coll_btype[0];
    }
    return btyp;
}

//1EAA8
void init_obj_in_the_air(obj_t* obj) {
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
}

//1EAB4
void make_my_fruit_go_down(obj_t* obj, i16 a2) {
    //stub
}

//1EB18
void switchOff(obj_t* obj) {
    if (EOA(obj)) {
        obj->flags.alive = 0;
        obj->is_active = 0;
        if (!(ray_mode == 5 && obj->type == TYPE_161_WIZ)) {
            del_alwobj(obj->id);
        }
        if (obj->type == TYPE_75_PAILLETTE || obj->type == TYPE_170_RAYON) {
            obj->x = -32000;
            obj->y = -32000;
        }
    }
}

//1EB80
void obj_hurt(obj_t* target) {
    if (poing.damage > target->hit_points) {
        target->hit_points = 0;
    } else {
        target->hit_points -= poing.damage;
    }
}

//1EBA0
void Projectil_to_RM(obj_t* obj, i16* a2, i16* a3, i16 a4, i16 a5) {
    //stub
}

//1EE18
void del_actobj(obj_t* obj) {
    //stub
}

//1EEB8
i32 calc_let_Width(u8 font_size, i32 num_let) {
    obj_t* obj = NULL;
    sprite_t* sprite;
    if (num_let > 1000) {
        sprite = alpha_numbers->sprites + (num_let - 1000);
    } else {
        switch (font_size) {
            case 0:
                sprite = alpha2->sprites + num_let;
                break;
            case 1:
                sprite = alpha2->sprites + num_let + 41;
                break;
            case 2:
                sprite = alpha->sprites + num_let;
                break;
            default:
                // NOTE: possible bug here with an uninitialized pointer in the PC version (?)
                return 0;
        }
    }
    if (sprite) {
        return sprite->outer_width;
    } else {
        return 0;
    }
}

//1EF1C
i32 calc_let_Width2(u8 font_size, i32 num_let) {
    obj_t* obj = NULL;
    sprite_t* sprite;
    if (num_let > 1000) {
        sprite = alpha_numbers->sprites + (num_let - 1000);
    } else {
        if (font_size <= 1) {
            if (font_size == 1) {
                num_let += 41;
            }
            sprite = alpha2->sprites + num_let;
        } else {
            if (font_size != 2) {
                return 0; // NOTE: possible bug here with an uninitialized pointer in the PC version (?)
            }
            sprite = alpha->sprites + num_let;
        }
    }
    if (sprite) {
        i32 width = (sprite->field_9 & 0xF) + sprite->inner_width;
        return width;
    } else {
        return 0;
    }
}

//1EF80
void INIT_TEXT_TO_DISPLAY(void) {
    for (i32 i = 0; i < COUNT(text_to_display); ++i) {
        text_to_display[i].text[0] = '\0';
        text_to_display[i].is_fond = 0;
        text_to_display[i].field_D5 = 0;
    }
}

//1EFB4
i32 deter_num_let(u8 c, const char* next_chars) {
    if (c == ']') {
        char next_text[4] = {0};
        strncpy(next_text, next_chars, 3);
        return atoi(next_text) + 1000;
    }
    if (c >= 'a' && c <= 'z') {
        return c - 93;
    } else if (c >= '0' && c <= '9') {
        return c - 18;
    } else if (c == '?') {
        return 1;
    } else if (c == '|') {
        return 49;
    } else if (c == '~') {
        return 46;
    } else if (c == 129) {
        return 44;
    } else if (c == 148) {
        return 43;
    } else if (c == 248) {
        return 45;
    } else if (c == 156) {
        return 42;
    } else if (c == '#') {
        return 47;
    } else if (c == '%') {
        return 45;
    } else if (c == '!') {
        return 2;
    } else if (c == '\'') {
        return 41;
    } else if (c == '*') {
        return 48;
    } else if (c == '.') {
        return 3;
    } else if (c == ':') {
        return 40;
    }
    return 0;
}

//1F194
void calc_num_let_spr(u8 a1, u8* a2) {
    //stub
}

//1F1B0
i16 calc_largmax_text(const char* text, i16 char_index, i16 space_width, i16 char_spacing, u8 font_size) {
    i16 width = 0;
    for (;;) {
        ++char_index;
        char c = text[char_index];
        if (c == '\0' || c == '/') {
            break;
        }
        if (c == ' ') {
            width += space_width;
        } else {
            i32 num_let = deter_num_let(c, text + char_index + 1);
            if (num_let != 0) {
                width += calc_let_Width2(font_size, num_let) - char_spacing;
                if (num_let >= 1000) {
                    char_index += 3;
                }
            }
        }
    }
    return width;
}

//1F21C
void INIT_TXT_BOX(display_item_t* box) {
    char text[200];
    u8 font_size = box->font_size;
    i32 total_width = 0;
    i32 max_width = 0;
    i32 line_height = 0;
    i32 num_lines = 0;
    i32 max_chars = 0;
    i32 total_chars = 0;
    i32 is_slash = 0;
    i32 char_spacing = 0;
    i32 space_width = 0;
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
    memcpy(text, box->text, sizeof(text));
    if (text[0] != '\0') {
        for (i32 i = 0; i < sizeof(text); ++i) {
            char c = text[i];
            if (c == '\0') {
                break;
            }
            if (c == '/') {
                is_slash = 1;
                ++num_lines;
                if (total_width > max_width) {
                    max_width = total_width;
                }
                total_width = 0;
                if (total_chars > max_chars) {
                    max_chars = total_chars;
                }
                total_chars = 0;
                num_let = 0;
            } else if (c == ' ') {
                num_let = 0;
                total_width += space_width;
                ++total_chars;
            } else {
                num_let = deter_num_let(text[i], text + i + 1);
            }
            if (num_let != 0) {
                total_chars += 1;
                total_width += calc_let_Width2(font_size, num_let) - char_spacing;
                if (num_let >= 1000) {
                    i += 3;
                }
            }
        }
        if (num_lines != 0) {
            --num_lines;
        } else {
            max_width = total_width;
            num_lines = 1;
        }
        if (is_slash) {
            box->centered_x_pos = box->xpos - (max_width >> 1);
        } else {
            box->centered_x_pos = box->xpos;
        }
        box->width = max_width + 2;
        box->centered_y_pos = box->ypos - line_height + 2;
        box->height = num_lines * line_height - 2;
    }
}

//1F4A0
void Deter_Option_Caract(char* text, i16 key, i16 offset) {
    if (offset == 255) {
        i16 len = (i16)strlen(text);
        for (i16 i = 0; i < len; ++i) {
            char c = text[i];
            if (c == '4') {
                offset = i;
            }
        }
    }
    if (offset != 255) {
        switch(key) {
            case 0: {
                text[offset] = 'a';
            } break;
            case 1: {
                text[offset] = 'b';
            } break;
            case 2: {
                text[offset] = 'c';
            } break;
            case 3: {
                text[offset] = 'd';
            } break;
            default: break;
        }
    }
}

//1F50C
void SwapAB(i16* a, i16* b) {
    i16 temp = *a;
    *a= *b;
    *b = temp;
}

// Taken from the PS1 decomp for now (reconstructing this doesn't look fun)
//1F520
void Bresenham(void (*func)(i16, i16), i16 origin_x, i16 origin_y, i16 dest_x, i16 dest_y, i16 param_6, i16 percent)
{
    s16 sp10;
    s16 sp12;
    s16 sp14;
    s16 sp16;
    void (*sp18)(s16, s16);
    s16 sp20;
    s32 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s32 var_s1_1;
    s16 var_s1_2;
    s16 var_s1_3;
    s16 var_s1_4;
    s16 var_s3;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_lo_1;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_s6_1;
    s32 temp_s6_2;
    s32 temp_s6_3;
    s32 temp_s6_4;
    s16 var_s0_1;
    s32 var_s2_1;
    s32 var_s2_2;
    s32 var_s4;
    s32 var_s5;
    s32 var_s7_1;
    s32 var_s7_2;
    s32 var_v0;
    s32 test_1;
    s32 test_2;

    /*sp18 = param_1;*/
    sp10 = origin_x;
    sp14 = origin_y;
    sp12 = dest_x;
    var_s0_1 = 0;
    var_s5 = dest_x - origin_x;
    sp16 = dest_y;
    var_s4 = dest_y - origin_y;
    test_1 = param_6;
    sp20 = test_1;
    temp_s6_1 = percent;
    if (var_s5 == 0)
    {
        var_s3 = 0;
    }
    else
    {
        var_s3 = abs(var_s4 / var_s5) < 1;
    }
    if (var_s3 == 1)
    {
        if (var_s5 < 0)
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            SwapAB(&sp14, &sp16);
            var_s5 = -var_s5;
            var_s4 = -var_s4;
        }
        else if ((var_s5 == 0) && (var_s4 < 0))
        {
            var_s0_1 = 1;
            SwapAB(&sp14, &sp16);
            var_s4 = -var_s4;
        }

        if (var_s4 <= 0)
        {
            var_s7_1 = var_s4 >> 0x1F;
        }
        else
            var_s7_1 = 1;

        var_s4 = abs(var_s4);
        var_s2_1 = (var_s4 * 2) - var_s5;
        temp_a3 = var_s4 - var_s5;
        if (var_s0_1 != 0)
        {
            var_s0_2 = sp12;
            temp_s6_1 = var_s0_2 + ((sp10 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp16;

            var_s7_1 = -var_s7_1;
            while (var_s0_2 >= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a3 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s4 * 2);
                }

                if (sp20 < var_s3)
                {
                    func(var_s0_2, var_s1_1);
                    var_s3 = 0;
                }
                var_s0_2 -= 1;
            }
        }
        else
        {
            var_s0_2 = sp10;
            temp_s6_1 = var_s0_2 + ((sp12 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2 = var_s0_2 + 1;
            var_s1_1 = sp14;

            while (var_s0_2 <= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a3 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s4 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s0_2, var_s1_1);
                    var_s3 = 0;
                }
                var_s0_2 += 1;
            }
        }
    }
    else
    {
        if (var_s4 < 0)
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            SwapAB(&sp14, &sp16);
            var_s5 = -var_s5;
            var_s4 = -var_s4;
        }
        else if ((var_s4 == 0) && (var_s5 < 0))
        {
            var_s0_1 = 1;
            SwapAB(&sp10, &sp12);
            var_s5 = -var_s5;
        }

        if (var_s5 <= 0)
        {
            var_s7_1 = var_s5 >> 0x1F;
        }
        else
            var_s7_1 = 1;
        var_s5 = abs(var_s5);
        var_s2_1 = (var_s5 * 2) - var_s4;
        temp_a0 = var_s5 - var_s4;
        if (var_s0_1 != 0)
        {
            var_s0_2 = sp16;
            temp_s6_1 = var_s0_2 + ((sp14 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp12;
            var_s7_1 = -var_s7_1;
            while (var_s0_2 >= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a0 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s5 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s1_1, var_s0_2);
                    var_s3 = 0;
                }
                var_s0_2 -= 1;
            }
        }
        else
        {
            var_s0_2 = sp14;
            temp_s6_1 = var_s0_2 + ((sp16 - var_s0_2) * (s16) temp_s6_1 / 100);
            var_s0_2++;
            var_s1_1 = sp10;
            while (var_s0_2 <= temp_s6_1)
            {
                var_s3 += 1;
                if (var_s2_1 > 0)
                {
                    var_s1_1 += var_s7_1;
                    var_s2_1 += (s16) (temp_a0 * 2);
                }
                else
                {
                    var_s2_1 += (s16) (var_s5 * 2);
                }
                if (sp20 < var_s3)
                {
                    func(var_s1_1, var_s0_2);
                    var_s3 = 0;
                }
                var_s0_2 += 1;
            }
        }
    }
}

//1F880
void init_finBossLevel(void) {
    memset(&finBosslevel, 0, sizeof(finBosslevel));
}

//1F8A4
void Change_Wait_Anim(void) {
    eta_t** ray_eta = ray.eta;
    u8 prev_00 = ray_eta[0][0].anim_index;
    ray_eta[3][20].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][0].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][56].anim_index = prev_00;
    first_boss_meet = 1 - first_boss_meet;
}

//1F8E8
void add_alwobj(obj_t* obj) {
    level_obj.obj_ids[level_obj.nb_objects++] = obj->id;
}

//1F918
void del_alwobj(i16 obj_index) {
    i32 found_index = -1;
    for (i32 i = 0; i < level_obj.nb_objects; ++i) {
        if (level_obj.obj_ids[i] == i) {
            found_index = i;
            break;
        }
    }
    if (found_index >= 0 && found_index < level_obj.nb_objects) {
        --level_obj.nb_objects;
        level_obj.obj_ids[found_index] = level_obj.obj_ids[level_obj.nb_objects];
    }
}

//1F988
void FatalError(char* message) {
    stop_cd();
    InitTextMode();
    printf("Rayman says fatal error :\n%s\n", message);
    //PrintMemInfo();
    exit(-1);
    //stub
}

//1FA00
u8 sprite_of_ref(obj_t* obj, u8 a2) {
    return 0; //stub
}

