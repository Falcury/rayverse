
typedef struct basic_shader_t {
    u32 program;
    s32 u_texture0;
    s32 attrib_location_pos;
    s32 attrib_location_tex_coord;
} basic_shader_t;

basic_shader_t basic_shader;
static u32 vbo_screen;
static u32 vao_screen;

static char vertex_shader_source[] =
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec2 pos;\n"
        "layout (location = 1) in vec2 tex_coord;\n"
        "\n"
        "out VS_OUT {\n"
        "    vec2 tex_coord;\n"
        "} vs_out;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = vec4(pos.x, -pos.y, 0.0f, 1.0f);\n"
        "    vs_out.tex_coord = tex_coord;\n"
        "}";

static char fragment_shader_source[] =
        "#version 330 core\n"
        "\n"
        "in VS_OUT {\n"
        "    vec2 tex_coord;\n"
        "} fs_in;\n"
        "\n"
        "uniform sampler2D texture0;\n"
        "uniform float t;\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "    fragColor = texture(texture0, fs_in.tex_coord);\n"
        "}";

void load_shader(u32 shader, const char* shader_source) {
    const char* sources[] = { shader_source, };
    glShaderSource(shader, COUNT(sources), sources, NULL);
    glCompileShader(shader);

    s32 success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) {
//		printf("Loaded %sshader: %s\n", source_from_file ? "" : "cached ", source_filename);
    } else {
        char info_log[2048];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        printf("Error: compilation of shader %d failed:\n%s", shader, info_log);
        printf("Shader source: %s\n", shader_source);
    }
}

u32 load_basic_shader_program(const char* vert_source, const char* frag_source) {
    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    load_shader(vertex_shader, vert_source);
    load_shader(fragment_shader, frag_source);

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    {
        s32 success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[2048];
            glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
            printf("Error: shader linking failed: %s", info_log);
            fatal_error();
        }
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

s32 get_attrib(u32 program, const char *name) {
    s32 attribute = glGetAttribLocation(program, name);
    if (attribute == -1)
        printf("Could not get attribute location %s\n", name);
    return attribute;
}

s32 get_uniform(u32 program, const char *name) {
    s32 uniform = glGetUniformLocation(program, name);
    if (uniform == -1)
        printf("Could not get uniform location %s\n", name);
    return uniform;
}

void init_draw_normalized_quad() {
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    glGenVertexArrays(1, &vao_screen);
    glBindVertexArray(vao_screen);

    glGenBuffers(1, &vbo_screen);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_screen);

    static float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, // x, y, (z = 0,)  u, v
            +1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, +1.0f, 0.0f, 1.0f,

            -1.0f, +1.0f, 0.0f, 1.0f,
            +1.0f, -1.0f, 1.0f, 0.0f,
            +1.0f, +1.0f, 1.0f, 1.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLsizei vertex_stride = 4 * sizeof(float);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)0); // position coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(2*sizeof(float))); // texture coordinates
    glEnableVertexAttribArray(1);
}

bool linux_init_opengl(app_state_t* app_state) {

    SDL_GLContext gl_context = SDL_GL_CreateContext(app_state->sdl.window);
    SDL_GL_MakeCurrent(app_state->sdl.window, gl_context);

//    char* version_string = (char*)glGetString(GL_VERSION);
//    printf("OpenGL supported version: %s\n", version_string);

    SDL_GL_SetSwapInterval(1); // Enable vsync
    app_state->vsync_enabled = true;

#ifndef __APPLE__
    bool err = glewInit() != GLEW_OK;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        fatal_error();
    }
#endif

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &app_state->opengl.max_texture_size);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &app_state->opengl.screen_texture);
	glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // load the shader that renders the surface to the screen
    basic_shader.program = load_basic_shader_program(vertex_shader_source, fragment_shader_source);
    basic_shader.u_texture0 = get_uniform(basic_shader.program, "texture0");
    basic_shader.attrib_location_pos = get_attrib(basic_shader.program, "pos");
    basic_shader.attrib_location_tex_coord = get_attrib(basic_shader.program, "tex_coord");

    glUseProgram(basic_shader.program);
    glUniform1i(basic_shader.u_texture0, 0);

    init_draw_normalized_quad();

	return true;
}

void surface_clear(surface_t* surface) {
	memset(surface->memory, 0, surface->memory_size);
}

void surface_resize(surface_t* surface, int width, int height) {
	surface->width = width;
	surface->height = height;
	surface->bytes_per_pixel = 4;
    // NOTE: on Windows, we use OpenGL textures with a power-of-2 size here. This is for compatibility with old GPUs.
    // (On Windows, you might want to run on old GPUs paired with Win9x.)
    // However, on platforms other than Windows there is not really a need to deal with this restriction.
    surface->width_pow2 = width;
    surface->height_pow2 = height;
	surface->pitch = surface->width * surface->bytes_per_pixel;
	u32 memory_needed = surface->width * surface->height * surface->bytes_per_pixel;
	if (memory_needed > surface->memory_size) {
		u8* new_ptr = (u8*) realloc(surface->memory, memory_needed);
		if (!new_ptr) {
			fatal_error();
		}
		surface->memory = new_ptr;
		surface->memory_size = memory_needed;
	}
	surface_clear(surface);
}

void opengl_upload_surface(app_state_t* app_state, surface_t* surface, int client_width, int client_height) {
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, client_width, client_height);

    glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width_pow2, surface->height_pow2, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->memory);

    glUseProgram(basic_shader.program);
    glBindVertexArray(vao_screen);
    glDisable(GL_DEPTH_TEST); // because we want to make sure the quad always renders in front of everything else
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
