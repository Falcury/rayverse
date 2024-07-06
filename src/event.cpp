

// sub_1EB80
void decrease_hitp_from_fist(event_t* target) {
	if (fist_hit_strength > target->hitp) {
		target->hitp = 0;
	} else {
		target->hitp -= fist_hit_strength;
	}
}

// sub_7B628
void destroy_event_with_blink(event_t* event) {
	// stub
}

// sub_1DFB8
void turn_to_face_rayman(event_t* event) {
	if (ray.xpos + ray.offset_bx > event->xpos + event->offset_bx) {
		event->flags |= event_flags_8_dir_right;
	} else {
		event->flags &= ~event_flags_8_dir_right;
	}
}

// sub_25820
void interact_with_rayman(event_t* event) {
	// stub (?)
}


// Generic event procs

// sub_2F658
void update_default(event_t* event) {
	sub_2F63C(event);
	u8 cmd = event->command;
	u8 etat = event->etat;
	if (cmd == cmd_0_left || cmd == cmd_1_right) {
		if (cmd == cmd_0_left) {
			event->flags &= ~8;
		} else {
			event->flags |= 8;
		}
		if (etat == 1) {
			// stub
			update_x_momentum(event);
			interact_with_rayman(event);
		} else if (etat == 2) {
			update_x_momentum(event);
		} else if (etat == 0) {
			//sub_2F488()
		}
	} else if (cmd == cmd_2_up) {
		do_cmd_up(event);
	} else if (cmd == cmd_3_down || cmd == cmd_4) {
		do_cmd_3_4(event);
	} else if (cmd == cmd_20_speed) {
		event->xspeed = event->iframes_timer;
		event->yspeed = event->command_par2;
	}
}

void obj_not_hit_by_fist(event_t* event, u16 unk) {
	// nop
}

void touched_default(event_t*event) {
	// nop
}

// sub_2ED74
void obj_generic_coll(event_t* event) {
	if ((obj_type_flags[event->type] & obj_type_flags_bit_20) && get_eta(event)->interaction_flags & 0x20) {
		if (!(ray.etat == 3 && ray.subetat == 32)) {
			i16 command_par3 = ray.iframes_timer;
			if (command_par3 >= 60 || command_par3 == -1) {
				ray_hit(0, event);
				ray.iframes_timer = command_par3;
			} else {
				ray_hit(1, event);
				if (!(word_CF868 == 3 || word_CF868 == 4)) {
					ray.iframes_timer = 60;
				}
			}
		}
	}
}

// sub_612F0
void obj_generic_flip_direction(event_t* event) {
	event->flags ^= event_flags_8_dir_right;
}


// Event 0: Livingstone

// sub_62B54
void punched_livingstone(event_t* event, u16 unk) {
	decrease_hitp_from_fist(event);
	if (rayman_fist->xspeed > 0) {
		script_goto_label(event, 3, 1);
	} else if (rayman_fist->xspeed < 0) {
		script_goto_label(event, 2, 1);
	}
	event->xspeed = 0;
	event->ypos -= 2;
	if (event->hitp != 0) {
		event->yspeed = -4;
		set_event_state(event, 2, (get_eta(event)->interaction_flags & 0x40) ? 10 : 2);
		play_sound(28, event->event_index);
	} else {
		event->yspeed = -8;
		set_event_state(event, 0, (get_eta(event)->interaction_flags & 0x40) ? 6 : 3);
	}

}

void touched_livingstone(event_t* event) {
	if ((event->flags & event_flags_8_dir_right) != (ray.flags & event_flags_8_dir_right)) {
		if ((event->etat == 1 && event->subetat == 0) || (event->etat == 0 && event->subetat == 0)) {
			if (ray.etat == 0 && ray.subetat == 18 /* stick out tongue */) {
				event->xspeed = 0;
				event->yspeed = 0;
				set_event_state(event, 0, 2);
				script_goto_label(event, (event->flags & event_flags_8_dir_right) ? 8 : 7, 1);
			}
		}
	}
}

// Event 1: Platform

// sub_643BC
void punched_platform(event_t* event, u16 unk) {
	if (event->hit_sprite == unk) {
		if (rayman_fist->xspeed > 0) {
			set_subetat(event, 25);
		} else if (rayman_fist->xspeed < 0) {
			set_subetat(event, 26);
		}
	}
}


// Event 2: Energy point

// sub_61EA0
void touched_energypoint(event_t* event) {
	destroy_event_with_blink(event);
	ray.hitp += event->hitp;
	if (ray.hitp > ray_max_hitp) {
		ray.hitp = ray_max_hitp;
	}
	event->flags &= ~event_flags_4_switched_on;
	play_sound(8, event->event_index);

}


// Event 3: Electoon

// sub_291B4
void update_electoon(event_t* event) {
	// stub
}

// sub_2937C
void touched_electoon(event_t* event) {
	if (get_eta(event)->interaction_flags & 1) {
		script_goto_label(event, 0, 1);
	}
}

// Event 5: Bonus magician

// sub_6154B
void update_magician(event_t* event) {
	// stub
}

// Event 6: Yin with pin
// Event 7: Bouncing platform

// sub_3167C
void punched_bouncing_platform(event_t* event, u16 unk) {
	// stub
}

// Event 9: Small livingstone
// Event 165: Small livingstone (aggressive/evil)

// sub_629D8
void punched_small_livingstone(event_t* event, u16 unk) {
	if (unk == 255 || !(get_eta(event)->interaction_flags & 1)) {
		decrease_hitp_from_fist(event);
		if (event->hitp == 0) {
			set_event_state(event, 0, 3);
			script_goto_label(event, 2, 1);
			event->ypos -= 2;
			event->flags &= ~event_flags_0x10;
		} else {

		}
	}
}

// sub_62AD4
void touched_small_livingstone(event_t* event) {
	if (event->etat == 1 && event->subetat == 11) {
		set_subetat(event, 14);
	} else if ((event->etat == 0 && event->subetat == 0) || (event->etat == 1 && event->subetat == 0)) {
		set_event_state(event, 1, 11);
		event->flags &= ~event_flags_0x10;
		script_goto_label(event, facing_right(event) ? 3 : 2, 1);
	}
}

// Event 10: Piranha

// sub_7AD20
void create_splash(event_t* event) {
	// stub
}

// sub_673B4
bool check_piranha_respawn(event_t* piranha) {
	// stub
	bool result = true;
	for (i32 i = 0; i < event_count; ++i) {
		event_t* event = event_bodies + i;
		if (event->type == event_10_piranha && event->spawn_x == piranha->spawn_x && !event->is_active && event->timer >= 100) {
			result = false;
			break;
		}
	}
	return result;
}

// sub_674C8
void update_piranha(event_t* event) {
	update_default(event);
	bool respawn = false;
	if (event->etat == 0) {
		i32 y_offset_by = event->ypos + event->offset_by;
		if (y_offset_by + 20 < level_height + 200) {
			++event->timer;
		}
		if (event->subetat == 3) {
			event->yspeed = 0;
		} else if (event->subetat == 9) {
			event->yspeed = 6;
			if ((event->ypos + event->offset_hy > level_height + 200) || (y_offset_by + 14 > level_height_tiles * 16)) {
				if (y_offset_by + 14 > level_height_tiles * 16) {
					create_splash(event);
				}
				event->ypos = level_height + 200;
				event->is_active = 0;
				event->flags &= ~event_flags_4_switched_on;
				event->timer = 0;
				respawn = check_piranha_respawn(event);
			}
		} else {
			if (y_offset_by < level_height) { //?
				event->ypos = level_height + 200;
				event->is_active = 0;
				event->flags &= ~event_flags_4_switched_on;
				respawn = check_piranha_respawn(event);
			}
		}

		if (event->timer == 100) {
			respawn = check_piranha_respawn(event);
		}
		if (respawn) {
			// stub
		}
	}
}

void sub_67410(event_t* event) {
	// stub
}

// sub_676EC
void punched_piranha(event_t* event, u16 unk) {
	decrease_hitp_from_fist(event);
	if (event->hitp != 0) {
		event->xspeed = 0;
		event->yspeed = 0;
		set_event_state(event, 0, 1);
	} else {
		set_event_state(event, 0, 3);
	}
	sub_67410(event);
	event->command = facing_right(event) ? cmd_1_right : cmd_0_left;
}

// Event 12: Hunter 1
// Event 14: Hunter 2

// sub_628C0
void punched_hunter(event_t* event, u16 unk) {
	if (get_eta(event)->interaction_flags & 1) {
		decrease_hitp_from_fist(event);
		if (event->hitp != 0) {
			event->xspeed = 0;
			event->yspeed = 0;
			set_event_state(event, 0, 1); // startled
		} else {
			set_event_state(event, 0, 3); // dead
		}
		event->command = facing_right(event) ? cmd_0_left : cmd_1_right;
	}
}

// sub_62928
void touched_hunter(event_t* event) {
	if (event->etat == 0 && (event->subetat == 0 || event->subetat == 2)) {
		event->xspeed = 0;
		event->yspeed = 0;
		set_event_state(event, 0, 2);
		event->flags &= ~event_flags_0x10;
		turn_to_face_rayman(event);
		event->command = facing_right(event) ? cmd_1_right : cmd_0_left;
	}
}

// Event 20: Tentacle

void update_distance_bottom_right(event_t* event) {
	event->y_from_bottom = event->ypos - level_height;
	event->x_from_right = event->xpos - level_width + 8;
}

// tentacle_spawn_enemy
void tentacle_spawn_enemy(event_t* tentacle, i16 which_enemy, i32 xspeed, i32 yspeed) {
	// stub
	for (i32 event_index = 0; event_index < event_count; ++event_index) {
		event_t* spawned = event_bodies + event_index;
		if (((which_enemy == 1 && spawned->type == event_0_livingstone) || (which_enemy == 2 && spawned->type == event_9_small_livingstone))
				&& spawned->spawn_x <= 0 && spawned->is_active == 0
		) {
			spawned->flags &= ~(event_flags_4_switched_on | event_flags_0x40);
			spawned->is_active = 1;
			spawned->flags |= event_flags_4_switched_on; // Note: this does not make sense, this bit was cleared just before!
			spawned->active_timer = 0;
			spawned->field_30 = 1;
			set_event_state(spawned, 2, 2); // thrown up in air
			spawned->xpos = tentacle->xpos + tentacle->offset_bx - spawned->offset_bx;
			spawned->ypos = tentacle->ypos - (spawned->offset_by / 4);
			turn_to_face_rayman(spawned);
			spawned->yspeed = yspeed;
			spawned->xspeed = facing_right(spawned) ? xspeed : -xspeed;
			update_distance_bottom_right(spawned);
			spawned->flags &= ~event_flags_0x10;
			return;
		}
	}
}

// sub_7B060
void sub_7B060(event_t* event) {
	// stub
	// something with a linked event 24?
}

// sub_62658
void update_tentacle(event_t* event) {
	if (event->command == cmd_0_left) {
		set_facing_left(event);
	} else if (event->command == cmd_1_right) {
		set_facing_right(event);
	}
	if (event->etat == 0 && event->subetat == 11) {
		if (event->anim_frame >= 5 && event->timer == 0) {
			++event->timer;
			++event->configuration; // alternate between 1=livingstone and 2=small livingstone
			if (event->configuration > 2) {
				event->configuration = 1;
			}
			if (event->hitp == 1) {
				tentacle_spawn_enemy(event, 2 /*small livingstone*/, 1, -2);
				tentacle_spawn_enemy(event, 1 /*livingstone*/      , 2, -1);
			} else {
				tentacle_spawn_enemy(event, event->configuration   , 1, -1);
			}
		} else {
			if (event->anim_frame > 2) {
				event->timer = 0;
			}
		}
	} else if (event->etat == 0 && event->subetat == 21) {
		if (event->anim_frame >= 5 && event->timer == 0) {
			// last hit: spawn 4 enemies
			++event->timer;
			i16 xspeed = -4;
			for (i32 i = 0; i < 4; ++i) {
				i16 which_enemy = (i % 2) + 1;
				i16 yspeed = -which_enemy;
				++xspeed;
				tentacle_spawn_enemy(event, which_enemy, xspeed, yspeed);
				if (xspeed == -2) xspeed = 3;
			}
		} else {
			if (event->anim_frame > 2) {
				event->timer = 0;
			}
		}
	} else if (event->etat == 0 && event->subetat == 3) {
		++event->timer;
		if (!(event->configuration == 0 || event->timer < -3)) {
			u8 saved_configuration = event->configuration;
			sub_7B060(event);
			event->configuration = saved_configuration;
		}
		event->xspeed = 0;
		event->yspeed = 0;
		if (event->anim_frame > 12) {
			event->anim_frame = 3;
		} else if (event->anim_frame <= 1){
			event->timer = 0;
		}
	} else if (event->etat == 1) {
		if (event->phase != 0) ++event->timer;
		if (event->timer >= -2) {
			script_goto_label(event, 2, 1);
		}
		update_x_momentum(event);
		interact_with_rayman(event);
	}
}

// sub_62854
void punched_tentacle(event_t* event) {

}

// sub_628B8
void touched_tentacle(event_t* event) {

}



typedef struct {
	void (*update)(event_t* event);
	void (*punched)(event_t* event, u16 unk);
	void (*coll)(event_t* event);
	void (*touched)(event_t* event);
	void (*flipped)(event_t* event);
} event_procs_t;

event_procs_t event_procs[] = {
		{update_default,  punched_livingstone,       obj_generic_coll, touched_livingstone,       obj_generic_flip_direction}, // 0: livingstone
		{update_default,  punched_platform,          obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 1: platform
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_energypoint,       obj_generic_flip_direction}, // 2: energy point
		{update_electoon, obj_not_hit_by_fist,       obj_generic_coll, touched_electoon,          obj_generic_flip_direction}, // 3: electoon
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 4: scenery
		{update_magician, obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 5: magician
		{update_default,  punched_bouncing_platform, obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 6: yin with pin
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 7: swinging spiky fruit
		{update_default,  punched_bouncing_platform, obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 8: plum 1
		{update_default,  punched_small_livingstone, obj_generic_coll, touched_small_livingstone, obj_generic_flip_direction}, // 9: small livingstone
		{update_piranha,  punched_piranha,           obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 10: piranha
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 11: hit effect
		{update_default,  punched_hunter,            obj_generic_coll, touched_hunter,            obj_generic_flip_direction}, // 12: hunter 1
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 13: hunter bullet
		{update_default,  punched_hunter,            obj_generic_coll, touched_hunter,            obj_generic_flip_direction}, // 14: hunter 2
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 15:
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 16: falling platform
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 17: rising platform
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 18:
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 19: splash
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 20: tentacle
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 21: photographer
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 22: moving platform
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 23: rayman
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 24:
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 25: ejecting platform
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 26: disappearing cloud
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 27: trampoline cloud
		{update_default,  obj_not_hit_by_fist,       obj_generic_coll, touched_default,           obj_generic_flip_direction}, // 28: blinking cloud
};

