#include "sim.h"
#include "tilemap.h"
#include "gfx.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>

#pragma GCC push_options
#pragma GCC optimize ("short-enums")

typedef enum tile_type {
    tile_type_air,
    tile_type_sand
} tile_type_t;

#ifndef __INTELLISENSE__
_Static_assert(sizeof(tile_type_t) == 1, "tile_type_t must be 1 byte");
#endif

#pragma GCC pop_options

_Alignas(64) static tile_type_t tile_types[NUM_TILES] = {0};

void sim_init(void) {

}

static size_t x = 0;
static size_t y = 0;
static size_t index = 0;

void sim_update(void) {
    if (x < TILEMAP_WIDTH) {
        tile_types[index] = tile_type_sand;
        set_tile_to_color(index, (vec3s){{ 194.0f/255.0f, 154.0f/255.0f, 52.0f/255.0f }});

        x++;
        index++;
    } else if (y < TILEMAP_HEIGHT) {
        x = 0;
        y++;
    }
}