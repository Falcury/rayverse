
// NOTE: This section of the PC executable contains lower-level stuff, including DOS stuff and drawing routines.
// These parts were likely written in assembly (the calling convention and register use also seems to vary a lot).


// functions omitted up to 145F1 (including various draw functions for X mode)


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
void DrawWldPointPlan2Normal(void* a1, i32 a2, i32 a3, u8* buffer) {
    //stub
}

//14AEF
void DrawBlackBoxNormal(u8* buffer, i32 x, i32 y, i32 height, i32 width) {
    //stub
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
void ClearDrawAndDisplayBufferNormal(u8* draw_buf, u8* display_buf) {
    memset(draw_buf, 0, 320*200);
    memset(display_buf, 0, 320*200);
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
bool clip_sprite_on_screen(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
    saved_sprite_width = proj_size->x;
    if (*proj_x < XMIN) {
        i32 x_left_of_screen = -(*proj_x - XMIN);
        if (proj_size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            proj_size->x -= (u8) x_left_of_screen;
            *proj_x = XMIN;
            *image_data += x_left_of_screen;
        }
    }
    i32 proj_right = *proj_x + proj_size->x;
    if (proj_right > XMAX) {
        if (*proj_x >= XMAX) {
            return false;
        } else {
            proj_size->x = XMAX - *proj_x;
        }
    }
    if (*proj_y < YMIN) {
        if (*proj_y + proj_size->y < YMIN) {
            return false;
        } else {
            i32 y_above_screen = -(*proj_y - YMIN);
            y_above_screen &= 0xFFFF;
            proj_size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *proj_y = YMIN;
        }
    }
    i32 proj_bottom = *proj_y + proj_size->y;
    if (proj_bottom > YMAX) {
        if (*proj_y >= YMAX) {
            return false;
        } else {
            proj_size->y = YMAX - *proj_y;
            return true;
        }
    } else {
        return true;
    }
}

//16323
bool clip_sprite_on_screen_flipped(i32* proj_x, i32* proj_y, vec2b_t* proj_size, u8** image_data) {
    saved_sprite_width = proj_size->x;
    if (*proj_x < XMIN) {
        i32 x_left_of_screen = -(*proj_x - XMIN);
        if (proj_size->x <= x_left_of_screen) {
            return false;
        } else {
            x_left_of_screen &= 0xFFFF;
            proj_size->x -= (u8) x_left_of_screen;
            *proj_x = XMIN;
        }
    }
    i32 proj_right = *proj_x + proj_size->x;
    if (proj_right > XMAX) {
        if (*proj_x >= XMAX) {
            return false;
        } else {
            proj_size->x = XMAX - *proj_x;
            *image_data += saved_sprite_width - proj_size->x;
        }
    }
    if (*proj_y < YMIN) {
        if (*proj_y + proj_size->y < YMIN) {
            return false;
        } else {
            i32 y_above_screen = -(*proj_y - YMIN);
            y_above_screen &= 0xFFFF;
            proj_size->y -= (u8) y_above_screen;
            *image_data += saved_sprite_width * y_above_screen;
            *proj_y = YMIN;
        }
    }
    i32 proj_bottom = *proj_y + proj_size->y;
    if (proj_bottom > YMAX) {
        if (*proj_y >= YMAX) {
            return false;
        } else {
            proj_size->y = YMAX - *proj_y;
            return true;
        }
    } else {
        return true;
    }
}


#define DRAW_FUNC(name) void name (i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data)

//163E6
void DrawSpriteFlipNoClipX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//163F2
void DrawSpriteX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16559
void DrawSpriteNoClipX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16565
void DrawSpriteFlipX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//166DF
void DrawSpriteColorX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16876
void DrawSpriteColorFlipX(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16A18
void DrawSpriteFlipNormalNoClip(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16A24
void DrawSpriteNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16A9D
void DrawSpriteNormal256(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen(&proj_x, &proj_y, &proj_size, &image_data) && proj_size.x > 0) {
        u8* draw_pos = draw_buf->memory + proj_y * draw_buf->width /*320*/ + proj_x;
        u8* draw_end = draw_pos + proj_size.y * draw_buf->width;
        ASSERT(draw_pos >= draw_buf->memory && draw_pos < draw_buf->memory + draw_buf->memory_size);
        ASSERT(draw_end >= draw_buf->memory && draw_end < draw_buf->memory + draw_buf->memory_size);
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < proj_size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c != 0) {
                    draw_pos[i] = c;
                }
            }
            sprite_pos += sprite_width;
            draw_pos += draw_buf->width;
        }
    }
}

//16AFC
void DrawSpriteNormalNoClip(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16B08
void DrawSpriteFlipNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16B88
void DrawSpriteFlipNormal256(i32 proj_x /*eax*/, i32 sprite_field_A /*edx*/, i32 proj_y /*ebx*/, vec2b_t proj_size /*ecx*/, image_t* draw_buf /*edi*/, u8* image_data /*esi*/) {
    if (clip_sprite_on_screen_flipped(&proj_x, &proj_y, &proj_size, &image_data) && proj_size.x > 0) {
        u8* draw_pos = draw_buf->memory + proj_y * draw_buf->width /*320*/ + proj_x;
        u8* draw_end = draw_pos + proj_size.y * draw_buf->width;
        ASSERT(draw_pos >= draw_buf->memory && draw_pos < draw_buf->memory + draw_buf->memory_size);
        ASSERT(draw_end >= draw_buf->memory && draw_end < draw_buf->memory + draw_buf->memory_size);
        i32 sprite_width = saved_sprite_width; // this was saved in clip_sprite_on_screen()
        u8* sprite_pos = image_data;
        while (draw_pos < draw_end) {
            for (i32 i = 0; i < proj_size.x; ++i) {
                u8 c = sprite_pos[i];
                if (c != 0) {
                    draw_pos[proj_size.x-i-1] = c; // reverse order (flipped)
                }
            }
            sprite_pos += sprite_width;
            draw_pos += draw_buf->width;
        }
    }
}

//16BEA
void DrawSpriteColorNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16C89
void DrawSpriteColorFlipNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16D33
void DrawSpriteDiffNormal_clip(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
    //stub
}

//16E0D
void DrawSpriteDiffNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buf, u8* image_data) {
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
void wait_until(void) {
    //stub
}

//16F78
void speaker_enable(void) {
    //stub
}


