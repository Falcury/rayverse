
//293B0
u8 sub_293B0(void) {
    return byte_DEEFB;
}

//29C74
i16 sub_29C74(u8 a1) {
    //stub
    return 0;
}

//29CC0
i16 sub_29CC0(u8 a1) {
    word_DE8BC = sub_29C74(a1);
    word_DE8BC = sub_29C74(a1);
    return word_DE8BC;
}

//2A5B8
void sub_2A5B8(u8 a1) {
    word_DE8BC = sub_29CC0(a1);
}
