
//24E90
void MURDUR(i16 a1, i16 a2) {
    //stub
}

//24EAC
void dist_to_bloc_floor(i16 a1, i16 a2, i16 a3) {
    //stub
}

//24EC8
void bloc_floor(i16 a1, i16 a2, i16 a3) {
    //stub
}

//24F60
u8 calc_typ_trav(obj_t* obj, u8 a2) {
    return 0; //stub
}

//251F8
u8 calc_typ_travd(obj_t* obj, u8 a2) {
    return 0; //stub
}

//25388
void TEST_FIN_BLOC(obj_t* obj) {
    //stub
}

//256D4
void TEST_IS_ON_RESSORT_BLOC(obj_t* obj) {
    //stub
}

//25708
void IS_ON_RESSORT_BLOC(obj_t* obj) {
    //stub
}

//25820
void CALC_MOV_ON_BLOC(obj_t* obj) {
    //stub
}

//25B30
void recale_position(obj_t* obj) {
    //stub
}

//25BB0
i32 blocs1_empty(i32 a1, i32 a2) {
    return a2 + 1;
}

//25BB4
i32 blocs1_right_45(i32 a1, i32 a2) {
    return 15 - a1;
}

//25BC0
i32 blocs1_left_45(i32 a1, i32 a2) {
    return a1;
}

//25BC4
i32 blocs1_right1_30(i32 a1, i32 a2) {
    return 15 - (a1 >> 1);
}

//25BD4
i32 blocs1_right2_30(i32 a1, i32 a2) {
    return 7 - (a1 >> 1);
}

//25BE4
i32 blocs1_left1_30(i32 a1, i32 a2) {
    return a1 >> 1;
}

//25BE8
i32 blocs1_left2_30(i32 a1, i32 a2) {
    return (a1 >> 1) + 8;
}

//25BF0
i32 blocs3_empty(i32 a1, i32 a2) {
    return a2;
}
#define blocs1_liane blocs3_empty

//25BF4
i32 blocs4_empty(i32 a1, i32 a2) {
    return 0;
}
#define blocs1_hor blocs4_empty

//25BF8
i32 blocs2_empty(i32 a1, i32 a2) {
    return 16;
}

//25C7C
void init_calcbloc_func(void) {
    for (i32 i = 0; i < 31; ++i) {
        switch(i) {
            default: {
                calcbloc1[i] = blocs1_empty;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            } break;
            case 2:
            case 18: {
                calcbloc1[i] = blocs1_right_45;
                calcbloc2[i] = blocs1_right_45;
                calcblocfloor[i] = blocs1_right_45;
                calcblocrecal[i] = blocs1_right_45;
            } break;
            case 3:
            case 19: {
                calcbloc1[i] = blocs1_left_45;
                calcbloc2[i] = blocs1_left_45;
                calcblocfloor[i] = blocs1_left_45;
                calcblocrecal[i] = blocs1_left_45;
            } break;
            case 4:
            case 20: {
                calcbloc1[i] = blocs1_right1_30;
                calcbloc2[i] = blocs1_right1_30;
                calcblocfloor[i] = blocs1_right1_30;
                calcblocrecal[i] = blocs1_right1_30;
            } break;
            case 5:
            case 21: {
                calcbloc1[i] = blocs1_right2_30;
                calcbloc2[i] = blocs1_right2_30;
                calcblocfloor[i] = blocs1_right2_30;
                calcblocrecal[i] = blocs1_right2_30;
            } break;
            case 6:
            case 22: {
                calcbloc1[i] = blocs1_left1_30;
                calcbloc2[i] = blocs1_left1_30;
                calcblocfloor[i] = blocs1_left1_30;
                calcblocrecal[i] = blocs1_left1_30;
            } break;
            case 7:
            case 23: {
                calcbloc1[i] = blocs1_left2_30;
                calcbloc2[i] = blocs1_left2_30;
                calcblocfloor[i] = blocs1_left2_30;
                calcblocrecal[i] = blocs1_left2_30;
            } break;
            case 9:
            case 14:
            case 15:
            case 30: {
                calcbloc1[i] = blocs1_hor;
                calcbloc2[i] = blocs1_hor;
                calcblocfloor[i] = blocs1_hor;
                calcblocrecal[i] = blocs1_hor;
            } break;
            case 12: {
                calcbloc1[i] = blocs1_liane;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            }
        }
    }
}

