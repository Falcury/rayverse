
//44DE0
void load_world(mem_t* mem_world, mem_t* mem_sprite, const char* filename) {
    stop_cd();
    if (GameModeVideo == 1) {
        PLAN1_BUFFER = block_malloc(mem_world, 71680);
    } else {
        PLAN1_BUFFER = 0;
    }
    for (i32 i = 0; i < 10; ++i) {
        Plan0NumPcx[i] = -1;
    }

    char full_filename[100];
    snprintf(full_filename, 100, "PCMAP" PATH_SEP "%s", filename);

    mem_t* mem = read_entire_file(full_filename, true);
    if (mem) {
        u16 background_width;
        u16 background_height;
        u8 plan0_num_pcx_count;

        // Header block
        mem_read(&background_width, mem, 2);
        mem_read(&background_height, mem, 2);
        if (GameModeVideo != 0) {
            if (FondAutorise != 0) {
                PLAN0 = allocate_PLAN0(mem_world, background_width, background_height);
            }
        } else {
            PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);
        }
        mem_read(&plan0_num_pcx_count, mem, 1);
        mem_read(&BiosCheckSum, mem, 1);
        mem_read(&VideoBiosCheckSum, mem, 1);

        mem_read(Plan0NumPcx, mem, plan0_num_pcx_count);
        for (i32 i = 0; i < plan0_num_pcx_count; ++i) {
            Plan0NumPcx[i] ^= 0x15;
        }

        // Sprites block
        mem_read(&nb_des, mem, 2);
        for (i32 des_index = nb_fix_des; des_index < nb_des + nb_fix_des; ++des_index) {

            obj_t* cur_wldobj = wldobj + des_index;

            u8 sprite_is_always_used; // bg clearing flag?
            mem_read(&sprite_is_always_used, mem, 1);
            mem_read(&cur_wldobj->x, mem, 4); // NOTE: this is read, but cur_wldobj->xpos is used instead

            if (sprite_is_always_used || ToutSpriteAutorise) {
                cur_wldobj->img_buffer = (u8*) block_malloc(mem_sprite, cur_wldobj->x);
                mem_read(cur_wldobj->img_buffer, mem, cur_wldobj->x);

                u8 img_buffer_checksum;
                mem_read(&img_buffer_checksum, mem, 1);

                for (i32 i = 0; i < plan0_num_pcx_count; ++i) {
                    cur_wldobj->img_buffer[i] ^= 0x8F;
                    img_buffer_checksum -= cur_wldobj->img_buffer[i];
                }

                if (img_buffer_checksum != 0) {
                    // error condition
                }

                mem_read(&cur_wldobj->nb_sprites, mem, 2);
                cur_wldobj->sprites = (sprite_t*)block_malloc(mem_world, cur_wldobj->nb_sprites * sizeof(sprite_t));
                mem_read(cur_wldobj->sprites, mem, cur_wldobj->nb_sprites * sizeof(sprite_t));

                mem_read(&cur_wldobj->anim_count, mem, 1);
                if (cur_wldobj->anim_count != 0) {
                    cur_wldobj->animations = (anim_t*)block_malloc(mem_world, cur_wldobj->anim_count * sizeof(anim_t));
                    for (i32 anim_index = 0; anim_index < cur_wldobj->anim_count; ++anim_index) {
                        anim_t* anim = cur_wldobj->animations + anim_index;
                        mem_read(&anim->layers_per_frame, mem, 2);
                        mem_read(&anim->frame_count, mem, 2);
                        i32 frames_ptr = 0;
                        mem_read(&frames_ptr, mem, 4); // ptr field, but might be -1
                        u16 layer_table_size;
                        mem_read(&layer_table_size, mem, 2);
                        anim->layers = (anim_layer_t*) block_malloc(mem_world, layer_table_size);
                        mem_read(anim->layers, mem, layer_table_size);
                        if (frames_ptr != -1) {
                            ASSERT(sizeof(anim_frame_t) == 4);
                            anim->frames = (anim_frame_t*) block_malloc(mem_world, anim->frame_count * sizeof(anim_frame_t));
                            mem_read(anim->frames, mem, anim->frame_count * sizeof(anim_frame_t));
                        }

                    }
                }
            } else {
                mem->cursor += cur_wldobj->x; // skip
                u8 img_buffer_checksum;
                mem_read(&img_buffer_checksum, mem, 1);
                mem_read(&cur_wldobj->nb_sprites, mem, 2);
                mem->cursor += cur_wldobj->nb_sprites * sizeof(sprite_t);
                mem_read(&cur_wldobj->anim_count, mem, 1);
                if (cur_wldobj->anim_count != 0) {
                    cur_wldobj->animations = NULL;
                    for (i32 anim_index = 0; anim_index < cur_wldobj->anim_count; ++anim_index) {
                        u16 layers_per_frame = 0;
                        mem_read(&layers_per_frame, mem, 2);
                        u16 frame_count = 0;
                        mem_read(&frame_count, mem, 2);
                        i32 frames_ptr = 0;
                        mem_read(&frames_ptr, mem, 4); // ptr field, but might be -1
                        u16 layer_table_size;
                        mem_read(&layer_table_size, mem, 2);
                        mem->cursor += layer_table_size;
                        if (frames_ptr != -1) {
                            mem->cursor += frame_count * sizeof(anim_frame_t);
                        }
                    }
                }
            }
        }
        nb_des += nb_fix_des;

        // ETA block
        mem_read(&nb_loaded_eta, mem, 1);
        for (i32 i = nb_fix_eta; i < nb_loaded_eta + nb_fix_eta; ++i) {
            u8 num_eta_groups = 0;
            mem_read(&num_eta_groups, mem, 1);
            loaded_eta[i] = (eta_t**)block_malloc(mem_world, num_eta_groups * sizeof(eta_t*));
            for (i32 j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries = 0;
                mem_read(&num_eta_entries, mem, 1);
                loaded_eta[i][j] = (eta_t*)block_malloc(mem_world, num_eta_entries * sizeof(eta_t));
                mem_read(loaded_eta[i][j], mem, sizeof(eta_t) * num_eta_entries);
            }
        }
        nb_loaded_eta += nb_fix_eta;

        if (FondAutorise != 2) {
            //LoadPlan0InVignet(Plan0NumPcx[0]);
        }

        free(mem);

        //do_some_drm_check();

    }
}

//45688
void load_level(mem_t* mem_level, const char* filename) {
    mem_t* mem = read_entire_file(filename, true);
    if (mem) {
        //stream_t stream = mem_create_stream(mem);

        // Header
        u32 event_block_offset;
        u32 texture_block_offset;
        mem_read(&event_block_offset, mem, 4);
        mem_read(&texture_block_offset, mem, 4);
        mem_read(&mp.width, mem, 2);
        mem_read(&mp.height, mem, 2);
        mem_read(rvb, mem, sizeof(rvb));
        mem_read(&active_palette, mem, 1);

        // Map
        mp.length = mp.width * mp.height;
        map_tile_t* tiles = (map_tile_t*) malloc(mp.length * sizeof(map_tile_t));
        mem_read(tiles, mem, mp.length * sizeof(map_tile_t));

        u8 background_image_static;
        u8 background_image_scrolling;
        u32 background_DES;
        mem_read(&background_image_static, mem, 1);
        mem_read(&background_image_scrolling, mem, 1);
        mem_read(&background_DES, mem, 4);

        // X Mode Textures
        u32 x_texture_count;
        u32 x_unk_table_size;
        u8 x_texture_checksum;
        u32 x_texture_index_table[1200];
        mem_read(&x_texture_count, mem, 4);
        mem_read(&x_unk_table_size, mem, 4);
        x_texture_t* x_textures = (x_texture_t*) malloc(x_texture_count * sizeof(x_texture_t));
        mem_read(x_textures, mem, x_texture_count * sizeof(x_texture_t));
        mem_read(&x_texture_checksum, mem, 1);
        mem_read(x_texture_index_table, mem, sizeof(x_texture_index_table));

        u8* x_unk_table = (u8*) malloc(x_unk_table_size);
        u8 x_unk_table_checksum;
        u32 x_unk_offsets[1200];
        mem_read(x_unk_table, mem, x_unk_table_size);
        mem_read(&x_unk_table_checksum, mem, 1);
        mem_read(x_unk_offsets, mem, sizeof(x_unk_offsets));

        // Normal Mode Textures
        u32 texture_offsets[1200];
        u32 texture_count;
        u32 opaque_texture_count;
        u32 texture_data_size;
        u8 texture_data_checksum;
        mem_read(texture_offsets, mem, sizeof(texture_offsets));
        mem_read(&texture_count, mem, 4);
        mem_read(&opaque_texture_count, mem, 4);
        mem_read(&texture_data_size, mem, 4);
        u8* texture_data = (u8*) malloc(texture_data_size);
        mem_read(texture_data, mem, texture_data_size);
        mem_read(&texture_data_checksum, mem, 1);
        // TODO: further parse texture data block


        // Events






    }
}

//46078
void load_big_ray(mem_t* buffer) {
    // TODO: allocate from a memory pool (main_mem_tmp), so that it can be freed at once later
    FILE* fp = open_data_file("PCMAP" PATH_SEP "BRAY.DAT", true);
    if (fp) {
        obj_t* sprite_group = wldobj + nb_des;
        u32 sprite_group_size = 0;
        fread(&sprite_group_size, 4, 1, fp);
        sprite_group->x = (i32)sprite_group_size; // why is this saved in the xpos field??
        u8* image_atlas = block_malloc(buffer, sprite_group_size);
        fread(image_atlas, sprite_group_size, 1, fp);
        for (u32 i = 0; i < sprite_group_size; ++i) {
            image_atlas[i] ^= 0x8F;
        }
        sprite_group->img_buffer = image_atlas;

        fread(&sprite_group->nb_sprites, 2, 1, fp);
        ASSERT(sizeof(sprite_t) == 12);
        u8* sprite_buffer = block_malloc(buffer, sprite_group->nb_sprites * sizeof(sprite_t));
        fread(sprite_buffer, sizeof(sprite_t), sprite_group->nb_sprites, fp);
        sprite_group->sprites = (sprite_t*)sprite_buffer;

        fread(&sprite_group->anim_count, 1, 1, fp);
        sprite_group->animations = (anim_t*)block_malloc(buffer, sprite_group->anim_count * sizeof(anim_t));
        for (i32 i = 0; i < sprite_group->anim_count; ++i) {
            anim_t* anim_desc = sprite_group->animations + i;
            fread(&anim_desc->layers_per_frame, 2, 1, fp);
            fread(&anim_desc->frame_count, 2, 1, fp);
            i32 field_4 = 0;
            fread(&field_4, 4, 1, fp); // ptr field, but might be -1
//			anim_desc->anim_layers_or_frames = (u8*)field_4;
            u16 layer_table_size = 0;
            fread(&layer_table_size, 2, 1, fp);
            anim_desc->layers = (anim_layer_t*) block_malloc(buffer, layer_table_size);
            fread(anim_desc->layers, 1, layer_table_size, fp);
            if (field_4 != -1) {
                anim_desc->frames = (anim_frame_t*) block_malloc(buffer, anim_desc->frame_count * 4);
                fread(anim_desc->frames, 4, anim_desc->frame_count, fp);
            }
        }
        fread(&nb_loaded_eta, 1, 1, fp);
        ASSERT(nb_loaded_eta < COUNT(loaded_eta));
        ASSERT(sizeof(eta_t) == 8);
        for (i32 i = 0; i < nb_loaded_eta; ++i) {
            u8 num_eta_groups = 0;
            fread(&num_eta_groups, 1, 1, fp);
            loaded_eta[i] = (eta_t**) block_malloc(buffer, num_eta_groups * sizeof(eta_t*));
            for (i32 j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries = 0;
                fread(&num_eta_entries, 1, 1, fp);
                loaded_eta[i][j] = (eta_t*) block_malloc(buffer, num_eta_entries * sizeof(eta_t));
                fread(loaded_eta[i][j], sizeof(eta_t), num_eta_entries, fp);
            }
        }
        sprite_group->eta = loaded_eta[0];
        bigray = *sprite_group; // instantiates the bigray object?
        ++nb_des;

        fclose(fp);

    }
    //sub_7FA70(filename);
    //sub_6BAE0();
}

//464BC
void LOAD_ALL_FIX(void) {
    init_memory(&main_mem_sprite, TailleMainMemSprite);
    init_memory(&main_mem_level, TailleMainMemLevel);
    init_memory(&main_mem_world, TailleMainMemWorld);
    init_memory(&main_mem_fix, TailleMainMemFix);
    const char* filename = "PCMAP" PATH_SEP "ALLFIX.DAT";
    mem_t* mem = read_entire_file(filename, true);
    if (mem) {
        mem_read(&nb_loaded_eta, mem, 1);
        nb_fix_eta = nb_loaded_eta;
        for (i32 i = 0; i < nb_loaded_eta; ++i) {
            u8 num_eta_groups = 0;
            mem_read(&num_eta_groups, mem, 1);
            loaded_eta[i] = (eta_t**)block_malloc(main_mem_fix, num_eta_groups * sizeof(eta_t*));
            for (i32 j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries = 0;
                mem_read(&num_eta_entries, mem, 1);
                loaded_eta[i][j] = (eta_t*)block_malloc(main_mem_fix, num_eta_entries * sizeof(eta_t));
                mem_read(loaded_eta[i][j], mem, sizeof(eta_t) * num_eta_entries);
            }
        }

        mem_read(&nb_des, mem, 2);
        nb_fix_des = nb_des;
        i32 des_index = 1; // apparently 0 gets skipped?
        for (; des_index < nb_des; ++des_index) {

            obj_t* obj = wldobj + des_index;

            i32 which_eta = 0;
            mem_read(&which_eta, mem, 4);
            if (which_eta == -1) {
                obj->eta = NULL;
            } else {
                obj->eta = loaded_eta[which_eta];
            }

            mem_read(&RaymanExeSize, mem, 4);
            mem_read(&RaymanExeCheckSum1, mem, 4);

            u32 sprite_group_size = 0;
            mem_read(&sprite_group_size, mem, 4);
            obj->x = (i32)sprite_group_size; // why is this saved in the xpos field??
            u8* image_atlas = block_malloc(main_mem_sprite, sprite_group_size);
            i32 saved_cursor = mem->cursor; // in the original, this is a call to ftell()
            mem_read(image_atlas, mem, sprite_group_size);

            u8 checksum_byte = 0;
            mem_read(&checksum_byte, mem, 1);

            for (u32 i = 0; i < sprite_group_size; ++i) {
                checksum_byte -= image_atlas[i];
                image_atlas[i] ^= 0x8F;
            }
            if (checksum_byte != 0) {
                printf("%s : File integrity fault.\n", filename);
                fatal_error();
            }
            obj->img_buffer = image_atlas;

            mem_read(&RaymanExeCheckSum2, mem, 4);
            mem_read(&obj->nb_sprites, mem, 2);
            ASSERT(sizeof(sprite_t) == 12);
            u8* sprite_buffer = block_malloc(main_mem_fix, obj->nb_sprites * sizeof(sprite_t));
            mem_read(sprite_buffer, mem, sizeof(sprite_t) * obj->nb_sprites);
            obj->sprites = (sprite_t*)sprite_buffer;

            mem_read(&obj->anim_count, mem, 1);
            if (obj->anim_count > 0) {
                obj->animations = (anim_t*)block_malloc(main_mem_fix, obj->anim_count * sizeof(anim_t));
                for (i32 i = 0; i < obj->anim_count; ++i) {
                    anim_t* anim_desc = obj->animations + i;
                    mem_read(&anim_desc->layers_per_frame, mem, 2);
                    mem_read(&anim_desc->frame_count, mem, 2);
                    i32 frames_ptr = 0;
                    mem_read(&frames_ptr, mem, 4); // ptr field, but might be -1
                    u16 layer_table_size = 0;
                    mem_read(&layer_table_size, mem, 2);
                    anim_desc->layers = (anim_layer_t*) block_malloc(main_mem_fix, layer_table_size);
                    mem_read(anim_desc->layers, mem, layer_table_size);
                    if (frames_ptr != -1) {
                        ASSERT(sizeof(anim_frame_t) == 4);
                        anim_desc->frames = (anim_frame_t*) block_malloc(main_mem_fix, anim_desc->frame_count * sizeof(anim_frame_t));
                        mem_read(anim_desc->frames, mem, anim_desc->frame_count * sizeof(anim_frame_t));
                    }
                }
            }
        }
        mem_read(&RaymanExeCheckSum3, mem, 4);

        mem_read(&des_index, mem, 4);
        ray = wldobj[des_index];

        mem_read(&des_index, mem, 4);
        alpha = wldobj + des_index;
        alpha_image_atlas = alpha->img_buffer;
        alpha_sprite_count = alpha->nb_sprites;

        mem_read(&des_index, mem, 4);
        alpha2 = wldobj + des_index;
        alpha2_image_atlas = alpha2->img_buffer;
        alpha2_sprite_count = alpha2->nb_sprites;

        mem_read(&des_index, mem, 4);
        obj_t* obj = wldobj + des_index;
        raylittle.sprites = obj->sprites;
        raylittle.img_buffer = obj->img_buffer;
        raylittle.nb_sprites = obj->nb_sprites;

        mem_read(&des_index, mem, 4);
        obj = wldobj + des_index;
        for (i32 i = 0; i < 25; ++i) {
            obj_t* m = mapobj + i;
            m->sprites = obj->sprites;
            m->animations = obj->animations;
            m->img_buffer = obj->img_buffer;
            m->nb_sprites = obj->nb_sprites;
            m->anim_count = obj->anim_count;
            m->eta = obj->eta;
        }

        mem_read(&des_index, mem, 4);
        clockobj = wldobj[des_index];

        mem_read(&des_index, mem, 4);
        div_obj = wldobj[des_index];

        free(mem);
    }
}

//46D68
image_t LoadPcxInVignet(mem_t* buffer, i32 resource_id, i16* width, i16* height, rgb_palette_t* pal_to_save) {
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
        image.width = header->max_x - header->min_x + 1;
        image.height = header->max_y - header->min_y + 1;
        if (width) *width = (i16)image.width;
        if (height) *height = (i16)image.height;

        if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 /*|| header->palette_mode != 1*/) {
            // unsupported file structure
            fatal_error();
        } else {
            i32 decompressed_byte_count = image.width * image.height;
            image.memory = (u8*)block_malloc(buffer, decompressed_byte_count);
            image.memory_size = decompressed_byte_count;
            // run-length decoding
            u8* pos = pcx_buf + 128;
            u8* end = pcx_buf + info->size;
            i32 i = 0;
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
            // NOTE: This code is not in the original - I added it to avoid a redundant call to LoadPcxPaletteInVignet
            // in case you want to use the palette (otherwise you will need to read the file twice...)
            if (pal_to_save) {
                i32 pal_offset = info->size - 768;
                image.pal = pal_to_save;
                image.pal_needs_free = false;
                memcpy(pal_to_save, pcx_buf + pal_offset, sizeof(rgb_palette_t));
            }
            image.is_valid = true;

        }
        free(pcx_buf);

    } else {
        fatal_error();
    }
    return image;
}

//47280
void LoadPcxPaletteInVignet(i32 resource_id, rgb_palette_t* palette) {
    ASSERT(sizeof(pcx_header_t) == 128);
    ASSERT(palette);
    FILE* fp = open_data_file("VIGNET.DAT", true);
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
        if (checksum != 0) {
            fatal_error();
        }

        pcx_header_t* header = (pcx_header_t*)pcx_buf;
        if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 /*|| header->palette_mode != 1*/) {
            // unsupported file structure
            fatal_error();
        } else {
            // Save the image palette at the end of the file
            // NOTE: This code is not in the original - I added it to avoid a redundant call to LoadPcxPaletteInVignet
            // in case you want to use the palette (otherwise you will need to read the file twice...)

            i32 pal_offset = info->size - 768;
            memcpy(palette, pcx_buf + pal_offset, sizeof(rgb_palette_t));

        }
        free(pcx_buf);
    } else {
        fatal_error();
    }
}

//473D0
void LoadPlan0InVignet(i32 resource_id) {
    //stub
}

//47BB0
u8* allocate_PLAN0(mem_t* mem_world, i32 width, i32 height) {
    u8* buffer1;
    u8* buffer2;
    if (FondAutorise == 2) {
        buffer1 = DrawBufferNormal;
        buffer2 = EffetBufferNormal;
    } else {
        buffer1 = block_malloc(mem_world, width * height);
        buffer2 = block_malloc(mem_world, 2 * width * height + 8 * width);
    }
    PLAN0BIT = buffer2;
    return buffer1;
}

//47C10
void PLAN0FND_to_bits_planes(u8* buffer, i16 width, i16 height) {
    //stub
}

//47D3C
void LoadPlan2InVignet(mem_t* buffer, i32 resource_id) {
    stop_cd();
    i16 width = 0;
    i16 height = 0;
    image_t image = LoadPcxInVignet(buffer, resource_id, &width, &height, &current_rvb);
    PLAN2 = image.memory;
    //LoadPcxPaletteInVignet(resource_id, &current_rvb); // we already loaded the palette in LoadPcxInVignet(), so we can skip this
    rvb_pres = current_rvb;
    plan2_width = width;
    plan2bit_length = width * height;
    plan2bit_nb_bytes = width >> 2;
    PLAN2BIT = PLAN2;

    //stub
}

//47DD8
char* GetStringTxt(char* txt, char* out_buf) {
    char* pos = txt;
    char c = *pos;
    if (c != ',') {
        for (;;) {
            c = *pos;
            if (c == '*') {
                break;
            }
            if (c >= ' ') {
                *out_buf++ = c;
            }
            ++pos;
            if (*pos == ',') {
                break;
            }
        }
    }
    if (*pos == '*') {
        return NULL;
    } else {
        *out_buf = '\0';
        return pos + 1;
    }
}


char* language_buffer;

//47E10
void LoadLanguageTxt(mem_t* mem, i32 language_index) {
    mem_t* lng_file = read_entire_file("RAY.LNG", true);
    if (lng_file) {
        archive_header_t* lng_archive_header = &language_infos[language_index]; // is language_index used here?
        language_buffer = (char*) block_malloc(mem, lng_archive_header->size);
        lng_file->cursor = lng_archive_header->offset;
        i32 bytes_read = mem_read(language_buffer, lng_file, lng_archive_header->size);
        if (bytes_read != lng_archive_header->size) {
            // error
            exit(1);
        }

        // decode language txts
        u8* buffer_pos = (u8*) language_buffer;
        u8* buffer_end = (u8*) language_buffer + lng_archive_header->size;
        u8 checksum_byte = lng_archive_header->checksum_byte;
        u8 decoding_byte = lng_archive_header->xor_byte;
        for (; buffer_pos < buffer_end; ++buffer_pos) {
            u8 c = *buffer_pos;
            checksum_byte -= c;
            c ^= decoding_byte;
            *buffer_pos = c;
        }

        char* txt = language_buffer;
        char* next_txt = txt;
        for (i32 i = 0; next_txt != NULL; ++i) {
            char temp_buffer[400];
            next_txt = GetStringTxt(txt, temp_buffer);
            if (next_txt != NULL) {
                language_txt[i] = txt;
                next_txt[-1] = '\0'; // zero-terminate txt
                txt = next_txt + 2; // skip \r\n
            }
        }

#if 0
        // DEBUG: dump to file
		FILE* fp = fopen("language_dump.txt", "wb");
		fwrite(language_buffer, language->size, 1, fp);
		fclose(fp);
#endif

    }
}

//48038
void LoadPcxOptions(u8* buffer, i32 resource_id, i16* width, i16* height, rgb_palette_t* pal_to_save) {
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
        image.width = header->max_x - header->min_x + 1;
        image.height = header->max_y - header->min_y + 1;
        if (width) *width = (i16)image.width;
        if (height) *height = (i16)image.height;

        if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 /*|| header->palette_mode != 1*/) {
            // unsupported file structure
            fatal_error();
        } else {
            i32 decompressed_byte_count = image.width * image.height;
            // run-length decoding
            u8* pos = pcx_buf + 128;
            u8* end = pcx_buf + info->size;
            i32 i = 0;
            while (i < decompressed_byte_count && pos < end) {
                u8 b = *pos;
                if (b >= 0xC0) {
                    // upper two bits are set -> remaining bits interpreted as run length (0-63)
                    i32 run_length = b & 0x3F;
                    ++pos;
                    if (i + run_length <= decompressed_byte_count && pos < end) {
                        b = *pos;
                        memset(buffer + i, b, run_length);
                    } else {
                        fatal_error(); // RLE encoding exceeding the end of one of the buffers
                    }
                    ++pos;
                    i += run_length;
                } else {
                    // byte interpreted as single pixel value
                    buffer[i] = b;
                    ++pos;
                    ++i;
                }
            }
            if (i != decompressed_byte_count) {
                fatal_error();
            }
            // Save the image palette at the end of the file
            // NOTE: This code is not in the original - I added it to avoid a redundant call to LoadPcxPaletteInVignet
            // in case you want to use the palette (otherwise you will need to read the file twice...)
            if (pal_to_save) {
                i32 pal_offset = info->size - 768;
                image.pal = pal_to_save;
                image.pal_needs_free = false;
                memcpy(pal_to_save, pcx_buf + pal_offset, sizeof(rgb_palette_t));
            }
            image.is_valid = true;

        }
        free(pcx_buf);

    } else {
        fatal_error();
    }
}


