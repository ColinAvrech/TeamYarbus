#version 430

in vec2 position;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec2 Texcoord;
out vec3 Normal;

void main ()
{
  gl_Position = vec4 (position, 1.0, 1.0);
  Position = vec3 (position, -1.0);
  Color = vec4 (1,1,1,1);
  Texcoord = texcoord;
  Normal = vec3 (0, 0, -1);
}