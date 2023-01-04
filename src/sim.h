#pragma once
#include <stddef.h>

typedef struct GLFWwindow GLFWwindow;

void sim_init(GLFWwindow* win);
void sim_update(float width_norm_factor, float height_norm_factor, GLFWwindow* win, size_t tick_count);