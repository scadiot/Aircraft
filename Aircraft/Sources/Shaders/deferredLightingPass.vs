#version 400

uniform mat4 inverseMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

out vec2 aTexCoord;
out vec4 aNear;
out vec4 aFar;

void main() {
  gl_Position = vec4(inPosition, 1.0);
  aTexCoord = inTexCoord;

  vec2 pos = gl_Position.xy/gl_Position.w;
  aNear = inverseMatrix * (vec4(pos, -1.0, 1.0));
  aFar  = inverseMatrix * (vec4(pos, 1.0, 1.0));
}