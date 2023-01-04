#pragma once
#include <assert.h>

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