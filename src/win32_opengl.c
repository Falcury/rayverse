const char* wgl_extensions_string;

typedef const char *(WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (void);
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

bool win32_wgl_extension_supported(const char* extension_name) {
	if (wgl_extensions_string == NULL) {
		return false;
	}
	bool supported = (strstr(wgl_extensions_string, extension_name) != NULL);
	return supported;
}

bool win32_init_opengl(app_state_t* app_state) {
	HDC dc = GetDC(app_state->win32.window);

	PIXELFORMATDESCRIPTOR pixel_desc;
	memset(&pixel_desc, 0, sizeof(pixel_desc));
	pixel_desc.nSize = sizeof(pixel_desc);
	pixel_desc.nVersion = 1;
	pixel_desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixel_desc.iPixelType = PFD_TYPE_RGBA;
	pixel_desc.cColorBits = 32;
	pixel_desc.cRedBits = 8;
	pixel_desc.cRedShift = 16;
	pixel_desc.cGreenBits = 8;
	pixel_desc.cGreenShift = 8;
	pixel_desc.cBlueBits = 8;
	pixel_desc.cBlueShift = 0;
	pixel_desc.cAlphaBits = 0;
	pixel_desc.cAccumBits = 64;
	pixel_desc.cAccumRedBits = 16;
	pixel_desc.cAccumGreenBits = 16;
	pixel_desc.cAccumBlueBits = 16;
	pixel_desc.cAccumAlphaBits = 0;
	pixel_desc.cDepthBits = 32;
	pixel_desc.cStencilBits = 8;
	pixel_desc.cAuxBuffers = 0;
	pixel_desc.iLayerType = PFD_MAIN_PLANE;
	pixel_desc.bReserved = 0;
	pixel_desc.dwLayerMask = 0;
	pixel_desc.dwVisibleMask = 0;
	pixel_desc.dwDamageMask = 0;

	int pixel_index = ChoosePixelFormat(dc, &pixel_desc);
	if (pixel_index == 0) {
		pixel_index = 1;
		if (!DescribePixelFormat(dc, pixel_index, sizeof(pixel_desc), &pixel_desc)) {
			return EXIT_FAILURE;
		}
	}

	if (!SetPixelFormat(dc, pixel_index, &pixel_desc)) {
		return EXIT_FAILURE;
	}

	HGLRC glrc = wglCreateContext(dc);
	if (!glrc) {
		return EXIT_FAILURE;
	}

	if (!wglMakeCurrent(dc, glrc)) {
		return EXIT_FAILURE;
	}

	const char* version_string = (const char*) glGetString(GL_VERSION);
	//puts(version_string);

	const char* extensions_string = (const char*) glGetString(GL_EXTENSIONS);
	//puts(extensions_string);

#define GET_WGL_PROC(proc, type) do { proc = (type) wglGetProcAddress(#proc); } while(0)

	GET_WGL_PROC(wglSwapIntervalEXT, PFNWGLSWAPINTERVALEXTPROC);
	
	GET_WGL_PROC(wglGetExtensionsStringARB, PFNWGLGETEXTENSIONSSTRINGARBPROC);
	if (wglGetExtensionsStringARB) {
		GET_WGL_PROC(wglSwapIntervalEXT, PFNWGLSWAPINTERVALEXTPROC);
		if (wglSwapIntervalEXT) {
			wglSwapIntervalEXT(1);
//			printf("enabling vsync\n");
			app_state->vsync_enabled = true;
		}
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &app_state->opengl.max_texture_size);

	glEnable(GL_TEXTURE_2D); // is this needed?
	glGenTextures(1, &app_state->opengl.screen_texture);
	glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);

	ReleaseDC(app_state->win32.window, dc);

	return true;
}


void surface_clear(surface_t* surface) {
	memset(surface->memory, 0, surface->memory_size);
}

void surface_resize(surface_t* surface, int width, int height, bool use_power_of_2_texture) {
	surface->width = width;
	surface->height = height;
	surface->bytes_per_pixel = 4;
    // NOTE: we use OpenGL textures with a power-of-2 size here. This is for compatibility with old GPUs.
    // (On Windows, you might want to run on old GPUs paired with Win9x.)
    if (use_power_of_2_texture) {
        if (is_power_of_2((u32)width)) {
            surface->width_pow2 = width;
        } else {
            surface->width_pow2 = next_power_of_2((u32)width);
        }
        if (is_power_of_2((u32)height)) {
            surface->height_pow2 = height;
        } else {
            surface->height_pow2 = next_power_of_2((u32)height);
        }
    } else {
        surface->width_pow2 = width;
        surface->height_pow2 = height;
    }

	surface->pitch = surface->width_pow2 * surface->bytes_per_pixel;

	u32 memory_needed = surface->width_pow2 * surface->height_pow2 * surface->bytes_per_pixel;
	if (memory_needed > surface->memory_size) {
		u8* new_ptr = (u8*) realloc(surface->memory, memory_needed);
		if (!new_ptr) {
			// failed
			exit(1);
		}
		surface->memory = new_ptr;
		surface->memory_size = memory_needed;
	}
	surface_clear(surface);
}

void opengl_upload_surface(app_state_t* app_state, surface_t* surface, int client_width, int client_height) {
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, client_width, client_height);
//	glViewport(0, 0, surface->width, surface->height);
//	glViewport(0, 0, surface->width_pow2, surface->height_pow2);

	glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width_pow2, surface->height_pow2, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, surface->memory);

	GLenum error = glGetError();
	if (error) {
		const GLubyte* error_string = gluErrorString(error);
		printf("OpenGL error 0x%x: %s\n", error, error_string);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);

	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	float scale_x = (float)surface->width_pow2 / (float)surface->width;
	float scale_y = (float)surface->height_pow2 / (float)surface->height;
	float t_x = scale_x - 1.0f;
	float t_y = scale_y - 1.0f;
	float projection_matrix[] = {
			1.0f + t_x, 0.0f, 0.0f, 0.0f,
			0.0f, -1.0f - t_y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			t_x, -t_y, 0.0f, 1.0f,
	};
	glLoadMatrixf(projection_matrix);

	glBegin(GL_TRIANGLES);
	float p = 1.0f;

	// lower triangle
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-p, -p);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(p, -p);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(p, p);

	// upper triangle
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-p, -p);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(p, p);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-p, p);

	glEnd();

}
