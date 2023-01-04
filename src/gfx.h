#pragma once
#include <GL/glew.h>
#include <cglm/struct/vec3.h>
#include <bits/types/error_t.h>
#include <assert.h>

// TODO: Expose pixel_colors

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

_Static_assert(sizeof(color_t) == 3, "color_t must be 3 bytes");

error_t gfx_init(void);
void gfx_update(void);

void set_tile_to_color(size_t index, color_t color);