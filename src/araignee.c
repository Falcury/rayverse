
//1C0C0
void get_spi_zdc(obj_t* obj, i16* x, i16* y, i16* w, i16* h) {
    /* 537C0 80177FC0 -O2 -msoft-float */
    u8 eta_flags = obj->eta[obj->main_etat][obj->sub_etat].flags;

    if (eta_flags & 0x40)
    {
        *x = obj->x + 69;
        *y = obj->y + 38;
        *w = 21;
        *h = 30;
    }
    else if (eta_flags & 0x80)
    {
        *x = obj->x + 69;
        *y = obj->y + 86;
        *w = 34;
        *h = 10;
    }
    else
    {
        *x = obj->x + 71;
        *y = obj->y + 71;
        *w = 29;
        *h = 19;
    }
}

//1C160
void DO_SPIDER_PLAFOND(obj_t* obj) {
    //stub
}

//1C588
void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, i16 a2) {
    //stub
}

//1C6A0
void SPIDER_PLAFOND_REACT(obj_t* obj) {
    //stub
}

//1C75C
void DO_DARD_PLAFOND_ALWAYS(obj_t* obj) {
    //stub
}

//1CAC4
void DO_SPIDER_COMMAND(obj_t* obj) {
    //stub
}

//1CC24
void DO_SPIDER_TIR(obj_t* obj) {
    //stub
}

//1CC50
void allocateDard(obj_t* obj) {
    //stub
}

//1CDB0
void DoSpiderPoingCollision(obj_t* obj, i16 a2) {
    //stub
}

//1CE70
void DoSpiderRaymanZDD(obj_t* obj) {
    //stub
}

