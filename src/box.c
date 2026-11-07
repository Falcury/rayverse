
//23F30
void CalcTab(void) {
    u8* row = plasma_radial_lut;
    for (s32 j = 64; j > -64; --j) {
        u8* pos = row;
        for (s32 i = 64; i > -64; --i) {
            u8 radius = (u8)sqrt(j*j + i*i);
            double v = sin(radius * (1.0 / 9.5));
            *pos++ = (u8)((v + 1.0) * 40.0);
        }
        row += 128;
    }
}

//23FDC
void InitPlasma(u8 need_set_palette) {
    // Submenus reserve 70..101 for gauges; PC places their plasma at 112.
    plasma_palette_color_index = (menuEtape == 0) ? 74 : 112;
    for (s32 i = 0; i < 16; ++i) {
        rgb_t* color = rvb[0].colors + plasma_palette_color_index + i;
        // The original table stores BGR DAC values.
        color->r = palette_dac_value(PalPlasma[i].b);
        color->g = palette_dac_value(PalPlasma[i].g);
        color->b = palette_dac_value(PalPlasma[i].r);
    }
    if (need_set_palette) {
        SetPalette(&rvb[0], plasma_palette_color_index, 16);
    }
}

//24068
void Plasma(s16 x, s16 y, s16 width, s16 height,
            u8 layer1_offset_x, u8 layer1_offset_y,
            u8 layer2_offset_x, u8 layer2_offset_y,
            u8 layer3_offset_x, u8 layer3_offset_y,
            u8 layer4_offset_x, u8 layer4_offset_y,
            u8 color_phase
) {
    u8* row = draw_buffer + 320 * y + x;
    u32 v17 = layer1_offset_y | (layer2_offset_y << 8) | (layer3_offset_y << 16) | (layer4_offset_y << 24);
    for (s32 j = 0; j < height; ++j) {
        v17 = (v17 + 0x01010101) & 0x7F7F7F7F;
        u32 v18 = layer1_offset_x | (layer2_offset_x << 8) | (layer3_offset_x << 16) | (layer4_offset_x << 24);
        u8* pos = row;
        for (s32 i = 0; i < width; ++i) {
            v18 = (v18 + 0x01010101) & 0x7F7F7F7F;
            u8 v22 = color_phase + plasma_radial_lut[128 * (u8)v17 + (u8)v18];
            v22 += plasma_radial_lut[128 * (u8)(v17 >> 8) + (u8)(v18 >> 8)];
            v22 += plasma_radial_lut[128 * (u8)(v17 >> 16) + (u8)(v18 >> 16)];
            v22 += plasma_radial_lut[128 * (u8)(v17 >> 24) + (u8)(v18 >> 24)];
            v22 >>= 4;
            v22 += plasma_palette_color_index;
            *pos++ = v22;
        }
        row += 320;
    }
}

//241FC
void PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 a5) {
    if (a5 != 0) {
        plasma_layer2_phase_x += 1;
        plasma_layer2_phase_y -= 2;
        plasma_layer1_phase_x += 1;
        plasma_layer1_phase_y -= 2;
        plasma_layer3_phase_x += 3;
        plasma_layer3_phase_y -= 2;
        plasma_layer4_phase_x += 1;
        plasma_layer4_phase_y -= 3;
        plasma_color_phase += 5;
        plasma_layer2_offset_x = (sinus(plasma_layer2_phase_x) + 32) >> 4;
        plasma_layer2_offset_y = (cosinus(plasma_layer2_phase_y) + 32) >> 4;
        plasma_layer1_offset_x = (cosinus(plasma_layer1_phase_x) + 32) >> 4;
        plasma_layer1_offset_y = (sinus(plasma_layer1_phase_y) + 32) >> 4;
        plasma_layer3_offset_x = (cosinus(plasma_layer3_phase_x) + 32) >> 4;
        plasma_layer3_offset_y = (sinus(plasma_layer3_phase_y) + 32) >> 4;
        plasma_layer4_offset_x = (cosinus(plasma_layer4_phase_x) + 32) >> 4;
        plasma_layer4_offset_y = (sinus(plasma_layer4_phase_y) + 32) >> 4;
    }
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    } else if (x + width > 320) {
        width = 320 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    } else if (y + height > 200) {
        height = 200 - y;
    }
    Plasma(x, y, width, height,
           plasma_layer1_offset_x, plasma_layer1_offset_y,
           plasma_layer2_offset_x, plasma_layer2_offset_y,
           plasma_layer3_offset_x, plasma_layer3_offset_y,
           plasma_layer4_offset_x, plasma_layer4_offset_y,
           plasma_color_phase);
}

//243E0
void InitFire(void) {
    MakeMyRand(1);
    plasma_palette_color_index = (menuEtape != 0) ? 112 : 74;
    for (s32 i = 0; i < 16; ++i) {
        rgb_t* color = rvb[0].colors + plasma_palette_color_index + i;
        // Unlike PalPlasma, PC divides PalFire by four and uses offsets 0,2,1.
        // RGB8 mode retains this table's low bits, just like RGB8 PCX assets.
        color->r = use_rgb8_palettes ? PalFire[i].r : PalFire[i].r >> 2;
        color->g = use_rgb8_palettes ? PalFire[i].b : PalFire[i].b >> 2;
        color->b = use_rgb8_palettes ? PalFire[i].g : PalFire[i].g >> 2;
    }
    SetPalette(&rvb[0], plasma_palette_color_index, 16);
    for (s32 i = 0; i < COUNT(p1); ++i) {
        p1[i] = plasma_palette_color_index;
    }
}

//244C4
void Fire(s16 x, s16 y, s16 width, s16 height) {
    u8* row = draw_buffer + 320 * y + x;
    MakeMyRand(0);
    u8 c = plasma_palette_color_index;
    for (s32 i = 0; i < width; ++i) {
        s16 r = myRand(10);
        if (r < 5) {
            c = 15 * myRand(1) + plasma_palette_color_index;
        }
        p1[300 * height + i] = c;
        p1[300 * height + 300 + i] = c;
    }

    for (s32 i = 1; i <= height; ++i) {
        for (s32 j = 0; j < width; ++j){
            if (j != 0) {
                if (j == width - 1) {
                    p1[300 * i - 300 + j] =
                            (p1[300 * i + 300] +
                             p1[300 * i + j] +
                             p1[300 * i + j - 1] +
                             p1[300 * i + j + 300]) >> 2;
                } else {
                    p1[300 * i - 300 + j] =
                            (p1[300 * i + j + 1] +
                            p1[300 * i + j] +
                            p1[300 * i + j - 1] +
                            p1[300 * i + j + 300]) >> 2;
                }
            } else {
                p1[300 * i - 300 + j] =
                        (p1[300 * i - 301 + width] +
                         p1[300 * i] +
                         p1[300 * i - 1] +
                         p1[300 * i + 300]) >> 2;
            }
        }
    }

    for (s32 j = 0; j < height; ++j) {
        u8* pos = row;
        for (s32 i = 0; i < width; ++i) {
            *pos++ = p1[300 * j + i];
        }
        row += 320;
    }

}

//24738
void FireBox(s16 x, s16 y, s16 width, s16 height) {
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (x + width > 320) {
        width = 320 - x;
    }
    if (y + height > 200) {
        height = 200 - y;
    }
    Fire(x, y, width, height);
}

//2479C
void InitMenuPalette(void) {
    MenuPalette = rvb[current_pal_id];
    {
        // red-green gradient
        u8 r = 126;
        u8 g = 0;
        u8 b = 0;
        for (s32 i = 70, j = 0; i < 102; ++i, ++j) {
            // Native mode retains PC's 126..64 red bytes until DAC upload.
            // RGB8 masks before expansion to fit in a byte: final colors match
            // PC, but interpolation uses the visible ramp, not PC's raw bytes.
            MenuPalette.colors[i].r = palette_dac_value(r);
            MenuPalette.colors[i].g = palette_dac_value(g);
            MenuPalette.colors[i].b = b;
            r -= 2;
            g += 2;
        }
    }
    for (s32 i = 112, j = 0; i < 128; ++i, ++j) {
        // The PC table is BGR, with six-bit channels (same as InitPlasma).
        MenuPalette.colors[i].r = palette_dac_value(PalPlasma[j].b);
        MenuPalette.colors[i].g = palette_dac_value(PalPlasma[j].g);
        MenuPalette.colors[i].b = palette_dac_value(PalPlasma[j].r);
    }
    // grayscale palette
    for (s32 i = 128, j = 0; i < 192; ++i, ++j) {
        u8 c = palette_dac_value(j);
        MenuPalette.colors[i].r = c;
        MenuPalette.colors[i].g = c;
        MenuPalette.colors[i].b = c;
    }
    // blue overlay over grayscale
    for (s32 i = 192, j = 0; i < 256; ++i, ++j) {
        u8 c = j;
        if (c <= 3) {
            c = 3;
        }
        c = palette_dac_value(c);
        MenuPalette.colors[i].r = 0;
        MenuPalette.colors[i].g = 0;
        MenuPalette.colors[i].b = c;
    }
    if (dans_la_map_monde) {
        MenuPalette.colors[32].r = palette_dac_value(16);
        MenuPalette.colors[32].g = palette_dac_value(14);
        MenuPalette.colors[32].b = palette_dac_value(13);
        MenuPalette.colors[38].r = palette_dac_value(49);
        MenuPalette.colors[38].g = palette_dac_value(47);
        MenuPalette.colors[38].b = palette_dac_value(45);

    }
}

//2494C
void CaptureVideo(u8* source_buf, u8* dest_buf, u8 grayscale) {
    if (ModeVideoActuel == MODE_X) {
        // PC reads four VGA planes here; Android only has the linear path below.
        // Keep this deferred until Rayverse has an actual Mode-X buffer layout.
        print_once("Not implemented: CaptureVideo"); //stub
    } else {
        if (grayscale) {
            u8* source_pos = source_buf;
            u8* dest_pos = dest_buf;
            rgb_palette_t* palette = rvb + current_pal_id;
            for (s32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
                u8 c = *source_pos++;
                rgb_t rgb = palette->colors[c];
                u8 gray = palette_gray_index(rgb);
                *dest_pos++ = gray;
            }
        } else {
            memcpy(dest_buf, source_buf, SCREEN_WIDTH * SCREEN_HEIGHT);
        }
    }
}

//24A98
void CadreTrans(s16 x, s16 y, s16 w, s16 h) {
    u8* line = draw_buffer + SCREEN_WIDTH * y + x;
    for (s32 j = 0; j < h; ++j) {
        u8* pos = line;
        for (s32 i = 0; i < w; ++i) {
            if (*pos) {
                *pos += 0x40;
            } else {
                *pos = 0xC8;
            }
            ++pos;
        }
        line += SCREEN_WIDTH;
    }
    DrawBorderBoxNormal(draw_buffer, x, y, h, w, 0x2026);
}

//24B80
void DrawBlackBorderBox(s16 x, s16 y, s16 height, s16 width, u8 a5) {
    u16 colors = a5 ? 0x2026 : 0x2620;
    DrawBlackBoxNormal(draw_buffer, x, y, height, width);
    DrawBorderBoxNormal(draw_buffer, x, y, height, width, colors);
}

//24BF0
void InitGauge(void) {
    if (menuEtape != 0 && !OptionGame) {
        for (s32 i = 0; i < 32; ++i) {
            rgb_t* color = &rvb[current_pal_id].colors[70 + i];
            color->r = palette_dac_value(63 - 2 * i);
            color->g = palette_dac_value(2 * i);
            color->b = 0;
        }
    }
}

//24C54
void gauge(s16 x, s16 y, s16 width, s16 height, s16 minimum, s16 maximum, s16 value) {
    const s32 thumb_width = MAX(width >> 3, 8);
    // Original callers use on-screen 128x12 and 80x12 gauges with nonempty ranges.
    // The border blitter also writes at x + width and y + height.
    if (width < thumb_width + 3 || height < 4 || maximum <= minimum ||
        x < 0 || y < 0 || x + width >= SCREEN_WIDTH || y + height >= SCREEN_HEIGHT) {
        return;
    }
    const s32 amount = MAX(minimum, MIN(value, maximum)) - minimum;
    const s32 thumb_x = x + 1 + amount * (width - thumb_width - 3) / (maximum - minimum);

    DrawBorderBoxNormal(draw_buffer, x, y, height, width, 0x2026);
    for (s32 row = 0; row < height - 2; ++row) {
        u8* dest = draw_buffer + (y + row + 1) * SCREEN_WIDTH + x + 1;
        s32 color = 70;
        s32 run = 0;
        for (s32 col = 0; col < width - 2; ++col) {
            dest[col] = (u8)color;
            // Preserve PC's post-write test, including its longer first band.
            if (run > ((width - 2) >> 5)) {
                run = 0;
                ++color;
            }
            ++run;
        }
    }
    DrawBlackBoxNormal(draw_buffer, thumb_x, y + 1, height - 3, thumb_width);
    DrawBorderBoxNormal(draw_buffer, thumb_x, y + 1, height - 3, thumb_width, 0x0308);
}

//24DCC
void ClearBorder(s16 lim_H1, s16 lim_H2, s16 lim_W1, s16 lim_W2) {
    memset(draw_buffer, 0, lim_H1 * SCREEN_WIDTH);
    memset(draw_buffer + lim_H2 * SCREEN_WIDTH, 0, (SCREEN_HEIGHT - lim_H2) * SCREEN_WIDTH);
    for (s32 y = lim_H1; y < lim_H2; ++y) {
        u8* row = draw_buffer + SCREEN_WIDTH * y;
        if (lim_W1 > 0) {
            memset(row, 0, lim_W1);
        }
        if (lim_W2 < SCREEN_WIDTH) {
            memset(row + lim_W2, 0, SCREEN_WIDTH - lim_W2);
        }
    }
}
