#version 330 core

uniform sampler2D tex_sampler;

in vec2 uv;

out vec3 frag_color;

void main() {
    frag_color = texture(tex_sampler, uv).rgb;
}