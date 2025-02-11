
#pragma pack(push,1)
typedef struct vec2b_t {
	u8 x, y;
} vec2b_t;

typedef struct rgb_t {
	u8 r, g, b;
} rgb_t;

typedef struct rgb_palette_t {
	rgb_t colors[256];
} rgb_palette_t;
#pragma pack(pop)

// platform.cpp


typedef struct mem_t {
	size_t len;
	size_t capacity;
	i32 cursor;
	u8 data[1]; // allowed to extend beyond end of struct
} mem_t;


// game.cpp

typedef struct image_t {
	u8* memory;
	u32 memory_size;
	rgb_palette_t* pal;
	i32 width;
	i32 height;
	bool pal_needs_free;
	bool is_valid;
} image_t;

typedef struct rect_t {
	i32 x;
	i32 y;
	i32 w;
	i32 h;
} rect_t;

typedef struct game_sound_buffer_t {
	i32 samples_per_second;
	u32 sample_count;
	i16* samples;
} game_sound_buffer_t;


typedef struct snd_t {
    u8* data;
    i32 offset;
    i32 size;
    i32 sample_count;
    i32 bnk_field_C;
    i32 cursor;
    i32 dest_cursor;
    i32 sample_rate;
    u8 bytes_per_sample;
    u8 is_playing;
} snd_t;


typedef struct game_state_t {
	bool initialized;
	image_t draw_buffer; // draw_buffer
    rgb_palette_t* draw_palette;
	game_sound_buffer_t sound_buffer;
} game_state_t;


// win32_sound.cpp
#ifdef _WIN32
typedef struct win32_sound_output_t {
	u32 samples_per_second;
	u32 bytes_per_sample;
	u32 secondary_buffer_size;
	u32 running_sample_index;
	u32 safety_bytes;
	IDirectSound* dsound;
	IDirectSoundBuffer* secondary_buffer;
	bool is_valid;
} win32_sound_output_t;
#else
typedef struct sdl_sound_output_t {
    u32 samples_per_second;
    u32 bytes_per_sample;
    u32 secondary_buffer_size;
    u8* secondary_buffer; // TODO: fix this instead of keep using SDL_QueueAudio()
    i64 running_sample_index_for_writing;
    i64 running_sample_index_for_reading;
    u32 safety_bytes;
    SDL_AudioDeviceID audio_device;
} sdl_sound_output_t;
#endif

typedef struct ogg_t {
	stb_vorbis* decoder;
	mem_t* file;
	u32 sample_count;
} ogg_t;


// rayverse.cpp

#ifdef _WIN32
typedef struct win32_state_t {
    HWND window;
    HINSTANCE instance;
    HCURSOR cursor;
    win32_sound_output_t sound_output;
} win32_state_t;
#else
typedef struct sdl_state_t {
    SDL_Window* window;
    sdl_sound_output_t sound_output;
} sdl_state_t;
#endif

typedef struct opengl_state_t {
	GLuint screen_texture;
	GLint max_texture_size;
} opengl_state_t;


typedef struct surface_t {
	u8* memory;
	u32 memory_size;
	i32 bytes_per_pixel;
	i32 width;
	i32 height;
	i32 width_pow2; // for compatibility with old graphics cards that only support power-of-2 size textures
	i32 height_pow2;
	i32 pitch;
} surface_t;

typedef struct app_state_t {
#ifdef _WIN32
	win32_state_t win32;
#else
    sdl_state_t sdl;
#endif
	opengl_state_t opengl;
	surface_t offscreen_surface;
	surface_t game_surface;
	surface_t* active_surface;
	i32 client_width;
	i32 client_height;
    float display_scale_factor;
    float display_points_per_pixel;
	bool running;
	bool vsync_enabled;
	i64 target_game_hz;
	float target_seconds_per_frame;
	game_state_t game;
	i64 flip_clock;
	i64 frame_clock;
} app_state_t;


#pragma pack(push,1)

typedef struct eta_t {
	i8 right_speed;
	i8 left_speed;
	u8 anim_index;
	u8 next_etat;
	u8 next_subetat;
	u8 anim_speed;
	u8 sound_index;
	u8 interaction_flags;
} eta_t;

typedef struct world_info_t {
	u16 xpos;
	u16 ypos;
	u8 index_up;
	u8 index_down;
	u8 index_left;
	u8 index_right;
	u8 state;
	u8 nb_cages;
	u8 world;
	u8 level;
	i8 color;
	u8 field_D;
	u8 field_E;
	u8 field_F;
	char* text;
} world_info_t;

typedef struct status_bar_t {
	u16 lives;
	u8 lives_digits[2];
	u8 hp_sprites[2];
	u8 num_wiz;
	u8 wiz_digits[2];
	u8 max_hitp;
} status_bar_t;

typedef struct {
    u8 lives;
    u8 tings;
    u8 cages;
    u8 continues;
} loadinforay_t;

typedef struct poing_t {
	u32 field_0; // Fixed-point y pos value?
	u16 field_4;
	u16 speed_x;
	u16 charge;
	u16 field_A;
	u8 sub_etat; // Normal: 1,3,5, Gold: 8,10,12
	u8 is_returning;
	u8 is_active;
	u8 damage;
	u8 is_charging;
	u8 is_boum;
	u8 field_12;
	u8 field_13;
} poing_t;

typedef struct x_texture_t {
	u8 pixels[256]; // 16x16 array of color indices
} x_texture_t;

typedef struct sprite_t {
	u32 offset_in_atlas;
	u8 unk_index;
	u8 outer_width;
	u8 outer_height;
	u8 inner_width;
	u8 inner_height;
	u8 field_9;
	u8 color;
	u8 flags;
} sprite_t;

typedef struct anim_layer_t {
	u8 mirrored;
	u8 x;
	u8 y;
	u8 sprite_index;
} anim_layer_t;

typedef struct anim_frame_t {
	u8 unk_x;
	u8 unk_y;
	u8 unk_width;
	u8 unk_height;
} anim_frame_t;

typedef struct anim_t {
	anim_layer_t* layers;
	anim_frame_t* frames;
	u16 layers_per_frame;
	u16 frame_count;
} anim_t;

typedef struct pcx_header_t {
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
} pcx_header_t;

typedef struct obj_flags_t {
    u8 flag_1 : 1;          // 1
    u8 command_test : 1;    // 2
    u8 alive : 1;           // 4
    u8 flip_x : 1;          // 8
    u8 read_commands : 1;   // 0x10
    u8 follow_enabled : 1;  // 0x20
    u8 flag_0x40 : 1;       // 0x40
    u8 anim_changed : 1;       // 0x80
    u8 flag_0x100 : 1;      // 0x100
} obj_flags_t;

typedef struct obj_t {
	sprite_t* sprites; // ImgDescriptorsPointer
	anim_t* animations; // AnimDescriptorsPointer
	u8* img_buffer; // ImageBufferPointer
	eta_t** eta;
	u8* cmds;
	u8* cmd_labels;
	u8* cmd_contexts;
	u32 field_1C; // 0x1C
	u32 link_has_gendoor; // 0x20 - ?
	i32 is_active; // 24
	i32 x; // 28
	i32 y; // 2C
	i32 active_flag; // 30
	i16 obj_index;
	i16 screen_x;
	i16 screen_y;
	u16 field_3A;
	i16 init_x;
	i16 init_y;
	i16 speed_x;
	i16 speed_y;
	i16 nb_sprites;
	i16 cmd_offset;
	i16 nb_cmd;
	i16 cmd_arg_2; // action (?) // command_par2? // field_4A
	u16 follow_y;
	u16 follow_x;
	u16 cmd_arg_1;
	i16 phase; // 52
	u16 rayman_dist; //54
	i16 iframes_timer; // timer (?)
	u16 test_block_index;
	u16 scale; // 5A
	u16 zdc; // ?
	u16 active_timer;
	u16 type;
	u8 coll_btype[5]; // 0 = center, 1 = left, 2 = right, 3 = up, 4 = down
	u8 field_67;
	u8 offset_bx;
	u8 offset_by;
	u8 anim_index;
	u8 anim_frame;
	u8 sub_etat; // 6C
	u8 main_etat; // 6D
	u8 init_sub_etat; // 6E
	u8 init_etat; // 6F
	u8 cmd; // 70
	u8 gravity_value_1; // 71
	u8 gravity_value_2; // 72
	u8 change_anim_mode; // etat updated (?) // 73
	i8 offset_hy;
	u8 follow_sprite; // the sprite index which uses the obj collision
	u8 hit_points;
	u8 init_hit_points;
	u8 init_flag; // 78
	u8 hit_sprite; // 79
	u8 detect_zone; // 7A
	u8 detect_zone_flag; // 7B
	i8 cmd_context_depth; // 7C
	u8 configuration; // 7D
	u8 display_prio; // layer the obj sprites get drawn to, between 1 and 7; 0 means it doesn't get drawn
	i8 timer; // 7F
	u8 anim_count;
    obj_flags_t flags;
	u8 field_83;
} obj_t;

typedef struct zdc_t {
    i16 x_pos;
    i16 y_pos;
    u8 width;
    u8 height;
    u8 flags;
    u8 sprite;
} zdc_t;

typedef struct level_t {
	obj_t* objects;
	u16 nb_objects;
} level_t;

typedef struct map_tile_t {
	u16 texture_id;
	u8 tile_type;
	u8 field_3;
	u8 transparency;
	u8 field_5;
} map_tile_t;

typedef struct map_data_t {
	i16 width;
	i16 height;
	i32 length;
	map_tile_t* map;
} map_data_t;

typedef struct voice_t {
	i16 field_0;
	i16 field_2;
	i16 field_4;
} voice_t;

typedef struct options_t {
	i16 field_0;
	i16 field_2;
	i16 SizeScreen;
	i16 Frequence;
	i16 fixon;
	i16 BackgroundOn;
	i16 ScrollDiffOn;
	i16 field_E; //unused?
	i16 field_10; //unused?
	i16 field_12; //unused?
} options_t;

typedef struct options_jeu_t {
	void* func_0;
	void* func_4;
	void* func_8;
	void* func_C;
	u16 jump;
	u16 fist;
	u16 field_14;
	u16 action;
	u16 music_enabled;
	u16 sound_volume;
	u16 is_stereo;
} options_jeu_t;

typedef struct record_t {
	i32 current_offset;
	i32 repeat_length;
	i32 repeat_index;
	i32 length;
	u8* data;
	u8 is_recording;
	u8 is_playing;
	u8 is_finished;
} record_t;

typedef struct save_state_t {
	u32 triggered_objects[8]; // 0
	i16 nb_floc[8]; // 0x20
	i16 vent_x; // 0x30
	i16 vent_y; // 0x32
	i16 x_map; // 0x34
	i16 y_map; // 0x36
	i16 ray_x_pos; //0x38
	i16 ray_y_pos; //0x3A
	i16 ray_screen_x; //0x3C
	i16 ray_screen_y; //0x3E
	u16 ray_flip_x; //0x40
	i16 save_obj_id; //0x42
	i16 save_obj_x_pos; //0x44
	i16 save_obj_y_pos; //0x46
	u16 link_init[256]; //0x48
	u8 save_obj_detect_zone_flag; //0x248
	u8 save_obj_flag_1;
	u8 ray_coll_btype[5];
	u8 ray_anim_index;
	u8 ray_anim_frame;
	u8 ray_main_etat;
	u8 ray_sub_etat;
	u8 poing_sub_etat;
	u8 rayevts_reverse;
	u8 rayevts_super_helico;
	u8 rayevts_poing;
	u8 current_pal_id;
	u8 dead_time;
	u8 is_just_saved;
	u8 status_bar_tings;
} save_state_t;

// TODO: check that this is right? seems to differ from PS1 sound table entries (4 bytes instead of 5; flags separate?)
typedef struct sound_table_entry_t {
    u8 note;
    u8 prog;
    u8 tone;
    u8 volume;
} sound_table_entry_t;

#pragma pack(pop)


typedef struct archive_header_t {
	i32 offset;
	i32 size;
	u8 xor_byte;
	u8 checksum_byte;
} archive_header_t;

typedef struct event_cmd_t {
	u16 num_tokens;
	u16 num_labels;
	u8* tokens;
	u16* labels;
} event_cmd_t;

typedef struct display_item_t {
    char text[200];
    i16 centered_x_pos;
    i16 centered_y_pos;
    i16 width;
    i16 height;
    i16 xpos;
    i16 ypos;
    u8 font_size;
    u8 field_D5;
    u8 is_fond;
    i8 color;
} display_item_t;


typedef struct {
    void (*command)(obj_t* obj);
    void (*poing_collision)(obj_t* obj, i16 unk);
    void (*rayman_collision)(obj_t* obj);
    void (*rayman_in_zone)(obj_t* obj);
    void (*u_turn)(obj_t* obj);
} obj_procs_t;

typedef struct {
    u8 (*read_arg)(obj_t* obj);
    u8 (*skip_arg)(obj_t* obj);
    void (*handle)(obj_t* obj);
} cptr_t;

typedef struct {
    i16 field_0;
    i16 field_2;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    i16 field_8;
} vitraux_info_t;

typedef struct {
    i32 offset;
    i32 size;
    i32 field_8;
    i32 field_C;
} bnk_header_t;


enum obj_type_enum {
	TYPE_0_BADGUY1 = 0, // livingstone
	TYPE_1_PLATFORM = 1,
	TYPE_2_POWERUP = 2, // energypoint
	TYPE_3_LIDOLPINK = 3, // electoon
	TYPE_4_NEUTRAL = 4, // scenery
	TYPE_5_WIZARD1 = 5, // magician
	TYPE_6_FALLING_YING_OUYE = 6, // yin with pin
	TYPE_7_MORNINGSTAR = 7, // swinging spiky fruit
	TYPE_8_FALLING_OBJ = 8, // plum
	TYPE_9_BADGUY2 = 9, // small livingstone
	TYPE_10_FISH = 10, // piranha
	TYPE_11_BOUM = 11, // hit effect
	TYPE_12_CHASSEUR1 = 12, // hunter 1
	TYPE_13_BALLE1 = 13, // hunter bullet
	TYPE_14_CHASSEUR = 14, // hunter 2 (shoots upward)
	TYPE_15_BALLE2 = 15,
	TYPE_16_FALLPLAT = 16, // falling platform
	TYPE_17_LIFTPLAT = 17, // rising platform
	TYPE_18_BTBPLAT = 18,
	TYPE_19_SPLASH = 19, // splash
	TYPE_20_GENEBADGUY = 20, // tentacle
	TYPE_21_PHOTOGRAPHE = 21, // photographer
	TYPE_22_MOVE_PLAT = 22, // moving platform
	TYPE_23_RAYMAN = 23,
	TYPE_24_INTERACTPLT = 24,
	TYPE_25_INST_PLAT = 25, // ejecting platform
	TYPE_26_CRUMBLE_PLAT = 26, // disappearing cloud
	TYPE_27_BOING_PLAT = 27, // trampoline cloud
	TYPE_28_ONOFF_PLAT = 28, // blinking cloud
	TYPE_29_AUTOJUMP_PLAT = 29, // bounce platform
	TYPE_30_AUDIOSTART = 30, // ambient starter
	TYPE_31_ONEUP_ALWAYS = 31, // 1-up collected
	TYPE_32_DARK_PHASE2 = 32, // Mr Dark 2nd phase
	TYPE_33_DARK2_SORT = 33, // Mr Dark fireballs
	TYPE_34_MOVE_AUTOJUMP_PLAT = 34, // horizontal nenuphar 2
	TYPE_35_STONEMAN1 = 35, // stoneman (throwing)
	TYPE_36_STONEBOMB = 36, // stoneman lava ball
	TYPE_37_TARZAN = 37, // tarayzan
	TYPE_38_GRAINE = 38, // magic seed
	TYPE_39_NEN_GRAINE = 39, // planted nenuphar
	TYPE_40_STONEDOG = 40, // stone dog
	TYPE_41_OUYE = 41, // prickly
	TYPE_42_SIGNPOST = 42, // exit sign
	TYPE_43_STONEMAN2 = 43, // stoneman (shooting)
	TYPE_44_CLASH = 44, // stoneman flame
	TYPE_45_MOVE_OUYE = 45, // moving prickly
	TYPE_46_BB1 = 46, // Mr Stone
	TYPE_47_STONEBOMB2 = 47, // bouncing lava ball
	TYPE_48_FLAMME2 = 48, // torch
	TYPE_49_MOVE_START_PLAT = 49, // moving flat stone
	TYPE_50_MOSKITO = 50, // moskito (Bzzit)
	TYPE_51_MST_FRUIT1 = 51, // moskito fruit small
	TYPE_52_MST_FRUIT2 = 52, // moskito fruit medium
	TYPE_53_MST_SHAKY_FRUIT = 53, // shaky fruit
	TYPE_54_MEDAILLON = 54, // medaillon
	TYPE_55_MUS_WAIT = 55, // mucisian
	TYPE_56_STONEWOMAN2 = 56, // nonmoving stoneman
	TYPE_57_STALAG = 57, // stake
	TYPE_58_CAGE = 58, // cage
	TYPE_59_CAGE2 = 59, // opened cage
	TYPE_60_BIG_CLOWN = 60, // big clown
	TYPE_61_WAT_CLOWN = 61, // big water clown
	TYPE_62_DROP = 62, // water balloon
	TYPE_63_MOVE_START_NUA = 63, // platform starts moving
	TYPE_64_SCROLL = 64, // autoscroll
	TYPE_65_SPIDER = 65, // spider
	TYPE_66_DARD = 66, // spider dart
	TYPE_67_SWING_PLAT = 67, // swinging flower
	TYPE_68_BIG_BOING_PLAT = 68,
	TYPE_69_STONEBOMB3 = 69, // big bouncing boulder
	TYPE_70_TROMPETTE = 70, // trumpet
	TYPE_71_NOTE = 71, // note
	TYPE_72_PIRATE_NGAWE = 72, // pirate with earring
	TYPE_73_RING = 73, // earring
	TYPE_74_SAXO = 74, // Mr Sax
	TYPE_75_PAILETTE = 75, // bounce glitter
	TYPE_76_DESTROYING_DOOR = 76, // kildoor
	TYPE_77_PIRATE_GUETTEUR = 77, // bomber pirate
	TYPE_78_PIRATE_BOMB = 78, // pirate bomb
	TYPE_79_STONECHIP = 79, // lava ball fragment
	TYPE_80_BIGSTONE = 80,
	TYPE_81_CYMBALE = 81, // cymbal
	TYPE_82_JAUGEUP = 82, // big powerup
	TYPE_83_EXPLOSION = 83, // dust effect
	TYPE_84_TIBETAIN = 84, // tibetan juggler
	TYPE_85_ROLLING_EYES = 85, // eyes
	TYPE_86_MARACAS = 86, // maracas
	TYPE_87_TAMBOUR1 = 87, // tambourin platform1
	TYPE_88_TAMBOUR2 = 88, // tambourin platform2
	TYPE_89_JOE = 89, // Joe
	TYPE_90_NOTE0 = 90, // Mr Sax wrong note
	TYPE_91_NOTE1 = 91, // note bomb 1
	TYPE_92_NOTE2 = 92, // note bomb 2
	TYPE_93_BONNE_NOTE = 93, // punched wrong note
	TYPE_94_POING = 94, // fist
	TYPE_95_POING_POWERUP = 95, // fist pickup
	TYPE_96_TOTEM = 96, // Mr Stone totem
	TYPE_97_BBL = 97, // Mr Stone boulder
	TYPE_98_SPACE_MAMA = 98, // Space Mama
	TYPE_99_RAY_POS = 99, // Rayman start pos
	TYPE_100_MITE = 100, // moth
	TYPE_101_MORNINGSTAR_MOUNTAI = 101, // swinging prickly
	TYPE_102_BNOTE = 102, // tiny wrong note
	TYPE_103_POI1 = 103, // large eyed fish
	TYPE_104_POI2 = 104, // large nosed fish
	TYPE_105_MARTEAU = 105, // bouncing crystal rock 1
	TYPE_106_MOVE_MARTEAU = 106, // bouncing crystal rock 2
	TYPE_107_GROSPIC = 107, // big spiky ball
	TYPE_108_DARK2_PINK_FLY = 108, // Mr Dark electoons
	TYPE_109_PI = 109, // boulder
	TYPE_110_PI_BOUM = 110, // destroyed boulder
	TYPE_111_PI_MUS = 111, // crushed guitar
	TYPE_112_WASHING_MACHINE = 112, // washing machine
	TYPE_113_BAG1 = 113, // mallets
	TYPE_114_UNUSED_114 = 114,
	TYPE_115_BB1_PLAT = 115, // Mr Stone cloud
	TYPE_116_CLOWN_TNT = 116, // TNT clown
	TYPE_117_CLOWN_TNT2 = 117, // TNT clown burst
	TYPE_118_CLOWN_TNT3 = 118, // TNT clown kamikaze
	TYPE_119_TNT_BOMB = 119, // clown TNT
	TYPE_120_BATTEUR_FOU = 120, // red drummer
	TYPE_121_ECLAIR = 121, // Mr Stone flame
	TYPE_122_STONEDOG2 = 122, // Mr Stone stone dog
	TYPE_123_BLACKTOON1 = 123, // antitoon
	TYPE_124_PANCARTE = 124, // return sign
	TYPE_125_BON3 = 125, // candy platform
	TYPE_126_FOURCHETTE = 126, // fork
	TYPE_127_COUTEAU_SUISSE = 127, // swiss knife
	TYPE_128_TIRE_BOUCHON = 128, // corkscrew
	TYPE_129_PETIT_COUTEAU = 129, // small knife
	TYPE_130_BLACKTOON_EYES = 130, // antitoon eyes
	TYPE_131_BAG3 = 131, // mallets spring
	TYPE_132_POI3 = 132, // large mouthed fish
	TYPE_133_SUPERHELICO = 133, // supercopter
	TYPE_134_FALLING_OBJ2 = 134, // plum2
	TYPE_135_ETINC = 135, // flame Mr Stone
	TYPE_136_DEMI_RAYMAN = 136, // tiny Rayman
	TYPE_137_REDUCTEUR = 137, // fairy
	TYPE_138_ROULETTE = 138, // sprinkler (4 heads)
	TYPE_139_MARACAS_BAS = 139, // maracas flying
	TYPE_140_PT_GRAPPIN = 140, // ring
	TYPE_141_NEIGE = 141, // snow
	TYPE_142_ONEUP = 142, // extralife
	TYPE_143_NOVA2 = 143, // item blink
	TYPE_144_LIDOLPINK2 = 144,
	TYPE_145_KILLING_EYES = 145, // bad eyes
	TYPE_146_FLASH = 146, // eyes bolt
	TYPE_147_MST_SCROLL = 147, // moskito huge spiky fruit
	TYPE_148_GRAP_BONUS = 148,
	TYPE_149_CLE_SOL = 149, // treble clef
	TYPE_150_SCORPION = 150, // Mr Skops
	TYPE_151_BULLET = 151, // bullet
	TYPE_152_CAISSE_CLAIRE = 152, // snare drum
	TYPE_153_FEE = 153, // Betilla
	TYPE_154_ROULETTE2 = 154, // sprinkler (4 heads, moving)
	TYPE_155_ROULETTE3 = 155, // sprinkler (3 heads)
	TYPE_156_WALK_NOTE_1 = 156, // wrong note 1
	TYPE_157_EAU = 157, // water
	TYPE_158_PALETTE_SWAPPER = 158, // palette change
	TYPE_159_TIBETAIN_6 = 159, // tibetan 6 platforms
	TYPE_160_TIBETAIN_2 = 160, // tibetan 2 platforms
	TYPE_161_WIZ = 161, // ting
	TYPE_162_UFO_IDC = 162, // flying saucer platform
	TYPE_163_INDICATOR 	= 163, // circuit box
	TYPE_164_GENERATING_DOOR = 164, // gendoor
	TYPE_165_BADGUY3 = 165, // evil livingstone
	TYPE_166_LEVIER = 166, // lever
	TYPE_167_FALLING_OBJ3 = 167, // plum 3
	TYPE_168_CYMBAL1 = 168, // cymbal single
	TYPE_169_CYMBAL2 = 169,
	TYPE_170_RAYON = 170, // Mr Skops beam
	TYPE_171_MST_COPAIN = 171, // Rayman Bzzit cutscene
	TYPE_172_STONEWOMAN = 172, // stoneman (aggressive)
	TYPE_173_BATEAU	= 173, // viking ship
	TYPE_174_PIRATE_POELLE = 174, // space pirate (left)
	TYPE_175_PUNAISE1 = 175, // punaise platform
	TYPE_176_CRAYON_BAS	= 176, // pencil pointing down
	TYPE_177_FALLING_YING = 177, // yin1
	TYPE_178_HERSE_BAS = 178, // pencil pointing down (moving)
	TYPE_179_HERSE_BAS_NEXT = 179, // pencil pointing down (wave)
	TYPE_180_SAXO2 = 180, // Mr Sax (chasing)
	TYPE_181_SCROLL_SAX = 181, // Mr Sax walk speed
	TYPE_182_NOTE3 = 182, // wrong note stationary punchable
	TYPE_183_SAXO3 = 183, // Mr Sax (first encounter)
	TYPE_184_PIRATE_POELLE_D = 184, // space pirate (right)
	TYPE_185_WALK_NOTE_2 = 185, // wrong note 2
	TYPE_186_POELLE_ALWAYS = 186, // pirate pan flying away
	TYPE_187_MAMA_PIRATE = 187, // Viking Mama
	TYPE_188_RUBIS = 188, // red crystal platform
	TYPE_189_MOVE_RUBIS = 189, // red crystal platform (moving)
	TYPE_190_COUTEAU = 190, // Viking Mama sword
	TYPE_191_FALLING_CRAYON = 191, // pencil (falling)
	TYPE_192_SMA_GRAND_LASER = 192, // Space Mama beam
	TYPE_193_SMA_BOMB = 193, // Space Mama pot
	TYPE_194_SMA_BOMB_CHIP = 194, // Space Mama pot part
	TYPE_195_SPIDER_PLAFOND = 195, // spider (ceiling)
	TYPE_196_DARD_PLAFOND = 196, // spider homing dart
	TYPE_197_MEDAILLON_TOON = 197, // medaillon
	TYPE_198_BB12 = 198, // Mr Stone (chase)
	TYPE_199_BB1_VIT = 199, // Mr Stone (chase, walking)
	TYPE_200_BB13 = 200,
	TYPE_201_BB14 = 201,
	TYPE_202_RAY_ETOILES = 202, // stars on Rayman
	TYPE_203_SMA_WEAPON = 203, // washing machine 2
	TYPE_204_BLACK_RAY = 204, // dark Rayman
	TYPE_205_BLACK_FIST = 205, // dark Rayman fist
	TYPE_206_PIEDS_RAYMAN = 206, // Rayman's feet
	TYPE_207_POELLE = 207, // frying pan
	TYPE_208_LANDING_SMOKE = 208, // dust circle
	TYPE_209_FIRE_LEFT = 209, // Mr Dark flame (left)
	TYPE_210_FIRE_RIGHT = 210, // Mr Dark flame (right)
	TYPE_211_BOUT_TOTEM = 211, // Mr Stone swinging totem
	TYPE_212_DARK = 212, // Mr Dark
	TYPE_213_SPACE_MAMA2 = 213, // Space Mama (with washing machine)
	TYPE_214_BOUEE_JOE = 214, // buoy
	TYPE_215_DARK_SORT = 215, // Mr Dark spell fireball
	TYPE_216_ENS = 216, // Eat at Joe's sign
	TYPE_217_MITE2 = 217, // moth 2
	TYPE_218_HYBRIDE_MOSAMS = 218, // Spaxkito
	TYPE_219_CORDE = 219, // flat stone rope
	TYPE_220_PIERREACORDE = 220, // flat stone (with ropes)
	TYPE_221_CFUMEE = 221, // rope dust
	TYPE_222_CORDEBAS = 222, // flat stone (detached rope)
	TYPE_223_HYBRIDE_STOSKO = 223, // Stoneskops
	TYPE_224_STOSKO_PINCE = 224, // Stoneskops claw
	TYPE_225_PIRATE_P_45 = 225, // pan pirate (45 degrees)
	TYPE_226_PIRATE_P_D_45 = 226, // pan pirate 2 (45 degrees)
	TYPE_227_MOSKITO2 = 227, // Moskito
	TYPE_228_PRI = 228, // electric plug
	TYPE_229_PUNAISE2 = 229, // punaise (facing down)
	TYPE_230_PUNAISE3 = 230, // punaise (horizontal)
	TYPE_231_HYB_BBF2_D = 231, // Moskitomama (right)
	TYPE_232_HYB_BBF2_G = 232, // Moskitomama (left)
	TYPE_233_HYB_BBF2_LAS = 233, // Moskitomama beam
	TYPE_234_LAVE = 234, // lava
	TYPE_235_PUNAISE4 = 235, // punaise
	TYPE_236_ANNULE_SORT_DARK = 236, // cancel Mr Dark spell
	TYPE_237_GOMME = 237, // eraser
	TYPE_238_POING_FEE = 238, // Betilla text fist
	TYPE_239_PIRATE_GUETTEUR2 = 239, // viking ship nest
	TYPE_240_CRAYON_HAUT = 240, // pencil (pointing up)
	TYPE_241_HERSE_HAUT = 241, // pencil (pointing up, moving)
	TYPE_242_HERSE_HAUT_NEXT = 242, // pencil (pointing up, wave)
	TYPE_243_MARK_AUTOJUMP_PLAT = 243, // stylo
	TYPE_244_SMA_PETIT_LASER = 244, // Space Mama second beam
	TYPE_245_DUNE = 245, // Joe's beach
	TYPE_246_CORDE_DARK = 246, // Mr Dark separate rope
	TYPE_247_VITRAIL = 247,
	TYPE_248_SKO_PINCE = 248, // Mr Skops claw
	TYPE_249_RIDEAU = 249, // theatre curtain
	TYPE_250_PUNAISE5 = 250, // punaise (up down)
	TYPE_251_VAGUE_DEVANT = 251, // theatre water (front)
	TYPE_252_VAGUE_DERRIERE = 252, // theatre water (back)
	TYPE_253_PLANCHES = 253,
	TYPE_254_SLOPEY_PLAT = 254,
	TYPE_255_INVALID = 255,
	TYPE_256_BREAKOUTBRICK_20 = 256, // breakout brick 20
	TYPE_257_BREAKOUT_GAME = 257, // breakout game
	TYPE_258_NOUGAT = 258, // nougat
	TYPE_259_EDU_LETTER = 259, // EDU letter
	TYPE_260_EDU_DIGIT = 260, // EDU digit
	TYPE_261_EDU_ROAD_SIGN = 261, // EDU road sign
};

// TODO: switch to these?
typedef enum ObjType {
    TYPE_BADGUY1=0,
    TYPE_PLATFORM=1,
    TYPE_POWERUP=2,
    TYPE_LIDOLPINK=3,
    TYPE_NEUTRAL=4,
    TYPE_WIZARD1=5,
    TYPE_FALLING_YING_OUYE=6,
    TYPE_MORNINGSTAR=7,
    TYPE_FALLING_OBJ=8,
    TYPE_BADGUY2=9,
    TYPE_FISH=10,
    TYPE_BOUM=11,
    TYPE_CHASSEUR1=12,
    TYPE_BALLE1=13,
    TYPE_CHASSEUR2=14,
    TYPE_BALLE2=15,
    TYPE_FALLPLAT=16,
    TYPE_LIFTPLAT=17,
    TYPE_BTBPLAT=18,
    TYPE_SPLASH=19,
    TYPE_GENEBADGUY=20,
    TYPE_PHOTOGRAPHE=21,
    TYPE_MOVE_PLAT=22,
    TYPE_RAYMAN=23,
    TYPE_INTERACTPLT=24,
    TYPE_INST_PLAT=25,
    TYPE_CRUMBLE_PLAT=26,
    TYPE_BOING_PLAT=27,
    TYPE_ONOFF_PLAT=28,
    TYPE_AUTOJUMP_PLAT=29,
    TYPE_AUDIOSTART=30,
    TYPE_ONEUP_ALWAYS=31,
    TYPE_DARK_PHASE2=32,
    TYPE_DARK2_SORT=33,
    TYPE_MOVE_AUTOJUMP_PLAT=34,
    TYPE_STONEMAN1=35,
    TYPE_STONEBOMB=36,
    TYPE_TARZAN=37,
    TYPE_GRAINE=38,
    TYPE_NEN_GRAINE=39,
    TYPE_STONEDOG=40,
    TYPE_OUYE=41,
    TYPE_SIGNPOST=42,
    TYPE_STONEMAN2=43,
    TYPE_CLASH=44,
    TYPE_MOVE_OUYE=45,
    TYPE_BB1=46,
    TYPE_STONEBOMB2=47,
    TYPE_FLAMME2=48,
    TYPE_MOVE_START_PLAT=49,
    TYPE_MOSKITO=50,
    TYPE_MST_FRUIT1=51,
    TYPE_MST_FRUIT2=52,
    TYPE_MST_SHAKY_FRUIT=53,
    TYPE_MEDAILLON=54,
    TYPE_MUS_WAIT=55,
    TYPE_STONEWOMAN2=56,
    TYPE_STALAG=57,
    TYPE_CAGE=58,
    TYPE_CAGE2=59,
    TYPE_BIG_CLOWN=60,
    TYPE_WAT_CLOWN=61,
    TYPE_DROP=62,
    TYPE_MOVE_START_NUA=63,
    TYPE_SCROLL=64,
    TYPE_SPIDER=65,
    TYPE_DARD=66,
    TYPE_SWING_PLAT=67,
    TYPE_BIG_BOING_PLAT=68,
    TYPE_STONEBOMB3=69,
    TYPE_TROMPETTE=70,
    TYPE_NOTE=71,
    TYPE_PIRATE_NGAWE=72,
    TYPE_RING=73,
    TYPE_SAXO=74,
    TYPE_PAILLETTE=75,
    TYPE_DESTROYING_DOOR=76,
    TYPE_PIRATE_GUETTEUR=77,
    TYPE_PIRATE_BOMB=78,
    TYPE_STONECHIP=79,
    TYPE_BIGSTONE=80,
    TYPE_CYMBALE=81,
    TYPE_JAUGEUP=82,
    TYPE_EXPLOSION=83,
    TYPE_TIBETAIN=84,
    TYPE_ROLLING_EYES=85,
    TYPE_MARACAS=86,
    TYPE_TAMBOUR1=87,
    TYPE_TAMBOUR2=88,
    TYPE_JOE=89,
    TYPE_NOTE0=90,
    TYPE_NOTE1=91,
    TYPE_NOTE2=92,
    TYPE_BONNE_NOTE=93,
    TYPE_POING=94,
    TYPE_POING_POWERUP=95,
    TYPE_TOTEM=96,
    TYPE_BBL=97,
    TYPE_SPACE_MAMA=98,
    TYPE_RAY_POS=99,
    TYPE_MITE=100,
    TYPE_MORNINGSTAR_MOUNTAI=101,
    TYPE_BNOTE=102,
    TYPE_POI1=103,
    TYPE_POI2=104,
    TYPE_MARTEAU=105,
    TYPE_MOVE_MARTEAU=106,
    TYPE_GROSPIC=107,
    TYPE_DARK2_PINK_FLY=108,
    TYPE_PI=109,
    TYPE_PI_BOUM=110,
    TYPE_PI_MUS=111,
    TYPE_WASHING_MACHINE=112,
    TYPE_BAG1=113,
    TYPE_UNUSED_114=114,
    TYPE_BB1_PLAT=115,
    TYPE_CLOWN_TNT=116,
    TYPE_CLOWN_TNT2=117,
    TYPE_CLOWN_TNT3=118,
    TYPE_TNT_BOMB=119,
    TYPE_BATTEUR_FOU=120,
    TYPE_ECLAIR=121,
    TYPE_STONEDOG2=122,
    TYPE_BLACKTOON1=123,
    TYPE_PANCARTE=124,
    TYPE_BON3=125,
    TYPE_FOURCHETTE=126,
    TYPE_COUTEAU_SUISSE=127,
    TYPE_TIRE_BOUCHON=128,
    TYPE_PETIT_COUTEAU=129,
    TYPE_BLACKTOON_EYES=130,
    TYPE_BAG3=131,
    TYPE_POI3=132,
    TYPE_SUPERHELICO=133,
    TYPE_FALLING_OBJ2=134,
    TYPE_ETINC=135,
    TYPE_DEMI_RAYMAN=136,
    TYPE_REDUCTEUR=137,
    TYPE_ROULETTE=138,
    TYPE_MARACAS_BAS=139,
    TYPE_PT_GRAPPIN=140,
    TYPE_NEIGE=141,
    TYPE_ONEUP=142,
    TYPE_NOVA2=143,
    TYPE_LIDOLPINK2=144,
    TYPE_KILLING_EYES=145,
    TYPE_FLASH=146,
    TYPE_MST_SCROLL=147,
    TYPE_GRAP_BONUS=148,
    TYPE_CLE_SOL=149,
    TYPE_SCORPION=150,
    TYPE_BULLET=151,
    TYPE_CAISSE_CLAIRE=152,
    TYPE_FEE=153,
    TYPE_ROULETTE2=154,
    TYPE_ROULETTE3=155,
    TYPE_WALK_NOTE_1=156,
    TYPE_EAU=157,
    TYPE_PALETTE_SWAPPER=158,
    TYPE_TIBETAIN_6=159,
    TYPE_TIBETAIN_2=160,
    TYPE_WIZ=161,
    TYPE_UFO_IDC=162,
    TYPE_INDICATOR=163,
    TYPE_GENERATING_DOOR=164,
    TYPE_BADGUY3=165,
    TYPE_LEVIER=166,
    TYPE_FALLING_OBJ3=167,
    TYPE_CYMBAL1=168,
    TYPE_CYMBAL2=169,
    TYPE_RAYON=170,
    TYPE_MST_COPAIN=171,
    TYPE_STONEWOMAN=172,
    TYPE_BATEAU=173,
    TYPE_PIRATE_POELLE=174,
    TYPE_PUNAISE1=175,
    TYPE_CRAYON_BAS=176,
    TYPE_FALLING_YING=177,
    TYPE_HERSE_BAS=178,
    TYPE_HERSE_BAS_NEXT=179,
    TYPE_SAXO2=180,
    TYPE_SCROLL_SAX=181,
    TYPE_NOTE3=182,
    TYPE_SAXO3=183,
    TYPE_PIRATE_POELLE_D=184,
    TYPE_WALK_NOTE_2=185,
    TYPE_POELLE_ALWAYS=186,
    TYPE_MAMA_PIRATE=187,
    TYPE_RUBIS=188,
    TYPE_MOVE_RUBIS=189,
    TYPE_COUTEAU=190,
    TYPE_FALLING_CRAYON=191,
    TYPE_SMA_GRAND_LASER=192,
    TYPE_SMA_BOMB=193,
    TYPE_SMA_BOMB_CHIP=194,
    TYPE_SPIDER_PLAFOND=195,
    TYPE_DARD_PLAFOND=196,
    TYPE_MEDAILLON_TOON=197,
    TYPE_BB12=198,
    TYPE_BB1_VIT=199,
    TYPE_BB13=200,
    TYPE_BB14=201,
    TYPE_RAY_ETOILES=202,
    TYPE_SMA_WEAPON=203,
    TYPE_BLACK_RAY=204,
    TYPE_BLACK_FIST=205,
    TYPE_PIEDS_RAYMAN=206,
    TYPE_POELLE=207,
    TYPE_LANDING_SMOKE=208,
    TYPE_FIRE_LEFT=209,
    TYPE_FIRE_RIGHT=210,
    TYPE_BOUT_TOTEM=211,
    TYPE_DARK=212,
    TYPE_SPACE_MAMA2=213,
    TYPE_BOUEE_JOE=214,
    TYPE_DARK_SORT=215,
    TYPE_ENS=216,
    TYPE_MITE2=217,
    TYPE_HYBRIDE_MOSAMS=218,
    TYPE_CORDE=219,
    TYPE_PIERREACORDE=220,
    TYPE_CFUMEE=221,
    TYPE_CORDEBAS=222,
    TYPE_HYBRIDE_STOSKO=223,
    TYPE_STOSKO_PINCE=224,
    TYPE_PIRATE_P_45=225,
    TYPE_PIRATE_P_D_45=226,
    TYPE_MOSKITO2=227,
    TYPE_PRI=228,
    TYPE_PUNAISE2=229,
    TYPE_PUNAISE3=230,
    TYPE_HYB_BBF2_D=231,
    TYPE_HYB_BBF2_G=232,
    TYPE_HYB_BBF2_LAS=233,
    TYPE_LAVE=234,
    TYPE_PUNAISE4=235,
    TYPE_ANNULE_SORT_DARK=236,
    TYPE_GOMME=237,
    TYPE_POING_FEE=238,
    TYPE_PIRATE_GUETTEUR2=239,
    TYPE_CRAYON_HAUT=240,
    TYPE_HERSE_HAUT=241,
    TYPE_HERSE_HAUT_NEXT=242,
    TYPE_MARK_AUTOJUMP_PLAT=243,
    TYPE_SMA_PETIT_LASER=244,
    TYPE_DUNE=245,
    TYPE_CORDE_DARK=246,
    TYPE_VITRAIL=247,
    TYPE_SKO_PINCE=248,
    TYPE_RIDEAU=249,
    TYPE_PUNAISE5=250,
    TYPE_VAGUE_DEVANT=251,
    TYPE_VAGUE_DERRIERE=252,
    TYPE_PLANCHES=253,
    TYPE_SLOPEY_PLAT=254,
    TYPE_INVALID=255
} ObjType;

typedef enum btyp_enum {
    BTYP_NONE=0,
    BTYP_CHDIR=1,
    BTYP_SOLID_RIGHT_45=2,
    BTYP_SOLID_LEFT_45=3,
    BTYP_SOLID_RIGHT1_30=4,
    BTYP_SOLID_RIGHT2_30=5,
    BTYP_SOLID_LEFT1_30=6,
    BTYP_SOLID_LEFT2_30=7,
    BTYP_HURT=8,
    BTYP_RESSORT=9, /*bounce*/
    BTYP_WATER=10,
    BTYP_LIANE=12, /*climb*/
    BTYP_SOLID_PASSTHROUGH=14,
    BTYP_SOLID=15,
    BTYP_SLIPPERY_RIGHT_45=18,
    BTYP_SLIPPERY_LEFT_45=19,
    BTYP_SLIPPERY_RIGHT1_30=20,
    BTYP_SLIPPERY_RIGHT2_30=21,
    BTYP_SLIPPERY_LEFT1_30=22,
    BTYP_SLIPPERY_LEFT2_30=23,
    BTYP_SPIKES=24,
    BTYP_CLIFF=25,
    BTYP_SLIPPERY=30,
} BlockType;

typedef struct rayevts_t {
	u8 poing : 1;               //1
	u8 hang : 1;                //2
	u8 helico : 1;              //4
	u8 super_helico : 1;        //8
	u8 handstand_dash : 1;      //0x10
	u8 handstand : 1;           //0x20
	u8 magicseed : 1;           //0x40
	u8 grab : 1;                //0x80
	u8 run : 1;                 //0x100
	u8 tiny : 1;                //0x200
	u8 firefly : 1;             //0x400
	u8 force_run : 2;           //0x800 and 0x1000
	u8 reverse : 2;             //0x2000 and 0x4000
	u8 squashed : 1;            //0x8000
} rayevts_t;

typedef struct finbosslevel_t {
    u8 bzzit : 1;           // 1
    u8 moskito : 1;         // 2
    u8 mr_sax : 1;          // 4
    u8 mr_stone : 1;        // 8
    u8 viking_mama : 1;     // 0x10
    u8 space_mama : 1;      // 0x20
    u8 mr_skops : 1;        // 0x40
    u8 mr_dark : 1;         // 0x80
    u8 crazy_drummer : 1;   // 0x100
    u8 helped_joe_1 : 1;    // 0x200
    u8 helped_joe_2 : 1;    // 0x400
    u8 helped_musician : 1; // 0x800
} finbosslevel_t;

typedef struct wi_save_zone_t {
    u8 unlocked : 1;
    u8 status_flag_4_bit: 1;
    u8 cages : 3;
} wi_save_zone_t;

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
	obj_flags_4_triggered = 4,
	obj_flags_8_flipped = 8,
	obj_flags_0x10_read_commands = 0x10,
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
	//obj_type_flags_bit_31                       = 1 << 31, // byte 3, 0x80
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

typedef void (draw_func_t)(i32 x, i32 sprite_field_A, i32 y, vec2b_t size, u8* draw_buf, u8* image_data);
typedef i16 (scene_func_t)(u32 a1);
typedef i32 (calcbloc_func_t)(i32 a1, i32 a2);
typedef void (void_func_t)(void);

