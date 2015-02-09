#version 430

in vec3 position;

in vec3 normal;

uniform mat4 mvp;

out vec3 Normal;
out vec3 Position;

void main ()
{
  Normal = normal;
  Position = position;
  gl_Position = mvp * vec4 (position, 1.0);
}
