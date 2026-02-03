#include "ray_imgui.h"
#include "imgui_internal.h"


void build_docking_layout()
{
    /* Build a default docking layout. (NOTE: If already present in imgui.ini, the ini setting will overrule this.)

    |  Col 1           | Col 2       | Col 3     | Col 4              |

    |------------------+-------------+-----------+--------------------|
    | Game             | Game Info   | Level     | Object Properties  |
    |                  |             |           |                    |
    |------------------+             |           |                    |
    | Rendering Info   |             |           |                    |
    |------------------+             |           |                    |
    | Sounds           |             |           |                    |
    |------------------+-------------+-----------+--------------------| */

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");

    if (ImGui::DockBuilderGetNode(dockspace_id)) {
        return; // In this case, probably we will have loaded the dockspace alreaddy from imgui.ini (--> use that instead)
    }

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    // Column splits (left → right)
    ImGuiID dock_col1;
    ImGuiID dock_rest;

    // Left column (Game / Rendering / Sounds)
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.30f, &dock_col1, &dock_rest);

    ImGuiID dock_col2;
    ImGuiID dock_rest2;

    // Game Info column
    ImGui::DockBuilderSplitNode(dock_rest, ImGuiDir_Left, 0.33f, &dock_col2, &dock_rest2);

    ImGuiID dock_col3;
    ImGuiID dock_col4;

    // Level / Object Properties columns
    ImGui::DockBuilderSplitNode(dock_rest2, ImGuiDir_Left, 0.5f, &dock_col3, &dock_col4);

    // Split left column vertically
    ImGuiID dock_game;
    ImGuiID dock_left_bottom;

    ImGui::DockBuilderSplitNode(dock_col1, ImGuiDir_Up, 0.45f, &dock_game, &dock_left_bottom);

    ImGuiID dock_rendering;
    ImGuiID dock_sounds;

    ImGui::DockBuilderSplitNode(dock_left_bottom, ImGuiDir_Up, 0.33f, &dock_rendering, &dock_sounds);

    // Dock windows
    ImGui::DockBuilderDockWindow("Game", dock_game);
    ImGui::DockBuilderDockWindow("Rendering Info", dock_rendering);
    ImGui::DockBuilderDockWindow("Sounds", dock_sounds);

    ImGui::DockBuilderDockWindow("Game Info", dock_col2);
    ImGui::DockBuilderDockWindow("Level", dock_col3);
    ImGui::DockBuilderDockWindow("Object Properties", dock_col4);

    ImGui::DockBuilderFinish(dockspace_id);
}


void show_game_window(bool* p_open)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    if (!ImGui::Begin("Game", p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavInputs))
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

    s32 frequency = VGA_FREQ;
    if (ImGui::SliderInt("Frequency", &frequency, 0, 100)) {
        NewFrequency(frequency);
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

            if (imgui_filter_active_objects && !obj->is_active) {
                continue;
            }

            if (imgui_filter_alive_objects && !obj->flags.alive) {
                continue;
            }

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

                ImGui::SameLine();
                ImGui::TextUnformatted("Filter:");

                ImGui::SameLine();
                ImGui::Checkbox("Alive", &imgui_filter_alive_objects);

                ImGui::SameLine();
                ImGui::Checkbox("Active", &imgui_filter_active_objects);

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
