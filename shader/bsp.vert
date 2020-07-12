#version 300 es

in vec3 inPosition;
in vec3 inColor;
in vec2 inTextureCoords;
in vec2 inLightmapCoords;

out lowp vec3 intermColor;
out lowp vec2 intermTextureCoords;
out lowp vec2 intermLightmapCoords;

uniform mat4 unifCameraTransform;
uniform mat4 unifProjTransform;

void main() {
  intermColor = inColor;
  intermTextureCoords = inTextureCoords;
  intermLightmapCoords = inLightmapCoords;
  gl_Position = unifProjTransform * unifCameraTransform * vec4(inPosition, 1.0);
}
