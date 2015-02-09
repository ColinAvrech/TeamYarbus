#version 430

in vec2 position;
in vec2 texcoord;

out vec2 Texcoord;
out vec2 Position;

void main ()
{
  gl_Position = vec4 (position, 1.0, 1.0);
  Position = position;
  Texcoord = texcoord;
}