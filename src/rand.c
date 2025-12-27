

//6BF60
s16 myRand(s16 max) {
    s32 r = RandomIndex + 1;
    if (r >= 256) {
        r = 0;
    }
    RandomIndex = r;
    return (s16)(RandArray[r] % (max + 1));
}

//6BF98
void MakeMyRand(s32 set_seed) {
    if (set_seed) {
        srand(time(NULL));
    }
    for (u32 i = 0; i < COUNT(RandArray); ++i) {
        s16 r = (s16)(rand() & 0x7FFF); // original: and ah, 0EFh ; does not clear the sign bit, maybe a bug?
        RandArray[i] = r;
    }
    RandomIndex = 0;
}
