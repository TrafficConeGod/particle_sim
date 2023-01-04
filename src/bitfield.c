#include "bitfield.h"

bitfield_t set_bit(bitfield_t bitfield, bitfield_t index) {
    return bitfield | (1 << index);
}

bitfield_t clear_bit(bitfield_t bitfield, bitfield_t index) {
    return bitfield & ~(1 << index);
}

bitfield_t toggle_bit(bitfield_t bitfield, bitfield_t index) {
    return bitfield ^ (1 << index);
}

bool is_bit_set(bitfield_t bitfield, bitfield_t index) {
    return bitfield & (1 << index);
}