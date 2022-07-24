#version 400

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 aPosition;
out vec3 aNormal;
out vec2 aTexCoord;

void main() {
  gl_Position = mvpMatrix * vec4(inPosition, 1.0);
  aPosition = (modelMatrix * vec4(inPosition, 1.0)).xyz;
  aNormal = (modelMatrix * vec4(inNormal, 1.0)).xyz;
  aTexCoord = inTexCoord;
}