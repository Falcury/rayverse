
//37B90
void sub_37B90(void) {
    print_once("Not implemented: sub_37B90"); //stub
}

//037C00
void sub_37C00(void) {
    print_once("Not implemented: sub_37C00"); //stub
}

//37C40
void sub_37C40(void) {
    print_once("Not implemented: sub_37C40"); //stub
}

//37C98
void sub_37C98(void) {
    print_once("Not implemented: sub_37C98"); //stub
}

//37CDC
void sub_37CDC(void) {
    print_once("Not implemented: sub_37CDC"); //stub
}

//37D4C
void sub_37D4C(void) {
    print_once("Not implemented: sub_37D4C"); //stub
}

//37D90
void sub_37D90(void) {
    print_once("Not implemented: sub_37D90"); //stub
}

//37E00
void sub_37E00(void) {
    print_once("Not implemented: sub_37E00"); //stub
}

//37E44
void sub_37E44(void) {
    print_once("Not implemented: sub_37E44"); //stub
}

//37E9C
void sub_37E9C(void) {
    print_once("Not implemented: sub_37E9C"); //stub
}

//37EE0
void sub_37EE0(void) {
    print_once("Not implemented: sub_37EE0"); //stub
}

//37F50
void sub_37F50(void) {
    print_once("Not implemented: sub_37F50"); //stub
}

//37F94
void sub_37F94(void) {
    print_once("Not implemented: sub_37F94"); //stub
}

//37FEC
void sub_37FEC(void) {
    print_once("Not implemented: sub_37FEC"); //stub
}

//38030
void sub_38030(void) {
    print_once("Not implemented: sub_38030"); //stub
}

//38054
void sub_38054(void) {
    print_once("Not implemented: sub_38054"); //stub
}

//38064
void sub_38064(void) {
    print_once("Not implemented: sub_38064"); //stub
}

//38074
u8 ChooseRandomPausePlaneVariant(void) {
    return (u8)(myRand(6) + 1);
}

/* 380A4-38290: PC-only world-vignette transition subsystem.
 *
 * The Android port replaced these blocking deformations with its
 * startWorldVignetInit/Update/End state machine and a plain palette fade. The
 * names below are therefore descriptive and provisional rather than canonical.
 * The original implementation delegates to seven VGA-era mesh/scanline
 * renderers. This translation reconstructs their persistent state, projected
 * geometry, and selection rules, while replacing only the VGA-specific affine
 * quadrilateral rasterizer with a portable 8-bit implementation. */
enum {
    VIGNETTE_TILE_COLUMNS = 8,
    VIGNETTE_TILE_ROWS = 8,
    VIGNETTE_TILE_COUNT = VIGNETTE_TILE_COLUMNS * VIGNETTE_TILE_ROWS,
    VIGNETTE_RADIAL_COLUMNS = 20,
    VIGNETTE_RADIAL_ROWS = 10,
    VIGNETTE_SPHERE_COLUMNS = 20,
    VIGNETTE_SPHERE_ROWS = 8,
    VIGNETTE_RADIAL_SAMPLES = 75
};

typedef struct vignette_tile_t {
    float center_x;
    float center_y;
    float center_z;
    float velocity_x;
    float velocity_y;
    float velocity_z;
    float angle_x;
    float angle_y;
    float angle_z;
    float angular_velocity_x;
    float angular_velocity_y;
    float angular_velocity_z;
    s16 source_x;
    s16 source_y;
    s16 source_width;
    s16 source_height;
} vignette_tile_t;

typedef struct world_vignette_transition_t {
    u8* source;
    u8* output;
    s16 effect;
    s16 variant;
    s16 frame;
    s16 frame_count;
    u8 opening;
    u8 active;
    vignette_tile_t tiles[VIGNETTE_TILE_COUNT];
    float radial_history[VIGNETTE_RADIAL_SAMPLES];
    float radial_source[VIGNETTE_RADIAL_SAMPLES];
    s16 radial_source_cursor;
    s16 lens_center_x;
    s16 lens_center_y;
    s16 lens_velocity_x;
    s16 lens_velocity_y;
    s16 sphere_center_x;
    s16 sphere_center_y;
    s16 sphere_velocity_x;
    s16 sphere_velocity_y;
    s16 sphere_rotation_x;
    s16 sphere_rotation_y;
    u8 sphere_stage_initialized;
    u8 sphere_horizontal_contact;
    u8 sphere_vertical_contact;
    u8 sphere_spin_y;
    u8 sphere_hit_right;
    u8 sphere_finished_bouncing;
    s16 roll_speed;
    s16 roll_position;
    s16 roll_initial_position;
    s16 roll_stage_target;
    s16 roll_arc_start;
    s16 roll_source_offset;
    u8 roll_stage;
    u8 roll_stage_initialized;
    s16 roll_arc_lookup[44];
} world_vignette_transition_t;

enum {
    VIGNETTE_EFFECT_TILE_GRID = 1,
    VIGNETTE_EFFECT_SCANLINE_WIPE = 2,
    VIGNETTE_EFFECT_RADIAL_WARP = 3,
    VIGNETTE_EFFECT_SPHERE_TUMBLE = 5,
    VIGNETTE_EFFECT_ROTATING_PLANE = 6,
    VIGNETTE_EFFECT_BOUNCING_SADDLE_LENS = 7,
    VIGNETTE_EFFECT_CYLINDRICAL_ROLL = 8
};

static world_vignette_transition_t world_vignette_transition;

static float clamp_vignette_fraction(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}

static void copy_vignette_row_segment(u8* output, const u8* source, s32 y, s32 x0, s32 x1) {
    if (y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }
    x0 = MAX(x0, 0);
    x1 = MIN(x1, SCREEN_WIDTH);
    if (x1 > x0) {
        memcpy(output + y * SCREEN_WIDTH + x0, source + y * SCREEN_WIDTH + x0, (size_t)(x1 - x0));
    }
}

/* Copy one of the ten-byte scanline descriptors consumed by PC routine 44760.
 * Its clipping reduces the width independently at the source and destination
 * edges; it does not compensate the opposite coordinate. */
static void copy_vignette_displaced_strip(u8* output, const u8* source,
                                          s32 source_x, s32 source_y,
                                          s32 destination_x, s32 destination_y,
                                          s32 width) {
    if (source_y < 0 || source_y >= SCREEN_HEIGHT ||
        destination_y < 0 || destination_y >= SCREEN_HEIGHT || width <= 0) {
        return;
    }
    if (source_x < 0) {
        width += source_x;
        source_x = 0;
    }
    if (source_x + width > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - source_x;
    }
    if (destination_x < 0) {
        width += destination_x;
        destination_x = 0;
    }
    if (destination_x + width > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - destination_x;
    }
    if (width > 0) {
        memcpy(output + destination_y * SCREEN_WIDTH + destination_x,
               source + source_y * SCREEN_WIDTH + source_x, (size_t)width);
    }
}

typedef struct vignette_vertex_t {
    float x;
    float y;
    float z;
    float u;
    float v;
} vignette_vertex_t;

typedef struct vignette_projected_vertex_t {
    float x;
    float y;
    float z;
    float u;
    float v;
} vignette_projected_vertex_t;

typedef struct vignette_mesh_cell_t {
    s16 column;
    s16 row;
    float depth;
} vignette_mesh_cell_t;

static float vignette_edge(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

/* Portable counterpart of the PC textured-quadrilateral rasterizer at 10045.
 * The DOS routine scan-converts affine texture coordinates into an 8-bit
 * framebuffer. Two triangles give equivalent geometry without retaining the
 * VGA-specific edge tables and self-modifying inner loops. */
static void draw_vignette_textured_triangle(const world_vignette_transition_t* transition,
                                             const vignette_projected_vertex_t* a,
                                             const vignette_projected_vertex_t* b,
                                             const vignette_projected_vertex_t* c) {
    float area = vignette_edge(a->x, a->y, b->x, b->y, c->x, c->y);
    if ((float)fabs(area) < 0.01f) {
        return;
    }

    s32 min_x = MAX(0, (s32)floor(MIN(a->x, MIN(b->x, c->x))));
    s32 max_x = MIN(SCREEN_WIDTH - 1, (s32)ceil(MAX(a->x, MAX(b->x, c->x))));
    s32 min_y = MAX(0, (s32)floor(MIN(a->y, MIN(b->y, c->y))));
    s32 max_y = MIN(SCREEN_HEIGHT - 1, (s32)ceil(MAX(a->y, MAX(b->y, c->y))));
    float inverse_area = 1.0f / area;

    for (s32 y = min_y; y <= max_y; ++y) {
        float pixel_y = y + 0.5f;
        for (s32 x = min_x; x <= max_x; ++x) {
            float pixel_x = x + 0.5f;
            float weight_a = vignette_edge(b->x, b->y, c->x, c->y, pixel_x, pixel_y) * inverse_area;
            float weight_b = vignette_edge(c->x, c->y, a->x, a->y, pixel_x, pixel_y) * inverse_area;
            float weight_c = 1.0f - weight_a - weight_b;
            if (weight_a >= -0.0005f && weight_b >= -0.0005f && weight_c >= -0.0005f) {
                s32 source_x = (s32)(a->u * weight_a + b->u * weight_b + c->u * weight_c + 0.5f);
                s32 source_y = (s32)(a->v * weight_a + b->v * weight_b + c->v * weight_c + 0.5f);
                source_x = MAX(0, MIN(source_x, SCREEN_WIDTH - 1));
                source_y = MAX(0, MIN(source_y, SCREEN_HEIGHT - 1));
                transition->output[y * SCREEN_WIDTH + x] = transition->source[source_y * SCREEN_WIDTH + source_x];
            }
        }
    }
}

static void draw_vignette_textured_quad(const world_vignette_transition_t* transition,
                                         const vignette_projected_vertex_t* top_left,
                                         const vignette_projected_vertex_t* top_right,
                                         const vignette_projected_vertex_t* bottom_right,
                                         const vignette_projected_vertex_t* bottom_left) {
    draw_vignette_textured_triangle(transition, top_left, top_right, bottom_right);
    draw_vignette_textured_triangle(transition, top_left, bottom_right, bottom_left);
}

static bool project_vignette_vertex(const vignette_vertex_t* source,
                                    float center_x, float center_y, float camera_distance,
                                    vignette_projected_vertex_t* result) {
    float denominator = source->z + camera_distance;
    if (denominator <= 1.0f) {
        return false;
    }
    result->x = center_x + source->x * 512.0f / denominator;
    result->y = center_y + source->y * 512.0f / denominator;
    result->z = source->z;
    result->u = source->u;
    result->v = source->v;
    return true;
}

static void rotate_vignette_vertex(vignette_vertex_t* vertex,
                                   float angle_x, float angle_y, float angle_z) {
    if (angle_z != 0.0f) {
        float cosine = (float)cos(angle_z);
        float sine = (float)sin(angle_z);
        float x = vertex->x * cosine - vertex->y * sine;
        vertex->y = vertex->y * cosine + vertex->x * sine;
        vertex->x = x;
    }
    if (angle_y != 0.0f) {
        float cosine = (float)cos(angle_y);
        float sine = (float)sin(angle_y);
        float x = vertex->z * sine + vertex->x * cosine;
        vertex->z = vertex->z * cosine - vertex->x * sine;
        vertex->x = x;
    }
    if (angle_x != 0.0f) {
        float cosine = (float)cos(angle_x);
        float sine = (float)sin(angle_x);
        float y = vertex->z * sine + vertex->y * cosine;
        vertex->z = vertex->z * cosine - vertex->y * sine;
        vertex->y = y;
    }
}

static void sort_vignette_cells_back_to_front(vignette_mesh_cell_t* cells, s32 count) {
    for (s32 i = 1; i < count; ++i) {
        vignette_mesh_cell_t cell = cells[i];
        s32 insert_at = i;
        while (insert_at > 0 && cells[insert_at - 1].depth < cell.depth) {
            cells[insert_at] = cells[insert_at - 1];
            --insert_at;
        }
        cells[insert_at] = cell;
    }
}

static void init_vignette_tile_grid(world_vignette_transition_t* transition) {
    for (s32 tile_y = 0; tile_y < VIGNETTE_TILE_ROWS; ++tile_y) {
        for (s32 tile_x = 0; tile_x < VIGNETTE_TILE_COLUMNS; ++tile_x) {
            s32 tile_index = tile_y * VIGNETTE_TILE_COLUMNS + tile_x;
            vignette_tile_t* tile = &transition->tiles[tile_index];
            s32 source_x = tile_x * SCREEN_WIDTH / VIGNETTE_TILE_COLUMNS;
            s32 source_y = tile_y * SCREEN_HEIGHT / VIGNETTE_TILE_ROWS;
            s32 next_x = (tile_x + 1) * SCREEN_WIDTH / VIGNETTE_TILE_COLUMNS;
            s32 next_y = (tile_y + 1) * SCREEN_HEIGHT / VIGNETTE_TILE_ROWS;

            tile->source_x = (s16)source_x;
            tile->source_y = (s16)source_y;
            tile->source_width = (s16)(next_x - source_x);
            tile->source_height = (s16)(next_y - source_y);
            tile->center_x = (source_x + next_x) * 0.5f - SCREEN_WIDTH * 0.5f;
            tile->center_y = (source_y + next_y) * 0.5f - SCREEN_HEIGHT * 0.5f;
            tile->center_z = 0.0f;

            /* These ranges and the left/right bias are the rand() expressions
               used by the PC tile initializer at 38B58. */
            tile->velocity_x = tile_index % 10 < 5 ? (float)(rand() % 3) : (float)(rand() % 3 - 2);
            tile->velocity_y = (float)(rand() % 8 - 6);
            tile->velocity_z = (float)(rand() % 5 - 2);
            tile->angular_velocity_x = (float)(rand() % 5 + 2) * two_pi32 / 256.0f;
            tile->angular_velocity_y = (float)(rand() % 5 + 2) * two_pi32 / 256.0f;
            tile->angular_velocity_z = (float)(rand() % 5 + 2) * two_pi32 / 256.0f;
            tile->angle_x = tile->angular_velocity_x * 5.0f;
            tile->angle_y = tile->angular_velocity_y * 5.0f;
            tile->angle_z = tile->angular_velocity_z * 5.0f;
        }
    }
}

static void init_vignette_radial_mesh(world_vignette_transition_t* transition) {
    memset(transition->radial_history, 0, sizeof(transition->radial_history));
    for (s32 i = 0; i < VIGNETTE_RADIAL_SAMPLES; ++i) {
        /* The PC builds this table with sinus(i * 20), a -40 amplitude,
           and the same 512-unit angular period used elsewhere in the game. */
        float angle = (float)(i * 20) * two_pi32 / 512.0f;
        transition->radial_source[i] = -40.0f * (float)sin(angle);
    }
    transition->radial_source_cursor = VIGNETTE_RADIAL_SAMPLES;
}

static void init_vignette_cylindrical_roll(world_vignette_transition_t* transition) {
    const s32 roll_radius = 22;
    transition->roll_speed = transition->opening ? -2 : 2;
    transition->roll_stage = transition->roll_speed > 0 ? 0 : 3;

    /* PC builder 733E0 stores radius * acos(x / radius) for x=-R..R-1.
       The draw stages select either side of this table to expose the visible
       face of the rolled image. */
    for (s32 x = -roll_radius; x < roll_radius; ++x) {
        transition->roll_arc_lookup[x + roll_radius] =
            (s16)(roll_radius * acos((double)x / roll_radius));
    }
}

static void init_vignette_effect_state(world_vignette_transition_t* transition) {
    if (transition->effect == VIGNETTE_EFFECT_TILE_GRID) {
        init_vignette_tile_grid(transition);
    } else if (transition->effect == VIGNETTE_EFFECT_RADIAL_WARP) {
        init_vignette_radial_mesh(transition);
    } else if (transition->effect == VIGNETTE_EFFECT_BOUNCING_SADDLE_LENS) {
        s16 speed = transition->opening ? 4 : -4;
        transition->lens_center_x = SCREEN_WIDTH / 2;
        transition->lens_center_y = SCREEN_HEIGHT / 2;
        transition->lens_velocity_x = (rand() & 7) < 4 ? speed : (s16)-speed;
        transition->lens_velocity_y = (rand() & 7) < 4 ? speed : (s16)-speed;
    } else if (transition->effect == VIGNETTE_EFFECT_CYLINDRICAL_ROLL) {
        init_vignette_cylindrical_roll(transition);
    }
}

static void render_vignette_tile_grid(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    vignette_mesh_cell_t draw_order[VIGNETTE_TILE_COUNT];
    memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    for (s32 tile_index = 0; tile_index < VIGNETTE_TILE_COUNT; ++tile_index) {
        vignette_tile_t* tile = &transition->tiles[tile_index];
        tile->center_x += tile->velocity_x;
        tile->center_y += tile->velocity_y;
        tile->center_z += tile->velocity_z;
        draw_order[tile_index].column = (s16)tile_index;
        draw_order[tile_index].depth = tile->center_z;
    }
    sort_vignette_cells_back_to_front(draw_order, VIGNETTE_TILE_COUNT);

    for (s32 order_index = 0; order_index < VIGNETTE_TILE_COUNT; ++order_index) {
        vignette_tile_t* tile = &transition->tiles[draw_order[order_index].column];
        float half_width = tile->source_width * 0.5f;
        float half_height = tile->source_height * 0.5f;
        float local_x[4] = {-half_width, half_width, half_width, -half_width};
        float local_y[4] = {-half_height, -half_height, half_height, half_height};
        float texture_u[4] = {(float)tile->source_x,
                              (float)(tile->source_x + tile->source_width - 1),
                              (float)(tile->source_x + tile->source_width - 1),
                              (float)tile->source_x};
        float texture_v[4] = {(float)tile->source_y, (float)tile->source_y,
                              (float)(tile->source_y + tile->source_height - 1),
                              (float)(tile->source_y + tile->source_height - 1)};
        vignette_projected_vertex_t projected[4];
        bool can_draw = true;

        for (s32 corner = 0; corner < 4; ++corner) {
            vignette_vertex_t vertex = {local_x[corner], local_y[corner], 0.0f,
                                        texture_u[corner], texture_v[corner]};
            rotate_vignette_vertex(&vertex, tile->angle_x, tile->angle_y, tile->angle_z);
            vertex.x += tile->center_x;
            vertex.y += tile->center_y;
            vertex.z += tile->center_z;
            can_draw &= project_vignette_vertex(&vertex, SCREEN_WIDTH * 0.5f,
                                                SCREEN_HEIGHT * 0.5f, 512.0f,
                                                &projected[corner]);
        }
        if (can_draw) {
            draw_vignette_textured_quad(&world_vignette_transition, &projected[0], &projected[1], &projected[2], &projected[3]);
        }
        tile->angle_x += tile->angular_velocity_x;
        tile->angle_y += tile->angular_velocity_y;
        tile->angle_z += tile->angular_velocity_z;
    }

    /* The PC adds one unit of downward acceleration every five updates after
       update 20, producing the characteristic shower of tumbling tiles. */
    if (transition->frame > 20 && transition->frame % 5 == 0) {
        for (s32 tile_index = 0; tile_index < VIGNETTE_TILE_COUNT; ++tile_index) {
            transition->tiles[tile_index].velocity_y += 1.0f;
        }
    }
}

static void render_vignette_scanline_wipe(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    s32 variant = transition->variant;
    s32 maximum = variant == 9 ? SCREEN_HEIGHT / 2
        : (variant == 3 || variant == 7 || variant == 11 || variant == 12
            ? SCREEN_WIDTH : SCREEN_HEIGHT);
    s32 counter = MIN((s32)transition->frame * 2, maximum);
    s32 displacement = transition->opening ? maximum - counter : counter;
    memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    if (variant >= 1 && variant <= 8) {
        for (s32 y = 0; y < SCREEN_HEIGHT; ++y) {
            s32 source_x = (variant == 6 || variant == 7 || variant == 8)
                ? displacement : 0;
            s32 source_y = y;
            s32 destination_x = (variant == 2 || variant == 3 || variant == 4)
                ? displacement : 0;
            s32 width = (variant == 1 || variant == 5)
                ? SCREEN_WIDTH : SCREEN_WIDTH - displacement;
            if (variant == 1 || variant == 2 || variant == 8) {
                source_y += displacement;
            } else if (variant == 4 || variant == 5 || variant == 6) {
                source_y -= displacement;
            }
            copy_vignette_displaced_strip(transition->output, transition->source,
                                           source_x, source_y, destination_x, y, width);
        }
        return;
    }

    if (variant == 10) {
        /* PC variant 10 sends alternate one-pixel scanlines in opposite
           vertical directions. It is selected only when the random
           scanline-family variant happens to be 10. */
        for (s32 y = 0; y < SCREEN_HEIGHT; y += 2) {
            copy_vignette_displaced_strip(transition->output, transition->source,
                                           0, y + displacement, 0, y, SCREEN_WIDTH);
            if (y + 1 < SCREEN_HEIGHT) {
                copy_vignette_displaced_strip(transition->output, transition->source,
                                               0, y - displacement, 0, y + 1, SCREEN_WIDTH);
            }
        }
        return;
    }

    if (variant == 11) {
        /* PC variant 11 is the corresponding horizontal split: even lines
           enter from the left and odd lines from the right (reversed while
           closing). */
        s32 width = SCREEN_WIDTH - displacement;
        for (s32 y = 0; y < SCREEN_HEIGHT; y += 2) {
            copy_vignette_displaced_strip(transition->output, transition->source,
                                           displacement, y, 0, y, width);
            if (y + 1 < SCREEN_HEIGHT) {
                copy_vignette_displaced_strip(transition->output, transition->source,
                                               0, y + 1, displacement, y + 1, width);
            }
        }
        return;
    }

    if (variant == 12) {
        /* Variant 12 negates the requested direction. Its counter is therefore
           the visible rectangle width, centered on both axes. */
        s32 extent = transition->opening ? counter : maximum - counter;
        s32 x = (SCREEN_WIDTH - extent) / 2;
        s32 y_margin = (SCREEN_HEIGHT - extent) / 2;
        for (s32 y = 0; y < SCREEN_HEIGHT; ++y) {
            if (y >= y_margin && y <= SCREEN_HEIGHT - y_margin) {
                copy_vignette_displaced_strip(transition->output, transition->source,
                                               x, y, x, y, extent);
            }
        }
        return;
    }

    /* Variant 9 is the 100-step centered member. Its PC update composites
       three descriptor passes; this equivalent final image retains the same
       centered vertical aperture and direction without the transient VGA-page
       writes between those passes. */
    s32 y0 = displacement;
    s32 y1 = SCREEN_HEIGHT - displacement;
    for (s32 y = y0; y < y1; ++y) {
        copy_vignette_row_segment(transition->output, transition->source,
                                  y, 0, SCREEN_WIDTH);
    }
}

static void render_vignette_radial_warp(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    vignette_projected_vertex_t vertices[VIGNETTE_RADIAL_ROWS + 1][VIGNETTE_RADIAL_COLUMNS + 1];

    float next_sample;
    if (transition->radial_source_cursor > 0) {
        --transition->radial_source_cursor;
        next_sample = transition->radial_source[transition->radial_source_cursor];
    } else {
        next_sample = transition->radial_history[VIGNETTE_RADIAL_SAMPLES - 1];
        if ((transition->frame + 1) * 2 > 600 && transition->radial_history[0] == 0.0f) {
            next_sample = 0.0f;
        }
    }
    memmove(&transition->radial_history[1], &transition->radial_history[0],
            sizeof(transition->radial_history) - sizeof(transition->radial_history[0]));
    transition->radial_history[0] = next_sample;

    memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    for (s32 row = 0; row <= VIGNETTE_RADIAL_ROWS; ++row) {
        float screen_y = (float)(row * SCREEN_HEIGHT) / VIGNETTE_RADIAL_ROWS;
        for (s32 column = 0; column <= VIGNETTE_RADIAL_COLUMNS; ++column) {
            float screen_x = (float)(column * SCREEN_WIDTH) / VIGNETTE_RADIAL_COLUMNS;
            float dx = screen_x - SCREEN_WIDTH * 0.5f;
            float dy = screen_y - SCREEN_HEIGHT * 0.5f;
            s32 radial_index = (s32)((dx * dx + dy * dy) / 512.0f);
            radial_index = MAX(0, MIN(radial_index, VIGNETTE_RADIAL_SAMPLES - 1));
            /* Update routine 7EF5C deliberately leaves the outer grid ring
               flat, anchoring the ripple to all four screen edges. */
            float depth = row == 0 || row == VIGNETTE_RADIAL_ROWS ||
                          column == 0 || column == VIGNETTE_RADIAL_COLUMNS
                ? 0.0f : transition->radial_history[radial_index];
            vignette_vertex_t vertex = {dx, dy, depth,
                                        MIN(screen_x, SCREEN_WIDTH - 1.0f),
                                        MIN(screen_y, SCREEN_HEIGHT - 1.0f)};
            project_vignette_vertex(&vertex, SCREEN_WIDTH * 0.5f,
                                    SCREEN_HEIGHT * 0.5f, 512.0f,
                                    &vertices[row][column]);
        }
    }

    for (s32 row = 0; row < VIGNETTE_RADIAL_ROWS; ++row) {
        for (s32 column = 0; column < VIGNETTE_RADIAL_COLUMNS; ++column) {
            draw_vignette_textured_quad(&world_vignette_transition, &vertices[row][column], &vertices[row][column + 1],
                                         &vertices[row + 1][column + 1], &vertices[row + 1][column]);
        }
    }
}

static void make_vignette_sphere_vertex(s32 row, s32 column,
                                        float angle_x, float angle_y,
                                        vignette_vertex_t* vertex) {
    const float sphere_radius = 61.0f;
    float latitude = -pi32 * 0.5f + (float)row * pi32 / VIGNETTE_SPHERE_ROWS;
    float longitude = (float)column * two_pi32 / VIGNETTE_SPHERE_COLUMNS;
    float latitude_radius = sphere_radius * (float)cos(latitude);
    vertex->x = -latitude_radius * (float)sin(longitude);
    vertex->y = sphere_radius * (float)sin(latitude);
    vertex->z = -latitude_radius * (float)cos(longitude);
    /* The PC first crops source X=32..287 into a 256-pixel work page, then
       assigns integer 256/20=12-pixel strips. The unused remainder means the
       flat mesh ends at X=272, not 288 as a floating-point division suggests. */
    vertex->u = (float)(32 + column * (256 / VIGNETTE_SPHERE_COLUMNS));
    vertex->v = (float)(row * (SCREEN_HEIGHT / VIGNETTE_SPHERE_ROWS));
    rotate_vignette_vertex(vertex, angle_x, angle_y, 0.0f);
}

static void render_vignette_sphere_cells(
    vignette_projected_vertex_t vertices[VIGNETTE_SPHERE_ROWS + 1][VIGNETTE_SPHERE_COLUMNS + 1]) {
    vignette_mesh_cell_t cells[VIGNETTE_SPHERE_ROWS * VIGNETTE_SPHERE_COLUMNS];
    s32 cell_count = 0;
    for (s32 row = 0; row < VIGNETTE_SPHERE_ROWS; ++row) {
        for (s32 column = 0; column < VIGNETTE_SPHERE_COLUMNS; ++column) {
            const vignette_projected_vertex_t* top_left = &vertices[row][column];
            const vignette_projected_vertex_t* top_right = &vertices[row][column + 1];
            const vignette_projected_vertex_t* bottom_right = &vertices[row + 1][column + 1];
            const vignette_projected_vertex_t* bottom_left = &vertices[row + 1][column];
            float diagonal_cross =
                (top_left->x - bottom_right->x) * (bottom_left->y - top_right->y) -
                (top_left->y - bottom_right->y) * (bottom_left->x - top_right->x);

            /* PC draw routine 268C8 rejects the opposite winding before
               calling its affine-quad rasterizer. Drawing both windings made
               the near, inward-facing half cover the picture-bearing front. */
            if (diagonal_cross >= 0.0f) {
                continue;
            }
            vignette_mesh_cell_t* cell = &cells[cell_count++];
            cell->row = (s16)row;
            cell->column = (s16)column;
            cell->depth = (vertices[row][column].z + vertices[row][column + 1].z +
                           vertices[row + 1][column + 1].z + vertices[row + 1][column].z) * 0.25f;
        }
    }
    sort_vignette_cells_back_to_front(cells, cell_count);
    for (s32 cell_index = 0; cell_index < cell_count; ++cell_index) {
        s32 row = cells[cell_index].row;
        s32 column = cells[cell_index].column;
        draw_vignette_textured_quad(&world_vignette_transition, &vertices[row][column], &vertices[row][column + 1],
                                     &vertices[row + 1][column + 1], &vertices[row + 1][column]);
    }
}

static void render_vignette_sphere_tumble(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    vignette_projected_vertex_t vertices[VIGNETTE_SPHERE_ROWS + 1][VIGNETTE_SPHERE_COLUMNS + 1];
    const s32 morph_frames = 26;
    memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    if (transition->opening || transition->frame < morph_frames) {
        float morph = clamp_vignette_fraction((float)transition->frame / (morph_frames - 1));
        float flat_amount = transition->opening ? morph : 1.0f - morph;
        for (s32 row = 0; row <= VIGNETTE_SPHERE_ROWS; ++row) {
            for (s32 column = 0; column <= VIGNETTE_SPHERE_COLUMNS; ++column) {
                vignette_vertex_t sphere_vertex;
                vignette_projected_vertex_t sphere_projection;
                make_vignette_sphere_vertex(row, column, 0.0f, 0.0f, &sphere_vertex);
                project_vignette_vertex(&sphere_vertex, SCREEN_WIDTH * 0.5f,
                                        SCREEN_HEIGHT * 0.5f, 512.0f,
                                        &sphere_projection);
                vertices[row][column] = sphere_projection;
                vertices[row][column].x = sphere_projection.x * (1.0f - flat_amount) +
                    sphere_vertex.u * flat_amount;
                vertices[row][column].y = sphere_projection.y * (1.0f - flat_amount) +
                    sphere_vertex.v * flat_amount;
            }
        }
        render_vignette_sphere_cells(vertices);
        return;
    }

    if (!transition->sphere_stage_initialized) {
        /* State 2 of PC update routine 26404 starts the completed sphere at
           (160,100), with quarter-pixel velocities (8,10). */
        transition->sphere_center_x = SCREEN_WIDTH / 2;
        transition->sphere_center_y = SCREEN_HEIGHT / 2;
        transition->sphere_velocity_x = 8;
        transition->sphere_velocity_y = 10;
        transition->sphere_rotation_x = 0;
        transition->sphere_rotation_y = 0;
        transition->sphere_stage_initialized = 1;
    }

    /* The PC stores these velocities in quarter pixels and advances this
       closing-only stage by two units per retrace. Preserve its asymmetric
       edge response and the extra Y rotation enabled after the first floor
       bounce. */
    const s32 sphere_radius = 61;
    const s32 step = 2;
    ++transition->sphere_velocity_y;
    if (transition->sphere_center_x > SCREEN_WIDTH - (sphere_radius + 4) &&
        !transition->sphere_horizontal_contact) {
        transition->sphere_velocity_x = (s16)(-transition->sphere_velocity_x + 1);
        transition->sphere_horizontal_contact = 1;
        if (transition->sphere_hit_right) {
            transition->sphere_finished_bouncing = 1;
        }
        transition->sphere_hit_right = 1;
    } else if (transition->sphere_center_x < sphere_radius + 4 &&
               !transition->sphere_horizontal_contact) {
        transition->sphere_velocity_x = (s16)(-transition->sphere_velocity_x - 1);
        transition->sphere_horizontal_contact = 1;
    } else if (transition->sphere_center_x >= sphere_radius + 4 &&
               transition->sphere_center_x <= SCREEN_WIDTH - (sphere_radius + 4)) {
        transition->sphere_horizontal_contact = 0;
    }

    if (transition->sphere_center_y > SCREEN_HEIGHT - (sphere_radius + 3) &&
        !transition->sphere_vertical_contact && !transition->sphere_finished_bouncing) {
        transition->sphere_velocity_y = (s16)(-transition->sphere_velocity_y + 2);
        transition->sphere_vertical_contact = 1;
        transition->sphere_spin_y = 1;
    } else if (transition->sphere_center_y <= SCREEN_HEIGHT - (sphere_radius + 3)) {
        transition->sphere_vertical_contact = 0;
    }

    transition->sphere_center_x += (transition->sphere_velocity_x / 4) * step;
    transition->sphere_center_y += (transition->sphere_velocity_y / 4) * step;
    transition->sphere_rotation_x = (s16)(transition->sphere_rotation_x + step * 2);
    if (transition->sphere_spin_y) {
        transition->sphere_rotation_y = (s16)(transition->sphere_rotation_y + step * 4);
    }

    for (s32 row = 0; row <= VIGNETTE_SPHERE_ROWS; ++row) {
        for (s32 column = 0; column <= VIGNETTE_SPHERE_COLUMNS; ++column) {
            vignette_vertex_t sphere_vertex;
            make_vignette_sphere_vertex(row, column,
                                        transition->sphere_rotation_x * two_pi32 / 512.0f,
                                        transition->sphere_rotation_y * two_pi32 / 512.0f,
                                        &sphere_vertex);
            project_vignette_vertex(&sphere_vertex,
                                    (float)transition->sphere_center_x,
                                    (float)transition->sphere_center_y,
                                    512.0f, &vertices[row][column]);
        }
    }
    render_vignette_sphere_cells(vertices);

    /* Once the second right-edge impact disables floor bounces, routine 26404
       forces its counter to the limit after the sphere clears the bottom. */
    if (transition->sphere_finished_bouncing &&
        transition->sphere_center_y >= SCREEN_HEIGHT + sphere_radius + 3) {
        transition->frame = (s16)(transition->frame_count - 1);
    }
}

static void render_vignette_rotating_plane(world_vignette_transition_t* transition, s32 rotation_step) {
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    float angle_z = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    float velocity_z = 0.0f;
    float pivot_x = SCREEN_WIDTH * 0.5f;
    float pivot_y = SCREEN_HEIGHT * 0.5f;
    s32 step = transition->opening ? -4 : 4;
    bool edge_rotation = transition->variant >= 8;
    s32 maximum = edge_rotation ? 132 : 512;

    switch (transition->variant) {
        case 1: velocity_x = (float)(rotation_step * 2); break;
        case 2: velocity_y = (float)rotation_step; break;
        case 3: velocity_z = (float)rotation_step; break;
        case 4: velocity_x = (float)(rotation_step * 2); velocity_y = (float)rotation_step; break;
        case 5: velocity_y = (float)(rotation_step * 2); velocity_z = (float)rotation_step; break;
        case 6: velocity_x = (float)(rotation_step * 2); velocity_z = (float)rotation_step; break;
        case 7:
            velocity_x = (float)(rotation_step * 2);
            velocity_y = (float)(rotation_step * 3);
            velocity_z = (float)rotation_step;
            break;
        case 8:
            angle_x = step > 0 ? 512.0f : 384.0f;
            velocity_x = (float)-step;
            pivot_y = SCREEN_HEIGHT + 1.0f;
            break;
        case 9:
            angle_x = step > 0 ? 0.0f : 128.0f;
            velocity_x = (float)step;
            pivot_y = SCREEN_HEIGHT + 1.0f;
            break;
        case 10:
            angle_x = step > 0 ? 0.0f : 128.0f;
            velocity_x = (float)step;
            pivot_y = -2.0f;
            break;
        case 11:
            angle_x = step > 0 ? 512.0f : 384.0f;
            velocity_x = (float)-step;
            pivot_y = -2.0f;
            break;
        case 12:
            angle_y = step > 0 ? 0.0f : 128.0f;
            velocity_y = (float)step;
            pivot_x = -2.0f;
            break;
        case 13:
            angle_y = step > 0 ? 512.0f : 384.0f;
            velocity_y = (float)-step;
            pivot_x = -2.0f;
            break;
        case 14:
            angle_y = step > 0 ? 512.0f : 384.0f;
            velocity_y = (float)-step;
            pivot_x = SCREEN_WIDTH + 1.0f;
            break;
        case 15:
            angle_y = step > 0 ? 0.0f : 128.0f;
            velocity_y = (float)step;
            pivot_x = SCREEN_WIDTH + 1.0f;
            break;
        default:
            velocity_x = (float)(step * 2);
            break;
    }

    angle_x = (angle_x + velocity_x * transition->frame) * two_pi32 / 512.0f;
    angle_y = (angle_y + velocity_y * transition->frame) * two_pi32 / 512.0f;
    angle_z = (angle_z + velocity_z * transition->frame) * two_pi32 / 512.0f;
    s32 counter = MIN(transition->frame * 4, maximum);
    float camera_distance = 512.0f;
    if (!edge_rotation) {
        camera_distance += 4.0f * (step >= 0 ? counter : maximum - counter);
    }

    const float point_x[6] = {0.0f, 0.0f, SCREEN_WIDTH - 1.0f,
                              SCREEN_WIDTH - 1.0f, SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f};
    const float point_y[6] = {0.0f, SCREEN_HEIGHT - 1.0f, SCREEN_HEIGHT - 1.0f,
                              0.0f, 0.0f, SCREEN_HEIGHT - 1.0f};
    vignette_projected_vertex_t projected[6];
    memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    for (s32 point = 0; point < 6; ++point) {
        vignette_vertex_t vertex = {point_x[point] - pivot_x, point_y[point] - pivot_y, 0.0f,
                                    point_x[point], point_y[point]};
        rotate_vignette_vertex(&vertex, angle_x, angle_y, angle_z);
        project_vignette_vertex(&vertex, pivot_x, pivot_y, camera_distance, &projected[point]);
    }
    draw_vignette_textured_quad(transition, &projected[0], &projected[4], &projected[5], &projected[1]);
    draw_vignette_textured_quad(transition, &projected[4], &projected[3], &projected[2], &projected[5]);
}

static void render_vignette_bouncing_saddle_lens(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    const s32 half_size = 48;
    transition->lens_center_x += transition->lens_velocity_x;
    transition->lens_center_y += transition->lens_velocity_y;
    if (transition->lens_center_x + half_size >= SCREEN_WIDTH) {
        transition->lens_center_x = SCREEN_WIDTH - half_size - 1;
        transition->lens_velocity_x = (s16)-transition->lens_velocity_x;
    } else if (transition->lens_center_x - half_size < 0) {
        transition->lens_center_x = half_size;
        transition->lens_velocity_x = (s16)-transition->lens_velocity_x;
    }
    if (transition->lens_center_y + half_size >= SCREEN_HEIGHT) {
        transition->lens_center_y = SCREEN_HEIGHT - half_size - 1;
        transition->lens_velocity_y = (s16)-transition->lens_velocity_y;
    } else if (transition->lens_center_y - half_size < 0) {
        transition->lens_center_y = half_size;
        transition->lens_velocity_y = (s16)-transition->lens_velocity_y;
    }

    s32 center_x = transition->lens_center_x;
    s32 center_y = transition->lens_center_y;
    const float radius_squared = (float)(half_size * half_size);
    memcpy(transition->output, transition->source, SCREEN_WIDTH * SCREEN_HEIGHT);

    for (s32 y = center_y - half_size; y <= center_y + half_size; ++y) {
        for (s32 x = center_x - half_size; x <= center_x + half_size; ++x) {
            float dx = (float)(x - center_x);
            float dy = (float)(y - center_y);
            float distance_squared = dx * dx + dy * dy;
            /* PC initializer 7C3B0 constructs a saddle-shaped lens field:
                 (-dx, dy) * exp(-4 * distance^2 / radius^2)
               It stores those signed-byte source offsets in a 97x97 table.
               The Gaussian is important: at the table edge its displacement
               has already rounded below one pixel, so the moving effect has
               no visible square boundary. Calculating it directly avoids a
               second allocation while preserving the original field. */
            float falloff = (float)exp(-4.0f * distance_squared / radius_squared);
            s32 source_x = x + (s32)(-dx * falloff);
            s32 source_y = y + (s32)(dy * falloff);
            source_x = MAX(0, MIN(source_x, SCREEN_WIDTH - 1));
            source_y = MAX(0, MIN(source_y, SCREEN_HEIGHT - 1));
            transition->output[y * SCREEN_WIDTH + x] = transition->source[source_y * SCREEN_WIDTH + source_x];
        }
    }
}

static u8 sample_vignette_roll_pixel(const u8* source, s32 x, s32 y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return 0;
    }
    return source[y * SCREEN_WIDTH + x];
}

static void write_vignette_roll_pixel(u8* output, s32 x, s32 y, u8 color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        output[y * SCREEN_WIDTH + x] = color;
    }
}

static void render_vignette_cylindrical_roll(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    const s32 roll_radius = 22;
    const s32 circumference = (s32)(pi32 * roll_radius);
    const s32 half_circumference = (s32)(pi32 * roll_radius * 0.5f);
    const s32 speed = transition->roll_speed;
    const s32 speed_magnitude = abs(speed);

    /* Update the four PC stages from 73614. Closing runs 0->3; opening runs
       the same states backwards. Keeping the stage positions is essential:
       a single linear cylinder eventually samples beyond the source image and
       presents a mostly black apparent back face. */
    switch (transition->roll_stage) {
        case 0: {
            if (!transition->roll_stage_initialized) {
                transition->roll_initial_position = SCREEN_WIDTH - 1;
                if (speed > 0) {
                    transition->roll_position = SCREEN_WIDTH - 1;
                    transition->roll_stage_target =
                        (s16)(SCREEN_WIDTH - 1 - half_circumference);
                } else {
                    transition->roll_stage_target = transition->roll_position;
                }
                transition->roll_stage_initialized = 1;
            }
            s32 distance = transition->roll_initial_position - transition->roll_position;
            transition->roll_arc_start =
                (s16)(roll_radius * sin((double)distance / roll_radius));
            transition->roll_source_offset =
                (s16)(transition->roll_position - half_circumference);
            break;
        }
        case 1: {
            if (!transition->roll_stage_initialized) {
                if (speed > 0) {
                    transition->roll_initial_position = transition->roll_position;
                    transition->roll_stage_target = (s16)roll_radius;
                } else {
                    transition->roll_stage_target = transition->roll_position;
                    transition->roll_initial_position =
                        (s16)(SCREEN_WIDTH - 1 - half_circumference);
                }
                transition->roll_stage_initialized = 1;
            }
            s32 distance = transition->roll_initial_position - transition->roll_position;
            transition->roll_arc_start = distance <= circumference
                ? (s16)(roll_radius * cos((double)distance / roll_radius))
                : (s16)-roll_radius;
            transition->roll_source_offset = (s16)(SCREEN_WIDTH - 1 - distance);
            break;
        }
        case 2:
            if (!transition->roll_stage_initialized) {
                if (speed > 0) {
                    transition->roll_initial_position = transition->roll_position;
                } else {
                    transition->roll_position = (s16)(roll_radius - 1);
                    transition->roll_initial_position = (s16)roll_radius;
                }
                transition->roll_stage_target = (s16)-roll_radius;
                transition->roll_stage_initialized = 1;
            }
            transition->roll_arc_start = (s16)-transition->roll_position;
            transition->roll_source_offset =
                (s16)(transition->roll_position + half_circumference);
            break;
        case 3:
            memset(transition->output, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
            transition->roll_stage_initialized = 0;
            transition->roll_stage = (u8)(transition->roll_stage + (speed > 0 ? 1 : -1));
            return;
        default:
            return;
    }

    if (transition->roll_stage == 0 && speed > 0 &&
        transition->roll_stage_initialized == 1) {
        memcpy(transition->output, transition->source, SCREEN_WIDTH * SCREEN_HEIGHT);
        transition->roll_stage_initialized = 2;
    }

    for (s32 y = 0; y < SCREEN_HEIGHT; ++y) {
        if (transition->roll_stage == 0) {
            for (s32 offset = 0; offset < transition->roll_arc_start; ++offset) {
                s32 table_index = roll_radius - offset;
                s32 source_x = transition->roll_source_offset +
                    transition->roll_arc_lookup[table_index];
                s32 destination_x = transition->roll_position + offset;
                write_vignette_roll_pixel(transition->output, destination_x, y,
                    sample_vignette_roll_pixel(transition->source, source_x, y));
            }
            if (speed > 0) {
                for (s32 offset = transition->roll_arc_start;
                     offset < transition->roll_arc_start + speed_magnitude; ++offset) {
                    write_vignette_roll_pixel(transition->output,
                                               transition->roll_position + offset, y, 0);
                }
            } else {
                for (s32 offset = -1; offset < speed_magnitude - 1; ++offset) {
                    s32 x = transition->roll_position - offset;
                    write_vignette_roll_pixel(transition->output, x, y,
                        sample_vignette_roll_pixel(transition->source, x, y));
                }
            }
        } else if (transition->roll_stage == 1) {
            for (s32 offset = transition->roll_arc_start; offset < roll_radius; ++offset) {
                s32 table_value = transition->roll_arc_lookup[roll_radius + offset];
                s32 destination_x = transition->roll_position + offset;
                s32 source_x = MIN(SCREEN_WIDTH - 1,
                                   transition->roll_source_offset + table_value);
                u8 color = sample_vignette_roll_pixel(transition->source, source_x, y);
                /* The PC treats palette index zero as the hidden cylinder
                   face and substitutes the front/mirrored sample. Omitting
                   this is what made the roll appear predominantly black. */
                if (color == 0) {
                    source_x = offset > 0
                        ? transition->roll_source_offset - table_value - 1
                        : destination_x;
                    color = sample_vignette_roll_pixel(transition->source, source_x, y);
                }
                write_vignette_roll_pixel(transition->output, destination_x, y, color);
            }
            if (speed > 0) {
                for (s32 offset = roll_radius;
                     offset < roll_radius + speed_magnitude; ++offset) {
                    write_vignette_roll_pixel(transition->output,
                                               transition->roll_position + offset, y, 0);
                }
            } else if (roll_radius < transition->roll_position) {
                for (s32 offset = -roll_radius - speed_magnitude;
                     offset < transition->roll_arc_start; ++offset) {
                    s32 x = transition->roll_position + offset;
                    write_vignette_roll_pixel(transition->output, x, y,
                        sample_vignette_roll_pixel(transition->source, x, y));
                }
            }
        } else {
            for (s32 offset = transition->roll_arc_start; offset < roll_radius; ++offset) {
                s32 table_value = transition->roll_arc_lookup[roll_radius + offset];
                s32 destination_x = transition->roll_position + offset;
                s32 source_x = transition->roll_source_offset + table_value;
                u8 color = sample_vignette_roll_pixel(transition->source, source_x, y);
                if (color == 0) {
                    source_x = offset < 0
                        ? transition->roll_source_offset - table_value
                        : destination_x;
                    color = sample_vignette_roll_pixel(transition->source, source_x, y);
                }
                write_vignette_roll_pixel(transition->output, destination_x, y, color);
            }
            if (speed > 0) {
                for (s32 offset = roll_radius;
                     offset < roll_radius + speed_magnitude; ++offset) {
                    write_vignette_roll_pixel(transition->output,
                                               transition->roll_position + offset, y, 0);
                }
            }
        }
    }

    transition->roll_position = (s16)(transition->roll_position - speed);
    if (speed > 0 && transition->roll_position < transition->roll_stage_target) {
        ++transition->roll_stage;
        transition->roll_stage_initialized = 0;
    } else if (speed < 0 &&
               transition->roll_position > transition->roll_initial_position) {
        --transition->roll_stage;
        transition->roll_stage_initialized = 0;
    }
}

static void render_world_vignette_transition(void) {
    world_vignette_transition_t* transition = &world_vignette_transition;

    switch (transition->effect) {
        case VIGNETTE_EFFECT_TILE_GRID:
            render_vignette_tile_grid();
            break;
        case VIGNETTE_EFFECT_SCANLINE_WIPE:
            render_vignette_scanline_wipe();
            break;
        case VIGNETTE_EFFECT_RADIAL_WARP:
            render_vignette_radial_warp();
            break;
        case VIGNETTE_EFFECT_SPHERE_TUMBLE:
            render_vignette_sphere_tumble();
            break;
        case VIGNETTE_EFFECT_ROTATING_PLANE:
            render_vignette_rotating_plane(transition, transition->opening ? -4 : 4);
            break;
        case VIGNETTE_EFFECT_BOUNCING_SADDLE_LENS:
            render_vignette_bouncing_saddle_lens();
            break;
        case VIGNETTE_EFFECT_CYLINDRICAL_ROLL:
            render_vignette_cylindrical_roll();
            break;
        default:
            memcpy(transition->output, transition->source, SCREEN_WIDTH * SCREEN_HEIGHT);
            break;
    }
}

static s16 world_vignette_transition_prg(u32 unused) {
    (void)unused;
    readinput();
    render_world_vignette_transition();
    ++world_vignette_transition.frame;
    if (but0pressed() || but1pressed() || but2pressed() || but3pressed() || TOUCHE(SC_SPACE)) {
        return 1;
    }
    return world_vignette_transition.frame >= world_vignette_transition.frame_count;
}

static s16 world_vignette_transition_pc_frames(s16 effect, s16 variant, bool opening) {
    switch (effect) {
        case VIGNETTE_EFFECT_TILE_GRID:
            /* 0..120, advanced by one per vertical retrace. */
            return 121;
        case VIGNETTE_EFFECT_SCANLINE_WIPE:
            /* The twelve variants use a 100, 200, or 320-pixel extent and
               advance it by two pixels per retrace. */
            if (variant == 9) {
                return 51;
            }
            if (variant == 3 || variant == 7 || variant == 11 || variant == 12) {
                return 161;
            }
            return 101;
        case VIGNETTE_EFFECT_RADIAL_WARP:
            /* The original radial grid counter covers 0..400. */
            return 401;
        case VIGNETTE_EFFECT_SPHERE_TUMBLE:
            /* The inward pass stops after its initial 0..50 stage. The
               outward pass has a second 0..1000 safety limit, but normally
               ends earlier once the bouncing sphere falls below the screen. */
            return opening ? 26 : 527;
        case VIGNETTE_EFFECT_ROTATING_PLANE:
            /* Variants 8..15 use the short 132-unit projection; the first
               seven use the full 512-unit rotation. Both advance by four. */
            return variant >= 8 ? 34 : 129;
        case VIGNETTE_EFFECT_BOUNCING_SADDLE_LENS:
            /* 0..1000 in four-unit steps. */
            return 251;
        case VIGNETTE_EFFECT_CYLINDRICAL_ROLL:
            /* The four PC stages run in reverse while opening. That pass
               skips most of stages 2 and 0 and therefore finishes earlier
               than the complete closing roll. */
            return opening ? 152 : 173;
        default:
            return 1;
    }
}

static u8 init_world_vignette_transition(s32 requested_effect, s32 variant_selector,
                                         u8* pc_display_buffer, u8* draw_buffer_target,
                                         bool opening) {
    world_vignette_transition_t* transition = &world_vignette_transition;
    (void)pc_display_buffer;
    free(transition->source);
    memset(transition, 0, sizeof(*transition));
    if (!draw_buffer_target) {
        return 0;
    }

    s16 effect = (s16)requested_effect;
    if (effect == 9) {
        /* The PC excludes effect 1 while opening, but permits it while closing. */
        effect = opening ? (s16)(myRand(6) + 2) : (s16)(myRand(7) + 1);
    }
    if (effect == 2 || effect == 4) {
        transition->variant = variant_selector == 13 || variant_selector == 100
            ? (s16)(myRand(11) + 1) : (s16)variant_selector;
    } else if (effect == 6) {
        transition->variant = variant_selector == 16 || variant_selector == 100
            ? (s16)(myRand(14) + 1) : (s16)variant_selector;
    }
    if (effect == 4) {
        effect = VIGNETTE_EFFECT_SCANLINE_WIPE;
    }

    transition->source = (u8*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
    if (!transition->source) {
        return 0;
    }

    /* The original PC effects use the VGA display buffer as their source on
       the closing pass. Rayverse presents DrawBufferNormal directly and does
       not mirror that buffer in SWAP_BUFFERS(), so the current draw target is
       the equivalent stable image to capture here. */
    memcpy(transition->source, draw_buffer_target, SCREEN_WIDTH * SCREEN_HEIGHT);
    transition->output = draw_buffer_target;
    transition->effect = effect;
    transition->opening = opening;
    /* The PC advances these counters once per selected video retrace. It uses
       60 Hz by default and 70 Hz only when configured; Rayverse's frame limiter
       follows that same setting, so retaining the callback count also retains
       the original duration. */
    transition->frame_count = world_vignette_transition_pc_frames(
        effect, transition->variant, opening);
    init_vignette_effect_state(transition);
    transition->active = 1;
    return 1;
}

//38208
u8 InitWorldVignetteTransitionIn(s32 effect, s32 variant_selector, u8* display, u8* draw) {
    return init_world_vignette_transition(effect, variant_selector, display, draw, true);
}

//38220
u8 InitWorldVignetteTransitionOut(s32 effect, s32 variant_selector, u8* display, u8* draw) {
    return init_world_vignette_transition(effect, variant_selector, display, draw, false);
}

//38258
void RunWorldVignetteTransition(void) {
    if (!world_vignette_transition.active) {
        return;
    }
    SAVE_PALETTE(&rvb_plan3);
    if (world_vignette_transition.opening) {
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(world_vignette_transition_prg);
    if (!world_vignette_transition.opening) {
        DO_FADE_OUT();
    }
    RESTORE_PALETTE();
}

//38290
void FreeWorldVignetteTransition(void) {
    free(world_vignette_transition.source);
    memset(&world_vignette_transition, 0, sizeof(world_vignette_transition));
}

//382AC
void SaveScreen(const u8* source, u8* dest) {
    // Rayverse's buffers are linear, including the fallback for PC Mode X.
    if (source != NULL && dest != NULL) {
        memmove(dest, source, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
}

//38320
void RestoreScreen(const u8* source, u8* dest) {
    SaveScreen(source, dest);
}

//3839C
void DO_AFFICHE_PAUSE(void) {
    s32 xmin, xmax, ymin, ymax;
    get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
    sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    display_text(language_txt[160], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 1);
    sprite_clipping(xmin, xmax, ymin, ymax);
}

//38400
s16 dummy_prg(u32 a1) {
    (void)a1;
    return 1;
}

typedef struct pause_effect_t {
    world_vignette_transition_t plane;
    // Portable renderer/callback state; recovered PC globals live in data.c.
    s16 simple_frames;
    s16 simple_shift_frames;
    bool waiting_for_shift;
} pause_effect_t;

static pause_effect_t pause_effect;

static bool pause_shifts_held(void) {
    return TOUCHE(SC_LSHIFT) && TOUCHE(SC_RSHIFT);
}

static void restore_pause_picture(void) {
    RestoreScreen(pause_effect.plane.source, pause_effect.plane.output);
}

static s16 pause_simple_prg(u32 timer) {
    (void)timer;
    if (FondAutorise == 2) {
        // The original FondAutorise == 2 fallback waits twelve retraces first.
        if (pause_effect.simple_frames > 0) {
            --pause_effect.simple_frames;
            return 0;
        }
        return !in_pause;
    }
    if (!in_pause) {
        restore_pause_picture();
        return 1;
    }
    if (pause_shifts_held()) {
        ++pause_effect.simple_shift_frames;
    }
    --pause_effect.simple_frames;
    if (pause_effect.simple_frames == 0 || pause_effect.simple_shift_frames > 7) {
        if (pause_effect.simple_shift_frames != 0) {
            effect_pause_bloque ^= 1;
            restore_pause_picture();
            if (!effect_pause_bloque) {
                DO_AFFICHE_PAUSE();
            }
        }
        pause_effect.simple_frames = 15;
        pause_effect.simple_shift_frames = 0;
        PC_do_cheats();
    }
    return 0;
}

//38408
void Do_Effect_Pause_Simple(void) {
    restore_pause_picture();
    DO_AFFICHE_PAUSE();
    pause_effect.simple_frames = FondAutorise == 2 ? 12 : 15;
    pause_effect.simple_shift_frames = 0;
    SYNCHRO_LOOP(pause_simple_prg);
    restore_pause_picture();
}

static void build_pause_plane(void) {
    pause_effect.plane.variant = ChooseRandomPausePlaneVariant();
    pause_effect.plane.frame = 0;
    /* PC BuildWorldVignetteRotatingPlane negates effect_speed internally.
     * Negative input therefore recedes; positive input approaches the viewer.
     * Rotation velocities for variants 1..7 are positive on both passes. */
    pause_effect.plane.opening = effect_speed > 0;
}

static void advance_pause_plane(void) {
    render_vignette_rotating_plane(&pause_effect.plane, 4);
    ++pause_effect.plane.frame;
    // PC Update renders counters 0,4,...,512 and then returns zero.
    pause_counter = pause_effect.plane.frame <= 128 ? pause_effect.plane.frame * 4 : 0;
}

//38574
s16 pause_prg(u32 timer) {
    (void)timer;
    PC_do_cheats();
    if (!in_pause) {
        restore_pause_picture();
        return 1;
    }
    if (pause_intro_frames < 7 * VGA_FREQ) {
        ++pause_intro_frames;
        restore_pause_picture();
        if (pause_shifts_held()) {
            if (++pause_counter >= 5) {
                effect_pause_bloque ^= 1;
                pause_counter = 0;
            }
        } else {
            pause_counter = 0;
        }
        if (!effect_pause_bloque) {
            DO_AFFICHE_PAUSE();
        }
        return 0;
    }
    if (pause_intro_frames == 7 * VGA_FREQ) {
        ++pause_intro_frames;
        pause_delay_frames = 0;
        pause_counter = 0;
        effect_pause_bloque = 0;
    }

    if (pause_effect.waiting_for_shift) {
        // Replace the original busy wait with one event-pumping frame per tick.
        if (pause_shifts_held()) {
            pause_effect.waiting_for_shift = false;
            effect_pause_bloque = 0;
            pause_delay_frames = 0;
            pause_counter = 0;
            effect_speed = -4;
            build_pause_plane();
        }
        return 0;
    }

    if (!effect_pause_bloque) {
        if (pause_counter >= 20 && pause_shifts_held()) {
            effect_pause_bloque = 1;
        }
        // PC finishes this unblocked branch even when Shift sets the latch.
        if (pause_delay_frames != 0) {
            --pause_delay_frames;
            return 0;
        }
        advance_pause_plane();
        DO_AFFICHE_PAUSE();
        if (pause_counter == 0) {
            effect_speed = -effect_speed;
            build_pause_plane();
            if (effect_speed < 0) {
                restore_pause_picture();
                DO_AFFICHE_PAUSE();
                pause_delay_frames = VGA_FREQ;
            }
        }
    } else if (pause_counter != 0 || effect_speed >= 0) {
        // Finish returning the plane to its full-size image, without text.
        advance_pause_plane();
        if (pause_counter == 0) {
            effect_speed = -effect_speed;
            build_pause_plane();
        }
    } else {
        restore_pause_picture();
        pause_effect.waiting_for_shift = true;
    }
    return 0;
}

//38924
void Do_Effect_Pause(void) {
    if (draw_buffer == NULL) {
        in_pause = 0;
        MapTimePause = 0;
        return;
    }
    memset(&pause_effect, 0, sizeof(pause_effect));
    pause_intro_frames = 0;
    pause_delay_frames = 0;
    pause_counter = 0;
    effect_pause_bloque = 0;
    const bool freeze_sound = CarteSonAutorisee != 0;
    const bool resume_music = MusicCdActive && is_ogg_playing;
    if (freeze_sound) {
        start_freeze_snd();
    }
    /* The PC pauses the CD and offsets its DOS playback timer on return.
     * Holding the OGG decoder in place does both for our sample-based backend;
     * stop_cd() would instead re-arm level music and restart the track. */
    if (resume_music) {
        is_ogg_playing = false;
    }

    u8* saved_screen = (u8*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
    pause_effect.plane.source = saved_screen != NULL ? saved_screen : EffetBufferNormal;
    pause_effect.plane.output = draw_buffer;
    // Rayverse presents draw_buffer directly; the DOS display buffer is stale.
    SaveScreen(draw_buffer, pause_effect.plane.source);
    // Android keeps only the simple pause. PC normal mode animates the scene;
    // Mode X and failed effect allocation use the static fallback.
    if (GameModeVideo == MODE_NORMAL && saved_screen != NULL) {
        effect_speed = -4;
        build_pause_plane();
        SYNCHRO_LOOP(pause_prg);
    } else {
        Do_Effect_Pause_Simple();
    }
    restore_pause_picture();
    free(saved_screen);
    memset(&pause_effect, 0, sizeof(pause_effect));
    pause_intro_frames = 0;
    pause_delay_frames = 0;
    pause_counter = 0;

    if (resume_music) {
        is_ogg_playing = true;
    }
    if (freeze_sound) {
        stop_freeze_snd();
    }
    in_pause = 0;
    MapTimePause = 0;
}

//38A30
void sub_38A30(void) {
    print_once("Not implemented: sub_38A30"); //stub
}

//38B58
void sub_38B58(void) {
    print_once("Not implemented: sub_38B58"); //stub
}

//38DA0
void sub_38DA0(void) {
    print_once("Not implemented: sub_38DA0"); //stub
}

//38DCC
void sub_38DCC(void) {
    print_once("Not implemented: sub_38DCC"); //stub
}

//38ED4
void sub_38ED4(void) {
    print_once("Not implemented: sub_38ED4"); //stub
}

//391C0
void sub_391C0(void) {
    print_once("Not implemented: sub_391C0"); //stub
}
