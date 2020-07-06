#version 300 es

in vec3 inPosition;
in vec3 inColor;

out lowp vec3 intermColor;

uniform mat4 unifCameraTransform;
uniform mat4 unifProjTransform;

void main() {
  intermColor = inColor;
  gl_Position = unifProjTransform * unifCameraTransform * vec4(inPosition, 1.0);
}
