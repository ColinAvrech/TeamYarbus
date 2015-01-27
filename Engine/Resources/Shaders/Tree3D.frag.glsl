#version 430


in vec3 Normal;
in vec3 Position;

out vec4 outColor;

uniform vec4 color;

void main ()
{
  outColor = vec4(Normal.r * 0.5 + 0.5, Normal.g * 0.5 + 0.5, Normal.b * 0.5 + 0.5, 1.0);
}