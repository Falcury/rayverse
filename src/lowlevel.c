
// NOTE: This section of the PC executable contains lower-level stuff, including DOS stuff and drawing routines.
// These parts were likely written in assembly (the calling convention and register use also seems to vary a lot).


// functions omitted up to 145F1 (including various draw functions for X mode)

//12783
void display_emptypicture(void) {
    print_once("Not implemented: display_emptypicture"); //stub
}

//127A2
void display_anypicture(void) {
    print_once("Not implemented: display_anypicture");
}

//145F1
void select_display_buffer(u8* buffer) {
    print_once_dos("Not implemented: select_display_buffer");
}

//145FC
void InitFirstModeVideo(void) {
    print_once_dos("Not implemented: InitFirstModeVideo");
}

//1465F
void InitModeNormal(void) {
    print_once_dos("Not implemented: InitModeNormal");
}

//146B8
void InitModeX(void) {
    print_once_dos("Not implemented: InitModeX");
}

//1471D
void block_clear(u8* source, u8* dest) {
    print_once("Not implemented: block_clear");
}

//14765
void block_copy(u8* source, u8* dest) {
    print_once("Not implemented: block_copy");
}

//147E7
void textmode(void) {
    print_once_dos("Not implemented: textmode");
}

//14828
void draw_sprite_deform(void) {
    print_once("Not implemented: draw_sprite_deform");
}

//14AB7
void DisplayBufferModeNormal(u8* source, u8* dest) {
    print_once("Not implemented: DisplayBufferModeNormal");
}

//14AC1
void DrawWldPointPlan2Normal(u8* buffer, s32 x, s32 y) {
    s32 stride = 384;
    u8* pos = buffer + stride * y + x;
    *(u16*)(pos + 0 * stride + 1) = 0x0C00;
    *(u32*)(pos + 1 * stride)   = 0x0C0E0A00;
    *(u32*)(pos + 2 * stride)   = 0x000A0900;
    *(u16*)(pos + 3 * stride + 1) = 0x0C00;
}

//14AEF
void DrawBlackBoxNormal(u8* buffer /*edi*/, s32 x /*edx*/, s32 y /*ecx*/, s32 height /*ebx*/, s32 width /*eax*/) {
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 200) {
        height = 200 - y;
    }
    if (x + width > 320) {
        width = 320 - x;
    }
    if (width > 0 && height > 0) {
        for (s32 current_y = y; current_y < y + height && height < 320; ++current_y) {
            memset(buffer + 320 * current_y + x, 0, width);
        }
    }
}

//14B2C
void DrawFondBoxNormal(u8* buffer /*edi*/, s32 x /*edx*/, s32 y /*ecx*/, s32 height /*ebx*/, s32 width /*esi*/, u8 fond_type /*eax*/) {
    if (fond_type == 2) {
        u8* last_line = buffer + SCREEN_WIDTH * (y + height) + width - SCREEN_WIDTH - 1;
        u8* second_to_last_line = last_line - SCREEN_WIDTH - 1;
        s32 skip_amount = SCREEN_WIDTH - x;

        //stub

    } else {

    }
    print_once("Not implemented: DrawFondBoxNormal"); //stub
}

//14BBB
void DrawBorderBoxNormal(u8* buffer /*edi*/, s32 x, s32 y, s32 height, s32 width, u16 colors /*eax*/) {
    u8* pos = buffer + SCREEN_WIDTH * y + x;
    u8 light_color = (u8)colors;
    u8 dark_color = colors >> 8;

    // draw the left and right vertical lines
    for (s32 i = height; i != 0; --i) {
        *pos = light_color;
        *(pos + width - 1) = dark_color;
        *(pos + width) = dark_color;
        pos += SCREEN_WIDTH;
    }

    // draw the bottom 2 darker lines
    memset(pos, dark_color, width);
    memset(pos - SCREEN_WIDTH + 1, dark_color, width);

    // draw the top line (light color)
    pos = buffer + SCREEN_WIDTH * y + x;
    memset(pos, light_color, width); // draw the top line
}

//14C19
void DisplayAnyPictureNormal(u8* source_buffer, u8* dest_buffer, s32 source_x, s32 source_y, s32 dest_x, s32 dest_y, s32 stride, s32 width, s32 height) {
    // NOTE: bounds checks added
    if (source_x < 0) {
        dest_x -= source_x;
        width += source_x;
        source_x = 0;
    }
    if (source_y < 0) {
        dest_y -= source_y;
        height += source_y;
        source_y = 0;
    }
    if (dest_x + width > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - dest_x;
    }
    if (dest_y + height > SCREEN_HEIGHT) {
        height = SCREEN_HEIGHT - dest_y;
    }
    if (width <= 0 || height <= 0) {
        return;
    }

    u8* dest = dest_buffer + SCREEN_WIDTH * dest_y + dest_x;
    u8* source = source_buffer + stride * source_y + source_x;
    for (s32 i = height; i != 0; --i) {
        memcpy(dest, source, width);
        dest += SCREEN_WIDTH;
        source += stride;
    }
}

//14C6B
void ClearDrawAndDisplayBufferNormal(u8* draw_buf /*edi*/, u8* display_buf /*esi*/) {
    // added NULL checks
    if (draw_buf) {
        memset(draw_buf, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
    if (display_buf) {
        memset(display_buf, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
}

//14C8E
void N_CLRSCR(u8* buffer) {
    memset(buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}

//14C9C
void Swap_To_Screen(u8* draw_buf, u8* display_buf, s32 width, s32 height) {
    print_once("Not implemented: Swap_To_Screen"); //stub
}

//14CC7
void clear_borders_Normal(u8* buffer, s32 height, s32 width) {
    print_once("Not implemented: clear_borders_Normal"); //stub
}

//14CFA
void Copy_Plan0Diff_To_Buf(u8* source, u8* dest, s32 width, s32 height, s32 draw_width) {
    for (s32 i = 0; i < height; ++i) {
        memcpy(dest, source, draw_width);
        dest += SCREEN_WIDTH;
        source += width;
    }
}

//14E64
void Display_Bloc(void* a1, void* a2) {
    print_once("Not implemented: Display_Bloc"); //stub
}

//15041
void Display_Bloc_Plein(void* a1, void* a2) {
    print_once("Not implemented: Display_Bloc_Plein"); //stub
}

//152DA
void Clip(void) {
    print_once("Not implemented: Clip"); //stub
}

//153C6
void Display_Bloc_Clippe(void* a1, void* a2, s16 a3, s16 a4) {
    print_once("Not implemented: Display_Bloc_Clippe"); //stub
}

//1555E
void Display_Bloc_Plein_Clippe(void* a1, void* a2, s16 a3, s16 a4) {
    print_once("Not implemented: Display_Bloc_Plein_Clippe"); //stub
}

//15609
void fplot_Normal(u8* buffer, s32 x, s32 y, u8 color) {
    buffer[y * SCREEN_WIDTH + x] = color;
}

s32 Bloc_floc_H1 = 0; //15618
s32 Bloc_floc_H2 = 0; //1561C
s32 Bloc_floc_W1 = 0; //15620
s32 Bloc_floc_W2 = 0; //15624

//15628
void flocon_clipping(s32 h1, s32 h2, s32 w1, s32 w2) {
    Bloc_floc_H1 = h1;
    Bloc_floc_H2 = h2;
    Bloc_floc_W1 = w1;
    Bloc_floc_W2 = w2;
}

//15640
void draw_flocon5_Normal(void) {
    print_once("Not implemented: draw_flocon5_Normal"); //stub
}

//15670
void draw_flocon1_Normal(void) {
    print_once("Not implemented: draw_flocon1_Normal"); //stub
}

//156A0
void draw_flocon2_Normal(void) {
    print_once("Not implemented: draw_flocon2_Normal"); //stub
}

//156DC
void draw_flocon3_Normal(void) {
    print_once("Not implemented: draw_flocon3_Normal"); //stub
}

//15725
void draw_pluie4_Normal(void) {
    print_once("Not implemented: draw_pluie4_Normal"); //stub
}

//15755
void draw_flocon4_Normal(void) {
    print_once("Not implemented: draw_flocon4_Normal"); //stub
}

//157A6
void draw_pluie5_Normal(void) {
    print_once("Not implemented: draw_pluie5_Normal"); //stub
}

//157DF
void draw_flocon7_Normal(void) {
    print_once("Not implemented: draw_flocon7_Normal"); //stub
}

//15830
void draw_pluie6_Normal(void) {
    print_once("Not implemented: draw_pluie6_Normal"); //stub
}

//1587D
void draw_flocon6_Normal(void) {
    print_once("Not implemented: draw_flocon6_Normal"); //stub
}

//158F4
void draw_pluie7_Normal(void) {
    print_once("Not implemented: draw_pluie7_Normal"); //stub
}

//1595D
void get_joy_input2_dos1(u8* source_buf, u8* dest_buf, s32 width, s32 height) {
    print_once("Not implemented: get_joy_input2_dos1"); //stub
}

//15A57
void get_joy_input2_dos2(u8* source_buf, u8* dest_buf, s32 width, s32 height) {
    print_once("Not implemented: get_joy_input2_dos2"); //stub
}

//15B55
void get_joy_input2_windows1(u8* source_buf, u8* dest_buf, s32 width, s32 height) {
    print_once("Not implemented: get_joy_input2_windows1"); //stub
}

//15C13
void get_joy_input2_windows2(u8* source_buf, u8* dest_buf, s32 width, s32 height) {
    print_once("Not implemented: get_joy_input2_windows2"); //stub
}

//15CDF
void dos_game_io_port_something(void) {
    print_once("Not implemented: dos_game_io_port_something"); //stub
}

//15D14
void dos_game_io_port_something2(void) {
    print_once("Not implemented: dos_game_io_port_something2"); //stub
}

//15D4A
void get_joy_input1_dos1(void) {
    print_once("Not implemented: get_joy_input1_dos1"); //stub
}

//15F34
void get_joy_input1_dos2(void) {
    print_once("Not implemented: get_joy_input1_dos2"); //stub
}

//1611E
void DetectCpu(void) {
    print_once("Not implemented: DetectCpu"); //stub
}

s32 XMIN = 8; //16184 //TODO: change XMIN to 0?
s32 XMAX = 312; //16188
s32 YMIN = 0; //1618C
s32 YMAX = 200; //16190


//16194
void default_sprite_clipping(void) {
    XMIN = 0; //TODO: change XMIN to 0?
    XMAX = 312;
    YMIN = 0;
    YMAX = 200;
}

//161BD
void get_sprite_clipping(s32* xmin, s32* xmax, s32* ymin, s32* ymax) {
    *xmin = XMIN;
    *xmax = XMAX;
    *ymin = YMIN;
    *ymax = YMAX;
}

//145F1
s32 get_sprite_clipping_xmin(void) {
    return XMIN;
}

//145F1
s32 get_sprite_clipping_xmax(void) {
    return XMAX;
}

//145F1
s32 get_sprite_clipping_ymin(void) {
    return YMIN;
}

//145F1
s32 get_sprite_clipping_ymax(void) {
    return YMAX;
}

//161FA
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax) {
    if (xmin < 0) xmin = 0;
    if (xmin > 320) xmin = 320;
    XMIN = xmin;
    if (xmax < 0) xmax = 0;
    if (xmax > 320) xmax = 320;
    XMAX = xmax;
    if (ymin < 0) ymin = 0;
    if (ymin > 200) ymin = 200;
    YMIN = ymin;
    if (ymax < 0) ymax = 0;
    if (ymax > 200) ymax = 200;
    YMAX = ymax;
}

//1626D
bool clip_sprite_on_screen(s32* x, s32* y, vec2b_t* size, u8** image_data) {
    saved_sprite_width = size->x;
    if (*x < XMIN) {
        s32 x_left_of_screen = -(*x - XMIN);
        if (size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            size->x -= (u8) x_left_of_screen;
            *x = XMIN;
            *image_data += x_left_of_screen;
        }
    }
    s32 proj_right = *x + size->x;
    if (proj_right > XMAX) {
        if (*x >= XMAX) {
            return false;
        } else {
            size->x = XMAX - *x;
        }
    }
    if (*y < YMIN) {
        if (*y + size->y < YMIN) {
            return false;
        } else {
            s32 y_above_screen = -(*y - YMIN);
            y_above_screen &= 0xFFFF;
            size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *y = YMIN;
        }
    }
    s32 proj_bottom = *y + size->y;
    if (proj_bottom > YMAX) {
        if (*y >= YMAX) {
            return false;
        } else {
            size->y = YMAX - *y;
            return true;
        }
    } else {
        return true;
    }
}

//16323
bool clip_sprite_on_screen_flipped(s32* x, s32* y, vec2b_t* size, u8** image_data) {
    saved_sprite_width = size->x;
    if (*x < XMIN) {
        s32 x_left_of_screen = -(*x - XMIN);
        if (size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            size->x -= (u8) x_left_of_screen;
            *x = XMIN;
        }
    }
    s32 proj_right = *x + size->x;
    if (proj_right > XMAX) {
        if (*x >= XMAX) {
            return false;
        } else {
            size->x = XMAX - *x;
            *image_data += saved_sprite_width - size->x;
        }
    }
    if (*y < YMIN) {
        if (*y + size->y < YMIN) {
            return false;
        } else {
            s32 y_above_screen = -(*y - YMIN);
            y_above_screen &= 0xFFFF;
            size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *y = YMIN;
        }
    }
    s32 proj_bottom = *y + size->y;
    if (proj_bottom > YMAX) {
        if (*y >= YMAX) {
            return false;
        } else {
            size->y = YMAX - *y;
            return true;
        }
    } else {
        return true;
    }
}


#define DRAW_FUNC(name) void name (s32 proj_x, s32 color, s32 proj_y, vec2b_t proj_size, u8* draw_buf, u8* image_data)

//163E6
void DrawSpriteFlipNoClipX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteFlipNoClipX"); //stub
}

//163F2
void DrawSpriteX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteX"); //stub
}

//16559
void DrawSpriteNoClipX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteNoClipX"); //stub
}

//16565
void DrawSpriteFlipX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteFlipX"); //stub
}

//166DF
void DrawSpriteColorX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteColorX"); //stub
}

//16876
void DrawSpriteColorFlipX(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    print_once("Not implemented: DrawSpriteColorFlipX"); //stub
}

//16A18
void DrawSpriteNormalNoClip(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    u8* draw_pos = draw_buf + y * 320 + x;
    u8* draw_end = draw_pos + size.y * 320;
    s32 sprite_width = size.x;
    u8* sprite_pos = image_data;
    while (draw_pos < draw_end) {
        for (s32 i = 0; i < size.x; ++i) {
            u8 c = sprite_pos[i];
            if (c < 160) {
                draw_pos[i] = c;
            } else {
                i += (c - 160);
            }
        }
        sprite_pos += sprite_width;
        draw_pos += 320;
    }
}

//16A24
void DrawSpriteNormal(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c < 160) {
                    draw_pos[i] = c;
                } else {
                    i += (c - 160);
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

//16A9D
void DrawSpriteNormal256(s32 x /*eax*/, s32 color /*edx*/, s32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c != 0) {
                    draw_pos[i] = c;
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

//16AFC
void DrawSpriteFlipNormalNoClip(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    u8* draw_pos = draw_buf + y * 320 + x;
    u8* draw_end = draw_pos + size.y * 320;
    s32 sprite_width = size.x;
    u8* sprite_pos = image_data;
    while (draw_pos < draw_end) {
        for (s32 i = 0; i < size.x; ++i) {
            u8 c = sprite_pos[i];
            if (c < 160) {
                draw_pos[size.x - i - 1] = c; // reverse order (flipped)
            } else {
                i += (c - 160);
            }
        }
        sprite_pos += sprite_width;
        draw_pos += 320;
    }
}

//16B08
void DrawSpriteFlipNormal(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    if (clip_sprite_on_screen_flipped(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c < 160) {
                    draw_pos[size.x - i - 1] = c; // reverse order (flipped)
                } else {
                    i += (c - 160);
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

//16B88
void DrawSpriteFlipNormal256(s32 x /*eax*/, s32 color /*edx*/, s32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen_flipped(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c != 0) {
                    draw_pos[size.x - i - 1] = c; // reverse order (flipped)
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

//16BEA
void DrawSpriteColorNormal(s32 x /*eax*/, s32 color /*edx*/, s32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    saved_sprite_color = color * 8;
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        u8 current_color = saved_sprite_color;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c < 160) {
                    draw_pos[i] = c | current_color;
                } else {
                    i += (c - 160);
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

//16C89
void DrawSpriteColorFlipNormal(s32 x, s32 color, s32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    saved_sprite_color = color * 8;
    if (clip_sprite_on_screen_flipped(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        s32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        u8 current_color = saved_sprite_color;
        while (draw_pos < draw_end) {
            for (s32 i = 0; i < size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c < 160) {
                    draw_pos[size.x - i - 1] = c | current_color; // reverse order (flipped)
                } else {
                    i += (c - 160);
                }
            }
            sprite_pos += sprite_width;
            draw_pos += 320;
        }
    }
}

static s32 diff_saved_sprite_width; //16D2F

//16D33
bool DrawSpriteDiffNormal_clip(s32* x /*eax*/, s32* y /*ebx*/, vec2b_t* size /*ecx*/, u8** image_data /*esi*/) {
    diff_saved_sprite_width = size->x;
    if (*x < XMIN) {
        s32 x_left_of_screen = -(*x - XMIN);
        if (size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFC;
            size->x -= (u8) x_left_of_screen;
            *x = XMIN;
            *image_data += *x + x_left_of_screen;
            size->x >>= 2;
        }
    } else if (*x + size->x > XMAX) {
        if (*x >= XMAX) {
            return false;
        } else {
            size->x = XMAX - *x;
            size->x >>= 2;
            ++size->x;
        }
    }
    if (*y < YMIN) {
        if (*y + size->y < YMIN) {
            return false;
        } else {
            s32 y_above_screen = -(*y - YMIN);
            y_above_screen &= 0xFFFF;
            size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *y = YMIN;
        }
    }
    if (*y + size->y > YMAX) {
        if (*y >= YMAX) {
            return false;
        } else {
            size->y = YMAX - *y;
            return true;
        }
    } else {
        return true;
    }
}

//16E0D
void DrawSpriteDiffNormal(s32 x /*eax*/, s32 y /*ebx*/, vec2b_t size /*ecx*/, u8* mask /*edx*/, u8* draw_buf /*esi*/, u8* image_data /*edi*/) {
    // NOTE: this seems to be implemented in a different way in the PC version, I guess for blitting performance reasons?
    // Because we don't worry about small performance optimizations, we just use the regular draw method that's already working fine.
    DrawSpriteNormal256(x, 0, y, size, draw_buf, image_data);
}

//16E8A
void synchro(void) {
    print_once_dos("Not implemented: synchro"); //stub
}

//16E9A
void endsynchro(void) {
    advance_frame();
}

//16EAA
void set_speaker_on(void) {
    print_once_dos("Not implemented: set_speaker_on"); //stub
}

//16EEA
void set_speaker_off(void) {
    print_once_dos("Not implemented: set_speaker_off"); //stub
}

//16EFD
void clock_ticks(void) {
    print_once_dos("Not implemented: clock_ticks"); //stub
}

//16F5D
void wait_until(s32 target /*ebx*/) {
    print_once_dos("Not implemented: wait_until"); //stub
}

//16F78
void speaker_enable(void) {
    set_speaker_on();
}

// Below functions are re-implemented from Rayman Designer

void draw_pixel_to_display_buffer(s16 x, s16 y, u8 color) {
    if (x > 0 && x < SCREEN_WIDTH && y > 0 && y < SCREEN_HEIGHT) {
        if (ModeVideoActuel == MODE_X) {
            // NOTE: Not implemented in rayverse
        }
        else {
            // NOTE: The game draws directly to display_buffer here - we instead use a separate buffer which we overlay later on
            debug_overlay_draw_buffer[y * SCREEN_WIDTH + x] = color;
        }
    }
}

void draw_pixel_to_buffer(u8* buffer, s16 x, s16 y, u8 color) {
    if (x > 0 && x < SCREEN_WIDTH && y > 0 && y < SCREEN_HEIGHT) {
        if (ModeVideoActuel == MODE_X) {
            // NOTE: Not implemented in rayverse
        }
        else {
            buffer[y * SCREEN_WIDTH + x] = color;
        }
    }
}

void draw_horizontal_line_to_display_buffer(s16 start_x, s16 start_y, s16 width, u8 color) {
    if (start_y > 0 && start_y < SCREEN_HEIGHT) {
        for (s16 x = start_x; x < start_x + width; x++) {
            if (x > 0 && x < SCREEN_WIDTH) {
                draw_pixel_to_display_buffer(x, start_y, color);
            }
        }
    }
}

void draw_horizontal_line_to_draw_buffer(s16 start_x, s16 start_y, s16 width, u8 color) {
    if (start_y > 0 && start_y < SCREEN_HEIGHT) {
        for (s16 x = start_x; x < start_x + width; x++) {
            if (x > 0 && x < SCREEN_WIDTH) {
                draw_pixel_to_buffer(draw_buffer, x, start_y, color);
            }
        }
    }
}

void draw_vertical_line_to_display_buffer(s16 start_x, s16 start_y, s16 height, u8 color) {
    if (start_x > 0 && start_x < SCREEN_WIDTH) {
        for (s16 y = start_y; y < start_y + height; y++) {
            if (y > 0 && y < SCREEN_HEIGHT) {
                draw_pixel_to_display_buffer(start_x, y, color);
            }
        }
    }
}

void draw_vertical_line_to_draw_buffer(s16 start_x, s16 start_y, s16 height, u8 color) {
    if (start_x > 0 && start_x < SCREEN_WIDTH) {
        for (s16 y = start_y; y < start_y + height; y++) {
            if (y > 0 && y < SCREEN_HEIGHT) {
                draw_pixel_to_buffer(draw_buffer, start_x, y, color);
            }
        }
    }
}

void draw_collision_box(s16 x, s16 y, s16 width, s16 height) {
    draw_horizontal_line_to_display_buffer(x, y, width, 0xC);
    draw_horizontal_line_to_display_buffer(x, y + height - 1, width, 0xC);
    draw_vertical_line_to_display_buffer(x, y, height, 0xC);
    draw_vertical_line_to_display_buffer(x + width - 1, y, height, 0xC);
}

void draw_deformed_line(u8* buffer, s16 x1, s16 y1, s16 x2, s16 y2, u8 color) {
    s16 next_x2;
    s16 next_y2;

    do {
        if (x1 == x2 || x1 == x2 + 1 || x1 == x2 - 1) {
            if (y1 == y2 || y1 == y2 + 1 || y1 == y2 - 1) {
                draw_pixel_to_buffer(buffer, x1, y1, color);
                return;
            }
        }
        
        next_x2 = ((x2 + x1) >> 1);
        next_y2 = ((y2 + y1) >> 1);
        draw_deformed_line(buffer, x1, y1, next_x2, next_y2, color);
        x1 = next_x2;
        y1 = next_y2;
    } while (true);
}
