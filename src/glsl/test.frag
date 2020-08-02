#version 300 es

in lowp vec2 intermTextureCoords;
out lowp vec4 outColor;

uniform sampler2D unifTexture;

void main() {
  outColor = texture(unifTexture, intermTextureCoords);
}
