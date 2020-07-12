#version 300 es

in lowp vec3 intermColor;
in lowp vec2 intermTextureCoords;
in lowp vec2 intermLightmapCoords;

uniform sampler2D unifTexture;
uniform sampler2D unifLightmapTexture;

out lowp vec4 outColor;

void main() {
  lowp vec4 light = texture(unifLightmapTexture, intermLightmapCoords);
  lowp vec4 color = texture(unifTexture, intermTextureCoords);
  outColor = color * light * 4.0;
  // outColor = color * light * 2.0;
  // * vec4(intermColor, 1.0);
}
