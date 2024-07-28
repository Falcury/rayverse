

// sub_1EB80
void decrease_hitp_from_fist(obj_t* target) {
	if (fist_hit_strength > target->hitp) {
		target->hitp = 0;
	} else {
		target->hitp -= fist_hit_strength;
	}
}

// sub_7B628
void destroy_obj_with_blink(obj_t* obj) {
	// stub
}

// sub_1DFB8
void turn_to_face_rayman(obj_t* obj) {
	if (ray.xpos + ray.offset_bx > obj->xpos + obj->offset_bx) {
		obj->flags |= obj_flags_8_flipped;
	} else {
		obj->flags &= ~obj_flags_8_flipped;
	}
}

// sub_25820
void interact_with_rayman(obj_t* obj) {
	// stub (?)
}


// Generic obj procs

// sub_2F658
void update_default(obj_t* obj) {
	sub_2F63C(obj);
	u8 cmd = obj->command;
	u8 etat = obj->etat;
	if (cmd == cmd_0_left || cmd == cmd_1_right) {
		if (cmd == cmd_0_left) {
			obj->flags &= ~obj_flags_8_flipped;
		} else {
			obj->flags |= obj_flags_8_flipped;
		}
		if (etat == 1) {
			// stub
			update_x_momentum(obj);
			interact_with_rayman(obj);
		} else if (etat == 2) {
			update_x_momentum(obj);
		} else if (etat == 0) {
			//sub_2F488()
		}
	} else if (cmd == cmd_2_up) {
		do_cmd_up(obj);
	} else if (cmd == cmd_3_down || cmd == cmd_4) {
		do_cmd_3_4(obj);
	} else if (cmd == cmd_20_speed) {
		obj->xspeed = obj->iframes_timer;
		obj->yspeed = obj->command_par2;
	}
}

void obj_not_hit_by_fist(obj_t* obj, u16 unk) {
	// nop
}

void touched_default(obj_t* obj) {
	// nop
}

// sub_2ED74
void obj_generic_coll(obj_t* obj) {
	if ((obj_type_flags[obj->type] & obj_type_flags_bit_20) && get_eta(obj)->interaction_flags & 0x20) {
		if (!(ray.etat == 3 && ray.subetat == 32)) {
			i16 command_par3 = ray.iframes_timer;
			if (command_par3 >= 60 || command_par3 == -1) {
				ray_hit(0, obj);
				ray.iframes_timer = command_par3;
			} else {
				ray_hit(1, obj);
				if (!(word_CF868 == 3 || word_CF868 == 4)) {
					ray.iframes_timer = 60;
				}
			}
		}
	}
}

// sub_612F0
void obj_generic_flip_direction(obj_t* obj) {
	obj->flags ^= obj_flags_8_flipped;
}


// Event 0: Livingstone

// sub_62B54
void punched_livingstone(obj_t* obj, u16 unk) {
	decrease_hitp_from_fist(obj);
	if (rayman_fist->xspeed > 0) {
		script_goto_label(obj, 3, 1);
	} else if (rayman_fist->xspeed < 0) {
		script_goto_label(obj, 2, 1);
	}
	obj->xspeed = 0;
	obj->ypos -= 2;
	if (obj->hitp != 0) {
		obj->yspeed = -4;
		set_obj_state(obj, 2, (get_eta(obj)->interaction_flags & 0x40) ? 10 : 2);
		play_sound(28, obj->obj_index);
	} else {
		obj->yspeed = -8;
		set_obj_state(obj, 0, (get_eta(obj)->interaction_flags & 0x40) ? 6 : 3);
	}

}

void touched_livingstone(obj_t* obj) {
	if ((obj->flags & obj_flags_8_flipped) != (ray.flags & obj_flags_8_flipped)) {
		if ((obj->etat == 1 && obj->subetat == 0) || (obj->etat == 0 && obj->subetat == 0)) {
			if (ray.etat == 0 && ray.subetat == 18 /* stick out tongue */) {
				obj->xspeed = 0;
				obj->yspeed = 0;
				set_obj_state(obj, 0, 2);
				script_goto_label(obj, (obj->flags & obj_flags_8_flipped) ? 8 : 7, 1);
			}
		}
	}
}

// Event 1: Platform

// sub_643BC
void punched_platform(obj_t* obj, u16 unk) {
	if (obj->hit_sprite == unk) {
		if (rayman_fist->xspeed > 0) {
			set_subetat(obj, 25);
		} else if (rayman_fist->xspeed < 0) {
			set_subetat(obj, 26);
		}
	}
}


// Event 2: Energy point

// sub_61EA0
void touched_energypoint(obj_t* obj) {
	destroy_obj_with_blink(obj);
	ray.hitp += obj->hitp;
	if (ray.hitp > ray_max_hitp) {
		ray.hitp = ray_max_hitp;
	}
	obj->flags &= ~obj_flags_4_switched_on;
	play_sound(8, obj->obj_index);

}


// Event 3: Electoon

// sub_291B4
void update_electoon(obj_t* obj) {
	// stub
}

// sub_2937C
void touched_electoon(obj_t* obj) {
	if (get_eta(obj)->interaction_flags & 1) {
		script_goto_label(obj, 0, 1);
	}
}

// Event 5: Bonus magician

// sub_6154B
void update_magician(obj_t* obj) {
	// stub
}

// Event 6: Yin with pin
// Event 7: Bouncing platform

// sub_3167C
void punched_bouncing_platform(obj_t* obj, u16 unk) {
	// stub
}

// Event 9: Small livingstone
// Event 165: Small livingstone (aggressive/evil)

// sub_629D8
void punched_small_livingstone(obj_t* obj, u16 unk) {
	if (unk == 255 || !(get_eta(obj)->interaction_flags & 1)) {
		decrease_hitp_from_fist(obj);
		if (obj->hitp == 0) {
			set_obj_state(obj, 0, 3);
			script_goto_label(obj, 2, 1);
			obj->ypos -= 2;
			obj->flags &= ~obj_flags_0x10;
		} else {

		}
	}
}

// sub_62AD4
void touched_small_livingstone(obj_t* obj) {
	if (obj->etat == 1 && obj->subetat == 11) {
		set_subetat(obj, 14);
	} else if ((obj->etat == 0 && obj->subetat == 0) || (obj->etat == 1 && obj->subetat == 0)) {
		set_obj_state(obj, 1, 11);
		obj->flags &= ~obj_flags_0x10;
		script_goto_label(obj, obj_flipped(obj) ? 3 : 2, 1);
	}
}

// Event 10: Piranha

// sub_7AD20
void create_splash(obj_t* obj) {
	// stub
}

// sub_673B4
bool check_piranha_respawn(obj_t* piranha) {
	// stub
	bool result = true;
	for (i32 i = 0; i < obj_count; ++i) {
		obj_t* event = obj_bodies + i;
		if (event->type == obj_10_piranha && event->spawn_x == piranha->spawn_x && !event->is_active && event->timer >= 100) {
			result = false;
			break;
		}
	}
	return result;
}

// sub_674C8
void update_piranha(obj_t* obj) {
	update_default(obj);
	bool respawn = false;
	if (obj->etat == 0) {
		i32 y_offset_by = obj->ypos + obj->offset_by;
		if (y_offset_by + 20 < level_height + 200) {
			++obj->timer;
		}
		if (obj->subetat == 3) {
			obj->yspeed = 0;
		} else if (obj->subetat == 9) {
			obj->yspeed = 6;
			if ((obj->ypos + obj->offset_hy > level_height + 200) || (y_offset_by + 14 > level_height_tiles * 16)) {
				if (y_offset_by + 14 > level_height_tiles * 16) {
					create_splash(obj);
				}
				obj->ypos = level_height + 200;
				obj->is_active = 0;
				obj->flags &= ~obj_flags_4_switched_on;
				obj->timer = 0;
				respawn = check_piranha_respawn(obj);
			}
		} else {
			if (y_offset_by < level_height) { //?
				obj->ypos = level_height + 200;
				obj->is_active = 0;
				obj->flags &= ~obj_flags_4_switched_on;
				respawn = check_piranha_respawn(obj);
			}
		}

		if (obj->timer == 100) {
			respawn = check_piranha_respawn(obj);
		}
		if (respawn) {
			// stub
		}
	}
}

void sub_67410(obj_t* obj) {
	// stub
}

// sub_676EC
void punched_piranha(obj_t* obj, u16 unk) {
	decrease_hitp_from_fist(obj);
	if (obj->hitp != 0) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_obj_state(obj, 0, 1);
	} else {
		set_obj_state(obj, 0, 3);
	}
	sub_67410(obj);
	obj->command = obj_flipped(obj) ? cmd_1_right : cmd_0_left;
}

// Event 12: Hunter 1
// Event 14: Hunter 2

// sub_628C0
void punched_hunter(obj_t* obj, u16 unk) {
	if (get_eta(obj)->interaction_flags & 1) {
		decrease_hitp_from_fist(obj);
		if (obj->hitp != 0) {
			obj->xspeed = 0;
			obj->yspeed = 0;
			set_obj_state(obj, 0, 1); // startled
		} else {
			set_obj_state(obj, 0, 3); // dead
		}
		obj->command = obj_flipped(obj) ? cmd_0_left : cmd_1_right;
	}
}

// sub_62928
void touched_hunter(obj_t* obj) {
	if (obj->etat == 0 && (obj->subetat == 0 || obj->subetat == 2)) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_obj_state(obj, 0, 2);
		obj->flags &= ~obj_flags_0x10;
		turn_to_face_rayman(obj);
		obj->command = obj_flipped(obj) ? cmd_1_right : cmd_0_left;
	}
}

// Event 20: Tentacle

void update_distance_bottom_right(obj_t* obj) {
	obj->screen_y = obj->ypos - level_height;
	obj->screen_x = obj->xpos - level_width + 8;
}

// tentacle_spawn_enemy
void tentacle_spawn_enemy(obj_t* tentacle, i16 which_enemy, i32 xspeed, i32 yspeed) {
	// stub
	for (i32 event_index = 0; event_index < obj_count; ++event_index) {
		obj_t* spawned = obj_bodies + event_index;
		if (((which_enemy == 1 && spawned->type == obj_0_livingstone) || (which_enemy == 2 && spawned->type == obj_9_small_livingstone))
				&& spawned->spawn_x <= 0 && spawned->is_active == 0
		) {
			spawned->flags &= ~(obj_flags_4_switched_on | obj_flags_0x40);
			spawned->is_active = 1;
			spawned->flags |= obj_flags_4_switched_on; // Note: this does not make sense, this bit was cleared just before!
			spawned->active_timer = 0;
			spawned->field_30 = 1;
			set_obj_state(spawned, 2, 2); // thrown up in air
			spawned->xpos = tentacle->xpos + tentacle->offset_bx - spawned->offset_bx;
			spawned->ypos = tentacle->ypos - (spawned->offset_by / 4);
			turn_to_face_rayman(spawned);
			spawned->yspeed = yspeed;
			spawned->xspeed = obj_flipped(spawned) ? xspeed : -xspeed;
			update_distance_bottom_right(spawned);
			spawned->flags &= ~obj_flags_0x10;
			return;
		}
	}
}

// sub_7B060
void sub_7B060(obj_t* obj) {
	// stub
	// something with a linked event 24?
}

// sub_62658
void update_tentacle(obj_t* obj) {
	if (obj->command == cmd_0_left) {
		obj_set_not_flipped(obj);
	} else if (obj->command == cmd_1_right) {
		obj_set_flipped(obj);
	}
	if (obj->etat == 0 && obj->subetat == 11) {
		if (obj->anim_frame >= 5 && obj->timer == 0) {
			++obj->timer;
			++obj->configuration; // alternate between 1=livingstone and 2=small livingstone
			if (obj->configuration > 2) {
				obj->configuration = 1;
			}
			if (obj->hitp == 1) {
				tentacle_spawn_enemy(obj, 2 /*small livingstone*/, 1, -2);
				tentacle_spawn_enemy(obj, 1 /*livingstone*/      , 2, -1);
			} else {
				tentacle_spawn_enemy(obj, obj->configuration   , 1, -1);
			}
		} else {
			if (obj->anim_frame > 2) {
				obj->timer = 0;
			}
		}
	} else if (obj->etat == 0 && obj->subetat == 21) {
		if (obj->anim_frame >= 5 && obj->timer == 0) {
			// last hit: spawn 4 enemies
			++obj->timer;
			i16 xspeed = -4;
			for (i32 i = 0; i < 4; ++i) {
				i16 which_enemy = (i % 2) + 1;
				i16 yspeed = -which_enemy;
				++xspeed;
				tentacle_spawn_enemy(obj, which_enemy, xspeed, yspeed);
				if (xspeed == -2) xspeed = 3;
			}
		} else {
			if (obj->anim_frame > 2) {
				obj->timer = 0;
			}
		}
	} else if (obj->etat == 0 && obj->subetat == 3) {
		++obj->timer;
		if (!(obj->configuration == 0 || obj->timer < -3)) {
			u8 saved_configuration = obj->configuration;
			sub_7B060(obj);
			obj->configuration = saved_configuration;
		}
		obj->xspeed = 0;
		obj->yspeed = 0;
		if (obj->anim_frame > 12) {
			obj->anim_frame = 3;
		} else if (obj->anim_frame <= 1){
			obj->timer = 0;
		}
	} else if (obj->etat == 1) {
		if (obj->phase != 0) ++obj->timer;
		if (obj->timer >= -2) {
			script_goto_label(obj, 2, 1);
		}
		update_x_momentum(obj);
		interact_with_rayman(obj);
	}
}

// sub_62854
void punched_tentacle(obj_t* obj) {

}

// sub_628B8
void touched_tentacle(obj_t* obj) {

}



typedef struct {
	void (*update)(obj_t* obj);
	void (*punched)(obj_t* obj, u16 unk);
	void (*coll)(obj_t* obj);
	void (*touched)(obj_t* obj);
	void (*flipped)(obj_t* obj);
} obj_procs_t;

obj_procs_t obj_procs[] = {
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

