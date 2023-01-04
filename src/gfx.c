#include "gfx.h"
#include "util.h"
#include "tilemap.h"
// Stored in row major order

_Alignas(64) static vec2s pos_verts[] = {
    {{ -1, -1 }},
    {{ 1, -1 }},
    {{ -1, 1 }},
    //
    {{ 1, 1 }},
    {{ -1, 1 }},
    {{ 1, -1 }}
};
_Alignas(64) static vec2s uv_verts[] = {
    {{ 0, 0 }},
    {{ 1, 0 }},
    {{ 0, 1 }},
    //
    {{ 1, 1 }},
    {{ 0, 1 }},
    {{ 1, 0 }}
};

static union {
    GLuint data[2];
    struct PACKED {
        GLuint pos;
        GLuint uv;
    };
} buffers;

void gfx_init(void) {
    glGenBuffers(SIZEOF_ARRAY(buffers.data), buffers.data);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_verts), pos_verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_verts), uv_verts, GL_STATIC_DRAW);
}

void gfx_update(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.uv);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(pos_verts));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void set_tile_to_color(UNUSED size_t index, UNUSED vec3s color) {
}