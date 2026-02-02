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

void show_game_window(bool* p_open)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 400), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Game", p_open, ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::End();
        return;
    }

    float desired_aspect_ratio = 320.0f / 200.0f;
    const ImVec2 size_avail = ImGui::GetContentRegionAvail();
    float width = size_avail.x;
    float height = size_avail.y;
    float aspect_ratio = width / height;
    if (aspect_ratio != desired_aspect_ratio) {
        if (aspect_ratio > desired_aspect_ratio) {
            width = height * desired_aspect_ratio;
        } else {
            height = width / desired_aspect_ratio;
        }
    }
    ImGui::Image((ImTextureID)(intptr_t)global_app_state.opengl.screen_texture, ImVec2(width, height));

    ImGui::End();
}

void show_rendering_info_window(bool* p_open)
{
    if (!ImGui::Begin("Rendering Info", p_open))
    {
        ImGui::End();
        return;
    }

    // TODO: Add fields

    ImGui::End();
}

void show_sounds_window(bool* p_open)
{
    if (!ImGui::Begin("Sounds", p_open))
    {
        ImGui::End();
        return;
    }

    // TODO: Add fields. Show currently playing sounds, sound table, loaded banks etc.

    ImGui::End();
}

void show_game_info_window(bool* p_open)
{
    if (!ImGui::Begin("Game Info", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::SeparatorText("World Info");

    if (ImGui::BeginTable("world_info", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
    {
        // Set headers
        ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Unlocked", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Unlocking", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Cages", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        // Set the rows
        for (s32 i = 0; i < LEN(t_world_info); i++)
        {
            ImGui::TableNextRow();
            ImGui::PushID(i);

            ImGui::TableNextColumn();
            ImGui::Text("%d", i);

            ImGui::TableNextColumn();
            ImGui::Text("%d-%d", t_world_info[i].world, t_world_info[i].level);

            ImGui::TableNextColumn();
            if (t_world_info[i].text)
                ImGui::Text(t_world_info[i].text);

            ImGui::TableNextColumn();
            bool unlocked = t_world_info[i].state & 1;
            if (ImGui::Checkbox("##unlocked", &unlocked))
            {
                if (unlocked)
                    t_world_info[i].state |= 1;
                else
                    t_world_info[i].state &= ~1;
            }

            ImGui::TableNextColumn();
            bool path = t_world_info[i].state & 2;
            if (ImGui::Checkbox("##path", &path))
            {
                if (path)
                    t_world_info[i].state |= 2;
                else
                    t_world_info[i].state &= ~2;
            }

            ImGui::TableNextColumn();
            bool unlocking = t_world_info[i].state & 4;
            if (ImGui::Checkbox("##unlocking", &unlocking))
            {
                if (unlocking)
                    t_world_info[i].state |= 4;
                else
                    t_world_info[i].state &= ~4;
            }

            ImGui::TableNextColumn();
            ImGui::Text("%d", t_world_info[i].nb_cages);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    if (ImGui::Button("Unlock all"))
    {
        for (s32 i = 0; i < LEN(t_world_info); ++i)
        {
            if ((t_world_info[i].state & 1) == 0)
            {
                t_world_info[i].state |= 4;
                chemin_percent = 0;
            }
        }
    }

    ImGui::SameLine();
    ImGui::BeginDisabled(!dans_la_map_monde);
    if (ImGui::Button("All cages"))
    {
        for (s32 i = 0; i < COUNT(t_world_info); ++i) {
            world_info_t* wi = t_world_info + i;
            wi->nb_cages = 6;
            wi->state = 1;
            DISPLAY_PTS_WAY();
            set_sub_etat(mapobj + i, 46 + wi->nb_cages);
        }
    }
    ImGui::EndDisabled();

    ImGui::Text("chemin_percent: %d", chemin_percent);

    ImGui::End();
}

void show_objects_table(bool is_always, float height)
{
    if (ImGui::BeginTable("objects_table", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY, ImVec2(0.0f, height)))
    {
        // Set headers
        ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Hp", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Speed", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Cmd", ImGuiTableColumnFlags_WidthFixed, 90);
        ImGui::TableHeadersRow();

        // Set row for each object
        for (s32 i = 0; i < level.nb_objects; i++)
        {
            obj_t* obj = &level.objects[i];

            if ((!is_always && flags[obj->type] & flags0_1_always) ||
                (is_always && !(flags[level.objects[i].type] & flags0_1_always)))
                continue;

            ImGui::TableNextRow();
            ImGui::PushID(i);

            // Color based on if active/inactive/dead
            if (level.objects[i].is_active)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(10, 180, 10, 255));
            }
            else if (level.objects[i].flags.alive)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 10, 255));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 10, 10, 255));
            }

            ImGui::TableNextColumn();

            char objIdText[4];
            sprintf(objIdText, "%d", level.objects[i].id);

            bool selected = imgui_selected_object_id == i;
            if (ImGui::Selectable(objIdText, &selected, ImGuiSelectableFlags_SpanAllColumns))
                imgui_selected_object_id = i;

            ImGui::TableNextColumn();
            ImGui::Text(imgui_obj_type_names[level.objects[i].type]);

            ImGui::TableNextColumn();
            ImGui::Text("%d-%d", level.objects[i].main_etat, level.objects[i].sub_etat);

            ImGui::TableNextColumn();
            ImGui::Text("%d/%d", (u8)level.objects[i].hit_points, (u8)level.objects[i].init_hit_points);

            ImGui::TableNextColumn();
            ImGui::Text("(%d, %d)", level.objects[i].speed_x, level.objects[i].speed_y);

            ImGui::TableNextColumn();
            ImGui::Text(imgui_cmd_names[level.objects[i].cmd]);

            ImGui::PopStyleColor();
            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void show_level_window(bool* p_open)
{
    if (!ImGui::Begin("Level", p_open))
    {
        ImGui::End();
        return;
    }

    if (RaymanDansUneMapDuJeu)
    {
        if (ImGui::BeginTabBar("##level_window_tabs"))
        {
            if (ImGui::BeginTabItem("Objects"))
            {
                ImGui::SeparatorText("Options");

                ImGui::BeginDisabled(imgui_selected_object_id == -2);
                if (ImGui::Button("Deselect"))
                    imgui_selected_object_id = -2;
                ImGui::EndDisabled();

                ImGui::SameLine();
                ImGui::BeginDisabled(imgui_selected_object_id == -1);
                if (ImGui::Button("Select Rayman"))
                    imgui_selected_object_id = -1;
                ImGui::EndDisabled();

                if (level.objects)
                {
                    ImGui::Spacing();
                    ImGui::SeparatorText("Normal Objects");
                    ImVec2 availSize = ImGui::GetContentRegionAvail();
                    show_objects_table(false, availSize.y * (2.0f / 3.0f));

                    ImGui::Spacing();
                    ImGui::SeparatorText("Always Objects");
                    availSize = ImGui::GetContentRegionAvail();
                    show_objects_table(true, availSize.y);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Map"))
            {
                ImGui::SeparatorText("Big Map");

                ImGui::Text("Map Size: %d x %d", BIG_MAP.width, BIG_MAP.height);
                ImGui::Text("Map Tile Pos: (%d, %d)", BIG_MAP.display_map->x, BIG_MAP.display_map->y);
                ImGui::Text("Map Tile Index: %d", BIG_MAP.display_map->tile_index);

                ImGui::Spacing();
                ImGui::SeparatorText("Scroll");

                ImGui::Checkbox("Scroll Locked", (bool*)&scrollLocked);

                s32 pos[2] = { xmap, ymap };
                if (ImGui::InputInt2("Position", pos))
                {
                    xmap = pos[0];
                    ymap = pos[1];
                }

                pos[0] = scroll_x;
                pos[1] = scroll_y;
                if (ImGui::InputInt2("Scroll", pos))
                {
                    scroll_x = pos[0];
                    scroll_y = pos[1];
                }

                pos[0] = scroll_start_x;
                pos[1] = scroll_start_y;
                if (ImGui::InputInt2("Scroll Start", pos))
                {
                    scroll_start_x = pos[0];
                    scroll_start_y = pos[1];
                }

                pos[0] = scroll_end_x;
                pos[1] = scroll_end_y;
                if (ImGui::InputInt2("Scroll End", pos))
                {
                    scroll_end_x = pos[0];
                    scroll_end_y = pos[1];
                }

                pos[0] = h_scroll_speed;
                pos[1] = v_scroll_speed;
                if (ImGui::InputInt2("Scroll Speed", pos))
                {
                    h_scroll_speed = pos[0];
                    v_scroll_speed = pos[1];
                }

                ImGui::Spacing();
                ImGui::SeparatorText("Global");

                ImGui::Text("Map Time: %d", map_time);

                for (s32 i = 0; i < 3; ++i)
                {
                    if (i != 0)
                        ImGui::SameLine();
                    char palName[10];
                    sprintf(palName, "Palette %d", i);

                    if (ImGui::RadioButton(palName, current_pal_id == i))
                    {
                        actualize_palette(i);
                    }
                }

                // TODO: Add more global values, like bonus map, level effects (snow, shake etc.) and more

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Background"))
            {
                ImGui::SeparatorText("Background");

                ImGui::InputScalar("Background", ImGuiDataType_U8, &no_fnd);
                ImGui::InputScalar("Background Type", ImGuiDataType_U8, &type_fnd);
                ImGui::InputScalar("Background Scroll X Type", ImGuiDataType_U8, &type_fndscrX);
                ImGui::InputScalar("Sprites", ImGuiDataType_S16, &NbSprite);
                ImGui::InputScalar("Bands", ImGuiDataType_S16, &NbBande);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    else
    {
        ImGui::Text("Not currently in a map");
    }

    ImGui::End();
}

void show_object_properties_window(bool* p_open)
{
    if (!ImGui::Begin("Object Properties", p_open))
    {
        ImGui::End();
        return;
    }

    if (RaymanDansUneMapDuJeu && level.objects && imgui_selected_object_id < level.nb_objects && imgui_selected_object_id != -2)
    {
        obj_t* obj;

        if (imgui_selected_object_id == -1)
            obj = &ray;
        else
            obj = &level.objects[imgui_selected_object_id];

        if (ImGui::BeginTabBar("##object_properties_window_tabs"))
        {
            if (ImGui::BeginTabItem("Fields"))
            {
                ImGui::InputScalar("Field 1C", ImGuiDataType_U32, &obj->field_1C);
                ImGui::Checkbox("Link Has Gendoor", (bool*)&obj->link_has_gendoor);
                ImGui::Checkbox("Is Active", (bool*)&obj->is_active);

                s32 pos[2] = { obj->x, obj->y };
                if (ImGui::InputInt2("Position", pos))
                {
                    obj->x = pos[0];
                    obj->y = pos[1];
                }

                ImGui::InputScalar("Active Flag", ImGuiDataType_S32, &obj->active_flag);

                pos[0] = obj->screen_x;
                pos[1] = obj->screen_y;
                if (ImGui::InputInt2("Screen Position", pos))
                {
                    obj->screen_x = pos[0];
                    obj->screen_y = pos[1];
                }

                ImGui::InputScalar("Sub Speed", ImGuiDataType_S16, &obj->sub_speed);

                pos[0] = obj->init_x;
                pos[1] = obj->init_y;
                if (ImGui::InputInt2("Initial Position", pos))
                {
                    obj->init_x = pos[0];
                    obj->init_y = pos[1];
                }

                pos[0] = obj->speed_x;
                pos[1] = obj->speed_y;
                if (ImGui::InputInt2("Speed", pos))
                {
                    obj->speed_x = pos[0];
                    obj->speed_y = pos[1];
                }

                ImGui::InputScalar("Cmd Offset", ImGuiDataType_S16, &obj->cmd_offset);
                ImGui::InputScalar("Nb Cmd", ImGuiDataType_S16, &obj->nb_cmd);
                ImGui::InputScalar("Follow Id", ImGuiDataType_S16, &obj->follow_id);
                ImGui::InputScalar("Follow Y", ImGuiDataType_S16, &obj->follow_y);
                ImGui::InputScalar("Follow X", ImGuiDataType_S16, &obj->follow_x);
                ImGui::InputScalar("Param", ImGuiDataType_S16, &obj->param);
                ImGui::InputScalar("Link", ImGuiDataType_S16, &obj->link);
                ImGui::InputScalar("Ray Dist", ImGuiDataType_S16, &obj->ray_dist);
                ImGui::InputScalar("IFrames Timer", ImGuiDataType_S16, &obj->iframes_timer);
                ImGui::InputScalar("Test Block Index", ImGuiDataType_U16, &obj->test_block_index);
                ImGui::InputScalar("Scale", ImGuiDataType_U16, &obj->scale);
                ImGui::InputScalar("Active Timer", ImGuiDataType_U16, &obj->active_timer);

                if (ImGui::BeginCombo("Type", imgui_obj_type_names[obj->type], ImGuiComboFlags_HeightLargest))
                {
                    for (u16 i = 0; i < 262; i++)
                    {
                        bool is_selected = obj->type == i;
                        if (ImGui::Selectable(imgui_obj_type_names[i], is_selected))
                            obj->type = i;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                // TODO: Show block names
                ImGui::InputScalar("Block Center", ImGuiDataType_U8, &obj->btypes[0]);
                ImGui::InputScalar("Block Left", ImGuiDataType_U8, &obj->btypes[1]);
                ImGui::InputScalar("Block Right", ImGuiDataType_U8, &obj->btypes[2]);
                ImGui::InputScalar("Block Up", ImGuiDataType_U8, &obj->btypes[3]);
                ImGui::InputScalar("Block Down", ImGuiDataType_U8, &obj->btypes[4]);

                pos[0] = obj->offset_bx;
                pos[1] = obj->offset_by;
                if (ImGui::InputInt2("Offset", pos))
                {
                    obj->offset_bx = pos[0];
                    obj->offset_by = pos[1];
                }

                ImGui::InputScalar("Animation Index", ImGuiDataType_U8, &obj->anim_index);
                ImGui::InputScalar("Animation Frame", ImGuiDataType_U8, &obj->anim_frame);
                ImGui::InputScalar("Sub Etat", ImGuiDataType_U8, &obj->sub_etat);
                ImGui::InputScalar("Main Etat", ImGuiDataType_U8, &obj->main_etat);
                ImGui::InputScalar("Initial Sub Etat", ImGuiDataType_U8, &obj->init_sub_etat);
                ImGui::InputScalar("Initial Main Etat", ImGuiDataType_U8, &obj->init_main_etat);

                if (ImGui::BeginCombo("Cmd", imgui_cmd_names[obj->cmd], ImGuiComboFlags_HeightLargest))
                {
                    for (u8 i = 0; i < 34; i++)
                    {
                        bool is_selected = obj->cmd == i;
                        if (ImGui::Selectable(imgui_cmd_names[i], is_selected))
                            obj->cmd = i;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }
                
                ImGui::InputScalar("Gravity Value 1", ImGuiDataType_U8, &obj->gravity_value_1);
                ImGui::InputScalar("Gravity Value 2", ImGuiDataType_U8, &obj->gravity_value_2);
                ImGui::InputScalar("Change Animation Mode", ImGuiDataType_U8, &obj->change_anim_mode);
                ImGui::InputScalar("Offset HY", ImGuiDataType_S8, &obj->offset_hy);
                ImGui::InputScalar("Follow Sprite", ImGuiDataType_U8, &obj->follow_sprite);
                ImGui::InputScalar("Hit Points", ImGuiDataType_S8, &obj->hit_points);
                ImGui::InputScalar("Initial Hit Points", ImGuiDataType_S8, &obj->init_hit_points);
                ImGui::InputScalar("Init Flag", ImGuiDataType_U8, &obj->init_flag);
                ImGui::InputScalar("Hit Sprite", ImGuiDataType_U8, &obj->hit_sprite);
                ImGui::InputScalar("Detect Zone", ImGuiDataType_U8, &obj->detect_zone);
                ImGui::InputScalar("Detect Zone Flag", ImGuiDataType_U8, &obj->detect_zone_flag);
                ImGui::InputScalar("Config", ImGuiDataType_U8, &obj->config);
                ImGui::InputScalar("Display Prio", ImGuiDataType_U8, &obj->display_prio);
                ImGui::InputScalar("Timer", ImGuiDataType_U8, &obj->timer);

                bool flag;
                flag = obj->flags.hurtbyfist;
                if (ImGui::Checkbox("Hurt By Fist", &flag))
                    obj->flags.hurtbyfist = flag;

                flag = obj->flags.command_test;
                if (ImGui::Checkbox("Command Test", &flag))
                    obj->flags.command_test = flag;

                flag = obj->flags.alive;
                if (ImGui::Checkbox("Alive", &flag))
                    obj->flags.alive = flag;

                flag = obj->flags.flip_x;
                if (ImGui::Checkbox("Flip X", &flag))
                    obj->flags.flip_x = flag;

                flag = obj->flags.read_commands;
                if (ImGui::Checkbox("Read Commands", &flag))
                    obj->flags.read_commands = flag;

                flag = obj->flags.follow_enabled;
                if (ImGui::Checkbox("Follow Enabled", &flag))
                    obj->flags.follow_enabled = flag;

                flag = obj->flags.flag_0x40;
                if (ImGui::Checkbox("Flag 0x40", &flag))
                    obj->flags.flag_0x40 = flag;

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("States"))
            {
                if (ImGui::BeginTable("obj_states", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
                {
                    // Set headers
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Next", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Speed", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Anim", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Sound", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableHeadersRow();

                    // Get the state counts. We store a pointer to these right before the data itself so we can easily access it.
                    u8* eta_counts = *(u8**)&obj->eta[-1];

                    s32 etat_index = 0;
                    for (u8 main_etat = 0; main_etat < eta_counts[0]; main_etat++)
                    {
                        for (u8 sub_etat = 0; sub_etat < eta_counts[main_etat + 1]; sub_etat++)
                        {
                            eta_t* state = &obj->eta[main_etat][sub_etat];

                            // Filter out invalid states
                            ImGui::BeginDisabled(
                                state->anim_index >= obj->anim_count ||
                                state->next_main_etat >= eta_counts[0] ||
                                state->next_sub_etat >= eta_counts[state->next_main_etat + 1] ||
                                abs(state->speed_x_left) != abs(state->speed_x_right) );

                            ImGui::TableNextRow();
                            ImGui::PushID(etat_index);
                            
                            ImGui::TableNextColumn();
                            if (ImGui::RadioButton("##state_enabled", obj->main_etat == main_etat && obj->sub_etat == sub_etat))
                            {
                                set_main_and_sub_etat(obj, main_etat, sub_etat);
                            }

                            ImGui::TableNextColumn();
                            ImGui::Text("%d-%d", main_etat, sub_etat);

                            ImGui::TableNextColumn();
                            ImGui::Text("%d-%d", state->next_main_etat, state->next_sub_etat);

                            ImGui::TableNextColumn();
                            ImGui::Text("%d", state->speed_x_right);

                            ImGui::TableNextColumn();
                            ImGui::Text("%d (%d)", state->anim_index, state->anim_speed);
                            
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", state->sound_index);
                            
                            ImGui::EndDisabled();
                            ImGui::PopID();
                            etat_index++;
                        }
                    }

                    ImGui::EndTable();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Commands"))
            {
                if (obj->cmds)
                {
                    s16 original_cmd_offset = obj->cmd_offset;
                    obj->cmd_offset = -1;

                    while (true)
                    {
                        // Read next command
                        obj->cmd_offset++;
                        u8 cmd = obj->cmds[obj->cmd_offset];

                        // End once we reach the invalid command
                        if (cmd == INVALID_CMD)
                            break;

                        ImGui::PushID(obj->cmd_offset);

                        ImGui::Text("%03d: ", obj->cmd_offset + 1);

                        ImGui::SameLine();
                        if (ImGui::RadioButton(imgui_cmd_names[cmd], obj->cmd_offset + 1 == original_cmd_offset))
                        {
                            original_cmd_offset = obj->cmd_offset + 1;
                            obj->cmd_offset = obj->cmd_offset - 1;
                            readOneCommand(obj);
                            obj->cmd_offset = original_cmd_offset - 1;
                        }

                        ImGui::PopID();

                        // Get the arguments
                        s16 saved_cmd_offset = obj->cmd_offset;
                        cptr_tab[cmd].skip(obj);
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 10, 255));
                        for (s32 i = 0; i < obj->cmd_offset - saved_cmd_offset; ++i)
                        {
                            u8 arg = obj->cmds[saved_cmd_offset + i];
                            ImGui::SameLine();
                            ImGui::Text("%d", arg);
                        }
                        ImGui::PopStyleColor();
                    }

                    obj->cmd_offset = original_cmd_offset;
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Global"))
            {
                // TODO: Implement. Show different global values based on the object type. For example Rayman has a lot of values for
                //       his movement etc. Bosses have specific values for their states and phases etc.

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    else
    {
        ImGui::Text("No object has been selected");
    }

    ImGui::End();
}

extern "C" void win32_prepare_frame(app_state_t* app_state) {
    s64 frames_elapsed = 0;
    s64 clocks_per_tick = performance_counter_frequency / app_state->target_game_hz;
    while (frames_elapsed < 1) {
        frames_elapsed = ((get_clock() / clocks_per_tick) - (app_state->frame_clock / clocks_per_tick));
        Sleep(1);
    }
    app_state->frame_clock += clocks_per_tick * frames_elapsed;

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

    glDrawBuffer(GL_BACK);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    win32_get_window_dimension(app_state->win32.window, &app_state->client_width, &app_state->client_height);

    // Upload surface to texture
    surface_t* surface = app_state->active_surface;
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, global_app_state.client_width, global_app_state.client_height);

    glBindTexture(GL_TEXTURE_2D, app_state->opengl.screen_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->width_pow2, surface->height_pow2, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->memory);

    // Output sound
    win32_produce_sound_for_frame(app_state, &app_state->win32.sound_output, &app_state->game.sound_buffer, app_state->flip_clock);

    ImGui::DockSpaceOverViewport();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //if (imgui_show_demo_window)
    //    ImGui::ShowDemoWindow(&imgui_show_demo_window);

    if (imgui_show_game_window) {
        show_game_window(&imgui_show_game_window);
    }

    if (imgui_show_rendering_info_window)
        show_rendering_info_window(&imgui_show_rendering_info_window);
    
    if (imgui_show_sounds_window)
        show_sounds_window(&imgui_show_sounds_window);

    if (imgui_show_game_info_window)
        show_game_info_window(&imgui_show_game_info_window);

    if (imgui_show_level_window)
        show_level_window(&imgui_show_level_window);

    if (imgui_show_object_properties_window)
        show_object_properties_window(&imgui_show_object_properties_window);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, global_app_state.client_width, global_app_state.client_height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
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
        ::UnregisterClass(app_state->win32.window_class.lpszClassName, app_state->win32.window_class.hInstance);
        exit(0);
    }
}

// Main code
int main(int argc, char** argv) {
    app_state_t* app_state = &global_app_state;

    app_state->win32.instance = GetModuleHandle(NULL);
    app_state->win32.cursor = LoadCursor(NULL, IDC_ARROW);

    // Make process DPI aware and obtain main monitor scale
    ::SetProcessDPIAware();
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSA window_class;
    memset(&window_class, 0, sizeof(window_class));

    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = WndProc;
    window_class.hInstance = GetModuleHandle(NULL);
    window_class.lpszClassName = "Rayverse";
    window_class.hbrBackground = NULL;
    app_state->win32.window_class = window_class;

    if (!RegisterClass(&window_class)) {
        return EXIT_FAILURE;
    }

    RECT desired_window_rect;
    memset(&desired_window_rect, 0, sizeof(desired_window_rect));
    int desired_width = 1280;
    int desired_height = 800;
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

    // Initialize OpenGL
    if (!CreateDeviceWGL(app_state->win32.window, &g_MainWindow))
    {
        CleanupDeviceWGL(app_state->win32.window, &g_MainWindow);
        ::DestroyWindow(app_state->win32.window);
        ::UnregisterClass(window_class.lpszClassName, window_class.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(app_state->win32.window, SW_MAXIMIZE);
    ::UpdateWindow(app_state->win32.window);

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
    ImGui_ImplWin32_InitForOpenGL(app_state->win32.window);
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
