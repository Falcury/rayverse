/*
void win32_print_current_directory() {
	char buffer[2048];
	if (GetCurrentDirectory(sizeof(buffer), buffer)) {
		printf("%s\n", buffer);
	}
}
*/

image_t create_palettized_image(i32 width, i32 height) {
	image_t result = {0};
	memset(&result, 0, sizeof(result));
	result.width = width;
	result.height = height;
	result.memory_size = width * height;
	result.memory = (u8*) calloc(1, result.memory_size);
	result.pal = (rgb_palette_t*) calloc(1, sizeof(rgb_palette_t));
	result.pal_needs_free = true;
	return result;
}

void destroy_image(image_t* image) {
	if (image->memory) {
		free(image->memory);
		image->memory = NULL;
	}
	if (image->pal_needs_free && image->pal) {
		free(image->pal);
		image->pal = NULL;
	}
}

void game_init_sound(game_sound_buffer_t* sound, i32 samples_per_second) {
	// Prepare a sound buffer for the game code to write into.
	sound->samples_per_second = samples_per_second;
	sound->samples = (i16*)calloc(1, sound->samples_per_second * 2 * sizeof(i16));
}

void game_init(game_state_t* game) {
	CarteSonAutorisee = 1;
	game->draw_buffer = create_palettized_image(320, 200);
	game->initialized = true;
	global_game = game;
}

void advance_frame(void) {
	app_state_t* app_state = &global_app_state;
	game_state_t* game = &app_state->game;
	rgb_t clear_color = { 0, 0, 0 };
	render_clear(app_state->active_surface, clear_color);
    surface_blit_palettized_image(&game->draw_buffer, &game->draw_palette, NULL, app_state->active_surface, NULL);
#if _WIN32
	win32_advance_frame(app_state);
#else
    linux_advance_frame(app_state);
#endif
}


