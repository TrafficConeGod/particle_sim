#include "sim.h"
#include "tilemap.h"
#include "tile_type.h"
#include "gfx.h"
#include "rand_range.h"
#include "util.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#define GRAVITY 0.01f

static tile_type_t current_tile_type = tile_type_sand;
_Alignas(64) static tile_type_t tile_types_0[NUM_TILES] = {0};
_Alignas(64) static tile_type_t tile_types_1[NUM_TILES] = {0};

static const tile_type_t* in_tile_types = tile_types_0;
static tile_type_t* out_tile_types = tile_types_1;

static bool catch_index(size_t index) {
    return index >= NUM_TILES;
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

static color_t get_tile_color(tile_type_t tile_type) {
    switch (tile_type) {
        default: return (color_t){ 255, 0, 0 };
        case tile_type_air: return (color_t){ 0, 0, 0 };
        case tile_type_water: return (color_t){ 66, 135, 245 };
        case tile_type_sand: return (color_t){ 194, 154, 52 };
        case tile_type_stone: return (color_t){ 76, 79, 79 };
        case tile_type_acid: return (color_t){ 255, 0, 255 };
    }
}

static void handle_water(size_t index) {
    // Move down by 1
    size_t move_to = index - TILEMAP_WIDTH;
    // Check if we are in bounds or if we are colliding with something else
    if (catch_index(move_to) || in_tile_types[move_to] != tile_type_air) {
        int dir = (rand() % 2 ? -1 : 1);
        // Move left or right randomly
        move_to = index + dir;
        // Check if we are in bounds or if we are colliding with something else
        if (catch_index(move_to) || in_tile_types[move_to] != tile_type_air) {
            move_to = index - dir;
            // Check if we are in bounds
            if (catch_index(move_to)) {
                return;
            }
            // Check if we are colliding with something else
            if (in_tile_types[move_to] != tile_type_air) {
                return;
            }
        }
    }
    // Tile movement
    out_tile_types[index] = tile_type_air;
    out_tile_types[move_to] = tile_type_water;
    pixel_colors[index] = get_tile_color(tile_type_air);
    pixel_colors[move_to] = get_tile_color(tile_type_water);
}

static void handle_sand(size_t index) {
    // Move down by 1
    size_t move_to = index - TILEMAP_WIDTH;
    // Check if we are in bounds
    if (catch_index(move_to)) {
        return;
    }
    // Check if we are colliding with something else
    if (in_tile_types[move_to] > tile_type_water) {
        // Move left or right randomly
        move_to += rand() % 2 ? -1 : 1;
        // Check if we are in bounds
        if (catch_index(move_to)) {
            return;
        }
        // Check if we are colliding with something else
        if (in_tile_types[move_to] > tile_type_air) {
            return;
        }
    }
    // Tile movement
    color_t replaced_color = pixel_colors[move_to];

    out_tile_types[index] = in_tile_types[move_to];
    out_tile_types[move_to] = tile_type_sand;
    pixel_colors[index] = replaced_color;
    pixel_colors[move_to] = get_tile_color(tile_type_sand);
}

static void spread_acid(size_t index) {
    // Check if we are in bounds
    if (catch_index(index)) {
        return;
    }
    // Check if we are in air
    if (in_tile_types[index] == tile_type_air) {
        return;
    }
    // Set the tile
    out_tile_types[index] = tile_type_acid;
    pixel_colors[index] = get_tile_color(tile_type_acid);
}

static void handle_acid(size_t index) {
    // Spread the acid by random chance
    if (rand() % 2) {
        spread_acid(index + 1);
        spread_acid(index - 1);
        spread_acid(index + TILEMAP_WIDTH);
        spread_acid(index - TILEMAP_WIDTH);
    }

    // Delete the tile
    out_tile_types[index] = tile_type_air;
    pixel_colors[index] = get_tile_color(tile_type_air);
}

void sim_update(float width_norm_factor, float height_norm_factor, GLFWwindow* win, UNUSED size_t tick_count) {
    memcpy(out_tile_types, in_tile_types, sizeof(tile_types_0));
    for (size_t i = 0; i < NUM_TILES; i++) {
        pixel_colors[i] = get_tile_color(in_tile_types[i]);
    }

    for (size_t i = 0; i < NUM_TILES; i++) {
        switch (in_tile_types[i]) {
            default: break;
            case tile_type_water: handle_water(i); break;
            case tile_type_sand: handle_sand(i); break;
            case tile_type_acid: handle_acid(i); break;
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
            size_t index = (cursor_x_floor + rand_range(0, 6) - 3) + ((cursor_y_floor + rand_range(0, 6) - 3) * TILEMAP_WIDTH);
            // Check if we are in bounds
            if (catch_index(index)) {
                continue;
            }
            // Spawn current tile type if we press mouse button 1, delete tile if we press mouse button 2
            tile_type_t tile_type_to_place = mouse_button_1_pressed ? current_tile_type : tile_type_air;
            out_tile_types[index] = tile_type_to_place;
            pixel_colors[index] = get_tile_color(tile_type_to_place);
        }
    }

    // Swap our tile type buffers
    tile_type_t* temp = (tile_type_t*)in_tile_types;
    in_tile_types = out_tile_types;
    out_tile_types = temp;
}