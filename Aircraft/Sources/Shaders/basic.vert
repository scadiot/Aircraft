#version 400

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 outColor;
out vec2 TexCoord;

void main() {
  gl_Position = mvpMatrix * vec4(inPos, 1.0);
  outColor = inNormal;
  TexCoord = inTexCoord;
}