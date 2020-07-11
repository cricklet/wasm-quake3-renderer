#version 300 es

in vec3 inPosition;
in vec3 inColor;
in vec2 inTextureCoords;

out lowp vec3 intermColor;
out lowp vec2 intermTextureCoords;

uniform mat4 unifCameraTransform;
uniform mat4 unifProjTransform;

void main() {
  intermColor = inColor;
  intermTextureCoords = inTextureCoords;
  gl_Position = unifProjTransform * unifCameraTransform * vec4(inPosition, 1.0);
}
