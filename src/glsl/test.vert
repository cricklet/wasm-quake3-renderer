#version 300 es

in vec2 position;
in vec2 texCoord;
out lowp vec2 vertexTexCoord;

void main() {
  vertexTexCoord = texCoord;
  gl_Position = vec4(position, 0.0, 1.0);
}
