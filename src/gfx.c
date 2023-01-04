#include "gfx.h"
#include "util.h"
#include <cglm/struct/vec2.h>
#include <stdalign.h>

_Alignas(64) static vec2s vert_positions[] = {
    {{ -1, -1 }},
    {{ 1, -1 }},
    {{ -1, 1 }},
    //
    {{ 1, 1 }},
    {{ -1, 1 }},
    {{ 1, -1 }}
};

_Alignas(64) static vec3s vert_colors[] = {
    {{ 1, 0, 0 }},
    {{ 1, 0, 0 }},
    {{ 1, 0, 0 }},
    //
    {{ 0, 1, 0 }},
    {{ 0, 1, 0 }},
    {{ 0, 1, 0 }}
};

static union {
    GLuint data[2];
    struct PACKED {
        GLuint pos;
        GLuint color;
    };
} buffers;

void gfx_main(void) {

    glGenBuffers(SIZEOF_ARRAY(buffers.data), buffers.data);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_positions), vert_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_colors), vert_colors, GL_STATIC_DRAW);
}

void gfx_update(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(vert_positions));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}