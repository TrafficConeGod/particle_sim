#pragma once
#include "tilemap.h"
#include "bitfield.h"
#include <GL/glew.h>
#include <cglm/struct/vec3.h>
#include <bits/types/error_t.h>
#include <assert.h>

// TODO: Figure out a way to make sure pixel copy ops avoid >1 cache miss

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

_Static_assert(sizeof(color_t) == 3, "color_t must be 3 bytes");

typedef struct tile {
    color_t color;
    bitfield_t type;
} tile_t;

_Static_assert(sizeof(tile_t) == 4, "tile_t must be 4 bytes");

extern tile_t tiles[NUM_TILES];

error_t gfx_init(void);
void gfx_update(void);