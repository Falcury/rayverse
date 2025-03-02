
//5B7A0
void DoFirstFlocons(void) {
    //stub
}

//5B848
void add_one_floc(void) {
    //stub
}

//5B8A4
void add_256_flocs(void) {
    //stub
}

//5B910
void sub_5B910(void) {
    // NOTE: this function doesn't have a debug symbol in the Android version, I guess it was marked static?
    //stub
}

//5B9A0 (adapted from PS1 decomp)
void init_flocons(void) {
    s16 flc_i;
    s16 i;
    s32 unk_1;
    s16 unk_2; /* param_1 into get_proj_* is sometimes ray.scale, so similar? */
    s16 j;
    s16 unk_3; s16 unk_4;
    u16 unk_5; u16 unk_6;

    tot_nb_flocs[0] = 20;
    tot_nb_flocs[1] = 30;
    tot_nb_flocs[2] = 50;
    tot_nb_flocs[3] = 60;
    tot_nb_flocs[4] = 70;
    tot_nb_flocs[5] = 80;
    tot_nb_flocs[6] = 96;
    tot_nb_flocs[7] = 106;
    flc_i = 0;
    i = 0;
    unk_1 = 0xffff; /* TODO: should be -1 ... */
    while (i < (s16) LEN(nb_floc))
    {
        nb_floc[i] = 0;
        floc_ind[i] = flc_i;
        unk_2 = 5;
        unk_2 = (i << unk_2) - 64;
        invpy0[i] = inverse_proj_y(unk_2, 0);
        invpy200[i] = inverse_proj_y(unk_2, SCREEN_HEIGHT);
        invpx0[i] = inverse_proj_x(unk_2, 0);
        invpx320[i] = inverse_proj_x(unk_2, SCREEN_WIDTH);
        j = 0;
        while (j < tot_nb_flocs[i])
        {
            do
            {
                do
                {
                    unk_3 = myRand(700);
                    unk_4 = myRand(400) - 150;
                    unk_5 = get_proj_x(unk_2, unk_3);
                    unk_6 = get_proj_y(unk_2, unk_4);
                } while ((u16) (unk_5 + unk_1) > SCREEN_WIDTH - 2);
            } while ((u16) (unk_6 + unk_1) > SCREEN_HEIGHT - 2);
            flocon_tab[flc_i].field_0 = unk_3;
            flocon_tab[flc_i].field_2 = unk_4;
            flocon_tab[flc_i].field_4 = unk_2;
            flc_i++;
            j++;
        }
        i++;
    }
    VENT_X = 0;
    VENT_Y = 0;
}

//5BB78
void do_flocons(i16 a1, i16 a2, i16 a3, i16 a4) {
    //stub
}

//5BE74
void set_snow_sequence(i16 seq, i16 len) {
    SNSEQ_no = seq;
    SNSEQ_ptr = 0;
    SNSEQ_len[seq] = len;
}

//5BE8F and 5C43C (apparently split in two parts?)
void set_SNSEQ_list(i16 a1) {
    MapAvecPluieOuNeige = 1;
    switch (a1) {
        case 0: {
            SNSEQ_list[0] = 4;
            SNSEQ_list[1] = 2;
            SNSEQ_list[2] = 128;
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 1: {
            SNSEQ_list[0] = 4;
            SNSEQ_list[1] = 3;
            SNSEQ_list[2] = 128;
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 2: {
            SNSEQ_list[0] = 4;
            SNSEQ_list[1] = 3;
            SNSEQ_list[2] = 512;
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 3: {
            if (VENT_Y == 8) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_Y > 8) {
                SNSEQ_list[1] = 8;
                SNSEQ_list[2] = 2 * VENT_Y - 16;
            } else {
                SNSEQ_list[1] = 9;
                SNSEQ_list[2] = 16 - 2 * VENT_Y;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 4: {
            if (VENT_Y == 0) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_Y > 0) {
                SNSEQ_list[1] = 8;
                SNSEQ_list[2] = 2 * VENT_Y;
            } else {
                SNSEQ_list[1] = 9;
                SNSEQ_list[2] = -2 * VENT_Y;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 5: {
            if (VENT_X == -8) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_X > -8) {
                SNSEQ_list[1] = 6;
                SNSEQ_list[2] = 2 * VENT_X + 16;
            } else {
                SNSEQ_list[1] = 7;
                SNSEQ_list[2] = -16 - 2 * VENT_X;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 6: {
            if (VENT_X == 8) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_X > 8) {
                SNSEQ_list[1] = 6;
                SNSEQ_list[2] = 2 * VENT_X - 16;
            } else {
                SNSEQ_list[1] = 7;
                SNSEQ_list[2] = 16 - 2 * VENT_X;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 7: {
            if (VENT_X == -16) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_X > -16) {
                SNSEQ_list[1] = 6;
                SNSEQ_list[2] = 2 * VENT_X + 32;
            } else {
                SNSEQ_list[1] = 7;
                SNSEQ_list[2] = -32 - 2 * VENT_X;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 8: {
            if (VENT_X == 16) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_X > 16) {
                SNSEQ_list[1] = 6;
                SNSEQ_list[2] = 2 * VENT_X - 32;
            } else {
                SNSEQ_list[1] = 7;
                SNSEQ_list[2] = 32 - 2 * VENT_X;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 9: {
            if (VENT_X == 0) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_X > 0) {
                SNSEQ_list[1] = 6;
                SNSEQ_list[2] = 2 * VENT_X;
            } else {
                SNSEQ_list[1] = 7;
                SNSEQ_list[2] = -2 * VENT_X;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        case 10: {
            if (VENT_Y == -16) {
                SNSEQ_list[0] = 2;
            } else {
                SNSEQ_list[0] = 4;
            }
            if (VENT_Y > -16) {
                SNSEQ_list[1] = 8;
                SNSEQ_list[2] = 2 * VENT_Y + 32;
            } else {
                SNSEQ_list[1] = 9;
                SNSEQ_list[2] = -32 - 2 * VENT_Y;
            }
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 128;
            SNSEQ_list[5] = 9;
            SNSEQ_list[6] = 32;
        } break;
        case 11: {
            SNSEQ_list[0] = 4;
            SNSEQ_list[1] = 10;
            SNSEQ_list[2] = 1;
            SNSEQ_list[3] = 0;
            SNSEQ_list[4] = 0x7FFF;
        } break;
        default: break;
    }
    SNSEQ_list_ptr = 1;
    set_snow_sequence((i16)SNSEQ_list[1], (i16)SNSEQ_list[2]);
    SNSEQ_list_ptr += 2;
}

//5C278
void DO_SNOW_SEQUENCE(void) {
    //stub
}

//5C3DC
void DoNeigeRaymanCollision(obj_t* obj) {
    //stub
}
