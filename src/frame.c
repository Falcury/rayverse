
// omitted: sub_3A1C0 - sub_3B274 (I think these are related to movie playback)

//3B288
void display_movie_frames(void) {
    //stub
}

//3B314
i32 playVideo2(const char* path, const char* filename, i32 a3, u8 a4) {
    playing_intro_video = strcasecmp("intro.dat", filename) == 0;
    if (byte_CFA2A) {
        // stub
    }
    FILE* fp = open_data_file(filename, false);
    if (!fp) {
        return -5;
    }
    if (MusicCdActive) {

    }
    return 0; // stub
}

//3B4A8
i32 playVideo(const char* path, const char* filename, i32 a2) {
    return playVideo2(path, filename, a2, 0);
}

//3B4B8
i32 playVideo_alt(const char* path, const char* filename, i32 a2) {
    return playVideo2(path, filename, a2, 1);
}

//3B4D0
void SWAP_BUFFERS(void) {
    //stub
}

//3B580
void sub_3B580(void) {
    //stub
}

//3B5E8
void sub_3B5E8(void) {
    //stub
}

//3B64C
void calc_gros_type(void) {
    //stub
}

//3B7D4
void find_in_map(i16 a1, i16 a2) {
    //stub
}

//3B804
void init_find_in_map(void) {
    //stub
}

//3B838
void end_find_in_map(void) {
    //stub
}

//3B844
void build_map(i16 a1, i16 a2) {
    //stub
}

//3B948
void build_line_map(void* a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//3BA64
void build_column_map(void* a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//3BB84
void update_map(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//3BE20
void sub_3BE20(void) {
    //stub
}

//3BEE0
void sub_3BEE0(i16 a1, i16 a2) {
    //stub
}

//3C194
void set_vga_frequency(u8 a1) {
    //stub
}

//3C2D0
void GetVideoRegister(void) {
    //stub
}

//3C34C
void SetVideoRegister(void) {
    //stub
}

//3C3BC
void sub_3C3BC(void) {
    //stub
}

//3C4FC
void clear_palette(rgb_palette_t* palette) {
    memset(palette, 0, sizeof(rgb_palette_t));
    memset(rvb_fade, 0, sizeof(rvb_fade));
}

//3C520
void set_fade_palette(rgb_palette_t* palette) {
    //stub
}

//3C54C
void start_fade_in(i16 speed) {
    // apply palette par_0?
    nb_fade = 1 << (6 - speed);
    fade = 1; // fade in
    clear_palette(&current_rvb);
    fade_speed = speed;
    // apply palette
}

//3C5A4
void start_fade_out(i16 speed) {
    nb_fade = 1 << (6 - speed);
    fade = 2; // fade out
    fade_speed = speed;
}

//3C5CC
void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
    if (nb_fade > 0) {
        --nb_fade;
        if (fade == 1) {
            for (i32 i = 0; i < 256*3; ++i) {
                u8 c1 = ((u8*)(source_pal->colors))[i];
                u32 temp = c1 << fade_speed;
                rvb_fade[i] += temp;
            }
        } else if (fade == 2) {
            for (i32 i = 0; i < 256*3; ++i) {
                if (rvb_fade[i] > 0) {
                    u8 c1 = ((u8*)(source_pal->colors))[i];
                    u32 temp = c1 << fade_speed;
                    rvb_fade[i] -= temp;
                }
            }
        }
        for (i32 i = 0; i < 256*3; ++i) {
            u16 temp = rvb_fade[i] >> 6;
            ((u8*)(dest_pal->colors))[i] = (u8)temp;
        }
        // (apply palette)
        if (nb_fade == 0) {
            fade |= 0x40;
        }
    }
}

//3C6BC
void fade_out(i16 speed, rgb_palette_t* palette) {
    start_fade_out(speed);
    u16 steps = nb_fade;
    for (i32 i = 0; i < steps; ++i) {
        advance_frame();
        do_fade(palette, &current_rvb);
    }
    advance_frame();
}

//3C6F8
void actualize_palette(u8 new_pal_id) {
    //stub
}

//3C770
void cyclage_palette(i16 a1, i16 a2, i16 a3) {
    //stub
}

//3C8C8
void DO_SWAP_PALETTE(void) {
    //stub
}

//3CA64
void DO_FADE(void) {
    rgb_palette_t* palette = rvb + current_pal_id;
    do_fade(palette, &current_rvb);
}

//3CA8C
void INIT_FADE_IN(void) {
    start_fade_in(2);
}

//3CAB4
void INIT_FADE_OUT(void) {
    //stub
}

//3CADC
void DO_FADE_OUT(void) {
    fade_out(2, rvb + current_pal_id);
}

//3CB04
void EFFACE_VIDEO(void) {
    endsynchro();
    if (ModeVideoActuel == 1) {
        SWAP_BUFFERS();
        display_emptypicture();
        display_emptypicture();
    } else {
        ClearDrawAndDisplayBufferNormal(DrawBufferNormal, DisplayBufferNormal);
    }
}

//3CB54
void SYNCHRO_LOOP(scene_func_t scene_func) {
    i16 scene_ended = 0;
    do {
        advance_frame();
        DO_FADE();
        u32 timer = 0;
        scene_ended = scene_func(timer);
    } while(!scene_ended);
}

//3CBE8
void DISPLAY_ANYSIZE_PICTURE(void* a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    //stub
}

//3CCE4
void SAVE_PALETTE(rgb_palette_t* palette) {
    save_current_pal = current_pal_id;
    rvb_save = rvb[0];
    rvb[0] = *palette;
    current_rvb = *palette;
    current_pal_id = 0;
}

//3CD58
void RESTORE_PALETTE(void) {
    current_pal_id = save_current_pal;
    rvb[0] = rvb_save;
}

//3CD88
void SAVE_PLAN3(void) {
    //stub
}

//3CDD8
void RESTORE_PLAN3(void) {
    //stub
}

//3CDF8
void DISPLAY_FOND3(void) {
    memcpy(DrawBufferNormal, PLAN3BIT, 320*200);
}

//3CE20
void DISPLAY_FOND_MENU(void) {
    i32 v1 = 320 - Bloc_lim_W2 + Bloc_lim_W1;
    if (OptionGame) {
        if (prev_Bloc_lim_W1 != Bloc_lim_W1) {
            if (prev_Bloc_lim_W1 < Bloc_lim_W1) {
                ClearBorder((i16)Bloc_lim_H1, (i16)Bloc_lim_H2, (i16)Bloc_lim_W1, (i16)Bloc_lim_W2);
            }
            Bloc_lim_W1 = prev_Bloc_lim_W1;
        }
        memset(draw_buffer, 0, Bloc_lim_H1 * 320);
        i32 buffer_offset = Bloc_lim_H1 * 320 + Bloc_lim_W1;
        for (i32 y = Bloc_lim_H1; y < Bloc_lim_H2; ++y) {
            u8* dest_pos = draw_buffer + buffer_offset;
            u8* source_pos = EffetBufferNormal + buffer_offset;
            for (i32 x = Bloc_lim_W1; x < Bloc_lim_W2; ++x) {
                *dest_pos++ = *source_pos++;
            }
            buffer_offset += 320;
        }
        memset(draw_buffer, 0, (200 - Bloc_lim_H2) * 320);
    } else {
        memcpy(draw_buffer, EffetBufferNormal, 320*200);
    }
}

//3CF70
void InitPaletteSpecialPC(void) {
    //stub
}

//3D54C
void DoFadePaletteSpecialPC(i16 a1, i16 a2) {
    //stub
}

//3D704
void DoPaletteSpecialPC(void) {
    //stub
}

//3D8AC
void InitModeXWithFrequency(u8 freq) {
    //stub
}

//3D9D4
void InitTextMode(void) {
    textmode();
    ModeVideoActuel = 255;
}

//3D9E4
void InitModeNormalWithFrequency(u8 freq) {
    if (ModeVideoActuel == 255) {
//        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != 0) {
        //InitModeNormal();
//		memset(DrawBufferNormal, 0, 320 * 200);
//		memset(DisplayBufferNormal, 0, 320 * 200);
        //set_vga_frequency();
        ModeVideoActuel = 0;
        //dword_CD0F4 = sub_16A18;
        //dword_CD114 = sub_16AFC;
        DrawSpriteColorNormalEtX = DrawSpriteColorNormal;
        //dword_CD11C = (int)sub_16C89;
        draw_buffer = DrawBufferNormal;
        DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
        DrawSpriteNormalEtX = DrawSpriteNormal;
//		drawflocon1NormalETX = draw_flocon1_Normal;
//		drawflocon2NormalETX = draw_flocon2_Normal;
//		drawflocon3NormalETX = draw_flocon3_Normal;
//		drawflocon4NormalETX = draw_flocon4_Normal;
//		drawflocon5NormalETX = draw_flocon5_Normal;
//		drawflocon6NormalETX = draw_flocon6_Normal;
//		drawflocon7NormalETX = draw_flocon7_Normal;
//		drawpluie4NormalETX = draw_pluie4_Normal;
//		drawpluie6NormalETX = draw_pluie6_Normal;
//		drawpluie5NormalETX = draw_pluie5_Normal;
//		drawpluie7NormalETX = draw_pluie7_Normal;
//		fplotNormalETX = fplot_Normal;
    }
}

//3DB24
void WaitNSynchro(i32 n_frames) {
    for (i32 i = 0; i < n_frames; ++i) {
        advance_frame();
    }
}

