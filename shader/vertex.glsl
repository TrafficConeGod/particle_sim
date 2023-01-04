#version 330 core

layout(location = 0) in vec2 pos_attr;
layout(location = 1) in vec3 color_attr;

out vec2 pos;
out vec3 color;

void main() {
    gl_Position = vec4(pos_attr, 0, 1);
    color = color_attr;
}