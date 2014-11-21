#version 430

in vec3 position;
in vec3 color;

out vec3 Color;

void main ()
{
  Color = color;
}