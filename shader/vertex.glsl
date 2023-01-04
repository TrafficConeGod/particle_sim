#version 330 core

layout(location = 0) in vec2 pos_attr;
layout(location = 1) in vec2 uv_attr;

out vec2 uv;

void main() {
    gl_Position = vec4(pos_attr, 0, 1);
    uv = uv_attr;
}