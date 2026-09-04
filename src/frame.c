
/*
 * The PC movie files are Autodesk FLC streams with a Rayman-specific, truncated
 * 12-byte header: size, AF12 magic, frame count, width, and height.  The normal
 * 128-byte FLC header (including its speed field) is absent.  The original PC
 * reader supplies a 30 ms frame delay for this variant.  intro.dat and
 * conclu.dat use only the codecs implemented below.
 *
 * The original routines at 3A1C0-3B274 combine DOS file I/O, VGA/PIT control,
 * CD control, surface primitives, and FLC decoding.  Rayverse keeps the same
 * supplied stream and visible behavior, but deliberately uses its portable
 * framebuffer, palette, input, and audio abstractions instead of reproducing
 * the DOS hardware layer.
 */

enum {
    MOVIE_FILE_HEADER_SIZE = 12,
    MOVIE_FRAME_HEADER_SIZE = 16,
    MOVIE_TRUNCATED_HEADER_DELAY_MS = 30,
    MOVIE_EFFECTIVE_FRAME_DELAY_MS = MOVIE_TRUNCATED_HEADER_DELAY_MS * 2,
    MOVIE_MAX_FRAME_SIZE = 16 * 1024 * 1024,

    FLC_COLOR_256 = 4,
    FLC_DELTA_SS2 = 7,
    FLI_COLOR_64 = 11,
    FLI_DELTA_LC = 12,
    FLI_BLACK = 13,
    FLI_BYTE_RUN = 15,
    FLI_COPY = 16,
    FLI_POSTAGE_STAMP = 18,
};

typedef struct movie_reader_t {
    const u8* data;
    size_t size;
    size_t pos;
} movie_reader_t;

typedef struct movie_decoder_t {
    FILE* file;
    u16 frame_count;
    u16 width;
    u16 height;
    s16 screen_x;
    s16 screen_y;
    u8* pixels;
    u8* frame_data;
    size_t frame_data_capacity;
    u32 delay_tick_remainder;
    rgb_palette_t palette;
} movie_decoder_t;

static u16 movie_read_le16(const u8* data) {
    return (u16)(data[0] | ((u16)data[1] << 8));
}

static u32 movie_read_le32(const u8* data) {
    return (u32)data[0] |
           ((u32)data[1] << 8) |
           ((u32)data[2] << 16) |
           ((u32)data[3] << 24);
}

static bool movie_reader_u8(movie_reader_t* reader, u8* value) {
    if (reader->pos >= reader->size) {
        return false;
    }
    *value = reader->data[reader->pos++];
    return true;
}

static bool movie_reader_u16(movie_reader_t* reader, u16* value) {
    if (reader->size - reader->pos < 2) {
        return false;
    }
    *value = movie_read_le16(reader->data + reader->pos);
    reader->pos += 2;
    return true;
}

static bool movie_reader_bytes(movie_reader_t* reader, const u8** data, size_t count) {
    if (count > reader->size - reader->pos) {
        return false;
    }
    *data = reader->data + reader->pos;
    reader->pos += count;
    return true;
}

static bool movie_decode_palette(movie_decoder_t* movie, const u8* data, size_t size, bool six_bit) {
    movie_reader_t reader = {data, size, 0};
    u16 packet_count;
    u32 color_index = 0;
    if (!movie_reader_u16(&reader, &packet_count)) {
        return false;
    }

    for (u32 packet = 0; packet < packet_count; ++packet) {
        u8 skip;
        u8 encoded_count;
        if (!movie_reader_u8(&reader, &skip) || !movie_reader_u8(&reader, &encoded_count)) {
            return false;
        }

        color_index += skip;
        u32 color_count = encoded_count ? encoded_count : 256;
        if (color_index + color_count > 256 || color_count * 3 > reader.size - reader.pos) {
            return false;
        }

        for (u32 i = 0; i < color_count; ++i) {
            u8 r;
            u8 g;
            u8 b;
            movie_reader_u8(&reader, &r);
            movie_reader_u8(&reader, &g);
            movie_reader_u8(&reader, &b);

            /* PC VGA palettes are 6-bit, whereas Rayverse's renderer stores
               8-bit components.  FLC type 4 is already 8-bit; FLI type 11 is
               the older 6-bit representation. */
            if (six_bit) {
                r = (u8)MIN((u32)r << 2, 255u);
                g = (u8)MIN((u32)g << 2, 255u);
                b = (u8)MIN((u32)b << 2, 255u);
            }
            rgb_t* color = &movie->palette.colors[color_index++];
            color->r = r;
            color->g = g;
            color->b = b;
        }
    }
    return true;
}

static bool movie_decode_byte_run(movie_decoder_t* movie, const u8* data, size_t size) {
    movie_reader_t reader = {data, size, 0};
    for (u32 y = 0; y < movie->height; ++y) {
        u8 ignored_packet_count;
        if (!movie_reader_u8(&reader, &ignored_packet_count)) {
            return false;
        }

        u32 x = 0;
        while (x < movie->width) {
            u8 encoded_count;
            if (!movie_reader_u8(&reader, &encoded_count)) {
                return false;
            }
            s32 count = (s8)encoded_count;
            if (count == 0) {
                return false;
            }

            if (count > 0) {
                u8 value;
                if (!movie_reader_u8(&reader, &value) || x + (u32)count > movie->width) {
                    return false;
                }
                memset(movie->pixels + y * movie->width + x, value, (size_t)count);
                x += (u32)count;
            } else {
                size_t literal_count = (size_t)(-count);
                const u8* literal;
                if (x + literal_count > movie->width ||
                    !movie_reader_bytes(&reader, &literal, literal_count)) {
                    return false;
                }
                memcpy(movie->pixels + y * movie->width + x, literal, literal_count);
                x += (u32)literal_count;
            }
        }
    }
    return true;
}

static bool movie_decode_line_compressed(movie_decoder_t* movie, const u8* data, size_t size) {
    movie_reader_t reader = {data, size, 0};
    u16 skipped_lines;
    u16 line_count;
    if (!movie_reader_u16(&reader, &skipped_lines) || !movie_reader_u16(&reader, &line_count)) {
        return false;
    }

    u32 y = skipped_lines;
    for (u32 line = 0; line < line_count; ++line, ++y) {
        u8 packet_count;
        if (y >= movie->height || !movie_reader_u8(&reader, &packet_count)) {
            return false;
        }

        u32 x = 0;
        for (u32 packet = 0; packet < packet_count; ++packet) {
            u8 skip;
            u8 encoded_count;
            if (!movie_reader_u8(&reader, &skip) || !movie_reader_u8(&reader, &encoded_count)) {
                return false;
            }
            x += skip;
            if (x > movie->width) {
                return false;
            }

            s32 count = (s8)encoded_count;
            if (count >= 0) {
                size_t literal_count = (size_t)count;
                const u8* literal;
                if (x + literal_count > movie->width ||
                    !movie_reader_bytes(&reader, &literal, literal_count)) {
                    return false;
                }
                memcpy(movie->pixels + y * movie->width + x, literal, literal_count);
                x += (u32)literal_count;
            } else {
                size_t repeat_count = (size_t)(-count);
                u8 value;
                if (x + repeat_count > movie->width || !movie_reader_u8(&reader, &value)) {
                    return false;
                }
                memset(movie->pixels + y * movie->width + x, value, repeat_count);
                x += (u32)repeat_count;
            }
        }
    }
    return true;
}

static bool movie_decode_delta_ss2(movie_decoder_t* movie, const u8* data, size_t size) {
    movie_reader_t reader = {data, size, 0};
    u16 lines_left;
    if (!movie_reader_u16(&reader, &lines_left)) {
        return false;
    }

    u32 y = 0;
    while (lines_left > 0) {
        u16 opcode;
        if (!movie_reader_u16(&reader, &opcode)) {
            return false;
        }

        if (opcode & 0x8000) {
            if ((opcode & 0xC000) == 0xC000) {
                s32 skipped_lines = -(s16)opcode;
                if (skipped_lines <= 0 || y + (u32)skipped_lines > movie->height) {
                    return false;
                }
                y += (u32)skipped_lines;
                continue;
            }

            /* 10xxxxxx xxxxxxxx sets the final pixel of the current line. */
            if (y >= movie->height || movie->width == 0) {
                return false;
            }
            movie->pixels[y * movie->width + movie->width - 1] = (u8)opcode;
            continue;
        }

        if (y >= movie->height) {
            return false;
        }
        u32 x = 0;
        u32 packet_count = opcode;
        for (u32 packet = 0; packet < packet_count; ++packet) {
            u8 skip;
            u8 encoded_word_count;
            if (!movie_reader_u8(&reader, &skip) ||
                !movie_reader_u8(&reader, &encoded_word_count)) {
                return false;
            }
            x += skip;
            if (x > movie->width) {
                return false;
            }

            s32 word_count = (s8)encoded_word_count;
            if (word_count >= 0) {
                size_t literal_count = (size_t)word_count * 2;
                const u8* literal;
                if (x + literal_count > movie->width ||
                    !movie_reader_bytes(&reader, &literal, literal_count)) {
                    return false;
                }
                memcpy(movie->pixels + y * movie->width + x, literal, literal_count);
                x += (u32)literal_count;
            } else {
                size_t repeat_count = (size_t)(-word_count);
                const u8* pair;
                if (x + repeat_count * 2 > movie->width ||
                    !movie_reader_bytes(&reader, &pair, 2)) {
                    return false;
                }
                for (size_t i = 0; i < repeat_count; ++i) {
                    movie->pixels[y * movie->width + x++] = pair[0];
                    movie->pixels[y * movie->width + x++] = pair[1];
                }
            }
        }
        ++y;
        --lines_left;
    }
    return true;
}

static bool movie_decode_frame_chunks(movie_decoder_t* movie, const u8* data, size_t size, u16 chunk_count) {
    size_t offset = 0;
    for (u32 chunk_index = 0; chunk_index < chunk_count; ++chunk_index) {
        if (size - offset < 6) {
            return false;
        }

        u32 chunk_size = movie_read_le32(data + offset);
        u16 chunk_type = movie_read_le16(data + offset + 4);
        if (chunk_size < 6 || chunk_size > size - offset) {
            return false;
        }

        const u8* chunk_data = data + offset + 6;
        size_t chunk_data_size = chunk_size - 6;
        bool decoded = true;
        switch (chunk_type) {
            case FLC_COLOR_256:
                decoded = movie_decode_palette(movie, chunk_data, chunk_data_size, false);
                break;
            case FLI_COLOR_64:
                decoded = movie_decode_palette(movie, chunk_data, chunk_data_size, true);
                break;
            case FLC_DELTA_SS2:
                decoded = movie_decode_delta_ss2(movie, chunk_data, chunk_data_size);
                break;
            case FLI_DELTA_LC:
                decoded = movie_decode_line_compressed(movie, chunk_data, chunk_data_size);
                break;
            case FLI_BLACK:
                memset(movie->pixels, 0, (size_t)movie->width * movie->height);
                break;
            case FLI_BYTE_RUN:
                decoded = movie_decode_byte_run(movie, chunk_data, chunk_data_size);
                break;
            case FLI_COPY: {
                size_t pixel_count = (size_t)movie->width * movie->height;
                /* Rayman's type-16 chunks declare a size two bytes shorter
                   than their pixel data: the final word is stored as trailing
                   data in the enclosing frame.  MovieDecodeCopyChunk copies
                   width*height unconditionally in the PC executable, so
                   accept this precise final-chunk quirk while retaining the
                   enclosing-frame bounds check. */
                bool has_trailing_word =
                    chunk_index + 1 == chunk_count &&
                    chunk_data_size + 2 == pixel_count &&
                    size - (offset + 6) >= pixel_count;
                if (chunk_data_size < pixel_count && !has_trailing_word) {
                    decoded = false;
                } else {
                    memcpy(movie->pixels, chunk_data, pixel_count);
                }
            } break;
            case FLI_POSTAGE_STAMP:
                /* A decoder thumbnail, not part of the displayed frame. */
                break;
            default:
                /* The PC switch also ignores unknown ancillary chunks. */
                break;
        }
        if (!decoded) {
            return false;
        }
        offset += chunk_size;
    }
    return true;
}

static s32 movie_read_frame(movie_decoder_t* movie) {
    u8 header[MOVIE_FRAME_HEADER_SIZE];
    if (fread(header, 1, sizeof(header), movie->file) != sizeof(header)) {
        return -6;
    }

    u32 frame_size = movie_read_le32(header);
    u16 frame_magic = movie_read_le16(header + 4);
    u16 chunk_count = movie_read_le16(header + 6);
    if (frame_magic != 0xF1FA || frame_size < MOVIE_FRAME_HEADER_SIZE ||
        frame_size > MOVIE_MAX_FRAME_SIZE) {
        return -4;
    }

    size_t data_size = frame_size - MOVIE_FRAME_HEADER_SIZE;
    if (data_size > movie->frame_data_capacity) {
        u8* resized = (u8*)realloc(movie->frame_data, data_size);
        if (!resized && data_size != 0) {
            return -2;
        }
        movie->frame_data = resized;
        movie->frame_data_capacity = data_size;
    }
    if (data_size != 0 && fread(movie->frame_data, 1, data_size, movie->file) != data_size) {
        return -6;
    }
    return movie_decode_frame_chunks(movie, movie->frame_data, data_size, chunk_count) ? 0 : -4;
}

static bool movie_skip_requested(void) {
    readinput();
    return but0pressed() || but1pressed() || but2pressed() || but3pressed() || TOUCHE(SC_SPACE);
}

static bool movie_present_frame(movie_decoder_t* movie) {
    for (u32 y = 0; y < movie->height; ++y) {
        memcpy(DrawBufferNormal + (movie->screen_y + y) * SCREEN_WIDTH + movie->screen_x,
               movie->pixels + y * movie->width, movie->width);
    }
    SetPalette(&movie->palette, 0, 256);

    /* The shortened header is assigned 30 ms, but that is not the encoded
       frame period seen by the PC player: one timing path waits about 60 ms
       per frame, while the Windows fallback consumes two frames per roughly
       125 ms display interval.  Both advance the stream at about 16 fps.

       Keep the game's global 60 Hz rate intact and distribute the 60 ms delay
       over whole presentation ticks.  At 60 Hz this repeats 3/4/3/4/4 ticks,
       averaging exactly 60 ms instead of the previous, much too fast 33 ms. */
    s64 configured_hz = global_app_state.target_game_hz;
    u32 target_hz = configured_hz > 0 && configured_hz <= 1000 ? (u32)configured_hz : 60;
    movie->delay_tick_remainder += MOVIE_EFFECTIVE_FRAME_DELAY_MS * target_hz;
    u32 ticks = movie->delay_tick_remainder / 1000;
    movie->delay_tick_remainder %= 1000;
    if (ticks == 0) {
        ticks = 1;
    }
    for (u32 tick = 0; tick < ticks; ++tick) {
        advance_frame();
        if (movie_skip_requested()) {
            return true;
        }
    }
    return false;
}

static s32 movie_decode_and_present_frame(movie_decoder_t* movie, bool* skipped) {
    s32 result = movie_read_frame(movie);
    if (result < 0) {
        return result;
    }
    *skipped = movie_present_frame(movie);
    return 0;
}

static s32 movie_play_once(movie_decoder_t* movie) {
    for (u32 frame = 0; frame < movie->frame_count; ++frame) {
        if (movie_skip_requested()) {
            return 0;
        }
        bool skipped = false;
        s32 result = movie_decode_and_present_frame(movie, &skipped);
        if (result < 0 || skipped) {
            return result;
        }
    }
    return 0;
}

static s32 movie_play_loop(movie_decoder_t* movie) {
    bool skipped = false;
    s32 result = movie_decode_and_present_frame(movie, &skipped);
    if (result < 0 || skipped) {
        return result;
    }

    /* A looping FLC has an uncounted ring frame after its declared frames.
       The PC loop decodes the first frame once, then repeatedly decodes the
       remaining declared frames plus that ring frame. */
    long loop_offset = ftell(movie->file);
    if (loop_offset < 0) {
        return -6;
    }
    for (;;) {
        if (fseek(movie->file, loop_offset, SEEK_SET) != 0) {
            return -6;
        }
        for (u32 frame = 0; frame < movie->frame_count; ++frame) {
            if (movie_skip_requested()) {
                return 0;
            }
            result = movie_decode_and_present_frame(movie, &skipped);
            if (result < 0 || skipped) {
                return result;
            }
        }
    }
}

static FILE* movie_open_file(const char* path, const char* filename) {
    /* The DOS executable conditionally concatenates its installation path.
       Try that spelling first, then use Rayverse's normal data/ lookup. */
    if (path && path[0]) {
        char full_path[512];
        s32 length = snprintf(full_path, sizeof(full_path), "%s%s", path, filename);
        if (length > 0 && length < (s32)sizeof(full_path)) {
            FILE* file = fopen(full_path, "rb");
            if (file) {
                return file;
            }
        }
    }
    return open_data_file(filename, false);
}

static s32 movie_open_decoder(movie_decoder_t* movie, FILE* file) {
    u8 header[MOVIE_FILE_HEADER_SIZE];
    memset(movie, 0, sizeof(*movie));
    movie->file = file;
    if (fread(header, 1, sizeof(header), file) != sizeof(header)) {
        return -6;
    }

    u32 header_size = movie_read_le32(header);
    u16 magic = movie_read_le16(header + 4);
    movie->frame_count = movie_read_le16(header + 6);
    movie->width = movie_read_le16(header + 8);
    movie->height = movie_read_le16(header + 10);
    if (magic != 0xAF12 || header_size != MOVIE_FILE_HEADER_SIZE || movie->frame_count == 0) {
        return -11;
    }
    if (movie->width == 0 || movie->height == 0 ||
        movie->width > SCREEN_WIDTH || movie->height > SCREEN_HEIGHT) {
        return -3;
    }

    movie->screen_x = (s16)((SCREEN_WIDTH - movie->width) / 2);
    movie->screen_y = (s16)((SCREEN_HEIGHT - movie->height) / 2);
    movie->pixels = (u8*)calloc((size_t)movie->width, movie->height);
    return movie->pixels ? 0 : -2;
}

static void movie_close_decoder(movie_decoder_t* movie) {
    free(movie->frame_data);
    free(movie->pixels);
    movie->frame_data = NULL;
    movie->pixels = NULL;
}

//3B288
void MovieFadeOutPalette(void) {
    if (!global_game) {
        return;
    }

    rgb_palette_t faded_palette = global_game->draw_palette;
    for (u32 step = 0; step < 32; ++step) {
        /* The VGA routine subtracts two from each 6-bit component.  Rayverse
           uses 8-bit palettes, so the corresponding step is eight.  Color
           zero is intentionally preserved, matching the PC's 1..255 upload. */
        for (u32 color = 1; color < 256; ++color) {
            rgb_t* rgb = &faded_palette.colors[color];
            rgb->r = rgb->r > 8 ? (u8)(rgb->r - 8) : 0;
            rgb->g = rgb->g > 8 ? (u8)(rgb->g - 8) : 0;
            rgb->b = rgb->b > 8 ? (u8)(rgb->b - 8) : 0;
        }
        SetPalette(&faded_palette, 1, 255);
        advance_frame();
    }
}

//3B314
s32 MoviePlayInternal(const char* path, const char* filename, s32 cd_track, u8 loop) {
    if (!filename) {
        return -5;
    }
    playing_intro_video = strcasecmp("intro.dat", filename) == 0;

    FILE* file = movie_open_file(path, filename);
    if (!file) {
        return -5;
    }

    bool music_enabled = MusicCdActive != 0;
    if (music_enabled) {
        stop_cd();
    }

    movie_decoder_t movie;
    s32 result = movie_open_decoder(&movie, file);
    if (result < 0) {
        fclose(file);
        return result;
    }

    u8* saved_draw_buffer = draw_buffer;
    draw_buffer = DrawBufferNormal;
    memset(DrawBufferNormal, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    memset(&movie.palette, 0, sizeof(movie.palette));
    SetPalette(&movie.palette, 0, 256);

    if (music_enabled && cd_track > 0) {
        play_cd_track(cd_track, false);
    }

    result = loop ? movie_play_loop(&movie) : movie_play_once(&movie);
    MovieFadeOutPalette();

    if (music_enabled) {
        stop_cd();
    }
    draw_buffer = saved_draw_buffer;
    movie_close_decoder(&movie);
    fclose(file);
    return result;
}

//3B4A8
s32 playVideo(const char* path, const char* filename, s32 a2) {
    return MoviePlayInternal(path, filename, a2, 0);
}

//3B4B8
s32 playVideoLoop(const char* path, const char* filename, s32 a2) {
    return MoviePlayInternal(path, filename, a2, 1);
}

//3B4D0
void SWAP_BUFFERS(void) {
    if (ModeVideoActuel == MODE_X) {
        print_once("Not implemented: SWAP_BUFFERS"); //stub
    } else {
        draw_buffer = DrawBufferNormal;
        display_buffer = DisplayBufferNormal;
        if (Mode_Pad == 1 && Main_Control) {
            Swap_And_Test_Joystick(display_buffer, DrawBufferNormal, 320, 200);
        }
    }
    print_once("Not implemented: SWAP_BUFFERS"); //stub
}

//3B580
void sub_3B580(void) {
    print_once("Not implemented: sub_3B580"); //stub
}

//3B5E8
void sub_3B5E8(void) {
    print_once("Not implemented: sub_3B5E8"); //stub
}

//3B64C
void calc_gros_type(void) {
    print_once("Not implemented: calc_gros_type"); //stub
}

//3B7D4
void find_in_map(s16 a1, s16 a2) {
    print_once("Not implemented: find_in_map"); //stub
}

//3B804
void init_find_in_map(void) {
    print_once("Not implemented: init_find_in_map"); //stub
}

//3B838
void end_find_in_map(void) {
    print_once("Not implemented: end_find_in_map"); //stub
}

//3B844
void build_map(s16 a1, s16 a2) {
    print_once("Not implemented: build_map"); //stub
}

//3B948
void build_line_map(void* a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: build_line_map"); //stub
}

//3BA64
void build_column_map(void* a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: build_column_map"); //stub
}

//3BB84
void update_map(s16 a1, s16 a2, s16 a3, s16 a4) {
    print_once("Not implemented: update_map"); //stub
}

//3BE20
void sub_3BE20(void) {
    print_once("Not implemented: sub_3BE20"); //stub
}

//3BEE0
void sub_3BEE0(s16 a1, s16 a2) {
    print_once("Not implemented: sub_3BEE0"); //stub
}

//3C194
void set_vga_frequency(u8 freq) {
    SetVideoRegister();
    switch(freq) {
        case 100:
        case 80:
        case 70:
        case 60:
        case 50:
        default: break; //stub
    }
    if (freq != 0) {
        VGA_FREQ = freq;
        global_app_state.target_game_hz = freq; // added
        global_app_state.target_seconds_per_frame = 1.0f / (float)global_app_state.target_game_hz;
    }
    print_once_dos("Not implemented: set_vga_frequency"); //stub
}

//3C2D0
void GetVideoRegister(void) {
    print_once_dos("Not implemented: GetVideoRegister"); //stub
}

//3C34C
void SetVideoRegister(void) {
    print_once_dos("Not implemented: SetVideoRegister"); //stub
}

//3C3BC
void sub_3C3BC(void) {
    print_once("Not implemented: sub_3C3BC"); //stub
}

//3C4FC
void clear_palette(rgb_palette_t* palette) {
    memset(palette, 0, sizeof(rgb_palette_t));
    memset(rvb_fade, 0, sizeof(rvb_fade));
}

//3C520
void set_fade_palette(rgb_palette_t* palette) {
    for (s32 i = 0; i < 256 * 3; ++i) {
        u8 c1 = ((u8*)(palette))[i];
        u32 temp = c1 << 6;
        rvb_fade[i] = temp;
    }
}

//3C54C
void start_fade_in(s16 speed) {
    // apply palette par_0?
    nb_fade = 1 << (6 - speed);
    fade = 1; // fade in
    clear_palette(&current_rvb);
    fade_speed = speed;
    SetPalette(&current_rvb, 0, 256);
}

//3C5A4
void start_fade_out(s16 speed) {
    nb_fade = 1 << (6 - speed);
    fade = 2; // fade out
    fade_speed = speed;
}

//3C5CC
void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
    if (nb_fade > 0) {
        --nb_fade;
        if (fade == 1) {
            for (s32 i = 0; i < 256 * 3; ++i) {
                u8 c1 = ((u8*)(source_pal->colors))[i];
                u32 temp = c1 << fade_speed;
                rvb_fade[i] += temp;
            }
        } else if (fade == 2) {
            for (s32 i = 0; i < 256 * 3; ++i) {
                if (rvb_fade[i] > 0) {
                    u8 c1 = ((u8*)(source_pal->colors))[i];
                    u32 temp = c1 << fade_speed;
                    rvb_fade[i] -= temp;
                }
            }
        }
        for (s32 i = 0; i < 256 * 3; ++i) {
            u16 temp = rvb_fade[i] >> 6;
            ((u8*)(dest_pal->colors))[i] = (u8)temp;
        }
        SetPalette(&current_rvb, 0, 256);
        if (nb_fade == 0) {
            fade |= 0x40;
        }
    }
}

//3C6BC
void fade_out(s16 speed, rgb_palette_t* palette) {
    start_fade_out(speed);
    u16 steps = nb_fade;
    for (s32 i = 0; i < steps; ++i) {
        advance_frame();
        do_fade(palette, &current_rvb);
    }
    advance_frame();
}

//3C6F8
void actualize_palette(u8 new_pal_id) {
    if (new_pal_id != current_pal_id) {
        current_pal_id = new_pal_id;
        current_rvb = rvb[new_pal_id];
        set_fade_palette(&current_rvb);
        if (fade == 65) {
            SetPalette(&current_rvb, 0, 256);
        }
    }
}

//3C770
void cyclage_palette(s16 palette_index, s16 first_color, s16 last_color) {
    if (palette_index < 0 || palette_index >= (s16)LEN(rvb) ||
        first_color < 0 || last_color >= (s16)LEN(rvb[0].colors) ||
        first_color > last_color) {
        return;
    }

    rgb_palette_t* palette = &rvb[palette_index];
    rgb_t wrapped_color = palette->colors[first_color];
    for (s16 color = first_color; color < last_color; ++color) {
        palette->colors[color] = palette->colors[color + 1];
    }
    palette->colors[last_color] = wrapped_color;

    current_rvb = *palette;
    set_fade_palette(&current_rvb);
    if (nb_fade == 0) {
        nb_fade = 1;
    }
}

//3C8C8
void DO_SWAP_PALETTE(void) {
    if (last_plan1_palette != 0) {
        if (fade & 0x40) {
            for (s32 i = 0; i < actobj.num_active_objects; ++i) {
                obj_t* obj = level.objects + actobj.objects[i];
                if (obj->type == TYPE_158_PALETTE_SWAPPER) {
                    u8 pal_a = 0;
                    u8 pal_b = 0;
                    u8 new_pal = current_pal_id;
                    s16 ray_pos;
                    s16 obj_pos;
                    switch(obj->sub_etat) {
                        default: break;
                        case 0:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 1:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 2:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 3:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 4:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 5:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 6:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapVertical;
                        case 7:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapVertical;
                        case 8:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapVertical;
                        case 9:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapVertical;
                        case 10:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapVertical;
                        case 11:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapVertical;

                        SwapVertical:
                            ray_pos = ray.y + ray.offset_by;
                            obj_pos = obj->y + obj->offset_by;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;

                        SwapHorizontal:
                            ray_pos = ray.x + ray.offset_bx;
                            obj_pos = obj->x + obj->offset_bx;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;
                    }
                    actualize_palette(new_pal);
                }
            }
        }
    }
}

//3CA64
void DO_FADE(void) {
    rgb_palette_t* palette = rvb + current_pal_id;
    do_fade(palette, &current_rvb);
}

//3CA8C
void INIT_FADE_IN(void) {
    start_fade_in(2);
}

//3CAB4
void INIT_FADE_OUT(void) {
    start_fade_out(2);
}

//3CADC
void DO_FADE_OUT(void) {
    fade_out(2, rvb + current_pal_id);
}

//3CB04
void EFFACE_VIDEO(void) {
    endsynchro();
    if (ModeVideoActuel == MODE_X) {
        SWAP_BUFFERS();
        display_emptypicture();
        display_emptypicture();
    } else {
        ClearDrawAndDisplayBufferNormal(DrawBufferNormal, DisplayBufferNormal);
    }
}

//3CB54
void SYNCHRO_LOOP(scene_func_t scene_func) {
    s16 scene_ended = 0;
    do {
        advance_frame();
        DO_FADE();
        u32 timer = 0;
        scene_ended = scene_func(timer);
    } while(!scene_ended);
}

//3CBE8
void DISPLAY_ANYSIZE_PICTURE(void* a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    print_once("Not implemented: DISPLAY_ANYSIZE_PICTURE"); //stub
}

//3CCE4
void SAVE_PALETTE(rgb_palette_t* palette) {
    save_current_pal = current_pal_id;
    rvb_save = rvb[0];
    rvb[0] = *palette;
    current_rvb = *palette;
    current_pal_id = 0;
}

//3CD58
void RESTORE_PALETTE(void) {
    current_pal_id = save_current_pal;
    rvb[0] = rvb_save;
}

//3CD88
void SAVE_PLAN3(void) {
    PLANTMPBIT = PLAN3BIT;
    plantmp_length = plan3bit_length;
    plantmp_nb_bytes = plan3bit_nb_bytes;
    PLAN3BIT = PLANVIGBIT[display_Vignet];
    plan3bit_length = planVigbit_length[display_Vignet];
    plan3bit_nb_bytes = planVigbit_nb_bytes[display_Vignet];
}

//3CDD8
void RESTORE_PLAN3(void) {
    PLAN3BIT = PLANTMPBIT;
    plan3bit_length = plantmp_length;
    plan3bit_nb_bytes = plantmp_nb_bytes;
}

//3CDF8
void DISPLAY_FOND3(void) {
    memcpy(DrawBufferNormal, PLAN3BIT, 320*200);
}

//3CE20
void DISPLAY_FOND_MENU(void) {
    s32 v1 = 320 - Bloc_lim_W2 + Bloc_lim_W1;
    if (OptionGame) {
        if (prev_Bloc_lim_W1 != Bloc_lim_W1) {
            if (prev_Bloc_lim_W1 < Bloc_lim_W1) {
                ClearBorder((s16)Bloc_lim_H1, (s16)Bloc_lim_H2, (s16)Bloc_lim_W1, (s16)Bloc_lim_W2);
            }
            Bloc_lim_W1 = prev_Bloc_lim_W1;
        }
        memset(draw_buffer, 0, Bloc_lim_H1 * 320);
        s32 buffer_offset = Bloc_lim_H1 * 320 + Bloc_lim_W1;
        for (s32 y = Bloc_lim_H1; y < Bloc_lim_H2; ++y) {
            u8* dest_pos = draw_buffer + buffer_offset;
            u8* source_pos = EffetBufferNormal + buffer_offset;
            for (s32 x = Bloc_lim_W1; x < Bloc_lim_W2; ++x) {
                *dest_pos++ = *source_pos++;
            }
            buffer_offset += 320;
        }
        memset(draw_buffer, 0, (200 - Bloc_lim_H2) * 320);
    } else {
        memcpy(draw_buffer, EffetBufferNormal, 320*200);
    }
}

// NOTE: ChatGPT (GPT-5.2) was used to clean up InitPaletteSpecialPC() and DoFadePaletteSpecialPC()

// Helpers for InitPaletteSpecialPC
static inline u8 clamp_u8_255(s32 x) {
    // NOTE: In the original PC version the palettes are 6-bit (0..63).
    // However, in Rayverse we're instead working with 8-bit palettes (0..255). -> adjust the clamp accordingly
    if (x < 0)  return 0;
    if (x > 255) return 255;
    return (u8)x;
}

static inline u8 scale_div_clamp(u8 v, s32 mul, s32 div) {
    // assembly does signed division, but inputs are u8 so this is equivalent
    return clamp_u8_255((mul * v) / div);
}

//3CF70
void InitPaletteSpecialPC(void) {
    CompteurEclair = 0;
    bool lightning_level = (num_world == 1 && num_level == 9) || (num_world == 2 && num_level == 4) || (num_world == 4 && num_level == 9);
    if (lightning_level) {
        for (s32 i = 0; i < 256; ++i) {
            rgb_t src = rvb[0].colors[i];

            // pal15 = original (raw copy)
            rvb_special[15].colors[i] = src;

            // pal0: r,g = (4*v)/8 = v/2 ; b = (4*v)/5
            rvb_special[0].colors[i].r  = scale_div_clamp(src.r, 4, 8);
            rvb_special[0].colors[i].g  = scale_div_clamp(src.g, 4, 8);
            rvb_special[0].colors[i].b  = scale_div_clamp(src.b, 4, 5);

            // pal5: (4*v)/3
            rvb_special[5].colors[i].r  = scale_div_clamp(src.r, 4, 3);
            rvb_special[5].colors[i].g  = scale_div_clamp(src.g, 4, 3);
            rvb_special[5].colors[i].b  = scale_div_clamp(src.b, 4, 3);

            // pal7: r,g = (4*v)/6 ; b = (4*v)/4 = v
            rvb_special[7].colors[i].r  = scale_div_clamp(src.r, 4, 6);
            rvb_special[7].colors[i].g  = scale_div_clamp(src.g, 4, 6);
            rvb_special[7].colors[i].b  = scale_div_clamp(src.b, 4, 4);

            // pal10: (8*v)/4 = 2*v
            rvb_special[10].colors[i].r = scale_div_clamp(src.r, 8, 4);
            rvb_special[10].colors[i].g = scale_div_clamp(src.g, 8, 4);
            rvb_special[10].colors[i].b = scale_div_clamp(src.b, 8, 4);

            // pal14: same formula as pal0 in this branch
            rvb_special[14].colors[i].r = scale_div_clamp(src.r, 4, 8);
            rvb_special[14].colors[i].g = scale_div_clamp(src.g, 4, 8);
            rvb_special[14].colors[i].b = scale_div_clamp(src.b, 4, 5);

            rvb[0].colors[i] = rvb_special[0].colors[i];
        }

        // Fill intermediate palettes
        DoFadePaletteSpecialPC(0, 5);
        DoFadePaletteSpecialPC(5, 7);
        DoFadePaletteSpecialPC(7, 10);
        DoFadePaletteSpecialPC(10, 14);

        numero_palette_special = 0;
        ProchainEclair = (s16)(myRand(200) + 1);
    } else if (num_world == 5 && num_level == 4) {
        // First loop runs for 255 colors (0..254) -> 0x2FD bytes copied
        for (int i = 0; i < 255; ++i) {
            rgb_t src = rvb[0].colors[i];

            rvb_special[15].colors[i] = src;

            // pal0: r=min(r,63), g=min(77*g/100,63), b=min(12*b/100,63)
            rvb_special[0].colors[i].r = clamp_u8_255((int)src.r);
            rvb_special[0].colors[i].g = clamp_u8_255((77 * (int)src.g) / 100);
            rvb_special[0].colors[i].b = clamp_u8_255((12 * (int)src.b) / 100);

            // pal14 initially same as pal0 (asm writes to +0x2A00 block too)
            rvb_special[14].colors[i] = rvb_special[0].colors[i];

            rvb[0].colors[i] = rvb_special[0].colors[i];

        }

        DoFadePaletteSpecialPC(0, 14);

        // Second pass: modify palette 9 for color indices 128..174 (ecx=0x180..0x20D step 3)
        for (int i = 128; i <= 174; ++i) {
            rgb_t src = rvb[0].colors[i];

            rvb_special[9].colors[i].r = clamp_u8_255((84 * (int)src.r) / 100);
            rvb_special[9].colors[i].g = clamp_u8_255((62 * (int)src.g) / 100);
            rvb_special[9].colors[i].b = clamp_u8_255((12 * (int)src.b) / 100);

            rvb[0].colors[i] = rvb_special[9].colors[i];
        }

        DoFadePaletteSpecialPC(0, 9);
        DoFadePaletteSpecialPC(9, 14);
    }
}

//3D54C
void DoFadePaletteSpecialPC(s16 start, s16 end) {
    if (start + 1 >= end)
        return;

    s32 denom = end - start;

    for (s32 p = start + 1; p < end; ++p) {
        s32 w0 = end - p;     // weight for start
        s32 w1 = p - start;   // weight for end

        for (s32 i = 0; i < 256; ++i) {
            rgb_t a = rvb_special[start].colors[i];
            rgb_t b = rvb_special[end].colors[i];

            rvb_special[p].colors[i].r = clamp_u8_255((a.r * w0 + b.r * w1) / denom);
            rvb_special[p].colors[i].g = clamp_u8_255((a.g * w0 + b.g * w1) / denom);
            rvb_special[p].colors[i].b = clamp_u8_255((a.b * w0 + b.b * w1) / denom);
        }
    }
}

//3D704
void DoPaletteSpecialPC(void) {
    if (fade & 0x40) {
        if (num_world == 5 && num_level == 4) {
            if (horloge[5] != 0) {
                if (numero_palette_special == 0) {
                    numero_palette_special = 1;
                }
            } else {
                SetPalette(&rvb_special[numero_palette_special], 1, 255);
                ++numero_palette_special;
            }
            if (numero_palette_special == 15) {
                numero_palette_special = 0;
            }
        } else if ((num_world == 1 && num_level == 9) || (num_world == 2 && num_level == 4) || (num_world == 4 && num_level == 9)) {
            ++CompteurEclair;
            if (CompteurEclair >= ProchainEclair || numero_palette_special != 0) {
                SetPalette(&rvb_special[numero_palette_special], 1, 255);
                if (numero_palette_special == 0) {
                    PlaySnd_old(195);
                    ++numero_palette_special;
                } else {
                    if (CompteurEclair % 2 != 0) {
                        // NOTE: the code below is unreachable (statement is always false); seems to be a minor bug in the original game?
                        if (numero_palette_special == 0) {
                            numero_palette_special = 1;
                        }
                    } else {
                        ++numero_palette_special;
                    }
                }
                if (numero_palette_special == 15) {
                    CompteurEclair = 0;
                    numero_palette_special = 0;
                    ProchainEclair = (s16)(myRand(400) + 1);
                }
            }
        }
    }
}

//3D8AC
void InitModeXWithFrequency(u8 freq) {
    print_once("Not implemented: InitModeXWithFrequency"); //stub
}

//3D9D4
void InitTextMode(void) {
    textmode();
    ModeVideoActuel = MODE_TEXT;
}

//3D9E4
void InitModeNormalWithFrequency(u8 freq) {
    if (ModeVideoActuel == MODE_TEXT) {
        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != MODE_NORMAL) {
        InitModeNormal();
//		memset(DrawBufferNormal, 0, 320 * 200);
//		memset(DisplayBufferNormal, 0, 320 * 200);
        select_display_buffer(DisplayBufferNormal);
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_NORMAL;
        DrawSpriteNoClipNormalEtX = DrawSpriteNormalNoClip;
        DrawSpriteFlipNoClipNormalEtX = DrawSpriteFlipNormalNoClip;
        DrawSpriteColorNormalEtX = DrawSpriteColorNormal;
        DrawSpriteColorFlipNormalEtX = DrawSpriteColorFlipNormal;
        draw_buffer = DrawBufferNormal;
        DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
        DrawSpriteNormalEtX = DrawSpriteNormal;
		drawflocon1NormalETX = draw_flocon1_Normal;
		drawflocon2NormalETX = draw_flocon2_Normal;
		drawflocon3NormalETX = draw_flocon3_Normal;
		drawflocon4NormalETX = draw_flocon4_Normal;
		drawflocon5NormalETX = draw_flocon5_Normal;
		drawflocon6NormalETX = draw_flocon6_Normal;
		drawflocon7NormalETX = draw_flocon7_Normal;
		drawpluie4NormalETX = draw_pluie4_Normal;
		drawpluie6NormalETX = draw_pluie6_Normal;
		drawpluie5NormalETX = draw_pluie5_Normal;
		drawpluie7NormalETX = draw_pluie7_Normal;
		fplotNormalETX = fplot_Normal;
    }
}

//3DB24
void WaitNSynchro(s32 n_frames) {
    for (s32 i = 0; i < n_frames; ++i) {
        advance_frame();
    }
}

