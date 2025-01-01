
//1CEE0
void SetCompteurTrameAudio(void) {
    CompteurTrameAudio = 0;
}

//1CEEC
void DoCdCredits(void) {
    //stub
}

//1CF2C
void start_cd_credits(void) {
    //stub
}

//1CF54
void start_cd_ubi_soft(void) {
    //stub
}

//1CF7C
void start_cd_gros_rayman(void) {
    play_cd_track(19); // Menu music - "World Map"
    // TODO: remove this once DoCdRap() is implemented (I think that loops the menu music)
}

//1CF80
void start_cd_perdu(void) {
    //stub
}

//1CF94
void DoCdRap(void) {
    //stub
}

//1CFD4
void start_cd_bbdead(void) {
    //stub
}

//1CFFC
void start_cd_gameover(void) {
    //stub
}

//1D028
void stop_cd(void) {
    MusicCdActive = 0;
    SetCompteurTrameAudio();
    is_ogg_playing = false;
}

//1D050
void cd_playing(void) {
    //stub
}
