
//23A00
i16 get_allowed_time(void) {
    return allowed_time[32 * (num_world - 1) + (num_level - 1)];
}

//23A28
void calc_left_time(void) {
    if (bonus_map) {
        i16 allowed_time_in_seconds = allowed_time[32 * (num_world - 1) + (num_level - 1)];
        if (map_time == 1) {
            if (allowed_time_in_seconds == -2) {
                left_time = -2;
            } else {
                left_time = allowed_time_in_seconds * 60;
            }
        } else if (allowed_time_in_seconds != -2 && map_time > 120) {
            if (left_time != 0) {
                ray.is_active = 1;
                left_time = 60 * allowed_time_in_seconds - map_time + 120;
                if (left_time == 0) {
                    if (nb_wiz != 0) {
                        status_bar.num_wiz = nb_wiz_save;
                        departlevel = 0;
                        fix_numlevel(&ray);
                        DO_FADE_OUT();
                    }
                }
            }
        }
    }
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

