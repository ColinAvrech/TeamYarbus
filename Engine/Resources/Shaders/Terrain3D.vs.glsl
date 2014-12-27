#version 430

in vec3 position;
in vec3 normal;

uniform mat4 mvp;

out vec4 Position;
out vec3 Normal;

void main ()
{
  Position = mvp * vec4 (position, 1.0);
  gl_Position = Position;
  //Position = position;
  Normal = normal;
}