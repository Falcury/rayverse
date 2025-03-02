

//6BF60
i16 myRand(i16 max) {
    i32 r = RandomIndex + 1;
    if (r >= 256) {
        r = 0;
    }
    RandomIndex = r;
    return (i16)(RandArray[r] % (max + 1));
}

//6BF98
void MakeMyRand(i32 set_seed) {
    if (set_seed) {
        srand(time(NULL));
    }
    for (u32 i = 0; i < COUNT(RandArray); ++i) {
        i16 r = (i16)(rand() & 0x7FFF); // original: and ah, 0EFh ; does not clear the sign bit, maybe a bug?
        RandArray[i] = r;
    }
    RandomIndex = 0;
}
