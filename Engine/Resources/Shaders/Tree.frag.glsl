#version 430

uniform vec4 color;

out vec4 outColor;
in vec2 Position;

void main ()
{
  outColor = vec4(Position.x, Position.y, color.b, 1);
  //outColor = color;
}