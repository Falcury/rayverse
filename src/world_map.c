
//69518
void INIT_PASTILLES_SAUVE() {
	char* text = (NBRE_SAVE != 0) ? language_txt[150] /*/save game/*/: language_txt[152]; /*/no save available/*/
	t_world_info[18].text = text;
	t_world_info[19].text = text;
	t_world_info[20].text = text;
	t_world_info[21].text = text;
	t_world_info[22].text = text;
	t_world_info[23].text = text;
}


//68BC0
void INIT_WORLD_INFO() {
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
