#include "sim.h"
#include "tilemap.h"
#include "tile_type.h"
#include "gfx.h"
#include "rand_range.h"
#include "util.h"
#include "bitfield.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define GRAVITY 0.01f

// How the bit value at index 7 of a tile type indicates its processed status, if this bool is true then a bit 1 at 7 means processed, otherwise it means that a bit 0 at 7 means processed
static bool is_tile_processed_flag_1 = true;

static bitfield_t get_processed_tile_type_bitfield(bitfield_t bitfield) {
    if (is_tile_processed_flag_1) {
        return set_bit(bitfield, 7);
    }
    return clear_bit(bitfield, 7);
}

static bitfield_t toggle_processed_flag(bitfield_t bitfield) {
    return toggle_bit(bitfield, 7);
}

static bitfield_t get_tile_type(bitfield_t bitfield) {
    return clear_bit(bitfield, 7);
}

static bool is_tile_processed(bitfield_t bitfield) {
    return 
        (is_bit_set(bitfield, 7) && is_tile_processed_flag_1) || // If is_tile_processed_flag_1 is true, then check if the flag is 1
        (!is_bit_set(bitfield, 7) && !is_tile_processed_flag_1); // If is_tile_processed_flag_1 is false, then check if the flag is 0
}

static tile_type_t current_tile_type = tile_type_sand;

static bool catch_index(size_t index) {
    return index >= NUM_TILES;
}

static bool catch_horizontal_move(size_t index, int dir) {
    // This will also catch out of bounds as well
    return (int)((index % TILEMAP_WIDTH) - ((index - dir) % TILEMAP_WIDTH)) != dir;
}

static void key_callback(UNUSED GLFWwindow* _0, int key, UNUSED int _1, UNUSED int _2, UNUSED int _3) {
    tile_type_t tile_type = (key - GLFW_KEY_1) + 1;
    if (tile_type > MAX_TILE_TYPE) {
        return;
    }
    current_tile_type = tile_type;
}

void sim_init(GLFWwindow* win) {
    glfwSetKeyCallback(win, key_callback);
}

_Alignas(64) static color_t sand_colors[] = {
    { 176, 133, 23 },
    { 194, 154, 52 },
    { 209, 178, 100 },
    { 217, 198, 150 }
};

_Alignas(64) static color_t stone_colors[] = {
    { 76, 79, 79 },
    { 76, 84, 84 },
    { 101, 107, 107 },
    { 134, 145, 145 }
};

static color_t get_tile_color(tile_type_t tile_type) {
    switch (tile_type) {
        default: return (color_t){ 255, 0, 0 };
        case tile_type_air: return (color_t){ 0, 0, 0 };
        case tile_type_water: return (color_t){ 66, 135, 245 };
        case tile_type_sand: return sand_colors[rand_range(0, SIZEOF_ARRAY(sand_colors) - 1)];
        case tile_type_stone: return stone_colors[rand_range(0, SIZEOF_ARRAY(stone_colors) - 1)];
        case tile_type_acid: return (color_t){ 255, 0, 255 };
    }
}

static void handle_water(size_t index, bitfield_t tile_type_bitfield) {
    if (is_tile_processed(tile_type_bitfield)) {
        return;
    }
    // Move down by 1
    size_t move_to = index - TILEMAP_WIDTH;
    // Check if we are in bounds or if we are colliding with something else
    if (catch_index(move_to) || get_tile_type(tiles[move_to].type) != tile_type_air) {
        int dir = (rand() % 2 ? -1 : 1);
        // Move left or right randomly
        move_to = index + dir;
        // Check if we are in bounds or if we are colliding with something else
        if (catch_horizontal_move(move_to, dir) || get_tile_type(tiles[move_to].type) != tile_type_air) {
            tiles[index].type = toggle_processed_flag(tile_type_bitfield);
            return;
        }
    }
    // Tile movement
    tiles[move_to] = tiles[index];
    tiles[move_to].type = toggle_processed_flag(tile_type_bitfield);
    tiles[index].color = get_tile_color(tile_type_air);
    tiles[index].type = tile_type_air;
}

static void handle_sand(size_t index, bitfield_t tile_type_bitfield) {
    if (is_tile_processed(tile_type_bitfield)) {
        return;
    }
    // Move down by 1
    size_t move_to = index - TILEMAP_WIDTH;
    // Check if we are in bounds
    if (catch_index(move_to)) {
        tiles[index].type = toggle_processed_flag(tile_type_bitfield);
        return;
    }
    // Check if we are colliding with something else
    if (get_tile_type(tiles[move_to].type) > tile_type_water) {
        int dir = rand() % 2 ? -1 : 1;
        // Move left or right randomly
        move_to += dir;
        // Check if we are in bounds or if we are colliding with something else
        if (catch_horizontal_move(move_to, dir) || get_tile_type(tiles[move_to].type) > tile_type_water) {
            tiles[index].type = toggle_processed_flag(tile_type_bitfield);
            return;
        }
    }
    // Tile movement
    tile_t replaced_tile = tiles[move_to];
    replaced_tile.type = get_processed_tile_type_bitfield(replaced_tile.type);

    tiles[move_to] = tiles[index];
    tiles[move_to].type = toggle_processed_flag(tile_type_bitfield);

    tiles[index] = replaced_tile;
}

static void spread_acid(size_t index, bitfield_t tile_type_bitfield) {
    // // Check if we are in bounds
    // if (catch_index(index)) {
    //     return;
    // }
    // // Check if we are in air
    // if (get_tile_type(tile_types[index]) == tile_type_air) {
    //     return;
    // }
    // // Set the tile
    // tile_types[index] = tile_type_bitfield;
    // pixel_colors[index] = get_tile_color(tile_type_acid);
}

static void handle_acid(size_t index, bitfield_t tile_type_bitfield) {
    // if (is_tile_processed(tile_type_bitfield)) {
    //     return;
    // }
    // tile_type_bitfield = toggle_processed_flag(tile_type_bitfield);
    // // Spread the acid by random chance
    // if (rand() % 2) {
    //     spread_acid(index + 1, tile_type_bitfield);
    //     spread_acid(index - 1, tile_type_bitfield);
    //     spread_acid(index + TILEMAP_WIDTH, tile_type_bitfield);
    //     spread_acid(index - TILEMAP_WIDTH, tile_type_bitfield);
    // }

    // // Delete the tile
    // tile_types[index] = tile_type_air;
    // pixel_colors[index] = get_tile_color(tile_type_air);
}

void sim_update(float width_norm_factor, float height_norm_factor, GLFWwindow* win, UNUSED size_t tick_count) {
    // for (size_t i = 0; i < NUM_TILES; i++) {
    //     if (get_tile_type(tile_types[i]) != tile_type_air && is_tile_processed(tile_types[i])) {
    //         printf("%d, %ld\n", get_tile_type(tile_types[i]), i);
    //     }
    // }

    for (size_t i = 0; i < NUM_TILES; i++) {
        bitfield_t tile_type_bitfield = tiles[i].type;
        switch (get_tile_type(tile_type_bitfield)) {
            default: break;
            case tile_type_water: handle_water(i, tile_type_bitfield); break;
            case tile_type_sand: handle_sand(i, tile_type_bitfield); break;
            case tile_type_acid: handle_acid(i, tile_type_bitfield); break;
        }
    }

    bool mouse_button_1_pressed = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
    bool mouse_button_2_pressed = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
    if (mouse_button_1_pressed || mouse_button_2_pressed) {
        double cursor_x;
        double cursor_y;
        glfwGetCursorPos(win, &cursor_x, &cursor_y);
        vec2s cursor_pos = {{ (float)cursor_x * width_norm_factor * (float)TILEMAP_WIDTH, (1.0f - ((float)cursor_y * height_norm_factor)) * (float)TILEMAP_HEIGHT }};
        size_t cursor_x_floor = cursor_pos.x;
        size_t cursor_y_floor = cursor_pos.y;
        // Place at most 5 tiles
        for (size_t i = 0; i < 5; i++) {
            // Randomized position
            size_t index = ((cursor_y_floor + rand_range(0, 6) - 3) * TILEMAP_WIDTH);
            // Check if we are in bounds
            if (catch_index(index)) {
                continue;
            }
            int dir = (cursor_x_floor + rand_range(0, 6) - 3);
            index += dir;
            // Check if we are in bounds horizontally
            if (catch_horizontal_move(index, dir)) {
                continue;
            }

            // Spawn current tile type if we press mouse button 1, delete tile if we press mouse button 2
            tile_type_t tile_type_to_place = mouse_button_1_pressed ? current_tile_type : tile_type_air;
            tiles[index].color = get_tile_color(tile_type_to_place);
            tiles[index].type = get_processed_tile_type_bitfield(tile_type_to_place);
        }
    }

    is_tile_processed_flag_1 = !is_tile_processed_flag_1;
}