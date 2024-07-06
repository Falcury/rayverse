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
#include "event.cpp"




bool global_running;
app_state_t global_app_state;
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
		global_running = false;
				   } break;
	case WM_DESTROY: {
		global_running = false;
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
		global_running = false;
	}

	switch(message.message) {
		default: {
//			printf("Window message: %x\n", message.message);
			TranslateMessage(&message);
			DispatchMessage(&message);
		} break;
		case WM_ERASEBKGND:
		case WM_MOUSEMOVE: break;
	}
}

bool process_input(HWND window) {
	bool allow_idling = true;

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

	app_state->target_game_hz = 60.0f;
	app_state->target_seconds_per_frame = 1.0f / app_state->target_game_hz;


	// Initialize DirectSound.
	win32_sound_output_t sound = {0};
	sound.samples_per_second = 44100;
	sound.bytes_per_sample = sizeof(i16) * 2;
	sound.secondary_buffer_size = sound.samples_per_second * sound.bytes_per_sample; // 1 second
	sound.safety_bytes = (u32)((float)(sound.samples_per_second * sound.bytes_per_sample) * app_state->target_seconds_per_frame * 0.3333f);
	win32_init_dsound(app_state->win32.window, &sound);
	win32_clear_sound_buffer(&sound);
	sound.secondary_buffer->Play(0, 0, DSBPLAY_LOOPING);

	// Prepare a sound buffer for the game code to write into.
	game_sound_buffer_t game_sound_buffer = {0};
	game_sound_buffer.samples_per_second = sound.samples_per_second;
	game_sound_buffer.samples = (i16*)calloc(1, game_sound_buffer.samples_per_second * 2 * sizeof(i16));


	if (!app_state->game.initialized) {
		game_init(&app_state->game);
	}

	ShowWindow(app_state->win32.window, SW_SHOW);
	SwapBuffers(wglGetCurrentDC()); // the very first frame, this may stall for a bit, so do this before entering the loop

	i64 flip_clock = get_clock();

	global_running = true;
	while(global_running) {
		process_input(app_state->win32.window);

		glDrawBuffer(GL_BACK);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

		i32 client_width, client_height;
		win32_get_window_dimension(app_state->win32.window, &client_width, &client_height);
		game_update_and_render(app_state);
		opengl_upload_surface(app_state, app_state->active_surface, client_width, client_height);
		win32_produce_sound_for_frame(app_state, &sound, &game_sound_buffer, flip_clock);

		SwapBuffers(wglGetCurrentDC());
		flip_clock = get_clock();

		//Sleep(1000);
	}

	printf("Hello World!\n");
	return 0;
}

