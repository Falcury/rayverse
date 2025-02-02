
// NOTE: This section of the PC executable contains lower-level stuff, including DOS stuff and drawing routines.
// These parts were likely written in assembly (the calling convention and register use also seems to vary a lot).


// functions omitted up to 145F1 (including various draw functions for X mode)

//12783
void display_emptypicture(void) {
    //stub
}

//127A2
void display_anypicture(void) {
    //stub
}

//145F1
void select_display_buffer(u8* buffer) {
    //stub
}

//145FC
void InitFirstModeVideo(void) {
    //stub
}

//1465F
void InitModeNormal(void) {
    //stub
}

//146B8
void InitModeX(void) {
    //stub
}

//1471D
void block_clear(u8* source, u8* dest) {
    //stub
}

//14765
void block_copy(u8* source, u8* dest) {
    //stub
}

//147E7
void textmode(void) {
    //stub
}

//14828
void draw_sprite_deform(void) {
    //stub
}

//14AB7
void DisplayBufferModeNormal(u8* source, u8* dest) {
    //stub
}

//14AC1
void DrawWldPointPlan2Normal(u8* buffer, i32 x, i32 y) {
    i32 stride = 384;
    u8* pos = buffer + stride * y + x;
    *(u16*)(pos + 0 * stride + 1) = 0x0C00;
    *(u32*)(pos + 1 * stride)   = 0x0C0E0A00;
    *(u32*)(pos + 2 * stride)   = 0x000A0900;
    *(u16*)(pos + 3 * stride + 1) = 0x0C00;
}

//14AEF
void DrawBlackBoxNormal(u8* buffer, i32 x, i32 y, i32 height, i32 width) {
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
        for (i32 current_y = y; current_y < y + height && height < 320; ++current_y) {
            memset(buffer + 320 * current_y + x, 0, width);
        }
    }
}

//14B2C
void DrawFondBoxNormal(u8* buffer, i32 x, i32 y, i32 height, i32 width) {
    //stub
}

//14BBB
void DrawBorderBoxNormal(u8* buffer, i32 x, i32 y, i32 height, i32 width, i32 a6) {
    //stub
}

//14C19
void DisplayAnyPictureNormal(u8* source_buffer, u8* dest_buffer, i32 a3, i32 a4, i32 a5, i32 a6, i32 a7, i32 a8, i32 a9) {
    //stub
}

//14C6B
void ClearDrawAndDisplayBufferNormal(u8* draw_buf /*edi*/, u8* display_buf /*esi*/) {
    // added NULL checks
    if (draw_buf) {
        memset(draw_buf, 0, 320*200);
    }
    if (display_buf) {
        memset(display_buf, 0, 320*200);
    }
}

//14C8E
void N_CLRSCR(u8* buffer) {
    memset(buffer, 0, 320*200);
}

//14C9C
void Swap_To_Screen(u8* draw_buf, u8* display_buf, i32 height, i32 width) {
    //stub
}

//14CC7
void clear_borders_Normal(u8* buffer, i32 height, i32 width) {
    //stub
}

//14CFA
void Copy_Plan0Diff_To_Buf(u8* a1, u8* a2, i32 a3, i32 a4) {
    //stub
}

//14E64
void Display_Bloc(void* a1, void* a2) {
    //stub
}

//15041
void Display_Bloc_Plein(void* a1, void* a2) {
    //stub
}

//152DA
void Clip(void) {
    //stub
}

//153C6
void Display_Bloc_Clippe(void* a1, void* a2, i16 a3, i16 a4) {
    //stub
}

//1555E
void Display_Bloc_Plein_Clippe(void* a1, void* a2, i16 a3, i16 a4) {
    //stub
}

//15609
void fplot_Normal(u8* buffer, i32 a2, i32 a3, u8 a4) {
    //stub
}

i32 Bloc_floc_H1 = 0; //15618
i32 Bloc_floc_H2 = 0; //1561C
i32 Bloc_floc_W1 = 0; //15620
i32 Bloc_floc_W2 = 0; //15624

//15628
void flocon_clipping(i32 h1, i32 h2, i32 w1, i32 w2) {
    Bloc_floc_H1 = h1;

    //stub
}

//15640
void draw_flocon5_Normal(void) {
    //stub
}

//15670
void draw_flocon1_Normal(void) {
    //stub
}

//156A0
void draw_flocon2_Normal(void) {
    //stub
}

//156DC
void draw_flocon3_Normal(void) {
    //stub
}

//15725
void draw_pluie4_Normal(void) {
    //stub
}

//15755
void draw_flocon4_Normal(void) {
    //stub
}

//157A6
void draw_pluie5_Normal(void) {
    //stub
}

//157DF
void draw_flocon7_Normal(void) {
    //stub
}

//15830
void draw_pluie6_Normal(void) {
    //stub
}

//1587D
void draw_flocon6_Normal(void) {
    //stub
}

//158F4
void draw_pluie7_Normal(void) {
    //stub
}

//1595D
void get_joy_input2_dos1(void) {
    //stub
}

//15A57
void get_joy_input2_dos2(void) {
    //stub
}

//15B55
void get_joy_input2_windows1(void) {
    //stub
}

//15C13
void get_joy_input2_windows2(void) {
    //stub
}

//15CDF
void dos_game_io_port_something(void) {
    //stub
}

//15D14
void dos_game_io_port_something2(void) {
    //stub
}

//15D4A
void get_joy_input1_dos1(void) {
    //stub
}

//15F34
void get_joy_input1_dos2(void) {
    //stub
}

//1611E
void DetectCpu(void) {
    //stub
}

i32 XMIN = 8; //16184
i32 XMAX = 312; //16188
i32 YMIN = 0; //1618C
i32 YMAX = 200; //16190


//16194
void default_sprite_clipping(void) {
    XMIN = 8;
    XMAX = 312;
    YMIN = 0;
    YMAX = 200;
}

//161BD
void get_sprite_clipping(i32* xmin, i32* xmax, i32* ymin, i32* ymax) {
    *xmin = XMIN;
    *xmax = XMAX;
    *ymin = YMIN;
    *ymax = YMAX;
}

//145F1
i32 get_sprite_clipping_xmin(void) {
    return XMIN;
}

//145F1
i32 get_sprite_clipping_xmax(void) {
    return XMAX;
}

//145F1
i32 get_sprite_clipping_ymin(void) {
    return YMIN;
}

//145F1
i32 get_sprite_clipping_ymax(void) {
    return YMAX;
}

//161FA
void sprite_clipping(i32 xmin, i32 xmax, i32 ymin, i32 ymax) {
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
bool clip_sprite_on_screen(i32* x, i32* y, vec2b_t* size, u8** image_data) {
    saved_sprite_width = size->x;
    if (*x < XMIN) {
        i32 x_left_of_screen = -(*x - XMIN);
        if (size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            size->x -= (u8) x_left_of_screen;
            *x = XMIN;
            *image_data += x_left_of_screen;
        }
    }
    i32 proj_right = *x + size->x;
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
            i32 y_above_screen = -(*y - YMIN);
            y_above_screen &= 0xFFFF;
            size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *y = YMIN;
        }
    }
    i32 proj_bottom = *y + size->y;
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
bool clip_sprite_on_screen_flipped(i32* x, i32* y, vec2b_t* size, u8** image_data) {
    saved_sprite_width = size->x;
    if (*x < XMIN) {
        i32 x_left_of_screen = -(*x - XMIN);
        if (size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            size->x -= (u8) x_left_of_screen;
            *x = XMIN;
        }
    }
    i32 proj_right = *x + size->x;
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
            i32 y_above_screen = -(*y - YMIN);
            y_above_screen &= 0xFFFF;
            size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *y = YMIN;
        }
    }
    i32 proj_bottom = *y + size->y;
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


#define DRAW_FUNC(name) void name (i32 proj_x, i32 color, i32 proj_y, vec2b_t proj_size, u8* draw_buf, u8* image_data)

//163E6
void DrawSpriteFlipNoClipX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//163F2
void DrawSpriteX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16559
void DrawSpriteNoClipX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16565
void DrawSpriteFlipX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//166DF
void DrawSpriteColorX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16876
void DrawSpriteColorFlipX(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16A18
void DrawSpriteFlipNormalNoClip(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16A24
void DrawSpriteNormal(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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
void DrawSpriteNormal256(i32 x /*eax*/, i32 color /*edx*/, i32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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
void DrawSpriteNormalNoClip(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16B08
void DrawSpriteFlipNormal(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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
void DrawSpriteFlipNormal256(i32 x /*eax*/, i32 color /*edx*/, i32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen_flipped(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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
void DrawSpriteColorNormal(i32 x /*eax*/, i32 color /*edx*/, i32 y /*ebx*/, vec2b_t size /*ecx*/, u8* draw_buf /*edi*/, u8* image_data /*esi*/) {
    saved_sprite_color = color * 8;
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        u8 current_color = saved_sprite_color;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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
void DrawSpriteColorFlipNormal(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    saved_sprite_color = color * 8;
    if (clip_sprite_on_screen(&x, &y, &size, &image_data) && size.x > 0) {
        u8* draw_pos = draw_buf + y * 320 + x;
        u8* draw_end = draw_pos + size.y * 320;
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        u8 current_color = saved_sprite_color;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < size.x; ++i) {
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

//16D33
void DrawSpriteDiffNormal_clip(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16E0D
void DrawSpriteDiffNormal(i32 x, i32 color, i32 y, vec2b_t size, u8* draw_buf, u8* image_data) {
    //stub
}

//16E8A
void synchro(void) {
    //stub
}

//16E9A
void endsynchro(void) {
    advance_frame();
}

//16EAA
void set_speaker_on(void) {
    //stub
}

//16EEA
void set_speaker_off(void) {
    //stub
}

//16EFD
void clock_ticks(void) {
    //stub
}

//16F5D
void wait_until(i32 target /*ebx*/) {
    //stub
}

//16F78
void speaker_enable(void) {
    //stub
}


