

s64 performance_counter_frequency;
s32 monitor_refresh_hz;

s64 get_clock(void) {
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result.QuadPart;
}

float get_seconds_elapsed(s64 start, s64 end) {
	return (float)(end - start) / (float)performance_counter_frequency;
}

void message_box(const char* message) {
	MessageBoxA(global_app_state.win32.window, message, "Rayverse", MB_ICONERROR);
}

WINDOWPLACEMENT window_position = { sizeof(window_position) };
void toggle_fullscreen(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	if (style & WS_OVERLAPPEDWINDOW) {
#if defined(_MSC_VER) && (_MSC_VER < 1900)
		s32 screen_width = GetSystemMetrics(SM_CXSCREEN);
		s32 screen_height = GetSystemMetrics(SM_CYSCREEN);
		if (GetWindowPlacement(window, &window_position)) {
			SetWindowLong(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			// See: https://stackoverflow.com/questions/23145217/flickering-when-borderless-window-and-desktop-dimensions-are-the-same
			// Why????
			SetWindowPos(window, HWND_TOP, 0, 0, screen_width +1, screen_height+1, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		
#else
		MONITORINFO monitor_info = {0};
		monitor_info.cbSize = sizeof(monitor_info);
		if (GetWindowPlacement(window, &window_position) &&
		    GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
		{
			SetWindowLong(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
			// See: https://stackoverflow.com/questions/23145217/flickering-when-borderless-window-and-desktop-dimensions-are-the-same
			// Why????
			SetWindowPos(window, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
			             monitor_info.rcMonitor.right - monitor_info.rcMonitor.left + 1,
			             monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
#endif
		
	} else {
		SetWindowLong(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &window_position);
		SetWindowPos(window, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		// On Windows 98, the taskbar might need repainting after returning from fullscreen
		OSVERSIONINFOA versioninfo = {0};
		versioninfo.dwOSVersionInfoSize = sizeof(versioninfo);
		GetVersionEx(&versioninfo);
		if (versioninfo.dwMajorVersion < 5) {
			HWND hwnd_taskbar = FindWindow("Shell_TrayWnd", NULL);
			ShowWindow(hwnd_taskbar, SW_HIDE);
			ShowWindow(hwnd_taskbar, SW_SHOW);
			SetActiveWindow(window);
		}
	}
}


static void win32_get_window_dimension(HWND window, int* client_width, int* client_height) {
	RECT rect;
	GetClientRect(window, &rect);
	if (client_width) *client_width = rect.right - rect.left;
	if (client_height) *client_height = rect.bottom - rect.top;
}


LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {

	LRESULT result = 0;
	switch(message) {
	default: {
		result = DefWindowProc(window, message, wparam, lparam);
			 } break;
	case WM_CLOSE:
    case WM_DESTROY: {
		global_app_state.running = false;
	} break;
	case WM_SETCURSOR: {
		u16 hit_test_result = LOWORD(lparam);
//		printf("hit_test_result = %d\n", hit_test_result);
		if (hit_test_result >= HTLEFT && hit_test_result <= HTBOTTOMRIGHT) {
			result = DefWindowProc(window, message, wparam, lparam);
		} else {
			SetCursor(global_app_state.win32.cursor);
		}
	} break;
	case WM_SIZE: {
		int client_width, client_height;
		win32_get_window_dimension(window, &client_width, &client_height);
		surface_resize(&global_app_state.offscreen_surface, client_width, client_height);
	} break;
    case WM_SYSCOMMAND: {
        if (wparam == SC_KEYMENU && (lparam>>16) <=0) {
            // do nothing
        } else {
            result = DefWindowProc(window, message, wparam, lparam);
        }
    } break;
    case WM_KILLFOCUS: {
        memset(Touche_Enfoncee, 0, sizeof(Touche_Enfoncee)); // Window loses focus -> release all keys
    } break;
	//case WM_ERASEBKGND: {
	//	result = TRUE; // prevent flickering
//						} break;
	/*case WM_PAINT: {
		result = TRUE;
				   } break;*/
	}


	return result;
}

void win32_process_keyboard_event(u32 vk_code, bool is_down) {
    static u8 windows_code_to_dos_scancode[256] = {
            0, 0, 0, 0, 0, 0, 0, 0, SC_BACKSPACE, SC_TAB, 0, 0, 0, SC_ENTER, 0, 0,
            SC_LSHIFT, SC_CONTROL, SC_ALT, 0, SC_CAPSLOCK, 0, 0, 0, 0, 0, 0, SC_ESCAPE, 0, 0, 0, 0,
            SC_SPACE, SC_PAGEUP, SC_PAGEDOWN, SC_END, SC_HOME, SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, 0, 0, 0, 0, SC_INSERT, SC_DELETE, 0,
            SC_0, SC_1, SC_2, SC_3, SC_4, SC_5, SC_6, SC_7, SC_8, SC_9, 0, 0, 0, 0, 0, 0, 0,
            SC_A, SC_B, SC_C, SC_D, SC_E, SC_F, SC_G, SC_H, SC_I, SC_J, SC_K, SC_L, SC_M, SC_N, SC_O,
            SC_P, SC_Q, SC_R, SC_S, SC_T, SC_U, SC_V, SC_W, SC_X, SC_Y, SC_Z, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            SC_F1, SC_F2, SC_F3, SC_F4, SC_F5, SC_F6, SC_F7, SC_F8, SC_F9, SC_F10, SC_F11, SC_F12, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            SC_NUMLOCK, SC_SCROLLLOCK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            SC_LSHIFT, SC_RSHIFT, SC_CONTROL, SC_CONTROL, SC_ALT, SC_ALT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC_SEMICOLON, SC_PLUS, SC_COMMA, SC_MINUS, SC_PERIOD, SC_SLASH,
            SC_TILDE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SC_LEFTBRACKET, SC_BACKSLASH, SC_RIGHTBRACKET, SC_QUOTE, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    u8 dos_scancode = windows_code_to_dos_scancode[vk_code & 0xFF];
    if (is_down && ((dos_scancode == SC_ENTER && Touche_Enfoncee[SC_ALT]) || dos_scancode == SC_F11)) {
        toggle_fullscreen(global_app_state.win32.window);
    } else {
        if (!is_down) {
            dos_scancode |= 0x80;
        }
        PC_keyboard_interrupt_handler(dos_scancode);
    }
}

void process_message(HWND window, MSG message) {
	if (message.message == WM_QUIT) {
		global_app_state.running = false;
	}

	switch(message.message) {
		default: {
//			printf("Window message: %x\n", message.message);
			TranslateMessage(&message);
			DispatchMessage(&message);
		} break;
		case WM_ERASEBKGND:
		case WM_MOUSEMOVE: break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: {
			TranslateMessage(&message);
			DispatchMessageA(&message);

			//https://stackoverflow.com/questions/8737566/rolling-ones-own-keyboard-input-system-in-c-c
			u32 vk_code = (u32) message.wParam;
			u32 lparam = message.lParam;
			u32 scancode = ((lparam >> 16) & 0x7f) | ((lparam & (1 << 24)) != 0 ? 0x80 : 0);
            //https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input?redirectedfrom=MSDN#extended-key-flag
            bool is_extended_key = (message.lParam & KF_EXTENDED) == KF_EXTENDED;
            if (is_extended_key) {
                scancode = MAKEWORD(scancode, 0xE0);
            }
            u32 vk_code_ext = vk_code;
            // NOTE: MAPVK_VSC_TO_VK_EX is not available on Win9x
#ifdef MAPVK_VSC_TO_VK_EX
            switch(vk_code) {
                default: break;
                case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
                case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
                case VK_MENU:    // converts to VK_LMENU or VK_RMENU
                    vk_code_ext = LOWORD(MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX));
                    break;
            }
#endif
//			u32 hid_code = keycode_windows_to_hid(scancode);
//			if (vk_code == VK_SPACE) {
//				hid_code = KEY_Space; // NOTE: for some reason, Space is missing from the table in keycode_windows_to_hid()
//			}
			bool alt_down = ((message.lParam & (1 << 29)) != 0);
			bool is_down = ((message.lParam & (1 << 31)) == 0);
			bool was_down = ((message.lParam & (1 << 30)) != 0);
			int repeat_count = message.lParam & 0xFFFF;
			s16 ctrl_state = GetKeyState(VK_CONTROL);
			bool32 ctrl_down = (ctrl_state < 0); // 'down' determined by high order bit == sign bit
			if (was_down && is_down) break; // uninteresting: repeated key

            win32_process_keyboard_event(vk_code_ext, is_down);

			switch (vk_code) {
				default: break;
				case VK_F4: {
					if (is_down && alt_down) {
						global_app_state.running = false;
					}
				} break;
			}
            break;
		} break;
        case WM_LBUTTONDOWN: {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            global_app_state.was_client_leftclicked = true;
            global_app_state.click_x = (s16)(message.lParam & 0xFFFF);
            global_app_state.click_y = (s16)(message.lParam >> 16);
        } break;
	}
}

bool process_input(HWND window) {
    global_app_state.was_client_leftclicked = false;
	bool allow_idling = false;

	HWND foreground_window = GetForegroundWindow();
	if (foreground_window == window) {
		allow_idling = false;
	}

	bool did_idle = false;
	MSG message;
	BOOL has_message = PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
	if (!has_message) {
		if (!allow_idling) {
			return false;
		} else {
			did_idle = true;
			BOOL ret = GetMessage(&message, window, 0, 0);
			if (ret == -1) {
				return false;
			}
		}
	}

	do {
		process_message(window, message);
	} while (PeekMessage(&message, 0, 0, 0, PM_REMOVE));
	return true;
}

#ifndef WITH_IMGUI
int main(int argc, char** argv) {
	app_state_t* app_state = &global_app_state;

	app_state->win32.instance = GetModuleHandle(NULL);
	app_state->win32.cursor = LoadCursor(NULL, IDC_ARROW);

	WNDCLASSA window_class;
	memset(&window_class, 0, sizeof(window_class));

	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = main_window_callback;
	window_class.hInstance = app_state->win32.instance;
	window_class.lpszClassName = "Rayverse";
	window_class.hbrBackground = NULL;

	if (!RegisterClass(&window_class)) {
		return EXIT_FAILURE;
	}

	RECT desired_window_rect;
	memset(&desired_window_rect, 0, sizeof(desired_window_rect));
	int desired_width = 640;
	int desired_height = 400;
	desired_window_rect.right = desired_width;
	desired_window_rect.bottom = desired_height;

	DWORD window_style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&desired_window_rect, window_style, FALSE);
	int initial_width = desired_window_rect.right - desired_window_rect.left;
	int initial_height = desired_window_rect.bottom - desired_window_rect.top;

	app_state->win32.window = CreateWindowEx(0, window_class.lpszClassName, "Rayverse", window_style, 0, 0, 
		initial_width, initial_height, NULL, NULL, app_state->win32.instance, 0);

	if (!app_state->win32.window) {
		return EXIT_FAILURE;
	}

	win32_init_opengl(app_state);


	// Initialize the offscreen buffer
	memset(&app_state->offscreen_surface, 0, sizeof(app_state->offscreen_surface));
	surface_resize(&app_state->offscreen_surface, desired_width, desired_height);

	memset(&app_state->game_surface, 0, sizeof(app_state->game_surface));
	surface_resize(&app_state->game_surface, 320, 200);
	app_state->active_surface = &app_state->game_surface;



	// Initialize timing-related stuff.
	LARGE_INTEGER perf_counter_frequency_result;
	QueryPerformanceFrequency(&perf_counter_frequency_result);
	performance_counter_frequency = perf_counter_frequency_result.QuadPart;
	// Make Sleep() more granular
	UINT desired_scheduler_granularity_ms = 1;
	bool sleep_is_granular = (timeBeginPeriod(desired_scheduler_granularity_ms) == TIMERR_NOERROR);

	HDC monitor_refresh_dc = GetDC(app_state->win32.window);
	s32 refresh_rate = GetDeviceCaps(monitor_refresh_dc, VREFRESH);
	ReleaseDC(app_state->win32.window, monitor_refresh_dc);

	if (refresh_rate > 1) {
		monitor_refresh_hz = refresh_rate;
	} else {
		monitor_refresh_hz = 60;
	}

	app_state->target_game_hz = 60;
	app_state->target_seconds_per_frame = 1.0f / (float)app_state->target_game_hz;


	// Initialize DirectSound.
	win32_sound_output_t* sound_output = &app_state->win32.sound_output;
	sound_output->samples_per_second = 44100;
	sound_output->bytes_per_sample = sizeof(s16) * 2;
	sound_output->secondary_buffer_size = sound_output->samples_per_second * sound_output->bytes_per_sample; // 1 second
	sound_output->safety_bytes = (u32)((float)(sound_output->samples_per_second * sound_output->bytes_per_sample) * app_state->target_seconds_per_frame * 0.3333f);
	win32_init_dsound(app_state->win32.window, sound_output);
	win32_clear_sound_buffer(sound_output);
	VFUNC(sound_output->secondary_buffer, Play) (SELF(sound_output->secondary_buffer) 0, 0, DSBPLAY_LOOPING);

	game_init_sound(&app_state->game.sound_buffer, (s32)sound_output->samples_per_second);

	if (!app_state->game.initialized) {
		game_init(&app_state->game);
	}

	ShowWindow(app_state->win32.window, SW_SHOW);
	SwapBuffers(wglGetCurrentDC()); // the very first frame, this may stall for a bit, so do this before entering the loop

	app_state->flip_clock = get_clock();

	global_app_state.running = true;
	return main_Ray(argc, argv); // run the game!
}
#endif //WITH_IMGUI

void win32_prepare_frame(app_state_t* app_state) {
	process_input(app_state->win32.window);

	glDrawBuffer(GL_BACK);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	win32_get_window_dimension(app_state->win32.window, &app_state->client_width, &app_state->client_height);
}

void win32_end_frame(app_state_t* app_state) {
	s64 frames_elapsed = 0;
	s64 clocks_per_tick = performance_counter_frequency / app_state->target_game_hz;
	while (frames_elapsed < 1) {
		frames_elapsed = ((get_clock() / clocks_per_tick) - (app_state->frame_clock / clocks_per_tick));
		Sleep(1);
	}
	app_state->frame_clock += clocks_per_tick * frames_elapsed;

	opengl_upload_surface(app_state, app_state->active_surface, app_state->client_width, app_state->client_height);
	win32_produce_sound_for_frame(app_state, &app_state->win32.sound_output, &app_state->game.sound_buffer, app_state->flip_clock);

	SwapBuffers(wglGetCurrentDC());
	app_state->flip_clock = get_clock();
}

void win32_advance_frame(app_state_t* app_state) {
	win32_prepare_frame(app_state);
	win32_end_frame(app_state);
	if (!app_state->running) {
		exit(0);
	}
}

void win32_input_func(void) {

}

