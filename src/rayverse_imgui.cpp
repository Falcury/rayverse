// Dear ImGui: standalone example application for Windows API + OpenGL

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "glad.h"
#include <tchar.h>

#include <mmsystem.h> // needed for dsound.h
#include <dsound.h> // needed for IDirectSound and IDirectSoundBuffer in types.h

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c" // needed for ogg_t in types.h

#include "common.h" // basic macros and types
#include "types.h"  // Rayverse type definitions
#include "proto.h"  // Rayverse function prototypes
#include "data.c"   // Rayverse global variables

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Support function for multi-viewports
// Unlike most other backend combination, we need specific hooks to combine Win32+OpenGL.
// We could in theory decide to support Win32-specific code in OpenGL backend via e.g. an hypothetical ImGui_ImplOpenGL3_InitForRawWin32().
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport)
{
    assert(viewport->RendererUserData == NULL);

    WGL_WindowData* data = IM_NEW(WGL_WindowData);
    CreateDeviceWGL((HWND)viewport->PlatformHandle, data);
    viewport->RendererUserData = data;
}

static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport)
{
    if (viewport->RendererUserData != NULL)
    {
        WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData;
        CleanupDeviceWGL((HWND)viewport->PlatformHandle, data);
        IM_DELETE(data);
        viewport->RendererUserData = NULL;
    }
}

static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*)
{
    // Activate the platform window DC in the OpenGL rendering context
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, g_hRC);
}

static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*)
{
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}

//TODO: refactor/move this part to another file?
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

void rayverse_imgui_init_opengl(app_state_t* app_state) {
    // load the shader that renders the surface to the screen
    basic_shader.program = load_basic_shader_program(vertex_shader_source, fragment_shader_source);
    basic_shader.u_texture0 = get_uniform(basic_shader.program, "texture0");
    basic_shader.attrib_location_pos = get_attrib(basic_shader.program, "pos");
    basic_shader.attrib_location_tex_coord = get_attrib(basic_shader.program, "tex_coord");

    glUseProgram(basic_shader.program);
    glUniform1i(basic_shader.u_texture0, 0);

    init_draw_normalized_quad();
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

}


extern "C" void win32_prepare_frame(app_state_t* app_state) {
    global_app_state.was_client_leftclicked = false;
    // Poll and handle messages (inputs, window resize, etc.)
    // See the WndProc() function below for our to dispatch events to the Win32 backend.
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            app_state->running = false;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

//    ImGui::DockSpaceOverViewport(); //TODO




    glDrawBuffer(GL_BACK);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    win32_get_window_dimension(app_state->win32.window, &app_state->client_width, &app_state->client_height);

    s64 frames_elapsed = 0;
    s64 clocks_per_tick = performance_counter_frequency / app_state->target_game_hz;
    while (frames_elapsed < 1) {
        frames_elapsed = ((get_clock() / clocks_per_tick) - (app_state->frame_clock / clocks_per_tick));
        Sleep(1);
    }
    app_state->frame_clock += clocks_per_tick * frames_elapsed;

//    opengl_upload_surface(app_state, app_state->active_surface, app_state->client_width, app_state->client_height); //TODO
    // Upload surface
    surface_t* surface = app_state->active_surface;
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, global_app_state.client_width, global_app_state.client_height);

    glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width_pow2, surface->height_pow2, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->memory);
/*
    glUseProgram(basic_shader.program);
    glBindVertexArray(vao_screen);
    glDisable(GL_DEPTH_TEST); // because we want to make sure the quad always renders in front of everything else
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);*/


    win32_produce_sound_for_frame(app_state, &app_state->win32.sound_output, &app_state->game.sound_buffer, app_state->flip_clock);

    ImGui::DockSpaceOverViewport();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    if (show_game_window) {
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(640, 400), ImGuiCond_FirstUseEver);

        ImGui::Begin("Game", NULL, ImGuiWindowFlags_NoScrollbar);
        float desired_aspect_ratio = 320.0f / 200.0f;
        float width = ImGui::GetWindowWidth();
        float height = ImGui::GetWindowHeight();
        float aspect_ratio = width / height;
        if (aspect_ratio != desired_aspect_ratio) {
            if (aspect_ratio > desired_aspect_ratio) {
                width = height * desired_aspect_ratio;
            } else {
                height = width / desired_aspect_ratio;
            }
        }
        ImGui::Image((ImTextureID)(intptr_t)app_state->opengl.screen_texture, ImVec2(width, height));
        ImGui::End();
    }


    // Rendering
    ImGui::Render();
    glViewport(0, 0, global_app_state.client_width, global_app_state.client_height);
//    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
        wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    }

    // Present
    ::SwapBuffers(g_MainWindow.hDC);
    app_state->flip_clock = get_clock();
}

extern "C" void win32_advance_frame(app_state_t* app_state) {
    win32_prepare_frame(app_state);
    if (!app_state->running) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceWGL(app_state->win32.window, &g_MainWindow);
        wglDeleteContext(g_hRC);
        ::DestroyWindow(app_state->win32.window);
//        ::UnregisterClassW(app_state->win32..lpszClassName, wc.hInstance);
        exit(0);
    }
}

// Main code
int main(int argc, char** argv)
{
    // Make process DPI aware and obtain main monitor scale
    ::SetProcessDPIAware();
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Rayverse", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Rayverse", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);
    global_app_state.win32.window = hwnd;

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        IM_ASSERT(platform_io.Renderer_CreateWindow == NULL);
        IM_ASSERT(platform_io.Renderer_DestroyWindow == NULL);
        IM_ASSERT(platform_io.Renderer_SwapBuffers == NULL);
        IM_ASSERT(platform_io.Platform_RenderWindow == NULL);
        platform_io.Renderer_CreateWindow = Hook_Renderer_CreateWindow;
        platform_io.Renderer_DestroyWindow = Hook_Renderer_DestroyWindow;
        platform_io.Renderer_SwapBuffers = Hook_Renderer_SwapBuffers;
        platform_io.Platform_RenderWindow = Hook_Platform_RenderWindow;
    }

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will call AddFontDefault() to select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    style.FontSizeBase = 12.0f;
    io.Fonts->AddFontDefaultVector();
//    io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Initialize Rayverse
    win32_init_game();
    rayverse_imgui_init_opengl(&global_app_state);

    return main_Ray(argc, argv);
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            global_app_state.client_width = LOWORD(lParam);
            global_app_state.client_height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return win32_main_window_callback(hWnd, msg, wParam, lParam);
}
