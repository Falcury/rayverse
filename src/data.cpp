
// Note: MSVC 6 doesn't support __VA_ARGS__

app_state_t global_app_state;
game_state_t* global_game;


obj_t global_sprite_groups[16]; // used in load_big_ray()
u16 num_sprite_groups;

rgb_palette_t fade_source_palette;
u8 fade_mode; //CFA87 (?)
u16 fade_temp[256*3];
u16 fade_speed; //CF7EA

// engine.cpp

ogg_t ogg_cd_track;
bool is_ogg_playing;
bool is_ogg_finished;

eta_t** global_eta_block[100]; //CCE48

draw_func_t* curr_obj_draw_proc; //CD10C
void* draw_flocon_1_normal_etx; //CD110
void* dword_CD114; //CD114
draw_func_t* curr_obj_draw_proc_flipped; //CD118

// 0xCEF47
u8 plan0_num_pcx[10];
// 0xC7F88
rgb_palette_t color_palettes[3];
u32 flocon_tab; //CD8BC
u32 map_time; //CD8C0
u32 time_left; //CD8C4

// 0xCCDF0
obj_t* obj_bodies;
// 0xCCDF4
u16 obj_count;

// 0xCE634
u32 background_DES;
// 0xCF81E
u16 bosses_beaten;

// 0xCE638
u16 level_width_tiles;
// 0xCE63A
u16 level_height_tiles;
// 0xCE63C
u32 level_tile_count;
// 0xCE640
maptile_t* level_tiles;


// 0xCE650
obj_t ray;

// 0xCE6DC
obj_t* rayman_fist;
// 0xCE6EC
obj_t rayman_tiny;
// 0xCE770
fist_t rayfist;
// 0xCE774
u16 word_CE774;
// 0xCE776
u16 word_CE776;
// 0xCE778
u16 word_CE778;
// 0xCE77A
u16 word_CE77A;
// 0xCE77C
u8 fist_state;
// 0xCE77D
u8 byte_CE77D;
// 0xCE77E
u8 is_fist_thrown;
// 0xCE77F
u8 fist_hit_strength;
// 0xCE780
u8 byte_CE780;
// 0xCE781
u8 byte_CE781;
obj_t bigray; //CE784


// 0xCEF52
player_t player;
// 0xCEF5B
u8 ray_max_hitp;
// 0xCF81A
u16 skills;

i16 proj_center_y; //CF836
i16 proj_center_x; //CF838
u16 current_map_location; //CF83A
u16 word_CF83C; //CF83C
u16 word_CF868; //CF868
u16 ray_clic; //CF840
u16 id_obj_grabbed; //CF842
u16 word_CF844; //CF844
u16 id_cling_pow; //CF846
u16 ray_between_clic; //CF848
i16 boss_y_to_reach; //CF84A
u16 screen_trembling3; //CF84C
u16 screen_trembling2; //CF84E
i16 boss_x_to_reach; //CF850
u16 screen_trembling; //CF852
i16 scroll_start_y; //CF854
i16 boss_scroll_start_y; //CF856
i16 boss_scroll_start_x; //CF858
i16 boss_scroll_end_x; //CF85A
i16 boss_scroll_end_y; //CF85C
i16 scroll_end_y; //CF85E
i16 ray_zdc_x; //CF860
i16 ray_zdc_h; //CF862
i16 ray_zdc_w; //CF864
u16 decalage_en_cours; //CF866
i16 ray_mode; //CF868
i16 remote_ray_x_to_reach; //CF86A
i16 scroll_start_x; //CF86E
i16 ray_wind_force; //CF870
i16 scroll_end_x; //CF872
i16 weather_wind; //CF874
i16 ray_zdc_y; //CF876
u16 mst_scroll_obj_id; //CF878
u16 num_scroll_obj; //CF87A
i16 curr_level_number; //CF87C
u16 word_CF87E; //CF87E
i16 new_level; //CF880
u16 water_obj_id; //CF882
i16 ymapmax; //CF884
u16 reduced_rayman_id; //CF886
u16 fee_obj_id; //CF888
u16 word_CF88A; //CF88A
i16 curr_world; // 0xCF88C
u16 pierre_acorde_obj_id; // CF88E
u16 xmapmap; //CF890
u16 new_world; // 0xCF892
u16 rayman_obj_id; //CF894
u16 mus_obd_id; //CF896
i16 helico_time; //CF898
u16 word_CF89A; //CF89A
u16 word_CF89C; //CF89C
i16 xmap; //CF89E
i16 departlevel; //CF8A0
i16 ray_y_main_pos_init; //CF8A2
i16 jump_time; //CF8A4
i16 ymap; //CF8A6
i16 special_ray_mov_win_x_left; //CF8A8
i16 ray_x_main_pos_init; //CF8AA
i16 ymap_old; //CF8AC
i16 special_ray_mov_win_x_right; //CF8AE
i16 xmap_old; //CF8B0
i16 h_scroll_speed; //CF8B2
i16 v_scroll_speed; //CF8B4
i16 saved_xmap; //CF8B6
i16 saved_ymap; //CF8B8
i16 scroll_y; //CF8BA
u16 black_ray_obj_id; //CF8BC
u16 word_CF8BE; //CF8BE
u16 final_pass_n; //CF8C0
u16 final_pass_a; //CF8C2
u16 ray_old_etat; //CF8C4
u16 final_pass_f; //CF8C6
u16 moskitomama_left_obj_id; //CF8C8
u16 final_pass_x; //CF8CA
u16 rideau_obj_id; //CF8CC
u16 corde_dark_obj_id; //CF8CE
u16 stosko_obj_id; //CF8D0
i16 scroll_x; //CF8D2
u16 ray_old_subetat; //CF8D4
u16 moskitosaxo_obj_id; //CF8D6
u16 moskitomama_right_obj_id; //CF8D8
u16 nb_fade; //CF8DA
u16 skops_last_action; //CF8DC
u16 skops_beam_on; //CF8DE
i16 skops_final_y; //CF8E0
i16 skops_final_x; //CF8E2
u16 word_CF8E4; //CF8E4
i16 skops_beam_speed; //CF8E6
i16 skops_beam_ds; //CF8E8
i16 skops_beam_dy; //CF8EA
u16 rubis_list_calculated; //CF8EC
i16 skops_beam_x; //CF8EE
i16 skops_beam_y; //CF8F0
u16 word_CF8F2; //CF8F2
u16 word_CF8F4; //CF8F4

// 0xCF89E
u16 level_width;


// 0xCF8A6
u16 level_height;

u8 horloge[25]; //CF9F5
u8 windows_version_major; //CFA0E
u8 windows_version_minor; //CFA0F
u8 is_master_mode; //CFA10
u8 byte_CFA11; //CFA11
u8 windows_version; //CFA12
u8 son_limite; //CFA13
u8 byte_CFA14; //CFA14
u8 byte_CFA15; //CFA15
u8 byte_CFA16; //CFA16
u8 is_sound_available; //CFA17
u8 is_background_clearing_needed; //CFA18
u8 byte_CFA19; //CFA19
u8 is_background_available; //CFA1A
u8 byte_CFA1B; //CFA1B
u8 cfg_24_default_01; //CFA1C
u8 something_with_cfg_25_default_00_joy; //CFA1D (?)
u8 cfg_25_default_00; //CFA1E
u8 cfg_diff_scrolling_enabled; //CFA1F
u8 vesa_mode; //CFA20
u8 byte_CFA21; //CFA21
u8 background_image_index; //CFA22
u8 is_joypad_mode; //CFA23
u8 type_fndscr_x; //CFA24
u8 cfg_15_default_03; //CFA25
u8 cpu_type; //CFA26
u8 byte_CFA27; //CFA27
u8 world_unk_offset_7_always_DA; //CFA28
u8 byte_CFA29; //CFA29
u8 byte_CFA2A; //CFA2A
u8 world_unk_offset_6; //CFA2B
u8 is_limited_mode; //CFA2C
u8 byte_CFA2D; //CFA2D
u8 byte_CFA2E; //CFA2E
u8 byte_CFA2F; //CFA2F
u8 byte_CFA30; //CFA30
u8 byte_CFA31; //CFA31
u8 is_zoom_effect_available; //CFA32
u8 is_normal_mode_available; //CFA33
u8 byte_CFA34; //CFA34
u8 cfg_language; //CFA35
u8 is_demo_mode; //CFA36
u8 menu_etape; //CFA37
u8 byte_CFA38; //CFA38
u8 need_quit; //CFA39
u8 byte_CFA3A; //CFA3A
u8 continues; //CFA3B
u8 byte_CFA3C; //CFA3C
u8 is_save_available; //CFA3D
u8 byte_CFA3E; //CFA3E
u8 byte_CFA3F; //CFA3F
u8 proc_exit; //CFA40
u8 byte_CFA41; //CFA41
u8 byte_CFA42; //CFA42
u8 byte_CFA53; //CFA53
u8 byte_CFA43; //CFA43
u8 vignet_to_display; //CFA44
u8 byte_CFA45; //CFA45
u8 is_game_won; //CFA46 (?)
u8 byte_CFA47; //CFA47
u8 byte_CFA48; //CFA48
u8 byte_CFA49; //CFA49
u8 byte_CFA4A; //CFA4A (?)
u8 need_skip_timer; //CFA4B (?)
u8 normal_mode_frequency; //CFA4C
u8 saved_current_pal; //CFA4D
u8 byte_CFA4E; //CFA4E
u8 confirmation_dialog_result; //CFA4F
u8 is_cd_music_active; //CFA50
u8 input_mode; //CFA51
u8 is_paused; //CFA52
u8 in_the_world_map; //CFA53
u8 ray_on_poelle; //CFA54
u8 nb_wiz_collected; //CFA55
u8 bonus_tings_to_get; //CFA56
u8 byte_CFA57; //CFA57
u8 byte_CFA58; //CFA58
u8 byte_CFA59; //CFA59
u8 byte_CFA5A; //CFA5A
u8 byte_CFA5B; //CFA5B
u8 fin_continue; //CFA5C
u8 byte_CFA5D; //CFA5D
u8 ray_inertia; //CFA5E
u8 byte_CFA5F; //CFA5F
u8 saved_map_location; // 0xCFA60
u8 byte_CFA61; // 0xCFA61
u8 byte_CFA62; // 0xCFA62
u8 zoom_mode; // 0xCFA63
u8 boss_reaching_accuracy_x; // CFA64
u8 boss_reaching_accuracy_y; // CFA65
u8 lidol_to_allocate; // CFA66
u8 save_current_boss_action; //CFA67
u8 scroll_locked; //CFA68
u8 save_boss_encounter; //CFA69
u8 boss_reaching_timer; //CFA6A
u8 boss_safe_timer; //CFA6B
u8 current_boss_action_is_over; //CFA6C
u8 byte_CFA6D; //CFA6D
u8 boss_encounter; //CFA6E
u8 current_boss_action; //CFA6F
u8 gerbe; //CFA70
u8 byte_CFA71; //CFA71
u8 ray_last_ground_btyp; //CFA72
u8 ray_mode_speed; //CFA73
u8 byte_CFA74; //CFA74
u8 ray_superhelico_bis; //CFA75
u8 fin_boss; //CFA76
u8 ray_in_fee_zone; //CFA77
u8 boss_dead; //CFA78
u8 byte_CFA79; //CFA79
u8 fin_du_jeu; //CFA7A
u8 is_ray_drowning; //CFA7B
u8 byte_CFA7C; //CFA7C
u8 button_released; //CFA7D
u8 dead_time; //CFA7E
u8 byte_CFA7F; //CFA7F
u8 byte_CFA80; //CFA80
u8 nb_cymbal_in_map; //CFA81
u8 current_pal_id; //CFA82
u8 active_palette; //CFA83
u8 pal_id_at_wizard; //CFA84
u8 save_save; //CFA85
u8 new_ms; //CFA86
u8 fade; //CFA87
u8 is_dark_rayman_active; // (?) CFA88
u8 text_dark2_affiche; //CFA89
u8 byte_CFA8A; //CFA8A
u8 ray_pos_in_stack; //CFA8B
u8 in_air_because_hit; //CFA8C
u8 dark_phase; //CFA8D
u8 is_boss_there; //CFA8E
u8 skops_enfonce_enable; //CFA8F
u8 byte_CFA90; //CFA90
u8 ecroule_plat_index; //CFA91
u8 byte_CFA92; //CFA92
u8 byte_CFA93; //CFA93
u8 boss_timer; // (?) CFA94
u8 byte_CFA95; //CFA95
u8 skops_nb_lave; //CFA96
u8 nb_trames_ecrase; //CFA97
u8 byte_CFA98; //CFA98
u8 life_becoz_wiz; //CFA99
u8 byte_CFA9A; //CFA9A
u8 byte_CFA9B; //CFA9B
u8 num_eta_blocks; //CFA9C

// 0xE5490
i16 voice_obj_ids[20];
// 0xE5750
u16 word_E5750;
// 0xE54B8
i16 sounds_playing[20];


// 0x9253C
u8 tile_flags[64] = {
		0x00, 0x20, 0x42, 0xC2, 0x42, 0x42, 0xC2, 0xC2, 0x04, 0x0B, //0-9
		0x00, 0x00, 0x00, 0x00, 0x03, 0x13, 0x00, 0x00, 0x4A, 0xCA, //10-19
		0x4A, 0x4A, 0xCA, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //20-29
		0x0B, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //30-39
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //40-49
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //50-59
		0x20, 0x20, 0x20, 0x20, //60-63
};



// 0x9257C
u32 obj_type_flags[262] = {
		0x23038028, 0x02030004, 0x42020000, 0x42218021, 0x42020004, 0x02020020, 0x26471008, 0x02020008, 0x20431100,
		0x23038028, 0x02018018, 0x4A000005, 0x03028028, 0x4222800B, 0x03028028, 0x4222800B, 0x02028104, 0x02028104,
		0x02038004, 0x4A200001, 0x03039128, 0x02028020, 0x02038100, 0x02038040, 0x02038000, 0x02020000, 0x02024000, 0x202C000,
		0x02024100, 0x02024000, 0x20100000, 0x42000001, 0x02028008, 0x4222B009, 0x02038100, 0x03038028, 0x42239009,
		0x02030000, 0x42038001, 0x42238001, 0x03039008, 0x02020008, 0x02120000, 0x03038028, 0x42228009, 0x02028108, 0x3038098,
		0x22031008, 0x02028004, 0x02028000, 0x0300B098, 0x42239009, 0x42239019, 0x02039028, 0x02028000, 0x02028020, 0x3038028,
		0x03028028, 0x22030204, 0x42230005, 0x03038028, 0x03038028, 0x42239009, 0x0202B100, 0x42008000, 0x03038028,
		0x42228009, 0x02020104, 0x0202C000, 0x23031008, 0x03038028, 0x43038009, 0x03230028, 0x4203B009, 0x03038098,
		0x48200005, 0x02038000, 0x03220028, 0x43639009, 0x42238009, 0x02030000, 0x03138820, 0x22120000, 0x42200009,
		0x02020000, 0x03020000, 0x02028100, 0x42020100, 0x42020100, 0x02038020, 0x42238009, 0x42238009, 0x42238009,
		0x42238001, 0x42000011, 0x42020000, 0x03028000, 0x43278019, 0x0303B098, 0x42000004, 0x03138028, 0x02020008,
		0x42238009, 0x02028000, 0x02028000, 0x02028008, 0x02038108, 0x02020008, 0x4212B001, 0x32060000, 0x42220001,
		0x02028000, 0x43231009, 0x02028008, 0x02038000, 0x4202C001, 0x03038028, 0x03038028, 0x03038028, 0x42238009, 0x3039038,
		0x42238009, 0x43239009, 0x230B9028, 0x42120000, 0x02038800, 0x02038008, 0x02038008, 0x02038008, 0x02078008,
		0x42631005, 0x02038000, 0x03028028, 0x02030000, 0x20431100, 0x42228009, 0x42020001, 0x22120000, 0x02038900,
		0x42220005, 0x02038100, 0x42130000, 0x22120200, 0x4A000011, 0x42018200, 0x02028008, 0x42230009, 0x03038028,
		0x42038000, 0x02030004, 0x030284B8, 0x22038008, 0x0303C020, 0x03038030, 0x02038900, 0x02038900, 0x03038008, 0x2000010,
		0x42030000, 0x02030800, 0x02030800, 0x42120000, 0x42020100, 0x02020000, 0x02120000, 0x23038028, 0x02020000,
		0x20431000, 0x0303B000, 0x0313B800, 0x4A228009, 0x42030005, 0x031B8028, 0x0223B000, 0x03029028, 0x02020008,
		0x12060008, 0x26471100, 0x12060008, 0x12060008, 0x03138098, 0x43038020, 0x03038008, 0x03038028, 0x03029028,
		0x03038008, 0x42220001, 0x03038498, 0x02020008, 0x02038108, 0x4203F019, 0x02020008, 0x43229019, 0x4302B009,
		0x4322B009, 0x03031028, 0x4222B009, 0x42020005, 0x03138498, 0x03038020, 0x03138408, 0x03138008, 0x42030005,
		0x42030009, 0x42028019, 0x42028019, 0x42020001, 0x00018000, 0x4A020001, 0x42139009, 0x42139009, 0x42430001,
		0x03028028, 0x0303B098, 0x42831000, 0x4A228011, 0x02028000, 0x03038008, 0x03039008, 0x02028010, 0x0202A010, 0x42020001,
		0x42220001, 0x03028008, 0x43029009, 0x03029028, 0x03029028, 0x0200B098, 0x02038000, 0x02020008, 0x02020008, 0x4302A008,
		0x4302A008, 0x42228009, 0x42020008, 0x02030008, 0x02020000, 0x0203C000, 0x42000005, 0x02230028, 0x12060008,
		0x12060008, 0x12060008, 0x0206C008, 0x42229019, 0x02028000, 0x42038000, 0x02038000, 0x42029009, 0x4202A004,
		0x02020008, 0x42020004, 0x42020004, 0x42020004, 0x02038000, 0x02038000, 0x4203B001, 0x0203B000, 0x02038800,
		0x02038000, 0x02038000, 0x02038000,
};

// 0x97544
u8 sound_flags[256] = {
		0x07, 0x03, 0x1B, 0x07, 0x07, 0x03, 0x1B, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, // 0-15
		0x07, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x0F, // 16-31
		0x0B, 0x07, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x07, 0x03, 0x07, 0x07, // 32-47
		0x1B, 0x07, 0x07, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, // 48-63
		0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x0B, 0x0B, 0x03, 0x07, 0x03, 0x03, 0x0B, 0x07, 0x03, 0x03, // 64-79
		0x07, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x0F, 0x07, 0x1F, 0x07, 0x07, 0x03, 0x03, 0x03, // 80-95
		0x03, 0x03, 0x03, 0x1B, 0x03, 0x07, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, // 96-111
		0x07, 0x03, 0x1B, 0x03, 0x03, 0x03, 0x07, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x1B, 0x03, 0x07, // 112-127
		0x1B, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0B, 0x03, 0x0B, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, // 128-143
		0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, // 144-159
		0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x07, 0x1B, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, // 160-175
		0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x03, 0x07, 0x03, 0x07, 0x03, 0x03, 0x03, 0x03, // 176-191
		0x04, 0x04, 0x07, 0x03, 0x07, 0x07, 0x03, 0x07, 0x07, 0x1B, 0x07, 0x03, 0x00, 0x1F, 0x07, 0x07, // 192-207
		0x03, 0x07, 0x07, 0x07, 0x03, 0x07, 0x07, 0x03, 0x07, 0x07, 0x07, 0x03, 0x07, 0x1B, 0x1F, 0x03, // 208-223
		0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x0F, // 224-239
		0x03, 0x03, 0x07, 0x07, 0x04, 0x1B, 0x04, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, // 240-255
};

medaillion_t medaillions[24] = {
		{29,  264, 18, 0,  0,  1,  1, 0, 1, 1,  2, 0, 0, 0, 0xBFEA8}, // 0: Pink Plant Woods
		{100, 254, 2,  1,  0,  4,  0, 0, 1, 5,  0, 0, 0, 0, 0xBFEA8}, // 1: Anguish Lagoon
		{90,  212, 2,  1,  3,  2,  0, 0, 1, 9,  2, 0, 0, 0, 0xBFEA8}, // 2: Swamps of Forgetfulness
		{47,  200, 3,  3,  3,  2,  0, 0, 1, 12, 1, 0, 0, 0, 0xBFEA8}, // 3: Moskito's Nest
		{180, 215, 5,  19, 1,  4,  0, 0, 2, 1,  2, 0, 0, 0, 0xBFEA8}, // 4: Bongo Hills
		{185, 175, 8,  4,  6,  8,  0, 0, 2, 7,  0, 0, 0, 0, 0xBFEA8}, // 5: Allegro Presto
		{135, 160, 20, 6,  7,  5,  0, 0, 2, 12, 2, 0, 0, 0, 0xBFEA8}, // 6: Gong Heights
		{91,  143, 7,  7,  7,  6,  0, 0, 2, 14, 3, 0, 0, 0, 0xBFEA8}, // 7: Mr Sax's Hullaballoo
		{212, 133, 9,  5,  5,  8,  0, 0, 3, 1,  1, 0, 0, 0, 0xBFEA8}, // 8: Twilight Gulch
		{209, 92,  21, 8,  9,  10, 0, 0, 3, 3,  0, 0, 0, 0, 0xBFEA8}, // 9: The Hard Rocks
		{257, 80,  10, 11,  9, 10, 0, 0, 3, 6,  1, 0, 0, 0, 0xBFEA8}, // 10: Mr Stone's Peaks
		{234, 235, 10, 11, 11, 12, 0, 0, 4, 1,  1, 0, 0, 0, 0xBFEA8}, // 11: Eraser Plains
		{278, 190, 13, 11, 11, 12, 0, 0, 4, 5,  5, 0, 0, 0, 0xBFEA8}, // 12: Pencil Pentathlon
		{290, 145, 22, 12, 13, 14, 0, 0, 4, 8,  1, 0, 0, 0, 0xBFEA8}, // 13: Space Mama's Crater
		{335, 180, 13, 15, 13, 14, 0, 0, 5, 1,  0, 0, 0, 0, 0xBFEA8}, // 14: Crystal Palace
		{312, 228, 14, 23, 15, 16, 0, 0, 5, 3,  5, 0, 0, 0, 0xBFEA8}, // 15: Eat at Joe's
		{360, 220, 17, 16, 15, 16, 0, 0, 5, 9,  2, 0, 0, 0, 0xBFEA8}, // 16: Mr Skops' Stalactites
		{357, 80,  17, 16, 17, 17, 0, 0, 6, 1,  2, 0, 0, 0, 0xBFEA8}, // 17: Mr Dark's Dare
		{10,  212, 18, 0,  18, 18, 0, 0, 7, 20, 5, 0, 0, 0, 0xBFEA8}, // 18: Save Game (Jungle)
		{182, 266, 4,  19, 19, 19, 0, 0, 7, 20, 3, 0, 0, 0, 0xBFEA8}, // 19: Save Game (Music 1)
		{145, 97,  20, 6,  20, 20, 0, 0, 7, 20, 0, 0, 0, 0, 0xBFEA8}, // 20: Save Game (Music 2)
		{190, 45,  21, 9,  21, 21, 0, 0, 7, 20, 2, 0, 0, 0, 0xBFEA8}, // 21: Save Game (Mountain)
		{276, 115, 22, 13, 22, 22, 0, 0, 7, 20, 1, 0, 0, 0, 0xBFEA8}, // 22: Save Game (Picture)
		{306, 267, 15, 23, 23, 23, 0, 0, 7, 20, 1, 0, 0, 0, 0xBFEA8}, // 23: Save Game (Cave)
};

// sound
u8 snd8b_headers[0x800];
u8* snd8b_data;


i32 saved_sprite_width; //92144

//9215C
archive_header_t vignetinfos[75] = {
		{0, 42275, 5, 253}, // 0
		{42275, 36462, 145, 81}, // 1
		{78737, 36152, 208, 209}, // 2
		{114889, 28818, 129, 241}, // 3
		{143707, 46061, 192, 26}, // 4
		{189768, 65113, 59, 169}, // 5
		{254881, 61578, 122, 21}, // 6
		{316459, 50466, 14, 223}, // 7
		{366925, 74739, 69, 202}, // 8
		{441664, 80357, 200, 179}, // 9
		{522021, 70496, 106, 158}, // 10
		{592517, 32003, 16, 240}, // 11
		{624520, 22540, 157, 59}, // 12
		{647060, 24561, 221, 81}, // 13
		{671621, 60596, 60, 233}, // 14
		{732217, 33238, 95, 109}, // 15
		{765455, 31569, 200, 105}, // 16
		{797024, 22153, 98, 77}, // 17
		{819177, 30899, 226, 214}, // 18
		{850076, 21609, 33, 253}, // 19
		{871685, 77649, 175, 199}, // 20
		{949334, 76571, 255, 79}, // 21
		{1025905, 92627, 115, 181}, // 22
		{1118532, 51216, 158, 97}, // 23
		{1169748, 42076, 203, 254}, // 24
		{1211824, 54578, 85, 83}, // 25
		{1266402, 38474, 185, 82}, // 26
		{1304876, 48091, 228, 82}, // 27
		{1352967, 37440, 150, 4}, // 28
		{1390407, 9177, 1, 225}, // 29
		{1399584, 16215, 121, 97}, // 30
		{1415799, 16215, 26, 100}, // 31
		{1432014, 23972, 180, 235}, // 32
		{1455986, 30360, 81, 64}, // 33
		{1486346, 55090, 128, 213}, // 34
		{1541436, 54064, 165, 17}, // 35
		{1595500, 73264, 225, 55}, // 36
		{1668764, 72077, 196, 210}, // 37
		{1740841, 133683, 114, 67}, // 38
		{1874524, 133683, 252, 153}, // 39
		{2008207, 63971, 154, 15}, // 40
		{2072178, 24933, 68, 50}, // 41
		{2097111, 50885, 82, 197}, // 42
		{2147996, 45348, 75, 151}, // 43
		{2193344, 33633, 139, 71}, // 44
		{2226977, 62831, 34, 119}, // 45
		{2289808, 18921, 208, 47}, // 46
		{2308729, 37610, 203, 107}, // 47
		{2346339, 53687, 187, 96}, // 48
		{2400026, 21443, 213, 68}, // 49
		{2421469, 26623, 86, 136}, // 50
		{2448092, 17234, 220, 97}, // 51
		{2465326, 26265, 121, 159}, // 52
		{2491591, 23760, 114, 125}, // 53
		{2515351, 15276, 209, 19}, // 54
		{2530627, 18626, 231, 31}, // 55
		{2549253, 17447, 5, 33}, // 56
		{2566700, 21935, 235, 231}, // 57
		{2588635, 24099, 64, 176}, // 58
		{2612734, 22162, 246, 40}, // 59
		{2634896, 13699, 255, 247}, // 60
		{2648595, 18002, 8, 23}, // 61
		{2666597, 23196, 253, 150}, // 62
		{2689793, 23083, 16, 198}, // 63
		{2712876, 13216, 145, 86}, // 64
		{2726092, 41228, 37, 210}, // 65
		{2767320, 26633, 206, 20}, // 66
		{2793953, 16768, 86, 238}, // 67
		{2810721, 26848, 91, 151}, // 68
		{2837569, 26694, 176, 104}, // 69
		{2864263, 18229, 171, 146}, // 70
		{2882492, 45240, 222, 238}, // 71
		{2927732, 19114, 33, 36}, // 72
		{2946846, 47407, 97, 52}, // 73
		{2994253, 14885, 77, 155}, // 74
};
