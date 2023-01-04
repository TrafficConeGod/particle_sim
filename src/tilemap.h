#pragma once
#include <stdint.h>

#define TILEMAP_WIDTH 160
#define TILEMAP_HEIGHT 90
#define NUM_TILES (TILEMAP_WIDTH * TILEMAP_HEIGHT)

_Static_assert(NUM_TILES <= UINT16_MAX, "There must be less tiles than 65535");
_Static_assert(TILEMAP_HEIGHT <= UINT8_MAX, "The tilemap height must be less than 255");

typedef uint16_t short_tile_index_t;
typedef uint8_t short_tile_height_t;