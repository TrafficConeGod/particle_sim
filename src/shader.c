#include "shader.h"
#include "fs.h"
#include <stdalign.h>
#include <stdio.h>

_Alignas(64) static char info_log[1024];

GLuint load_shader_program(const char* vertex_shader_path, const char* fragment_shader_path) {
    const char* vertex_shader_source = load_file_null_terminated(vertex_shader_path);
    if (vertex_shader_source == NULL) {
        return -1;
    }
    const char* fragment_shader_source = load_file_null_terminated(fragment_shader_path);
    if (fragment_shader_source == NULL) {
        return -1;
    }

    GLint info_log_size;
    
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_size);
    if (info_log_size > 0) {
        info_log[info_log_size] = '\0';
        glGetShaderInfoLog(vertex_shader, info_log_size, NULL, info_log);
        printf("%s\n", info_log);
        return -1;
    }
    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_size);
    if (info_log_size > 0) {
        info_log[info_log_size] = '\0';
        glGetShaderInfoLog(fragment_shader, info_log_size, NULL, info_log);
        printf("%s\n", info_log);
        return -1;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_size);
    if (info_log_size > 0) {
        info_log[info_log_size] = '\0';
        glGetProgramInfoLog(shader_program, info_log_size, NULL, info_log);
        printf("%s\n", info_log);
        return -1;
    }

    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}