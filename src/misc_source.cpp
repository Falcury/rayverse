

FILE* open_data_file(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		char data_path[256];
		snprintf(data_path, 256, "data\\%s", filename);
		fp = fopen(data_path, "rb");
	}
	return fp;
}



static inline eta_t* get_eta(obj_t* obj) {
	return &obj->ETA[obj->etat][obj->subetat];
}

#define obj_flipped(obj) ((obj)->flags & obj_flags_8_flipped)

static inline void obj_set_flipped(obj_t* obj) {
	obj->flags |= obj_flags_8_flipped;
}

static inline void obj_set_not_flipped(obj_t* obj) {
	obj->flags &= ~obj_flags_8_flipped;
}

