


void skipToLabel(obj_t* obj, u8 unk1, u8 unk2) {
    // stub
}

static inline eta_t* get_eta(obj_t* obj) {
	return &obj->eta[obj->main_etat][obj->sub_etat];
}

#define obj_flipped(obj) ((obj)->flags & obj_flags_8_flipped)

static inline void obj_set_flipped(obj_t* obj) {
	obj->flags |= obj_flags_8_flipped;
}

static inline void obj_set_not_flipped(obj_t* obj) {
	obj->flags &= ~obj_flags_8_flipped;
}

