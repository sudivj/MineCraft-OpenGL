#version 330 core
out vec4 FragColor;

in vec3 vertexColour;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alpha;

void main()
{
   // FragColor = vec4((vertexColour + 0.5), 1.0f);
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), alpha);
   //FragColor = texture(texture1, TexCoord);
}