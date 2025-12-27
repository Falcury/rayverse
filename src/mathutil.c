
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
	return popcnt((x & -((s32)x)) - 1);
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
void set2bits(u32 * x, u8 index, u32 value) {
    u32 new_x = (value << (2 * (15 - index))) | (*x & ~(3 << (2 * (15 - index))));
    *x = new_x;
}

//5DF24
u32 read2bits(u32* x, u8 index, u32* high_bit, u32* low_bit) {
    u32 result = ((3 << (2 * (15 - index))) & *x) >> (2 * (15 - index));
    *high_bit = (result & 2) >> 1;
    *low_bit = result & 1;
    return result;
}

//5DF58
void setbit(u8 *buffer, u16 index) {
    buffer[index >> 3] |= 1 << (index & 7);
}

//5DF74
void clearbit(u8 *buffer, u16 index) {
    buffer[index >> 3] &= ~(1 << (index & 7));
}

//5DF98
u8 getbit(u8 *buffer, u16 index) {
    return buffer[index >> 3] & (1 << (index & 7));
}

//5DFB8
s32 reflexion(s32 a1, s32 a2) {
    return 2 * a1 - a2;
}

//inlined
s32 ashl16(s16 x, u8 bits) {
    s32 result = x << bits;
    return result;
}

//inlined
s16 ashr16(s16 x, u8 bits) {
    s16 result;
    if (x < 0) {
        result = (s16)-((-x) >> bits);
    } else {
        result = (s16)(x >> bits);
    }
    return result;
}

//inlined
s32 ashl32(s32 x, u8 bits) {
    s32 result = x << bits;
    return result;
}

//inlined
s32 ashr32(s32 x, u8 bits) {
    s32 result;
    if (x < 0) {
        result = -((-x) >> bits);
    } else {
        result = x >> bits;
    }
    return result;
}

//inlined
s32 Abs(s32 x) {
    return abs(x);
}

//inlined
s32 mini(s32 a, s32 b) {
    return ((a) < (b) ? (a) : (b));
}

//inlined
s32 maxi(s32 a, s32 b) {
    return (a) > (b) ? (a) : (b);
}

//inlined
s32 sgn(s32 x) {
    return (x < 0) ? -1 : (x > 0);
}

//5DFC0
s32 cosinus(s32 x) {
    s32 result;
    x += 128;
    x &= (512-1);
    if (x >= 256) {
        s32 index = x - 384;
        if (index < 0) {
            index = -index;
        }
        result = -costab[index];
    } else {
        s32 index = x - 128;
        if (index < 0) {
            index = -index;
        }
        result = costab[index];

    }
    return result;
}

//5E000
s32 sinus(s32 x) {
    s32 result;
    x &= (512-1);
    if (x >= 256) {
        s32 index = x - 384;
        if (index < 0) {
            index = -index;
        }
        result = -costab[index];
    } else {
        s32 index = x - 128;
        if (index < 0) {
            index = -index;
        }
        result = costab[index];

    }
    return result;
}

//5E03C
void sinus_cosinus(s32 x, s16 *cosinus_result, s16 *sinus_result) {
    s32 result;
    x &= (512-1);
    if (x >= 256) {
        s32 index = x - 384;
        if (index < 0) {
            index = -index;
            *cosinus_result = (s16)-costab[128 - index];
        } else {
            *cosinus_result = (s16)costab[128 - index];
        }
        *sinus_result = (s16)-costab[index];
    } else {
        s32 index = x - 128;
        if (index < 0) {
            index = -index;
            *cosinus_result = (s16)costab[128 - index];
        } else {
            *cosinus_result = (s16)-costab[128 - index];
        }
        *sinus_result = costab[index];
    }
}

//5E0F0
s32 abs_sinus(s32 x) {
    s32 t = (u8)x - 128;
    if (t < 0) {
        t = -t;
    }
    return costab[t];
}

//5E10C
s32 abs_cosinus(s32 x) {
    s32 t = (u8)(x + 128) - 128;
    if (t < 0) {
        t = -t;
    }
    return costab[t];
}

//5E12C
void abs_sinus_cosinus(s32 x, s16 *sinus_result, s16 *cosinus_result) {
    s32 t = (u8)x - 128;
    if (t < 0) {
        t = -t;
    }
    *sinus_result = costab[t];
    *cosinus_result = costab[128 - t];
}


