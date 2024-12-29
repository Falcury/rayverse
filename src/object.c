
// sub_7B628
void DO_NOVA(obj_t* obj) {
	// stub
}


typedef struct {
	void (*command)(obj_t* obj);
	void (*poing_collision)(obj_t* obj, i16 unk);
	void (*rayman_collision)(obj_t* obj);
	void (*rayman_in_zone)(obj_t* obj);
	void (*u_turn)(obj_t* obj);
} obj_procs_t;

//935FC
obj_procs_t ObjectsFonctions[] = {
		{DO_ONE_CMD,                 DoBadGuy1PoingCollision,    DoRaymanCollisionDefault,    DoBadGuy1RaymanZDD,       ObjectUTurnDefault}, // 0: livingstone
		{DO_ONE_CMD,                 DoPlatformPoingCollision,   DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 1: platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoPowerupRaymanCollision, ObjectUTurnDefault}, // 2: energy point
		{DO_ONE_PINK_CMD,            DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoLidolPinkRaymanZDD,     ObjectUTurnDefault}, // 3: electoon
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 4: scenery
		{TEST_WIZARD,                DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 5: magician
		{DO_ONE_CMD,                 DoFallingObjPoingCollision, DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 6: yin with pin
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 7: swinging spiky fruit
		{DO_ONE_CMD,                 DoFallingObjPoingCollision, DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 8: plum 1
		{DO_ONE_CMD,                 DoBadGuy23PoingCollision,   DoRaymanCollisionDefault,    DoBadGuy23RaymanZDD,      ObjectUTurnDefault}, // 9: small livingstone
		{DO_PYRANHA,                 DoFishPoingCollision,       DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 10: piranha
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 11: hit effect
		{DO_ONE_CMD,                 DoChasseurPoingCollision,   DoRaymanCollisionDefault,    DoChasseurRaymanZDD,      ObjectUTurnDefault}, // 12: hunter 1
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 13: hunter bullet
		{DO_ONE_CMD,                 DoChasseurPoingCollision,   DoRaymanCollisionDefault,    DoChasseurRaymanZDD,      ObjectUTurnDefault}, // 14: hunter 2
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 15:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 16: falling platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 17: rising platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 18:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 19: splash
		{DO_TEN_COMMAND,             DoGeneBadGuyPoingCollision, DoRaymanCollisionDefault,    DoGeneBadGuyRaymanZDD,    ObjectUTurnDefault}, // 20: tentacle
		{DO_PHOTOGRAPHE_CMD,         DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 21: photographer
		{DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 22: moving platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 23: rayman
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 24:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 25: ejecting platform
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 26: disappearing cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 27: trampoline cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 28: blinking cloud
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 29:
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoAudioStartRaymanCollision, DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 30: audio start
		{DO_ONE_CMD,                 DoPoingCollisionDefault,    DoRaymanCollisionDefault,    DoRaymanInZDDDefault,     ObjectUTurnDefault}, // 31:
};

