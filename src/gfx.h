#pragma once
#include "tilemap.h"
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

extern color_t pixel_colors[NUM_TILES];

error_t gfx_init(void);
void gfx_update(void);