#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t bitfield_t;

bitfield_t set_bit(bitfield_t bitfield, bitfield_t index);
bitfield_t clear_bit(bitfield_t bitfield, bitfield_t index);
bitfield_t toggle_bit(bitfield_t bitfield, bitfield_t index);
bool is_bit_set(bitfield_t bitfield, bitfield_t index);