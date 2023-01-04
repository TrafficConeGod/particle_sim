#version 330 core

in vec2 uv;

out vec3 frag_color;

void main() {
    frag_color = vec3(uv, 0);
}