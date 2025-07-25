
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

//optimized out
void start_cd_gagne(void) {
    //nullsub
}

//optimized out
void start_cd_suspence(void) {
    //nullsub
}

//1CF7C
void start_cd_gros_rayman(void) {
    //play_cd_track(19); // Menu music - "World Map"
    // TODO: remove this once DoCdRap() is implemented (I think that loops the menu music)
}

//1CF80
void start_cd_perdu(void) {
    if (CarteSonAutorisee) {
        PlayTchatchVignette(8);
    } else {
        start_cd_gros_rayman();
    }
}

//1CF94
void DoCdRap(void) {
    if (CompteurTrameAudio > 180) {
        CompteurTrameAudio = 0;
    }
    if (MusicCdActive && CompteurTrameAudio == 0) {
        if (!cd_playing()) {
            start_cd_bbdead();
        }
    }
}

//1CFD4
void start_cd_bbdead(void) {
    if (JeuCracker) {
        MusicCdActive = 0;
    }
    if (MusicCdActive) {
        play_cd_track(19); // Menu music - "World Map"
    }
}

//1CFFC
void start_cd_gameover(void) {
    //nullsub
}

//1D028
void stop_cd(void) {
    SetCompteurTrameAudio();
    is_ogg_playing = false;
}

//1D050
bool cd_playing(void) {
    if (MusicCdActive) {
        return is_ogg_playing;
    } else {
        return false;
    }
}
