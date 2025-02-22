



static inline eta_t* get_eta(obj_t* obj) {
	return &obj->eta[obj->main_etat][obj->sub_etat];
}

static inline void obj_set_flipped(obj_t* obj) {
	obj->flags.flip_x = true;
}

static inline void obj_set_not_flipped(obj_t* obj) {
	obj->flags.flip_x = false;
}


// maybe move these to memory-related procedures?

//7FB48
void SpriteFixeOffset(mem_t* mem) {
    OffsetSpriteFixe = mem->cursor;
}

//7FB60
void SpriteWorldOffset(mem_t* mem) {
    OffsetSpriteWorld = mem->cursor;
}

//7FB78
void SpriteFixeBlocksFree(mem_t* mem) {
    mem->cursor = OffsetSpriteFixe;
}

//7FB84
void SpriteWorldBlocksFree(mem_t* mem) {
    mem->cursor = OffsetSpriteWorld;
}


// sub_1a3f0
void display_in_game_text(const char* text, int unknown1, int x, int y, int unknown2) {
    // stub
}



// Decodes and verifies xor-encoded data
u8 decode_xor(u8* data, u32 size, u8 encoding_byte, u8 checksum_byte) {
    u8* end = data + size;
    for (u8* pos = data; pos < end; ++pos) {
        u8 curr_byte = *pos;
        checksum_byte -= curr_byte; // overflows
        curr_byte ^= encoding_byte;
        *pos = curr_byte;
    }
    return checksum_byte; // should be 0
}


//inlined
void SetPalette(rgb_palette_t* palette, u8 first_color, i32 num_colors) {
    memcpy(global_game->draw_palette.colors + first_color, palette->colors + first_color, num_colors * sizeof(rgb_t));
}



//38400
i16 dummy_scene_func(u32 par_0) {
    return 1;
}





//498A4
void set_special_key_descriptions(const char** descriptions) {
    descriptions[SC_BACKSPACE] = language_txt[212]; // backspace
    descriptions[SC_TAB] = language_txt[213]; // tab
    descriptions[SC_ENTER] = language_txt[214]; // return
    descriptions[SC_CONTROL] = language_txt[210]; // ctrl
    descriptions[SC_ALT] = language_txt[211]; // alt
    descriptions[SC_SPACE] = language_txt[209]; // space
    descriptions[SC_CAPSLOCK] = language_txt[215]; // caps lock
    descriptions[SC_HOME] = language_txt[216]; // home
    descriptions[SC_UP] = language_txt[205]; // up arrow
    descriptions[SC_PAGEUP] = language_txt[217]; // page up
    descriptions[SC_LEFT] = language_txt[204]; // left arrow
    descriptions[SC_RIGHT] = language_txt[206]; // right arrow
    descriptions[SC_END] = language_txt[218]; // end
    descriptions[SC_DOWN] = language_txt[207]; // down arrow
    descriptions[SC_PAGEDOWN] = language_txt[219]; // page down
    descriptions[SC_INSERT] = language_txt[220]; // insert
    descriptions[SC_DELETE] = language_txt[221]; // del
}



image_t load_vignet_pcx(u32 resource_id) {
    ASSERT(sizeof(pcx_header_t) == 128);
    FILE* fp = open_data_file("VIGNET.DAT", true);
    image_t image = {0};
    if (fp) {
        archive_header_t* info = HeaderFilevignet + resource_id;
        fseek(fp, info->offset, 0);

        u8* pcx_buf = (u8*) malloc(info->size);
        fread(pcx_buf, info->size, 1, fp);
        u8* p = (u8*)pcx_buf;
        u8 checksum = info->checksum_byte;
        for (i32 i = 0; i < info->size; ++i) {
            u8 b = *p;
            checksum -= b;
            b ^= info->xor_byte;
            *p++ = b;
        }
#if 0
        FILE* test_out = fopen("test_out.pcx", "wb");
		fwrite(pcx_buf, info->size, 1, test_out);
		fclose(test_out);

#endif
        if (checksum != 0) {
            fatal_error();
        }

        pcx_header_t* header = (pcx_header_t*)pcx_buf;
        i32 width = header->max_x - header->min_x + 1;
        i32 height = header->max_y - header->min_y + 1;
        image.width = width;
        image.height = height;

        if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 /*|| header->palette_mode != 1*/) {
            // unsupported file structure
            fatal_error();
        } else {
            image.memory = (u8*)malloc(width * height);
            image.memory_size = width * height;
            // run-length decoding
            u8* pos = pcx_buf + 128;
            u8* end = pcx_buf + info->size;
            i32 i = 0;
            i32 decompressed_byte_count = width * height;
            while (i < decompressed_byte_count && pos < end) {
                u8 b = *pos;
                if (b >= 0xC0) {
                    // upper two bits are set -> remaining bits interpreted as run length (0-63)
                    i32 run_length = b & 0x3F;
                    ++pos;
                    if (i + run_length <= decompressed_byte_count && pos < end) {
                        b = *pos;
                        memset(image.memory + i, b, run_length);
                    } else {
                        fatal_error(); // RLE encoding exceeding the end of one of the buffers
                    }
                    ++pos;
                    i += run_length;
                } else {
                    // byte interpreted as single pixel value
                    image.memory[i] = b;
                    ++pos;
                    ++i;
                }
            }
            if (i != decompressed_byte_count) {
                fatal_error();
            }
            // Save the image palette at the end of the file
            i32 pal_offset = info->size - 768;
            image.pal = (rgb_palette_t*)malloc(sizeof(rgb_palette_t));
            image.pal_needs_free = true;
            memcpy(image.pal, pcx_buf + pal_offset, sizeof(rgb_palette_t));
            image.is_valid = true;
        }

        free(pcx_buf);


    } else {
        fatal_error();
    }
    return image;
}
