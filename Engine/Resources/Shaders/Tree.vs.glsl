#version 430

in vec2 position;
//in vec4 color;

//out vec4 Color;

uniform mat4 mvp;

void main ()
{
  gl_Position = mvp * vec4 (position, 1.0, 1.0);
  //Color = color;
}