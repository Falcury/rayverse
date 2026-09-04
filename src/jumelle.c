
enum {
    JUMELLE_MIN_RADIUS = 25,
    JUMELLE_MAX_RADIUS = 64,
    JUMELLE_ZOOM_CENTER_Y = 64,
    JUMELLE_ZOOM_LEFT_CENTER_X = 64,
    JUMELLE_ZOOM_RIGHT_CENTER_X = 159,
    JUMELLE_ZOOM_MATRIX_STRIDE = 224,
    JUMELLE_ZOOM_MATRIX_HEIGHT = 129,
    JUMELLE_ZOOM_MATRIX_CAPACITY = 29025
};

static void add_jumelle_zoom_offset(s32 x, s32 y, s32 offset) {
    if (x >= 0 && x < JUMELLE_ZOOM_MATRIX_STRIDE &&
        y >= 0 && y < JUMELLE_ZOOM_MATRIX_HEIGHT) {
        JumelleZoomDef[y * JUMELLE_ZOOM_MATRIX_STRIDE + x] += offset;
    }
}

/* Canonical Android name for the helper corresponding to the PC rep-stos
 * clear. The Android port extracts it because its table layout is larger. */
static void ClearJumelles(s32* zoom_def, size_t count) {
    memset(zoom_def, 0, count * sizeof(*zoom_def));
}

/* Android extracts this distorted copy loop under the canonical name
 * MoveWithZoom; the PC executable inlines it at all six scanline sites. */
static void MoveWithZoom(s32 source_offset, s32 dest_offset, s32 matrix_x, s32 matrix_y,
                         s32 width, const u8* effet_buf, u8* draw_buf) {
    /* The PC renderer stores a byte displacement for every pixel in a
     * 224-by-129 lookup grid around the maximum-size binocular mask. */
    for (s32 i = 0; i < width; ++i) {
        s32 source_pixel = source_offset + i;
        s32 lookup_x = matrix_x + i;
        if (matrix_y >= 0 && matrix_y < JUMELLE_ZOOM_MATRIX_HEIGHT &&
            lookup_x >= 0 && lookup_x < JUMELLE_ZOOM_MATRIX_STRIDE) {
            source_pixel += JumelleZoomDef[matrix_y * JUMELLE_ZOOM_MATRIX_STRIDE + lookup_x];
        }

        /* The original relies on the circle geometry to keep these accesses
         * inside the 320x200 scratch image. Keep that invariant explicit in
         * the portable renderer so a malformed state cannot corrupt memory. */
        if (source_pixel >= 0 && source_pixel < SCREEN_WIDTH * SCREEN_HEIGHT) {
            draw_buf[dest_offset + i] = effet_buf[source_pixel];
        }
    }
}

static void copy_jumelle_scanline(s32 jumelle_x, s32 jumelle_y, s32 screen_x, s32 screen_y,
                                  s32 width, const u8* effet_buf, u8* draw_buf) {
    if (width <= 0) {
        return;
    }

    s32 dest_offset = screen_y * SCREEN_WIDTH + screen_x;
    s32 source_offset = dest_offset - (JumelleYMin * SCREEN_WIDTH + JumelleXMin);
    if (JumelleZoomActif && JumelleZoomDef) {
        s32 matrix_y = screen_y - (jumelle_y - JUMELLE_ZOOM_CENTER_Y);
        s32 matrix_x = screen_x - (jumelle_x - 112);
        MoveWithZoom(source_offset, dest_offset, matrix_x, matrix_y, width, effet_buf, draw_buf);
    } else {
        memcpy(draw_buf + dest_offset, effet_buf + source_offset, (size_t)width);
    }
}

//42930
void plot2linejumelle(s32 jumelle_x, s32 jumelle_y, s32 plot_x, s32 plot_y, s32 circle2_offset, u8* effet_buf, u8* draw_buf) {
    s32 v7 = jumelle_x - plot_x - circle2_offset;
    if (plot_x <= circle2_offset) {
        s32 v60 = 2 * plot_x;
        if (v7 < 0) {
            v60 += v7;
            v7 = 0;
        }
        if (v7 + v60 >= SCREEN_WIDTH) {
            v60 = SCREEN_WIDTH - v7 - 1;
        }
        s32 v25 = circle2_offset + jumelle_x - plot_x;
        s32 v59 = 2 * plot_x;
        s32 v57 = v25;
        if (v25 < 0) {
            v59 += v25;
            v57 = 0;
        }
        if (v59 + v57 >= SCREEN_WIDTH) {
            v59 = SCREEN_WIDTH - v57 - 1;
        }

        // Draw top segments
        s32 v26 = jumelle_y - plot_y;
        s32 v54 = v26;
        if (jumelle_y - plot_y >= 0 && v26 < SCREEN_HEIGHT) {
            // Top segment of left circle
            if (v60 > 0) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v7, v26, v60, effet_buf, draw_buf);
            }

            // Top segment of right circle
            if (v59 > 0) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v57, v54, v59, effet_buf, draw_buf);
            }
        }

        // Draw bottom segments
        s32 v8 = plot_y + jumelle_y;
        s32 v52 = v8;
        if (v8 >= 0 && v8 < SCREEN_HEIGHT) {
            // Bottom segment of left circle
            if (v60 > 0) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v7, v52, v60, effet_buf, draw_buf);
            }

            if (v59 > 0) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v57, v52, v59, effet_buf, draw_buf);
            }
        }
    } else {
        // Draw middle part of both circles
        s32 v58 = 2 * (plot_x + circle2_offset);
        if (v7 < 0) {
            v58 += v7;
            v7 = 0;
        }
        if (v7 + v58 >= SCREEN_WIDTH) {
            v58 = SCREEN_WIDTH - v7 - 1;
        }
        if (v58 > 0) {
            s32 v9 = jumelle_y - plot_y;
            if (v9 >= 0 && v9 < SCREEN_HEIGHT) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v7, v9, v58, effet_buf, draw_buf);
            }

            s32 v16 = jumelle_y + plot_y;
            if (v16 >= 0 && v16 < SCREEN_HEIGHT) {
                copy_jumelle_scanline(jumelle_x, jumelle_y, v7, v16, v58, effet_buf, draw_buf);
            }
        }
    }

}

bool skip_jumelle = false;

//42E1C
void DisplayJumellesNormal(s32 x, s32 y, s32 rayon, s32 a4, u8* effet_buf, u8* draw_buf) {
    // NOTE: this is debug code!
    if (skip_jumelle) {
#if 1
        for (s32 i = 0; i < HauteurJumelle; ++i) {
            if (JumelleYMin + i >= 0 && JumelleYMin + i < SCREEN_HEIGHT) {
                s32 width = LargeurJumelle;
                s32 draw_x = JumelleXMin;
                s32 source_x = 0;
                if (JumelleXMin < 0) {
                    source_x = -JumelleXMin;
                    width += JumelleXMin;
                    draw_x = 0;
                }
                if (JumelleXMin + LargeurJumelle > SCREEN_WIDTH) {
                    width -= (JumelleXMin + LargeurJumelle) - SCREEN_WIDTH;
                }
                memcpy(draw_buf + SCREEN_WIDTH * (JumelleYMin + i) + draw_x,
                       effet_buf + SCREEN_WIDTH * i + source_x, (size_t)width);
            } else {
                continue;
            }
        }

#else
        memcpy(draw_buf, effet_buf, 320*200);
#endif
    }

    s32 v6 = 0;
    s32 v7 = 0;
    s32 v8 = rayon;
    while (v8 >= v7) {
        plot2linejumelle(x, y, v7, v8, a4, effet_buf, draw_buf);
        plot2linejumelle(x, y, v8, v7, a4, effet_buf, draw_buf);
        v6 = v6 + 2 * v7 + 1;
        ++v7;
        if (v6 >= v8) {
            v6 = v6 - 2 * v8 + 1;
            --v8;
        }
    }
    if (ParamZoomChange) {
        PrepareJumelleZoom();
    }
}

//42EC4
void InitMatriceJumelle(void) {
    if (JumelleEffetAutorise) {
        DistPointX = (s32*)calloc(161, sizeof(s32));
        DistPointY = (s32*)calloc(65, sizeof(s32));
        ExpPoint = (s32*)calloc(5120, sizeof(s32));
        JumelleZoomDef = (s32*)calloc(JUMELLE_ZOOM_MATRIX_CAPACITY, sizeof(s32));
        if (DistPointX && DistPointY && ExpPoint && JumelleZoomDef) {
            for (s32 y = 0; y < 65; ++y) {
                DistPointY[y] = y * y;
            }
            // NOTE: the for loop for X is inside the Y loop in the original (probably a bug?)
            for (s32 x = 0; x < 161; ++x) {
                DistPointX[x] = x * x;
            }
            for (s32 i = 0; i < 5120; ++i) {
                ExpPoint[i] = (s32)(exp((double)-i / 1024.0) * 65536.0);
            }
        } else {
            if (DistPointX) {
                free(DistPointX);
                DistPointX = NULL;
            }
            if (DistPointY) {
                free(DistPointY);
                DistPointY = NULL;
            }
            if (ExpPoint) {
                free(ExpPoint);
                ExpPoint = NULL;
            }
            if (JumelleZoomDef) {
                free(JumelleZoomDef);
                JumelleZoomDef = NULL;
            }
            JumelleEffetAutorise = 0;
            fatal_error();
        }
    }
    DefaultJumelleVariable();
}

//43038
void DefaultJumelleVariable(void) {
    if (JumelleEffetAutorise) {
        JumelleZoomActif = 1;
    } else {
        JumelleZoomActif = 0;
    }
    ChangeJumelleSizeOK = 3;
    RayonJumelle = JUMELLE_MIN_RADIUS;
    LargeurJumelle = 87;
    HauteurJumelle = 50;
    JumellePosX = 160;
    JumellePosY = 100;
    PositionJumelleX16 = 2560;
    PositionJumelleY16 = 1600;
    ModeAutoJumelle = 0;
    PositionStageNameCalcule = 0;
    ChangeJumelleVariable();
    if (JumelleEffetAutorise) {
        JumelleZoomAmp = -26 * RayonJumelle + 32150;
        PrepareJumelleZoom();
    }
}

//430FC
void ChangeDeltaSizeJumelle(s32 delta_size) {
    /* Android deliberately widened this range to 30..69 and uses a larger
     * lookup layout. The PC world-map state machine requires 25..64. */
    RayonJumelle = MAX(JUMELLE_MIN_RADIUS, MIN(JUMELLE_MAX_RADIUS, RayonJumelle + delta_size));
    ChangeJumelleVariable();
    if (JumelleZoomActif) {
        ChangeDeltaZoomAmpJumelle(delta_size <= 0 ? 65 : -65);
    }
}

//4314C
void CalculVitesseJumelle(void) {
    s32 delta_x = PositionJumelleDemandeX - JumellePosX;
    s32 delta_y = PositionJumelleDemandeY - JumellePosY;
    s32 dist = Abs(delta_y) + Abs(delta_x);
    if (dist > 0) {
        delta_x = ashl16(delta_x, 6) / dist;
        delta_y = ashl16(delta_y, 6) / dist;
    }
    XSpeedJumelle = MAX(MIN(XSpeedJumelle + sgn(delta_x - XSpeedJumelle), 16), -16);
    YSpeedJumelle = MAX(MIN(YSpeedJumelle + sgn(delta_y - YSpeedJumelle), 16), -16);
}

//43230
void ChangeDeltaPosXJumelleWithoutLimit(s32 delta_pos_x) {
    delta_pos_x = MAX(-16, MIN(16, delta_pos_x));

    if (ModeAutoJumelle & 1) {
        Xmap16 = (PositionJumelleX16 & 0xF) | (Xmap16 & ~0xF);
        Xmap16 = MAX(16 * scroll_start_x, MIN(16 * scroll_end_x, delta_pos_x + Xmap16));
        xmap = Xmap16 >> 4;
    } else {
        xmap = MAX(scroll_start_x, MIN(scroll_end_x, xmap + (delta_pos_x >> 4)));
    }

    PositionJumelleX16 += delta_pos_x;
    JumellePosX = PositionJumelleX16 >> 4;
    ChangeJumelleVariable();
}

//43300
void ChangeDeltaPosYJumelleWithoutLimit(s32 delta_pos_y) {
    delta_pos_y = MAX(-16, MIN(16, delta_pos_y));

    if (ModeAutoJumelle & 1) {
        Ymap16 = (PositionJumelleY16 & 0xF) | (Ymap16 & ~0xF);
        Ymap16 = MAX(16 * scroll_start_y, MIN(16 * scroll_end_y, delta_pos_y + Ymap16));
        ymap = Ymap16 >> 4;
    } else {
        ymap = MAX(scroll_start_y, MIN(scroll_end_y, ymap + (delta_pos_y >> 4)));
    }

    PositionJumelleY16 += delta_pos_y;
    JumellePosY = PositionJumelleY16 >> 4;
    ChangeJumelleVariable();
}

//433D0
s32 ChangeDeltaPosXJumelleWithLimit(s32 delta_pos_x) {
    delta_pos_x = MAX(-16, MIN(16, delta_pos_x));

    bool changed = false;
    if (((xmap != scroll_start_x && xmap != scroll_end_x) ||
        (xmap == scroll_start_x && delta_pos_x > 0) ||
        (xmap == scroll_end_x && delta_pos_x < 0)) &&
        JumellePosX > RayonJumelle / 3 && JumellePosX < SCREEN_WIDTH - RayonJumelle / 3
    ) {
        if (ModeAutoJumelle & 1) {
            Xmap16 = (PositionJumelleX16 & 0xF) | (Xmap16 & ~0xF);
            Xmap16 = MAX(16 * scroll_start_x, MIN(16 * scroll_end_x, delta_pos_x + Xmap16));
            xmap = Xmap16 >> 4;
        } else {
            xmap = MAX(scroll_start_x, MIN(scroll_end_x, xmap + (delta_pos_x >> 4)));
        }

        PositionJumelleX16 += delta_pos_x;
        JumellePosX = PositionJumelleX16 >> 4;
        changed = true;
    }

    ChangeJumelleVariable();
    return changed;
}

//43520
s32 ChangeDeltaPosYJumelleWithLimit(s32 delta_pos_y) {
    delta_pos_y = MAX(-16, MIN(16, delta_pos_y));

    bool changed = false;
    if (((ymap != scroll_start_y && ymap != scroll_end_y) ||
         (ymap == scroll_start_y && delta_pos_y > 0) ||
         (ymap == scroll_end_y && delta_pos_y < 0)) &&
        JumellePosY > RayonJumelle / 3 && JumellePosY < SCREEN_HEIGHT - RayonJumelle / 3
            ) {
        if (ModeAutoJumelle & 1) {
            Ymap16 = (PositionJumelleY16 & 0xF) | (Ymap16 & ~0xF);
            Ymap16 = MAX(16 * scroll_start_y, MIN(16 * scroll_end_y, delta_pos_y + Ymap16));
            ymap = Ymap16 >> 4;
        } else {
            ymap = MAX(scroll_start_y, MIN(scroll_end_y, ymap + (delta_pos_y >> 4)));
        }

        PositionJumelleY16 += delta_pos_y;
        JumellePosY = PositionJumelleY16 >> 4;
        changed = true;
    }

    ChangeJumelleVariable();
    return changed;
}

//4368C
void ChangeJumelleVariable(void) {
    LargeurJumelle = (7 * RayonJumelle) / 2;
    HauteurJumelle = 2 * RayonJumelle;
    s32 half_width = LargeurJumelle / 2;
    s32 half_height = ((2 * RayonJumelle) / 2);
    JumelleXMin = JumellePosX - half_width;
    JumelleYMin = JumellePosY - half_height;
    scroll_start_x = (s16)(half_width + 2);
    scroll_start_y = (s16)(half_height + 2);
    scroll_end_x = (s16)(WidthNworld - half_width - 4);
    scroll_end_y = (s16)(HeightNworld - half_height - 4);

//    scroll_start_x = (half_width + 2);
//    scroll_start_y = (half_height + 2);
//    scroll_end_x = WidthNworld - SCREEN_WIDTH + 32;
//    scroll_end_y = HeightNworld - SCREEN_HEIGHT + 32;
}

//43730
void ChangeDeltaZoomAmpJumelle(s32 a1) {
    if ((JumAmpDefPlus + JumAmpDefMul * -RayonJumelle == JumelleZoomAmp &&
        (!(ModeAutoJumelle & 2) || JumelleZoomAmp == ModeAutoJumelleZoomAmp)) || !JumelleEffetAutorise
    ) {
        if (JumelleZoomAmp != 0 && JumelleEffetAutorise) {
            JumelleZoomActif = 1;
            ParamZoomChange = 0;
        } else {
            JumelleZoomActif = 0;
            ParamZoomChange = 0;
        }
    } else {
        JumelleZoomAmp = MAX(0, MIN(-26 * RayonJumelle + 32150, a1 + JumelleZoomAmp));
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
        ChangeJumelleVariable();
    }
}

//437FC
void PrepareJumelleZoom(void) {
    if (!JumelleEffetAutorise || !DistPointX || !DistPointY || !ExpPoint ||
        !JumelleZoomDef || RayonJumelle <= 0 || RayonJumelle > JUMELLE_MAX_RADIUS ||
        LargeurJumelle < RayonJumelle || LargeurJumelle - RayonJumelle > 161) {
        return;
    }

    s32 radius_scale = 0x200000 / (RayonJumelle * RayonJumelle);
    s32 center_margin = (3 * (JUMELLE_MAX_RADIUS - RayonJumelle)) >> 2;
    s32 left_center_x = JUMELLE_ZOOM_LEFT_CENTER_X + center_margin;
    s32 right_center_x = JUMELLE_ZOOM_RIGHT_CENTER_X - center_margin;

    ClearJumelles(JumelleZoomDef, JUMELLE_ZOOM_MATRIX_CAPACITY);
    ParamZoomChange = 0;

    for (s32 y = 0; y <= RayonJumelle; ++y) {
        for (s32 x = 0; x < LargeurJumelle - RayonJumelle; ++x) {
            s32 scaled_distance = (DistPointX[x] + DistPointY[y]) * radius_scale;
            if ((u32)scaled_distance >= 0x500000) {
                continue;
            }

            s32 exponential = ExpPoint[(u32)scaled_distance >> 10];
            s32 displacement_scale = (JumelleZoomAmp * exponential) >> 16;
            s32 horizontal = (x * displacement_scale + 0x8000) >> 16;
            s32 vertical = ((y * displacement_scale + 0x8000) >> 16) * SCREEN_WIDTH;

            if (x != 0) {
                add_jumelle_zoom_offset(left_center_x + x, JUMELLE_ZOOM_CENTER_Y - y,
                                         vertical - horizontal);
                add_jumelle_zoom_offset(right_center_x - x, JUMELLE_ZOOM_CENTER_Y + y,
                                         horizontal - vertical);
            }
            if (y != 0) {
                add_jumelle_zoom_offset(left_center_x + x, JUMELLE_ZOOM_CENTER_Y + y,
                                         -horizontal - vertical);
                add_jumelle_zoom_offset(right_center_x - x, JUMELLE_ZOOM_CENTER_Y - y,
                                         horizontal + vertical);
            }
            if (x <= RayonJumelle) {
                if (y != 0) {
                    add_jumelle_zoom_offset(left_center_x - x, JUMELLE_ZOOM_CENTER_Y - y,
                                             horizontal + vertical);
                    add_jumelle_zoom_offset(right_center_x + x, JUMELLE_ZOOM_CENTER_Y + y,
                                             -horizontal - vertical);
                }
                if (x != 0) {
                    add_jumelle_zoom_offset(left_center_x - x, JUMELLE_ZOOM_CENTER_Y + y,
                                             horizontal - vertical);
                    add_jumelle_zoom_offset(right_center_x + x, JUMELLE_ZOOM_CENTER_Y - y,
                                             vertical - horizontal);
                }
            }
        }
    }

    /* PS1 draws a fixed sprite-and-black-box binocular frame in its 240-line
     * world-map renderer. This moving, radially sampled lookup is PC-specific. */
}

//43A50
void RecaleRayPosInJumelle(void) {
    // NOTE: in the PS1 version this is identical to recale_ray_pos()
    //recale_ray_pos(); return;
    if (scroll_y == -1) {
        s32 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;
        if (v_scroll_speed != 255 || decalage_en_cours != 0) {
            s32 v14 = ray.screen_y - v13;
            v_scroll_speed = ashr16(v14, 2);
            if (Abs(ray.speed_y) <= Abs(v_scroll_speed)) {
                s32 v16 = MAX(3, Abs(ray.speed_y));
                if (v_scroll_speed <= 0) {
                    if (v_scroll_speed != 0) {
                        v_scroll_speed = MAX(v_scroll_speed, -v16);
                    } else if (v14 <= 0) {
                        if (v14 != 0) {
                            v_scroll_speed = -1;
                        }
                    } else {
                        v_scroll_speed = 1;
                    }
                } else {
                    v_scroll_speed = MIN(v_scroll_speed, v16);
                }
            }
        } else if (ray.main_etat != 1) {
            v_scroll_speed = (ray.screen_y >= v13 + 48) ? 0 : -4;
        }
    }

    if (scroll_x == -1) {
        s32 v2 = 3 * LargeurJumelle + 7;
        if (3 * LargeurJumelle >= 0) {
            v2 = 3 * LargeurJumelle;
        }
        s32 min_x = (v2 >> 3) - ray.offset_bx;

        s32 v4 = 5 * LargeurJumelle + 7;
        if (5 * LargeurJumelle >= 0) {
            v4 = 5 * LargeurJumelle;
        }
        s32 max_x = (v4 >> 3) - ray.offset_bx;

        if (decalage_en_cours > 0 || ray.speed_x > 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else if (decalage_en_cours < 0 || ray.speed_x < 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else {
            if (ray.flags.flip_x) {
                s16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            } else {
                s16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            }
        }

        if (Abs(dhspeed) > 4) {
            dhspeed = (dhspeed > 0) ? 4 : -4;
        }
        h_scroll_speed += ashr16(dhspeed, 2);

        if ((max_x > ray.screen_x && ray.speed_x < 0) || (min_x < ray.screen_x && ray.speed_x > 0)) {
            h_scroll_speed += ray.speed_x;
        }

    }
}

//43DBC
void DisplayJumellesFondNormal(void) {
    s32 width = LargeurJumelle + 2;
    s32 height = HauteurJumelle + 2;
    sprite_clipping(0, width, 0, height);
//    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, 320, 200);
    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, width, height);
}

//43E3C
void DoScrollInWorldMap(s16 h_speed, s16 v_speed) {
    bool need_scroll_xmap = true;
    bool need_scroll_ymap = true;
    s32 delta_pos_x = 0;
    s32 delta_pos_y = 0;
    if (ChangeJumelleSizeOK != 4) {
        if (ray.speed_x != 0 || ray.speed_y != 0) {
            s32 ray_x = ray.offset_bx + ray.x;
            if (ray_x > scroll_start_x && ray_x < scroll_end_x) {
                delta_pos_x = 16 * (ray.offset_bx + ray.screen_x - LargeurJumelle / 2);
                if (ChangeDeltaPosXJumelleWithLimit(delta_pos_x)) {
                    need_scroll_xmap = false;
                }
            } else {
                delta_pos_x = 16 * ((SCREEN_WIDTH / 2) - JumellePosX);
                ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            }

            s32 ray_y = ray.offset_by + ray.y;
            if (ray_y > scroll_start_y && ray_y < scroll_end_y) {
                delta_pos_y = 16 * (ray.screen_y + ray.offset_by - ((3 * HauteurJumelle) >> 2));
                if (ChangeDeltaPosYJumelleWithLimit(delta_pos_y)) {
                    need_scroll_ymap = false;
                }
            } else {
                delta_pos_y = 16 * (SCREEN_HEIGHT / 2 - JumellePosY);
                ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
            }
        } else {
            PositionJumelleDemandeY = SCREEN_HEIGHT / 2;
            PositionJumelleDemandeX = SCREEN_WIDTH / 2;
            delta_pos_x = 16 * (PositionJumelleDemandeX - JumellePosX);
            delta_pos_y = 16 * (PositionJumelleDemandeY - JumellePosY);
            ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
        }

        if (need_scroll_xmap) {
            xmap += MAX(-1, MIN(1, h_speed));
        }
        if (need_scroll_ymap) {
            ymap += MAX(-1, MIN(1, v_speed));
        }

        s16 scroll_limit_x = scroll_start_x;
        if (xmap < scroll_start_x || (scroll_limit_x = scroll_end_x, xmap > scroll_end_x)) {
            xmap = scroll_limit_x;
            dhspeed = 0;
            h_speed = 0;
        }

        s16 scroll_limit_y = scroll_start_y;
        if (ymap < scroll_start_y || (scroll_limit_y = scroll_end_y, ymap > scroll_end_y)) {
            ymap = scroll_limit_y;
            dvspeed = 0;
            v_speed = 0;
        }
    }

    CalcObjPosInWorldMap(&ray);

    if (v_speed == 0) {
        ChangeJumelleSizeOK &= ~1;
    }

    if (h_speed == 0) {
        ChangeJumelleSizeOK &= ~2;
    }

    /* Bit 4 marks the idle/countdown state; bits 0 and 1 still mean that
     * vertical or horizontal scrolling has not settled. */
    if (delta_pos_x == 0 && delta_pos_y == 0 && ray.speed_y == 0 && ray.speed_x == 0 &&
        (ChangeJumelleSizeOK & ~4) == 0) {
        if (RayonJumelle < JUMELLE_MAX_RADIUS && !ModeAutoJumelle) {
            ChangeDeltaSizeJumelle(1);
        }

        if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle >= JumelleZoomAmp || ModeAutoJumelle) {
            if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle > JumelleZoomAmp && !ModeAutoJumelle) {
                ChangeDeltaZoomAmpJumelle(300);
            }
        } else {
            ChangeDeltaZoomAmpJumelle(-300);
        }

        if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle == JumelleZoomAmp &&
            RayonJumelle == JUMELLE_MAX_RADIUS && ChangeJumelleSizeOK == 0) {
            ModeAutoJumelle = 0;
            CompteurJumelle = 0;
            ChangeJumelleSizeOK = 4;
        }

    } else if (ray.speed_x != 0 || ray.speed_y != 0) {
        ChangeJumelleSizeOK &= ~4;
    }

    if (ChangeJumelleSizeOK == 4) {
        ++CompteurJumelle;
        if (CompteurJumelle == 1830) {
            ModeAutoRayonJumelle = JUMELLE_MIN_RADIUS;
            ModeAutoJumelle |= ChangeJumelleSizeOK;
        } else if (CompteurJumelle == 1800) {
            ModeAutoJumelle |= 2;
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
        } else if (ModeAutoJumelle != 0) {
            if (!(ModeAutoJumelle & 1) && RayonJumelle == JUMELLE_MIN_RADIUS) {
                if (JumelleZoomAmp == -26 * JUMELLE_MIN_RADIUS + 32150) {
                    ModeAutoJumelle |= 1;
                    RayonJumelle = JUMELLE_MIN_RADIUS;
                    ModeAutoPosXJumelle = myRand(SCREEN_WIDTH) + RayonJumelle / 3;
                    ModeAutoPosYJumelle = myRand(SCREEN_HEIGHT) + RayonJumelle / 3;
                    PositionJumelleDemandeX = ModeAutoPosXJumelle;
                    PositionJumelleDemandeY = ModeAutoPosYJumelle;
                    Xmap16 = 16 * xmap;
                    Ymap16 = 16 * ymap;
                    PositionJumelleX16 = 16 * JumellePosX;
                    PositionJumelleY16 = 16 * JumellePosY;
                }
            }
        }
    }

    if (ModeAutoJumelle & 1) {
        CalculVitesseJumelle();
        ChangeDeltaPosXJumelleWithoutLimit(XSpeedJumelle);
        ChangeDeltaPosYJumelleWithoutLimit(YSpeedJumelle);
        if (JumellePosX == ModeAutoPosXJumelle && JumellePosY == ModeAutoPosYJumelle) {
            ModeAutoPosXJumelle = myRand(SCREEN_WIDTH) + RayonJumelle / 3;
            ModeAutoPosYJumelle = myRand(SCREEN_HEIGHT) + RayonJumelle / 3;
            PositionJumelleDemandeX = ModeAutoPosXJumelle;
            PositionJumelleDemandeY = ModeAutoPosYJumelle;
        }
    }

    if (ModeAutoJumelle & 2) {
        if (JumelleZoomAmp < ModeAutoJumelleZoomAmp) {
            ChangeDeltaZoomAmpJumelle(300);
            if (JumelleZoomAmp > ModeAutoJumelleZoomAmp) {
                JumelleZoomAmp = ModeAutoJumelleZoomAmp;
            }
        } else if (JumelleZoomAmp > ModeAutoJumelleZoomAmp) {
            ChangeDeltaZoomAmpJumelle(-300);
            if (JumelleZoomAmp < ModeAutoJumelleZoomAmp) {
                JumelleZoomAmp = ModeAutoJumelleZoomAmp;
            }
        }
    }

    if (ModeAutoJumelle & 4) {
        if (RayonJumelle < ModeAutoRayonJumelle) {
            ChangeDeltaSizeJumelle(1);
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
        } else if (RayonJumelle > ModeAutoRayonJumelle) {
            ChangeDeltaSizeJumelle(-1);
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
        }
    }

}

//444D4
void CalcObjPosInWorldMap(obj_t* obj) {
    obj->screen_x = obj->x + 8 - xmap + JumellePosX - JumelleXMin;
    obj->screen_y = obj->y - ymap + JumellePosY - JumelleYMin;
}

//44524
void MoveRayInWorldMap(void) {
    h_scroll_speed = 0;
    v_scroll_speed = 0;
    ray.x += ray.speed_x;
    ray.y += ray.speed_y;
    decalage_en_cours = ray.speed_x;
}

