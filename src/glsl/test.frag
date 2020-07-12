#version 300 es

in lowp vec2 vertexTexCoord;
out lowp vec4 outColor;

uniform sampler2D tex;

void main() {
  outColor = texture(tex, vertexTexCoord);
}
