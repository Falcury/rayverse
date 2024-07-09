
// (win32) rayverse.cpp
void win32_advance_frame(app_state_t* app_state);
void win32_prepare_frame(app_state_t* app_state);
void win32_end_frame(app_state_t* app_state);

// engine.cpp
void advance_frame();
void wait_frames(i32 n_frames);
void copy_full_image_contents(image_t* dest, image_t* source);
void copy_full_image_to_draw_buffer(image_t* image);
void destroy_image(image_t* image);

// platform routines
i64 get_clock();
float get_seconds_elapsed(i64 start, i64 end);

// sound.cpp
void game_get_sound_samples(game_sound_buffer_t* sound_buffer);