#version 300 es

in lowp vec3 intermColor;
in lowp vec2 intermTextureCoords;

uniform sampler2D unifTexture;

out lowp vec4 outColor;

void main() {
  outColor = texture(unifTexture, intermTextureCoords) * vec4(intermColor, 1.0);
}
