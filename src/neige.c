
//5B7A0
void DoFirstFlocons(void) {
    if (MapAvecPluieOuNeige && save1.is_just_saved && save1.save_obj_id == -1 && !fin_continue) {
        for (s16 i = 0; i < 100; ++i) {
            s16 random1 = myRand(i);
            s16 random2 = myRand(random1);
            do_flocons(xmap + random1, ymap + random2, xmap_old, ymap_old);
            DO_SNOW_SEQUENCE();
            horloges(0);
            xmap_old = xmap;
            ymap_old = ymap;
        }
    }
}

//5B848
void add_one_floc(void) {
    /* 3B264 8015FA64 -O2 -msoft-float */

    s16 unk_1 = myRand(511);
    s16 i = LEN(floc_ind) - 1;
    s16 done = false;

    while (!done)
    {
        if (floc_ind[i] < unk_1)
        {
            done = true;
            nb_floc[i]++;
        }
        i--;
    }

    if (nb_floc[i + 1] > tot_nb_flocs[i + 1])
        nb_floc[i + 1]--;
}

//5B8A4
void add_256_flocs(void) {
    /* 3B324 8015FB24 -O2 -msoft-float */

    /* THEY LIED TO US, IT'S REALLY 251! D: */
    nb_floc[0] = 10;
    nb_floc[1] = 15;
    nb_floc[2] = 25;
    nb_floc[3] = 30;
    nb_floc[4] = 35;
    nb_floc[5] = 40;
    nb_floc[6] = 43;
    nb_floc[7] = 53;
}

//5B910
void sub_one_floc(void) {
    // NOTE: this function doesn't have a debug symbol in the Android version, I guess it was marked static?

    /* 3B38C 8015FB8C -O2 -msoft-float */

    s16 unk_1 = myRand(511);
    s16 i = LEN(nb_floc) - 1;
    s16 done = false;

    while (!done && i > -1)
    {
        if (floc_ind[i] <= unk_1 && nb_floc[i] > 0)
        {
            nb_floc[i]--;
            done = true;
        }
        i--;
    }

    if (i == -1)
    {
        i = LEN(nb_floc) - 1;
        done = false;
        while (!done && i > -1)
        {
            if (nb_floc[i] > 0)
            {
                nb_floc[i]--;
                done = true;
            }
            i--;
        }
    }
}

//5B9A0 (adapted from PS1 decomp)
void init_flocons(void) {
    /* 3B488 8015FC88 -O2 -msoft-float */

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
void do_flocons(s16 x, s16 y, s16 x_old, s16 y_old) {
    /* 3B718 8015FF18 -O2 -msoft-float */

    s16 unk_x_1; s16 unk_y_1;
    s32 remove_1; s32 remove_2; /* TODO: ??? */
    s16 unk_x_2; s16 unk_y_2;
    s16 i;
    s16 unk_1;
    s16 flc_i_1;
    s16 unk_x_3; s16 unk_y_3;
    s16 x_0; s16 x_320; s16 y_0; s16 y_200;
    flocon_t *cur_floc;
    s16 unk_2;
    s16 flc_i_2;
    s16 prev_vy = VENT_Y;
    s16 prev_vx = VENT_X;

    VENT_Y += 4;
    unk_x_1 = VENT_X / 4;
    unk_y_1 = VENT_Y / 4;
    remove_1 = VENT_X - (VENT_X / 4 * 4);
    unk_x_2 = remove_1;
    remove_2 = VENT_Y - (VENT_Y / 4 * 4);
    unk_y_2 = remove_2;
    if (Abs(unk_x_2) == 1 && horloge[4] == 0)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_x_2) == 2 && horloge[2] == 0)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_x_2) == 3 && horloge[4] < 3)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_y_2) == 1 && horloge[4] == 0)
        unk_y_1 += sgn(VENT_Y);

    if (Abs(unk_y_2) == 2 && horloge[2] == 0)
        unk_y_1 += sgn(VENT_Y);

    if (Abs(unk_y_2) == 3 && horloge[4] < 3)
        unk_y_1 += sgn(VENT_Y);

    set_proj_center(SCREEN_WIDTH / 2, 170);
    i = 0;
    unk_1 = -64;
    while (unk_1 < 32 * 6)
    {
        flc_i_1 = floc_ind[i];
        unk_y_3 = y_old - y;
        unk_y_3 += unk_y_1;
        unk_x_3 = x_old - x + unk_x_1;
        y_0 = invpy0[i];
        y_200 = invpy200[i];
        x_0 = invpx0[i];
        x_320 = invpx320[i];

        cur_floc = &flocon_tab[flc_i_1];
        unk_2 = flc_i_1 + nb_floc[i];
        flc_i_2 = flc_i_1;
        while (flc_i_2 < unk_2)
        {
            cur_floc->field_0 += unk_x_3;
            cur_floc->field_2 += unk_y_3;

            if (cur_floc->field_0 > x_320)
                cur_floc->field_0 = x_0;
            else if (cur_floc->field_0 < x_0)
                cur_floc->field_0 = x_320;

            if (cur_floc->field_2 > y_200)
                cur_floc->field_2 = y_0;
            else if (cur_floc->field_2 < y_0)
                cur_floc->field_2 = y_200;

            cur_floc++;
            flc_i_2++;
        }
        unk_1 += 32;
        i++;
    }

    VENT_Y = prev_vy;
    VENT_X = prev_vx;
    if (num_world != 1)
        weather_wind = VENT_X / 8;
}

//5BE74
void set_snow_sequence(s16 seq, s16 len) {
    SNSEQ_no = seq;
    SNSEQ_ptr = 0;
    SNSEQ_len[seq] = len;
}

//5BE8F and 5C43C (apparently split in two parts?)
void set_SNSEQ_list(s16 a1) {
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
    set_snow_sequence(SNSEQ_list[1], SNSEQ_list[2]);
    SNSEQ_list_ptr += 2;
}

//5C278
void DO_SNOW_SEQUENCE(void) {
    /* 3C164 80160964 -O2 -msoft-float */

    switch (SNSEQ_no)
    {
        case 0:
            break;
        case 1:
            VENT_X = 0;
            VENT_Y = 0;
            break;
        case 2:
            add_one_floc();
            break;
        case 3:
            sub_one_floc();
            break;
        case 6:
            if (horloge[2] == 0)
                VENT_X--;
            break;
        case 4:
            VENT_X--;
            break;
        case 7:
            if (horloge[2] == 0)
                VENT_X++;
            break;
        case 5:
            VENT_X++;
            break;
        case 8:
            if (horloge[2] == 0)
                VENT_Y--;
            break;
        case 9:
            if (horloge[2] == 0)
                VENT_Y++;
            break;
        case 10:
            VENT_X = -8;
            VENT_Y = 8;
            add_256_flocs();
    }

    if (++SNSEQ_ptr == SNSEQ_len[SNSEQ_no])
    {
        s16 first = SNSEQ_list[SNSEQ_list_ptr];
        SNSEQ_list_ptr++;
        s16 second = SNSEQ_list[SNSEQ_list_ptr];
        SNSEQ_list_ptr++;

        set_snow_sequence(first, second);
        if (SNSEQ_list_ptr == SNSEQ_list[0])
            SNSEQ_list_ptr = 0;
    }
}

//5C3DC
void DoNeigeRaymanCollision(obj_t* obj) {
    print_once("Not implemented: DoNeigeRaymanCollision"); //stub
}
