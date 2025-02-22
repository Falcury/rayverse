
//778F0
void Init_Bande(u8 fnd, i16 width, i16 height, u8* source_buf, u8* dest_buf) {
    Num_Fond = fnd;

    if (height <= 200) {
        Val_Div_Scroll_Y = 1;
    } else {
        Val_Div_Scroll_Y = (16 * mp.height - 200) / (height - 200);
    }
    if (Val_Div_Scroll_Y != 0) {
        ++Val_Div_Scroll_Y;
    } else {
        Val_Div_Scroll_Y = 2;
    }
    Val_Add_Scroll_Y = height - 200 - (16 * mp.height - 200) / Val_Div_Scroll_Y - 1;

    if (num_world == world_5_cave && num_level == 10) {
        Init_Effet_Chaleur(width, height, source_buf, dest_buf);
        return;
    }

    NbBande = Tab_NbBande[10 * (num_world - 1) + fnd];

    for (i32 i = 0; i < NbBande; ++i) {

    }
    //stub
}

//77BA0
void Display_Back_Screen(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6) {
    //stub
}

//784A8
void Display_Sprite_On_Front(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6) {
    //stub
}

//787E8
void Calcul_Deplacement_Bande(i16 a1, i16 a2, i16 a3) {
    //stub
}

//78C14
void Init_Effet_Chaleur(i16 width, i16 height, u8* source_buf, u8* dest_buf) {
    //stub
}

//78CA8
void Do_Effet_Chaleur(i16 a1, i16 a2) {
    //stub
}

