
// NOTE: unsure about the source filename map.c (in the function order, should come after neige.c and before ngawe.c)


//5C4A0
void deter_vide_plein_panach(void* a1, i16 a2) {
    //stub
}

//5C4DC
void deter_nb_blocks(void* a1, i16 a2, i16 a3, i32* a4, i32* a5) {
    //stub
}

//5C564
void Copy_Blocks(mem_t* buffer, void* a2, i16 a3, i16 a4) {
    //stub
}

//5C82C
void construct_MAP(mem_t* buffer, void* a2, void* a3) {
    //stub
}

//5C8FC
void init_build_map(void* a1) {
    //stub
}

//5C914
void update_display_map(void* a1) {
    //stub
}

//5C9F0
void set_default_Bloc_clipping(void) {
    Bloc_lim_H1 = 0;
    Bloc_lim_H2 = 200;
    Bloc_lim_W1 = 4;
    Bloc_lim_W2 = 320;
}


//5CA24
void set_xymapini(void) {
    //stub
}

//5CAD0
void set_xymap(void) {
    //stub
}

//5CB40
void set_whmap(i32 a1, i32 a2, void* a3) {
    //stub
}

//5CD64
void recaleRaysurlesBords(void) {
    //stub
}

//5CE08
void set_xywhmap(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//5D144
void MaskScrollDiffSprites(mem_t* buffer) {
    //stub
}

//5D230
void DRAW_MAP(void* a1, void* a2) {
    //stub
}

//5D5B0
void INIT_GAME_MODE_NORMAL(void) {
    sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    DrawBufferNormalIni = DrawBufferNormal;
    DrawBufferNormal += 4; //?
//    Copy_Plan0_To_Buf = (void*)&Copy_Plan0Diff_To_Buf; // TODO
    init_build_map(BIG_MAP);
    N_CLRSCR(DrawBufferNormal);
}

//5D620
void FIN_GAME_MODE_NORMAL(void) {
    DrawBufferNormal = DrawBufferNormalIni;
    draw_buffer = DrawBufferNormalIni;
}
