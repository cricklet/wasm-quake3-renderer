#version 300 es

in lowp vec3 intermColor;
in lowp vec2 intermTextureCoords;
in lowp vec2 intermLightmapCoords;

uniform lowp float unifAlpha;

uniform sampler2D unifTexture;
uniform sampler2D unifLightmapTexture;

out lowp vec4 outColor;

void main() {
  lowp vec4 light = texture(unifLightmapTexture, intermLightmapCoords);
  lowp vec4 color = texture(unifTexture, intermTextureCoords);
  outColor = (vec4(0.2, 0.2, 0.2, 0.2) + color * light * 4.0) * unifAlpha;
}
