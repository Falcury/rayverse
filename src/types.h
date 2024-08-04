
#pragma pack(push,1)
struct vec2b_t {
	u8 x, y;
};

struct rgb_t {
	u8 r, g, b;
};

struct rgb_palette_t {
	rgb_t colors[256];
};
#pragma pack(pop)

// platform.cpp


struct mem_t {
	size_t len;
	size_t capacity;
	i32 cursor;
	u8 data[1]; // allowed to extend beyond end of struct
};


// game.cpp

struct image_t {
	u8* memory;
	u32 memory_size;
	rgb_palette_t* pal;
	i32 width;
	i32 height;
	bool pal_needs_free;
	bool is_valid;
};

struct rect_t {
	i32 x;
	i32 y;
	i32 w;
	i32 h;
};

struct game_sound_buffer_t {
	i32 samples_per_second;
	u32 sample_count;
	i16* samples;
};

struct game_state_t {
	bool initialized;
	image_t draw_buffer;
	image_t draw_buffer_bg;
	game_sound_buffer_t sound_buffer;
};


// win32_sound.cpp

struct win32_sound_output_t {
	u32 samples_per_second;
	u32 bytes_per_sample;
	u32 secondary_buffer_size;
	u32 running_sample_index;
	u32 safety_bytes;
	IDirectSound* dsound;
	IDirectSoundBuffer* secondary_buffer;
	bool is_valid;
};

struct ogg_t {
	stb_vorbis* decoder;
	mem_t* file;
	u32 sample_count;
};


// rayverse.cpp

struct win32_state_t {
	HWND window;
	HINSTANCE instance;
	HCURSOR cursor;
	win32_sound_output_t sound_output;
};

struct opengl_state_t {
	GLuint screen_texture;
	GLint max_texture_size;
};


struct surface_t {
	u8* memory;
	u32 memory_size;
	i32 bytes_per_pixel;
	i32 width;
	i32 height;
	i32 width_pow2;
	i32 height_pow2;
	i32 pitch;
};

struct app_state_t {
	win32_state_t win32;
	opengl_state_t opengl;
	surface_t offscreen_surface;
	surface_t game_surface;
	surface_t* active_surface;
	i32 client_width;
	i32 client_height;
	bool running;
	bool vsync_enabled;
	i64 target_game_hz;
	float target_seconds_per_frame;
	game_state_t game;
	i64 flip_clock;
	i64 frame_clock;
};


// ray_engine.cpp

#pragma pack(push,1)

struct eta_t {
	i8 right_speed;
	i8 left_speed;
	u8 anim_index;
	u8 next_etat;
	u8 next_subetat;
	u8 anim_speed;
	u8 sound_index;
	u8 interaction_flags;
};

struct medaillion_t {
	u16 xpos;
	u16 ypos;
	u8 field_4;
	u8 field_5;
	u8 field_6;
	u8 field_7;
	u8 status;
	u8 cages;
	u8 world;
	u8 start_level;
	u8 color;
	u8 field_D;
	u8 field_E;
	u8 field_F;
	u32 text;
};

struct player_t {
	u16 lives;
	u8 field_2;
	u8 field_3;
	u8 field_4;
	u8 field_5;
	u8 tings;
	u8 field_7;
	u8 field_8;
	u8 max_hitp;
};

struct fist_t {
	u32 field_0;
	u16 field_4;
	u16 field_6;
	u16 field_8;
	u16 field_A;
	u8 fist_state;
	u8 field_D;
	u8 is_fist_thrown;
	u8 hit_strength;
	u8 field_10;
	u8 field_11;
	u8 field_12;
	u8 field_13;
};

struct maptile_t {
	u16 texture_id;
	u8 tile_type;
	u8 field_3;
	u8 transparency;
	u8 field_5;
};

struct x_texture_t {
	u8 pixels[256]; // 16x16 array of color indices
};

struct sprite_desc_t {
	u32 offset_in_atlas;
	u8 unk_index;
	u8 outer_width;
	u8 outer_height;
	u8 inner_width;
	u8 inner_height;
	u8 field_9;
	u8 field_A;
	u8 field_B;
};

/*struct anim_desc_t {
	u8 layers_per_frame;
	u8 unknown1;
	u8 frame_count;
	u8 unknown2;
	u32 unknown3;
	u16 frame_table_offset; // equals 4 * (layers_per_frame * frame_count + 1)
	u16 frame_count;
};*/


struct anim_layer_t {
	u8 mirrored;
	u8 x;
	u8 y;
	u8 sprite_index;
};

struct anim_frame_t {
	u8 unk_x;
	u8 unk_y;
	u8 unk_width;
	u8 unk_height;
};

struct anim_desc_t {
	anim_layer_t* layers;
	anim_frame_t* frames;
	u16 layers_per_frame;
	u16 frame_count;
};


struct pcx_header_t {
	u8 header_byte; // always 0x0A
	u8 software_version;
	u8 encoding_method; //0=no encoding, 1=run-length encoding
	u8 bits_per_bitplane;
	u16 min_x;
	u16 min_y;
	u16 max_x;
	u16 max_y;
	u16 hor_dpi;
	u16 ver_dpi;
	u8 ega_palette[48];
	u8 reserved1;
	u8 n_color_planes;
	u16 bytes_per_scanline;
	u16 palette_mode; //1=monochrome or color, 2=grayscale
	u16 source_hor_res;
	u16 source_ver_res;
	u8 reserved2[54];
};


struct obj_t {
	sprite_desc_t* sprites; // ImgDescriptorsPointer
	anim_desc_t* animations; // AnimDescriptorsPointer
	u8* image_atlas; // ImageBufferPointer
	eta_t** ETA;
	u32 commands; // 0x10 - ptr
	u32 command_labels; // 0x14 - ptr
	u32 cmd_stored_contexts; // 0x18 - ptr?
	u32 field_1C; // 0x1C
	u32 link_has_gendoor; // 0x20 - ?
	i32 is_active;
	i32 xpos;
	i32 ypos;
	u32 field_30;
	u16 obj_index;
	i16 screen_x;
	i16 screen_y;
	u16 field_3A;
	i16 spawn_x;
	i16 spawn_y;
	i16 xspeed;
	i16 yspeed;
	i16 sprites_count;
	i16 next_command_index;
	i16 command_count;
	i16 command_par2; // action (?) // command_par2?
	u16 follow_y;
	u16 follow_x;
	u16 cmd_arg_1;
	i16 phase;
	u16 rayman_distance; //54
	i16 iframes_timer; // timer (?)
	u16 test_block_index;
	u16 scale; // playing sound
	u16 zdc_meta; // ?
	u16 active_timer;
	u16 type;
	u8 coll_tile_0; // some_tile_type (?)
	u8 coll_tile_1;
	u8 coll_tile_2;
	u8 coll_tile_3;
	u8 field_66;
	u8 field_67;
	u8 offset_bx;
	u8 offset_by;
	u8 anim_index;
	u8 anim_frame;
	u8 subetat; // 6C
	u8 etat; // 6D
	u8 spawn_subetat; // 6E
	u8 spawn_etat; // 6F
	u8 command; // 70
	u8 gravity_value_1;
	u8 gravity_value_2;
	u8 change_anim_mode; // etat updated (?)
	i8 offset_hy;
	u8 follow_sprite; // the sprite index which uses the obj collision
	u8 hitp;
	u8 spawn_hitp;
	u8 init_flag; // display_prio?
	u8 hit_sprite; // 79
	u8 active_flag;
	u8 detect_zone;
	i8 cmd_context_depth;
	u8 configuration; // 7D
	u8 display_prio; // layer the obj sprites get drawn to, between 1 and 7; 0 means it doesn't get drawn
	i8 timer;
	i8 anim_count;
	u8 flags;
	u8 field_82;
	u8 field_83;
};


#pragma pack(pop)


struct archive_header_t {
	i32 offset;
	i32 size;
	u8 xor_byte;
	u8 checksum_byte;
};

struct event_cmd_t {
	u16 num_tokens;
	u16 num_labels;
	u8* tokens;
	u16* labels;
};


enum obj_type_enum {
	obj_0_livingstone						= 0,
	obj_1_platform							= 1,
	obj_2_energypoint						= 2,
	obj_3_electoon							= 3,
	obj_4_scenery							= 4,
	obj_5_magician							= 5,
	obj_6_yin_with_pin						= 6,
	obj_7_swinging_spiky_fruit				= 7,
	obj_8_plum1								= 8,
	obj_9_small_livingstone					= 9,
	obj_10_piranha							= 10,
	obj_11_hit_effect						= 11,
	obj_12_hunter1							= 12,
	obj_13_hunterbullet						= 13,
	obj_14_hunter2							= 14, // shoots upward
	obj_15									= 15,
	obj_16_falling_platform					= 16,
	obj_17_rising_platform					= 17,
	obj_18									= 18,
	obj_19_splash							= 19,
	obj_20_tentacle							= 20,
	obj_21_photographer						= 21,
	obj_22_moving_platform					= 22,
	obj_23_rayman			            	= 23,
	obj_24									= 24,
	obj_25_ejecting_platform				= 25,
	obj_26_disappearing_cloud				= 26,
	obj_27_trampoline_cloud					= 27,
	obj_28_blinking_cloud					= 28,
	obj_29_bounce_platform					= 29,
	obj_30_ambient_starter					= 30,
	obj_31_oneup_collected					= 31,
	obj_32_mr_dark_2nd_phase				= 32,
	obj_33_mr_dark_fireballs				= 33,
	obj_34_horizontal_nenuphar2				= 34,
	obj_35_stoneman_throwing				= 35,
	obj_36_stoneman_lava_ball				= 36,
	obj_37_tarayzan							= 37,
	obj_38_magicseed						= 38,
	obj_39_planted_nenuphar					= 39,
	obj_40_stone_dog						= 40,
	obj_41_prickly							= 41,
	obj_42_exit_sign						= 42,
	obj_43_stoneman_shooting				= 43,
	obj_44_stoneman_flame					= 44,
	obj_45_moving_prickly 					= 45,
	obj_46_mr_stone							= 46,
	obj_47_bouncing_lava_ball				= 47,
	obj_48_torch							= 48,
	obj_49_moving_flat_stone				= 49,
	obj_50_moskito_bzzit					= 50,
	obj_51_moskito_fruit_small				= 51,
	obj_52_moskito_fruit_medium				= 52,
	obj_53_shaky_fruit        				= 53,
	obj_54									= 54,
	obj_55_musician							= 55,
	obj_56_nonmoving_stoneman				= 56,
	obj_57_stake							= 57,
	obj_58_cage								= 58,
	obj_59_opened_cage    					= 59,
	obj_60_big_clown						= 60,
	obj_61_big_water_clown					= 61,
	obj_62_water_balloon					= 62,
	obj_63_platform_starts_moving 			= 63,
	obj_64_autoscroll						= 64,
	obj_65_spider							= 65,
	obj_66_spider_dart						= 66,
	obj_67_swinging_flower					= 67,
	obj_68									= 68,
	obj_69_big_bouncing_boulder				= 69,
	obj_70_trumpet							= 70,
	obj_71_note								= 71,
	obj_72_pirate_with_earring				= 72,
	obj_73_earring							= 73,
	obj_74_mr_sax							= 74,
	obj_75_bounce_glitter					= 75,
	obj_76_kildoor							= 76,
	obj_77_bomber_pirate					= 77,
	obj_78_pirate_bomb						= 78,
	obj_79_lava_ball_fragment				= 79,
	obj_80									= 80,
	obj_81_cymbal							= 81,
	obj_82_big_powerup						= 82,
	obj_83_explosion						= 83, // dust effect
	obj_84_tibetan_juggler					= 84,
	obj_85_eyes								= 85,
	obj_86_maracas							= 86,
	obj_87_tambourin_platform1				= 87,
	obj_88_tambourin_platform2				= 88,
	obj_89_joe								= 89,
	obj_90_mr_sax_wrong_note				= 90,
	obj_91_note_bomb1						= 91,
	obj_92_note_bomb2						= 92,
	obj_93_punched_wrong_note				= 93,
	obj_94_fist								= 94,
	obj_95_fistpickup						= 95,
	obj_96_mr_stone_totem					= 96,
	obj_97_mr_stone_boulder					= 97,
	obj_98_space_mama						= 98,
	obj_99_rayman_start_pos					= 99,
	obj_100_moth							= 100,
	obj_101_swinging_prickly      			= 101,
	obj_102_tiny_wrong_note					= 102,
	obj_103_large_eyed_fish					= 103,
	obj_104_large_nosed_fish				= 104,
	obj_105_bouncing_crystalrock1 			= 105,
	obj_106_bouncing_crystalrock2 			= 106,
	obj_107_big_spiky_ball					= 107,
	obj_108_mr_dark_electoons				= 108,
	obj_109_boulder							= 109,
	obj_110_destroyed_boulder				= 110,
	obj_111_crushed_guitar				    = 111,
	obj_112_washing_machine					= 112,
	obj_113_mallets							= 113,
	obj_114									= 114,
	obj_115_mr_stone_cloud					= 115,
	obj_116_tnt_clown						= 116,
	obj_117_tnt_clown_burst					= 117,
	obj_118_tnt_clown_kamikaze				= 118,
	obj_119_clown_tnt						= 119,
	obj_120_red_drummer						= 120,
	obj_121_mr_stone_flame					= 121,
	obj_122_mr_stone_stone_dog				= 122,
	obj_123_antitoon						= 123,
	obj_124_return_sign						= 124,
	obj_125_candy_platform					= 125,
	obj_126_fork					       	= 126,
	obj_127_swiss_knife						= 127,
	obj_128_corkscrew						= 128,
	obj_129_small_knife			   			= 129,
	obj_130_antitoon_eyes					= 130,
	obj_131_mallets_spring					= 131,
	obj_132_large_mouthed_fish				= 132,
	obj_133_supercopter						= 133,
	obj_134_plum2							= 134,
	obj_135_flame_mr_stone					= 135,
	obj_136_tiny_rayman						= 136,
	obj_137_fairy						   	= 137,
	obj_138_sprinkler_4heads				= 138,
	obj_139_maracas_flying					= 139,
	obj_140_ring							= 140,
	obj_141_snow							= 141,
	obj_142_extralife						= 142,
	obj_143_item_blink						= 143,
	obj_144									= 144,
	obj_145_bad_eyes						= 145,
	obj_146_eyes_bolt						= 146,
	obj_147_moskito_huge_spiky_fruit		= 147,
	obj_148									= 148,
	obj_149_treble_clef						= 149,
	obj_150_mr_skops						= 150,
	obj_151_bullet							= 151,
	obj_152_snare_drum						= 152,
	obj_153_betilla							= 153,
	obj_154_sprinkler_4heads_moving			= 154,
	obj_155_sprinkler_3heads				= 155,
	obj_156_wrong_note1						= 156,
	obj_157_water							= 157,
	obj_158_palette_change					= 158,
	obj_159_tibetan_6platforms				= 159,
	obj_160_tibetan_2platforms				= 160,
	obj_161_ting							= 161,
	obj_162_flying_saucer_platform			= 162,
	obj_163_circuit_box						= 163,
	obj_164_gendoor							= 164,
	obj_165_evil_livingstone				= 165,
	obj_166_lever							= 166,
	obj_167_plum3							= 167,
	obj_168_cymbal_single					= 168,
	obj_169									= 169,
	obj_170_mr_skops_beam					= 170,
	obj_171_rayman_bzzit_cutscene			= 171,
	obj_172_stoneman_aggressive   			= 172,
	obj_173_viking_ship						= 173,
	obj_174_space_pirate_left				= 174,
	obj_175_punaise_platform				= 175,
	obj_176_pencil_pointing_down			= 176,
	obj_177_yin1							= 177,
	obj_178_pencil_pointing_down_moving		= 178,
	obj_179_pencil_pointing_down_wave		= 179,
	obj_180_mr_sax_chasing					= 180,
	obj_181_mr_sax_walk_speed				= 181,
	obj_182_wrong_note_stationary_punchable	= 182,
	obj_183_mr_sax_first_encounter			= 183,
	obj_184_space_pirate_right				= 184,
	obj_185_wrong_note2						= 185,
	obj_186_priate_pan_flying_away			= 186,
	obj_187_viking_mama						= 187,
	obj_188_red_crystal_platform			= 188,
	obj_189_red_crystal_platform_moving		= 189,
	obj_190_viking_mama_sword				= 190,
	obj_191_pencil_falling					= 191,
	obj_192_space_mama_beam					= 192,
	obj_193_space_mama_pot					= 193,
	obj_194_space_mama_pot_part				= 194,
	obj_195_spider_ceiling					= 195,
	obj_196_spider_homing_dart				= 196,
	obj_197_medaillion						= 197,
	obj_198_mr_stone_chase					= 198,
	obj_199_mr_stone_chase_walking			= 199,
	obj_200									= 200,
	obj_201									= 201,
	obj_202_stars_on_rayman					= 202,
	obj_203_washing_machine2				= 203,
	obj_204_dark_rayman						= 204,
	obj_205_dark_rayman_fist				= 205,
	obj_206_rayman_feet						= 206,
	obj_207_frying_pan						= 207,
	obj_208_dust_circle						= 208,
	obj_209_mr_dark_flame_left				= 209,
	obj_210_mr_dark_flame_right				= 210,
	obj_211_mr_stone_swinging_totem			= 211,
	obj_212_mr_dark							= 212,
	obj_213_space_mama_with_washing_machine	= 213,
	obj_214_buoy							= 214,
	obj_215_mr_dark_spell_fireball			= 215,
	obj_216_eat_at_joes_sign				= 216,
	obj_217_moth2							= 217,
	obj_218_spaxkito						= 218,
	obj_219_flat_stone_rope					= 219,
	obj_220_flat_stone_with_ropes			= 220,
	obj_221_rope_dust						= 221,
	obj_222_float_stone_detached_rope		= 222,
	obj_223_stoneskops						= 223,
	obj_224_stoneskops_claw					= 224,
	obj_225_pan_pirate_45					= 225,
	obj_226_pan_pirate2_45					= 226,
	obj_227_moskito							= 227,
	obj_228_electric_plug					= 228,
	obj_229_punaise_facing_down				= 229,
	obj_230_punaise_horizontal				= 230,
	obj_231_moskitomama_right				= 231,
	obj_232_moskitomama_left				= 232,
	obj_233_moskitomama_beam				= 233,
	obj_234_lava							= 234,
	obj_235_punaise							= 235,
	obj_236_cancel_mr_dark_spell			= 236,
	obj_237_eraser							= 237,
	obj_238_betilla_text_fist				= 238,
	obj_239_viking_ship_nest				= 239,
	obj_240_pencil_pointing_up				= 240,
	obj_241_pencil_pointing_up_moving		= 241,
	obj_242_pencil_pointing_up_wave			= 242,
	obj_243_stylo							= 243,
	obj_244_space_mama_second_beam			= 244,
	obj_245_joes_beach						= 245,
	obj_246_mr_dark_separate_rope			= 246,
	obj_247									= 247,
	obj_248_mr_skops_claw					= 248,
	obj_249_theatre_curtain					= 249,
	obj_250_punaise_up_down					= 250,
	obj_251_theatre_water_front				= 251,
	obj_252_theatre_water_back				= 252,
	obj_253									= 253,
	obj_254									= 254,
	obj_255									= 255,
	obj_256_breakoutbrick_20				= 256,
	obj_257_breakout_game					= 257,
	obj_258_nougat							= 258,
	obj_259_edu_letter						= 259,
	obj_260_edu_digit						= 260,
	obj_261_edu_roadsign					= 261,
};

enum skills_enum {
	skills_1_punch   = 1,
	skills_2_hang    = 2,
	skills_4_copter  = 4,
	skills_8_supercopter  = 8,
	skills_0x40_magicseed  = 0x40,
	skills_0x80_grab  = 0x80,
	skills_0x100_run  = 0x100,
	skills_0x200_tiny  = 0x200,
	skills_0x400_firefly  = 0x400,
	skills_0x8000_squashed = 0x8000,
};


enum world_enum {
	world_0_none     = 0,
	world_1_jungle   = 1,
	world_2_music    = 2,
	world_3_mountain  = 3,
	world_4_image    = 4,
	world_5_cave     = 5,
	world_6_cake     = 6,
	world_7_unknown  = 7,
};

enum cmd_enum {
	cmd_0_left       = 0,
	cmd_1_right      = 1,
	cmd_2_up         = 2,
	cmd_3_down       = 3,
	cmd_4            = 4,
	cmd_5_subetat    = 5,
	cmd_6_skip       = 6,
	cmd_7            = 7,
	cmd_8_etat       = 8,
	cmd_9_prepareloop  = 9,
	cmd_10_doloop    = 10,
	cmd_11_label     = 11,
	cmd_12_goto      = 12,
	cmd_13_gosub     = 13,
	cmd_14_return    = 14,
	cmd_15_branchtrue  = 15,
	cmd_16_branchfalse  = 16,
	cmd_17_test      = 17,
	cmd_18_settest   = 18,
	cmd_19_waitstate  = 19,
	cmd_20_speed     = 20,
	cmd_21_go_x      = 21,
	cmd_22_go_y      = 22,
	cmd_23_reserved_skip  = 23,
	cmd_24_reserved_goto  = 24,
	cmd_25_reserved_gosub  = 25,
	cmd_26_reserved_branchtrue  = 26,
	cmd_27_reserved_branchfalse  = 27,
	cmd_28_reserved_skiptrue  = 28,
	cmd_29_reserved_skipfalse  = 29,
	cmd_30_idle      = 30,
	cmd_31_skiptrue  = 31,
	cmd_32_skipfalse  = 32,
	cmd_33_invalid_cmd  = 33,

};

enum sound_enum {
	sound_0 = 0,
	sound_1 = 1,
	sound_2 = 2,
	sound_3 = 3,
	sound_4 = 4,
	sound_5 = 5,
	sound_6 = 6,
	sound_7 = 7,
	sound_8 = 8,
	sound_9 = 9,
	sound_10 = 10,
	sound_11 = 11,
	sound_12 = 12,
	sound_13 = 13,
	sound_14 = 14,
	sound_15 = 15,
	sound_16 = 16,
	sound_17 = 17,
	sound_18 = 18,
	sound_19 = 19,
	sound_20 = 20,
	sound_21 = 21,
	sound_22 = 22,
	sound_23 = 23,
	sound_24 = 24,
	sound_25 = 25,
	sound_26 = 26,
	sound_27 = 27,
	sound_28_punched_livingstone = 28,
	sound_29 = 29,
	sound_30 = 30,
	sound_31 = 31,
	sound_32 = 32,
	sound_33 = 33,
	sound_34 = 34,
	sound_35 = 35,
	sound_36 = 36,
	sound_37 = 37,
	sound_38 = 38,
	sound_39 = 39,
	sound_40 = 40,
	sound_41 = 41,
	sound_42 = 42,
	sound_43 = 43,
	sound_44 = 44,
	sound_45 = 45,
	sound_46 = 46,
	sound_47 = 47,
	sound_48 = 48,
	sound_49 = 49,
	sound_50 = 50,
	sound_51 = 51,
	sound_52 = 52,
	sound_53 = 53,
	sound_54 = 54,
	sound_55 = 55,
	sound_56 = 56,
	sound_57 = 57,
	sound_58 = 58,
	sound_59 = 59,
	sound_60 = 60,
};

enum event_flags_enum {
	obj_flags_1 = 1,
	obj_flags_2 = 2,
	obj_flags_4_switched_on = 4,
	obj_flags_8_flipped = 8,
	obj_flags_0x10 = 0x10,
	obj_flags_0x20_follow_enabled = 0x20,
	obj_flags_0x40 = 0x40,
	obj_flags_0x80_anim_changed = 0x80,
};

// Also see:
// https://github.com/Adsolution/Ray1Map/blob/master/Assets/Scripts/DataTypes/Rayman1/Common/Events/Flags/R1_EventFlags.cs
enum obj_type_flags_enum {
	obj_type_flags_bit_0_is_always              = 1 << 0,  // byte 0, 0x1     If true the game sets the pos to (-32000, -32000) on init
	obj_type_flags_bit_1_is_balle               = 1 << 1,  // byte 0, 0x2     Indicates if the event is TYPE_BALLE1 or TYPE_BALLE2
	obj_type_flags_bit_2_no_collision           = 1 << 2,  // byte 0, 0x4     Indicates if the event has no collision - does not include follow
	obj_type_flags_bit_3_hurts_rayman           = 1 << 3,  // byte 0, 0x8     Indicates if the event damages Rayman
	obj_type_flags_bit_4                        = 1 << 4,  // byte 0, 0x10
	obj_type_flags_bit_5_has_detect_zone        = 1 << 5,  // byte 0, 0x20    Indicates if the detect zone should be set
	obj_type_flags_bit_6                        = 1 << 6,  // byte 0, 0x40
	obj_type_flags_bit_7_is_boss                = 1 << 7,  // byte 0, 0x80    Indicates if the boss bar should show
	obj_type_flags_bit_8                        = 1 << 8,  // byte 1, 0x1
	obj_type_flags_bit_9_is_collectible         = 1 << 9,  // byte 1, 0x2     Indicates if the event can be collected and thus not respawn again
	obj_type_flags_bit_10_fast_bump             = 1 << 10, // byte 1, 0x4
	obj_type_flags_bit_11                       = 1 << 11, // byte 1, 0x8
	obj_type_flags_bit_12_move_x                = 1 << 12, // byte 1, 0x10    Indicates if the object x position should be changed by SpeedX in MOVE_OBJECT
	obj_type_flags_bit_13_move_y                = 1 << 13, // byte 1, 0x20    Indicates if the object y position should be changed by SpeedY in MOVE_OBJECT
	obj_type_flags_bit_14_is_special_platform   = 1 << 14, // byte 1, 0x40    Indicates if DO_SPECIAL_PLATFORM should be called
	obj_type_flags_bit_15_do_cmds               = 1 << 15, // byte 1, 0x80    Indicates if commands should be read for the event, otherwise the command is set to 30 (NOP)
	obj_type_flags_bit_16_check_tile_type       = 1 << 16, // byte 2, 0x1     Indicates if the object reacts to block types (tile collision), thus calling calc_btyp
	obj_type_flags_bit_17                       = 1 << 17, // byte 2, 0x2
	obj_type_flags_bit_18                       = 1 << 18, // byte 2, 0x4
	obj_type_flags_bit_19_can_jump              = 1 << 19, // byte 2, 0x8     Indicates if obj_jump gets called when on a ressort (spring) block
	obj_type_flags_bit_20                       = 1 << 20, // byte 2, 0x10
	obj_type_flags_bit_21                       = 1 << 21, // byte 2, 0x20
	obj_type_flags_bit_22                       = 1 << 22, // byte 2, 0x40
	obj_type_flags_bit_23                       = 1 << 23, // byte 2, 0x80
	obj_type_flags_bit_24                       = 1 << 24, // byte 3, 0x1
	obj_type_flags_bit_25                       = 1 << 25, // byte 3, 0x2
	obj_type_flags_bit_26                       = 1 << 26, // byte 3, 0x4
	obj_type_flags_bit_27                       = 1 << 27, // byte 3, 0x8
	obj_type_flags_bit_28                       = 1 << 28, // byte 3, 0x10
	obj_type_flags_bit_29_link_requires_gendoor = 1 << 29, // byte 3, 0x20    Indicates if the object requires a gendoor in the link group to be valid
	obj_type_flags_bit_30_no_link               = 1 << 30, // byte 3, 0x40    Indicates that the event can't be linked
	obj_type_flags_bit_31                       = 1 << 31, // byte 3, 0x80
};

enum tile_flags_enum {
	tile_flags_1 = 1,
	tile_flags_2 = 2,
	tile_flags_4 = 4,
	tile_flags_8_icy = 8,
	tile_flags_0x10 = 0x10,
	tile_flags_0x20 = 0x20,
	tile_flags_0x40_floor = 0x40,
};

enum DES_enum {
	DES_0 = 0,
	DES_1_RAY = 1,
	DES_2_DIV = 2,
	DES_3_RED = 3,
	DES_4_PNG = 4,
	DES_5_LET = 5,
	DES_6_LET2 = 6,
	DES_7_CLK = 7,
	DES_8 = 8,
	DES_9 = 9,
	DES_10_CHA = 10,
	DES_11_EXP = 11,
	DES_12_ELE = 12,
	DES_13_LIV = 13,
	DES_14_MST = 14,
	DES_15_MST2 = 15,
	DES_16_NEN = 10,
	DES_17_PIR = 10,
	DES_18_SPL = 10,
	DES_19_TEN = 10,
};

enum ETA_enum {
	ETA_0_RAY = 0,
	ETA_1_DIV = 1,
	ETA_2_PNG = 2,
	ETA_3 = 3,
	ETA_4_CHA = 4,
	ETA_5_BAL = 5,
	ETA_6_EXP = 6,
	ETA_7_ELE = 7,
	ETA_8_PAN = 8,
	ETA_9 = 9,
	ETA_10_LIV = 10,
	ETA_11_LIVPTI = 11,
	ETA_12_MST = 12,
	ETA_13_NEN = 13,
	ETA_14_PIR = 14,
	ETA_15_SPL = 15,
	ETA_16_TEN = 16,
	ETA_17_TRZ = 17,
	ETA_18_BLK = 18,
	ETA_19_RMS = 19,
};

enum ray_state_enum {
	ray_state_0_0_anim_23_standing_idle = 0,
	ray_state_0_1_anim_23_standing_idle = 1,
	ray_state_0_2_anim_23_standing_idle = 2,
	ray_state_0_3_anim_4_leaning_edge_forward = 3,
	ray_state_0_4_anim_9_standing_on_hands = 4,
	ray_state_0_5_anim_11_jumping_on_hands = 5,
	ray_state_0_6_anim_12_jumping_on_hands = 6,
	ray_state_0_7_anim_13_jumping_on_hands = 7,
	ray_state_0_8_anim_20_landing = 8,
	ray_state_0_9_anim_11_on_hands = 9,
	ray_state_0_10_anim_12_on_hands = 10,
	ray_state_0_11_anim_0_prepare_punch = 11,
	ray_state_0_12_anim_1_build_up_punch = 12,
	ray_state_0_13_anim_2_throw_punch = 13,
	ray_state_0_14_anim_0_unk_cancel_punch = 14,
	ray_state_0_15_anim_27_frozen = 15,
	ray_state_0_16_anim_0_unk_idle = 16,
	ray_state_0_17_anim_48_unk_pick_up_stand = 17,
	ray_state_0_18_anim_5_stick_out_tongue = 18,
	ray_state_0_19_anim_19_falling = 19,
	ray_state_0_20_anim_53_prone_to_standing_on_hands = 20,
	ray_state_0_21_anim_59_unk_yellow_spheres = 21,
	ray_state_0_22_anim_59_unk_yellow_spheres = 22,
	ray_state_0_23_anim_59_unk_yellow_spheres = 23,
	ray_state_0_24_anim_59_unk_yellow_spheres = 24,
	ray_state_0_25_anim_59_unk_yellow_spheres = 25,
	ray_state_0_26_anim_59_unk_yellow_spheres = 26,
	ray_state_0_27_anim_59_unk_yellow_spheres = 27,
	ray_state_0_28_anim_59_unk_yellow_spheres = 28,
	ray_state_0_29_anim_59_unk_yellow_spheres = 29,
	ray_state_0_30_anim_59_unk_yellow_spheres = 30,
	ray_state_0_31_anim_0_unk_idle = 31,
};

enum eta_flags_enum {
	eta_flags_1 = 1,
	eta_flags_2 = 2,
	eta_flags_4 = 4,
	eta_flags_8_detect_fist = 8,
	eta_flags_0x10_anim_reverse = 0x10,
	eta_flags_0x20_detect_ray = 0x20,
};

// reference: http://www.manmrk.net/tutorials/basic/FBASIC/html/GfxScancodes.html
enum dos_scancode_enum {
	SC_ESCAPE       = 0x01,
	SC_1            = 0x02,
	SC_2            = 0x03,
	SC_3            = 0x04,
	SC_4            = 0x05,
	SC_5            = 0x06,
	SC_6            = 0x07,
	SC_7            = 0x08,
	SC_8            = 0x09,
	SC_9            = 0x0A,
	SC_0            = 0x0B,
	SC_MINUS        = 0x0C,
	SC_EQUALS       = 0x0D,
	SC_BACKSPACE    = 0x0E,
	SC_TAB          = 0x0F,
	SC_Q            = 0x10,
	SC_W            = 0x11,
	SC_E            = 0x12,
	SC_R            = 0x13,
	SC_T            = 0x14,
	SC_Y            = 0x15,
	SC_U            = 0x16,
	SC_I            = 0x17,
	SC_O            = 0x18,
	SC_P            = 0x19,
	SC_LEFTBRACKET  = 0x1A,
	SC_RIGHTBRACKET = 0x1B,
	SC_ENTER        = 0x1C,
	SC_CONTROL      = 0x1D,
	SC_A            = 0x1E,
	SC_S            = 0x1F,
	SC_D            = 0x20,
	SC_F            = 0x21,
	SC_G            = 0x22,
	SC_H            = 0x23,
	SC_J            = 0x24,
	SC_K            = 0x25,
	SC_L            = 0x26,
	SC_SEMICOLON    = 0x27,
	SC_QUOTE        = 0x28,
	SC_TILDE        = 0x29,
	SC_LSHIFT       = 0x2A,
	SC_BACKSLASH    = 0x2B,
	SC_Z            = 0x2C,
	SC_X            = 0x2D,
	SC_C            = 0x2E,
	SC_V            = 0x2F,
	SC_B            = 0x30,
	SC_N            = 0x31,
	SC_M            = 0x32,
	SC_COMMA        = 0x33,
	SC_PERIOD       = 0x34,
	SC_SLASH        = 0x35,
	SC_RSHIFT       = 0x36,
	SC_MULTIPLY     = 0x37,
	SC_ALT          = 0x38,
	SC_SPACE        = 0x39,
	SC_CAPSLOCK     = 0x3A,
	SC_F1           = 0x3B,
	SC_F2           = 0x3C,
	SC_F3           = 0x3D,
	SC_F4           = 0x3E,
	SC_F5           = 0x3F,
	SC_F6           = 0x40,
	SC_F7           = 0x41,
	SC_F8           = 0x42,
	SC_F9           = 0x43,
	SC_F10          = 0x44,
	SC_NUMLOCK      = 0x45,
	SC_SCROLLLOCK   = 0x46,
	SC_HOME         = 0x47,
	SC_UP           = 0x48,
	SC_PAGEUP       = 0x49,
	SC_LEFT         = 0x4B,
	SC_RIGHT        = 0x4D,
	SC_PLUS         = 0x4E,
	SC_END          = 0x4F,
	SC_DOWN         = 0x50,
	SC_PAGEDOWN     = 0x51,
	SC_INSERT       = 0x52,
	SC_DELETE       = 0x53,
	SC_F11          = 0x57,
	SC_F12          = 0x58,
};

typedef void (draw_func_t)(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
typedef i16 (scene_func_t)(u32 par_0);

