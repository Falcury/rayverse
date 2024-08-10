


//46078
void load_big_ray() {
	FILE* fp = open_data_file("PCMAP\\BRAY.DAT");
	if (fp) {
		obj_t* sprite_group = global_sprite_groups + num_sprite_groups;
		u32 sprite_group_size = 0;
		fread(&sprite_group_size, 4, 1, fp);
		sprite_group->xpos = (i32)sprite_group_size; // why is this saved in the xpos field??
		u8* image_atlas = (u8*)malloc(sprite_group_size);
		fread(image_atlas, sprite_group_size, 1, fp);
		for (u32 i = 0; i < sprite_group_size; ++i) {
			image_atlas[i] ^= 0x8F;
		}
		sprite_group->image_atlas = image_atlas;

		fread(&sprite_group->sprites_count, 2, 1, fp);
		ASSERT(sizeof(sprite_t) == 12);
		u8* sprite_buffer = (u8*)malloc(sprite_group->sprites_count * sizeof(sprite_t));
		fread(sprite_buffer, sizeof(sprite_t), sprite_group->sprites_count, fp);
		sprite_group->sprites = (sprite_t*)sprite_buffer;

		fread(&sprite_group->anim_count, 1, 1, fp);
		sprite_group->animations = (anim_t*)calloc(sprite_group->anim_count, sizeof(anim_t));
		for (i32 i = 0; i < sprite_group->anim_count; ++i) {
			anim_t* anim_desc = sprite_group->animations + i;
			fread(&anim_desc->layers_per_frame, 2, 1, fp);
			fread(&anim_desc->frame_count, 2, 1, fp);
			i32 field_4 = 0;
			fread(&field_4, 4, 1, fp); // ptr field, but might be -1
//			anim_desc->anim_layers_or_frames = (u8*)field_4;
			u16 layer_table_size = 0;
			fread(&layer_table_size, 2, 1, fp);
			anim_desc->layers = (anim_layer_t*)malloc(layer_table_size);
			fread(anim_desc->layers, 1, layer_table_size, fp);
			if (field_4 != -1) {
				anim_desc->frames = (anim_frame_t*)malloc(anim_desc->frame_count * 4);
				fread(anim_desc->frames, 4, anim_desc->frame_count, fp);
			}
		}
		fread(&num_eta_blocks, 1, 1, fp);
		ASSERT(num_eta_blocks < COUNT(global_eta_block));
		ASSERT(sizeof(eta_t) == 8);
		for (i32 i = 0; i < num_eta_blocks; ++i) {
			u8 num_eta_groups = 0;
			fread(&num_eta_groups, 1, 1, fp);
			global_eta_block[i] = (eta_t**)calloc(num_eta_groups, sizeof(eta_t*));
			for (i32 j = 0; j < num_eta_groups; ++j) {
				u8 num_eta_entries = 0;
				fread(&num_eta_entries, 1, 1, fp);
				global_eta_block[i][j] = (eta_t*)calloc(num_eta_entries, sizeof(eta_t));
				fread(global_eta_block[i][j], sizeof(eta_t), num_eta_entries, fp);
			}

		}
		sprite_group->ETA = global_eta_block[0];
		bigray = *sprite_group; // instantiates the bigray object?
		++num_sprite_groups;

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
	bigray.etat = 1;
	bigray.subetat = 0;
	bigray.command = 0;
	bigray.flags &= ~obj_flags_8_flipped;
}

//6B258
void DO_LOADER_ANIM() {
	eta_t* eta = get_eta(&bigray);
	bigray.xspeed = 0;
	if ((eta->anim_speed & 15) != 0 && (horloge[eta->anim_speed] == 0)) {
		set_x_speed(&bigray);
	}
	bigray.screen_x += bigray.xspeed;
	if (proc_exit == 1) {
		proc_exit = 0;
		bigray.command = 5;
		set_obj_state(&bigray, 0, 2);
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
					set_obj_state(&bigray, 1, 1);
					bigray.screen_x = (-bigray.offset_bx) - 60;
					bigray.flags |= obj_flags_8_flipped;
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
			case 1: {
				if (bigray.screen_x + bigray.offset_bx > 400) {
					++bigray.command;
					set_obj_state(&bigray, 1, 2);
					bigray.anim_frame = 0;
					bigray.screen_x = 350 - bigray.offset_bx;
					bigray.flags &= ~obj_flags_8_flipped;
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
			case 2: {
				if (bigray.etat == 0 && bigray.subetat == 0) {
					++bigray.command;
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
			case 3: {
				if (end_of_animation(&bigray)) {
					++bigray.command;
					set_obj_state(&bigray, 1, 3);
					bigray.screen_x = -60 - bigray.offset_bx;
					bigray.flags |= obj_flags_8_flipped;
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
			case 4: {
				if (bigray.screen_x + bigray.offset_bx > 400) {
					++bigray.command;
					set_obj_state(&bigray, 0, 1);
					bigray.screen_x = 160 - bigray.offset_bx - 16;
					bigray.flags &= ~obj_flags_8_flipped;
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
			case 5: {
				if (bigray.etat == 0 && bigray.subetat == 2) {
					if (bigray.timer == 0) {
						proc_exit = 1;
					} else {
						--bigray.timer;
					}
					do_anim(&bigray);
				} else {
					do_anim(&bigray);
				}
			} break;
		}
	}
}


i16 loader_anim_prg(u32 par_0) {
	//readinput();
	bool valid_button_pressed = false; //stub
	if (valid_button_pressed) {
		proc_exit = true;
	}
	horloges(1);
	//sub_1CF94()
	clrscr();
	display2(&bigray);
	DO_LOADER_ANIM();
	if (proc_exit && nb_fade == 0) {
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
	//stub
}

//35CC4
void DO_GROS_RAYMAN() {
	load_big_ray();
	LOAD_SAVE_SCREEN();
	INIT_FADE_IN();
	play_cd_track(19); // Menu music - "World Map"
	init_loader_anim();
	curr_obj_draw_proc = draw_simple;
	curr_obj_draw_proc_flipped = draw_simple_flipped;
	synchro_loop(loader_anim_prg);
	curr_obj_draw_proc = sub_16A24;
	curr_obj_draw_proc_flipped = sub_16B08;
	LOAD_ALL_FIX();
	fade_out(2, &fade_source_palette);
	//restore_palette();
//	wait_frames(600);
}

