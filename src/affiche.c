
//18CE8
void display2(obj_t* obj) {
    anim_t* anim = obj->animations + obj->anim_index;
    u16 layers_per_frame = anim->layers_per_frame & 0x3FFF;
    anim_layer_t* layer = anim->layers + (obj->anim_frame * layers_per_frame);
    for (i32 layer_index = 0; layer_index < layers_per_frame; ++layer_index) {
        i32 proj_y = get_proj_y(obj->scale, layer->y + obj->screen_y);
        if (layer->sprite_index != 0) {
            sprite_t* sprite = obj->sprites + layer->sprite_index;
            if (sprite->unk_index != 0) {
                i32 x;
                if (obj->flags & obj_flags_8_flipped) {
                    if (obj->scale == 256 && layer_index == 5 && obj->anim_index >= 14 && obj->anim_index <= 16) {
                        x = -16;
                    } else {
                        x = 0;
                    }
                    x += obj->offset_bx * 2 - layer->x - sprite->outer_width + obj->screen_x;
                } else {
                    x = layer->x + obj->screen_x;
                }

                draw_func_t* draw_func = NULL;
                if ((obj->flags & obj_flags_8_flipped) ^ layer->mirrored) {
                    draw_func = DrawSpriteFlipNormalEtX;
                } else {
                    draw_func = DrawSpriteNormalEtX;
                }

                i32 proj_x = get_proj_x(obj->scale, x);
                i32 proj_height = get_proj_dist(obj->scale, sprite->outer_height);
                i32 proj_width = get_proj_dist(obj->scale, sprite->outer_width);
                vec2b_t proj_size = {(u8)proj_width, (u8)proj_height};
                u8 sprite_field_A = sprite->field_A >> 4;

                u8* image_data = obj->img_buffer + sprite->offset_in_atlas;

                draw_func(proj_x /*eax*/, sprite_field_A /*edx*/, proj_y /*ebx*/, proj_size /*ecx*/, /*edi*/ &global_game->draw_buffer, image_data /*esi*/);

            }
        }

        ++layer;
    }

}


//18EB0
void display_sprite(obj_t* obj, u8 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//18F7C
void display_sprite_NoClip(obj_t* obj, u8 a2, i16 a3, i16 a4, u8 a5) {
    //stub
}

//19048
void DISPLAY_POING(void) {
    if (poing.is_active) {
        display2(poing_obj);
    }
}

//19060
void DISPLAY_CLING(obj_t* obj) {
    //stub
}

//191A8
void display_bar_boss(obj_t* obj) {
    //stub
}

//193A8
void DisplayCrackers(void) {
    //stub
}

//193E4
void DisplayProgrammerMessage(void) {
    //stub
}

//19420
void DISPLAY_FIXE(i32 a1) {
    //stub
}

//19864
void DISPLAY_POINT(i16 a1, i16 a2) {
    //stub
}

//1987C
void DISPLAY_PTS_TO(i16 origin_x, i16 origin_y, i16 dest_x, i16 dest_y, i16 a5) {
    //stub
}

//198C4
void DISPLAY_PTS_TO_PLAN2(i16 origin_x, i16 origin_y, i16 dest_x, i16 dest_y, i16 a5) {
    //stub
}

//198F4
void DISPLAY_CYMBALE(obj_t* obj) {
    //stub
}

//19A2C
void DISPLAY_ALL_OBJECTS(void) {
    //stub
}

//19D2C
void display_flocons_behind(void) {
    //stub
}

//19F08
void display_flocons_before(void) {
    //stub
}

//1A110
void display_pix_gerbes(void) {
    //stub
}

//1A1C4
void DISPLAY_BLACKBOX(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, u8 a6) {
    //stub
}

//1A3F0
void display_text(const char* text, i16 a2, i16 a3, u8 a4, u8 a5) {
    //stub
}

//1A68C
void display_deform_text(const char* text, i16 a2, i16 a3, u8 a4, u8 a5, i16 a6, i16 a7, i16 a8) {
    //stub
}

//1A8DC
void display_text_sin(const char* text, i16 a2, i16 a3, u8 a4, u8 a5, u8 a6) {
    //stub
}

//1ABE0
void display_box_text(void* a1) {
    //stub
}

//1AD00
void CLRSCR(void) {
    //stub
}

//1AD38
void display_etoile(i16 a1, i16 a2) {
    //stub
}

//1AE50
void display_grp_stars(void) {
    //stub
}

//1AF1C
void DISPLAY_TEXT_FEE(void) {
    //stub
}

//1B0E0
void DISPLAY_SAVE_SPRITES(i16 a1, i16 a2) {
    //stub
}

//1B424
void DISPLAY_SAVE_POING(void) {
    //stub
}

//1B4AC
void DISPLAY_OPTIONS_POING(void) {
    //stub
}

//1B53C
void DISPLAY_YESNO_POING(void) {
    //stub
}

//1B56C
void display_time(i32 a1) {
    //stub
}

//1B79C
void DISPLAY_CONTINUE_SPR(void) {
    //stub
}

//1B944
void AFFICHE_SPRITE_DEFORME(void* a1, i32 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7, i16 a8, i16 a9, i16 a10) {
    //stub
}

//1BBC4
void DISPLAY_DEFORMATION(obj_t* obj, i16 a2, i16 a3, i16 a4) {
    //stub
}

//1BEA8
void DISPLAY_DEFORM_SPRITE(void* a1, i32 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7) {
    //stub
}

//1C030
void CALCUL_DEFORMATION(i16* a1, i16* a2, i16 a3, i16 a4, i16 a5) {
    //stub
}

