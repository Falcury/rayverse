
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

//3B314
i32 playVideo2(const char* path, const char* filename, u8 a3, i32 a4) {
    playing_intro_video = strcasecmp("intro.dat", filename) == 0;
    if (byte_CFA2A) {
        // stub
    }
    FILE* fp = open_data_file(filename, false);
    if (!fp) {
        return -5;
    }
    if (MusicCdActive) {

    }
    return 0; // stub
}


//3B4A8
void playVideo(const char* path, const char* filename, i32 a3) {
    playVideo2(path, filename, a3, 0);
}
