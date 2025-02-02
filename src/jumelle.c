
//42930
void plot2linejumelle(i32 a1, i32 a2, i32 a3, void* a4, i32 a5, i32 a6, i32 a7) {
    //stub
}

//42E1C
void DisplayJumellesNormal(i32 x, i32 y, i32 rayon, i32 a4, u8* effet_buf, u8* draw_buf) {
    //stub
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

//4314C
void CalculVitesseJumelle(void) {
    //stub
}

//43230
void ChangeDeltaPosXJumelleWithoutLimit(i32 a1) {
    //stub
}

//43300
void ChangeDeltaPosYJumelleWithoutLimit(i32 a1) {
    //stub
}

//433D0
void ChangeDeltaPosXJumelleWithLimit(i32 a1) {
    //stub
}

//43520
void ChangeDeltaPosYJumelleWithLimit(i32 a1) {
    //stub
}

//4368C
void ChangeJumelleVariable(void) {
    LargeurJumelle = (7 * RayonJumelle) / 2;
    i32 var1 = LargeurJumelle / 2;
    JumelleXMin = JumellePosX - var1;
    i32 result = ((2 * RayonJumelle) / 2);
    JumelleYMin = JumellePosY - result;
    scroll_start_x = (i16)(LargeurJumelle + 2);
    scroll_start_y = (i16)(result + 2);
    scroll_end_x = (i16)(WidthNworld - var1 - 4);
    HauteurJumelle = 2 * RayonJumelle;
    scroll_end_y = (i16)(HeightNworld - result - 4);
}

//43730
void ChangeDeltaZoomAmpJumelle(i32 a1) {
    //stub
}

//437FC
void PrepareJumelleZoom(void) {
    //stub
}

//43A50
void RecaleRayPosInJumelle(void) {
    //stub
}

//43DBC
void DisplayJumellesFondNormal(void) {
    //stub
}

//43E3C
void DoScrollInWorldMap(i16 a1, i16 a2) {
    //stub
}

//444D4
void CalcObjPosInWorldMap(obj_t* obj) {
    obj->screen_x = obj->xpos + 8 - xmap + JumellePosX - JumelleXMin;
    obj->screen_y = obj->ypos - ymap + JumellePosY - JumelleYMin;
}

//44524
void MoveRayInWorldMap(void) {
    //stub
}

