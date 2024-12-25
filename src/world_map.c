




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

//5A6D8
void INIT_MOTEUR_WORLD(void) {
    new_world = 0;
    new_level = 1;
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
