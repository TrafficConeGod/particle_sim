#pragma once
#include "tilemap.h"
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef struct sand_particle {
    short_tile_index_t index;
    short_tile_height_t y_velocity;
} sand_particle_t;

_Static_assert(sizeof(sand_particle_t) == 4, "sand_particle_t must be 4 bytes");