#include "sim.h"
#include "tilemap.h"
#include <stdalign.h>

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

void sim_update(void) {

}