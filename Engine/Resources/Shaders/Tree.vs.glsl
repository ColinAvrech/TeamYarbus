#version 430

in vec2 position;
//in vec4 color;

out vec2 Position;

uniform mat4 mvp;

void main ()
{
  Position = position;
  gl_Position = mvp * vec4 (position, 0.0, 1.0);
  //Color = color;
}