
//78DB0
void swap(i32 a1, i32 a2) {
    print_once("Not implemented: swap"); //stub
}

//78DD0
void set_rubis_list(void) {
    print_once("Not implemented: set_rubis_list"); //stub
}

//78F94
void allocate_rayon(i16 a1, i16 a2) {
    for (i32 i = 0; i < level.nb_objects; ++i) {
        obj_t* cur_obj = level.objects + i;
        if (cur_obj->type == TYPE_170_RAYON && !cur_obj->is_active) {
            print_once("Not implemented: allocate_rayon"); //stub

            break;
        }
    }
}

//7903C
void allocate_8_petits_rayons(i16 a1, i16 a2) {
    print_once("Not implemented: allocate_8_petits_rayons"); //stub
}

//79114
void do_sko_rayon(void) {
    print_once("Not implemented: do_sko_rayon"); //stub
}

//792AC
void do_sko_rayon2(void) {
    print_once("Not implemented: do_sko_rayon2"); //stub
}

//794D4
void start_sko_rayon(i16 a1, i16 a2) {
    print_once("Not implemented: start_sko_rayon"); //stub
}

//795A0
void start_sko_rayon2(i16 a1, i16 a2) {
    print_once("Not implemented: start_sko_rayon2"); //stub
}

//79638
void lance_pince(i32 a1) {
    print_once("Not implemented: lance_pince"); //stub
}

//79688
i32 sko_get_eject_sens(void) {
    ray.iframes_timer = 40;
    return -1;
}

//7969C
void DO_SOL_ENFONCE(void) {
    print_once("Not implemented: DO_SOL_ENFONCE"); //stub
}

//79904
void DO_SKO_PHASE_0(obj_t* obj) {
    print_once("Not implemented: DO_SKO_PHASE_0"); //stub
}

//79A90
void DO_SKO_PHASE_1(obj_t* obj) {
    print_once("Not implemented: DO_SKO_PHASE_1"); //stub
}

//79B5C
void DO_SKO_PHASE_2(obj_t* obj) {
    print_once("Not implemented: DO_SKO_PHASE_2"); //stub
}

//79C98
void DO_SKO_PHASE_3(obj_t* obj) {
    print_once("Not implemented: DO_SKO_PHASE_3"); //stub
}

//79D48
void DO_SKO_PINCE(obj_t* obj) {
    print_once("Not implemented: DO_SKO_PINCE"); //stub
}

//79E18
void DO_SCORPION_COLLISION(obj_t* obj) {
    print_once("Not implemented: DO_SCORPION_COLLISION"); //stub
}

//79F24
void DO_SCORPION_MORT(obj_t* obj) {
    print_once("Not implemented: DO_SCORPION_MORT"); //stub
}

//79F64
void DO_SKO(obj_t* obj) {
    print_once("Not implemented: DO_SKO"); //stub
}

//7A1DC
void SKO_ray_in_zone(obj_t* obj) {
    print_once("Not implemented: SKO_ray_in_zone"); //stub
}

//7A264
void DO_SKO_HIT(obj_t* obj, i16 sprite) {
    print_once("Not implemented: DO_SKO_HIT"); //stub
}

//7A338
i16 Get_PosLave_Y(void) {
    print_once("Not implemented: Get_PosLave_Y");
    return 0; //stub
}



