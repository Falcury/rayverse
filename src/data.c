
// Global data

app_state_t global_app_state;
game_state_t* global_game;


// engine.cpp

ogg_t ogg_cd_track;
bool is_ogg_playing;
bool is_ogg_finished;
snd_t digi_voices[32];




// Data sorted by offset
i32 saved_sprite_color; //92140
i32 saved_sprite_width; //92144

//9215C
archive_header_t HeaderFilevignet[75] = {
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

//924E0
archive_header_t language_infos[] = {
        {0, 4234, 48, 180},      // English
        {4234, 4713, 130, 161},  // French
        {8947, 4903, 207, 92},   // German
        {13850, 2511, 208, 192}, // Japanese
        {16361, 2366, 149, 20},  // Chinese
};

i16 nb_des = 0; //9251C
i16 nb_fix_des = 0; //9251E
i16 dhspeed = 0; //92520
i16 dvspeed = 0; //92522

//92524
u8 ecroule_rubis_order[24] = {2, 1, 3, 0, 4, 7, 6, 5, 1, 3, 2, 0, 4, 7, 6, 5, 4, 1, 3, 2, 0, 7, 6, 5};

// 0x9253C
u8 block_flags[64] = {
        0x00, 0x20, 0x42, 0xC2, 0x42, 0x42, 0xC2, 0xC2, 0x04, 0x0B, //0-9
        0x00, 0x00, 0x00, 0x00, 0x03, 0x13, 0x00, 0x00, 0x4A, 0xCA, //10-19
        0x4A, 0x4A, 0xCA, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //20-29
        0x0B, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //30-39
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //40-49
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //50-59
        0x20, 0x20, 0x20, 0x20, //60-63
};

//9257C
u32 flags[262] = {
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

u8 joy_done; //92994

//92996
i16 zonediffx[262] = {
        0, 0, 0, 0, 0, -50, 0, 0, -80, 0, 0, 0, 250, 0, 250, 0, -60, -60, 0, 0, 60, 0, 0, 0, 0, 0, -60, 0, 0, 0,
        0, -50, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, -50, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 150, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 450,
        -50, 0, 100, 0, -50, 200, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, -50,
        0, 0, 0, 0, 0, 0, 0, 0, 0, -80, 0, 0, 0, 0, 0, 0, -50, -50, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, -50, -50, -50, 0, 0, -80, 0, 0, 0, 0, 200, 40, 200, 0, 0, 0, -50, -50, 0, 0, 0, 0, 200, 0, 0, 0,
        0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 450, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 200, 200, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -50, -50, -50, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//92BA2
i16 zonediffy[262] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, -50, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, -50, 0, 150, 0, 0, 200, 200, 0, 0, 0, 0, 0, 350, -50, 0, 300,
        0, 0, 200, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 0, -50, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 120, -50, -50, 0, 0, 0, 100, 300, 0, 200, 0, 0, 120, 0, 0, 0, 0, 0, 0, 150, 150,
        0, -50, -50, 0, 0, 0, 0, 120, 120, 0, 0, 120, 200, 200, 0, 0, 0, 20, 20, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 150, 0, 50, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 50, 100, 150, 0,
        0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 20, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
};

//92DAE
zdc_t zdc_tab[200] = {
        {84, 44, 13, 17, 4, 255}, // 0
        {3, 4, 3, 13, 3, 0}, // 1
        {12, 11, 16, 16, 3, 4}, // 2
        {7, 6, 26, 25, 1, 255}, // 3
        {2, 1, 8, 13, 3, 2}, // 4
        {76, 63, 10, 22, 1, 255}, // 5
        {5, 8, 12, 8, 3, 0}, // 6
        {6, -10, 12, 35, 3, 2}, // 7
        {1, 7, 21, 5, 3, 4}, // 8
        {71, 49, 18, 6, 1, 255}, // 9
        {6, -10, 12, 35, 3, 2}, // 10
        {1, 7, 21, 5, 3, 4}, // 11
        {71, 49, 18, 6, 1, 255}, // 12
        {-21, -19, 64, 64, 3, 0}, // 13
        {105, 168, 48, 60, 1, 255}, // 14
        {97, 129, 2, 2, 1, 255}, // 15
        {8, 6, 20, 20, 3, 0}, // 16
        {6, 5, 10, 11, 3, 0}, // 17
        {74, 73, 13, 14, 1, 255}, // 18
        {9, 10, 16, 16, 3, 0}, // 19
        {14, 1, 14, 44, 3, 0}, // 20
        {9, 10, 16, 16, 3, 0}, // 21
        {8, 6, 20, 20, 3, 0}, // 22
        {64, 48, 32, 32, 1, 255}, // 23
        {0, 0, 16, 16, 3, 5}, // 24
        {17, 1, 9, 9, 3, 7}, // 25
        {24, 64, 112, 96, 1, 255}, // 26
        {11, 10, 16, 16, 3, 0}, // 27
        {16, 26, 48, 38, 3, 0}, // 28
        {11, 11, 16, 16, 3, 0}, // 29
        {1, -7, 12, 34, 3, 2}, // 30
        {8, 6, 20, 20, 3, 0}, // 31
        {1, -8, 13, 30, 3, 0}, // 32
        {6, 3, 11, 2, 3, 0}, // 33
        {17, 13, 58, 76, 3, 2}, // 34
        {15, 14, 23, 59, 3, 5}, // 35
        {63, 77, 21, 10, 4, 255}, // 36
        {8, 5, 18, 23, 3, 3}, // 37
        {7, 14, 6, 7, 3, 0}, // 38
        {0, 2, 208, 6, 3, 3}, // 39
        {79, 52, 6, 19, 1, 255}, // 40
        {71, 56, 19, 18, 1, 255}, // 41
        {71, 56, 19, 18, 1, 255}, // 42
        {75, 58, 12, 12, 1, 255}, // 43
        {6, 4, 24, 25, 6, 4}, // 44
        {2, 4, 18, 14, 3, 5}, // 45
        {8, 8, 49, 51, 3, 6}, // 46
        {18, 9, 16, 16, 3, 1}, // 47
        {19, 2, 14, 68, 3, 3}, // 48
        {-3, -2, 11, 7, 3, 10}, // 49
        {10, 6, 16, 19, 3, 0}, // 50
        {11, 7, 14, 17, 3, 1}, // 51
        {10, 8, 17, 16, 3, 2}, // 52
        {24, 24, 54, 53, 3, 4}, // 53
        {76, 60, 8, 9, 1, 255}, // 54
        {21, 20, 56, 56, 3, 0}, // 55
        {63, 40, 43, 45, 1, 255}, // 56
        {123, 130, 43, 45, 1, 255}, // 57
        {168, 140, 32, 48, 1, 255}, // 58
        {2, -9, 8, 28, 3, 2}, // 59
        {2, -9, 8, 28, 3, 2}, // 60
        {2, -9, 8, 28, 3, 2}, // 61
        {10, 6, 19, 11, 3, 0}, // 62
        {10, 6, 19, 11, 3, 1}, // 63
        {2, 20, 32, 37, 3, 2}, // 64
        {11, 13, 59, 12, 3, 3}, // 65
        {18, 14, 41, 41, 3, 14}, // 66
        {18, 14, 41, 41, 3, 15}, // 67
        {0, 0, 19, 32, 6, 10}, // 68
        {4, 0, 19, 32, 6, 11}, // 69
        {74, 73, 13, 14, 1, 255}, // 70
        {70, 68, 14, 12, 1, 255}, // 71
        {15, 3, 12, 42, 3, 0}, // 72
        {109, 51, 54, 56, 1, 255}, // 73
        {111, 152, 51, 23, 1, 255}, // 74
        {79, 83, 116, 38, 1, 255}, // 75
        {50, 114, 52, 15, 1, 255}, // 76
        {71, 140, 54, 21, 1, 255}, // 77
        {71, 140, 54, 21, 1, 0}, // 78
        {7, 6, 26, 25, 1, 255}, // 79
        {68, 53, 33, 34, 1, 255}, // 80
        {4, 1, 16, 28, 3, 2}, // 81
        {9, 9, 36, 9, 3, 0}, // 82
        {64, 48, 32, 32, 1, 255}, // 83
        {0, 0, 16, 16, 3, 5}, // 84
        {17, 1, 9, 9, 3, 7}, // 85
        {24, 64, 112, 96, 1, 255}, // 86
        {3, 3, 12, 12, 3, 4}, // 87
        {7, 4, 13, 11, 3, 6}, // 88
        {4, 3, 17, 17, 3, 7}, // 89
        {12, 11, 31, 28, 3, 11}, // 90
        {15, 20, 22, 47, 3, 12}, // 91
        {13, -6, 53, 33, 3, 16}, // 92
        {6, 7, 30, 14, 3, 17}, // 93
        {71, 49, 18, 6, 1, 255}, // 94
        {0, 0, 75, 80, 3, 2}, // 95
        {79, 52, 6, 19, 1, 255}, // 96
        {7, 5, 19, 19, 1, 255}, // 97
        {2, 1, 8, 13, 3, 2}, // 98
        {76, 63, 10, 22, 1, 255}, // 99
        {7, 6, 26, 25, 1, 255}, // 100
        {0, 2, 208, 6, 3, 3}, // 101
        {4, 4, 16, 16, 3, 0}, // 102
        {-5, -12, 27, 42, 3, 5}, // 103
        {5, 5, 17, 16, 3, 6}, // 104
        {2, -1, 2, 27, 3, 1}, // 105
        {71, 11, 14, 106, 4, 255}, // 106
        {4, -6, 2, 22, 3, 3}, // 107
        {71, 11, 14, 106, 4, 255}, // 108
        {4, -6, 2, 22, 3, 3}, // 109
        {71, 11, 14, 106, 4, 255}, // 110
        {4, -6, 2, 22, 3, 3}, // 111
        {17, 13, 58, 76, 3, 2}, // 112
        {15, 14, 23, 59, 3, 5}, // 113
        {79, 52, 6, 19, 1, 255}, // 114
        {17, 13, 58, 76, 3, 2}, // 115
        {15, 14, 23, 59, 3, 5}, // 116
        {-5, -12, 27, 42, 3, 5}, // 117
        {5, 5, 17, 16, 3, 6}, // 118
        {79, 52, 6, 19, 1, 255}, // 119
        {18, 7, 32, 29, 3, 2}, // 120
        {7, 0, 26, 23, 3, 5}, // 121
        {9, 11, 33, 13, 1, 255}, // 122
        {9, 11, 33, 13, 1, 255}, // 123
        {-6, -129, 22, 139, 3, 3}, // 124
        {16, 4, 8, 8, 3, 0}, // 125
        {-8, 4, 8, 8, 3, 1}, // 126
        {0, 4, 8, 8, 3, 2}, // 127
        {4, 4, 8, 8, 3, 4}, // 128
        {4, 4, 8, 8, 3, 5}, // 129
        {4, 12, 23, 6, 3, 0}, // 130
        {7, 12, 24, 8, 3, 1}, // 131
        {4, 12, 23, 6, 3, 0}, // 132
        {7, 12, 24, 8, 3, 1}, // 133
        {14, 5, 93, 12, 3, 8}, // 134
        {3, 2, 22, 5, 3, 1}, // 135
        {12, -40, 60, 153, 1, 255}, // 136
        {72, -40, 60, 153, 1, 255}, // 137
        {105, 168, 48, 60, 1, 255}, // 138
        {168, 140, 32, 48, 1, 255}, // 139
        {10, 6, 21, 16, 3, 0}, // 140
        {5, 4, 12, 12, 3, 3}, // 141
        {6, 4, 12, 12, 3, 6}, // 142
        {11, 7, 23, 18, 3, 8}, // 143
        {7, 2, 16, 9, 3, 2}, // 144
        {12, 7, 47, 15, 3, 4}, // 145
        {15, 7, 56, 17, 3, 5}, // 146
        {4, 2, 6, 50, 3, 7}, // 147
        {10, 22, 56, 7, 3, 8}, // 148
        {22, 18, 22, 50, 3, 6}, // 149
        {16, 2, 43, 30, 3, 1}, // 150
        {-5, -12, 27, 42, 3, 5}, // 151
        {5, 5, 17, 16, 3, 6}, // 152
        {-5, -12, 27, 42, 3, 5}, // 153
        {5, 5, 17, 16, 3, 6}, // 154
        {64, 48, 32, 32, 1, 255}, // 155
        {0, 0, 16, 16, 3, 5}, // 156
        {17, 1, 9, 9, 3, 7}, // 157
        {24, 64, 112, 96, 1, 255}, // 158
        {2, 2, 2, 40, 3, 1}, // 159
        {1, 1, 31, 2, 3, 1}, // 160
        {-13, -13, 48, 43, 3, 3}, // 161
        {5, 2, 30, 17, 3, 7}, // 162
        {-13, -13, 48, 43, 3, 3}, // 163
        {5, 2, 30, 17, 3, 7}, // 164
        {13, 4, 7, 8, 3, 8}, // 165
        {0, 80, 128, 1, 1, 255}, // 166
        {64, 0, 32, 128, 1, 255}, // 167
        {6, 5, 7, 17, 3, 1}, // 168
        {7, -8, 9, 34, 3, 3}, // 169
        {71, 33, 14, 95, 4, 255}, // 170
        {4, 4, 2, 19, 3, 2}, // 171
        {71, 33, 14, 95, 4, 255}, // 172
        {4, 4, 2, 19, 3, 2}, // 173
        {71, 33, 14, 95, 4, 255}, // 174
        {4, 4, 2, 19, 3, 2}, // 175
        {84, 42, 13, 98, 4, 255}, // 176
        {5, 1, 2, 19, 3, 0}, // 177
        {16, 4, 8, 8, 3, 0}, // 178
        {-8, 4, 8, 8, 3, 1}, // 179
        {0, 4, 8, 8, 3, 2}, // 180
        {4, 4, 8, 8, 3, 4}, // 181
        {4, 4, 8, 8, 3, 5}, // 182
        {11, 186, 56, 34, 1, 255}, // 183
        {16, -24, 2, 81, 3, 0}, // 184
};

//933EE
u16 type_zdc[262] = {
        0, 0, 0, 0, 0, 0, 4096, 2050, 2051, 4100, 2054, 0, 4103,
        2057, 4106, 2060, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2061, 0, 2062, 2063, 0, 0, 2064, 2065, 0, 0, 2066,
        2067, 2068, 0, 0, 2069, 0, 2070, 0, 0, 8215, 2075, 2076,
        2077, 0, 0, 0, 0, 0, 0, 0, 2078, 0, 0, 0, 0, 0, 0, 0,
        2079, 0, 0, 2080, 2081, 4130, 0, 0, 4132, 2086, 0, 0,
        2087, 0, 0, 0, 0, 0, 0, 0, 0, 2088, 2089, 2090, 0, 0,
        2091, 6188, 0, 6191, 0, 0, 8242, 2102, 0, 0, 0, 0, 2103,
        0, 2104, 0, 2105, 2106, 0, 0, 0, 2107, 2108, 2109, 0,
        16446, 0, 2118, 2119, 2120, 0, 2121, 6218, 4173, 0,
        0, 0, 0, 0, 2127, 0, 0, 2128, 0, 0, 2129, 0, 0, 0, 0,
        2130, 0, 8275, 0, 0, 14423, 2142, 2143, 0, 0, 0, 2144,
        0, 0, 0, 0, 2145, 0, 0, 0, 4194, 0, 2148, 2149, 0, 2150,
        0, 0, 0, 4199, 2153, 4202, 0, 4204, 4206, 4208, 0, 2162,
        4211, 4213, 2167, 0, 4216, 2170, 2171, 0, 2172, 10365,
        4226, 4228, 0, 0, 0, 0, 0, 0, 0, 0, 2182, 0, 0, 2183,
        0, 0, 2184, 2185, 0, 2186, 2187, 0, 0, 0, 0, 8332, 0,
        0, 0, 0, 12432, 2198, 4247, 4249, 8347, 0, 2207, 2208,
        4257, 4259, 2213, 2214, 0, 2215, 0, 0, 4264, 4266, 4268,
        4270, 4272, 10418, 0, 0, 0, 2231, 0, 2232, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};


//935FC
obj_procs_t ObjectsFonctions[262] = {
        {DO_ONE_CMD,                       DoBadGuy1PoingCollision,          DoRaymanCollisionDefault,        DoBadGuy1RaymanZDD,        ObjectUTurnDefault}, // 0: livingstone
        {DO_ONE_CMD,                       DoPlatformPoingCollision,         DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 1: platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoPowerupRaymanCollision,  ObjectUTurnDefault}, // 2: energy point
        {DO_ONE_PINK_CMD,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoLidolPinkRaymanZDD,      ObjectUTurnDefault}, // 3: electoon
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 4: scenery
        {TEST_WIZARD,                      DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 5: magician
        {DO_ONE_CMD,                       DoFallingObjPoingCollision,       DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 6: yin with pin
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 7: swinging spiky fruit
        {DO_ONE_CMD,                       DoFallingObjPoingCollision,       DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 8: plum 1
        {DO_ONE_CMD,                       DoBadGuy23PoingCollision,         DoRaymanCollisionDefault,        DoBadGuy23RaymanZDD,       ObjectUTurnDefault}, // 9: small livingstone
        {DO_PYRANHA,                       DoFishPoingCollision,             DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 10: piranha
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 11: hit effect
        {DO_ONE_CMD,                       DoChasseurPoingCollision,         DoRaymanCollisionDefault,        DoChasseurRaymanZDD,       ObjectUTurnDefault}, // 12: hunter 1
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 13: hunter bullet
        {DO_ONE_CMD,                       DoChasseurPoingCollision,         DoRaymanCollisionDefault,        DoChasseurRaymanZDD,       ObjectUTurnDefault}, // 14: hunter 2
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 15:
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 16: falling platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 17: rising platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 18:
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 19: splash
        {DO_TEN_COMMAND,                   DoGeneBadGuyPoingCollision,       DoRaymanCollisionDefault,        DoGeneBadGuyRaymanZDD,     ObjectUTurnDefault}, // 20: tentacle
        {DO_PHOTOGRAPHE_CMD,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 21: photographer
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 22: moving platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 23: rayman
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 24:
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 25: ejecting platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 26: disappearing cloud
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 27: trampoline cloud
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 28: blinking cloud
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 29:
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoAudioStartRaymanCollision,     DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 30: audio start
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 31: 1-up collected
        {DO_DARK_PHASE2_COMMAND,           DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 32: Mr Dark 2nd phase
        {DO_DARK2_SORT_COMMAND,            DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 33: Mr Dark fireballs
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 34: horizontal nenuphar 2
        {DO_STONEMAN1_TIR,                 DO_PNG_COLL_STONEMAN,             DoRaymanCollisionDefault,        DoStoneMan1et2RaymanZDD,   ObjectUTurnDefault}, // 35: stoneman (throwing)
        {DO_STONEBOMB_COMMAND,             DO_STONE_EXPLOSION,               DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 36: stoneman lava ball
        {DO_TARZAN,                        DoTarzanPoingCollision,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 37: tarayzan
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoGraineRaymanCollision,         DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 38: magic seed
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 39: planted nenuphar
        {DO_STONEDOG_COMMAND,              DoStoneDogPoingCollision,         DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 40: stone dog
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 41: prickly
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoSignPostRaymanCollision,       DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 42: exit sign
        {DO_STONEMAN2_TIR,                 DO_PNG_COLL_STONEMAN,             DoRaymanCollisionDefault,        DoStoneMan1et2RaymanZDD,   ObjectUTurnDefault}, // 43: stoneman (shooting)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 44: stoneman flame
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 45: moving prickly
        {DO_BBMONT_COMMAND,                DO_BBMONT_TOUCHE,                 DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 46: Mr Stone
        {DO_STONEBOMB_COMMAND,             DO_STONE_EXPLOSION,               DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 47: bouncing lava ball
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 48: torch
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 49: moving flat stone
        {doMoskitoCommand,                 doMoskitoHit,                     DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 50: moskito (Bzzit)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 51: moskito fruit small
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 52: moskito fruit medium
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoMstShakyFruitRaymanZDD,  ObjectUTurnDefault}, // 53: shaky fruit
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 54: medaillon
        {DO_MUSICIEN,                      DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoMusicienRaymanInZDD,     ObjectUTurnDefault}, // 55: musician
        {DO_NOMOVE_STONEWOMAN_COMMAND,     DO_PNG_COLL_STONEWOMAN,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 56: nonmoving stoneman
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 57: stake
        {DO_CAGE,                          DoCagePoingCollision,             DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 58: cage
        {DO_CAGE2,                         DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 59: opened cage
        {DO_ONE_CMD,                       DoWatAndBigClownPoingCollision,   DoRaymanCollisionDefault,        DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 60: big clown
        {DO_ONE_CMD,                       DoWatAndBigClownPoingCollision,   DoRaymanCollisionDefault,        DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 61: big water clown
        {DO_PROP_COMMAND,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 62: water balloon
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 63: platform starts moving
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 64: autoscroll
        {DO_SPIDER_TIR,                    DoSpiderPoingCollision,           DoRaymanCollisionDefault,        DoSpiderRaymanZDD,         ObjectUTurnDefault}, // 65: spider
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 66: spider dart
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 67: swinging flower
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 68: big boing plat
        {DO_STONEBOMB_COMMAND,             DO_STONE_EXPLOSION,               DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 69: big bouncing boulder
        {DO_TRP_COMMAND,                   DoTrumpetPoingCollision,          DoRaymanCollisionDefault,        DoTrompetteRaymanZDD,      ObjectUTurnDefault}, // 70: trumpet
        {DO_NOTE_CMD,                      DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 71: note
        {DO_ONE_NGW_COMMAND,               DO_NGW_POING_COLLISION,           DoRaymanCollisionDefault,        NGW_REACT_TO_RAY_IN_ZONE,  ObjectUTurnDefault}, // 72: pirate with earring
        {DO_ONE_NGW_RING_COMMAND,          DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 73: earring
        {DO_SAXO_COMMAND,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 74: Mr Sax
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 75: bounce glitter
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 76: kildoor
        {DO_ONE_PAR_COMMAND,               DO_PAR_POING_COLLISION,           DoRaymanCollisionDefault,        PAR_REACT_TO_RAY_IN_ZONE,  ObjectUTurnDefault}, // 77: bomber pirate
        {DO_PAR_BOMB_COMMAND,              DoPoingCollisionDefault,          BombExplosion,                   DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 78: pirate bomb
        {DO_ONE_STONECHIP_COMMAND,         DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 79: lava ball fragment
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 80: bigstone
        {DO_CYMBAL_COMMAND,                DoPoingCollisionDefault,          DO_COLL_RAY_CYMBALE,             DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 81: cymbal
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoJaugeUpRaymanCollision,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 82: big powerup
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 83: explosion / dust effect
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 84: tibetan juggler
        {DO_ROLL_EYES,                     DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 85: eyes
        {DO_MOVE_MARACAS_COMMAND,          DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 86: maracas
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 87: tambourin platform1
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 88: tambourin platform2
        {DO_JOE_COMMAND,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoJoeRaymanZDD,            ObjectUTurnDefault}, // 89: Joe
        {DO_ONE_CMD,                       DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 90: Mr Sax wrong note
        {DO_ONE_CMD,                       DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 91: note bomb 1
        {DO_NOTE_CMD,                      DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 92: note bomb 2
        {DO_NOTE_CMD,                      DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 93: punched wrong note
        {DO_POING,                         DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 94: fist
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoPoingPowerupRaymanCollision,   DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 95: fist pickup
        {DO_TOTEM_COMMAND,                 DO_TOTEM_TOUCHE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 96: Mr Stone totem
        {DO_BBL_COMMAND,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 97: Mr Stone boulder
        {doMereDenisCommand,               doMereDenisHit,                   DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 98: Space Mama
        {DO_RAY_POS_CMD,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 99: Rayman start pos
        {DO_MIT_COMMAND,                   DoMitePoingCollision,             DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 100: moth
        {DO_ONE_CMD,                       DoMorningStarPoingCollision,      DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 101: swinging prickly
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 102: tiny wrong note
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 103: large eyed fish
        {DO_POISSON_VERT_CMD,              DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 104: large nosed fish
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 105: bouncing crystal rock 1
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 106: bouncing crystal rock 2
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 107: big spiky ball
        {DO_DARK2_TOONS_COMMAND,           DoPoingCollisionDefault,          ToonDonnePoing,                  DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 108: Mr Dark electoons
        {DO_ONE_CMD,                       DoPiPoingCollision,               DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 109: boulder
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 110: destroyed boulder
        {DO_ONE_CMD,                       DoPiMusPoingCollision,            DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 111: crushed guitar
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 112: washing machine
        {DO_BAG1_COMMAND,                  DoPoingCollisionDefault,          DoRaymanBag1Collision,           DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 113: mallets
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 114:
        {DO_BB1_PLAT_CMD,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 115: Mr Stone cloud
        {DO_CLOWN_TNT_COMMAND,             DoTntClownPoingCollision,         DoRaymanCollisionDefault,        DoTntClownRaymanZDD,       ObjectUTurnDefault}, // 116: TNT clown
        {DO_CLOWN_TNT2_COMMAND,            DoTntClownPoingCollision,         DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 117: TNT clown burst
        {DO_CLOWN_TNT3_COMMAND,            DoTntClownPoingCollision,         DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 118: TNT clown kamikaze
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          BombExplosion,                   DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 119: clown TNT
        {DO_BAT_COMMAND,                   DO_BAT_POING_COLLISION,           DoRaymanCollisionDefault,        BAT_ray_in_zone,           ObjectUTurnDefault}, // 120: red drummer
        {DO_ECLAIR_COMMAND,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 121: Mr Stone flame
        {DO_STONEDOG_COMMAND,              DoStoneDogPoingCollision,         DoRaymanCollisionDefault,        DoStoneDogRaymanZDD,       ObjectUTurnDefault}, // 122: Mr Stone stone dog
        {DO_BLKTOON_COMMAND,               DoBlackToonPoingCollision,        DoRaymanCollisionDefault,        DoBlackToonRaymanZDD,      blkUTurn          }, // 123: antitoon
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoPancarteRaymanCollision,       DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 124: return sign
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 125: candy platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 126: fork
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 127: swiss knife
        {DO_TIRE_BOUCHON_COMMAND,          DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 128: corkscrew
        {DO_PETIT_COUTEAU_COMMAND,         DoPetitCouteauPoingCollision,     DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 129: small knife
        {DO_BLKTOON_EYES_CMD,              DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 130: antitoon eyes
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 131: mallets spring
        {DO_POISSON_BLEU_CMD,              DoPoissonBleuPoingCollision,      DoRaymanCollisionDefault,        DoPoissonBleuRaymanZDD,    ObjectUTurnDefault}, // 132: large mouthed fish
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoSuperHelicoRaymanCollision,    DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 133: supercopter
        {DO_FALLING_OBJ_CMD,               DoFallingObjPoingCollision,       DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 134: plum2
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 135: flame Mr Stone
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 136: tiny Rayman
        {DO_REDUCTEUR,                     DoPoingCollisionDefault,          DoReducteurRaymanCollision,      DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 137: fairy
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 138: sprinkler (4 heads)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 139: maracas flying
        {DO_MOVING_PLATFORM_COMMAND,       DoPTGRAPPINPoingCollision,        DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 140: ring
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoNeigeRaymanCollision,          DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 141: snow
        {DO_ONE_CMD,                       DoOneUpPoingCollision,            DoOneUpRaymanCollision,          DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 142: extralife
        {DO_NOVA2_COMMAND,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 143: item blink
        {DO_ONE_PINK_CMD,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 144: lidolpink2
        {DO_ROLL_EYES,                     DoKillingEyesPoingCollision,      DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 145: bad eyes
        {DO_FLASH_COMMAND,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 146: eyes bolt
        {DO_MST_SCROLL_COMMAND,            DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DO_MST_CHANGE_COMMAND,     ObjectUTurnDefault}, // 147: moskito huge spiky fruit
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoGrapBonusRaymanCollision,      DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 148: grap bonus
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 149: treble clef
        {DO_SKO,                           DO_SKO_HIT,                       DoRaymanCollisionDefault,        SKO_ray_in_zone,           ObjectUTurnDefault}, // 150: Mr Skops
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 151: bullet
        {DO_CCL_COMMAND,                   DoCaisseClairePoingCollision,     DoRaymanCollisionDefault,        DoCaisseClaireRaymanZDD,   ObjectUTurnDefault}, // 152: snare drum
        {DO_FEE,                           DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoFeeRaymanZDD,            ObjectUTurnDefault}, // 153: Betilla
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 154: sprinkler (4 heads, moving)
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 155: sprinkler (3 heads)
        {DO_WLKNOT_COMMAND,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 156: walk note 1
        {DO_EAU_QUI_MONTE,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 157: water
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 158: palette change
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 159: tibetan 6 platforms
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 160: tibetan 2 platforms
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoWizRaymanCollision,            DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 161: ting
        {DO_MOVING_WITH_INDICATOR_COMMAND, DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 162: flying saucer platform
        {DO_IDC_COMMAND,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 163: circuit box
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoGeneratingDoorRaymanCollision, DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 164: gendoor
        {DO_PTI_ESQUIVE,                   DoBadGuy23PoingCollision,         DoRaymanCollisionDefault,        DoBadGuy23RaymanZDD,       ObjectUTurnDefault}, // 165: evil livingstone
        {DO_ONE_CMD,                       DO_LEV_POING_COLLISION,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 166: lever
        {DO_FALLING_OBJ_CMD,               DoFallingObjPoingCollision,       DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 167: plum 3
        {DO_2_PARTS_CYMBAL,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 168: cymbal1 single
        {DO_2_PARTS_CYMBAL,                DoPoingCollisionDefault,          DO_COLL_RAY_CYMBALE,             DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 169: cymbal2
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 170: Mr Skops beam
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 171: Rayman Bzzit cutscene
        {DO_STONEWOMAN_COMMAND,            DO_PNG_COLL_STONEWOMAN,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 172: stoneman (aggressive)
        {doShipCommand,                    DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 173: viking ship
        {DO_PIRATE_POELLE,                 DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault,        PIRATE_POELLE_REACT,       ObjectUTurnDefault}, // 174: space pirate (left)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 175: punaise platform
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 176: pencil pointing down
        {DO_ONE_CMD,                       DoFallingObjPoingCollision,       DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 177: yin1
        {doHerseCommand,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 178: pencil pointing down (moving)
        {doHerseCommand,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 179: pencil pointing down (wave)
        {DO_SAXO2_COMMAND,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 180: Mr Sax (chasing)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DO_SAXO_MARCHE,            ObjectUTurnDefault}, // 181: Mr Sax walk speed
        {DO_ONE_CMD,                       DO_NOTE_TOUCHEE,                  DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 182: wrong note stationary punchable
        {DO_SAXO3_COMMAND,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DO_SAXO3_DEBUT,            ObjectUTurnDefault}, // 183: Mr Sax (first encounter)
        {DO_PIRATE_POELLE,                 DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault,        PIRATE_POELLE_REACT,       ObjectUTurnDefault}, // 184: space pirate (right)
        {DO_WLKNOT_COMMAND,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 185: walk note 2
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 186: pirate pan flying away
        {DO_PMA_COMMAND,                   DO_PMA_POING_COLLISION,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 187: Viking Mama
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 188: red crystal platform
        {DO_MOVING_PLATFORM_COMMAND,       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 189: red crystal platform (moving)
        {DO_COU_COMMAND,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 190: Viking Mama sword
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 191: pencil (falling)
        {mereDenisBigLaserCommand,         DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 192: Space Mama beam
        {mereDenisBombCommand,             DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 193: Space Mama pot
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 194: Space Mama pot part
        {DO_SPIDER_PLAFOND,                DO_SPIDER_PLAFOND_POING_COLLISION,DoRaymanCollisionDefault,        SPIDER_PLAFOND_REACT,      ObjectUTurnDefault}, // 195: spider (ceiling)
        {DO_DARD_PLAFOND_ALWAYS,           DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 196: spider homing dart
        {DO_MEDAILLON_TOON,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 197: medaillon
        {DO_BBMONT2_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 198: Mr Stone (chase)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DO_BBMONT2_MARCHE,         ObjectUTurnDefault}, // 199: Mr Stone (chase, walking)
        {DO_BBMONT3_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 200: BB13
        {DO_BBMONT4_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 201: BB14
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 202: stars on Rayman
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 203: washing machine 2
        {doBlackRaymanCommand,             DoPoingCollisionDefault,          doBlKRayRaymanCollision,         DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 204: dark Rayman
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          doBlKRayRaymanCollision,         DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 205: dark Rayman fist
        {DO_PIEDS_RAYMAN,                  DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 206: Rayman's feet
        {DO_POELLE_COMMAND,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 207: frying pan
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 208: dust circle
        {DoFlammeCommand,                  DoPoingCollisionDefault,          DoFlammeRaymanCollision,         DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 209: Mr Dark flame (left)
        {DoFlammeCommand,                  DoPoingCollisionDefault,          DoFlammeRaymanCollision,         DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 210: Mr Dark flame (right)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 211: Mr Stone swinging totem
        {DO_DARK_COMMAND,                  DO_DARK_POING_COLLISION,          DoRaymanCollisionDefault,        DO_DARK_REACT,             ObjectUTurnDefault}, // 212: Mr Dark
        {doMereDenisCommand,               doMereDenisHit,                   DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 213: Space Mama (with washing machine)
        {DO_BOUEE_JOE_COMMAND,             DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 214: buoy
        {DO_DARK_SORT_COMMAND,             DoPoingCollisionDefault,          DO_DARK_SORT_COLLISION,          DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 215: Mr Dark spell fireball
        {DO_ENSEIGNE_COMMAND,              DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 216: Eat at Joe's sign
        {DO_MITE2_COMMAND,                 DoMite2PoingCollision,            DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 217: moth 2
        {doMOSAMScommand,                  DoMOSAMSPoingCollision,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 218: Spaxkito
        {DO_CORDE_COMMAND,                 DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 219: flat stone rope
        {DO_PAC_COMMAND,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 220: flat stone (with ropes)
        {DO_CFUMEE_COMMAND,                DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 221: rope dust
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 222: flat stone (detached rope)
        {doSTOSKOcommand,                  DoSTOSKOPoingCollision,           DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 223: Stoneskops
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 224: Stoneskops claw
        {DO_PIRATE_POELLE,                 DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault,        PIRATE_POELLE_REACT,       ObjectUTurnDefault}, // 225: pan pirate (45 degrees)
        {DO_PIRATE_POELLE,                 DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault,        PIRATE_POELLE_REACT,       ObjectUTurnDefault}, // 226: pan pirate 2 (45 degrees)
        {doMoskitoCommand,                 doMoskitoHit,                     DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 227: Moskito
        {DO_ONE_CMD,                       DoPrisePoingCollision,            DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 228: electric plug
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 229: punaise (facing down)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 230: punaise (horizontal)
        {doBBF2command,                    DO_HYB_BBF2_POING_COLLISION,      DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 231: Moskitomama (right)
        {doBBF2command,                    DO_HYB_BBF2_POING_COLLISION,      DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 232: Moskitomama (left)
        {DO_HYB_BBF2_LAS,                  DoPoingCollisionDefault,          DoHybBBF2LasRaymanCollision,     DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 233: Moskitomama beam
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 234: lava
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 235: punaise
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoAnnuleDarkSortRaymanCollision, DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 236: cancel Mr Dark spell
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 237: eraser
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 238: Betilla text fist
        {DO_ONE_PAR_COMMAND,               DO_PAR_POING_COLLISION,           DoRaymanCollisionDefault,        PAR_REACT_TO_RAY_IN_ZONE,  ObjectUTurnDefault}, // 239: viking ship nest
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 240: pencil (pointing up)
        {doHerseCommand,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 241: pencil (pointing up, moving)
        {doHerseCommand,                   DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 242: pencil (pointing up, wave)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 243: stylo
        {mereDenisBigLaserCommand,         DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 244: Space Mama second beam
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 245: Joe's beach
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 246: Mr Dark separate rope
        {DO_VITRAIL_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 247: vitrail
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 248: Mr Skops claw
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 249: theatre curtain
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 250: punaise (up down)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 251: theatre water (front)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 252: theatre water (back)
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 253: planches
        {DO_SLOPEY_PLAT_CMD,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 254: slopey plat
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 255: invalid
        {DO_CB_BRIK_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 256: breakout brick 20
        {DO_CB_BALL_COMMAND,               DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 257: breakout game
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 258: nougat
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 259: EDU letter
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 260: EDU digit
        {DO_ONE_CMD,                       DoPoingCollisionDefault,          DoRaymanCollisionDefault,        DoRaymanInZDDDefault,      ObjectUTurnDefault}, // 261: EDU road sign
};

u8 nb_levels_in_world[8] = {0, 22, 18, 13, 13, 12, 4, 0}; //94A74
i16 pente[16] = {0, 3, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 42, 45}; //94A7C

//94A9C
u8 N_anim[256] = {
        8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

i16 bagDy[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -60, -26, 8, 32, 75, 32, 8, -26, -60, -1}; //94B9C
i16 bagW[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 48, 48, 48, 48, 48, 32, 32, 0}; //94BC4
i16 bagH[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 48, 48, 60, 48, 48, 32, 32, 0}; //94BEC

//94C14
i16 expsin[63] = {
        0, 855, 825, 0, -769, -742, -1, 690, 666, 0, -621, -599, -1, 557, 537, 0,
        -501, -484, -1, 449, 434, 0, -405, -391, -1, 363, 350, 0, -327, -315, -1, 292,
        282, 0, -264, -255, -1, 236, 228, 0, -213, -206, -1, 190, 184, 0, -172, -166,
        -1, 153, 148, 0, -139, -134, -1, 124, 120, 0, -112, -108, -1, 80, 32640,
};

//94C92
i16 expsin2[48] = {
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0,
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0,
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0,
};

i16 EauDy[14] = {0, -3, -6, -8, -9, -10, -9, -7, -5, -2, 2, 6, 7, 4}; //94CF4
i16 DecEau[4] = {9, 0, 7, 5}; //94D10

//94D18
u8 angle_tab[65] = {
        128, 126, 125, 124, 122, 121, 120, 119, 117, 116, 115,
        114, 112, 111, 110, 109, 108, 106, 105, 104, 103, 102,
        101, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88,
        87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 78, 77, 76,
        75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 68, 67, 66,
        65, 65, 64, 64
};

//94D5C
world_info_t t_world_info[24] = {
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

//94F3C
u8 map_cd_tracks[128] = {
        14, 14, 9, 14, 14, 5, 14, 9, 14, 14, 14, 14, 14, 14, 14, 5, 9, 15, 15, 15, 15, 15,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 9, 17, 17, 17, 7, 7, 15, 15, 0, 0, 0, 0,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 6, 9, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        13, 13, 13, 4, 13, 13, 13, 13, 13, 13, 4, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 3, 3, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        11, 11, 11, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//94FBC
i32 timeCd[132] = {
        0, 215, 215, 80, 215, 215, 74, 215, 80, 215, 215, 215, 215, 215, 215, 215, 74, 80, 68, 68, 68, 68,
        68, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 80, 238, 238, 238, 84, 84, 68, 68, 0, 0, 0,
        0, 109, 109, 109, 109, 109, 109, 109, 109, 109, 82, 80, 68, 68, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 145, 145, 145, 118, 145, 145, 145, 145, 145, 145, 118, 68, 68, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 142, 142, 142, 142, 142, 142, 142, 142, 142, 81, 81, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 132, 132, 132, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 let_shadow = 0; //951D0

//95828
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
u8 ModeVideoActuel = MODE_TEXT; //95E2C
u8 GameModeVideo = MODE_TEXT; //95E2D
i32 JumAmpDefMul = 0; //95E30
i32 JumAmpDefPlus = 0; //95E34
u8 key_right = SC_RIGHT; //95E38
u8 key_up = SC_UP; //95E39
u8 key_left = SC_LEFT; //95E3A
u8 key_down = SC_DOWN; //95E3B
u8 key_fist = SC_ALT; //95E3C
u8 key_jump = SC_CONTROL; //95E3D
u8 key_action = SC_X; //95E3E
u8* tab_key[7] = {&key_left, &key_up, &key_right, &key_down, &key_jump, &key_fist, &key_action}; //95E40
u8 tab_key_sav[7] = {SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, SC_CONTROL, SC_ALT, SC_X}; //95E5C
u8 OptionGame = 0; //95E63
u8 GoMenu = 0; //95E64
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
u8 text_input_buffer[9]; //95EB7
u8 byte_95EC0; //95EC0
u8 byte_95EC1; //95EC1

i16 niveau; //95EC4

//95EC6
i16 SerieAtakBB[85] = {
        0, 2, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1,
        3, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 2, 2, 0, 1,
        1, 1, 0, 0, 255, 0, 0, 0, 3, 0, 0, 1, 1, 1, 255, 0,
        0, 0, 0, 0, 0, 0, 5, 1, 2, 3, 0, 0, 3, 1, 4, 0, 255,
        0, 0, 0, 7, 0, 3, 0, 6, 0, 3, 0, 0, 1, 4, 0, 255, 0, 0,
};

//95F70
rgb_t PalPlasma[16] = {
        {32, 7, 63}, {31, 7, 61}, {32, 7, 58}, {33, 7, 56},
        {36, 10, 53}, {37, 12, 51}, {45, 15, 48}, {52, 17, 46},
        {45, 15, 48}, {40, 15, 51}, {35, 12, 53}, {35, 10, 56},
        {32, 7, 58}, {30, 5, 61}, {27, 5, 61}, {32, 7, 63},
};
i16 word_95FA0; //95FA0
i16 word_95FA2; //95FA2
i16 word_95FA4; //95FA4
i16 word_95FA6; //95FA6
i16 word_95FA8; //95FA8
i16 word_95FAA; //95FAA
i16 word_95FAC; //95FAC
i16 word_95FAE; //95FAE
i16 word_95FB0; //95FB0

//95FB2
rgb_t PalFire[16] = {
        {0, 0, 9}, {15, 5, 19}, {20, 10, 21}, {25, 10, 34},
        {30, 10, 36}, {35, 10, 39}, {40, 10, 41}, {45, 10, 41},
        {50, 10, 46}, {55, 10, 49}, {60, 10, 51}, {60, 10, 54},
        {60, 10, 56}, {60, 10, 59}, {60, 10, 61}, {60, 10, 63},
};

i16 no_sound = 0; //95FE4
u8 InvadersMode = 0; //95FE6
u8 BonusMode = 0; //95FE7
u8 BonusBrik = 20; //95FE8
u8 BonusBrikType = 0; //95FE9
u8 NbClassicBrik = 0; //95FEA
u8 NbBrikTouched = 0; //95FEB
i16 CasseBriqueOffsetX[7] = {0, 0, 6, 2, 5, 4, 8}; //95FEC
i16 CasseBriqueOffsetY[7] = {16, 16, 8, 8, 16, 16, 8}; //95FFA
i16 LastTouched = 20; //96008
i16 NoCB = 0; //9600A
i16 NoCBInit = 0; //9600C
i16 tempo = 500; //9600E
i16 CBoffsetX = 0; //96010
i16 CBoffsetY = 0; //96012
i16 Xdir = 0; //96014
i16 Ydir = 0; //96016

i16 speed_ball_angle = 18; //9629E
i16 speed_ball_level = 0; //962A0
i16 speed_ball_x = 0; //962A2
i16 speed_ball_y = 0; //962A4
i32 ball_x = 0; //962A8
i32 ball_y = 0; //962AC
i32 dword_962B0 = 441; //962B0
u8 nb_max_collis = 0; //962B4
u8 cbout = 0; //962B5
u8 undestroy = 0; //962B6
u8 upscreen = 0; //962B7
u8 casse_brique_active = 0; //962B8
u8 CasseBriqueON = 0; //962B9
i16 Toon_Speeds_x[7] = {-3, -2, -1, 0, 1, 1, 3}; //962BC
i16 Toon_Speeds_y[7] = {-3, -5, -4, -6, -5, -3, -3}; //962CA

u8 dark_attaque = 0; //9637D
u8 type_dark_attaque = 0; //9637E
u8 ButtonReleasedSav3 = 0; //96380

u8 effect_choice; //964C9
u8 byte_964CA; //964CA
u8 effect_speed; //964CC
u8 effect_pause_bloque; //964CE

u8 playing_intro_video = 0; //964DD

i32 prev_Bloc_lim_W1; //9654C

//96550
archive_header_t snd8b_header_infos[7] = {
        {0, 2048, 77, 195},
        {2048, 2048, 217, 193},
        {4096, 2048, 36, 142},
        {6144, 2048, 250, 22},
        {8192, 2048, 103, 73},
        {10240, 2048, 171, 183},
        {12288, 2048, 99, 222},
};

//965A4
archive_header_t snd8b_data_infos[7] = {
        {0, 119104, 192, 60},
        {119104, 139852, 148, 104},
        {258956, 290280, 41, 149},
        {549236, 202780, 237, 23},
        {752016, 200344, 36, 180},
        {952360, 193264, 243, 59},
        {1145624, 185736, 248, 51},
};

//965F8
archive_header_t sndvig_infos[9] = {
        {0, 83700, 77, 96},
        {83700, 134576, 217, 38},
        {218276, 118290, 36, 67},
        {336566, 83104, 250, 122},
        {419670, 35392, 103, 177},
        {455062, 125648, 171, 164},
        {580710, 98378, 99, 219},
        {679088, 25472, 71, 84},
        {704560, 68350, 135, 79},
};

u8* bnkDataFixe; //966B0
u8* bnkDataWorld; //966B4 // original name: sound_buffer

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

u8* ptrTchatch = 0; //967B8
u8 (*TEST_KEY_BUTTON0)(void) = TestKeyWeapon; //967BC
u8 (*TEST_KEY_BUTTON1)(void) = TestKeyJump; //967C0
u8 (*TEST_KEY_BUTTON2)(void) = TestKeyAction; //967C4
u8 (*TEST_KEY_BUTTON3)(void) = TestKeyAction; //967C8
u8 byte_967CC = 0; //967CC
i32 XSpeedJumelle = 0; //967D0
i32 YSpeedJumelle = 0; //967D4

i16 TabW1[4] = {4, 12, 20, 24}; //9684C // TODO: change TabW1[0] to 0 here to fix black bar on left of screen?
i16 TabW2[4] = {320, 312, 304, 300}; //96854
i16 TabH1[4] = {0, 8, 16, 20}; //9685C
i16 TabH2[4] = {200, 192, 184, 180}; //96864

u8 msg_box_being_displayed = 0; //96880
u8 first_option = 0; //96881
u8 yesno_finished = 0; //96882
u8 byte_96883 = 0; //96883
u8 byte_96884 = 64; //96884

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

u8 dyingSequence[5] = {16, 23, 24, 22, 25}; //96F24
u8 hitSequence[6] = {16, 17, 15, 26, 11, 5}; //96F29
u8 arrivalSequence[4] = {13, 26, 14, 3}; //96F2F

///96F33
u8 firstEncounterSequence[40] = {
        13, 26, 14, 4, 19, 21, 4, 20, 18, 21, 4, 20, 19, 21, 4, 20, 18, 21, 4, 20, 10, 26,
        14, 4, 18, 21, 4, 20, 10, 26, 14, 4, 19, 21, 4, 20, 10, 26, 14, 2
};

u8 secondEncounterSequence1[13] = {13, 26, 14, 4, 7, 0, 7, 0, 10, 26, 14, 4, 3}; //96F5B
u8 secondEncounterSequenceSuite[12] = {7, 4, 7, 8, 10, 26, 14, 13, 26, 14, 4, 2}; //96F68
u8 secondEncounterSequence2[21] = {8, 0, 8, 0, 8, 0, 8, 4, 18, 21, 4, 20, 19, 21, 4, 20, 10, 26, 14, 4, 2}; //96F74
u8 secondEncounterSequence3[19] = {9, 0, 18, 21, 4, 20, 9, 4, 18, 21, 4, 20, 10, 26, 14, 13, 14, 4, 2}; //96F89
u8 secondEncounterSequence4[21] = {7, 8, 8, 8, 9, 0, 19, 21, 4, 20, 9, 8, 18, 21, 4, 20, 10, 26, 14, 4, 2}; //96F9C

////96FB4
u8* moskitoActionSequences[8] = {
        arrivalSequence, secondEncounterSequence1, secondEncounterSequenceSuite, secondEncounterSequence2,
        secondEncounterSequence3, secondEncounterSequence4, hitSequence, dyingSequence,
};

//96FE0
i16 costab[130] = {
        512, 512, 512, 512, 512, 512, 511, 511, 510, 509, 509,
        508, 507, 506, 505, 504, 503, 501, 500, 499, 497, 496,
        494, 492, 490, 489, 487, 485, 483, 480, 478, 476, 474,
        471, 469, 466, 463, 461, 458, 455, 452, 449, 446, 443,
        440, 436, 433, 430, 426, 423, 419, 415, 412, 408, 404,
        400, 396, 392, 388, 384, 380, 376, 371, 367, 363, 358,
        354, 349, 344, 340, 335, 330, 325, 320, 316, 311, 306,
        300, 295, 290, 285, 280, 274, 269, 264, 258, 253, 247,
        242, 236, 231, 225, 219, 214, 208, 202, 196, 191, 185,
        179, 173, 167, 161, 155, 149, 143, 137, 131, 125, 119,
        113, 107, 100, 94, 88, 82, 76, 69, 63, 57, 51, 44, 38,
        32, 26, 19, 13, 7, 1, 0,
};



//970E4
cptr_t cptr_tab[] = {
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 0
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 1
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 2
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 3
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 4
        {readOneArg, skipOneArg, handle_5_GO_SUBSTATE}, // 5
        {readOneArg, skipOneArg, handle_6_GO_SKIP}, // 6
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 7
        {readOneArg, skipOneArg, handle_8_GO_STATE}, // 8
        {readOneArg, skipOneArg, handle_9_GO_PREPARELOOP}, // 9
        {readNoArg, skipNoArg, handle_10_GO_DOLOOP}, // 10
        {readOneArg, skipOneArg, handle_11_GO_LABEL}, // 11
        {readOneArg, skipOneArg, handle_12_GO_GOTO}, // 12
        {readOneArg, skipOneArg, handle_13_GO_GOSUB}, // 13
        {readNoArg, skipNoArg, handle_14_GO_RETURN}, // 14
        {readOneArg, skipOneArg, handle_15_GO_BRANCHTRUE}, // 15
        {readOneArg, skipOneArg, handle_16_GO_BRANCHFALSE}, // 16
        {readTestArgs, skipTestArgs, handle_17_GO_TEST}, // 17
        {readOneArg, skipOneArg, handle_18_GO_SETTEST}, // 18
        {readOneArg, skipOneArg, handle_19_GO_WAITSTATE}, // 19
        {readSpeedArgs, skipSpeedArgs, handle_SELF_HANDLED}, // 20
        {readGoXYargs, skipGoXYArgs, handle_21_GO_X}, // 21
        {readGoXYargs, skipGoXYArgs, handle_22_GO_Y}, // 22
        {readOneArg, skipOneArg, handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO}, // 23
        {readOneArg, skipOneArg, handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO}, // 24
        {readOneArg, skipOneArg, handle_25_RESERVED_GO_GOSUB}, // 25
        {readOneArg, skipOneArg, handle_26_RESERVED_GO_BRANCHTRUE}, // 26
        {readOneArg, skipOneArg, handle_27_RESERVED_GO_BRANCHFALSE}, // 27
        {readOneArg, skipOneArg, handle_28_RESERVED_GO_SKIPTRUE}, // 28
        {readOneArg, skipOneArg, handle_29_RESERVED_GO_SKIPFALSE}, // 29
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 30
        {readOneArg, skipOneArg, handle_31_GO_SKIPTRUE}, // 31
        {readOneArg, skipOneArg, handle_32_GO_SKIPFALSE}, // 32
        {readInvalidArg, skipInvalidArg, handle_33_INVALID_CMD}, // 33
};

u8 pma_couteaux[6] = {3, 5, 5, 3, 3, 4}; //9727C
u8 pma_sequence[8] = {0, 3, 5, 0, 5, 1, 4, 2}; //97282
u8 place_sequence[5] = {3, 4, 2, 3, 2}; //9728A
u8 pst2_sequence[26] = {1, 0, 21, 0, 156, 255, 8, 0, 1, 0, 0, 0, 18, 0, 160, 0, 8, 0, 3, 0, 42, 0, 19, 0, 14, 0}; //97290
u8 moust1_sequence[18] = {1, 0, 21, 0, 156, 255, 8, 0, 6, 0, 15, 0, 18, 0, 74, 1, 14, 0}; //972AA
u8 moust2_sequence[18] = {0, 0, 21, 0, 74, 1, 8, 0, 6, 0, 16, 0, 18, 0, 136, 255, 14, 0}; //972BC
u8 pst1_sequence[26] = {0, 0, 21, 0, 64, 1, 8, 0, 1, 0, 0, 0, 18, 0, 248, 255, 8, 0, 3, 0, 41, 0, 19, 0, 14, 0}; //972CE
u8 worry_sequence[24] = {0, 0, 21, 0, 100, 0, 8, 0, 3, 0, 43, 0, 19, 0, 8, 0, 3, 0, 44, 0, 19, 0, 14, 0}; //972E8

//97300
u8 car_sequence[66] = {
        0, 0, 21, 0, 200, 0, 8, 0, 3, 0, 47, 0, 19, 0, 8, 0,
        3, 0, 48, 0, 19, 0, 17, 0, 18, 0, 20, 0, 248, 255, 17,
        0, 30, 0, 20, 0, 240, 255, 17, 0, 45, 0, 20, 0, 224,
        255, 17, 0, 60, 0, 20, 0, 192, 255, 17, 0, 65, 0, 20,
        0, 0, 0, 19, 0, 14, 0,
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

//97544
u8 sound_table[256] = {
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

//97644
u8 frame_snd[256] = {
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, // 0-15
        28, 0, 1, 2, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 7, 2, 0, 0, 0, // 32-47
        0, 0, 30, 15, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, // 48-63
        0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64-79
        0, 0, 10, 0, 0, 0, 0, 0, 30, 54, 0, 0, 0, 0, 0, 0, // 80-95
        0, 0, 0, 0, 40, 0, 0, 0, 0, 30, 60, 0, 60, 40, 0, 0, // 96-111
        0, 0, 0, 0, 0, 20, 0, 60, 1, 0, 0, 0, 0, 9, 0, 0, // 112-127
        0, 0, 0, 0, 0, 0, 0, 10, 0, 11, 0, 0, 0, 0, 31, 25, // 128-143
        30, 0, 0, 0, 0, 0, 0, 40, 0, 75, 0, 0, 0, 50, 0, 0, // 144-159
        0, 0, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 60, 0, // 160-175
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176-191
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 12, 0, 0, 0, 0, 0, // 192-207
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, // 208-223
        0, 0, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 70, // 224-239
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 20, 0, 0, 0, 0, // 240-255
};

//97744
u8 frame_snd_bis[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65, 0, 0, 0, 0, 0, // 32-47
        0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, // 48-63
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64-79
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-95
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96-111
        0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, // 112-127
        0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, // 128-143
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144-159
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160-175
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176-191
        0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, // 192-207
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208-223
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 15, // 224-239
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, // 240-255
};

//97844
u16 snd_bis[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, // 32-47
        0, 0, 0, 0, 0, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, // 48-63
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64-79
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-95
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96-111
        0, 0, 0, 0, 0, 0, 0, 0, 120, 0, 0, 0, 0, 0, 0, 0, // 112-127
        0, 0, 0, 0, 0, 0, 0, 0, 0, 137, 0, 0, 0, 0, 0, 0, // 128-143
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 144-159
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 160-175
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 176-191
        0, 0, 0, 0, 0, 0, 0, 199, 0, 0, 0, 0, 0, 0, 0, 0, // 192-207
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 208-223
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 238, 23, // 224-239
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, // 240-255
};

//97A44
sound_table_entry_t hard_sound_table[256] = {
        {0, 255, 0, 127}, // 0
        {53, 15, 0, 80}, // 1
        {53, 0, 0, 90}, // 2
        {53, 255, 0, 90}, // 3
        {60, 255, 0, 127}, // 4
        {53, 5, 0, 115}, // 5
        {55, 3, 0, 50}, // 6
        {53, 255, 0, 80}, // 7
        {53, 8, 0, 55}, // 8
        {53, 12, 0, 50}, // 9
        {53, 255, 0, 80}, // 10
        {72, 6, 0, 90}, // 11
        {53, 8, 0, 60}, // 12
        {65, 1, 0, 60}, // 13
        {53, 7, 0, 80}, // 14
        {58, 13, 0, 80}, // 15
        {54, 4, 0, 80}, // 16
        {53, 31, 0, 80}, // 17
        {55, 30, 0, 80}, // 18
        {62, 16, 0, 80}, // 19
        {55, 21, 0, 65}, // 20
        {53, 22, 0, 80}, // 21
        {49, 23, 0, 80}, // 22
        {60, 25, 0, 60}, // 23
        {55, 24, 0, 75}, // 24
        {53, 76, 0, 80}, // 25
        {53, 255, 0, 80}, // 26
        {56, 255, 0, 80}, // 27
        {53, 71, 0, 60}, // 28
        {53, 72, 0, 65}, // 29
        {53, 73, 0, 80}, // 30
        {53, 74, 0, 80}, // 31
        {60, 75, 0, 80}, // 32
        {60, 255, 0, 127}, // 33
        {53, 104, 0, 45}, // 34
        {55, 79, 0, 60}, // 35
        {54, 80, 0, 60}, // 36
        {57, 81, 0, 60}, // 37
        {53, 255, 0, 80}, // 38
        {53, 255, 0, 80}, // 39
        {53, 26, 0, 80}, // 40
        {53, 85, 0, 60}, // 41
        {53, 93, 0, 40}, // 42
        {53, 87, 0, 80}, // 43
        {53, 88, 0, 80}, // 44
        {53, 89, 0, 100}, // 45
        {53, 90, 0, 80}, // 46
        {53, 91, 0, 60}, // 47
        {53, 92, 0, 80}, // 48
        {53, 70, 0, 70}, // 49
        {56, 27, 0, 80}, // 50
        {51, 5, 0, 80}, // 51
        {57, 96, 0, 80}, // 52
        {53, 1, 0, 90}, // 53
        {56, 86, 0, 80}, // 54
        {60, 39, 0, 80}, // 55
        {65, 39, 0, 80}, // 56
        {70, 39, 0, 80}, // 57
        {53, 21, 0, 50}, // 58
        {53, 22, 0, 60}, // 59
        {53, 32, 0, 30}, // 60
        {53, 33, 0, 60}, // 61
        {53, 106, 0, 80}, // 62
        {53, 99, 0, 80}, // 63
        {53, 107, 0, 80}, // 64
        {53, 107, 0, 80}, // 65
        {53, 73, 0, 80}, // 66
        {53, 84, 0, 70}, // 67
        {53, 12, 0, 40}, // 68
        {53, 11, 0, 70}, // 69
        {58, 94, 0, 80}, // 70
        {51, 95, 0, 127}, // 71
        {53, 96, 0, 80}, // 72
        {60, 255, 0, 127}, // 73
        {53, 255, 0, 80}, // 74
        {53, 105, 0, 45}, // 75
        {53, 97, 0, 80}, // 76
        {53, 95, 0, 95}, // 77
        {53, 21, 0, 80}, // 78
        {53, 22, 0, 80}, // 79
        {53, 82, 0, 80}, // 80
        {53, 255, 0, 80}, // 81
        {53, 84, 0, 80}, // 82
        {53, 51, 0, 95}, // 83
        {54, 52, 0, 70}, // 84
        {53, 70, 0, 80}, // 85
        {53, 71, 0, 40}, // 86
        {53, 72, 0, 60}, // 87
        {56, 73, 0, 127}, // 88
        {53, 74, 0, 80}, // 89
        {53, 75, 0, 80}, // 90
        {53, 61, 0, 80}, // 91
        {53, 62, 0, 60}, // 92
        {53, 54, 0, 80}, // 93
        {53, 55, 0, 50}, // 94
        {48, 56, 0, 127}, // 95
        {53, 77, 0, 80}, // 96
        {53, 78, 0, 115}, // 97
        {53, 79, 0, 100}, // 98
        {53, 50, 0, 45}, // 99
        {53, 80, 0, 115}, // 100
        {53, 255, 0, 80}, // 101
        {53, 255, 0, 80}, // 102
        {53, 255, 0, 80}, // 103
        {53, 97, 0, 127}, // 104
        {53, 98, 0, 80}, // 105
        {53, 99, 0, 80}, // 106
        {55, 100, 0, 80}, // 107
        {53, 101, 0, 80}, // 108
        {53, 102, 0, 80}, // 109
        {53, 71, 0, 70}, // 110
        {55, 70, 0, 70}, // 111
        {49, 71, 0, 70}, // 112
        {53, 103, 0, 80}, // 113
        {53, 75, 0, 115}, // 114
        {50, 94, 0, 127}, // 115
        {51, 72, 0, 80}, // 116
        {53, 73, 0, 80}, // 117
        {57, 255, 0, 80}, // 118
        {53, 75, 0, 120}, // 119
        {55, 76, 0, 127}, // 120
        {53, 77, 0, 110}, // 121
        {53, 78, 0, 80}, // 122
        {53, 80, 0, 80}, // 123
        {56, 79, 0, 55}, // 124
        {55, 81, 0, 80}, // 125
        {63, 82, 0, 55}, // 126
        {53, 255, 0, 80}, // 127
        {62, 84, 0, 45}, // 128
        {57, 85, 0, 80}, // 129
        {60, 86, 0, 80}, // 130
        {53, 87, 0, 90}, // 131
        {53, 88, 0, 127}, // 132
        {53, 255, 0, 80}, // 133
        {53, 19, 0, 37}, // 134
        {53, 15, 0, 65}, // 135
        {53, 36, 0, 90}, // 136
        {53, 34, 0, 80}, // 137
        {53, 35, 0, 80}, // 138
        {53, 98, 0, 45}, // 139
        {55, 98, 0, 45}, // 140
        {53, 255, 0, 80}, // 141
        {53, 73, 0, 80}, // 142
        {50, 74, 0, 80}, // 143
        {65, 75, 0, 80}, // 144
        {53, 103, 0, 100}, // 145
        {55, 76, 0, 30}, // 146
        {59, 77, 0, 40}, // 147
        {53, 255, 0, 80}, // 148
        {53, 79, 0, 80}, // 149
        {53, 255, 0, 80}, // 150
        {53, 81, 0, 80}, // 151
        {53, 255, 0, 80}, // 152
        {61, 83, 0, 120}, // 153
        {53, 84, 0, 45}, // 154
        {53, 85, 0, 80}, // 155
        {53, 86, 0, 80}, // 156
        {53, 51, 0, 95}, // 157
        {53, 87, 0, 80}, // 158
        {53, 88, 0, 80}, // 159
        {53, 89, 0, 85}, // 160
        {53, 90, 0, 55}, // 161
        {60, 72, 0, 70}, // 162
        {60, 72, 0, 70}, // 163
        {53, 73, 0, 80}, // 164
        {60, 51, 0, 70}, // 165
        {53, 255, 0, 80}, // 166
        {53, 63, 0, 110}, // 167
        {53, 76, 0, 80}, // 168
        {53, 77, 0, 55}, // 169
        {53, 78, 0, 80}, // 170
        {53, 79, 0, 80}, // 171
        {53, 80, 0, 80}, // 172
        {50, 255, 0, 80}, // 173
        {53, 82, 0, 80}, // 174
        {55, 83, 0, 80}, // 175
        {48, 84, 0, 90}, // 176
        {46, 85, 0, 80}, // 177
        {50, 255, 0, 80}, // 178
        {53, 86, 0, 80}, // 179
        {53, 70, 0, 40}, // 180
        {53, 71, 0, 40}, // 181
        {53, 72, 0, 80}, // 182
        {53, 73, 0, 80}, // 183
        {53, 74, 0, 55}, // 184
        {53, 75, 0, 80}, // 185
        {53, 76, 0, 80}, // 186
        {53, 77, 0, 80}, // 187
        {53, 78, 0, 100}, // 188
        {53, 79, 0, 127}, // 189
        {53, 80, 0, 80}, // 190
        {53, 81, 0, 80}, // 191
        {50, 37, 0, 65}, // 192
        {53, 38, 0, 60}, // 193
        {53, 11, 0, 80}, // 194
        {37, 104, 0, 30}, // 195
        {60, 255, 0, 127}, // 196
        {53, 8, 0, 80}, // 197
        {53, 100, 0, 80}, // 198
        {56, 1, 0, 80}, // 199
        {53, 53, 0, 50}, // 200
        {53, 60, 0, 45}, // 201
        {53, 76, 0, 80}, // 202
        {53, 98, 0, 100}, // 203
        {53, 41, 0, 40}, // 204
        {53, 57, 0, 80}, // 205
        {53, 40, 0, 100}, // 206
        {60, 39, 0, 105}, // 207
        {53, 98, 0, 80}, // 208
        {53, 255, 0, 80}, // 209
        {53, 255, 0, 80}, // 210
        {53, 255, 0, 80}, // 211
        {53, 76, 0, 127}, // 212
        {60, 96, 0, 80}, // 213
        {53, 42, 0, 80}, // 214
        {53, 255, 0, 80}, // 215
        {53, 43, 0, 80}, // 216
        {53, 255, 0, 80}, // 217
        {46, 90, 0, 90}, // 218
        {53, 92, 0, 80}, // 219
        {53, 95, 0, 40}, // 220
        {53, 94, 0, 50}, // 221
        {53, 94, 0, 80}, // 222
        {53, 82, 0, 80}, // 223
        {53, 83, 0, 80}, // 224
        {50, 84, 0, 70}, // 225
        {52, 77, 0, 80}, // 226
        {56, 74, 0, 80}, // 227
        {50, 74, 0, 80}, // 228
        {53, 90, 0, 80}, // 229
        {53, 89, 0, 80}, // 230
        {53, 85, 0, 80}, // 231
        {48, 86, 0, 80}, // 232
        {53, 87, 0, 80}, // 233
        {57, 88, 0, 80}, // 234
        {50, 76, 0, 80}, // 235
        {60, 255, 0, 127}, // 236
        {60, 255, 0, 127}, // 237
        {53, 51, 0, 127}, // 238
        {60, 25, 0, 60}, // 239
        {65, 21, 0, 50}, // 240
        {65, 22, 0, 60}, // 241
        {53, 93, 0, 55}, // 242
        {53, 94, 0, 127}, // 243
        {53, 94, 0, 127}, // 244
        {53, 92, 0, 40}, // 245
        {55, 95, 0, 127}, // 246
        {53, 55, 0, 80}, // 247
        {70, 97, 0, 55}, // 248
        {60, 97, 0, 50}, // 249
        {60, 103, 0, 80}, // 250
        {58, 101, 0, 80}, // 251
        {53, 102, 0, 80}, // 252
        {53, 19, 0, 80}, // 253
        {60, 255, 0, 127}, // 254
        {60, 255, 0, 127}, // 255
};

//97E44
u8 bank_to_use[256] = {
        0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
};

u8 voice_is_working[32] = {0}; //97F44
i16 Volume_Snd = 127; //97F64

i16 was_in_freeze = 0; //97F76
u8 flagCDPlay; //97F78
u8 phaseCd; //97F79

//97F7E
i16 atak[34] = {
        1, -1, 0, 0, 1, 1, -3, 60, 0, 2, 2, -2, 0, 0, 1, 3, -4,
        30, 0, 1, 2, -4, 30, 0, 1, 2, -3, 42, 0, 1, 1, -4, 80, 0
};

//98084
u8 Type_Scroll[60] = {
        0, 0, 1, 2, 2, 0, 1, 2, 2, 2,
        0, 0, 2, 0, 0, 0, 2, 2, 2, 2,
        0, 0, 1, 0, 0, 1, 2, 0, 2, 2,
        0, 0, 2, 2, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 2, 0, 2, 2, 2, 2,
        0, 0, 2, 0, 0, 2, 2, 2, 2, 2,
};

//980C0
u8 Tab_NbBande[60] = {
        1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1,
        1, 1, 1, 1, 1, 1, 2, 10, 1, 2, 1, 1, 1, 1, 1, 1, 1,
        2, 4, 5, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1,
        2, 3, 1, 1, 1, 1, 1,
};

// NB. this is originally a static array: def_bande_t Def_Bande[18000];
// Initialization in init_def_bande()
def_bande_t* Def_Bande; //980FC

//BB37C
u8 Tab_NbSprite[60] = {
        0, 0, 0, 0, 0, 14, 9, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0,
        0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
        3, 10, 12, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0,
        0, 6, 13, 0, 0, 0, 0, 0,
};

#define REPEAT_50(X) \
X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X
#define REPEAT_40(X) \
X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X
#define REPEAT_100(X) REPEAT_50(X), REPEAT_50(X)
#define REPEAT_171(X) REPEAT_100(X), REPEAT_50(X), X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,X,X,X,X,X, X
#define REPEAT_165(X) REPEAT_100(X), REPEAT_50(X), X,X,X,X,X,X,X,X,X,X, X,X,X,X,X
#define REPEAT_56(X) REPEAT_50(X), X,X,X,X,X,X
#define REPEAT_158(X) REPEAT_100(X), REPEAT_50(X), X,X,X,X,X,X,X,X
#define REPEAT_148(X) REPEAT_100(X), REPEAT_40(X), X,X,X,X,X,X,X,X
#define REPEAT_149(X) REPEAT_100(X), REPEAT_40(X), X,X,X,X,X,X,X,X,X

//BB3B8
def_sprite_t Def_Sprite[1200] = {
        REPEAT_100({0}),
        {0, 70, 1}, {53, 64, 1}, {89, 36, 1}, {121, 49, 1}, {149, 71, 1}, {195, 59, 1}, {239, 24, 1}, {279, 0, 1}, //100-107
        {328, 71, 1}, {299, 175, 2}, {175, 170, 2}, {38, 174, 2}, {262, 185, 3}, {364, 148, 3}, //108-113
        {0}, {0}, {0}, {0}, {0}, {0}, //114-119
        {64, 0, 1}, {64, 120, 1}, {192, 0, 3}, {192, 120, 3}, {0}, {0, 153, 0}, {296, 0, 0}, {296, 53, 0}, {296, 141, 0}, //120-128
        REPEAT_171({0}), //129-299
        {170, 97, 1}, {234, 75, 1}, {298, 136, 1}, {362, 136, 1}, {42, 121, 1}, {106, 117, 1}, {170, 227, 2}, {216, 229, 2}, //300-307
        {306, 225, 2}, {370, 107, 2}, {50, 127, 2}, {82, 205, 2}, {138, 221, 2}, {360, 225, 3}, {40, 271, 3}, //308-314
        REPEAT_165({0}), //315-479
        {63, 143, 1}, {112, 143, 1}, {255, 143, 1}, {304, 143, 1}, //480-483
        REPEAT_56({0}), //484-539
        {6, 66, 1}, {264, 57, 1}, //540-541
        REPEAT_158({0}), //542-699
        {138, 141, 1}, {202, 147, 1}, {266, 151, 1}, //700-702
        {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //703-719
        {0, 123, 1}, {64, 137, 1}, {128, 131, 1}, {192, 141, 1}, {256, 161, 1}, {0, 185, 2}, {64, 166, 2}, {128, 164, 2}, //720-727
        {192, 164, 2}, {256, 176, 2}, //728-729
        {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //730-739
        {0, 147, 1}, {64, 173, 1}, {128, 132, 1}, {192, 112, 1}, {256, 114, 1}, {320, 127, 1}, {0, 222, 4}, {64, 236, 4}, //740-747
        {128, 219, 4}, {192, 227, 4}, {256, 233, 4}, {320, 231, 4}, //748-751
        REPEAT_148({0}), //752-899
        {0, 85, 1}, {64, 82, 1}, {147, 44, 1}, {211, 44, 1}, {332, 96, 1}, {0, 216, 2}, {64, 208, 2}, {128, 215, 2}, //900-907
        {192, 236, 2}, {256, 227, 2}, {320, 225, 2}, //908-910
        REPEAT_149({0}), //911-1059
        {67, 185, 1}, {115, 162, 1}, {211, 166, 1}, {259, 117, 1}, {323, 111, 1}, {345, 171, 1}, //1060-1065
        {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //1066-1079
        {25, 105, 1}, {162, 182, 1}, {214, 168, 1}, {330, 173, 1}, {0, 266, 2}, {32, 227, 2}, {65, 131, 2}, {84, 251, 2}, //1080-1087
        {164, 234, 2}, {216, 256, 2}, {239, 173, 2}, {284, 272, 2}, {344, 254, 2}, //1088-1092
        REPEAT_100({0}), {0}, {0}, {0}, {0}, {0}, {0}, {0}, //1093-1199
};

big_map_t BIG_MAP; //C21D8
u8* Scroll_Masque[20]; //C21F8
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

options_jeu_t options_jeu_save; //C2F58
options_jeu_t options_jeu; //C2F78

u32 gros_patai_src[1200]; //C2FA8
i32 block_add[1200]; //C4268
u8* PLANVIGBIT[8]; //C5528
i32 planVigbit_length[8]; //C5548
rgb_palette_t rvb_Vig[8]; //C5568
i32 planVigbit_nb_bytes[8]; //C6D68
rgb_palette_t rvb_save; //C6D88
rgb_palette_t rvb_pres; //C7088
u16 rvb_fade[256 * 3]; //C7388
rgb_palette_t current_rvb; //C7988
rgb_palette_t rvb_plan3; //C7C88
rgb_palette_t rvb[3]; //C7F88

i16 notbut[4]; //C97F0
i16 SNSEQ_list[256]; //C97F8
u8 grp_stars[120]; //C99F8
i16 invpy0[8]; //C9A70
pix_gerbe_t pix_gerbe[8]; //C9A80 // 2568 == 321*8
i16 invpx0[8]; //CAE90
i16 invpy200[8]; //CAEA0
i16 nb_floc[8]; //CAEB0
i16 invpx320[8]; //CAEC0
i16 SNSEQ_len[64]; //CAED0
record_t record; //CAF50
i16 tot_nb_flocs[8]; //CAF68
i16 floc_ind[8]; //CAF78
u8 wi_save_zone[24]; //CAF88
u8 save_zone[2592]; //CAFA0
i16 allowed_time[192]; //CB9C0
u8 bonus_perfect[24]; //CBB40
display_item_t info_dialog_text; //CBB58 //?
display_item_t msg_to_display; //CBC30
display_item_t menu_to_display[9]; //CBD08
display_item_t text_to_display[11]; //CC4A0
level_obj_t level_alw; //CCDE8
level_t level; //CCDF0
level_obj_t level_obj; //CCDFC
u8 ecroule_rubis_list[8]; //CCE00
u8 saveobj[64+3]; //CCE08 //NOTE: originally 64 bytes - I added some safety bytes because read/set2bits might access out of bounds
eta_t** loaded_eta[100]; //CCE48
i32 MapTimePause; //CCFD8
char CheminSauvegarde[100]; //CCFDC
i32 Port; //CD040
i32 DeviceID; //CD044
i32 Irq; //CD048
i32 Param; //CD04C
i32 Dma; //CD050
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
void_func_t* drawflocon6NormalETX; //CD0B8
void_func_t* drawflocon7NormalETX; //CD0BC
i32 JumellePosY; //CD0C0
i32 JumellePosX; //CD0C4
i32* JumelleZoomDef; //CD0C8
i32 LongueurStageNameWorld; //CD0CC
i32 LongueurStageNameLevel; //CD0D0
void_func_t* drawpluie7NormalETX; //CD0D4
void_func_t* drawpluie4NormalETX; //CD0D8
i32 JumelleYMin; //CD0DC
void_func_t* drawpluie5NormalETX; //CD0E0
i32 JumelleXMin; //CD0E4
void_func_t* drawflocon4NormalETX; //CD0E8
i32 RayonJumelle; //CD0EC
void_func_t* drawpluie6NormalETX; //CD0F0
draw_func_t* DrawSpriteNoClipNormalEtX; //CD0F4
draw_func_t* DrawSpriteColorNormalEtX; //CD0F8
i32 BufferNormalDeplt; //CD0FC
void_func_t* fplotNormalETX; //CD100
void_func_t* drawflocon5NormalETX; //CD104
void_func_t* drawflocon2NormalETX; //CD108
draw_func_t* DrawSpriteNormalEtX; //CD10C
void_func_t* drawflocon1NormalETX; //CD110
draw_func_t* DrawSpriteFlipNoClipNormalEtX; //CD114
draw_func_t* DrawSpriteFlipNormalEtX; //CD118
draw_func_t* DrawSpriteColorFlipNormalEtX; //CD11C
swap_func_t* Swap_And_Test_Joystick; //CD120
void (*Copy_Plan0_To_Buf)(u8* source, u8* dest, i32 width, i32 height, i32 a5); //CD124
void* DrawBufferNormalIni; //CD128
void_func_t* drawflocon3NormalETX; //CD12C
u8* EffetBufferNormal; //CD130
i32 TempsDemo; //CD134
u8* DrawBufferNormal; //CD138
void (*anim_func)(void); //CD13C

char save_ray_copy[4]; //CD794
char save_ray[3][4]; //CD798
options_t options; //CD7A4
u8* draw_buffer; //CD7B8
u32 new_sprite_deform; //CD7BC
u8* PLANTMPBIT; //CD7C0
u8* blocks_code; //CD7C4
i32 plantmp_length; //CD7C8
i32 plantmp_nb_bytes; //CD7CC
u8* display_buffer; //CD7D0
x_texture_t* gros_patai_block; //CD7D4
char world_filename[20]; //CD7D8
i32 plan2_width; //CD7EC
i32 plan3bit_nb_bytes; //CD7F0
u32 dword_CD7F4; //CD7F4
i32 plan2bit_length; //CD7F8
i32 plan2bit_nb_bytes; //CD7FC
u8* PLAN0; //CD800
u8* PLAN2; //CD804
i32 plan0bit_length; //CD808
char level_filename[20]; //CD80C
i32 plan3bit_length; //CD820
u8* PLAN0BIT; //CD824
u8* PLAN2BIT; //CD828
u8* PLAN3BIT; //CD82C
u8* MAP_BLOCKS; //CD830
u32 PLAN1_BUFFER_START_POINTER; //CD834
u8* PLAN1_BUFFER; //CD838
rgb_palette_t* rvb_special; //CD83C

u32 TailleMainMemLevel; //CD844
u32 TailleMainMemSprite; //CD848
mem_t* main_mem_level; //CD84C
i32 cdTimeStart; //CD858
i32 cdTime; //CD85C
mem_t* main_mem_sprite; //CD860

u32 TailleMainMemTmp; //CD86C
i32 cdTimeEnd; //CD870
mem_t* main_mem_fix; //CD874

mem_t* main_mem_world; //CD880
mem_t* temp_mem_buf; //CD88C
u32 TailleMainMemFix; //CD898
u32 TailleMainMemWorld; //CD89C
mem_t* main_mem_tmp; //CD8A0
i32 nb_wiz_save; //CD8AC
loadinforay_t LoadInfoRay[3]; //CD8B0
flocon_t* flocon_tab; //CD8BC
i32 map_time; //CD8C0
i32 left_time; //CD8C4
obj_t* lidol_source_obj; //CD8C8
obj_t mapobj[25]; //CD8CC
i32 bossSpeedFactor; //CE5B0
i32 alternateBossSpeedFactor; //CE5B4
i16 pos_stack[10]; //CE5B8
i16 cymbal_obj_id[2]; //CE5CC
i16 scroll_obj_id[50]; //CE5D0
obj_t* ScrollDiffSprites; //CE634
map_data_t mp; //CE638
obj_t* alpha_numbers; //CE644
u32 dword_CE648; //CE648
obj_t ray; //CE650
obj_t* star_ray_dev; //CE6D4
obj_t* star_ray_der; //CE6D8
obj_t* poing_obj; //CE6DC
obj_t* alpha; //CE6E0
u8* alpha_image_atlas; //CE6E4
i16 alpha_sprite_count; //CE6E8
obj_t div_obj; //CE6EC
poing_t poing; //CE770
obj_t bigray; //CE784
obj_t raylittle; //CE808
save_state_t save1; //CE88C
save_state_t save2; //CEAE8
obj_t clockobj; //CED44
obj_t* alpha2; //CEDC8
u8* alpha2_image_atlas; //CEDCC
i16 alpha2_sprite_count; //CEDD0
obj_t rms; //CEDD4
i16* link_init; //CEE58
obj_t raytmp; //CEE5C
u8 skops_lave_obj[20]; //CEEE0
i32 pixels_enfonce; //CEEF4

i8 Plan0NumPcx[10]; //CEF47
status_bar_t status_bar; //CEF52
active_objects_t actobj; //CEF5C

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
rayevts_t SauveRayEvts; //CF818
rayevts_t RayEvts; //CF81A
u16 ZDCPTR; //CF81C
finbosslevel_t finBosslevel; //CF81E
i16 floorLine; //CF820
i16 png_or_fee_id; //CF822
i16 loop_nb_trames; //CF824
i16 loop_timing; //CF826
i16 VENT_Y; //CF828
i16 VENT_X; //CF82A
i16 id_Cling_1up; //CF82C
i16 num_level_choice; //CF82E
i16 id_Cling_Old; //CF830
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
i16 id_Cling_Pow; //CF846
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
i16 decalage_en_cours; //CF866
i16 ray_mode; //CF868
i16 remoteRayXToReach; //CF86A
i16 scroll_start_x; //CF86E
i16 ray_wind_force; //CF870
i16 scroll_end_x; //CF872
i16 weather_wind; //CF874
i16 ray_zdc_y; //CF876
i16 mst_scroll_obj_id; //CF878
u16 NumScrollObj; //CF87A
i16 num_level; //CF87C
i16 sbar_obj_id; //CF87E
i16 new_level; //CF880
i16 eau_obj_id; //CF882
i16 ymapmax; //CF884
i16 reduced_rayman_id; //CF886
i16 fee_obj_id; //CF888
i16 poing_obj_id; //CF88A
i16 num_world; // 0xCF88C
i16 pierreAcorde_obj_id; // CF88E
i16 xmapmax; //CF890
i16 new_world; // 0xCF892
i16 rayman_obj_id; //CF894
i16 Mus_obj_id; //CF896
i16 helico_time; //CF898
u16 saveRMjumpY; //CF89A
u16 saveRMjumpX; //CF89C
i16 xmap; //CF89E
i16 departlevel; //CF8A0
i16 ray_Y_main_pos_init; //CF8A2
i16 jump_time; //CF8A4
i16 ymap; //CF8A6
i16 special_ray_mov_win_x_left; //CF8A8
i16 ray_X_main_pos_init; //CF8AA
i16 ymap_old; //CF8AC
i16 special_ray_mov_win_x_right; //CF8AE
i16 xmap_old; //CF8B0
i16 h_scroll_speed; //CF8B2
i16 v_scroll_speed; //CF8B4
i16 xmapsave; //CF8B6
i16 ymapsave; //CF8B8
i16 scroll_y; //CF8BA
i16 black_ray_obj_id; //CF8BC
u16 black_fist_obj_id; //CF8BE
u16 final_pass_n; //CF8C0
u16 final_pass_a; //CF8C2
u16 ray_old_main_etat; //CF8C4
u16 final_pass_f; //CF8C6
i16 moskitomama_gauche_obj_id; //CF8C8
u16 final_pass_x; //CF8CA
i16 rideau_obj_id; //CF8CC
i16 corde_dark_obj_id; //CF8CE
i16 stosko_obj_id; //CF8D0
i16 scroll_x; //CF8D2
u16 ray_old_sub_etat; //CF8D4
i16 moskitosaxo_obj_id; //CF8D6
i16 moskitomama_droite_obj_id; //CF8D8
u16 nb_fade; //CF8DA
u16 sko_last_action; //CF8DC
u16 sko_rayon_on; //CF8DE
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
u8 saved_argv[255]; //CF8F6
u8 horloge[25]; //CF9F5
u8 HVersionWindows; //CFA0E
u8 NVersionWindows; //CFA0F
u8 MessageProgram; //CFA10
u8 DosDetect; //CFA11
u8 WindowsLance; //CFA12
u8 SonLimite; //CFA13
u8 byte_CFA14; //CFA14
u8 need_check_cheats; //CFA15
u8 HVerDos; //CFA16
u8 CarteSonAutorisee; //CFA17
u8 ToutSpriteAutorise; //CFA18 // is background clearing needed
u8 MenuCredits; //CFA19
u8 FondAutorise; //CFA1A // background available
u8 LVerDos; //CFA1B
u8 Mode_Pad; //CFA1C // see also CFA23?
u8 something_with_cfg_25_default_00_joy; //CFA1D (?)
u8 Port_Pad; //CFA1E
u8 ScrollDiffOn; //CFA1F
u8 P486; //CFA20
u8 type_fnd; //CFA21
u8 no_fnd; //CFA22
u8 is_joypad_mode; //CFA23 //? may be Mode_Pad instead
u8 type_fndscrX; //CFA24
u8 NumCard; //CFA25
u8 TheProcesseur; //CFA26
u8 drm_byte_CFA27; //CFA27
u8 VideoBiosCheckSum; //CFA28
u8 byte_CFA29; //CFA29
u8 byte_CFA2A; //CFA2A
u8 BiosCheckSum; //CFA2B
u8 JeuCracker; //CFA2C
u8 PositionStageNameCalcule; //CFA2D
u8 ModeAutoJumelle; //CFA2E
u8 ChangeJumelleSizeOK; //CFA2F
u8 JumelleZoomActif; //CFA30
u8 ParamZoomChange; //CFA31
u8 JumelleEffetAutorise; //CFA32 // zoom effect available
u8 ModeNormalAutorise; //CFA33    // normal mode available
u8 first_credit; //CFA34
u8 language; //CFA35
u8 ModeDemo; //CFA36
u8 menuEtape; //CFA37
u8 MENU_SUITE; //CFA38
u8 MENU_RETURN; //CFA39
u8 last_credit; //CFA3A
u8 nb_continue; //CFA3B
u8 fin_dark; //CFA3C
u8 NBRE_SAVE; //CFA3D
u8 byte_CFA3E; //CFA3E
u8 nouvelle_partie; //CFA3F
u8 PROC_EXIT; //CFA40
u8 First_Hist; //CFA41
u8 First_Menu; //CFA42
u8 dontdoit; //CFA43
u8 Vignet_To_Display; //CFA44
u8 chemin_percent; //CFA45
u8 You_Win; //CFA46 (?)
u8 display_Vignet; //CFA47
u8 colour; //CFA48
u8 rayOnMsWasThere; //CFA49
u8 freeze; //CFA4A (?)
u8 need_timer; //CFA4B (?)
u8 VGA_FREQ; //CFA4C
u8 save_current_pal; //CFA4D
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
u8 old_txt_fee; //CFA59
u8 dir_on_wldmap; //CFA5A
u8 Nb_total_cages; //CFA5B
u8 fin_continue; //CFA5C
u8 display_txt_fee; //CFA5D
u8 ray_inertie; //CFA5E
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
i8 saveBossEncounter; //CFA69
u8 bossReachingTimer; //CFA6A
u8 boss_safe_timer; //CFA6B
u8 currentBossActionIsOver; //CFA6C
i8 laserSourceSprNumInAnim; //CFA6D
u8 bossEncounter; //CFA6E
u8 currentBossAction; //CFA6F
u8 gerbe; //CFA70
u8 first_boss_meet; //CFA71
u8 ray_last_ground_btyp; //CFA72
u8 RAY_MODE_SPEED; //CFA73
u8 fin_de_rayman; //CFA74
u8 ray_Suphelico_bis; //CFA75
u8 fin_boss; //CFA76
u8 ray_in_fee_zone; //CFA77
u8 boss_mort; //CFA78
u8 no_ray_landing_smoke; //CFA79
u8 fin_du_jeu; //CFA7A
u8 ray_se_noie; //CFA7B
u8 hand_btyp; //CFA7C
u8 button_released; //CFA7D
u8 dead_time; //CFA7E
u8 hand_btypg; //CFA7F
u8 hand_btypd; //CFA80
u8 nb_cymbal_in_map; //CFA81
u8 current_pal_id; //CFA82
u8 last_plan1_palette; //CFA83
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
u8 sko_enfonce_enable; //CFA8F
u8 WaitForAnim; //CFA90
u8 ecroule_plat_index; //CFA91
u8 byte_CFA92; //CFA92
u8 FinAnim; //CFA93
u8 Phase; // (?) CFA94
u8 WaitForFinAtan; //CFA95
u8 skops_nb_lave; //CFA96
u8 NbTramesEcrase; //CFA97
u8 RaymanDansUneMapDuJeu; //CFA98
u8 life_becoz_wiz; //CFA99
u8 nb_fix_eta; //CFA9A
u8 nb_loaded_eta; //CFA9B
u8 position_ray; //CFA9C
i32 CompteurTrameAudio; //CFAA0

i32 SpeedYLine[10]; //CFAC4
i32 PosXLine[10]; //CFAEC
i32 PosYLine[320]; //CFB14
char bonus_time_display_text[20]; //D0014

i16 p1[21000]; //D002C
u8 byte_DA43C[128*128]; //DA43C
rgb_palette_t MenuPalette; //DE43C
u8 plasma_palette_color_index; //DE73C

u16 word_DE73E; //DE73E
u16 word_DE740; //DE740
u16 word_DE742; //DE742
u16 word_DE744; //DE744
u16 word_DE746; //DE746
u16 word_DE748; //DE748
u16 word_DE74A; //DE74A
u16 word_DE74C; //DE74C

i16 old_num_world_choice; //DE80C
i16 old_num_level_choice; //DE80E
i16 CBoffsetXMin; //DE810
i16 CBoffsetYMin; //DE812
i16 CBoffsetYMax; //DE814
i16 CBoffsetXMax; //DE816
i16 LIMITE_GAUCHE; //DE818
i16 XOFFSETCBRIGHT; //DE81A
i16 XOFFSETRAY; //DE81C
i16 LIMITE_DROITE; //DE81E
i16 YOFFSETCB; //DE820
i16 XOFFSETCBLEFT; //DE822
u8 nb_collis; //DE824

i16 word_DE8BC; //DE8BC
u8 byte_DEEFB; //DEEFB

u8 sav_checksum; //DF70C
vitraux_info_t VitrauxInfos[5];
i16 dark_rayon_dy; //DF742
i16 dark_rayon_dx; //DF744
i16 corde_x; //DF746
i16 corde_y_haut; //DF748
i16 corde_y_bas; //DF74A
u8 byte_DF74C; //DF74C

i16 level_select; //DF750

u8 ButtonReleasedSav2; //DF756
u8 ButtonReleasedSav1; //DF757

u8* cheat_code_programmer_mode; //DF760
u8* cheat_code_win_map_alt; //DF764
u8* cheat_code_10_wiz_alt; //DF768
u8* cheat_code_all_powers_alt; //DF76C
u8* cheat_code_breakout_alt; //DF770
u8* cheat_code_golden_fist_alt; //DF774
u8* cheat_code_10_wiz; //DF778
u8* cheat_code_win_map; //DF77C
u8* cheat_code_unlock_all_levels_alt; //DF780
u8* cheat_code_99_lives_alt; //DF784
u8* cheat_code_5hp_alt; //DF788
u8* cheat_code_golden_fist; //DF78C
u8* cheat_code_unlock_all_levels; //DF790
u8* cheat_code_breakout; //DF794
u8* cheat_code_all_powers; //DF798
u8* cheat_code_99_lives; //DF79C
u8* cheat_code_free_movement; //DF7A0
u8* cheat_code_5hp; //DF7A4
u8* cheat_code_freq80; //DF7A8
u8* cheat_code_lens; //DF7AC
u8* cheat_code_breakout_before_victory; //DF7B0
u8* cheat_code_level_select; //DF7B4
i32 freq_from_cheat_code; //DF7B8
u8* cheat_code_freq100; //DF7BC

bnk_header_t bnkHeaderFixe[128]; //DFAF0
bnk_header_t bnkHeaderWorld[128]; //E02F0
i32 current_port; //E0AF0
i32 current_irq; //E0AF4
i32 current_dma; //E0AF8
i32 current_param; //E0AFC
u8* TchachPerduPtr; //E0B00
i32 TchachPerduSize; //E0B04
char current_device_name[100]; //E0B0A
i32 current_device_id; //E0B6E

obj_t* dark_obj; //E0B84
char txt_dark2[100]; //E0B88
u32 dword_E0BEC; //E0BEC
u32 dword_E0BF0; //E0BF0
i16 PosArYToon2; //E0BF4
i16 PosArXToon2; //E0BF6
i16 PosArYToon1; //E0BF8
i16 PosArXToon1; //E0BFA
i16 flamme_droite_id; //E0BFC
i16 XText; //E0BFE
i16 dark2_rayon_dx_1; //E0C00
i16 dark2_rayon_dx_2; //E0C02
i16 YText; //E0C04
i16 dark2_rayon_dy_1; //E0C06
i16 dark2_rayon_dy_2; //E0C08
i16 flamme_gauche_id; //E0C0A
i16 phase_dark2; //E0C0C
i16 dark2_rayon_2_pos_x; //E0C0E
i16 dark2_rayon_1_pos_x; //E0C10
i16 VitesseYText; //E0C12
i16 temps_text; //E0C14
i16 dark2_rayon_2_pos_y; //E0C16
i16 dark2_rayon_1_pos_y; //E0C18
i16 YPosBBF2D; //E0C1C
i16 YPosBBF2G; //E0C1E
i16 NiveauSol; //E0C20
u8 ToonJustGivePoing; //E0C22
u8 sinus_actif; //E0C23
u8 num_dark2_phrase; //E0C24
u8 sens_sinus_1; //E0C25
u8 sens_sinus_2; //E0C26
u8 flammes_actives; //E0C27
u8 BBF2DEsk; //E0C28
u8 BBF2GEsk; //E0C29
u8 Touche_Enfoncee[128]; //E0C30
void_func_t* Read_Input_Func; //E0CB0
void* saved_keyboard_interrupt_handler; //E0CB4

i16 joy_rec_up; //E0CBC
i16 bux11; //E0CBE
i16 joy_rec_right; //E0CC0
i16 bux10; //E0CC2
i16 bux01; //E0CC4
i16 bux00; //E0CC6
i16 joy_rec_down; //E0CC8
i16 auto_joy_left; //E0CCA (?)
i16 auto_joy_up; //E0CCC (?)
i16 joy_rec_left; //E0CCE
i16 word_E0CD0; //E0CD0
i16 auto_joy_down; //E0CD2 (?)
i16 word_E0CD4; //E0CD4
i16 auto_joy_right; //E0CD6 (?)
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
u8 sp_y; //E40A9

rgb_palette_t menu_rvb; //E43AC (?)
rgb_palette_t rvb_options_in_game; //E46AC (?)
rgb_palette_t rvb_menu_save; //E49AC (?)
display_item_t* dialog_display_item_ptr; // E4CAC (?)
const char** key_descriptions; //E4CB0
void (*pINIT_SCROLL)(void); //E4CB4
void (*pINIT_AFFICHE_SCREEN)(void); //E4CB8
void (*pINIT_SCREEN)(void); //E4CBC
void (*sEND_SCREEN)(void); //E4CC0
void (*pDO_COMMANDE)(void); //E4CC4
void (*pAFFICHE_SCREEN)(void); //E4CC8
void (*pEND_SCREEN)(void); //E4CCC
void (*pLOAD_SCREEN)(void); //E4CD0
void (*pCOMMANDE_BOX)(void); //E4CD4

i16 hFondu; //E4CE2
i16 xFondu; //E4CE4
i16 yFondu; //E4CE6
i16 wFondu; //E4CE8
i16 EtapeFondu; //E4CEA
i16 word_E4CEC; //E4CEC
i16 word_E4CEE; //E4CEE
i16 word_E4CF0; //E4CF0
i16 VideoSpeed; //E4CF2
i16 word_E4CF4; //E4CF4
u8 but_options[4]; //E4CF6
u8 InOut; //E4CFB
u8 byte_E4CFC; //E4CFC
u8 byte_E4CFD; //E4CFD
u8 byte_E4CFE; //E4CFE

i16 droppedBombIds[64]; //E4D00
u8 lastDroppedBombIdInSequence[8]; //E4D80
u32 dword_E4D88; //E4D88
i16 mereDenis_weapon_id; //E4D8C
i16 lastDroppedBombXCenterPos; //E4D8E
i16 machine_obj_id; //E4D90
i16 circle_x; //E4D92
i16 circle_y; //E4D94
i16 circle_index; //E4D96
u8 byte_E4D98; //E4D98
u8 currentLaserSize; //E4D99
u8 currentPhaseHitCounter; //E4D9A
u8 stepsForward; //E4D9B
u8 currentBombSequence; //E4D9C
u8 curAct; //E4DA0
u8 fistAvoided; //E4DA1
u8 mstMustLeaveScreenToProceed; //E4DA2
i16 ot; //E4DA4
u8 oldPrio; //E4DA6
u32 dword_E4DA8; //E4DA8

i16 cou_tempo; //E4E10
i16 pma_tempo; //E4E12
i16 pma_touched; //E4E14
i16 pma_nb_couteau; //E4E15
i16 pma_phase; //E4E16
i16 pma_attaque; //E4E17
i16 cou_place; //E4E18
i16 pma_groupe; //E4E19
i16 pma_type_attaque; //E4E1A

u8* anim_sequence; //E4E78

i32 RandomIndex; //E548C
i16 stk_obj[20]; //E5490
i16 stk_snd[20]; //E54B8

voice_t voice_table[32]; //E55D0

i16 indice_ray_wait; //E5750
i16 indice_trz_wait; //E5754
i16 pt_pile_snd; //E5756
i16 indice_snd_wiz; //E575A

i16 IndexSerie; //E577A
i16 IndexAtak; //E577C
i16 SaxMarche; //E577E
u8 NextNote; //E5780
bande_t Bande[300]; //E5784
def_sprite_t Sprite[20]; //E73A4
i16 Val_Div_Scroll_Y; //E7444
i16 NbSprite; //E7446
i16 Val_Add_Scroll_Y; //E7448
i16 NbBande; //E744A
i16 Num_Fond; //E744C
i32 skops_screen_tremble; //E7450
i16 PosLave_Y; //E7454

i32 OffsetSpriteWorld; //E7470
i32 OffsetSpriteFixe; //E7474
