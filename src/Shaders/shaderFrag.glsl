#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform int screen;

uniform float brightness = 0.0f;
uniform int textureIndex = 1;

int tex_index_y = (textureIndex / 16) + 1;
int tex_index_x = textureIndex - (textureIndex * 16);

float tex_offsetX = (0.0625f * (tex_index_x - 1));
float tex_offsetY = (0.0625f * (tex_index_y - 1));

uniform sampler2D atlas;
uniform sampler2D text;
uniform float alpha;
uniform vec3 textColor;

void main()
{
   if (screen == 1) {
      vec4 texColor = texture(atlas, vec2(TexCoord.x + tex_offsetX, TexCoord.y + tex_offsetY));
      FragColor = texture(atlas, vec2(TexCoord.x + tex_offsetX, TexCoord.y + tex_offsetY)) - brightness;
   } else if (screen == 2) {
      vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoord).r);
      FragColor = vec4(textColor, 1.0) * sampled;
   }
}