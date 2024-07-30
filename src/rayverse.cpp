// rayverse.cpp : Defines the entry point for the console application.
#include "StdAfx.h"
//
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _MSC_VER
#if _MSC_VER < 1900
#define snprintf _snprintf
#endif
#endif

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include "common.h"
#include "types.h"
#include "proto.h"

#include "data.cpp"

#include "intrinsics.cpp"
#include "mathutil.cpp"
#include "platform.cpp"
#include "win32_opengl.cpp"
#include "win32_sound.cpp"
#include "render.cpp"

#include "misc_source.cpp"
#include "sound.cpp"
#include "save.cpp"
#include "game.cpp"
#include "engine.cpp"
#include "object.cpp"





i64 performance_counter_frequency;
i32 monitor_refresh_hz;

i64 get_clock() {
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result.QuadPart;
}

float get_seconds_elapsed(i64 start, i64 end) {
	return (float)(end - start) / (float)performance_counter_frequency;
}

WINDOWPLACEMENT window_position = { sizeof(window_position) };
void toggle_fullscreen(HWND window) {
	LONG style = GetWindowLong(window, GWL_STYLE);
	if (style & WS_OVERLAPPEDWINDOW) {
		MONITORINFO monitor_info = {};
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
	} else {
		SetWindowLong(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(window, &window_position);
		SetWindowPos(window, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
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
	case WM_CLOSE: {
		global_app_state.running = false;
	} break;
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
	//case WM_ERASEBKGND: {
	//	result = TRUE; // prevent flickering
//						} break;
	/*case WM_PAINT: {
		result = TRUE;
				   } break;*/
	}


	return result;
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
//			u32 hid_code = keycode_windows_to_hid(scancode);
//			if (vk_code == VK_SPACE) {
//				hid_code = KEY_Space; // NOTE: for some reason, Space is missing from the table in keycode_windows_to_hid()
//			}
			bool32 alt_down = message.lParam & (1 << 29);
			bool32 is_down = ((message.lParam & (1 << 31)) == 0);
			bool32 was_down = ((message.lParam & (1 << 30)) != 0);
			int repeat_count = message.lParam & 0xFFFF;
			i16 ctrl_state = GetKeyState(VK_CONTROL);
			bool32 ctrl_down = (ctrl_state < 0); // 'down' determined by high order bit == sign bit
			if (was_down && is_down) break; // uninteresting: repeated key

			switch (vk_code) {
				default: break;
				case VK_F4: {
					if (is_down && alt_down) {
						global_app_state.running = false;
					}
				} break;
				case VK_F11: {
					if (is_down && message.hwnd && !alt_down) {
						toggle_fullscreen(message.hwnd);
					}
				} break;
				case VK_RETURN: {
					if (is_down && message.hwnd && alt_down) {
						toggle_fullscreen(message.hwnd);
					}
				} break;
			}
		}
	}
}

bool process_input(HWND window) {
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


int main(int argc, char* argv[])
{
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
	bool32 sleep_is_granular = (timeBeginPeriod(desired_scheduler_granularity_ms) == TIMERR_NOERROR);

	HDC monitor_refresh_dc = GetDC(app_state->win32.window);
	i32 refresh_rate = GetDeviceCaps(monitor_refresh_dc, VREFRESH);
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
	sound_output->bytes_per_sample = sizeof(i16) * 2;
	sound_output->secondary_buffer_size = sound_output->samples_per_second * sound_output->bytes_per_sample; // 1 second
	sound_output->safety_bytes = (u32)((float)(sound_output->samples_per_second * sound_output->bytes_per_sample) * app_state->target_seconds_per_frame * 0.3333f);
	win32_init_dsound(app_state->win32.window, sound_output);
	win32_clear_sound_buffer(sound_output);
	sound_output->secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);

	game_init_sound(&app_state->game.sound_buffer, (i32)sound_output->samples_per_second);


	if (!app_state->game.initialized) {
		game_init(&app_state->game);
	}

	ShowWindow(app_state->win32.window, SW_SHOW);
	SwapBuffers(wglGetCurrentDC()); // the very first frame, this may stall for a bit, so do this before entering the loop

	app_state->flip_clock = get_clock();

	global_app_state.running = true;
	rayman_main();
	while(app_state->running) {
		win32_prepare_frame(app_state);
		game_update_and_render(app_state);
		win32_end_frame(app_state);
	}

	printf("Hello World!\n");
	return 0;
}

void win32_prepare_frame(app_state_t* app_state) {
	process_input(app_state->win32.window);

	glDrawBuffer(GL_BACK);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	win32_get_window_dimension(app_state->win32.window, &app_state->client_width, &app_state->client_height);
}

void win32_end_frame(app_state_t* app_state) {
	opengl_upload_surface(app_state, app_state->active_surface, app_state->client_width, app_state->client_height);
	win32_produce_sound_for_frame(app_state, &app_state->win32.sound_output, &app_state->game.sound_buffer, app_state->flip_clock);

	SwapBuffers(wglGetCurrentDC());
	app_state->flip_clock = get_clock();

	//Sleep(1000);
}

void win32_advance_frame(app_state_t* app_state) {
	win32_prepare_frame(app_state);
	win32_end_frame(app_state);
	if (!app_state->running) {
		exit(0);
	}
}

