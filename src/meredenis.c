
//50B60
void findMereDenisWeapon(void) {
    mereDenis_weapon_id = -1;
    machine_obj_id = -1;
    for (s16 i = 0; i < level.nb_objects; ++i) {
        u16 type = level.objects[i].type;
        if (type == TYPE_203_SMA_WEAPON) {
            machine_obj_id = i;
        } else if (type == TYPE_112_WASHING_MACHINE) {
            mereDenis_weapon_id = i;
        }
    }
}

//50BB8
void mereDenisCanAttak(obj_t* obj) {
    print_once("Not implemented: mereDenisCanAttak"); //stub
}

//50CC8
void setMereDenisAtScrollBorder(obj_t* obj) {
    print_once("Not implemented: setMereDenisAtScrollBorder"); //stub
}

//50DA4
void setCirclePointToReach(obj_t* obj) {
    print_once("Not implemented: setCirclePointToReach"); //stub
}

//50E5C
void allocateMereDenisBombChips(obj_t* obj) {
    print_once("Not implemented: allocateMereDenisBombChips"); //stub
}

//51010
void mereDenisExplodeBombs(obj_t* obj) {
    print_once("Not implemented: mereDenisExplodeBombs"); //stub
}

//5108C
void mereDenisDropBomb(obj_t* obj) {
    print_once("Not implemented: mereDenisDropBomb"); //stub
}

//51470
void swapWeaponAnimState(obj_t* obj) {
    print_once("Not implemented: swapWeaponAnimState"); //stub
}

//5149C
void swapMereDenisCollZones(obj_t* obj, u8 a2) {
    print_once("Not implemented: swapMereDenisCollZones"); //stub
}

//515AC
void prepareNewMereDenisAttack(obj_t* obj) {
    print_once("Not implemented: prepareNewMereDenisAttack"); //stub
}

//525FC
void snapLaserToWeapon(obj_t* obj) {
    print_once("Not implemented: snapLaserToWeapon"); //stub
}

//52718
void allocateSpaceMamaLaser(obj_t* obj) {
    print_once("Not implemented: allocateSpaceMamaLaser"); //stub
}

//52870
void doMereDenisCommand(obj_t* obj) {
    print_once("Not implemented: doMereDenisCommand"); //stub
}

//529A8
void changeMereDenisPhase(obj_t* obj) {
    print_once("Not implemented: changeMereDenisPhase"); //stub
}

//52A28
void fitSaveCurrentAction(obj_t* obj) {
    print_once("Not implemented: fitSaveCurrentAction"); //stub
}

//52AF4
void doMereDenisHit(obj_t* obj, s16 sprite) {
    print_once("Not implemented: doMereDenisHit"); //stub
}

//52C54
void mereDenisBigLaserCommand(obj_t* obj) {
    print_once("Not implemented: mereDenisBigLaserCommand"); //stub
}

//52D10
void mereDenisBombCommand(obj_t* obj) {
    print_once("Not implemented: mereDenisBombCommand"); //stub
}
