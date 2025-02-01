
//18890
void setBossReachingSpeeds(obj_t* obj, u8 a2, u8 a3, u8 a4) {
    //stub
}

//18A64
void testActionEnd(obj_t* obj) {
    //stub
}

//18AD0
i16 firstFloorBelow(obj_t* obj) {
    return 0; //stub
}

//18B4C
void adjustBossScrollLocker(void) {
    //stub
}

//18C8C
void setBossScrollLimits(obj_t* obj) {
    bossScrollStartX = scroll_start_x;
    bossScrollEndX = scroll_end_x;
    floorLine = firstFloorBelow(obj); //TODO
    bossScrollStartY = Bloc_lim_H1 - Bloc_lim_H2 + floorLine - 16;
    bossScrollEndY = ymapmax;
    adjustBossScrollLocker(); //TODO
}
