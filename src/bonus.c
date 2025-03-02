
//23A00
i16 get_allowed_time(void) {
    return allowed_time[32 * (num_world - 1) - num_level - 1];
}

//23A28
void calc_left_time(void) {
    //stub
}

//23AF8
void get_bonus_map_complete(i16 a1, i16 a2) {
    //stub
}

//23B10
void set_bonus_map_complete(i16 a1, i16 a2) {
    //stub
}

//23B28
void DO_WIZ_AFTER_BONUS_MAP(void) {
    //stub
}

//23B8C
void init_bonus_perfect(void) {
    for (i32 i = 0; i < 24; ++i) {
        bonus_perfect[i] = 0;
    }
}

//23BA0
void do_perfect_bonus(void) {
    //stub
}

//23DF0
void DO_PERFECT_BONUS_MAP(void) {
    //stub
}

