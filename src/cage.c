
//28B40
void DO_CAGE2(obj_t* obj) {
    obj->speed_x = 0;
    obj->speed_y = -8;
}

//28B50
void DO_CAGE(obj_t* obj) {
    //stub
}

//28BA4
void DoCagePoingCollision(obj_t* obj) {
    //stub
}

//28C2C
void DO_MEDAILLON_TOON(obj_t* obj) {
    //stub
}

//28CA0
void DO_MEDAILLON_TOON_GELE(void) {
    //stub
}

//28DD4
void ALLOCATE_MEDAILLON_TOON(void) {
    //stub
}

//28F14
void allocate_toons(obj_t* obj, u8 a2) {
    //stub
}

//2912C
void allocateGrille(obj_t* obj) {
    //stub
}

//291B4
void DO_ONE_PINK_CMD(obj_t* obj) {
    //stub
}

//2937C
void DoLidolPinkRaymanZDD(obj_t* obj) {
    if (get_eta(obj)->interaction_flags & 1) {
        skipToLabel(obj, 0, 1);
    }
}
