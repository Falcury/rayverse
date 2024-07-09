
#pragma pack(push,1)
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
	float target_game_hz;
	float target_seconds_per_frame;
	game_state_t game;
	i64 flip_clock;
};


// ray_engine.cpp

#pragma pack(push,1)

struct eta_t {
	u8 right_speed;
	u8 left_speed;
	u8 anim_index;
	u8 etat;
	u8 subetat;
	u8 anim_speed;
	u8 sound_index;
	u8 interaction_flags;
};

struct event_t {
	eta_t*** DES0; // ImgDescriptorsPointer
	eta_t*** DES4; // AnimDescriptorsPointer
	eta_t*** DES8; // ImageBufferPointer
	eta_t*** ETA;
	u32 commands; // ptr
	u32 command_labels; // ptr
	u32 cmd_stored_contexts; // ptr?
	u32 field_1C;
	u32 link_has_gendoor; //?
	i32 is_active;
	i32 xpos;
	i32 ypos;
	u32 field_30;
	u16 event_index;
	u16 x_from_right;
	u16 y_from_bottom;
	u16 field_3A;
	u16 spawn_x;
	u16 spawn_y;
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
	i8 offset_bx;
	i8 offset_by;
	i8 anim_index;
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
	u8 unknown[3];
};

struct anim_desc_t {
	u8 layers_per_frame;
	u8 unknown1;
	u8 frame_count;
	u8 unknown2;
	u32 unknown3;
	u16 frame_table_offset; // equals 4 * (layers_per_frame * frame_count + 1)
};

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


#pragma pack(pop)


struct vignetinfo_t {
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


enum event_type_enum {
	event_0_livingstone							= 0,
	event_1_platform							= 1,
	event_2_energypoint							= 2,
	event_3_electoon							= 3,
	event_4_scenery								= 4,
	event_5_magician							= 5,
	event_6_yin_with_pin						= 6,
	event_7_swinging_spiky_fruit				= 7,
	event_8_plum1								= 8,
	event_9_small_livingstone					= 9,
	event_10_piranha							= 10,
	event_11_hit_effect						= 11,
	event_12_hunter1							= 12,
	event_13_hunterbullet						= 13,
	event_14_hunter2							= 14, // shoots upward
	event_15									= 15,
	event_16_falling_platform					= 16,
	event_17_rising_platform					= 17,
	event_18									= 18,
	event_19_splash								= 19,
	event_20_tentacle							= 20,
	event_21_photographer						= 21,
	event_22_moving_platform					= 22,
	event_23_rayman			            		= 23,
	event_24									= 24,
	event_25_ejecting_platform					= 25,
	event_26_disappearing_cloud					= 26,
	event_27_trampoline_cloud					= 27,
	event_28_blinking_cloud						= 28,
	event_29_bounce_platform					= 29,
	event_30_ambient_starter					= 30,
	event_31_oneup_collected					= 31,
	event_32_mr_dark_2nd_phase					= 32,
	event_33_mr_dark_fireballs					= 33,
	event_34_horizontal_nenuphar2				= 34,
	event_35_stoneman_throwing					= 35,
	event_36_stoneman_lava_ball					= 36,
	event_37_tarayzan							= 37,
	event_38_magicseed							= 38,
	event_39_planted_nenuphar					= 39,
	event_40_stone_dog							= 40,
	event_41_prickly							= 41,
	event_42_exit_sign							= 42,
	event_43_stoneman_shooting					= 43,
	event_44_stoneman_flame						= 44,
	event_45_moving_prickly 					= 45,
	event_46_mr_stone							= 46,
	event_47_bouncing_lava_ball					= 47,
	event_48_torch								= 48,
	event_49_moving_flat_stone					= 49,
	event_50_moskito_bzzit						= 50,
	event_51_moskito_fruit_small				= 51,
	event_52_moskito_fruit_medium				= 52,
	event_53_shaky_fruit        				= 53,
	event_54									= 54,
	event_55_musician							= 55,
	event_56_nonmoving_stoneman					= 56,
	event_57_stake								= 57,
	event_58_cage								= 58,
	event_59_opened_cage    					= 59,
	event_60_big_clown							= 60,
	event_61_big_water_clown					= 61,
	event_62_water_balloon						= 62,
	event_63_platform_starts_moving 			= 63,
	event_64_autoscroll							= 64,
	event_65_spider								= 65,
	event_66_spider_dart						= 66,
	event_67_swinging_flower					= 67,
	event_68									= 68,
	event_69_big_bouncing_boulder				= 69,
	event_70_trumpet							= 70,
	event_71_note								= 71,
	event_72_pirate_with_earring				= 72,
	event_73_earring							= 73,
	event_74_mr_sax								= 74,
	event_75_bounce_glitter						= 75,
	event_76_kildoor							= 76,
	event_77_bomber_pirate						= 77,
	event_78_pirate_bomb						= 78,
	event_79_lava_ball_fragment					= 79,
	event_80									= 80,
	event_81_cymbal								= 81,
	event_82_big_powerup						= 82,
	event_83_explosion							= 83, // dust effect
	event_84_tibetan_juggler					= 84,
	event_85_eyes								= 85,
	event_86_maracas							= 86,
	event_87_tambourin_platform1				= 87,
	event_88_tambourin_platform2				= 88,
	event_89_joe								= 89,
	event_90_mr_sax_wrong_note					= 90,
	event_91_note_bomb1							= 91,
	event_92_note_bomb2							= 92,
	event_93_punched_wrong_note					= 93,
	event_94_fist								= 94,
	event_95_fistpickup							= 95,
	event_96_mr_stone_totem						= 96,
	event_97_mr_stone_boulder					= 97,
	event_98_space_mama							= 98,
	event_99_rayman_start_pos					= 99,
	event_100_moth								= 100,
	event_101_swinging_prickly      			= 101,
	event_102_tiny_wrong_note					= 102,
	event_103_large_eyed_fish					= 103,
	event_104_large_nosed_fish					= 104,
	event_105_bouncing_crystalrock1 			= 105,
	event_106_bouncing_crystalrock2 			= 106,
	event_107_big_spiky_ball					= 107,
	event_108_mr_dark_electoons					= 108,
	event_109_boulder							= 109,
	event_110_destroyed_boulder					= 110,
	event_111_crushed_guitar					= 111,
	event_112_washing_machine					= 112,
	event_113_mallets							= 113,
	event_114									= 114,
	event_115_mr_stone_cloud					= 115,
	event_116_tnt_clown							= 116,
	event_117_tnt_clown_burst					= 117,
	event_118_tnt_clown_kamikaze				= 118,
	event_119_clown_tnt							= 119,
	event_120_red_drummer						= 120,
	event_121_mr_stone_flame					= 121,
	event_122_mr_stone_stone_dog				= 122,
	event_123_antitoon							= 123,
	event_124_return_sign						= 124,
	event_125_candy_platform					= 125,
	event_126_fork					        	= 126,
	event_127_swiss_knife						= 127,
	event_128_corkscrew							= 128,
	event_129_small_knife			   			= 129,
	event_130_antitoon_eyes						= 130,
	event_131_mallets_spring					= 131,
	event_132_large_mouthed_fish				= 132,
	event_133_supercopter						= 133,
	event_134_plum2								= 134,
	event_135_flame_mr_stone					= 135,
	event_136_tiny_rayman						= 136,
	event_137_fairy						    	= 137,
	event_138_sprinkler_4heads					= 138,
	event_139_maracas_flying					= 139,
	event_140_ring								= 140,
	event_141_snow								= 141,
	event_142_extralife							= 142,
	event_143_item_blink						= 143,
	event_144									= 144,
	event_145_bad_eyes							= 145,
	event_146_eyes_bolt							= 146,
	event_147_moskito_huge_spiky_fruit			= 147,
	event_148									= 148,
	event_149_treble_clef						= 149,
	event_150_mr_skops							= 150,
	event_151_bullet							= 151,
	event_152_snare_drum						= 152,
	event_153_betilla							= 153,
	event_154_sprinkler_4heads_moving			= 154,
	event_155_sprinkler_3heads					= 155,
	event_156_wrong_note1						= 156,
	event_157_water								= 157,
	event_158_palette_change					= 158,
	event_159_tibetan_6platforms				= 159,
	event_160_tibetan_2platforms				= 160,
	event_161_ting								= 161,
	event_162_flying_saucer_platform			= 162,
	event_163_circuit_box						= 163,
	event_164_gendoor							= 164,
	event_165_evil_livingstone					= 165,
	event_166_lever								= 166,
	event_167_plum3								= 167,
	event_168_cymbal_single						= 168,
	event_169									= 169,
	event_170_mr_skops_beam						= 170,
	event_171_rayman_bzzit_cutscene				= 171,
	event_172_stoneman_aggressive   			= 172,
	event_173_viking_ship						= 173,
	event_174_space_pirate_left						= 174,
	event_175_punaise_platform					= 175,
	event_176_pencil_pointing_down				= 176,
	event_177_yin1								= 177,
	event_178_pencil_pointing_down_moving		= 178,
	event_179_pencil_pointing_down_wave			= 179,
	event_180_mr_sax_chasing					= 180,
	event_181_mr_sax_walk_speed					= 181,
	event_182_wrong_note_stationary_punchable	= 182,
	event_183_mr_sax_first_encounter			= 183,
	event_184_space_pirate_right						= 184,
	event_185_wrong_note2						= 185,
	event_186_priate_pan_flying_away			= 186,
	event_187_viking_mama						= 187,
	event_188_red_crystal_platform				= 188,
	event_189_red_crystal_platform_moving		= 189,
	event_190_viking_mama_sword					= 190,
	event_191_pencil_falling					= 191,
	event_192_space_mama_beam					= 192,
	event_193_space_mama_pot					= 193,
	event_194_space_mama_pot_part				= 194,
	event_195_spider_ceiling					= 195,
	event_196_spider_homing_dart				= 196,
	event_197_medaillion						= 197,
	event_198_mr_stone_chase					= 198,
	event_199_mr_stone_chase_walking			= 199,
	event_200									= 200,
	event_201									= 201,
	event_202_stars_on_rayman					= 202,
	event_203_washing_machine2					= 203,
	event_204_dark_rayman						= 204,
	event_205_dark_rayman_fist					= 205,
	event_206_rayman_feet						= 206,
	event_207_frying_pan						= 207,
	event_208_dust_circle						= 208,
	event_209_mr_dark_flame_left				= 209,
	event_210_mr_dark_flame_right				= 210,
	event_211_mr_stone_swinging_totem			= 211,
	event_212_mr_dark							= 212,
	event_213_space_mama_with_washing_machine	= 213,
	event_214_buoy								= 214,
	event_215_mr_dark_spell_fireball			= 215,
	event_216_eat_at_joes_sign					= 216,
	event_217_moth2								= 217,
	event_218_spaxkito							= 218,
	event_219_flat_stone_rope					= 219,
	event_220_flat_stone_with_ropes				= 220,
	event_221_rope_dust							= 221,
	event_222_float_stone_detached_rope			= 222,
	event_223_stoneskops						= 223,
	event_224_stoneskops_claw					= 224,
	event_225_pan_pirate_45						= 225,
	event_226_pan_pirate2_45					= 226,
	event_227_moskito							= 227,
	event_228_electric_plug						= 228,
	event_229_punaise_facing_down				= 229,
	event_230_punaise_horizontal				= 230,
	event_231_moskitomama_right					= 231,
	event_232_moskitomama_left					= 232,
	event_233_moskitomama_beam					= 233,
	event_234_lava								= 234,
	event_235_punaise							= 235,
	event_236_cancel_mr_dark_spell				= 236,
	event_237_eraser							= 237,
	event_238_betilla_text_fist					= 238,
	event_239_viking_ship_nest					= 239,
	event_240_pencil_pointing_up				= 240,
	event_241_pencil_pointing_up_moving			= 241,
	event_242_pencil_pointing_up_wave			= 242,
	event_243_stylo								= 243,
	event_244_space_mama_second_beam			= 244,
	event_245_joes_beach						= 245,
	event_246_mr_dark_separate_rope				= 246,
	event_247									= 247,
	event_248_mr_skops_claw						= 248,
	event_249_theatre_curtain					= 249,
	event_250_punaise_up_down					= 250,
	event_251_theatre_water_front				= 251,
	event_252_theatre_water_back				= 252,
	event_253									= 253,
	event_254									= 254,
	event_255									= 255,
	event_256_breakoutbrick_20					= 256,
	event_257_breakout_game						= 257,
	event_258_nougat							= 258,
	event_259_edu_letter						= 259,
	event_260_edu_digit							= 260,
	event_261_edu_roadsign						= 261,
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
	event_flags_1 = 1,
	event_flags_2 = 2,
	event_flags_4_switched_on = 4,
	event_flags_8_dir_right = 8,
	event_flags_0x10 = 0x10,
	event_flags_0x20 = 0x20,
	event_flags_0x40 = 0x40,
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

