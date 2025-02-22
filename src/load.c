
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
            // NOTE: is the background buffer wider to allow for differential scrolling?
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
            // NOTE: I am not sure why exactly this is here. However, it seems that LoadPlan0InVignet should
            // ALWAYS fail the first time it is called because that erroneously sets PLAN0BIT = PLAN0 at the end, which
            // leaks away everything that was loaded into PLAN0, *UNLESS* they were the same buffer to begin with.
            // However, that will only be the case from the second time LoadPlan0InVignet is called.
            // Also, every time allocate_PLAN0() is called, the problem reappears, because then PLAN0 no longer equals
            // PLAN0BIT.

            // So, my guess is that they put this here as a workaround for the PLAN0BIT = PLAN0 bug, which they never
            // found the root cause for.

            // --> After the bug is fixed it should be safe to delete this?
            LoadPlan0InVignet(Plan0NumPcx[0]);
        }

        free(mem);

        //do_some_drm_check();

    }
}

//45688
void load_level(mem_t* mem_level, i32 world_id, const char* filename) {

    // NOTE: function signature changed: world_id and filename parameters added to allow more flexibility.

    static const char* world_names[] = {"JUNGLE", "MUSIC", "MOUNTAIN", "IMAGE", "CAVE", "CAKE"};
    if (!(world_id > 0 && world_id <= COUNT(world_names))) {
        return;
    }

    char full_filename[64];
    snprintf(full_filename, 64, "PCMAP" PATH_SEP "%s" PATH_SEP "%s", world_names[world_id - 1], filename);

    mem_t* mem = read_entire_file(full_filename, true);
    if (mem) {
        // Header
        i32 event_block_offset;
        i32 texture_block_offset;
        mem_read(&event_block_offset, mem, 4);
        mem_read(&texture_block_offset, mem, 4);
        mem_read(&mp.width, mem, 2);
        mem_read(&mp.height, mem, 2);
        mem_read(rvb, mem, sizeof(rvb));
        mem_read(&last_plan1_palette, mem, 1);
        current_rvb = rvb[last_plan1_palette];

        // Map
        mp.length = mp.width * mp.height;
        mp.map = (map_tile_t*) block_malloc(mem_level, mp.length * sizeof(map_tile_t));
        mem_read(mp.map, mem, mp.length * sizeof(map_tile_t));

        u8 background_image_static;
        u8 background_image_scrolling;
        i32 ScrollDiffSprites_value = 0;
        mem_read(&background_image_static, mem, 1);
        mem_read(&background_image_scrolling, mem, 1);
        mem_read(&ScrollDiffSprites_value, mem, 4);

        if (ScrollDiffSprites_value == -1) {
            ScrollDiffSprites = NULL;
        } else {
            ScrollDiffSprites = wldobj + ScrollDiffSprites_value;
            if (GameModeVideo == 0 && ScrollDiffOn) {
                MaskScrollDiffSprites(mem_level); //TODO
            }
        }

        if (GameModeVideo == 1 || !ScrollDiffOn) {
            no_fnd = background_image_static;
        } else if (GameModeVideo == 0 && ScrollDiffOn) {
            no_fnd = background_image_scrolling;
        }

        if (GameModeVideo == 1) {
            // X Mode Textures
            u32 x_texture_count;
            u32 x_unk_table_size;
            u8 x_texture_checksum;
            mem_read(&x_texture_count, mem, 4);
            mem_read(&x_unk_table_size, mem, 4);
            if (x_texture_count != 0) {
                gros_patai_block = (x_texture_t*) block_malloc(mem_level, x_texture_count * sizeof(x_texture_t));
                mem_read(gros_patai_block, mem, x_texture_count * sizeof(x_texture_t));
            } else {
                gros_patai_block = NULL;
            }

            mem_read(&x_texture_checksum, mem, 1);
            u8* pos = (u8*) gros_patai_block;
            for (i32 i = 0; i < x_texture_count * sizeof(x_texture_t); ++i) {
                x_texture_checksum -= *pos;
                *pos ^= 0x7D;
                ++pos;
            }
            if (x_texture_checksum != 0) {
                printf("%s : File integrity fault.\n", full_filename);
                fatal_error();
            }

            mem_read(gros_patai_src, mem, sizeof(gros_patai_src));

            if (x_unk_table_size != 0) {
                blocks_code = (u8*) block_malloc(mem_level, x_unk_table_size);
                mem_read(blocks_code, mem, x_unk_table_size);
            } else {
                blocks_code = NULL;
            }
            u8 x_unk_table_checksum;
            mem_read(&x_unk_table_checksum, mem, 1);
            pos = blocks_code;
            for (i32 i = 0; i < x_unk_table_size; ++i) {
                x_unk_table_checksum -= *pos;
                *pos ^= 0xF3;
                ++pos;
            }
            if (x_unk_table_checksum != 0) {
                printf("%s : File integrity fault.\n", full_filename);
                fatal_error();
            }

            mem_read(block_add, mem, sizeof(block_add));
            // NOTE: the game uses a direct pointer here, but we can't do that on a 64-bit system
            // so we're using an offset instead (need to keep this in mind when we want to use it)
            // Also we need to check for the value -1
#if 0
            for (i32 i = 0; i < 1200; ++i) {
                i32 value = block_add[i];
                if (value == -1) {
                    block_add[i] = 0;
                } else {

                    block_add[i] = blocks_code + value;
                }
            }
#endif
            mem_seek(mem, event_block_offset);
        } else if (GameModeVideo == 0) {
            // Normal Mode Textures

            mem_seek(mem, texture_block_offset);

            u32 texture_data_size;
            u8 texture_data_checksum;
            // offset table for the textures, based from the start of the tile texture arrays
            mem_read(block_add, mem, sizeof(block_add));

            mem_read(&nb_total_blocks, mem, 4);
            mem_read(&nb_blocks_plein, mem, 4); // opaque blocks
            mem_read(&texture_data_size, mem, 4);
            MAP_BLOCKS = (u8*) block_malloc(mem_level, texture_data_size);
            mem_read(MAP_BLOCKS, mem, texture_data_size);
            mem_read(&texture_data_checksum, mem, 1);
            u8* pos = MAP_BLOCKS;
            for (i32 i = 0; i < texture_data_size; ++i) {
                texture_data_checksum -= *pos;
                *pos ^= 0xFF;
                ++pos;
            }
            if (texture_data_checksum != 0) {
                printf("%s : File integrity fault (Block Normal).\n", full_filename);
                fatal_error();
            }
            construct_MAP(mem_level, &BIG_MAP, MAP_BLOCKS);
        } else {
            fatal_error();
        }

        // Events
        mem_read(&level.nb_objects, mem, 2);
        level.objects = (obj_t*)block_malloc(mem_level, sizeof(obj_t) * level.nb_objects);
        level_obj.field_0 = (i16*)block_malloc(mem_level, 2 * level.nb_objects);
        level_alw.field_0 = (i16*)block_malloc(mem_level, 2 * level.nb_objects);
        memset(level.objects, 0, sizeof(obj_t) * level.nb_objects);

        link_init = (i16*)block_malloc(mem_level, 2 * level.nb_objects);
        mem_read(link_init, mem, 2 * level.nb_objects);

        // We can't directly read the obj_t because the pointer size is not 4 bytes on all systems.
        // So we read everything field by field
        // TODO: make it work on big-endian systems
        i32 cursor = mem->cursor;
        for (i32 i = 0; i < level.nb_objects; ++i) {
            obj_t* obj = level.objects + i;

            // The wldobj index is stored only in the lower 2 bytes of the pointer values for sprites, animations, img_buffer and eta
            i16 sprites_value;
            i16 animations_value;
            i16 img_buffer_value;
            i16 eta_value;
            mem_read(&sprites_value, mem, 2);
            mem->cursor += 2;
            mem_read(&animations_value, mem, 2);
            mem->cursor += 2;
            mem_read(&img_buffer_value, mem, 2);
            mem->cursor += 2;
            mem_read(&eta_value, mem, 2);
            mem->cursor += 2;

            mem_read(&obj->cmds, mem, 4);
            mem_read(&obj->cmd_labels, mem, 4);
            mem_read(&obj->cmd_contexts, mem, 4);
            mem_read(&obj->field_1C, mem, 4);
            mem_read(&obj->link_has_gendoor, mem, 4);
            mem_read(&obj->is_active, mem, 4);
            mem_read(&obj->x, mem, 4);
            mem_read(&obj->y, mem, 4);
            mem_read(&obj->active_flag, mem, 4);
            mem_read(&obj->obj_index, mem, 2);
            mem_read(&obj->screen_x, mem, 2);
            mem_read(&obj->screen_y, mem, 2);
            mem_read(&obj->field_3A, mem, 2);
            mem_read(&obj->init_x, mem, 2);
            mem_read(&obj->init_y, mem, 2);
            mem_read(&obj->speed_x, mem, 2);
            mem_read(&obj->speed_y, mem, 2);
            mem_read(&obj->nb_sprites, mem, 2);
            mem_read(&obj->cmd_offset, mem, 2);
            mem_read(&obj->nb_cmd, mem, 2);
            mem_read(&obj->cmd_arg_2, mem, 2);
            mem_read(&obj->follow_y, mem, 2);
            mem_read(&obj->follow_x, mem, 2);
            mem_read(&obj->cmd_arg_1, mem, 2);
            mem_read(&obj->phase, mem, 2);
            mem_read(&obj->rayman_dist, mem, 2);
            mem_read(&obj->iframes_timer, mem, 2);
            mem_read(&obj->test_block_index, mem, 2);
            mem_read(&obj->scale, mem, 2);
            mem_read(&obj->zdc, mem, 2);
            mem_read(&obj->active_timer, mem, 2);
            mem_read(&obj->type, mem, 2);
            mem_read(&obj->coll_btype, mem, 5);
            mem_read(&obj->field_67, mem, 1);
            mem_read(&obj->offset_bx, mem, 1);
            mem_read(&obj->offset_by, mem, 1);
            mem_read(&obj->anim_index, mem, 1);
            mem_read(&obj->anim_frame, mem, 1);
            mem_read(&obj->sub_etat, mem, 1);
            mem_read(&obj->main_etat, mem, 1);
            mem_read(&obj->init_sub_etat, mem, 1);
            mem_read(&obj->init_etat, mem, 1);
            mem_read(&obj->cmd, mem, 1);
            mem_read(&obj->gravity_value_1, mem, 1);
            mem_read(&obj->gravity_value_2, mem, 1);
            mem_read(&obj->change_anim_mode, mem, 1);
            mem_read(&obj->offset_hy, mem, 1);
            mem_read(&obj->follow_sprite, mem, 1);
            mem_read(&obj->hit_points, mem, 1);
            mem_read(&obj->init_hit_points, mem, 1);
            mem_read(&obj->init_flag, mem, 1);
            mem_read(&obj->hit_sprite, mem, 1);
            mem_read(&obj->detect_zone, mem, 1);
            mem_read(&obj->detect_zone_flag, mem, 1);
            mem_read(&obj->cmd_context_depth, mem, 1);
            mem_read(&obj->configuration, mem, 1);
            mem_read(&obj->display_prio, mem, 1);
            mem_read(&obj->timer, mem, 1);
            mem_read(&obj->anim_count, mem, 1);

            // The exact bit field layout isn't guaranteed by the C standard (make extra effort to deserialize properly).
            u8 flags_lo;
            u8 flags_hi;
            mem_read(&flags_lo, mem, 1);
            mem_read(&flags_hi, mem, 1);
            obj_flags_t flags = {0};
            flags.flag_1 = flags_lo & 1u;
            flags.command_test = flags_lo & 2u;
            flags.alive = flags_lo & 4u;
            flags.flip_x = flags_lo & 8u;
            flags.read_commands = flags_lo & 0x10u;
            flags.follow_enabled = flags_lo & 0x20u;
            flags.flag_0x40 = flags_lo & 0x40u;
            flags.anim_changed = flags_lo & 0x80u;
            flags.flag_0x100 = flags_hi & 1u;
            obj->flags = flags;

            mem_read(&obj->field_83, mem, 1);

            // correct pointers
            obj->sprites = wldobj[sprites_value].sprites;
            obj->animations = wldobj[animations_value].animations;
            obj->img_buffer = wldobj[img_buffer_value].img_buffer;
            obj->eta = loaded_eta[eta_value];
        }

        for (i32 i = 0; i < level.nb_objects; ++i) {
            obj_t* obj = level.objects + i;

            u16 cmd_count = 0;
            u16 cmd_label_count = 0;
            mem_read(&cmd_count, mem, 2);
            mem_read(&cmd_label_count, mem, 2);

            if (cmd_count > 0) {
                obj->cmds = block_malloc(mem_level, cmd_count);
                mem_read(obj->cmds, mem, cmd_count);
            } else {
                obj->cmds = NULL;
            }

            if (cmd_label_count > 0) {
                obj->cmd_labels = (i16*)block_malloc(mem_level, 2 * cmd_label_count);
                mem_read(obj->cmd_labels, mem, 2 * cmd_label_count);
            } else {
                obj->cmd_labels = NULL;
            }

        }

        free(mem);
    } else {
        printf("Can not open file %s (load_level).\n", full_filename);
        fatal_error();
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
            if (buffer) {
                // Allocate image memory using a memory pool (will be deallocated on block_free())
                image.memory = (u8*)block_malloc(buffer, decompressed_byte_count);
            } else {
                // Allocate image memory using malloc()
                // NOTE: if used in this mode we need to remember to manually free the memory!
                image.memory = (u8*)malloc(decompressed_byte_count);
            }
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
    if (FondAutorise != 0) {
        if (FondAutorise == 2) {
            // In low memory mode the background gets read into DrawBufferNormal / EffetBufferNormal as scratch space.
            printf("Error: low memory mode not implemented.\n");
            fatal_error();
        }
        ASSERT(FondAutorise == 1);
        stop_cd();
        i16 width = 0;
        i16 height = 0;
        image_t image = LoadPcxInVignet(NULL, resource_id, &width, &height, NULL /* the palette is not used */);
        ASSERT(image.is_valid);

        u8* dest_buffer = PLAN0; // Notice that we're loading into PLAN0 here, and not PLAN0BIT.

        if (GameModeVideo != 0) {
            memcpy(dest_buffer, image.memory, width * height);
        } else {
            // We need to blit the image data into PLAN0 which has been allocated to be twice as wide.
            // (Maybe for the differential scrolling? Need to check later.)
            u8* source_row = image.memory;
            u8* dest_row = dest_buffer;
            for (i32 y = 0; y < height; ++y) {
                memcpy(dest_row, source_row, width);
                source_row += width;
                dest_row += 2 * width;
            }
        }
        free(image.memory);

        if (GameModeVideo != 0) {
            PLAN0FND_to_bits_planes(PLAN0, width, height);
        } else {
            // NOTE: this is a bug!!
            // We just loaded everything into PLAN0, but then here proceed to leak away everything by overwriting the pointer.
            // However: if you call LoadPlan0InVignet() multiple times, it will work the second time because the pointers
            // will be equal so this statement has no effect.
            // My guess is that this is the reason that LoadPlan0InVignet() is called from load_world(), even though
            // there seems to be no good reason to do that there. Because LoadPlan0InVignet() has already been called once
            // once we reach LOAD_FND(), the pointers will be equal so PLAN0 doesn't get overwritten.
            PLAN0 = PLAN0BIT;

            plan0_width = 2 * width;
            plan0_height = height;
        }
    }
}

//47BB0
u8* allocate_PLAN0(mem_t* mem_world, i32 width, i32 height) {
    u8* buffer1;
    u8* buffer2;
    if (FondAutorise == 2) {
        buffer1 = DrawBufferNormal;
        buffer2 = EffetBufferNormal;
    } else {
        // NOTE: In Normal mode, only one of these buffers is used.
        // Actually, one of the buffers is leaked in Normal mode, because in LoadPlan0InVignet we're setting PLAN0 = PLAN0BIT
        // (I think that is a bug in the game.)
        buffer1 = block_malloc(mem_world, width * height);
        buffer2 = block_malloc(mem_world, 2 * width * height + 8 * width);
    }
    PLAN0BIT = buffer2;
    return buffer1;
}

//47C10
void PLAN0FND_to_bits_planes(u8* buffer, i16 width, i16 height) {
    // NOTE: this procedure is only used for EGA (X) mode.
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
    plan2_height = height;
    plan2bit_length = width * height;
    plan2bit_nb_bytes = width >> 2;
    PLAN2BIT = PLAN2;
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


