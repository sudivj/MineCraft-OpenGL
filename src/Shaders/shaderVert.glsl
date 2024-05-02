#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 textvertex;

out vec3 vertexColour;
out vec2 TexCoord;

uniform int screen = 1; // 1 - world
                        // 2 - text

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    if (screen == 1) {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    } else if (screen == 2) {
        gl_Position = projection * vec4(textvertex.xy, 0.0, 1.0);
        TexCoord = textvertex.zw;
    }
}