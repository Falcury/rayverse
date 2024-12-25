
// (win32) rayverse.cpp
#ifdef _WIN32
void win32_advance_frame(app_state_t* app_state);
void win32_prepare_frame(app_state_t* app_state);
void win32_end_frame(app_state_t* app_state);
#else
void linux_advance_frame(app_state_t* app_state);
void linux_prepare_frame(app_state_t* app_state);
void linux_end_frame(app_state_t* app_state);
#endif
void message_box(const char* message);

// basic.c
void set_subetat(obj_t* obj, u8 sub_etat);
void set_main_etat(obj_t* obj, u8 etat);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void get_center_x(obj_t* obj);
void get_center_y(obj_t* obj);
void on_block_chdir(obj_t* obj, i16 center_x, i16 center_y);
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, i32 a2, i32 a3, i16 a4);
void GET_SPRITE_POS(obj_t* obj, i16 a2, i16* a3, i16* a4, i16* a5, i16* a6);
void GET_RAY_ZDC(obj_t* obj, i16* a2, i16* a4, i16* a5);
void GET_BB1_ZDCs(obj_t* obj, i16* a2, i16* a3, i16* a4, i16* a5, i16* a6, i16* a7, i16* a8, i16* a9);
void calc_obj_dir(obj_t* obj);
void OBJ_IN_ZONE(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void makeUturn(obj_t* obj);
u8 BTYP(i32 tile_x, i32 tile_y);
void calc_btyp_square(obj_t* obj);
void DO_OBJ_REBOND_EN_X(obj_t* obj);
void calc_btyp(obj_t* obj);
void init_obj_in_the_air(obj_t* obj);
void make_my_fruit_go_down(obj_t* obj, i16 a2);
void switchOff(obj_t* obj);
void obj_hurt(obj_t* target);
void Projectil_to_RM(obj_t* obj, i16* a2, i16* a3, i16 a4, i16 a5);
void del_actobj(obj_t* obj);
void calc_let_Width(u8 a1, u8 a2);
void calc_let_Width2(u8 a1, u8 a2);
void INIT_TEXT_TO_DISPLAY(void);
void deter_num_let(u8 a1);
void calc_num_let_spr(u8 a1, u8* a2);
void calc_largmax_text(void* a1, i16 a2, i16 a3, i16 a4, u8 a5);
void INIT_TXT_BOX(void* a1);
void Deter_Option_Caract(u8* a1, i16 a2, i16 a3);
void SwapAB(i16* a, i16* b);
void Bresenham(void* func, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7);
void init_finBossLevel(void);
void Change_Wait_Anim(void);
void add_alwobj(obj_t* obj);
void del_alwobj(i16 obj_index);
void FatalError(obj_t* obj);
u8 sprite_of_ref(obj_t* obj, u8 a2);

// bat.c
void DO_BAT_FLASH(i16 a1, i16 a2);
void DO_BAT_LEFT_FLASH(obj_t* obj);
void DO_BAT_RIGHT_FLASH(obj_t* obj);
void DO_BAT_LEFT_RIGHT_FLASH(obj_t* obj);
void bat_dir(obj_t* obj);
void DO_BAT_COMMAND(obj_t* obj);
void BAT_ray_in_zone(obj_t* obj);
void DO_BAT_POING_COLLISION(obj_t* obj);
void bat_get_eject_sens(obj_t* obj);

// bbmont.c
void BB2_MARCHE(obj_t* obj);
void DO_TOTEM_TOUCHE(obj_t* obj, i16 a2);
void DO_TOTEM_COMMAND(obj_t* obj);
void DO_TOTBT_REBOND(obj_t* obj);
void DO_PI_EXPLOSION2(obj_t* obj);
void DO_BBL_REBOND(obj_t* obj);
void allocateDog(obj_t* obj);
void allocateTir(obj_t* obj, i16 a2);
void CreateFirstBBL(void);
void INIT_BBMONT(obj_t* obj);
void DO_BBL_COMMAND(obj_t* obj);
void BBMONT_ECLAIR(obj_t* obj);
void BBMONT_ETINCELLES(obj_t* obj);
void BBMONT_TIRE(obj_t* obj);
void Cree_BBL(obj_t* obj);
void BB_Attaque(obj_t* obj);
void Fin_BB_Attaque(obj_t* obj);
void BB_Atan(obj_t* obj);
void DO_BBMONT_ATTER(obj_t* obj);
void DO_BBMONT_COMMAND(obj_t* obj);
void DO_BBMONT_TOUCHE(obj_t* obj, i16 a2);
void DO_BBMONT2_COMMAND(obj_t* obj);
void DO_BBMONT2_MARCHE(obj_t* obj);
void DO_BBMONT2_ATTER(obj_t* obj);
void DO_BBMONT3_COMMAND(obj_t* obj);
void DO_BBMONT3_ATTER(obj_t* obj);
void DO_BBMONT4_COMMAND(obj_t* obj);
void DO_ECLAIR_COMMAND(obj_t* obj);
void DO_BB1_PLAT_CMD(obj_t* obj);

// blacktoon.c
void allocateBlacktoonEyes(obj_t* obj);
void DO_BLK_SPEED_COMMAND(obj_t* obj);
void blkUTurn(obj_t* obj);
void DO_BLK_LR_COMMAND(obj_t* obj);
void DO_BLK_NOP_COMMAND(obj_t* obj);
void DO_BLKTOON_COMMAND(obj_t* obj);
void DoBlackToonPoingCollision(obj_t* obj);
void DoBlackToonRaymanZDD(obj_t* obj);

// blocs.c
void MURDUR(i16 a1, i16 a2);
void dist_to_bloc_floor(i16 a1, i16 a2, i16 a3);
void bloc_floor(i16 a1, i16 a2, i16 a3);
u8 calc_typ_trav(obj_t* obj, u8 a2);
u8 calc_typ_travd(obj_t* obj, u8 a2);
void TEST_FIN_BLOC(obj_t* obj);
void TEST_IS_ON_RESSORT_BLOC(obj_t* obj);
void IS_ON_RESSORT_BLOC(obj_t* obj);
void CALC_MOV_ON_BLOC(obj_t* obj);
void recale_position(obj_t* obj);
i32 blocs1_empty(i32 a1, i32 a2);
i32 blocs1_right_45(i32 a1, i32 a2);
i32 blocs1_left_45(i32 a1, i32 a2);
i32 blocs1_right1_30(i32 a1, i32 a2);
i32 blocs1_right2_30(i32 a1, i32 a2);
i32 blocs1_left1_30(i32 a1, i32 a2);
i32 blocs1_left2_30(i32 a1, i32 a2);
i32 blocs3_empty(i32 a1, i32 a2);
i32 blocs4_empty(i32 a1, i32 a2);
i32 blocs2_empty(i32 a1, i32 a2);
void init_calcbloc_func(void);

// breakout.c
u8 get_casse_brique_ON(void);
void set_casse_brique_ON(u8 on);
u8 get_casse_brique_active(void);
void set_casse_brique_active(void);
void enter_casse_brique(void);
void enter_casse_brique2(void);
void exit_casse_brique(void);
void exit_casse_brique2(void);
i32 get_speed_ball(void);
void speed_ball_null(obj_t* obj);
void speed_ball_init(obj_t* obj);
void speed_up_ball(obj_t* obj);
void invert_speed_ball_x(obj_t* obj);
void invert_speed_ball_y(obj_t* obj);
void set_ball_pos(obj_t* obj, i16 a2, i16 a3);
void angle_direction(i16 a1, i16 a2);
void move_ball(obj_t* obj);
void get_brik_bonus(void);
void do_bonus(void);
void get_brik_info(obj_t* obj, i16 a2, i16* a3, i16* a4, u8* a5);
void frame_briques(obj_t* obj);
void AllocateCasseBriqueBoum(obj_t* obj);
void AllocateBonus(obj_t* obj, i16 a2, u8 a3, u8 a4);
void bonus_fall(obj_t* obj);
void collision_briques(obj_t* obj);
void collision_rayman(obj_t* obj);
void move_briques(obj_t* obj);
void init_briques(obj_t* obj);
void reset_briques(void);
void casse_brique_init(obj_t* obj);
void casse_brique_start(obj_t* obj);
void ray_face_brik(i16 a1);
void casse_brique_play(obj_t* obj);
void casse_brique_end(obj_t* obj);
void casse_brique_done(obj_t* obj);
void DISPLAY_FIXE_CB(obj_t* obj);
void DO_CB_BALL_COMMAND(obj_t* obj);
void DO_CB_BRIK_COMMAND(obj_t* obj);
void DO_RAY_CASSE_BRIQUE(obj_t* obj);

// cage.c
void DO_CAGE2(obj_t* obj);
void DO_CAGE(obj_t* obj);
void DoCagePoingCollision(obj_t* obj);
void DO_MEDAILLON_TOON(obj_t* obj);
void DO_MEDAILLON_TOON_GELE(void);
void ALLOCATE_MEDAILLON_TOON(void);
void allocate_toons(obj_t* obj, u8 a2);
void allocateGrille(obj_t* obj);
void DO_ONE_PINK_CMD(obj_t* obj);
void DoLidolPinkRaymanZDD(obj_t* obj);

// clown.c
void DO_CLOWN_TNT_COMMAND(obj_t* obj);
void DO_CLOWN_TNT2_COMMAND(obj_t* obj);
void DO_CLOWN_TNT3_COMMAND(obj_t* obj);
void DO_BIG_CLOWN_ATTAK(obj_t* obj);
void allocatedrop(obj_t* obj);
void DO_WAT_CLOWN_ATTAK(obj_t* obj);
void DO_CLOWN_TNT_ATTACK(obj_t* obj);
void Clown_Music_Atter(obj_t* obj);
void DoWatAndBigClownPoingCollision(obj_t* obj);
void DoTntClownPoingCollision(obj_t* obj);
void DoWatAndBigClownRaymanZDD(obj_t* obj);
void DoTntClownRaymanZDD(obj_t* obj);

// collision.c
i32 get_nb_zdc(obj_t* obj);
i32 get_zdc_index(obj_t* obj);
zdc_t* get_zdc(obj_t* obj, i32 a2);
u16 get_ZDCPTR(void);
i32 in_coll_sprite_list(obj_t* obj, i16 a2);
bool box_inter_v_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7);
bool box_inter_h_line(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, i16 a6, i16 a7);
i32 inter_box(i16 box1_x, i16 box1_y, i16 box1_width, i16 box1_height, i16 box2_x, i16 box2_y, i16 box2_width, i16 box2_height);
void GET_OBJ_ZDC(obj_t* obj);
void GET_SPRITE_ZDC(obj_t* obj, i16 a2, i16* a3, i16* a4, i16* a5, i16* a6);
void BOX_HIT_SPECIAL_ZDC(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj);
void BOX_IN_COLL_ZONES(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, obj_t* obj);
void COLL_BOX_SPRITE(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj);
void CHECK_BOX_COLLISION(i16 a1, i16 a2, i16 a3, i16 a4, i16 a5, obj_t* obj);
void possible_sprite(obj_t* obj, i16 a2);
void setToleranceDist(i16 a1, i16 a2, i16 a3);
void SET_RAY_DIST_SLOPEY_PLAT(obj_t* obj);
void SET_RAY_DIST_MULTISPR_CANTCHANGE(obj_t* obj);
void SET_RAY_DIST_PI(obj_t* obj);
void SET_RAY_DIST_BAG(obj_t* obj);
void SET_RAY_DIST(obj_t* obj);
void do_boum(void);
void DO_POING_COLLISION(void);
void DoPoingCollisionDefault(obj_t* obj, u16 a2);
void COLL_BOX_ALL_SPRITES(i16 a1, i16 a2, i16 a3, i16 a4, obj_t* obj);
bool COLL_RAY_PIC(void);
void COLL_RAY_BLK_MORTEL(void);
void RAY_KO(obj_t* obj);
void RAY_HIT(bool put_above_solid_tiles, obj_t* other_obj);
void standard_frontZone(obj_t* obj, i16* a2, i16* a3);
void SET_DETECT_ZONE_FLAG(obj_t* obj);
void goToRay(obj_t* obj);
void unleashMonsterHost(obj_t* obj);
void SHOW_COLLISIONS_ZONES(void);
void DO_COLLISIONS(void);
void DoRaymanCollisionDefault(obj_t* obj);
void DO_OBJ_COLLISIONS(obj_t* obj, i16 a2);
void test_allowed(obj_t* obj, i16 center_x, i16 center_y);
void obj_jump(obj_t* obj);
void DO_MOVING_PLATFORM_COMMAND(obj_t* obj);
void DoPTGRAPPINPoingCollision(obj_t* obj);
void DO_ONE_CMD_WAIT(obj_t* obj); // TODO: maybe make a split here: command.c?
void DO_ONE_CMD_LR_ATTENTE(obj_t* obj);
void DO_ONE_CMD_UPDOWN(obj_t* obj);
void special_pour_liv(obj_t* event);
void DO_ONE_CMD(obj_t* obj);
void DO_ROLL_EYES(obj_t* obj);
void DoKillingEyesPoingCollision(obj_t* obj);
void YaUnBloc(obj_t* obj);
void DO_BAG1_COMMAND(obj_t* obj);
void DoRaymanBag1Collision(obj_t* obj);
void MarCoince(obj_t* obj, i16 a2);
void DO_MOVE_MARACAS_COMMAND(obj_t* obj);
void DO_FLASH_COMMAND(obj_t* obj);
void DO_WLKNOT_COMMAND(obj_t* obj);
void ACTIVE_L_EAU(obj_t* obj);
void DO_EAU_QUI_MONTE(obj_t* obj);
void allocateOtherPosts(obj_t* obj);
void doHerseCommand(obj_t* obj);
void doBlackRaymanCommand(obj_t* obj);
void doBlKRayRaymanCollision(obj_t* obj);
void DO_POELLE_COMMAND(obj_t* obj);
void DO_CORDE_COMMAND(obj_t* obj);
void DO_PAC_COMMAND(obj_t* obj);
void DO_CFUMEE_COMMAND(obj_t* obj);
void DO_NOVA2_COMMAND(obj_t* obj);
void doShipCommand(obj_t* obj);
void DO_PROP_COMMAND(obj_t* obj);
void move_fruit_in_water(obj_t* obj);
void DO_FALLING_OBJ_CMD(obj_t* obj);
void DoFallingObjPoingCollision(obj_t* obj, u16 a2);
void DO_BLKTOON_EYES_CMD(obj_t* obj);
void DO_RAY_POS_CMD(obj_t* obj);

// compress.c
// (todo)

// cymbal.c
void DO_CYMBAL_COMMAND(obj_t* obj);
void DO_2_PARTS_CYMBAL(obj_t* obj);
void DO_COLL_RAY_CYMBALE(obj_t* obj);

// dark.c
void PlaceDarkPhase1et2(obj_t* obj);
void dark_attaque_suivante(obj_t* obj);
void init_vitraux(void);
void poing_face_obj(obj_t* obj);
void DARK_phase1(obj_t* obj);
void DARK_phase3(obj_t* obj);
void DO_DARK_COMMAND(obj_t* obj);
void DO_DARK_POING_COLLISION(obj_t* obj);
void DO_DARK_REACT(obj_t* obj);
void DO_DARK_SORT_COMMAND(obj_t* obj);
void DO_DARK_SORT_COLLISION(obj_t* obj);
void allocate_DARK_SORT(obj_t* obj);
void DoAnnuleDarkSortRaymanCollision(obj_t* obj);
void corde_en_bas(obj_t* obj);
void corde_en_haut(obj_t* obj);
void init_corde(obj_t* obj);
void goto_phase1(obj_t* obj);
void goto_phase2(obj_t* obj);
void goto_phase3(obj_t* obj);
void goto_phase5(obj_t* obj);
void DO_VITRAIL_COMMAND(obj_t* obj);
void allume_vitraux(obj_t* obj);

// display.c
void display_inter_anim(void);
void continue_fonction(void);
void DO_CONTINUE(void);
void saisie_nom_prg(void);
void selection_save_option_prg(void);
void DO_SAVE_CHOICE(void);
void AFFICHE_ECRAN_SAVE(void);
void select_level_prg(void);
void SELECT_LEVEL(i16 a1);
void CLEAR_STAGE_NAMES(void);
void DISPLAY_STAGE_NAMES(void);
void WORLD_CHOICE(void);
void DO_WORLD_MAP(void);
void DISPLAY_TXT_VIGNET(void);
void display_vignet_prg(void);
void DISPLAY_GAME_VIGNET(void);
void DISPLAY_TXT_CREDITS(void);
void display_credits_prg(void);
void display_anim_victoire(void);
void DISPLAY_CREDITS(void);
void DISPLAY_CREDITS_MENU(void);
void DISPLAY_PROTOON_BACK(void);
void DO_ANIM_VICTOIRE(void);
void DO_VICTOIRE(void);
void InitDemoJeu(void);
void FinDemoJeu(void);
i16 loader_anim_prg(u32 a1);
void START_LOADER_ANIM(void);
void END_LOADER_ANIM(void);
void DO_GROS_RAYMAN(void);
void START_LEVEL_ANIM(void);
void PouvoirsParMap(void);
void sub_35F70(void);
void sub_3609C(void);
void sub_36150(void);
void FIRST_INIT(void);
void init_divers_level_PC(u8* a1);
void file_level(void);
void world_level(i32 world, char* filename);
void LoadPlan3InVignet(i32 a1, i32 a2);
void DISPLAY_AND_FADE_PLAN3(void);
void LOAD_VIGNET_PC(i32 a1, i16 a2);
void LOAD_SAVE_SCREEN(mem_t* mem);
void LOAD_PERFTIME_SCREEN(i32 a1);
void LOAD_PERFECT_SCREEN(i32 a1);
void LOAD_CONTINUE_SCREEN(i32 a1);
void DISPLAY_FOND_CONTINUE(void);
void SwapPlan2PlanVignInVignet(i32 a1, i32 a2, i16 a3);
void LOAD_VIGNET_GAME(i32 a1);
void LOAD_CREDITS_VIGNET(i32 a1, i32 a2, i16 a3);
void DEPART_WORLD(void);
void DEPART_LEVEL(void);
void REMOVE_FICHIER(void);
i32 TestSave(void);
void test_EXIT(void);
void test_Keyb_on_wldmap(void);
void Keyflush(void);
void LOAD_GENERAL_SCREEN(void);
void LOAD_OPTIONS_SCREEN(void);
void HANDLE_KEY(u8* a1);
void DEPART_INIT_LOOP(void);
void DEPART_DEAD_LOOP(void);
void FIN_GAME_LOOP(void);
void FIN_MAP_LOOP(void);
void FIN_DEAD_LOOP(void);
void END_GAME(void);
void INIT_EXTENSIONS(void);
void DISPLAY_LOADING(void);
void START_WORLD_VIGNET(void);
void END_WORLD_VIGNET(void);
void sub_36FAC(void);
void sub_36FBB(void);
void sub_370E8(void);
void sub_37134(void);
bool LOAD_BIG_RAYMAN(mem_t* mem);
bool ValidButPressed(void);
bool StartButPressed(void);
bool PauseButPressed(void);
bool ExitButPressed(void);
bool CancelButPressed(void);
void SelectButPressed(void);
void ToDoAtExit(void);

// fee.c
void INIT_TXT_FEE(void);
void allocate_poing_or_free(void);
void init_fee(void);
void wait_for_dialogue_fee(obj_t* obj, i16 a2);
void DO_FEE_ETAPE(obj_t* obj);
void fee_gives_super_evts(void);
void DO_FEE(obj_t* obj);
void DoFeeRaymanZDD(obj_t* obj);

// guetteur.c
void swapGuetteurCollZones(obj_t* obj);
void guetteurFollowsShip(obj_t* obj);
void DO_ONE_PAR_COMMAND(obj_t* obj);
void hasGuetteurABomb(obj_t* obj);
void allocatePirateGuetteurBomb(obj_t* obj);
void DO_PAR_TIR(obj_t* obj);
void DO_PAR_POING_COLLISION(obj_t* obj);
void PAR_REACT_TO_RAY_IN_ZONE(obj_t* obj);
void DO_PAR_BOMB_COMMAND(obj_t* obj);

// hybrid.c
void allocateSTOSKO(void);
void allocateMOSKITOMAMA(obj_t* obj);
void allocateMOSKITOSAXO(obj_t* obj);
void doMOSAMScommand(obj_t* obj);
void DoMOSAMSPoingCollision(obj_t* obj, i16 a2);
void allocateStoskoClaw(obj_t* obj);
void doSTOSKOcommand(obj_t* obj);
void DoSTOSKOPoingCollision(obj_t* obj, i16 a2);
void doBBF2command(obj_t* obj);
void DO_HYB_BBF2_POING_COLLISION(obj_t* obj);
void AllocateTirBBF2(obj_t* obj);
void DO_HYB_BBF2_LAS(obj_t* obj);
void DoHybBBF2LasRaymanCollision(obj_t* obj);
void OBJ_IN_COL_ZDC(obj_t* obj1, obj_t* obj2);
void AllocateDarkPhase2(obj_t* obj);
void DO_DARK2_AFFICE_TEXT(void);
void DO_DARK_PHASE2_COMMAND(obj_t* obj);
void DO_DARK2_SORT_COMMAND(obj_t* obj);
void allocate_DARK2_SORT(obj_t* obj);
void DoFlammeCommand(obj_t* obj);
void DoFlammeRaymanCollision(obj_t* obj);
void AllocateFlammes(i16 a1);
void AllocateToons(void);
void DO_DARK2_TOONS_COMMAND(obj_t* obj);
void ToonDonnePoing(obj_t* obj);

// input.c
void POINTEUR_BOUTONS_OPTIONS_BIS(void);
u8 TestKeyWeapon(void);
u8 TOUCHE(u8 key);
u8 TestKeyJump(void);
u8 TestKeyAction(void);
i32 leftjoy(void);
i32 rightjoy(void);
i32 downjoy(void);
i32 upjoy(void);
i32 but0pressed(void);
i32 but1pressed(void);
i32 but2pressed(void);
i32 but3pressed(void);
i32 butX0pressed(void);
i32 butX1pressed(void);
void sub_4212C(i16 a1);
void readinput(void);
void readinput_calibrate_joystick(void);
void readinput_center_joystick(void);
void sub_421C0(void);
void sub_4225C(void);
void read_record(const char* filename);
void load_record(void);
void sub_42408(u8 a1);
void do_record(record_t* rec);
void keyboard_interrupt_handler(void);
void sub_425AB(void);
void RESET_TOUCHE(u8 key);
void RESET_ALL_TOUCHE(void);
void Init_Clavier(void);
void Reset_Clavier(void);
bool JoystickPresent(void);

// jumelle.c
void plot2linejumelle(i32 a1, i32 a2, i32 a3, void* a4, i32 a5, i32 a6, i32 a7);
void DisplayJumellesNormal(i32 a1, i32 a2, void* a3, i32 a4, i32 a5, i32 a6);
void InitMatriceJumelle(void);
void DefaultJumelleVariable(void);
void CalculVitesseJumelle(void);
void ChangeDeltaPosXJumelleWithoutLimit(i32 a1);
void ChangeDeltaPosYJumelleWithoutLimit(i32 a1);
void ChangeDeltaPosXJumelleWithLimit(i32 a1);
void ChangeDeltaPosYJumelleWithLimit(i32 a1);
void ChangeJumelleVariable(void);
void ChangeDeltaZoomAmpJumelle(i32 a1);
void PrepareJumelleZoom(void);
void RecaleRayPosInJumelle(void);
void DisplayJumellesFondNormal(void);
void DoScrollInWorldMap(i16 a1, i16 a2);
void CalcObjPosInWorldMap(obj_t* obj);
void MoveRayInWorldMap(void);

// load.c
void load_world(mem_t* mem_world, mem_t* mem_sprite, const char* filename);
void load_level(mem_t* mem_level, const char* filename);
void load_big_ray(mem_t* buffer);
void LOAD_ALL_FIX(void);
void LoadPcxInVignet(mem_t* buffer, i32 resource_id, i16* width, i16* height);
void LoadPcxPaletteInVignet(i32 resource_id, rgb_palette_t* palette);
void LoadPlan0InVignet(i32 resource_id);
u8* allocate_PLAN0(mem_t* mem_world, i32 width, i32 height);
void PLAN0FND_to_bits_planes(u8* buffer, i16 width, i16 height);
void LoadPlan2InVignet(mem_t* buffer, i32 resource_id);
char* GetStringTxt(char* txt, char* out_buf);
void LoadLanguageTxt(i32 language_index);
void LoadPcxOptions(u8* buffer, i32 resource_id, i16* a3, i16* a4);

// luciole.c
void get_luciole(void);
void CLEAR_FIXE_LUCIOLE(void);
void set_luciole(i32 a1, i32 a2);
void init_aff_luciole(i32 a1, i32 a2);
void plot2line(i32 a1, i32 a2, i32 a3, i32 a4, i32 a5);
void aff_luciole(i32 a1, i32 a2, i32 a3, i32 a4);
void Display_and_free_luciole(i32 a1);
void free_luciole(void);
void INIT_LUCIOLE(void);
void DO_LUCIOLE(void);

// menu.c
void LOAD_FND(void);
void INIT_FND(void);
void calc_off_fnd(void);
void check_key_table(void);
void LOAD_CONFIG(void);
void NewFrequency(u8 a1);
void InitClipping(void);
bool IsBackgroundOn(void);
void init_key2txt(void);
void DO_NEW_MENUS(void);
void DO_OPTIONS_IN_GAME(void);
void INIT_GENERAL_CHOICE(void);
void AFFICHE_ECRAN_GENERAL(void);
void DO_COMMANDE_GENERAL(void);
void END_GENERAL_SCREEN(void);
void INIT_OPTIONS_CHOICE(void);
void INIT_AFFICHE_ECRAN_OPTIONS(void);
void AFFICHE_ECRAN_OPTIONS(void);
void DO_COMMANDE_OPTIONS(void);
void END_OPTIONS_SCREEN(void);
void INIT_KEY_SCREEN(void);
void INIT_AFFICHE_KEY_SCREEN(void);
void AFFICHE_KEY_SCREEN(void);
void DO_COMMANDE_KEY(void);
void END_KEY_SCREEN(void);
void INIT_BUTTON_SCREEN(void);
void INIT_AFFICHE_BUTTON_SCREEN(void);
void AFFICHE_BUTTON_JOYSTICK(void);
void DO_BUTTON_COMMANDE(void);
void END_BUTTON_COMMAND(void);
void INIT_PAD_SCREEN(void);
void INIT_AFFICHE_PAD_SCREEN(void);
void AFFICHE_PAD_SCREEN(void);
void DO_COMMANDE_PAD(void);
void END_PAD_SCREEN(void);
void InitParamGraphicsScreen(void);
void INIT_GRAPHIC_SCREEN(void);
void INIT_AFFICHE_GRAPHIC_SCREEN(void);
void AFFICHE_GRAPHIC_SCREEN(void);
void DO_COMMANDE_GRAPHIC(void);
void END_GRAPHIC_SCREEN(void);
void INIT_VIDEOMODE_SCREEN(void);
void INIT_AFFICHE_VIDEOMODE_SCREEN(void);
void AFFICHE_VIDEOMODE_SCREEN(void);
void DO_COMMANDE_VIDEOMODE(void);
void END_VIDEOMODE_SCREEN(void);
void INIT_CALIBRATE_JOYSTICK(void);
void AFFICHE_CALIBRATE_JOYSTICK(void);
void calibrate_joystick_prg(void);
void AFFICHE_CENTER_JOYSTICK(void);
void center_joystick_prg(void);
void MAIN_CALIBRATE_JOYSTICK(void);
void menu_prg(void);
void DO_INTER_MENU(void);
void DO_MENU(void);
void default_key(void);
void new_key(void);
i32 CalcSpeed(void);
void general_init_screen(i16 a1, i16 a2, i16 a3);
void TestCompteur(void);
void TestButtonReleased(void);
void display_box_text_plasma(void* a1, u8 a2);
void display_box_text_fire(void* a1);
void display_box_msg_prg(void);
void display_box_msg(void* a1);
void display_box_msg_commande(void* a1, void* a2);
void AfficeYesNo(void);
void DO_YESNOBIS(void);
void confirmation_msg(void);
void SYNCHRO_LOOP_PALETTE(void* func);
void apparition_prg(void);
void fondu_prg(void);
void change_couleur_prg(void);
void FonduOption(i16 a1, i16 a2, i16 a3, i16 a4, u8 a5);
bool FonduPixel_prg(void);
void FonduPixel(i16 a1, i16 a2, i16 a3, i16 a4);

// meredenis.c
void findMereDenisWeapon(obj_t* obj);
void mereDenisCanAttak(obj_t* obj);
void setMereDenisAtScrollBorder(obj_t* obj);
void setCirclePointToReach(obj_t* obj);
void allocateMereDenisBombChips(obj_t* obj);
void mereDenisExplodeBombs(obj_t* obj);
void mereDenisDropBomb(obj_t* obj);
void swapWeaponAnimState(obj_t* obj);
void swapMereDenisCollZones(obj_t* obj);
void prepareNewMereDenisAttack(obj_t* obj);
void snapLaserToWeapon(obj_t* obj);
void allocateSpaceMamaLaser(obj_t* obj);
void doMereDenisCommand(obj_t* obj);
void changeMereDenisPhase(obj_t* obj);
void fitSaveCurrentAction(obj_t* obj);
void doMereDenisHit(obj_t* obj);
void mereDenisBigLaserCommand(obj_t* obj);
void mereDenisBombCommand(obj_t* obj);

// mite.c
void mite_esquive_poing(obj_t* obj, i16* a2);
void HAS_MIT_JUMP(obj_t* obj);
void ray_over_mit(obj_t* obj, i16 a2);
void fix_mit_Xspeed(obj_t* obj, i16 a2);
void DO_MIT_ATTAK(obj_t* obj);
void IS_MIT_PAF(obj_t* obj);
void DO_MIT_COMMAND(obj_t* obj);
void DoMitePoingCollision(obj_t* obj, i16 a2);
void DO_MITE2_COMMAND(obj_t* obj);
void DoMite2PoingCollision(obj_t* obj, i16 a2);
void DO_MITE2_ESQUIVE(obj_t* obj);

// moskito.c
void getIdealStingCoords(obj_t* obj, i32* a2, i32* a3);
void closeEnoughToSting(obj_t* obj, i16 a2, i16 a3);
void moskitoCanAttak(obj_t* obj);
void setMoskitoAtScrollBorder(obj_t* obj, u8 a2);
void prepareNewMoskitoAttack(obj_t* obj);
void allocateMoskitoFruit(obj_t* obj);
void moskitoDropFruitOnRay(obj_t* obj);
void doMoskitoCommand(obj_t* obj);
void tellNextMoskitoAction(obj_t* obj);
void changeMoskitoPhase(obj_t* obj);
void doMoskitoHit(obj_t* obj);
void DO_MST_SCROLL_COMMAND(obj_t* obj, i32 a2);
void DO_MST_CHANGE_COMMAND(obj_t* obj);
void DoMstShakyFruitRaymanZDD(obj_t* obj);

// rand.c
i16 myRand(i16 max);
void MakeMyRand(void);

// engine.c
void advance_frame(void);
void WaitNSynchro(i32 n_frames);
void copy_full_image_contents(image_t* dest, image_t* source);
void copy_full_image_to_draw_buffer(image_t* image);
void copy_full_image_to_background_buffer(image_t* image); // TODO: keep or change?
void clrscr(void);
void game_init_sound(game_sound_buffer_t* sound, i32 samples_per_second);
void game_init(game_state_t* game);
void destroy_image(image_t* image);

// platform routines
i64 get_clock(void);
float get_seconds_elapsed(i64 start, i64 end);
// sound.c
void LoadBnkFile(u8** sound_buffer, i32 sound_set);
void game_get_sound_samples(game_sound_buffer_t* sound_buffer);
void play_cd_track(i32 track_number);
void stop_cd_music(void);
ogg_t open_cd_vorbis(i32 track_number);
void stop_ogg(ogg_t* ogg);
void play_ogg(game_sound_buffer_t* sound_buffer, ogg_t* ogg);
void SetCompteurTrameAudio(void);
void stop_cd(void);
void LoadBnkWorld(i16 world);
void manage_snd_event(void);
void SetVolumeSound(i16 volume);
void stop_all_snd(void);
void stop_ray_snd(void);
u8 get_pan_snd(obj_t* obj);
i32 get_vol_snd(obj_t* obj);
void raj_env_sound(i16 volume);
void raj_env_audio(void);
bool ray_env_stereo(i16 stereo);
bool InitSnd(void);
i16 last_snd(i32 obj_id);
i32 get_pile_obj(i32 a1);
i32 get_voice_obj(i32 a1);
i32 get_voice_snd(i32 a1);
i16 get_voice_obj_snd(i16 obj_id, i16 sound_id);
void erase_pile_snd(i16 obj_id);
void nettoie_pile_snd(void);
void erase_voice_table(i32 a1);
i32 snd_in_pile_snd(i16 a1);
i32 vol_r(i16 a1, i16 a2);
i32 vol_l(i16 a1, i16 a2);
void PlaySnd(u16 sound_id, i16 obj_id);
void PlaySnd_old(u16 sound_id);
void setvol(i16 obj_id);
void setpan(i16 obj_id);
void manage_snd(void);
void mute_snd_bouclant(void);
void mute_snd(void);
void start_freeze_snd(void);
void stop_freeze_snd(void);
void PlayTchatchVignette(i32 a1);
void TestCdLoop(void);
void InitMusic(void);
void DoneMusic(void);

//ray.c
void RAY_HURT(void);

// game.c
u8 decode_xor(u8* data, u32 size, u8 encoding_byte, u8 checksum_byte);

void horloges(u32 ticks);
i32 get_proj_dist(i32 scale, i32 outer_dim);
i32 get_proj_x(i32 scale, i32 par_1);
i32 get_proj_y(i32 scale, i32 par_1);
void display2(obj_t* obj);
void SET_X_SPEED(obj_t* obj);
void DO_ANIM(obj_t* obj);
bool EOA(obj_t* obj);
void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal);
void start_fade_in(u32 speed);
void start_fade_out(u32 speed);
void fade_out(u32 speed, rgb_palette_t* palette);
void INIT_FADE_IN(void);
void DrawSpriteNormal256(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void DrawSpriteFlipNormal256(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void DrawSpriteNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void DrawSpriteFlipNormal(i32 proj_x, i32 sprite_field_A, i32 proj_y, vec2b_t proj_size, image_t* draw_buffer, u8* image_data);
void SYNCHRO_LOOP(scene_func_t scene_func);
bool LoadOptionsOnDisk(void);
void set_special_key_descriptions(const char** descriptions);
void InitMemoryVariable(void);
void init_allowed_time(void);
void init_bonus_perfect(void);
void INIT_RAY_BEGIN(void);
void INIT_MOTEUR_BEGIN(void);
void InitTextMode(void);
void select_display_buffer(u8* buffer);
void InitModeNormalWithFrequency(u8 freq);
void set_speaker_on(void);
void SPECIAL_INIT(void);
void PcMain(void);

// bray.c
void init_loader_anim(void);
void DO_LOADER_ANIM(void);

// save.c
void set_medaillion_saved_data(void);
void load_sav(u8 which_save);
void SaveGameOnDisk(u8 which_save);
void reset_items_and_bosses(void);
void doneGameSave(void);
void saveGameState(obj_t* save_obj, save_state_t* save_state);
void restoreGameState(save_state_t* save_state);

// vignet.c
image_t load_vignet_pcx(u32 resource_id);
void load_vignet_palette(u32 resource_id, rgb_palette_t* palette);
void load_plan2_in_vignet(void* buffer, u32 resource_id);

// world_map.c
void SpriteFixeOffset(mem_t* mem);
void SpriteWorldOffset(mem_t* mem);
void SpriteFixeBlocksFree(mem_t* mem);
void SpriteWorldBlocksFree(mem_t* mem);
void INIT_MOTEUR_WORLD(void);
