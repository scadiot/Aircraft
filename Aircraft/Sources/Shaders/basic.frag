#version 400
in vec3 outColor;
in vec2 TexCoord;
uniform sampler2D texture1;
out vec4 frag_colour;
void main() {
  frag_colour = texture(texture1, TexCoord);
}