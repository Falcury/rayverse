

i64 get_clock(void) {
    struct timespec t = {0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_nsec + 1000000000 * t.tv_sec;
}

float get_seconds_elapsed(i64 start, i64 end) {
    i64 elapsed_nanoseconds = end - start;
    float elapsed_seconds = ((float)elapsed_nanoseconds) / 1e9f;
    return elapsed_seconds;
}

void linux_sleep(u32 ms) {
    struct timespec tim = {0}, tim2 = {0};
    tim.tv_sec = 0;
    tim.tv_nsec = ms * 1000000;
    nanosleep(&tim, &tim2);
}

void message_box(const char* message) {
    SDL_ShowSimpleMessageBox(0, "Rayverse", message, global_app_state.sdl.window);
}

void toggle_fullscreen(SDL_Window* window) {
    //stub
}

void linux_process_keyboard_event(SDL_Scancode scancode, bool is_down) {
    static u8 sdl_scancode_to_dos_scancode[256] = {
            0, 0, 0, 0, SC_A, SC_B, SC_C, SC_D, SC_E, SC_F,
            SC_G, SC_H, SC_I, SC_J, SC_K, SC_L, SC_M, SC_N, SC_O,
            SC_P, SC_Q, SC_R, SC_S, SC_T, SC_U, SC_V, SC_W, SC_X,
            SC_Y, SC_Z, SC_1, SC_2, SC_3, SC_4, SC_5, SC_6, SC_7,
            SC_8, SC_9, SC_0, SC_ENTER, SC_ESCAPE, SC_BACKSPACE, SC_TAB, SC_SPACE,
            SC_MINUS, SC_EQUALS, SC_LEFTBRACKET, SC_RIGHTBRACKET, SC_BACKSLASH, SC_BACKSLASH,
            SC_SEMICOLON, SC_QUOTE, SC_TILDE, SC_COMMA, SC_PERIOD, SC_SLASH, SC_CAPSLOCK,
            SC_F1, SC_F2, SC_F3, SC_F4, SC_F5, SC_F6, SC_F7,SC_F8,
            SC_F9, SC_F10, SC_F11, SC_F12, 0, SC_SCROLLLOCK, 0, SC_INSERT,
            SC_HOME, SC_PAGEUP, SC_DELETE, SC_END, SC_PAGEDOWN, SC_RIGHT, SC_LEFT,
            SC_DOWN, SC_UP, SC_NUMLOCK, SC_SLASH, SC_MULTIPLY, SC_MINUS, SC_PLUS,
            SC_ENTER, SC_1, SC_2, SC_3, SC_4, SC_5, SC_6, SC_7, SC_8, SC_9, SC_0,
            SC_PERIOD, SC_TILDE, 0, 0, SC_EQUALS, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC_COMMA, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, SC_CONTROL, SC_LSHIFT, SC_ALT, 0, SC_CONTROL,
            SC_RSHIFT, SC_ALT, 0, 0,
    };
    u8 dos_scancode = sdl_scancode_to_dos_scancode[scancode & 0xFF];
    Touche_Enfoncee[dos_scancode & 0x7F] = is_down;
}

bool process_input(SDL_Window* window) {
    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            global_app_state.running = false;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
            global_app_state.running = false;
        } else if (event.type == SDL_KEYDOWN) {
            linux_process_keyboard_event(event.key.keysym.scancode, true);
        } else if (event.type == SDL_KEYUP) {
            linux_process_keyboard_event(event.key.keysym.scancode, false);
        }
    }
    return true; //stub
}

int main(int argc, char** argv) {
    app_state_t* app_state = &global_app_state;

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
    // GL 3.3 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    int desired_window_width = 640;
    int desired_window_height = 400;

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    u32 window_flags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
//    if (window_start_maximized) {
//        window_flags |= SDL_WINDOW_MAXIMIZED;
//    }
    window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Renderer* renderer;
    SDL_Window* window;
    if (SDL_CreateWindowAndRenderer(desired_window_width, desired_window_height, window_flags, &window, &renderer) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
//    g_window = window;
    app_state->sdl.window = window;

    {
        i32 gl_w, gl_h;
        SDL_GL_GetDrawableSize(window, &gl_w, &gl_h);
        i32 window_w, window_h;
        SDL_GetWindowSize(window, &window_w, &window_h);
        app_state->client_width = gl_w;
        app_state->client_height = gl_h;
        app_state->display_scale_factor = (float) gl_w / (float) window_w;
        app_state->display_points_per_pixel = (float) window_w / (float) gl_w;
    }

    linux_init_opengl(app_state);

    // Initialize the RGBA buffer
    memset(&app_state->game_surface, 0, sizeof(app_state->game_surface));
    surface_resize(&app_state->game_surface, 320, 200);
    app_state->active_surface = &app_state->game_surface;

    app_state->target_game_hz = 60;
    app_state->target_seconds_per_frame = 1.0f / (float)app_state->target_game_hz;

    sdl_sound_output_t* sound_output = &app_state->sdl.sound_output;
    sound_output->samples_per_second = 44100;
    sound_output->bytes_per_sample = sizeof(i16) * 2;
    sound_output->secondary_buffer_size = sound_output->samples_per_second * sound_output->bytes_per_sample; // 1 second
    sound_output->secondary_buffer = (u8*)calloc(sound_output->bytes_per_sample, sound_output->samples_per_second);
    sound_output->safety_bytes = (u32)((float)(sound_output->samples_per_second * sound_output->bytes_per_sample) * app_state->target_seconds_per_frame * 0.3333f);

    SDL_AudioSpec spec = {0};
    spec.format = AUDIO_S16;
    spec.channels = 2;
    spec.freq = 44100;
    spec.samples = 256;
    spec.callback = NULL;
//    spec.callback = audio_callback;
    SDL_AudioSpec obtained_spec = {0};
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &spec, &obtained_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (dev == 0) {
        printf("Failed to open Audio Device: %s\n", SDL_GetError());
        return 1;
    }
    app_state->sdl.sound_output.audio_device = dev;
    SDL_PauseAudioDevice(app_state->sdl.sound_output.audio_device, 0);

    game_init_sound(&app_state->game.sound_buffer, (i32)sound_output->samples_per_second);

    if (!app_state->game.initialized) {
        game_init(&app_state->game);
    }

    app_state->flip_clock = get_clock();

    global_app_state.running = true;
    return main_Ray(argc, argv); // run the game!
}

void linux_prepare_frame(app_state_t* app_state) {
    process_input(app_state->sdl.window);

    glDrawBuffer(GL_BACK);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    win32_get_window_dimension(app_state->win32.window, &app_state->client_width, &app_state->client_height);
}

void linux_end_frame(app_state_t* app_state) {
    //stub
    i64 frames_elapsed = 0;
    i64 ns_per_tick = 1000000000 / app_state->target_game_hz;
    while (frames_elapsed < 1) {
        frames_elapsed = ((get_clock() / ns_per_tick) - (app_state->frame_clock / ns_per_tick));
        linux_sleep(1);
    }
    app_state->frame_clock += ns_per_tick * frames_elapsed;

    i32 gl_w, gl_h;
    SDL_GL_GetDrawableSize(app_state->sdl.window, &gl_w, &gl_h);
    opengl_upload_surface(app_state, app_state->active_surface, gl_w, gl_h);
    linux_produce_sound_for_frame(app_state, &app_state->sdl.sound_output, &app_state->game.sound_buffer, app_state->flip_clock);

    SDL_GL_SwapWindow(app_state->sdl.window);
    app_state->flip_clock = get_clock();
}

void linux_advance_frame(app_state_t* app_state) {
    linux_prepare_frame(app_state);
    linux_end_frame(app_state);
    if (!app_state->running) {
        exit(0);
    }
}
