
//23F30
void CalcTab(void) {
    u8* row = byte_DA43C;
    for (i32 j = 64; j > -64; --j) {
        u8* pos = row;
        for (i32 i = 64; i > -64; --i) {
            double v = sin(sqrt(j*j + i*i) / 9.5);
            *pos++ = (u8)((v + 1.0) * 40.0);
        }
        row += 128;
    }
}

//23FDC
void InitPlasma(u8 need_set_palette) {
    plasma_palette_color_index = (menuEtape == 0) ? 112 : 74;
    for (i32 i = 0; i < 16; ++i) {
        rgb_t* color = rvb[0].colors + plasma_palette_color_index + i;
        // NOTE: these are 6-bit colors; we convert to 8-bit by multiplying by 4
        // https://retrocomputing.stackexchange.com/questions/27400/what-is-the-most-accurate-way-to-map-6-bit-vga-palette-to-8-bit
        // Also, for some reason the blue and red channels are swapped here.
        color->r = PalPlasma[i].b << 2;
        color->g = PalPlasma[i].g << 2;
        color->b = PalPlasma[i].r << 2;
    }
    if (need_set_palette) {
        SetPalette(&rvb[0], plasma_palette_color_index, 16);
    }
}

//24068
void Plasma(i16 x, i16 y, i16 width, i16 height, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, u8 a10, u8 a11, u8 a12, u8 a13) {
    u8* row = draw_buffer + 320 * y + x;
    u32 v17 = a6 | (a8 << 8) | (a10 << 16) | (a12 << 24);
    for (i32 j = 0; j < height; ++j) {
        v17 = (v17 + 0x01010101) & 0x7F7F7F7F;
        u32 v18 = a5 | (a7 << 8) | (a9 << 16) | (a11 << 24);
        u8* pos = row;
        for (i32 i = 0; i < width; ++i) {
            v18 = (v18 + 0x01010101) & 0x7F7F7F7F;
            u8 v22 = a13 + byte_DA43C[128 * (u8)v17 + (u8)v18];
            v22 += byte_DA43C[128 * (u8)(v17 >> 8) + (u8)(v18 >> 8)];
            v22 += byte_DA43C[128 * (u8)(v17 >> 16) + (u8)(v18 >> 16)];
            v22 += byte_DA43C[128 * (u8)(v17 >> 24) + (u8)(v18 >> 24)];
            v22 >>= 4;
            v22 += plasma_palette_color_index;
            *pos++ = v22;
        }
        row += 320;
    }
}

//241FC
void PlasmaBox(i16 x, i16 y, i16 width, i16 height, u8 a5) {
    if (a5 != 0) {
        word_95FA0 += 1;
        word_95FA2 -= 2;
        word_95FA4 += 1;
        word_95FA6 -= 2;
        word_95FA8 += 3;
        word_95FAA -= 2;
        word_95FAC += 1;
        word_95FAE -= 3;
        word_95FB0 += 5;
        word_DE748 = (sinus(word_95FA0) + 32) >> 4;
        word_DE746 = (cosinus(word_95FA2) + 32) >> 4;
        word_DE74C = (cosinus(word_95FA4) + 32) >> 4;
        word_DE74A = (sinus(word_95FA6) + 32) >> 4;
        word_DE744 = (cosinus(word_95FA8) + 32) >> 4;
        word_DE742 = (sinus(word_95FAA) + 32) >> 4;
        word_DE740 = (cosinus(word_95FAC) + 32) >> 4;
        word_DE73E = (sinus(word_95FAE) + 32) >> 4;
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
    Plasma(x, y, width, height, word_DE74C, word_DE74A, word_DE748, word_DE746, word_DE744, word_DE742, word_DE740, word_DE73E, word_95FB0);
}

//243E0
void InitFire(void) {
    MakeMyRand(1);
    plasma_palette_color_index = (menuEtape != 0) ? 112 : 74;
    for (i32 i = 0; i < 16; ++i) {
        rgb_t* color = rvb[0].colors + plasma_palette_color_index + i;
        // NOTE: I don't understand the swapping of the channels here.
        color->r = PalFire[i].b; // << 2;
        color->g = PalFire[i].r; // << 2;
        color->b = PalFire[i].g; // << 2;
    }
    SetPalette(&rvb[0], plasma_palette_color_index, 16);
    for (i32 i = 0; i < COUNT(p1); ++i) {
        p1[i] = plasma_palette_color_index;
    }
}

//244C4
void Fire(i16 x, i16 y, i16 width, i16 height) {
    u8* row = draw_buffer + 320 * y + x;
    MakeMyRand(0);
    u8 c = plasma_palette_color_index;
    for (i32 i = 0; i < width; ++i) {
        i16 r = myRand(10);
        if (r < 5) {
            c = 15 * myRand(1) + plasma_palette_color_index;
        }
        p1[300 * height + i] = c;
        p1[300 * height + 300 + i] = c;
    }

    for (i32 i = 1; i <= height; ++i) {
        for (i32 j = 0; j < width; ++j){
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

    for (i32 j = 0; j < height; ++j) {
        u8* pos = row;
        for (i32 i = 0; i < width; ++i) {
            *pos++ = p1[300 * j + i];
        }
        row += 320;
    }

}

//24738
void FireBox(i16 x, i16 y, i16 width, i16 height) {
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
    rgb_palette_t* cur_pal = rvb + current_pal_id;
    MenuPalette = rvb[current_pal_id];
    {
        // red-green gradient
        u8 r = 126;
        u8 g = 0;
        u8 b = 0;
        for (i32 i = 70, j = 0; i < 102; ++i, ++j) {
            MenuPalette.colors[i].r = r;
            MenuPalette.colors[i].g = g;
            MenuPalette.colors[i].b = b;
            r -= 2;
            g += 2;
        }
    }
    for (i32 i = 112, j = 0; i < 128; ++i, ++j) {
        MenuPalette.colors[i] = PalPlasma[j];
    }
    // grayscale palette
    for (i32 i = 128, j = 0; i < 192; ++i, ++j) {
        u8 c = j << 2;
        MenuPalette.colors[i].r = c;
        MenuPalette.colors[i].g = c;
        MenuPalette.colors[i].b = c;
    }
    if (dans_la_map_monde) {
        MenuPalette.colors[32].r = 16 << 2;
        MenuPalette.colors[32].g = 14 << 2;
        MenuPalette.colors[32].b = 13 << 2;
        MenuPalette.colors[38].r = 49 << 2;
        MenuPalette.colors[38].g = 47 << 2;
        MenuPalette.colors[38].b = 45 << 2;

    }
}

//2494C
void CaptureVideo(u8* source_buf, u8* dest_buf, u8 grayscale) {
    if (ModeVideoActuel == MODE_X) {
        print_once("Not implemented: CaptureVideo"); //stub
    } else {
        if (grayscale) {
            u8* source_pos = source_buf;
            u8* dest_pos = dest_buf;
            rgb_palette_t* palette = rvb + current_pal_id;
            for (i32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
                u8 c = *source_pos++;
                rgb_t rgb = palette->colors[c];
                u8 gray = ((rgb.r + rgb.g + rgb.b) >> 2) / 3 + 128;
                *dest_pos++ = gray;
            }
        } else {
            memcpy(dest_buf, source_buf, SCREEN_WIDTH * SCREEN_HEIGHT);
        }
    }
}

//24A98
void CadreTrans(i16 x, i16 y, i16 w, i16 h) {
    u8* line = draw_buffer + SCREEN_WIDTH * y + x;
    for (i32 j = 0; j < h; ++j) {
        u8* pos = line;
        for (i32 i = 0; i < w; ++i) {
            if (*pos) {
                *pos += 0x40;
            } else {
                *pos = 0xC8;
            }
        }
        line += SCREEN_WIDTH;
    }
    DrawBorderBoxNormal(draw_buffer, x, y, h, w, 0x2026);
}

//24B80
void DrawBlackBorderBox(i16 x, i16 y, i16 height, i16 width, u8 a5) {
    u16 colors = a5 ? 0x2026 : 0x2620;
    DrawBlackBoxNormal(draw_buffer, x, y, height, width);
    DrawBorderBoxNormal(draw_buffer, x, y, height, width, colors);
}

//24BF0
void InitGauge(void) {
    print_once("Not implemented: InitGauge"); //stub
}

//24C54
void gauge(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    print_once("Not implemented: gauge"); //stub
}

//24DCC
void ClearBorder(i16 lim_H1, i16 lim_H2, i16 lim_W1, i16 lim_W2) {
    memset(draw_buffer, 0, lim_H1 * 320);
    memset(draw_buffer, 0, (200 - lim_H2) * 320);
    for (i32 y = lim_H1; y < lim_H2; ++y) {
        u8* row = draw_buffer + 320 * y;
        if (lim_W1 > 0) {
            memset(row, 0, lim_W1);
        }
        if (lim_W2 < 320) {
            memset(row + lim_W2, 0, 320 - lim_W2);
        }
    }
}
