



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


// maybe move these to memory-related procedures?

//7FB48
void SpriteFixeOffset(mem_t* mem) {
    OffsetSpriteFixe = mem->cursor;
}

//7FB60
void SpriteWorldOffset(mem_t* mem) {
    OffsetSpriteWorld = mem->cursor;
}

//7FB78
void SpriteFixeBlocksFree(mem_t* mem) {
    mem->cursor = OffsetSpriteFixe;
}

//7FB84
void SpriteWorldBlocksFree(mem_t* mem) {
    mem->cursor = OffsetSpriteWorld;
}
