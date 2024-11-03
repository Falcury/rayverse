// TODO: check for intrinsics headers, or maybe do some inline assembly?

u32 popcnt(u32 x) {
	x -= ((x >> 1) & 0x55555555);
	x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
	x = (((x >> 4) + x) & 0x0f0f0f0f);
	x += (x >> 8);
	x += (x >> 16);
	return x & 0x0000003f;
}

u32 clz(u32 x) {
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	return 32 - popcnt(x);
}

u32 ctz(u32 x) {
	return popcnt((x & -((i32)x)) - 1);
}

bool is_power_of_2(u32 x) {
	bool result = x && ((x & (x - 1)) == 0);
	return result;
}

u32 next_power_of_2(u32 x) {
	u32 result = 1 << (32 - clz(x - 1));
	return result;
}

//5DEF0
void set2bits(u32 x) {
    //stub
}

//5DF24
u32 read2bits(u32 x) {
    return 0; //stub
}

//5DF58
void setbit(u32 x) {
    //stub
}

//5DF98
u32 getbit(u32 x) {
    return 0; //stub
}

//5DFC0
i32 sine(i32 x) {
    return 0; //stub
}

//5E000
i32 cosine(i32 x) {
    return 0; //stub
}

//5E03C
i32 sinus_cosinus(i32 x) {
    return 0; //stub
}

//5E10C
i32 abs_cosinus(i32 x) {
    return 0; //stub
}

//5E10C
i32 abs_sinus_cosinus(i32 x) {
    return 0; //stub
}


