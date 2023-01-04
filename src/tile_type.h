#pragma once
#include <assert.h>

#pragma GCC push_options
#pragma GCC optimize ("short-enums")

// TODO: Add bitfield flag at the end of the enum to check if it has been processed yet

typedef enum tile_type {
    tile_type_air,
    tile_type_water,
    tile_type_sand,
    tile_type_stone,
    tile_type_spreadable_acid,
    tile_type_acid
} tile_type_t;

#define MAX_TILE_TYPE tile_type_spreadable_acid

#ifndef __INTELLISENSE__
_Static_assert(sizeof(tile_type_t) == 1, "tile_type_t must be 1 byte");
#endif

#pragma GCC pop_options