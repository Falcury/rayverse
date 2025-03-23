
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
    big_map->field_8 = block_malloc(mem, 8);
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
    //stub
}

//5C914
void update_display_map(big_map_t* a1) {
    //stub
}

//5C9F0
void set_default_Bloc_clipping(void) {
    Bloc_lim_H1 = 0;
    Bloc_lim_H2 = 200;
    Bloc_lim_W1 = 4;
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
    //stub
}

//5D230
void DRAW_MAP(void* draw_buf, big_map_t* big_map) {
    //stub
}

//5D5B0
void INIT_GAME_MODE_NORMAL(void) {
    sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    DrawBufferNormalIni = DrawBufferNormal;
    DrawBufferNormal += 4; //?
//    Copy_Plan0_To_Buf = (void*)&Copy_Plan0Diff_To_Buf; // TODO
    init_build_map(&BIG_MAP); //TODO
    N_CLRSCR(DrawBufferNormal);
}

//5D620
void FIN_GAME_MODE_NORMAL(void) {
    DrawBufferNormal = DrawBufferNormalIni;
    draw_buffer = DrawBufferNormalIni;
}
