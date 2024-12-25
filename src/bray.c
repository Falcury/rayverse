



//6B1EC
void init_loader_anim(void) {
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
void DO_LOADER_ANIM(void) {
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









