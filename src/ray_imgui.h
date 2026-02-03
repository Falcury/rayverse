#include "imgui.h"
#include "rayverse.h"

// Function prototypes for the Dear ImGui debugging layout for Rayman 1.
// It may be possible to decouple this code from Rayverse and have it shared with the PS1 decomp/port in the future.
// (Note that for that to work, the code for both projects should first be harmonized a bit further: bitfields, struct member names etc.)

void build_docking_layout();
void show_game_window(bool* p_open);
void show_rendering_info_window(bool* p_open);
void show_sounds_window(bool* p_open);
void show_game_info_window(bool* p_open);
void show_game_info_window(bool* p_open);
void show_objects_table(bool is_always, float height);
void show_level_window(bool* p_open);
void show_object_properties_window(bool* p_open);
