
//42930
void plot2linejumelle(i32 x, i32 y, i32 a3, i32 a4, i32 a5, u8* effet_buf, u8* draw_buf) {
    //stub
}

bool skip_jumelle = true;

//42E1C
void DisplayJumellesNormal(i32 x, i32 y, i32 rayon, i32 a4, u8* effet_buf, u8* draw_buf) {
    // NOTE: this is debug code!
    if (skip_jumelle) {
#if 1
        for (i32 i = 0; i < HauteurJumelle; ++i) {
            if (JumelleYMin + i >= 0 && JumelleYMin + i < 200) {
                i32 width = LargeurJumelle;
                if (JumelleXMin + LargeurJumelle > 320) {
                    width = 320 - JumelleXMin;
                }
                memcpy(draw_buf + 320 * (JumelleYMin + i) + JumelleXMin, effet_buf + 320 * i, width);
            } else {
//                printf("out of bounds!\n");
                break;
            }
        }
#else
        memcpy(draw_buf, effet_buf, 320*200);
#endif
        return;
    }

    i32 v6 = 0;
    i32 v7 = 0;
    i32 v8 = rayon;
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
    DistPointX = (i32*)calloc(161, sizeof(i32));
    DistPointY = (i32*)calloc(65, sizeof(i32));
    ExpPoint = (i32*)calloc(5120, sizeof(i32));
    JumelleZoomDef = (i32*)calloc(29025, sizeof(i32));
    if (JumelleEffetAutorise) {
        if (DistPointX && DistPointY && ExpPoint && JumelleZoomDef) {
            for (i32 y = 0; y < 65; ++y) {
                DistPointY[y] = y * y;
            }
            // NOTE: the for loop for X is inside the Y loop in the original (probably a bug?)
            for (i32 x = 0; x < 161; ++x) {
                DistPointX[x] = x * x;
            }
            for (i32 i = 0; i < 5120; ++i) {
                ExpPoint[i] = (i32)(expf((float)(-i) / 1024.0f ) * 65536.0f);
            }
        } else {
            if (DistPointX) {
                free(DistPointX);
            }
            if (DistPointY) {
                free(DistPointY);
            }
            if (ExpPoint) {
                free(ExpPoint);
            }
            if (JumelleZoomDef) {
                free(JumelleZoomDef);
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
    RayonJumelle = 25;
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
void ChangeDeltaSizeJumelle(i32 delta_size) {
    RayonJumelle = MAX(30, MIN(69, RayonJumelle + delta_size));
    ChangeJumelleVariable();
    if (JumelleZoomActif) {
        ChangeDeltaZoomAmpJumelle(delta_size <= 0 ? 65 : -65);
    }
}

//4314C
void CalculVitesseJumelle(void) {
    i32 delta_x = PositionJumelleDemandeX - JumellePosX;
    i32 delta_y = PositionJumelleDemandeY - JumellePosY;
    i32 dist = Abs(delta_y) + Abs(delta_x);
    if (dist > 0) {
        delta_x = ashl16(delta_x, 6) / dist;
        delta_y = ashl16(delta_y, 6) / dist;
    }
    XSpeedJumelle = MAX(MIN(XSpeedJumelle + sgn(delta_x - XSpeedJumelle), 16), -16);
    YSpeedJumelle = MAX(MIN(YSpeedJumelle + sgn(delta_y - YSpeedJumelle), 16), -16);
}

//43230
void ChangeDeltaPosXJumelleWithoutLimit(i32 delta_pos_x) {
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
void ChangeDeltaPosYJumelleWithoutLimit(i32 delta_pos_y) {
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
i32 ChangeDeltaPosXJumelleWithLimit(i32 delta_pos_x) {
    delta_pos_x = MAX(-16, MIN(16, delta_pos_x));

    bool changed = false;
    if (((xmap != scroll_start_x && xmap != scroll_end_x) ||
        (xmap == scroll_start_x && delta_pos_x > 0) ||
        (xmap == scroll_end_x && delta_pos_x < 0)) &&
        JumellePosX > RayonJumelle / 3 && JumellePosX < 320 - RayonJumelle / 3
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
i32 ChangeDeltaPosYJumelleWithLimit(i32 delta_pos_y) {
    delta_pos_y = MAX(-16, MIN(16, delta_pos_y));

    bool changed = false;
    if (((ymap != scroll_start_y && ymap != scroll_end_y) ||
         (ymap == scroll_start_y && delta_pos_y > 0) ||
         (ymap == scroll_end_y && delta_pos_y < 0)) &&
        JumellePosY > RayonJumelle / 3 && JumellePosY < 320 - RayonJumelle / 3
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
    i32 half_width = LargeurJumelle / 2;
    i32 half_height = ((2 * RayonJumelle) / 2);
    JumelleXMin = JumellePosX - half_width;
    JumelleYMin = JumellePosY - half_height;
    scroll_start_x = (i16)(half_width + 2);
    scroll_start_y = (i16)(half_height + 2);
    scroll_end_x = (i16)(WidthNworld - half_width - 4);
    scroll_end_y = (i16)(HeightNworld - half_height - 4);
}

//43730
void ChangeDeltaZoomAmpJumelle(i32 a1) {
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
    if (JumelleEffetAutorise) {

    }
    //stub
}

//43A50
void RecaleRayPosInJumelle(void) {
    if (scroll_y == -1) {
        i32 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;
        if (v_scroll_speed != 255 || decalage_en_cours != 0) {
            i32 v14 = ray.screen_y - v13;
            v_scroll_speed = ashr16(ray.screen_y - v13, 2);
            if (Abs(ray.speed_y) <= Abs(v_scroll_speed)) {
                i32 v16 = MAX(3, Abs(ray.speed_y));
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
        i32 v2 = 3 * LargeurJumelle + 7;
        if (3 * LargeurJumelle >= 0) {
            v2 = 3 * LargeurJumelle;
        }
        i32 min_x = (v2 >> 3) - ray.offset_bx;

        i32 v4 = 5 * LargeurJumelle + 7;
        if (5 * LargeurJumelle >= 0) {
            v4 = 5 * LargeurJumelle;
        }
        i32 max_x = (v4 >> 3) - ray.offset_bx;

        if (decalage_en_cours > 0 || ray.speed_x > 0) {
            i16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else if (decalage_en_cours < 0 || ray.speed_x < 0) {
            i16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else {
            if (ray.flags.flip_x) {
                i16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            } else {
                i16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
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
    i32 width = LargeurJumelle + 2;
    i32 height = HauteurJumelle + 2;
//    sprite_clipping(0, width, 0, height); //TODO: re-enable this
    // TODO: check if the source buffer might be read out of bounds?
    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, width, height);
}

//43E3C
void DoScrollInWorldMap(i16 h_speed, i16 v_speed) {
    bool need_scroll_xmap = true;
    bool need_scroll_ymap = true;
    i32 delta_pos_x = 0;
    i32 delta_pos_y = 0;
    if (ChangeJumelleSizeOK != 4) {
        if (ray.speed_x != 0 || ray.speed_y != 0) {
            i32 ray_x = ray.offset_bx + ray.x;
            if (ray_x > scroll_start_x && ray_x < scroll_end_x) {
                delta_pos_x = 16 * (ray.offset_bx + ray.screen_x - LargeurJumelle / 2);
                if (ChangeDeltaPosXJumelleWithLimit(delta_pos_x)) {
                    need_scroll_xmap = false;
                }
            } else {
                delta_pos_x = 16 * (160 - JumellePosX);
                ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            }

            i32 ray_y = ray.offset_by + ray.y;
            if (ray_y > scroll_start_y && ray_y < scroll_end_y) {
                delta_pos_y = 16 * (ray.screen_y + ray.offset_by - ((3 * HauteurJumelle) >> 2));
                if (ChangeDeltaPosYJumelleWithLimit(delta_pos_y)) {
                    need_scroll_ymap = false;
                }
            } else {
                delta_pos_y = 16 * (160 - JumellePosY);
                ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
            }
        } else {
            PositionJumelleDemandeY = 100;
            PositionJumelleDemandeX = 160;
            delta_pos_x = 16 * (160 - JumellePosX);
            delta_pos_y = 16 * (160 - JumellePosY);
            ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
        }

        if (need_scroll_xmap) {
            xmap += MAX(-1, MIN(1, h_speed));
        }
        if (need_scroll_ymap) {
            ymap += MAX(-1, MIN(1, v_speed));
        }

        i16 scroll_limit_x = scroll_start_x;
        if (xmap < scroll_start_x || (scroll_limit_x = scroll_end_x, xmap > scroll_end_x)) {
            xmap = scroll_limit_x;
            dhspeed = 0;
            h_speed = 0;
        }

        i16 scroll_limit_y = scroll_start_y;
        if (ymap < scroll_start_y || (scroll_limit_y = scroll_end_y, ymap > scroll_end_y)) {
            ymap = scroll_limit_y;
            dvspeed = 0;
            v_speed = 0;
        }

        CalcObjPosInWorldMap(&ray);

        if (v_speed == 0) {
            ChangeJumelleSizeOK &= ~1;
        }

        if (h_speed == 0) {
            ChangeJumelleSizeOK &= ~2;
        }

        if (delta_pos_x == 0 && delta_pos_y == 0 && ray.speed_y == 0 && ray.speed_x == 0 && ChangeJumelleSizeOK != 4) {
            if (RayonJumelle < 64 && !ModeAutoJumelle) {
                ChangeDeltaSizeJumelle(1);
            }

            if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle >= JumelleZoomAmp || ModeAutoJumelle) {
                if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle > JumelleZoomAmp && !ModeAutoJumelle) {
                    ChangeDeltaZoomAmpJumelle(300);
                }
            } else {
                ChangeDeltaZoomAmpJumelle(-300);
            }

            if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle == JumelleZoomAmp && RayonJumelle == 64 && ChangeJumelleSizeOK != 0) {
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
                ModeAutoRayonJumelle = 25;
                ModeAutoJumelle |= ChangeJumelleSizeOK;
            } else if (CompteurJumelle == 1800) {
                ModeAutoJumelle |= 2;
                ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
            } else if (ModeAutoJumelle != 0) {
                if (!(ModeAutoJumelle & 1) && RayonJumelle == 25) {
                    if (JumelleZoomAmp == 31500) {
                        ModeAutoJumelle |= 1;
                        RayonJumelle = 25;
                        ModeAutoPosXJumelle = myRand(320) + 8;
                        ModeAutoPosYJumelle = myRand(200) + RayonJumelle / 3;
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
                ModeAutoPosXJumelle = myRand(320) + RayonJumelle / 3;
                ModeAutoPosYJumelle = myRand(200) + RayonJumelle / 3;
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

