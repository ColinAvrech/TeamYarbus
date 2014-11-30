#version 430

in vec3 position;
in vec3 normal;

uniform mat4 mvp;

out vec3 Position;
out vec3 Normal;

void main ()
{
  gl_Position = mvp * vec4 (position, 1.0);
  Position = position;
  Normal = normal;
}