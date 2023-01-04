#include "sim.h"
#include "tilemap.h"
#include "particle.h"
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

bool catch_index(size_t index) {
    return index >= NUM_TILES;
}

void sim_init(void) {
    for (size_t i = TILEMAP_WIDTH * (TILEMAP_HEIGHT - 3); i < TILEMAP_WIDTH * (TILEMAP_HEIGHT - 2); i++) {
        tile_types[i] = tile_type_sand;
        pixel_colors[i] = (color_t){ 194, 154, 52 };
    }
}

void handle_sand(size_t index) {
    size_t move_to = index - TILEMAP_WIDTH;
    if (catch_index(move_to)) {
        return;
    }
    if (tile_types[move_to] != tile_type_air) {
        return;
    }
    tile_types[index] = tile_type_air;
    tile_types[move_to] = tile_type_sand;
    pixel_colors[index] = (color_t){ 0, 0, 0 };
    pixel_colors[move_to] = (color_t){ 194, 154, 52 };
}

void sim_update(void) {
    for (size_t i = 0; i < NUM_TILES; i++) {
        switch (tile_types[i]) {
            default: break;
            case tile_type_sand:
                handle_sand(i);
                break;
        }
    }
}