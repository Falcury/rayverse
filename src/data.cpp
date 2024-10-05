
// Note: MSVC 6 doesn't support __VA_ARGS__

app_state_t global_app_state;
game_state_t* global_game;

rgb_palette_t fade_source_palette;
u8 fade_mode; //CFA87 (?)
u16 fade_temp[256*3];

// engine.cpp

ogg_t ogg_cd_track;
bool is_ogg_playing;
bool is_ogg_finished;

i32 VitesseYStageNameLevel[30]; //C2248
i32 VitesseXStageNameWorld[30]; //C22C0
i32 VitesseXStageNameLevel[30]; //C2338
i32 PositionYStageNameWorld[50]; //C23B0
i32 VitesseYStageNameWorld[30]; //C2478
i32 PositionXStageNameLevel[50]; //C24F0
i32 PositionXStageNameWorld[50]; //C25B8
i32 PositionYStageNameLevel [50]; //C2680
calcbloc_func_t* calcblocrecal[64]; //C2748
calcbloc_func_t* calcbloc1[64]; //C2848
calcbloc_func_t* calcbloc2[64]; //C2948
calcbloc_func_t* calcblocfloor[64]; //C2A48
char* language_txt[304]; //C2B48

i16 options_jeu_KeyJump; //C2F88 // TODO: check these names; create struct or array?
i16 options_jeu_KeyWeapon; //C2F8A
i16 options_jeu_KeyUnknown_default_02; //C2F8C
i16 options_jeu_KeyAction; //C2F8E
i16 options_jeu_music_enabled; //C2F90
i16 options_jeu_sound_volume; //C2F92
i16 options_jeu_is_stereo; //C2F94

i16 notbut[4]; //C97F0

u8 save_zone[2592]; //CAFA0
record_t record; //CAF50
i16 tot_nb_flocs[8]; //CAF68
i16 floc_ind; //CAF78
i16 allowed_time[192]; //CB9C0
u8 bonus_perfect[24]; //CBB40

level_t level; //CCDF0
level_t level_obj; //CCDFC
u8 ecroule_rubis_list[8]; //CCE00
u8 saveobj[64]; //CCE08
eta_t** loaded_eta[100]; //CCE48
i32 idle_ticks; //CCFD8
char rayman_path[100]; //CCFDC
u32 Port; //CD040
u32 DeviceID; //CD044
u32 Irq; //CD048
u32 Param; //CD04C
u32 Dma; //CD050
u32 nb_blocks_plein; //CD054
u32 nb_total_blocks; //CD058
u32 RaymanExeSize; //CD05C
i32 Bloc_lim_W2_Aff; //CD060
i32 Bloc_lim_W1_Aff; //CD064
u32 RaymanExeCheckSum2; //CD068
u32 RaymanExeCheckSum3; //CD06C
u32 RaymanExeCheckSum1; //CD070
i32 Bloc_lim_H2_Glob; //CD074
i32 Bloc_lim_H1_Glob; //CD078
i32 Bloc_lim_W2_Glob; //CD07C
i32 Bloc_lim_W1_Glob; //CD080
i32 Bloc_lim_H2_Aff; //CD084
i32 Bloc_lim_H1_Aff; //CD088
i32 TailleStageNameLevel; //CD08C
i32 CouleurStageNameLevel; //CD090
i32 TailleStageNameWorld; //CD094
i32 Bloc_lim_W2; //CD098
i32 Bloc_lim_W1; //CD09C
i32 Bloc_lim_H2; //CD0A0
i32 Bloc_lim_H1; //CD0A4
i32 CouleurStageNameWorld; //CD0A8
i32 HauteurJumelle; //CD0AC
i32 LargeurJumelle; //CD0B0
i32 JumelleZoomAmp; //CD0B4
void* drawflocon6NormalETX; //CD0B8
void* drawflocon7NormalETX; //CD0BC
i32 JumellePosY; //CD0C0
i32 JumellePosX; //CD0C4
i32* JumelleZoomDef; //CD0C8
i32 LongueurStageNameWorld; //CD0CC
i32 LongueurStageNameLevel; //CD0D0
void* drawpluie7NormalETX; //CD0D4
void* drawpluie4NormalETX; //CD0D8
i32 JumelleYMin; //CD0DC
void* drawpluie5NormalETX; //CD0E0
i32 JumelleXMin; //CD0E4
void* drawflocon4NormalETX; //CD0E8
i32 RayonJumelle; //CD0EC
void* drawpluie6NormalETX; //CD0F0
void* dword_CD0F4; //CD0F4
draw_func_t* DrawSpriteColorNormalEtX; //CD0F8
i32 dword_CD0FC; //CD0FC
void* fplotNormalETX; //CD100
void* drawflocon5NormalETX; //CD104
void* drawflocon2NormalETX; //CD108
draw_func_t* DrawSpriteNormalEtX; //CD10C
void* drawflocon1NormalETX; //CD110
void* dword_CD114; //CD114
draw_func_t* DrawSpriteFlipNormalEtX; //CD118
void* dword_CD11C; //CD11C
void* Swap_And_Test_Joystick; //CD120
void* Copy_Plan0_To_Buf; //CD124
void* DrawBufferNormalIni; //CD128
void* drawflocon3NormalETX; //CD12C
u8* EffetBufferNormal; //CD130
i32 TempsDemo; //CD134
u8* DrawBufferNormal; //CD138
void* anim_func; //CD13C
char* save_ray[3]; //CD794

options_t options; //CD7A4
u8* draw_buffer; //CD7B8

rgb_palette_t color_palettes[3]; //C7F88

u32 TailleMainMemLevel; //CD844
u32 TailleMainMemSprite; //CD848
mem_t* main_mem_level; //CD84C
mem_t* main_mem_sprite; //CD860

u32 TailleMainMemTmp; //CD86C
mem_t* main_mem_fix; //CD874
mem_t* main_mem_world; //CD880
u32 TailleMainMemFix; //CD898
u32 TailleMainMemWorld; //CD89C
mem_t* main_mem_tmp; //CD8A0
mem_t* temp_mem_buf; //CD88C

u32 flocon_tab; //CD8BC
u32 map_time; //CD8C0
u32 time_left; //CD8C4

obj_t mapobj[25]; //CD8CC

u32 background_DES; //CE634
map_data_t mp; //CE638
u32 dword_CE644; //CE644
u32 dword_CE648; //CE648
obj_t ray; //CE650

obj_t* poing_obj; //CE6DC
obj_t* alpha; //CE6E0
u8* alpha_image_atlas; //CE6E4
i16 alpha_sprite_count; //CE6E8
obj_t div_obj; //CE6EC
poing_t poing; //CE770
obj_t bigray; //CE784
obj_t raylittle; //CE808

obj_t clockobj; //CED44
obj_t* alpha2; //CEDC8
u8* alpha2_image_atlas; //CEDCC
i16 alpha2_sprite_count; //CEDD0
obj_t rms; //CEDD4
u8* link_init; //CEE58
obj_t raytmp; //CEE5C
u8 skops_lave_obj[20]; //CEEE0
i32 pixels_enfonce; //CEEF4

u8 plan0_num_pcx[10]; //CEF47
status_bar_t status_bar; //CEF52
u8 ray_max_hitp; //CEF5B

i16 numero_palette_special; //CF740
i16 ProchainEclair; //CF742
i16 joyy0; //CF744
i16 joyx0; //CF746
i16 xpadmax; //CF748
i16 ypadmax; //CF74A
i16 ypadcentre; //CF74C
i16 xpadcentre; //CF74E
i16 xpadmin; //CF750
i16 ypadmin; //CF752
i16 option_exit; //CF754
i16 LEFT_MAP_BORDER; //CF756
i16 xmapmaxini; //CF758
i16 RIGHT_MAP_BORDER; //CF75A
i16 ymapmaxini; //CF75C
i16 HeightNworld; //CF75E
i16 WidthNworld; //CF760
u16 SauveRayEvtsDemo; //CF762
i16 nb_credits_lines; //CF764
i16 snd_flag_medaillon; //CF766
i16 RunTimeDemo; //CF768
i16 NumDemo; //CF76A
i16 animation_attente; //CF76C
i16 basex; //CF76E
i16 compteur_clignote; //CF772
i16 compteur_attente; //CF774
i16 sortie_save; //CF776
i16 realisation_effectuee; //CF778
i16 delai_stereo; //CF77A
i16 selection_effectuee; //CF77C
i16 repetition; //CF77E
i16 vignet_joe_affichee; //CF780
i16 clignotement; //CF782
i16 fin_saisie_nom; //CF784
i16 joe_exp_probleme; //CF786
i16 fichier_existant; //CF788
i16 compteur; //CF78A
i16 position; //CF78C
i16 sortie_options; //CF78E
i16 affiche_bon_ecran; //CF790
i16 delai_barre; //CF792
i16 max_compteur; //CF794
i16 prise_branchee; //CF796
i16 max_sound; //CF798
i16 delai_repetition; //CF79A
i16 ecart_barre; //CF79C
i16 word_CF79E; //CF79E
i16 action; //CF7A2
i16 nbre_options; //CF7A4
i16 positiony2; //CF7A6
i16 old_x_luc; //CF7A8
i16 positionx2; //CF7AA
i16 debut_titre; //CF7AC
i16 old_y_luc; //CF7AE
i16 choix_menu; //CF7B0
i16 ADDLUCLIP; //CF7B2
i16 ecarty; //CF7B4
i16 ecartx; //CF7B6
i16 debut_sortie; //CF7B8
i16 ecart_options; //CF7BA
i16 fichier_selectionne; //CF7BC
i16 positiony; //CF7BE
i16 positionx; //CF7C0
i16 fichier_a_copier; //CF7C2
i16 debut_options; //CF7C4
i16 rayon_luciole; //CF7C6
i16 x_luc; //CF7C8
i16 y_luc; //CF7CA
i16 coeffktxt; //CF7CC
i16 y_main_luc; //CF7CE
i16 x_main_luc; //CF7D0
i16 vx_luc; //CF7D2
i16 n_ray; //CF7D4
i16 vy_luc; //CF7D6
i16 ktxtenx; //CF7D8
i16 ktxteny; //CF7DA
i16 rotationtxt; //CF7DC
i16 plan0_width; //CF7DE
i16 yp0; //CF7E0
i16 plan2_height; //CF7E2
i16 plan0_height; //CF7E4
i16 word_CF7E6; //CF7E6
i16 display_mode; //CF7E8
i16 fade_speed; //CF7EA
i16 word_CF7EE; //CF7EE
i16 word_CF7F0; //CF7F0
i16 joy_buttonB1; //CF7F2
i16 joy_buttonB2; //CF7F4
i16 joy_buttonA1; //CF7F6
i16 joy_buttonA2; //CF7F8
i16 SNSEQ_list_ptr; //CF7FA
i16 current_star; //CF7FC
i16 SNSEQ_ptr; //CF7FE
i16 SNSEQ_no; //CF800
i16 MapAvecPluieOuNeige; //CF802
i16 skops_ecroule_plat; //CF804
i16 bonus_map; //CF806
i16 bateau_obj_id; //CF808
i16 sko_nb_hit; //CF80A
i16 mama_pirate_obj_id; //CF80C
i16 sko_nb_frap; //CF80E
i16 loop_time; //CF810
i16 bat_nb_frap; //CF812
i16 cb_ball_obj_id; //CF814
i16 sko_phase; //CF816
u16 SauveRayEvts; //CF818
u16 RayEvts; //CF81A
u16 finBossLevel; //CF81E
i16 bloc_lim_H2; //CF820
i16 png_or_fee_id; //CF822
i16 loop_nb_trames; //CF824
i16 loop_timing; //CF826
i16 VENT_Y; //CF828
i16 VENT_X; //CF82A
i16 id_Cling_1up; //CF82C
i16 num_level_choice; //CF82E
i16 id_cling_old; //CF830
i16 ywldmapsave; //CF832
i16 xwldmapsave; //CF834
i16 PROJ_CENTER_Y; //CF836
i16 PROJ_CENTER_X; //CF838
i16 num_world_choice; //CF83A
i16 ray_speed_inv; //CF83C
i16 ymapinit; //CF83E
i16 ray_clic; //CF840
i16 id_obj_grabbed; //CF842
i16 xmapinit; //CF844
i16 id_Cling_pow; //CF846
i16 ray_between_clic; //CF848
i16 bossYToReach; //CF84A
i16 screen_trembling3; //CF84C
i16 screen_trembling2; //CF84E
i16 bossXToReach; //CF850
i16 screen_trembling; //CF852
i16 scroll_start_y; //CF854
i16 bossScrollStartY; //CF856
i16 bossScrollStartX; //CF858
i16 bossScrollEndX; //CF85A
i16 bossScrollEndY; //CF85C
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
i16 num_level; //CF87C
u16 word_CF87E; //CF87E
i16 new_level; //CF880
u16 water_obj_id; //CF882
i16 ymapmax; //CF884
u16 reduced_rayman_id; //CF886
u16 fee_obj_id; //CF888
u16 word_CF88A; //CF88A
i16 num_world; // 0xCF88C
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
u16 sko_last_action; //CF8DC
u16 skops_beam_on; //CF8DE
i16 skops_final_y; //CF8E0
i16 skops_final_x; //CF8E2
u16 word_CF8E4; //CF8E4
i16 skops_beam_speed; //CF8E6
i16 skops_beam_ds; //CF8E8
i16 skops_beam_dy; //CF8EA
u16 rubis_list_calculated; //CF8EC
i16 sko_rayon_x; //CF8EE
i16 sko_rayon_y; //CF8F0
u16 path_has_changed; //CF8F2
u16 inter_select; //CF8F4



u8 horloge[25]; //CF9F5
u8 windows_version_major; //CFA0E
u8 windows_version_minor; //CFA0F
u8 is_master_mode; //CFA10
u8 byte_CFA11; //CFA11
u8 windows_version; //CFA12
u8 SonLimite; //CFA13
u8 byte_CFA14; //CFA14
u8 byte_CFA15; //CFA15
u8 byte_CFA16; //CFA16
u8 CarteSonAutorisee; //CFA17
u8 is_background_clearing_needed; //CFA18
u8 byte_CFA19; //CFA19
u8 FondAutorise; //CFA1A // background available
u8 byte_CFA1B; //CFA1B
u8 Mode_Pad; //CFA1C
u8 something_with_cfg_25_default_00_joy; //CFA1D (?)
u8 Port_Pad; //CFA1E
u8 ScrollDiffOn; //CFA1F
u8 P486; //CFA20
u8 type_fnd; //CFA21
u8 no_fnd; //CFA22
u8 is_joypad_mode; //CFA23
u8 type_fndscrX; //CFA24
u8 NumCard; //CFA25
u8 TheProcesseur; //CFA26
u8 byte_CFA27; //CFA27
u8 world_unk_offset_7_always_DA; //CFA28
u8 byte_CFA29; //CFA29
u8 byte_CFA2A; //CFA2A
u8 world_unk_offset_6; //CFA2B
u8 is_limited_mode; //CFA2C
u8 PositionStageNameCalcule; //CFA2D
u8 ModeAutoJumelle; //CFA2E
u8 ChangeJumelleSizeOK; //CFA2F
u8 JumelleZoomActif; //CFA30
u8 ParamZoomChange; //CFA31
u8 JumelleEffetAutorise; //CFA32 // zoom effect available
u8 NormalModeAutorise; //CFA33    // normal mode available
u8 first_credit; //CFA34
u8 language; //CFA35
u8 ModeDemo; //CFA36
u8 menuEtape; //CFA37
u8 MENU_SUITE; //CFA38
u8 MENU_RETURN; //CFA39
u8 last_credit; //CFA3A
u8 nb_continue; //CFA3B
u8 byte_CFA3C; //CFA3C
u8 NBRE_SAVE; //CFA3D
u8 byte_CFA3E; //CFA3E
u8 nouvelle_partie; //CFA3F
u8 PROC_EXIT; //CFA40
u8 First_Hit; //CFA41
u8 First_Menu; //CFA42
u8 dontdoit; //CFA43
u8 vignet_to_display; //CFA44
u8 chemin_percent; //CFA45
u8 You_Win; //CFA46 (?)
u8 display_Vignet; //CFA47
u8 colour; //CFA48
u8 rayOnMsWasThere; //CFA49
u8 freeze; //CFA4A (?)
u8 need_skip_timer; //CFA4B (?)
u8 VGA_FREQ; //CFA4C
u8 saved_current_pal; //CFA4D
u8 use_sync; //CFA4E
u8 pos_YN; //CFA4F
u8 MusicCdActive; //CFA50
u8 input_mode; //CFA51
u8 in_pause; //CFA52
u8 dans_la_map_monde; //CFA53
u8 ray_on_poelle; //CFA54
u8 nb_wiz_collected; //CFA55
u8 nb_wiz; //CFA56
u8 is_fee; //CFA57
u8 new_txt_fee; //CFA58
u8 byte_CFA59; //CFA59
u8 dir_on_wldmap; //CFA5A
u8 Nb_total_cages; //CFA5B
u8 fin_continue; //CFA5C
u8 display_txt_fee; //CFA5D
u8 ray_inertia; //CFA5E
u8 old_num_world; //CFA5F
u8 world_index; // 0xCFA60
u8 ALL_WORLD; // 0xCFA61
u8 gele; // 0xCFA62
u8 zoom_mode; // 0xCFA63
u8 bossReachingAccuracyX; // CFA64
u8 bossReachingAccuracyY; // CFA65
u8 lidol_to_allocate; // CFA66
u8 saveCurrentBossAction; //CFA67
u8 scrollLocked; //CFA68
u8 saveBossEncounter; //CFA69
u8 bossReachingTimer; //CFA6A
u8 boss_safe_timer; //CFA6B
u8 currentBossActionIsOver; //CFA6C
u8 byte_CFA6D; //CFA6D
u8 bossEncounter; //CFA6E
u8 currentBossAction; //CFA6F
u8 gerbe; //CFA70
u8 first_boss_meet; //CFA71
u8 ray_last_ground_btyp; //CFA72
u8 ray_mode_speed; //CFA73
u8 fin_de_rayman; //CFA74
u8 ray_superhelico_bis; //CFA75
u8 fin_boss; //CFA76
u8 ray_in_fee_zone; //CFA77
u8 boss_mort; //CFA78
u8 byte_CFA79; //CFA79
u8 fin_du_jeu; //CFA7A
u8 ray_se_noie; //CFA7B
u8 hand_btyp; //CFA7C
u8 button_released; //CFA7D
u8 dead_time; //CFA7E
u8 hand_btypg; //CFA7F
u8 hand_btypd; //CFA80
u8 nb_cymbal_in_map; //CFA81
u8 current_pal_id; //CFA82
u8 active_palette; //CFA83
u8 pal_id_at_wizard; //CFA84
u8 save_save; //CFA85
u8 NewMs; //CFA86
u8 fade; //CFA87
u8 ray_stack_is_full; // (?) CFA88
u8 TextDark2_Affiche; //CFA89
u8 black_pos_in_stack; //CFA8A
u8 ray_pos_in_stack; //CFA8B
u8 in_air_because_hit; //CFA8C
u8 dark_phase; //CFA8D
u8 IsBossThere; //CFA8E
u8 skops_enfonce_enable; //CFA8F
u8 WaitForAnim; //CFA90
u8 ecroule_plat_index; //CFA91
u8 byte_CFA92; //CFA92
u8 FinAnim; //CFA93
u8 Phase; // (?) CFA94
u8 WaitForFinAtan; //CFA95
u8 skops_nb_lave; //CFA96
u8 nb_trames_ecrase; //CFA97
u8 RaymanDansUneMapDuJeu; //CFA98
u8 life_becoz_wiz; //CFA99
u8 nb_fix_eta; //CFA9A
u8 nb_loaded_eta; //CFA9B
u8 byte_CFA9C; //CFA9C
i32 CompteurTrameAudio; //CFAA0


u16 nb_des; //9251C
u16 nb_fix_des; //9251E
i16 dhspeed; //92520
i16 dvspeed; //92522

//92524
u8 ecroule_rubis_order[24] = {2, 1, 3, 0, 4, 7, 6, 5, 1, 3, 2, 0, 4, 7, 6, 5, 4, 1, 3, 2, 0, 7, 6, 5};

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

//97344
i16 RandArray[256] = {
		4, 343, 34, 364, 222, 549, 200, 717, 972, 281, 454,
		111, 714, 577, 42, 169, 835, 701, 782, 847, 982, 224,
		437, 975, 859, 945, 830, 461, 619, 677, 614, 562, 737,
		116, 415, 124, 687, 486, 502, 577, 352, 889, 270, 184,
		433, 711, 167, 551, 660, 638, 3, 806, 275, 472, 395,
		385, 595, 617, 286, 174, 824, 302, 341, 418, 246, 423,
		55, 505, 1007, 1, 419, 71, 75, 260, 998, 363, 414, 201,
		201, 216, 255, 776, 910, 927, 753, 472, 544, 36, 133,
		469, 495, 611, 259, 117, 718, 664, 907, 896, 43, 566,
		522, 635, 789, 259, 222, 241, 343, 579, 136, 628, 31,
		971, 217, 827, 644, 732, 749, 565, 585, 172, 340, 890,
		844, 629, 976, 1015, 1005, 755, 522, 302, 735, 292,
		655, 504, 179, 572, 393, 793, 398, 666, 130, 148, 344,
		941, 670, 664, 726, 671, 677, 738, 1021, 431, 575, 385,
		1007, 15, 964, 463, 243, 559, 506, 641, 724, 597, 808,
		442, 3, 855, 733, 1010, 468, 361, 961, 659, 578, 715,
		228, 317, 919, 73, 980, 627, 83, 867, 744, 690, 947,
		505, 292, 697, 317, 363, 773, 1000, 943, 250, 420, 935,
		429, 346, 907, 149, 148, 330, 155, 639, 972, 275, 102,
		1003, 251, 221, 87, 540, 898, 926, 271, 629, 915, 671,
		85, 528, 842, 868, 756, 628, 560, 997, 1006, 65, 375,
		747, 99, 697, 346, 118, 388, 258, 516, 20, 1020, 1,
		84, 253, 555, 432, 225, 470, 149, 209, 425, 9, 475,
		466, 186, 10,
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

medaillion_t t_world_info[24] = {
		{29,  264, 18, 0,  0,  1,  1, 0, 1, 1,  2, 0, 0, 0, (char*)0xBFEA8}, // 0: Pink Plant Woods
		{100, 254, 2,  1,  0,  4,  0, 0, 1, 5,  0, 0, 0, 0, (char*)0xBFEA8}, // 1: Anguish Lagoon
		{90,  212, 2,  1,  3,  2,  0, 0, 1, 9,  2, 0, 0, 0, (char*)0xBFEA8}, // 2: Swamps of Forgetfulness
		{47,  200, 3,  3,  3,  2,  0, 0, 1, 12, 1, 0, 0, 0, (char*)0xBFEA8}, // 3: Moskito's Nest
		{180, 215, 5,  19, 1,  4,  0, 0, 2, 1,  2, 0, 0, 0, (char*)0xBFEA8}, // 4: Bongo Hills
		{185, 175, 8,  4,  6,  8,  0, 0, 2, 7,  0, 0, 0, 0, (char*)0xBFEA8}, // 5: Allegro Presto
		{135, 160, 20, 6,  7,  5,  0, 0, 2, 12, 2, 0, 0, 0, (char*)0xBFEA8}, // 6: Gong Heights
		{91,  143, 7,  7,  7,  6,  0, 0, 2, 14, 3, 0, 0, 0, (char*)0xBFEA8}, // 7: Mr Sax's Hullaballoo
		{212, 133, 9,  5,  5,  8,  0, 0, 3, 1,  1, 0, 0, 0, (char*)0xBFEA8}, // 8: Twilight Gulch
		{209, 92,  21, 8,  9,  10, 0, 0, 3, 3,  0, 0, 0, 0, (char*)0xBFEA8}, // 9: The Hard Rocks
		{257, 80,  10, 11,  9, 10, 0, 0, 3, 6,  1, 0, 0, 0, (char*)0xBFEA8}, // 10: Mr Stone's Peaks
		{234, 235, 10, 11, 11, 12, 0, 0, 4, 1,  1, 0, 0, 0, (char*)0xBFEA8}, // 11: Eraser Plains
		{278, 190, 13, 11, 11, 12, 0, 0, 4, 5,  5, 0, 0, 0, (char*)0xBFEA8}, // 12: Pencil Pentathlon
		{290, 145, 22, 12, 13, 14, 0, 0, 4, 8,  1, 0, 0, 0, (char*)0xBFEA8}, // 13: Space Mama's Crater
		{335, 180, 13, 15, 13, 14, 0, 0, 5, 1,  0, 0, 0, 0, (char*)0xBFEA8}, // 14: Crystal Palace
		{312, 228, 14, 23, 15, 16, 0, 0, 5, 3,  5, 0, 0, 0, (char*)0xBFEA8}, // 15: Eat at Joe's
		{360, 220, 17, 16, 15, 16, 0, 0, 5, 9,  2, 0, 0, 0, (char*)0xBFEA8}, // 16: Mr Skops' Stalactites
		{357, 80,  17, 16, 17, 17, 0, 0, 6, 1,  2, 0, 0, 0, (char*)0xBFEA8}, // 17: Mr Dark's Dare
		{10,  212, 18, 0,  18, 18, 0, 0, 7, 20, 5, 0, 0, 0, (char*)0xBFEA8}, // 18: Save Game (Jungle)
		{182, 266, 4,  19, 19, 19, 0, 0, 7, 20, 3, 0, 0, 0, (char*)0xBFEA8}, // 19: Save Game (Music 1)
		{145, 97,  20, 6,  20, 20, 0, 0, 7, 20, 0, 0, 0, 0, (char*)0xBFEA8}, // 20: Save Game (Music 2)
		{190, 45,  21, 9,  21, 21, 0, 0, 7, 20, 2, 0, 0, 0, (char*)0xBFEA8}, // 21: Save Game (Mountain)
		{276, 115, 22, 13, 22, 22, 0, 0, 7, 20, 1, 0, 0, 0, (char*)0xBFEA8}, // 22: Save Game (Picture)
		{306, 267, 15, 23, 23, 23, 0, 0, 7, 20, 1, 0, 0, 0, (char*)0xBFEA8}, // 23: Save Game (Cave)
};

// sound
u8 base_snd8b_headers[0x800];
u8* base_snd8b_data;


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

// 95828
u8 DemoRecord[1500] = {
		0, 41, 32, 8, 0, 19, 32, 27, 0, 30, 4, 2, 0, 60, 32, 11, 0, 16, 4, 12, 0, 69, 4, 7, 36, 20, 32, 27, 0, 74,
		128, 42, 160, 4, 32, 8, 36, 11, 32, 124, 36, 19, 32, 58, 0, 28, 16, 46, 0, 13, 128, 23, 144, 1, 16, 23, 144,
		1, 128, 31, 144, 2, 16, 52, 0, 33, 128, 3, 144, 1, 16, 23, 144, 1, 128, 27, 144, 3, 16, 74, 0, 68, 128, 31,
		144, 1, 16, 4, 20, 12, 16, 14, 0, 8, 160, 29, 128, 2, 144, 1, 128, 202, 144, 13, 16, 35, 0, 17, 32, 36, 0,
		19, 32, 13, 160, 1, 128, 22, 160, 25, 32, 38, 36, 14, 32, 29, 0, 16, 128, 25, 144, 17, 128, 1, 144, 1, 128,
		12, 160, 48, 32, 35, 36, 4, 32, 117, 36, 9, 32, 15, 0, 85, 16, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 32, 12, 36, 19, 4, 2, 20, 1, 16, 9, 20, 1, 4, 12, 0, 26, 4, 5, 0,
		43, 16, 7, 0, 7, 32, 7, 0, 2, 4, 3, 0, 2, 16, 1, 80, 1, 16, 6, 0, 7, 32, 8, 0, 75, 4, 5, 0, 63, 16, 6, 0,
		14, 32, 9, 0, 9, 4, 6, 0, 12, 32, 55, 0, 5, 16, 2, 80, 28, 16, 1, 4, 5, 0, 20, 16, 7, 0, 8, 32, 5, 0, 56, 4,
		4, 0, 14, 16, 8, 0, 34, 32, 6, 0, 25, 84, 4, 80, 23, 16, 21, 0, 8, 32, 22, 0, 15, 32, 11, 0, 1, 4, 6, 0,
		13, 16, 6, 0, 82, 4, 4, 0, 88, 32, 4, 36, 4, 32, 44, 0, 10, 16, 2, 80, 23, 16, 2, 0, 13, 4, 4, 0, 10, 16,
		3, 0, 10, 32, 4, 0, 16, 16, 5, 0, 52, 4, 4, 0, 51, 16, 4, 0, 13, 32, 3, 0, 27, 4, 5, 0, 71, 4, 4, 0, 81, 4,
		4, 0, 33, 16, 12, 0, 19, 32, 6, 0, 5, 4, 7, 0, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 29, 32, 50, 36, 8, 32, 13, 33, 5, 32, 8, 0, 12, 32, 1, 36, 8, 32, 7, 0, 9, 32, 6, 36, 5, 32,
		54, 36, 2, 4, 10, 0, 7, 32, 3, 0, 17, 32, 3, 34, 3, 38, 1, 102, 6, 98, 1, 34, 32, 2, 20, 34, 4, 38, 8, 34,
		15, 38, 10, 34, 22, 0, 24, 32, 8, 0, 71, 32, 5, 36, 5, 32, 27, 0, 5, 4, 6, 0, 4, 4, 7, 0, 13, 34, 137, 38,
		13, 34, 21, 2, 2, 0, 28, 32, 8, 36, 7, 32, 25, 0, 12, 32, 3, 36, 3, 32, 2, 0, 12, 32, 23, 36, 8, 32, 7, 36,
		1, 4, 6, 0, 22, 32, 9, 36, 11, 32, 34, 36, 3, 32, 21, 36, 10, 32, 16, 0, 27, 32, 8, 36, 8, 32, 12, 36, 13,
		32, 75, 0, 70, 2, 39, 34, 5, 38, 6, 34, 18, 38, 14, 34, 43, 2, 1, 0, 16, 32, 6, 0, 45, 4, 18, 0, 9, 4, 18,
		0, 21, 1, 87, 0, 22, 4, 11, 0, 14, 4, 35, 0, 43, 1, 34, 0, 27, 4, 13, 0, 26, 4, 19, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 40, 32, 74, 0, 3, 32, 37, 36, 12, 32, 7, 0, 21, 1, 9, 0, 14, 128, 29, 0, 30, 32, 10,
		36, 26, 32, 11, 0, 34, 32, 16, 36, 9, 32, 19, 0, 16, 32, 16, 0, 4, 1, 61, 0, 28, 128, 53, 0, 12, 1, 32, 129,
		3, 128, 47, 0, 3, 1, 27, 129, 4, 128, 52, 0, 15, 32, 9, 36, 8, 32, 23, 0, 32, 32, 7, 0, 9, 4, 20, 0, 17,
		1, 7, 0, 17, 128, 36, 0, 29, 32, 1, 36, 14, 32, 21, 0, 31, 32, 2, 36, 30, 32, 9, 0, 15, 16, 8, 0, 4, 1,
		41, 0, 44, 16, 14, 20, 7, 16, 44, 0, 18, 4, 31, 36, 7, 32, 34, 0, 6, 32, 2, 36, 5, 32, 15, 0, 46, 16, 24,
		0, 12, 32, 6, 0, 33, 32, 5, 36, 5, 32, 20, 36, 14, 32, 16, 0, 52, 32, 16, 0, 14, 128, 15, 160, 19, 164, 255,
		164, 34, 160, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 188, 32, 16, 0, 176, 2, 14, 0, 42, 4, 23, 0, 21, 4, 2, 36, 17, 32,
		2, 0, 29, 4, 1, 36, 8, 32, 2, 0, 37, 32, 13, 0, 10, 2, 11, 0, 118, 4, 15, 0, 43, 4, 10, 36, 4, 32, 2, 0,
		43, 16, 20, 20, 19, 16, 39, 20, 7, 16, 31, 0, 21, 2, 14, 0, 41, 4, 24, 0, 19, 32, 5, 0, 88, 4, 18, 36, 12,
		32, 1, 0, 102, 4, 5, 36, 7, 32, 11, 0, 6, 32, 1, 0, 26, 2, 10, 0, 52, 4, 12, 0, 25, 16, 4, 0, 23, 4, 2,
		20, 2, 4, 6, 0, 97, 4, 2, 20, 2, 84, 10, 20, 1, 16, 22, 0, 8, 1, 5, 0, 27, 16, 69, 0, 7, 32, 52, 0, 10,
		2, 10, 0, 46, 4, 16, 0, 16, 32, 9, 0, 10, 4, 6, 36, 14, 32, 5, 0, 4, 16, 8, 0, 2, 1, 6, 0, 165, 32, 33,
		0, 5, 32, 20, 0, 12, 2, 8, 0, 38, 4, 17, 0, 25, 16, 4, 20, 5, 4, 2, 0, 32, 20, 9, 16, 8, 0, 30, 16, 74,
		0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 16, 37, 0, 26, 16, 3, 0, 10, 32, 6, 0, 58, 16, 13,
		0, 9, 32, 3, 0, 255, 0, 11, 32, 3, 36, 17, 32, 19, 0, 23, 16, 7, 80, 2, 84, 31, 80, 17, 16, 1, 0, 17, 32,
		3, 0, 139, 32, 4, 36, 15, 32, 19, 0, 45, 16, 3, 80, 3, 84, 32, 80, 9, 16, 4, 0, 49, 32, 2, 0, 115, 4, 24,
		0, 53, 4, 24, 0, 74, 32, 8, 36, 30, 32, 11, 36, 18, 32, 3, 0, 25, 32, 39, 0, 68, 4, 14, 0, 7, 32, 5, 0,
		80, 4, 19, 20, 9, 4, 1, 0, 32, 16, 4, 0, 17, 32, 4, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
};

i16 DemoRecordSize[6] = {154, 192, 200, 172, 200, 120}; //95E04
i16 DemoRecordWorld[6] = {2, 3, 4, 5, 1, 5}; //95E10
i16 DemoRecordMap[6] = {7, 7, 1, 4, 9, 10}; //95E1C
u8* DisplayBufferNormal = NULL; //95E28 // initialized to 0xA0000 (= VGA video memory)
u8 ModeVideoActuel = 255; //95E2C
u8 GameModeVideo = 255; //95E2D
i32 JumAmpDefMul = 0; //95E30
i32 JumAmpDefPlus = 0; //95E34
u8 key_right = SC_RIGHT; //95E38
u8 key_up = SC_UP; //95E39
u8 key_left = SC_LEFT; //95E3A
u8 key_down = SC_DOWN; //95E3B
u8 key_fist = SC_ALT; //95E3C
u8 key_jump = SC_CONTROL; //95E3D
u8 key_action = SC_X; //95E3E
u8* key_table[7] = {&key_left, &key_up, &key_right, &key_down, &key_jump, &key_fist, &key_action}; //95E40
u8 tab_key_sav[7] = {SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, SC_CONTROL, SC_ALT, SC_X}; //95E5C
u8 is_in_game = 0; //95E63
u8 byte_95E64 = 0; //95E64
u16 fixon = 1; //95E66      // scores enabled
u16 fixontemp = 0; //95E68
u16 SizeScreen = 0; //95E6A
u8 Frequence = 0; //95E6C
u8 BackgroundOptionOn = 1; //95E6D
u8 BackgroundOn = 1; //95E6E

u16 tRefRam2VramNormalFix[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E70
u16 tRefRam2VramNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E80
u16 tRefTransFondNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E90
u16 tRefSpriteNormal[2] = {255, 255}; //95EA0
u16 tRefRam2VramX[2] = {255, 255}; //95EA4
u16 tRefVram2VramX[2] = {255, 255}; //95EA8
u16 tRefSpriteX[2] = {255, 255}; //95EAC
u8 ExitMenu = 0; //95EB0
u8 ReInitPlasma = 0; //95EB1
u8 Main_Control = 0; //95EB2
i16 During_The_Menu = 1; //95EB4
u8 OptionMusicCdActive = 1; //95EB6

u8 casse_brique_active = 0; //962B8
u8 CasseBriqueON = 0; //962B9

//966B8
u16 snd8b_offsets[128] = {
		11, 12, 13, 14, 15, 16, 16, 17, 19, 20, 21, 22, 23,
		25, 26, 28, 30, 32, 33, 35, 38, 40, 42, 45, 47, 50,
		53, 57, 60, 64, 67, 71, 76, 80, 85, 90, 96, 101, 107,
		114, 120, 128, 135, 143, 152, 161, 170, 181, 191, 203,
		215, 228, 241, 256, 271, 287, 304, 322, 341, 362, 383,
		406, 430, 456, 483, 512, 542, 574, 608, 645, 683, 724,
		767, 812, 861, 912, 966, 1024, 1084, 1149, 1217, 1290,
		1366, 1448, 1534, 1625, 1722, 1824, 1933, 2048, 2169,
		2298, 2435, 2581, 2733, 2896, 3068, 3250, 3444, 3649,
		3866, 4096, 4339, 4597, 4870, 5160, 5467, 5792, 6137,
		6501, 6888, 7298, 7732, 8192, 8679, 9195, 9741, 10321,
		10935, 11585, 12274, 13003, 13777, 14596, 15464, 16384,
		17358, 18390,
};

//96888
const char* key_descriptions_azerty[128] = {
		0, 0/*escape*/, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 0/*minus*/, 0/*equals*/, 0/*backspace*/,
		0/*tab*/, "a", "z", "e", "r", "t", "y", "u", "i", "o", "p", 0/*leftbracket*/, 0/*rightbracket*/, 0/*enter*/,
		0/*control*/, "q", "s,", "d", "f", "g", "h", "j", "k", "l", "m", 0/*quote*/, 0/*tilde*/, 0/*lshift*/, 0/*backslash*/,
		"w", "x", "c", "v", "b", "n", "?", "."/*comma*/, ":", 0, 0/*slash*/, 0/*rshift*/,
};
//96A88
const char* key_descriptions_qwerty[128] = {
		0, 0/*escape*/, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 0/*minus*/, 0/*equals*/, 0/*backspace*/,
		0/*tab*/, "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 0/*leftbracket*/, 0/*rightbracket*/, 0/*enter*/,
		0/*control*/, "a", "s,", "d", "f", "g", "h", "j", "k", "l", ":", 0/*quote*/, 0/*tilde*/, 0/*lshift*/, 0/*backslash*/,
		"z", "x", "c", "v", "b", "n", "m", 0/*comma*/, ".", "?", 0/*slash*/, 0/*rshift*/,
};

i16 Volume_Snd; //97F64

i16 PosArYToon2; //E0BF4
i16 PosArXToon2; //E0BF6
i16 PosArYToon1; //E0BF8
i16 PosArXToon1; //E0BFA
i16 flamme_droite_id; //E0BFC
i16 XText; //E0BFE

i16 joy_rec_left; //E0CCE
i16 word_E0CD0; //E0CD0
i16 word_E0CD2; //E0CD2
i16 word_E0CD4; //E0CD4
i16 word_E0CD6; //E0CD6
i16 word_E0CD8; //E0CD8
i32 PositionJumelleDemandeY; //E0CDC
i32 PositionJumelleDemandeX; //E0CE0
i32 PositionJumelleY16; //E0CE4
i32 PositionJumelleX16; //E0CE8
i32 Xmap16; //E0CEC
i32 Ymap16; //E0CF0
i32 ModeAutoPosXJumelle; //E0CF4
i32 ModeAutoPosYJumelle; //E0CF8
i32 ModeAutoJumelleZoomAmp; //E0CFC
i32 ModeAutoRayonJumelle; //E0D00
i32* DistPointY; //E0D04
i32* DistPointX; //E0D08
i32 CompteurJumelle; //E0D0C
i32* ExpPoint; //E0D10
i32 dword_E0D14; //E0D14
obj_t wldobj[100]; //E0D18

const char** key_descriptions; //E4CB0

i32 RandomIndex; //E548C
i16 stk_obj[20]; //E5490
i16 stk_snd[20]; //E54B8

voice_t voice_table[32]; //E55D0

i16 indice_ray_wait; //E5750
i16 indice_trz_wait; //E5754
i16 pt_pile_snd; //E5756
i16 indice_snd_wiz; //E575A

