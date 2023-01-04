#version 330 core

uniform sampler2D tex_sampler;

in vec2 uv;

out vec4 frag_color;

void main() {
    frag_color = texture(tex_sampler, uv);
}