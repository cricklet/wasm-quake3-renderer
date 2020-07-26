#version 300 es

in lowp vec2 intermTextureCoords;

uniform sampler2D unifSceneTexture;
uniform sampler2D unifEffectsTexture;

out lowp vec4 outColor;

void main() {
  lowp vec4 solid = texture(unifLightmapTexture, intermLightmapCoords);
  outColor = vec4(solid.r, solid.g, solid.b, 1.0);
}