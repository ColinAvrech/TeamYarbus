#version 430

in vec3 Position;
in vec3 Normal;
in vec4 color;

out vec4 outColor;

void main ()
{
  outColor = vec4(0.5 * Normal.r, Normal.g + Position.y - 2, Normal.b + Position.z, 1);
}