
//23F30
void CalcTab(void) {
    //stub
}

//23FDC
void InitPlasma(u8 a1) {
    //stub
}

//24068
void Plasma(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7, i16 a8, i16 a9, u8 a10, i16 a11, i8 a12, i16 a13) {
    //stub
}

//241FC
void PlasmaBox(i16 a1, i16 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//243E0
void InitFire(void) {
    //stub
}

//244C4
void Fire(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//24738
void FireBox(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//2479C
void InitMenuPalette(void) {
    //stub
}

//2494C
void CaptureVideo(u8* a1, u8 a2) {
    //stub
}

//24A98
void CadreTrans(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//24B80
void DrawBlackBorderBox(i16 a1, i16 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//24BF0
void InitGauge(void) {
    //stub
}

//24C54
void gauge(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    //stub
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
