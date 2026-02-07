
// omitted: sub_3A1C0 - sub_3B274 (I think these are related to movie playback)

//3B288
void display_movie_frames(void) {
    print_once("Not implemented: display_movie_frames"); //stub
}

//3B314
s32 playVideo2(const char* path, const char* filename, s32 a3, u8 a4) {
    print_once("Not implemented: playVideo2");
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
s32 playVideo(const char* path, const char* filename, s32 a2) {
    return playVideo2(path, filename, a2, 0);
}

//3B4B8
s32 playVideo_alt(const char* path, const char* filename, s32 a2) {
    return playVideo2(path, filename, a2, 1);
}

//3B4D0
void SWAP_BUFFERS(void) {
    if (ModeVideoActuel == MODE_X) {
        print_once("Not implemented: SWAP_BUFFERS"); //stub
    } else {
        draw_buffer = DrawBufferNormal;
        display_buffer = DisplayBufferNormal;
        if (Mode_Pad == 1 && Main_Control) {
            Swap_And_Test_Joystick(display_buffer, DrawBufferNormal, 320, 200);
        }
    }
    print_once("Not implemented: SWAP_BUFFERS"); //stub
}

//3B580
void sub_3B580(void) {
    print_once("Not implemented: sub_3B580"); //stub
}

//3B5E8
void sub_3B5E8(void) {
    print_once("Not implemented: sub_3B5E8"); //stub
}

//3B64C
void calc_gros_type(void) {
    print_once("Not implemented: calc_gros_type"); //stub
}

//3B7D4
void find_in_map(s16 a1, s16 a2) {
    print_once("Not implemented: find_in_map"); //stub
}

//3B804
void init_find_in_map(void) {
    print_once("Not implemented: init_find_in_map"); //stub
}

//3B838
void end_find_in_map(void) {
    print_once("Not implemented: end_find_in_map"); //stub
}

//3B844
void build_map(s16 a1, s16 a2) {
    print_once("Not implemented: build_map"); //stub
}

//3B948
void build_line_map(void* a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: build_line_map"); //stub
}

//3BA64
void build_column_map(void* a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: build_column_map"); //stub
}

//3BB84
void update_map(s16 a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: update_map"); //stub
}

//3BE20
void sub_3BE20(void) {
    print_once("Not implemented: sub_3BE20"); //stub
}

//3BEE0
void sub_3BEE0(s16 a1, s16 a2) {
    print_once("Not implemented: sub_3BEE0"); //stub
}

//3C194
void set_vga_frequency(u8 freq) {
    SetVideoRegister();
    switch(freq) {
        case 100:
        case 80:
        case 70:
        case 60:
        case 50:
        default: break; //stub
    }
    VGA_FREQ = freq;
    print_once_dos("Not implemented: set_vga_frequency"); //stub
}

//3C2D0
void GetVideoRegister(void) {
    print_once_dos("Not implemented: GetVideoRegister"); //stub
}

//3C34C
void SetVideoRegister(void) {
    print_once_dos("Not implemented: SetVideoRegister"); //stub
}

//3C3BC
void sub_3C3BC(void) {
    print_once("Not implemented: sub_3C3BC"); //stub
}

//3C4FC
void clear_palette(rgb_palette_t* palette) {
    memset(palette, 0, sizeof(rgb_palette_t));
    memset(rvb_fade, 0, sizeof(rvb_fade));
}

//3C520
void set_fade_palette(rgb_palette_t* palette) {
    for (s32 i = 0; i < 256 * 3; ++i) {
        u8 c1 = ((u8*)(palette))[i];
        u32 temp = c1 << 6;
        rvb_fade[i] = temp;
    }
}

//3C54C
void start_fade_in(s16 speed) {
    // apply palette par_0?
    nb_fade = 1 << (6 - speed);
    fade = 1; // fade in
    clear_palette(&current_rvb);
    fade_speed = speed;
    SetPalette(&current_rvb, 0, 256);
}

//3C5A4
void start_fade_out(s16 speed) {
    nb_fade = 1 << (6 - speed);
    fade = 2; // fade out
    fade_speed = speed;
}

//3C5CC
void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
    if (nb_fade > 0) {
        --nb_fade;
        if (fade == 1) {
            for (s32 i = 0; i < 256 * 3; ++i) {
                u8 c1 = ((u8*)(source_pal->colors))[i];
                u32 temp = c1 << fade_speed;
                rvb_fade[i] += temp;
            }
        } else if (fade == 2) {
            for (s32 i = 0; i < 256 * 3; ++i) {
                if (rvb_fade[i] > 0) {
                    u8 c1 = ((u8*)(source_pal->colors))[i];
                    u32 temp = c1 << fade_speed;
                    rvb_fade[i] -= temp;
                }
            }
        }
        for (s32 i = 0; i < 256 * 3; ++i) {
            u16 temp = rvb_fade[i] >> 6;
            ((u8*)(dest_pal->colors))[i] = (u8)temp;
        }
        SetPalette(&current_rvb, 0, 256);
        if (nb_fade == 0) {
            fade |= 0x40;
        }
    }
}

//3C6BC
void fade_out(s16 speed, rgb_palette_t* palette) {
    start_fade_out(speed);
    u16 steps = nb_fade;
    for (s32 i = 0; i < steps; ++i) {
        advance_frame();
        do_fade(palette, &current_rvb);
    }
    advance_frame();
}

//3C6F8
void actualize_palette(u8 new_pal_id) {
    if (new_pal_id != current_pal_id) {
        current_pal_id = new_pal_id;
        current_rvb = rvb[new_pal_id];
        set_fade_palette(&current_rvb);
        if (fade == 65) {
            SetPalette(&current_rvb, 0, 256);
        }
    }
}

//3C770
void cyclage_palette(s16 a1, s16 a2, s16 a3) {
    print_once("Not implemented: cyclage_palette"); //stub
}

//3C8C8
void DO_SWAP_PALETTE(void) {
    if (last_plan1_palette != 0) {
        if (fade & 0x40) {
            for (s32 i = 0; i < actobj.num_active_objects; ++i) {
                obj_t* obj = level.objects + actobj.objects[i];
                if (obj->type == TYPE_158_PALETTE_SWAPPER) {
                    u8 pal_a = 0;
                    u8 pal_b = 0;
                    u8 new_pal = current_pal_id;
                    s16 ray_pos;
                    s16 obj_pos;
                    switch(obj->sub_etat) {
                        default: break;
                        case 0:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 1:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 2:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 3:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 4:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 5:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 6:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapVertical;
                        case 7:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapVertical;
                        case 8:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapVertical;
                        case 9:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapVertical;
                        case 10:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapVertical;
                        case 11:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapVertical;

                        SwapVertical:
                            ray_pos = ray.y + ray.offset_by;
                            obj_pos = obj->y + obj->offset_by;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;

                        SwapHorizontal:
                            ray_pos = ray.x + ray.offset_bx;
                            obj_pos = obj->x + obj->offset_bx;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;
                    }
                    actualize_palette(new_pal);
                }
            }
        }
    }
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
    start_fade_out(2);
}

//3CADC
void DO_FADE_OUT(void) {
    fade_out(2, rvb + current_pal_id);
}

//3CB04
void EFFACE_VIDEO(void) {
    endsynchro();
    if (ModeVideoActuel == MODE_X) {
        SWAP_BUFFERS();
        display_emptypicture();
        display_emptypicture();
    } else {
        ClearDrawAndDisplayBufferNormal(DrawBufferNormal, DisplayBufferNormal);
    }
}

//3CB54
void SYNCHRO_LOOP(scene_func_t scene_func) {
    s16 scene_ended = 0;
    do {
        advance_frame();
        DO_FADE();
        u32 timer = 0;
        scene_ended = scene_func(timer);
    } while(!scene_ended);
}

//3CBE8
void DISPLAY_ANYSIZE_PICTURE(void* a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    print_once("Not implemented: DISPLAY_ANYSIZE_PICTURE"); //stub
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
    PLANTMPBIT = PLAN3BIT;
    plantmp_length = plan3bit_length;
    plantmp_nb_bytes = plan3bit_nb_bytes;
    PLAN3BIT = PLANVIGBIT[display_Vignet];
    plan3bit_length = planVigbit_length[display_Vignet];
    plan3bit_nb_bytes = planVigbit_nb_bytes[display_Vignet];
}

//3CDD8
void RESTORE_PLAN3(void) {
    PLAN3BIT = PLANTMPBIT;
    plan3bit_length = plantmp_length;
    plan3bit_nb_bytes = plantmp_nb_bytes;
}

//3CDF8
void DISPLAY_FOND3(void) {
    memcpy(DrawBufferNormal, PLAN3BIT, 320*200);
}

//3CE20
void DISPLAY_FOND_MENU(void) {
    s32 v1 = 320 - Bloc_lim_W2 + Bloc_lim_W1;
    if (OptionGame) {
        if (prev_Bloc_lim_W1 != Bloc_lim_W1) {
            if (prev_Bloc_lim_W1 < Bloc_lim_W1) {
                ClearBorder((s16)Bloc_lim_H1, (s16)Bloc_lim_H2, (s16)Bloc_lim_W1, (s16)Bloc_lim_W2);
            }
            Bloc_lim_W1 = prev_Bloc_lim_W1;
        }
        memset(draw_buffer, 0, Bloc_lim_H1 * 320);
        s32 buffer_offset = Bloc_lim_H1 * 320 + Bloc_lim_W1;
        for (s32 y = Bloc_lim_H1; y < Bloc_lim_H2; ++y) {
            u8* dest_pos = draw_buffer + buffer_offset;
            u8* source_pos = EffetBufferNormal + buffer_offset;
            for (s32 x = Bloc_lim_W1; x < Bloc_lim_W2; ++x) {
                *dest_pos++ = *source_pos++;
            }
            buffer_offset += 320;
        }
        memset(draw_buffer, 0, (200 - Bloc_lim_H2) * 320);
    } else {
        memcpy(draw_buffer, EffetBufferNormal, 320*200);
    }
}

// NOTE: ChatGPT (GPT-5.2) was used to clean up InitPaletteSpecialPC() and DoFadePaletteSpecialPC()

// Helpers for InitPaletteSpecialPC
static inline u8 clamp_u8_255(s32 x) {
    // NOTE: In the original PC version the palettes are 6-bit (0..63).
    // However, in Rayverse we're instead working with 8-bit palettes (0..255). -> adjust the clamp accordingly
    if (x < 0)  return 0;
    if (x > 255) return 255;
    return (u8)x;
}

static inline u8 scale_div_clamp(u8 v, s32 mul, s32 div) {
    // assembly does signed division, but inputs are u8 so this is equivalent
    return clamp_u8_255((mul * v) / div);
}

//3CF70
void InitPaletteSpecialPC(void) {
    CompteurEclair = 0;
    bool lightning_level = (num_world == 1 && num_level == 9) || (num_world == 2 && num_level == 4) || (num_world == 4 && num_level == 9);
    if (lightning_level) {
        for (s32 i = 0; i < 256; ++i) {
            rgb_t src = rvb[0].colors[i];

            // pal15 = original (raw copy)
            rvb_special[15].colors[i] = src;

            // pal0: r,g = (4*v)/8 = v/2 ; b = (4*v)/5
            rvb_special[0].colors[i].r  = scale_div_clamp(src.r, 4, 8);
            rvb_special[0].colors[i].g  = scale_div_clamp(src.g, 4, 8);
            rvb_special[0].colors[i].b  = scale_div_clamp(src.b, 4, 5);

            // pal5: (4*v)/3
            rvb_special[5].colors[i].r  = scale_div_clamp(src.r, 4, 3);
            rvb_special[5].colors[i].g  = scale_div_clamp(src.g, 4, 3);
            rvb_special[5].colors[i].b  = scale_div_clamp(src.b, 4, 3);

            // pal7: r,g = (4*v)/6 ; b = (4*v)/4 = v
            rvb_special[7].colors[i].r  = scale_div_clamp(src.r, 4, 6);
            rvb_special[7].colors[i].g  = scale_div_clamp(src.g, 4, 6);
            rvb_special[7].colors[i].b  = scale_div_clamp(src.b, 4, 4);

            // pal10: (8*v)/4 = 2*v
            rvb_special[10].colors[i].r = scale_div_clamp(src.r, 8, 4);
            rvb_special[10].colors[i].g = scale_div_clamp(src.g, 8, 4);
            rvb_special[10].colors[i].b = scale_div_clamp(src.b, 8, 4);

            // pal14: same formula as pal0 in this branch
            rvb_special[14].colors[i].r = scale_div_clamp(src.r, 4, 8);
            rvb_special[14].colors[i].g = scale_div_clamp(src.g, 4, 8);
            rvb_special[14].colors[i].b = scale_div_clamp(src.b, 4, 5);

            rvb[0].colors[i] = rvb_special[0].colors[i];
        }

        // Fill intermediate palettes
        DoFadePaletteSpecialPC(0, 5);
        DoFadePaletteSpecialPC(5, 7);
        DoFadePaletteSpecialPC(7, 10);
        DoFadePaletteSpecialPC(10, 14);

        numero_palette_special = 0;
        ProchainEclair = (s16)(myRand(200) + 1);
    } else if (num_world == 5 && num_level == 4) {
        // First loop runs for 255 colors (0..254) -> 0x2FD bytes copied
        for (int i = 0; i < 255; ++i) {
            rgb_t src = rvb[0].colors[i];

            rvb_special[15].colors[i] = src;

            // pal0: r=min(r,63), g=min(77*g/100,63), b=min(12*b/100,63)
            rvb_special[0].colors[i].r = clamp_u8_255((int)src.r);
            rvb_special[0].colors[i].g = clamp_u8_255((77 * (int)src.g) / 100);
            rvb_special[0].colors[i].b = clamp_u8_255((12 * (int)src.b) / 100);

            // pal14 initially same as pal0 (asm writes to +0x2A00 block too)
            rvb_special[14].colors[i] = rvb_special[0].colors[i];

            rvb[0].colors[i] = rvb_special[0].colors[i];

        }

        DoFadePaletteSpecialPC(0, 14);

        // Second pass: modify palette 9 for color indices 128..174 (ecx=0x180..0x20D step 3)
        for (int i = 128; i <= 174; ++i) {
            rgb_t src = rvb[0].colors[i];

            rvb_special[9].colors[i].r = clamp_u8_255((84 * (int)src.r) / 100);
            rvb_special[9].colors[i].g = clamp_u8_255((62 * (int)src.g) / 100);
            rvb_special[9].colors[i].b = clamp_u8_255((12 * (int)src.b) / 100);

            rvb[0].colors[i] = rvb_special[9].colors[i];
        }

        DoFadePaletteSpecialPC(0, 9);
        DoFadePaletteSpecialPC(9, 14);
    }
}

//3D54C
void DoFadePaletteSpecialPC(s16 start, s16 end) {
    if (start + 1 >= end)
        return;

    s32 denom = end - start;

    for (s32 p = start + 1; p < end; ++p) {
        s32 w0 = end - p;     // weight for start
        s32 w1 = p - start;   // weight for end

        for (s32 i = 0; i < 256; ++i) {
            rgb_t a = rvb_special[start].colors[i];
            rgb_t b = rvb_special[end].colors[i];

            rvb_special[p].colors[i].r = clamp_u8_255((a.r * w0 + b.r * w1) / denom);
            rvb_special[p].colors[i].g = clamp_u8_255((a.g * w0 + b.g * w1) / denom);
            rvb_special[p].colors[i].b = clamp_u8_255((a.b * w0 + b.b * w1) / denom);
        }
    }
}

//3D704
void DoPaletteSpecialPC(void) {
    if (fade & 0x40) {
        if (num_world == 5 && num_level == 4) {
            if (horloge[5] != 0) {
                if (numero_palette_special == 0) {
                    numero_palette_special = 1;
                }
            } else {
                SetPalette(&rvb_special[numero_palette_special], 0, 255);
                ++numero_palette_special;
            }
            if (numero_palette_special == 15) {
                numero_palette_special = 0;
            }
        } else if ((num_world == 1 && num_level == 9) || (num_world == 2 && num_level == 4) || (num_world == 4 && num_level == 9)) {
            ++CompteurEclair;
            if (CompteurEclair >= ProchainEclair || numero_palette_special != 0) {
                SetPalette(&rvb_special[numero_palette_special], 0, 255);
                if (numero_palette_special == 0) {
                    PlaySnd_old(195);
                    ++numero_palette_special;
                } else {
                    if (CompteurEclair % 2 != 0) {
                        // NOTE: the code below is unreachable (statement is always false); seems to be a minor bug in the original game?
                        if (numero_palette_special == 0) {
                            numero_palette_special = 1;
                        }
                    } else {
                        ++numero_palette_special;
                    }
                }
                if (numero_palette_special == 15) {
                    CompteurEclair = 0;
                    numero_palette_special = 0;
                    ProchainEclair = (s16)(myRand(400) + 1);
                }
            }
        }
    }
}

//3D8AC
void InitModeXWithFrequency(u8 freq) {
    print_once("Not implemented: InitModeXWithFrequency"); //stub
}

//3D9D4
void InitTextMode(void) {
    textmode();
    ModeVideoActuel = MODE_TEXT;
}

//3D9E4
void InitModeNormalWithFrequency(u8 freq) {
    if (ModeVideoActuel == MODE_TEXT) {
        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != MODE_NORMAL) {
        InitModeNormal();
//		memset(DrawBufferNormal, 0, 320 * 200);
//		memset(DisplayBufferNormal, 0, 320 * 200);
        select_display_buffer(DisplayBufferNormal);
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_NORMAL;
        DrawSpriteNoClipNormalEtX = DrawSpriteNormalNoClip;
        DrawSpriteFlipNoClipNormalEtX = DrawSpriteFlipNormalNoClip;
        DrawSpriteColorNormalEtX = DrawSpriteColorNormal;
        DrawSpriteColorFlipNormalEtX = DrawSpriteColorFlipNormal;
        draw_buffer = DrawBufferNormal;
        DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
        DrawSpriteNormalEtX = DrawSpriteNormal;
		drawflocon1NormalETX = draw_flocon1_Normal;
		drawflocon2NormalETX = draw_flocon2_Normal;
		drawflocon3NormalETX = draw_flocon3_Normal;
		drawflocon4NormalETX = draw_flocon4_Normal;
		drawflocon5NormalETX = draw_flocon5_Normal;
		drawflocon6NormalETX = draw_flocon6_Normal;
		drawflocon7NormalETX = draw_flocon7_Normal;
		drawpluie4NormalETX = draw_pluie4_Normal;
		drawpluie6NormalETX = draw_pluie6_Normal;
		drawpluie5NormalETX = draw_pluie5_Normal;
		drawpluie7NormalETX = draw_pluie7_Normal;
		fplotNormalETX = fplot_Normal;
    }
}

//3DB24
void WaitNSynchro(s32 n_frames) {
    for (s32 i = 0; i < n_frames; ++i) {
        advance_frame();
    }
}

