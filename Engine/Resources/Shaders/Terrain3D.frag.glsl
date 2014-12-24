#version 430

in vec3 Position;
in vec3 Normal;
in vec4 color;

out vec4 outColor;

void main ()
{
  outColor = vec4(Normal.r, Normal.g, Normal.b, 1);
}