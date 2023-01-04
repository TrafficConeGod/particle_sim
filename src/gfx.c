#include "gfx.h"
#include "util.h"
#include "tilemap.h"
#include "shader.h"

// TODO: Use Pixel Buffer Objects

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

_Alignas(64) static color_t pixel_colors[NUM_TILES] = {0};

static union {
    GLuint data[2];
    struct PACKED {
        GLuint pos;
        GLuint uv;
    };
} buffers;

error_t gfx_init(void) {
    glEnable(GL_CULL_FACE);

    GLuint vert_array;
    glGenVertexArrays(1, &vert_array);
    glBindVertexArray(vert_array);

    GLuint shader_program = load_shader_program("shader/vertex.glsl", "shader/fragment.glsl");
    if (shader_program == (GLuint)-1) {
        return 1;
    }
    glUseProgram(shader_program);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, TILEMAP_WIDTH, TILEMAP_HEIGHT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel_colors);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader_program, "tex_sampler"), 0);

    glGenBuffers(SIZEOF_ARRAY(buffers.data), buffers.data);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_verts), pos_verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.uv);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_verts), uv_verts, GL_STATIC_DRAW);

    return 0;
}

void gfx_update(void) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixel_colors);

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

void set_tile_to_color(size_t index, color_t color) {
    pixel_colors[index] = color;
}