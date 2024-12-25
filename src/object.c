
// sub_7B628
void DO_NOVA(obj_t* obj) {
	// stub
}

//5720C
void Drop_Atter(obj_t* obj) {
    //stub
}

//57230
void BadGuyAtter(obj_t* obj) {
    //stub
}

//572C4
void MiteAtter(obj_t* obj) {
    //stub
}

//572F8
void LidolPinkAtter(obj_t* obj) {
    //stub
}

//57344
void stoneDogAtter(obj_t* obj) {
    //stub
}

//5736C
void stoneDogBounces(obj_t* obj) {
    //stub
}

//574B8
void Spider_Atter(obj_t* obj) {
    //stub
}

//574F4
void Normal_Atter(obj_t* obj) {
    //stub
}



// Generic obj procs


void DoRaymanInZDDDefault(obj_t* obj) {
	// nop
}



// sub_612F0
void ObjectUTurnDefault(obj_t* obj) {
	obj->flags ^= obj_flags_8_flipped;
}


// Event 0: Livingstone

// sub_62B54
void DoBadGuy1PoingCollision(obj_t* obj, u16 unk) {
	obj_hurt(obj);
	if (poing_obj->xspeed > 0) {
        skipToLabel(obj, 3, 1);
	} else if (poing_obj->xspeed < 0) {
        skipToLabel(obj, 2, 1);
	}
	obj->xspeed = 0;
	obj->ypos -= 2;
	if (obj->hitp != 0) {
		obj->yspeed = -4;
		set_main_and_sub_etat(obj, 2, (get_eta(obj)->interaction_flags & 0x40) ? 10 : 2);
        PlaySnd(28, obj->obj_index);
	} else {
		obj->yspeed = -8;
		set_main_and_sub_etat(obj, 0, (get_eta(obj)->interaction_flags & 0x40) ? 6 : 3);
	}

}

void DoBudGuy1RaymanZDD(obj_t* obj) {
	if ((obj->flags & obj_flags_8_flipped) != (ray.flags & obj_flags_8_flipped)) {
		if ((obj->main_etat == 1 && obj->sub_etat == 0) || (obj->main_etat == 0 && obj->sub_etat == 0)) {
			if (ray.main_etat == 0 && ray.sub_etat == 18 /* stick out tongue */) {
				obj->xspeed = 0;
				obj->yspeed = 0;
				set_main_and_sub_etat(obj, 0, 2);
                skipToLabel(obj, (obj->flags & obj_flags_8_flipped) ? 8 : 7, 1);
			}
		}
	}
}

// Event 1: Platform

// sub_643BC
void DoPlatformPoingCollision(obj_t* obj, u16 unk) {
	if (obj->hit_sprite == unk) {
		if (poing_obj->xspeed > 0) {
			set_subetat(obj, 25);
		} else if (poing_obj->xspeed < 0) {
			set_subetat(obj, 26);
		}
	}
}


// Event 2: Energy point

// sub_61EA0
void DoPowerupRaymanCollision(obj_t* obj) {
	DO_NOVA(obj);
	ray.hitp += obj->hitp;
	if (ray.hitp > ray_max_hitp) {
		ray.hitp = ray_max_hitp;
	}
	obj->flags &= ~obj_flags_4_triggered;
    PlaySnd(8, obj->obj_index);

}



// Event 5: Bonus magician

// sub_6154B
void TEST_WIZARD(obj_t* obj) {
	// stub
}

// Event 6: Yin with pin
// Event 7: Bouncing platform
// Event 9: Small livingstone
// Event 165: Small livingstone (aggressive/evil)

// sub_629D8
void DoBadGuy23PoingCollision(obj_t* obj, u16 unk) {
	if (unk == 255 || !(get_eta(obj)->interaction_flags & 1)) {
		obj_hurt(obj);
		if (obj->hitp == 0) {
			set_main_and_sub_etat(obj, 0, 3);
            skipToLabel(obj, 2, 1);
			obj->ypos -= 2;
			obj->flags &= ~obj_flags_0x10;
		} else {

		}
	}
}

// sub_62AD4
void DoBadGuy23RaymanZDD(obj_t* obj) {
	if (obj->main_etat == 1 && obj->sub_etat == 11) {
		set_subetat(obj, 14);
	} else if ((obj->main_etat == 0 && obj->sub_etat == 0) || (obj->main_etat == 1 && obj->sub_etat == 0)) {
		set_main_and_sub_etat(obj, 1, 11);
		obj->flags &= ~obj_flags_0x10;
        skipToLabel(obj, obj_flipped(obj) ? 3 : 2, 1);
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
	for (i32 i = 0; i < level.nb_objects; ++i) {
		obj_t* event = level.objects + i;
		if (event->type == obj_10_piranha && event->spawn_x == piranha->spawn_x && !event->is_active && event->timer >= 100) {
			result = false;
			break;
		}
	}
	return result;
}

// sub_674C8
void DO_PYRANHA(obj_t* obj) {
	DO_ONE_CMD(obj);
	bool respawn = false;
	if (obj->main_etat == 0) {
		i32 y_offset_by = obj->ypos + obj->offset_by;
		if (y_offset_by + 20 < ymap + 200) {
			++obj->timer;
		}
		if (obj->sub_etat == 3) {
			obj->yspeed = 0;
		} else if (obj->sub_etat == 9) {
			obj->yspeed = 6;
			if ((obj->ypos + obj->offset_hy > ymap + 200) || (y_offset_by + 14 > mp.height * 16)) {
				if (y_offset_by + 14 > mp.height * 16) {
					create_splash(obj);
				}
				obj->ypos = ymap + 200;
				obj->is_active = 0;
				obj->flags &= ~obj_flags_4_triggered;
				obj->timer = 0;
				respawn = check_piranha_respawn(obj);
			}
		} else {
			if (y_offset_by < ymap) { //?
				obj->ypos = ymap + 200;
				obj->is_active = 0;
				obj->flags &= ~obj_flags_4_triggered;
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
void DoFishPoingCollision(obj_t* obj, u16 unk) {
	obj_hurt(obj);
	if (obj->hitp != 0) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_main_and_sub_etat(obj, 0, 1);
	} else {
		set_main_and_sub_etat(obj, 0, 3);
	}
	sub_67410(obj);
	obj->command = obj_flipped(obj) ? cmd_1_right : cmd_0_left;
}

// Event 12: Hunter 1
// Event 14: Hunter 2

// sub_628C0
void DoChasseurPoingCollision(obj_t* obj, u16 unk) {
	if (get_eta(obj)->interaction_flags & 1) {
		obj_hurt(obj);
		if (obj->hitp != 0) {
			obj->xspeed = 0;
			obj->yspeed = 0;
			set_main_and_sub_etat(obj, 0, 1); // startled
		} else {
			set_main_and_sub_etat(obj, 0, 3); // dead
		}
		obj->command = obj_flipped(obj) ? cmd_0_left : cmd_1_right;
	}
}

// sub_62928
void DoChasseurRaymanZDD(obj_t* obj) {
	if (obj->main_etat == 0 && (obj->sub_etat == 0 || obj->sub_etat == 2)) {
		obj->xspeed = 0;
		obj->yspeed = 0;
		set_main_and_sub_etat(obj, 0, 2);
		obj->flags &= ~obj_flags_0x10;
		calc_obj_dir(obj);
		obj->command = obj_flipped(obj) ? cmd_1_right : cmd_0_left;
	}
}

// Event 20: Tentacle

void update_distance_bottom_right(obj_t* obj) {
	obj->screen_y = obj->ypos - ymap;
	obj->screen_x = obj->xpos - xmap + 8;
}

// tentacle_spawn_enemy
void tentacle_spawn_enemy(obj_t* tentacle, i16 which_enemy, i32 xspeed, i32 yspeed) {
	// stub
	for (i32 event_index = 0; event_index < level.nb_objects; ++event_index) {
		obj_t* spawned = level.objects + event_index;
		if (((which_enemy == 1 && spawned->type == obj_0_livingstone) || (which_enemy == 2 && spawned->type == obj_9_small_livingstone))
				&& spawned->spawn_x <= 0 && spawned->is_active == 0
		) {
			spawned->flags &= ~(obj_flags_4_triggered | obj_flags_0x40);
			spawned->is_active = 1;
			spawned->flags |= obj_flags_4_triggered; // Note: this does not make sense, this bit was cleared just before!
			spawned->active_timer = 0;
			spawned->field_30 = 1;
			set_main_and_sub_etat(spawned, 2, 2); // thrown up in air
			spawned->xpos = tentacle->xpos + tentacle->offset_bx - spawned->offset_bx;
			spawned->ypos = tentacle->ypos - (spawned->offset_by / 4);
			calc_obj_dir(spawned);
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
void DO_TEN_COMMAND(obj_t* obj) {
	if (obj->command == cmd_0_left) {
		obj_set_not_flipped(obj);
	} else if (obj->command == cmd_1_right) {
		obj_set_flipped(obj);
	}
	if (obj->main_etat == 0 && obj->sub_etat == 11) {
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
	} else if (obj->main_etat == 0 && obj->sub_etat == 21) {
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
	} else if (obj->main_etat == 0 && obj->sub_etat == 3) {
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
	} else if (obj->main_etat == 1) {
		if (obj->phase != 0) ++obj->timer;
		if (obj->timer >= -2) {
            skipToLabel(obj, 2, 1);
		}
		SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
	}
}

// sub_62854
void DoGeneBadGuyPoingCollision(obj_t* obj, u16 unk) {

}

// sub_628B8
void DoGeneBadGuyRaymanZDD(obj_t* obj) {

}

//6309C
void DO_PHOTOGRAPHE_CMD(obj_t* obj) {

}

//6323C
void DoAudioStartRaymanCollision(obj_t* obj) {

}


typedef struct {
	void (*command)(obj_t* obj);
	void (*poing_collision)(obj_t* obj, u16 unk);
	void (*rayman_collision)(obj_t* obj);
	void (*rayman_in_zone)(obj_t* obj);
	void (*u_turn)(obj_t* obj);
} obj_procs_t;

//935FC
obj_procs_t ObjectsFonctions[] = {
		{DO_ONE_CMD,                 DoBadGuy1PoingCollision,    DoRaymanCollisionDefault,    DoBudGuy1RaymanZDD,       ObjectUTurnDefault}, // 0: livingstone
		{DO_ONE_CMD,                 DoPlatformPoingCollision,   DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 1: platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoPowerupRaymanCollision, ObjectUTurnDefault}, // 2: energy point
		{DO_ONE_PINK_CMD,            DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoLidolPinkRaymanZDD,     ObjectUTurnDefault}, // 3: electoon
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 4: scenery
		{TEST_WIZARD,                DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 5: magician
		{DO_ONE_CMD,                 DoFallingObjPoingCollision, DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 6: yin with pin
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 7: swinging spiky fruit
		{DO_ONE_CMD,                 DoFallingObjPoingCollision, DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 8: plum 1
		{DO_ONE_CMD,                 DoBadGuy23PoingCollision,   DoRaymanCollisionDefault,    DoBadGuy23RaymanZDD,      ObjectUTurnDefault}, // 9: small livingstone
		{DO_PYRANHA,                 DoFishPoingCollision,       DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 10: piranha
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 11: hit effect
		{DO_ONE_CMD,                 DoChasseurPoingCollision,   DoRaymanCollisionDefault,    DoChasseurRaymanZDD,      ObjectUTurnDefault}, // 12: hunter 1
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 13: hunter bullet
		{DO_ONE_CMD,                 DoChasseurPoingCollision,   DoRaymanCollisionDefault,    DoChasseurRaymanZDD,      ObjectUTurnDefault}, // 14: hunter 2
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 15:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 16: falling platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 17: rising platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 18:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 19: splash
		{DO_TEN_COMMAND,             DoGeneBadGuyPoingCollision, DoRaymanCollisionDefault,    DoGeneBadGuyRaymanZDD,    ObjectUTurnDefault}, // 20: tentacle
		{DO_PHOTOGRAPHE_CMD,         DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 21: photographer
		{DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 22: moving platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 23: rayman
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 24:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 25: ejecting platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 26: disappearing cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 27: trampoline cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 28: blinking cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 29:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoAudioStartRaymanCollision, DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 30: audio start
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 31:
};

