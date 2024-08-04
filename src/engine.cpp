/*
void win32_print_current_directory() {
	char buffer[2048];
	if (GetCurrentDirectory(sizeof(buffer), buffer)) {
		printf("%s\n", buffer);
	}
}
*/

image_t create_palettized_image(i32 width, i32 height) {
	image_t result = {};
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

void copy_full_image_contents(image_t* dest, image_t* source) {
	ASSERT(dest->width == source->width && dest->height == source->height && dest->memory_size == source->memory_size);
	memcpy(dest->memory, source->memory, source->memory_size);
	*dest->pal = *source->pal; //copy full palette by value
}

void copy_full_image_to_draw_buffer(image_t* image) {
	copy_full_image_contents(&global_game->draw_buffer, image);
}

void copy_full_image_to_background_buffer(image_t* image) {
	copy_full_image_contents(&global_game->draw_buffer_bg, image);
}

//3CDF8
void clrscr() {
	image_t* dest = &global_game->draw_buffer;
	image_t* source = &global_game->draw_buffer_bg;
	ASSERT(dest->width == source->width && dest->height == source->height && dest->memory_size == source->memory_size);
	memcpy(dest->memory, source->memory, source->memory_size);
}

image_t ubisoft_logo_image;

void game_init_sound(game_sound_buffer_t* sound, i32 samples_per_second) {
	// Prepare a sound buffer for the game code to write into.
	sound->samples_per_second = samples_per_second;
	sound->samples = (i16*)calloc(1, sound->samples_per_second * 2 * sizeof(i16));
}

void game_init(game_state_t* game) {
	//load_level("PCMAP\\JUNGLE\\RAY1.LEV");
	//load_world("PCMAP\\RAY1.WLD");
	LoadLanguageTxt(0); // debug
	load_snd8b(&snd8b_data, 2); // debug
	load_sav(1); // debug

	game->draw_buffer = create_palettized_image(320, 200);
	game->draw_buffer_bg = create_palettized_image(320, 200);

	game->initialized = true;
	global_game = game;

}

void game_update_and_render(app_state_t* app_state) {
	
	game_state_t* game = &app_state->game;

	rgb_t clear_color = { 0, 0, 0 };
	render_clear(app_state->active_surface, clear_color);

//	render_weird_gradient(app_state->active_surface, 0, 0);
	copy_full_image_contents(&game->draw_buffer, &ubisoft_logo_image);
	surface_blit_palettized_image(&game->draw_buffer, NULL, app_state->active_surface, NULL);

}

void advance_frame() {
	app_state_t* app_state = &global_app_state;
	game_state_t* game = &app_state->game;
	rgb_t clear_color = { 0, 0, 0 };
	render_clear(app_state->active_surface, clear_color);
	surface_blit_palettized_image(&game->draw_buffer, NULL, app_state->active_surface, NULL);
	win32_advance_frame(app_state);
}

void wait_frames(i32 n_frames) {
	for (i32 i = 0; i < n_frames; ++i) {
		advance_frame();
	}
}
