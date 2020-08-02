#version 300 es

in vec2 inPosition;
in vec2 inTextureCoords;
out lowp vec2 intermTextureCoords;

void main() {
  intermTextureCoords = inTextureCoords;
  gl_Position = vec4(inPosition, 0.0, 1.0);
}
