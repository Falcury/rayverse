


//46078
void load_big_ray(mem_t* buffer) {
	// TODO: allocate from a memory pool (main_mem_tmp), so that it can be freed at once later
	FILE* fp = open_data_file("PCMAP" PATH_SEP "BRAY.DAT", true);
	if (fp) {
		obj_t* sprite_group = wldobj + nb_des;
		u32 sprite_group_size = 0;
		fread(&sprite_group_size, 4, 1, fp);
		sprite_group->xpos = (i32)sprite_group_size; // why is this saved in the xpos field??
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

//6B1EC
void init_loader_anim() {
	bigray.offset_bx = 200;
	bigray.offset_by = 240;
	bigray.screen_y = -72;
	bigray.screen_x = 120;
	bigray.main_etat = 1;
	bigray.sub_etat = 0;
	bigray.command = 0;
	bigray.flags &= ~obj_flags_8_flipped;
}

//6B258
void DO_LOADER_ANIM() {
	eta_t* eta = get_eta(&bigray);
	bigray.xspeed = 0;
	if ((eta->anim_speed & 15) != 0 && (horloge[eta->anim_speed] == 0)) {
		SET_X_SPEED(&bigray);
	}
	bigray.screen_x += bigray.xspeed;
	if (PROC_EXIT == 1) {
		PROC_EXIT = 0;
		bigray.command = 5;
		set_main_and_sub_etat(&bigray, 0, 2);
		bigray.timer = 2;
		bigray.flags &= ~obj_flags_8_flipped;
		bigray.screen_x = 160 - bigray.offset_bx - 16;
	}

	if (bigray.command <= 5) {
		switch(bigray.command) {
			default: break;
			case 0: {
				if (bigray.screen_x + bigray.offset_bx < -100) {
					++bigray.command;
					set_main_and_sub_etat(&bigray, 1, 1);
					bigray.screen_x = (-bigray.offset_bx) - 60;
					bigray.flags |= obj_flags_8_flipped;
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
			case 1: {
				if (bigray.screen_x + bigray.offset_bx > 400) {
					++bigray.command;
					set_main_and_sub_etat(&bigray, 1, 2);
					bigray.anim_frame = 0;
					bigray.screen_x = 350 - bigray.offset_bx;
					bigray.flags &= ~obj_flags_8_flipped;
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
			case 2: {
				if (bigray.main_etat == 0 && bigray.sub_etat == 0) {
					++bigray.command;
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
			case 3: {
				if (EOA(&bigray)) {
					++bigray.command;
					set_main_and_sub_etat(&bigray, 1, 3);
					bigray.screen_x = -60 - bigray.offset_bx;
					bigray.flags |= obj_flags_8_flipped;
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
			case 4: {
				if (bigray.screen_x + bigray.offset_bx > 400) {
					++bigray.command;
					set_main_and_sub_etat(&bigray, 0, 1);
					bigray.screen_x = 160 - bigray.offset_bx - 16;
					bigray.flags &= ~obj_flags_8_flipped;
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
			case 5: {
				if (bigray.main_etat == 0 && bigray.sub_etat == 2) {
					if (bigray.timer == 0) {
						PROC_EXIT = 1;
					} else {
						--bigray.timer;
					}
					DO_ANIM(&bigray);
				} else {
					DO_ANIM(&bigray);
				}
			} break;
		}
	}
}


i16 loader_anim_prg(u32 par_0) {
	//readinput();
	bool valid_button_pressed = false; //stub
	if (valid_button_pressed) {
		PROC_EXIT = true;
	}
	horloges(1);
	//sub_1CF94()
	clrscr();
	display2(&bigray);
	DO_LOADER_ANIM();
	if (PROC_EXIT && nb_fade == 0) {
		return 1;
	} else {
		return 0;
	}
}

//365E0
void LOAD_SAVE_SCREEN() {
	// (implementation changed a bit from the original, maybe revise later)
	image_t background = load_vignet_pcx(12);
	copy_full_image_to_background_buffer(&background);
	clrscr();
	fade_source_palette = *background.pal;
	destroy_image(&background);
}

//464BC
void LOAD_ALL_FIX() {
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

			i32 eta_ptr_value = 0;
			mem_read(&eta_ptr_value, mem, 4);
			if (eta_ptr_value == -1) {
				obj->eta = NULL;
			} else {
				obj->eta = loaded_eta[des_index];
			}

			mem_read(&RaymanExeSize, mem, 4);
			mem_read(&RaymanExeCheckSum1, mem, 4);

			u32 sprite_group_size = 0;
			mem_read(&sprite_group_size, mem, 4);
			obj->xpos = (i32)sprite_group_size; // why is this saved in the xpos field??
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


//35CC4
void DO_GROS_RAYMAN() {
	init_memory(&main_mem_tmp, 0x41A00);
	load_big_ray(main_mem_tmp);
	LOAD_SAVE_SCREEN();
	INIT_FADE_IN();
	play_cd_track(19); // Menu music - "World Map"
	init_loader_anim();
	DrawSpriteNormalEtX = DrawSpriteNormal256;
	DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal256;
	SYNCHRO_LOOP(loader_anim_prg);
	DrawSpriteNormalEtX = DrawSpriteNormal;
	DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
	free(main_mem_tmp);
	LOAD_ALL_FIX();
	fade_out(2, &fade_source_palette);
	//restore_palette();
}

