#version 400

uniform sampler2D texture_diffuse;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outAlbedoSpec;

void main() {
  outPosition = aPosition;
  outNormal = aNormal;
  outAlbedoSpec.rgb = texture(texture_diffuse, aTexCoord).rgb;
  outAlbedoSpec.a = 1;
}