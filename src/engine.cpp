/*
void win32_print_current_directory() {
	char buffer[2048];
	if (GetCurrentDirectory(sizeof(buffer), buffer)) {
		printf("%s\n", buffer);
	}
}
*/

image_t create_palettized_image(i32 width, i32 height) {
	image_t result;
	memset(&result, 0, sizeof(result));
	result.width = width;
	result.height = height;
	result.memory_size = width * height;
	result.memory = (u8*) calloc(1, result.memory_size);
	return result;
}

image_t ubisoft_logo_image;

void game_init(game_state_t* game) {
	
	//load_level("data\\PCMAP\\JUNGLE\\RAY1.LEV");
	//load_world("data\\PCMAP\\RAY1.WLD");
	load_language_txt("data\\RAY.LNG");
	load_snd8b(&snd8b_data, 2);
	load_sav(1);
	ubisoft_logo_image = load_vignet_pcx(29); //load_ubisoft_logo();

	ogg_cd_track = open_cd_vorbis(12);
	is_ogg_playing = true;

	game->offscreen_buffer = create_palettized_image(320, 200);

	game->initialized = true;

}

void game_update_and_render(app_state_t* app_state) {
	
	game_state_t* game = &app_state->game;

	rgb_t clear_color = { 0, 0, 0 };
	render_clear(app_state->active_surface, clear_color);

	render_weird_gradient(app_state->active_surface, 0, 0);
	surface_blit_palettized_image(&ubisoft_logo_image, NULL, app_state->active_surface, NULL, ubisoft_logo_image.pal);

}