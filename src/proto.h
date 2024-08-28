
// (win32) rayverse.cpp
#ifdef _WIN32
void win32_advance_frame(app_state_t* app_state);
void win32_prepare_frame(app_state_t* app_state);
void win32_end_frame(app_state_t* app_state);
#else
void linux_advance_frame(app_state_t* app_state);
void linux_prepare_frame(app_state_t* app_state);
void linux_end_frame(app_state_t* app_state);
#endif
void message_box(const char* message);

// engine.cpp
void advance_frame();
void wait_frames(i32 n_frames);
void copy_full_image_contents(image_t* dest, image_t* source);
void copy_full_image_to_draw_buffer(image_t* image);
void copy_full_image_to_background_buffer(image_t* image); // TODO: keep or change?
void clrscr();
void game_init_sound(game_sound_buffer_t* sound, i32 samples_per_second);
void game_init(game_state_t* game);
void destroy_image(image_t* image);

// platform routines
i64 get_clock();
float get_seconds_elapsed(i64 start, i64 end);

// sound.cpp
void load_snd8b(u8** sound_buffer, i32 sound_set);
void game_get_sound_samples(game_sound_buffer_t* sound_buffer);
void play_cd_track(i32 track_number);
void stop_cd_music();

// game.cpp
u8 decode_xor(u8* data, u32 size, u8 encoding_byte, u8 checksum_byte);
void set_subetat(obj_t* obj, u8 subetat);
void set_etat(obj_t* obj, u8 etat);
void set_obj_state(obj_t* obj, u8 etat, u8 subetat);
void horloges(u32 ticks);
i32 get_proj_dist(i32 scale, i32 outer_dim);
i32 get_proj_x(i32 scale, i32 par_1);
i32 get_proj_y(i32 scale, i32 par_1);
void display2(obj_t* obj);
void set_x_speed(obj_t* obj);
void do_anim(obj_t* obj);
bool end_of_animation(obj_t* obj);
void synchro_loop(scene_func_t scene_func);
void do_fade_step(rgb_palette_t* source_pal, rgb_palette_t* dest_pal);
void start_fade_in(u32 speed);
void start_fade_out(u32 speed);
void fade_out(u32 speed, rgb_palette_t* palette);
void INIT_FADE_IN();
void draw_simple(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void draw_simple_flipped(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void sub_16A24(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void sub_16B08(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void rayman_main();
