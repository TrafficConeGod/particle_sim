#include "gfx.h"
#include "util.h"
#include "tilemap.h"

#define NUM_TILE_QUAD_VERTS 6

typedef struct pos_quad {
    vec2s verts[NUM_TILE_QUAD_VERTS];
} pos_quad_t;

typedef struct color_quad {
    vec3s verts[NUM_TILE_QUAD_VERTS];
} color_quad_t;

// Stored in row major order

_Alignas(64) static pos_quad_t pos_quads[NUM_TILES];
_Alignas(64) static color_quad_t color_quads[NUM_TILES] = {0};

static union {
    GLuint data[2];
    struct PACKED {
        GLuint pos;
        GLuint color;
    };
} buffers;

static vec2s convert_normalization(vec2s vec) {
    return (vec2s){{ (2.0f * vec.x) - 1.0f, (2.0f * vec.y) - 1.0f }};
}

static void fill_quad_arrays(void) {
    size_t i = 0;

    float x_step = 2.0f * (1.0f / (float)TILEMAP_WIDTH);
    float y_step = 2.0f * (1.0f / (float)TILEMAP_HEIGHT);

    for (size_t y = 0; y < TILEMAP_HEIGHT; y++) {
        for (size_t x = 0; x < TILEMAP_WIDTH; x++) {
            vec2s tile_p0 = convert_normalization((vec2s){{ x / (float)TILEMAP_WIDTH, y / (float)TILEMAP_HEIGHT }});
            vec2s tile_p1 = {{ tile_p0.x + x_step, tile_p0.y + y_step }};

            pos_quads[i] = (pos_quad_t){{
                {{ tile_p0.x, tile_p0.y }},
                {{ tile_p1.x, tile_p0.y }},
                {{ tile_p0.x, tile_p1.y }},
                //
                {{ tile_p1.x, tile_p1.y }},
                {{ tile_p0.x, tile_p1.y }},
                {{ tile_p1.x, tile_p0.y }}
            }};

            i++;
        }
    }
}

void gfx_init(void) {
    fill_quad_arrays();

    glGenBuffers(SIZEOF_ARRAY(buffers.data), buffers.data);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_quads), pos_quads, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.color);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(color_quads), NULL, GL_DYNAMIC_STORAGE_BIT);
}

void gfx_update(void) {
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_quads), color_quads);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, NUM_TILES * NUM_TILE_QUAD_VERTS);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void set_tile_to_color(size_t index, vec3s color) {
    color_quads[index] = (color_quad_t){{
        color,
        color,
        color,
        //
        color,
        color,
        color
    }};
}