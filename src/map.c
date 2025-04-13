
// NOTE: unsure about the source filename map.c (in the function order, should come after neige.c and before ngawe.c)


//5C4A0
void deter_vide_plein_panach(void* a1, i16 a2) {
    //stub
}

//5C4DC
void deter_nb_blocks(void* a1, i16 a2, i16 a3, i32* a4, i32* a5) {
    //stub
}

//5C564
void Copy_Blocks(mem_t* buffer, void* a2, i16 a3, i16 a4) {
    //stub
}

//5C82C
void construct_MAP(mem_t* mem, big_map_t* big_map, void* map_blocks) {
    big_map->map = mp.map;
    big_map->tile_texture_offsets = (u8**)block_malloc(mem, mp.length * sizeof(u8*));
    big_map->map_blocks = (u8*)map_blocks;
    big_map->display_map = (display_map_t*)block_malloc(mem, sizeof(display_map_t));
    for (i32 i = 0; i < mp.length; ++i) {
        i32 texture_offset = block_add[big_map->map[i].texture_id];
        big_map->tile_texture_offsets[i] = big_map->map_blocks + texture_offset;
        u32 v11;
        if (texture_offset > 288 * nb_blocks_plein) {
            v11 = *(u32*)(big_map->tile_texture_offsets[i] + 512);
        } else {
            v11 = *(u32*)(big_map->tile_texture_offsets[i] + 256);
        }
        u8 transparency;
        if (v11 == 0xAAAAAAAA) {
            transparency = 1;
        } else if (v11 == 0x55555555) {
            transparency = 0;
        } else {
            transparency = 2;
        }
        big_map->map[i].transparency = transparency;
    }
}

//5C8FC
void init_build_map(big_map_t* a1) {
    a1->width = mp.width;
    a1->height = mp.height;
    update_display_map(a1);
}

//5C914
void update_display_map(big_map_t* a1) {
    display_map_t* display_map = a1->display_map;

    i32 y = Bloc_lim_H1 + ymap;
    if (y < 0) {
        y = 0; // added bounds clip
    }
    i32 x = Bloc_lim_W1 + xmap - 8;
    if (x < 0) {
        x = 0; // added bounds clip
    }
    display_map->x = Bloc_lim_W1 - (x & 0xF);
    display_map->tile_index = (a1->width * (y >> 4)) + (x >> 4);
    display_map->y= Bloc_lim_H1 - (y & 0xF);

    i32 v6 = Bloc_lim_W2 - display_map->x;
    i32 v7 = Bloc_lim_H2 - display_map->y;
    display_map->width_in_tiles = (v6 >> 4) + ((v6 & 0xF) != 0);
    display_map->height_in_tiles = (v7 >> 4) + ((v7 & 0xF) != 0);

    BufferNormalDeplt = 0;
    // NOTE: in the PC version, there is a strange -4 here (not in the Android version). Also see INIT_GAME_MODE_NORMAL()
    DrawBufferNormal = DrawBufferNormalIni;
    draw_buffer = DrawBufferNormalIni;
}

//5C9F0
void set_default_Bloc_clipping(void) {
    Bloc_lim_H1 = 0;
    Bloc_lim_H2 = 200;
    Bloc_lim_W1 = 4; //TODO: change to 0? (it's also 0 in the Android version)
    Bloc_lim_W2 = 320;
}


//5CA24
void set_xymapini(void) {
    xmapmax = 16 * mp.width - Bloc_lim_W2;
    ymapmax = 16 * mp.height - Bloc_lim_H2;
    xmapmaxini = 16 * mp.width - 320;
    ymapmaxini = 16 * mp.height - 200;
    if (xmapmax < 0) {
        xmapmax = 0;
    }
    if (ymapmax < 0) {
        ymapmax = 0;
    }
    scroll_start_x = 8 - Bloc_lim_W1;
    scroll_end_x = xmapmax + 8;
    scroll_start_y = -Bloc_lim_H1;
    scroll_end_y = ymapmax;
}

//5CAD0
void set_xymap(void) {
    i32 v0 = xmap;
    if (xmap < -Bloc_lim_W1) {
        xmap = -Bloc_lim_W1;
    }
    if (xmap > xmapmax) {
        xmap = xmapmax;
    }
    if (ymap < -Bloc_lim_H1) {
        ymap = -Bloc_lim_H1;
    }
    if (ymap > ymapmax) {
        ymap = ymapmax;
    }
}

//5CB40
void set_whmap(i32 a1, i32 a2, void* a3) {
    //stub
}

//5CD64
void recaleRaysurlesBords(void) {
    if (ray.x + ray.offset_bx <= xmap + Bloc_lim_W1 +16) {
        ray.x = xmap + Bloc_lim_W1 - ray.offset_bx + 16;
        ray.screen_x = ray.x - scroll_start_x + 8;
    }
    if (ray.x + ray.offset_bx >= xmap + Bloc_lim_W2 - 23) {
        ray.x = xmap + Bloc_lim_W2 - 32;
        ray.screen_x = ray.x - xmap + 8;
    }
}

//5CE08
void set_xywhmap(i16 xmin, i16 xmax, i16 ymin, i16 ymax) {
    i32 delta_W1 = xmin - Bloc_lim_W1;
    i32 delta_W2 = Bloc_lim_W2 - xmax;
    i32 delta_H1 = ymin - Bloc_lim_H1;
    i32 delta_H2 = Bloc_lim_H2 - ymax;

    Bloc_lim_H1 = ymin;
    Bloc_lim_H2 = ymax;
    Bloc_lim_W1 = xmin;
    Bloc_lim_W2 = xmax;

    if (ymax > 200) {
        Bloc_lim_H2 = 200;
    }
    if (ymin < 0) {
        Bloc_lim_H1 = 0;
    }
    if (xmax > 320) {
        Bloc_lim_W2 = 320;
    }
    if (xmin < 0) {
        Bloc_lim_W1 = 0;
    }

    if (Bloc_lim_W1 >= 4) {
        scroll_start_x -= delta_W1;
        scroll_end_x += delta_W2;
        if (scroll_start_x > 12) {
            scroll_start_x = 12;
        }
        if (scroll_end_x < xmapmaxini) {
            scroll_end_x = xmapmaxini + 8;
        }
        xmapmax += delta_W2;
        if (xmapmax < xmapmaxini) {
            xmapmax = xmapmaxini;
        }
        if (xmapmax < xmap) {
            xmapmax = xmap;
        }
    }

    if (Bloc_lim_H1 >= 0) {
        scroll_start_y -= delta_H1;
        scroll_end_y += delta_H2;
        if (scroll_start_y > 0) {
            scroll_start_y = 0;
        }
        if (scroll_end_y < ymapmaxini) {
            scroll_end_y = ymapmaxini;
        }
        ymapmax += delta_H2;
        if (ymapmax < ymapmaxini) {
            ymapmax = ymapmaxini;
        }
        if (ymapmax < ymap) {
            ymapmax = ymap;
        }
    }

    if (bossScrollStartX != 0) {
        bossScrollStartX = scroll_start_x;
        bossScrollEndX = scroll_end_x;
    }
    if (mp.width <= 20 && Bloc_lim_W1 == 4) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    set_xymap();
    if (scrollLocked) {
        if (!((num_world == world_3_mountain && num_level == 10) || (num_world == world_2_music && num_level == 15))) {
            scroll_end_x = xmap;
            scroll_start_x = xmap;
        }
    }

    if (xmapmax + Bloc_lim_W2 > 16 * mp.width) {
        xmapmax = 16 * mp.width - Bloc_lim_W2;
    }
    if (ymapmax + Bloc_lim_H2 > 16 * mp.height) {
        ymapmax = 16 * mp.height - Bloc_lim_H2;
    }
    Bloc_lim_W1_Aff = Bloc_lim_W1;
    Bloc_lim_W2_Aff = Bloc_lim_W2;
    Bloc_lim_H1_Aff = Bloc_lim_H1;
    Bloc_lim_H2_Aff = Bloc_lim_H2;
    if (OptionGame) {
        recaleRaysurlesBords();
    }
}

//5D144
void MaskScrollDiffSprites(mem_t* buffer) {
    for (i32 i = 1; i < ScrollDiffSprites->nb_sprites; ++i) {
        sprite_t* sprite = ScrollDiffSprites->sprites + i;
        Scroll_Masque[i] = block_malloc(buffer, sprite->outer_height * sprite->inner_height);
        u8* masque_pos = Scroll_Masque[i];
        u8* image_pos = ScrollDiffSprites->img_buffer + sprite->offset_in_atlas;
        for (i32 y = 0; y < sprite->outer_height; ++y) {
            for (i32 x = 0; x < sprite->outer_width; ++x) {
                *masque_pos = *image_pos == 0 ? 0 : 0xFF;
                ++masque_pos;
                ++image_pos;
            }
        }
    }
}

//5D230
void DRAW_MAP(u8* draw_buf, big_map_t* big_map) {
    display_map_t* display_map = big_map->display_map;
    if (display_map->height_in_tiles > 1 && display_map->width_in_tiles > 1) {
        u8* draw_pos = draw_buf + 320 * display_map->y + display_map->x;
        for (i32 tile_y = 0; tile_y < display_map->height_in_tiles; ++tile_y) {
            for (i32 tile_x = 0; tile_x < display_map->width_in_tiles; ++tile_x) {
                i32 tile_index = display_map->tile_index + tile_y * big_map->width + tile_x;
                map_tile_t* tile = big_map->map + tile_index;
                i32 x = display_map->x + 16 * tile_x;
                i32 y = display_map->y + 16 * tile_y;
                if (tile->transparency != 0) {
                    vec2b_t size = {16, 16};
                    if (tile->transparency == 1) {
                        // NOTE: We don't need to do fancy optimized drawing routines here, the regular one also works fine.
                        DrawSpriteNormal256(x, 0, y, size, draw_buf, big_map->tile_texture_offsets[tile_index]);
                        //Display_Bloc_Plein_Clippe(...);
                    } else {
                        DrawSpriteNormal256(x, 0, y, size, draw_buf, big_map->tile_texture_offsets[tile_index]);
                        //Display_Bloc_Clippe(...);
                    }
                }
            }
        }

    }
}

//5D5B0
void INIT_GAME_MODE_NORMAL(void) {
    sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    DrawBufferNormalIni = DrawBufferNormal;
    //DrawBufferNormal += 4; //? Maybe some strange workaround for something? Doesn't make sense, commenting out for now
    // also see update_display_map()
    Copy_Plan0_To_Buf = Copy_Plan0Diff_To_Buf;
    init_build_map(&BIG_MAP);
    N_CLRSCR(DrawBufferNormal);
}

//5D620
void FIN_GAME_MODE_NORMAL(void) {
    DrawBufferNormal = DrawBufferNormalIni;
    draw_buffer = DrawBufferNormalIni;
}
