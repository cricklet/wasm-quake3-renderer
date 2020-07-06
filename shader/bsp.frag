#version 300 es

in lowp vec3 intermColor;

out lowp vec4 outColor;

void main() {
  outColor = vec4(intermColor, 1.0);
}
