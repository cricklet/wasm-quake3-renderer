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
  if (unifAlpha > 0.99f) {
    outColor = vec4(0.15, 0.15, 0.15, 0.15) + color * light * 4.0;
  } else {
    outColor = unifAlpha * color * 2.0;
  }
}
