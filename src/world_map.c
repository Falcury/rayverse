
//26A30
u8 get_casse_brique_active(void) {
    return casse_brique_active;
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
            mem_read(&cur_wldobj->xpos, mem, 4); // NOTE: this is read, but cur_wldobj->xpos is used instead

            if (sprite_is_always_used || ToutSpriteAutorise) {
                cur_wldobj->img_buffer = (u8*) block_malloc(mem_sprite, cur_wldobj->xpos);
                mem_read(cur_wldobj->img_buffer, mem, cur_wldobj->xpos);

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
                            anim->frames = (anim_frame_t*) block_malloc(main_mem_fix, anim->frame_count * sizeof(anim_frame_t));
                            mem_read(anim->frames, mem, anim->frame_count * sizeof(anim_frame_t));
                        }

                    }
                }
            } else {
                mem->cursor += cur_wldobj->xpos; // skip
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

//36394
void world_level(i32 world, char* filename) {
    snprintf(filename, 20, "RAY%d.WLD", world);
}

//36880
void DEPART_WORLD(void) {
    if (MusicCdActive) {
        stop_cd();
    }
    if (!NormalModeAutorise) {
        GameModeVideo = 1;
    }
    if (FondAutorise == 2) {
        P486 = 1;
    }
    if (num_world_choice == num_world) {
        if (get_casse_brique_active()) {
            num_level = 0;
            new_world = 1;
        } else {
            new_world = 0;
        }
        new_level = 1;
    } else {
        num_level = 0;
        new_world = 1;
        new_level = 1;
    }
    if (You_Win == 1 && num_world_choice == world_6_cake) {
        //DO_VICTOIRE();
    } else if (!fin_du_jeu && new_world != 0) {
        //START_WORLD_VIGNET();
        block_free(main_mem_world);
        block_free(main_mem_sprite);
        world_level(num_world_choice, world_filename);
        load_world(main_mem_world, main_mem_sprite);
    }
}

//69518
void INIT_PASTILLES_SAUVE(void) {
	char* text = (NBRE_SAVE != 0) ? language_txt[150] /*/save game/*/: language_txt[152]; /*/no save available/*/
	t_world_info[18].text = text;
	t_world_info[19].text = text;
	t_world_info[20].text = text;
	t_world_info[21].text = text;
	t_world_info[22].text = text;
	t_world_info[23].text = text;
}


//68BC0
void INIT_WORLD_INFO(void) {
	for (i32 i = 0; i < 24; ++i) {
		t_world_info[i].text = language_txt[i];
	}
	num_world = 0;
	num_level = 0;
	new_world = 1;
	new_level = 1;
	world_index = 0;
	xwldmapsave = 0;
	ywldmapsave = 0;
	dir_on_wldmap = 1;
	You_Win = 0;
	fin_du_jeu = 0;
	INIT_PASTILLES_SAUVE();
	MENU_RETURN = 0;
}
