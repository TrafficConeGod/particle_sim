#pragma once
#include <GL/glew.h>
#include <cglm/struct/vec3.h>

void gfx_init(void);
void gfx_update(void);

void set_tile_to_color(size_t index, vec3s color);