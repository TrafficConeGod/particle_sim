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
#include <GLFW/glfw3.h>

#define GRAVITY 0.01f

_Alignas(64) static tile_type_t tile_types[NUM_TILES] = {0};

bool catch_index(size_t index) {
    return index >= NUM_TILES;
}

void sim_init(void) {
}

void handle_sand(size_t index) {
    // Move down by 1
    size_t move_to = index - TILEMAP_WIDTH;
    // Check if we are in bounds
    if (catch_index(move_to)) {
        return;
    }
    // Check if we are colliding with something else
    if (tile_types[move_to] != tile_type_air) {
        // Move left or right randomly
        move_to += rand() % 2 ? -1 : 1;
        // Check if we are in bounds
        if (catch_index(move_to)) {
            return;
        }
        // Check if we are colliding with something else
        if (tile_types[move_to] != tile_type_air) {
            return;
        }
    }
    tile_types[index] = tile_type_air;
    tile_types[move_to] = tile_type_sand;
    pixel_colors[index] = (color_t){ 0, 0, 0 };
    pixel_colors[move_to] = (color_t){ 194, 154, 52 };
}

void sim_update(float width_norm_factor, float height_norm_factor, GLFWwindow* win, UNUSED size_t tick_count) {
    for (size_t i = 0; i < NUM_TILES; i++) {
        switch (tile_types[i]) {
            default: break;
            case tile_type_sand:
                handle_sand(i);
                break;
        }
    }

    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        double cursor_x;
        double cursor_y;
        glfwGetCursorPos(win, &cursor_x, &cursor_y);
        vec2s cursor_pos = {{ (float)cursor_x * width_norm_factor * (float)TILEMAP_WIDTH, (1.0f - ((float)cursor_y * height_norm_factor)) * (float)TILEMAP_HEIGHT }};
        size_t cursor_x_floor = cursor_pos.x;
        size_t cursor_y_floor = cursor_pos.y;
        cursor_x_floor += rand_range(0, 6) - 3;
        cursor_y_floor += rand_range(0, 6) - 3;
        size_t index = cursor_x_floor + (cursor_y_floor * TILEMAP_WIDTH);
        // Check if we are in bounds
        if (catch_index(index)) {
            return;
        }
        // Spawn sand
        tile_types[index] = tile_type_sand;
        pixel_colors[index] = (color_t){ 194, 154, 52 };
    }
}