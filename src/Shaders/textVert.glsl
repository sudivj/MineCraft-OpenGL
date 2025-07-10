#version 330 core
layout (location = 1) in vec4 textvertex;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(textvertex.xy, 0.0, 1.0);
    TexCoord = textvertex.zw;
}