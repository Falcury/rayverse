

//1FA40
void DO_BAT_FLASH(i16 a1, i16 a2) {
    //stub
}

//1FBC0
void DO_BAT_LEFT_FLASH(obj_t* obj) {
    //stub
}

//1FC18
void DO_BAT_RIGHT_FLASH(obj_t* obj) {
    //stub
}

//1FC70
void DO_BAT_LEFT_RIGHT_FLASH(obj_t* obj) {
    //stub
}

//1FD14
i16 bat_dir(obj_t* obj) {
    /* 71A5C 8019625C -O2 -msoft-float */
    return (ray.x + ray.offset_bx - obj->x - obj->offset_bx) > 0;
}

//1FD9C
void DO_BAT_COMMAND(obj_t* obj) {
    //stub
}

//20058
void BAT_ray_in_zone(obj_t* obj) {
    //stub
}

//20088
void DO_BAT_POING_COLLISION(obj_t* obj, i16 sprite) {
    //stub
}

//2008C
i32 bat_get_eject_sens(obj_t* obj) {
    /* 72344 80196B44 -O2 -msoft-float */
    ray.iframes_timer = 40;
    return bat_dir(obj) ? 1 : -1;
}

