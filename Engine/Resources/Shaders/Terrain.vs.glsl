#version 430

in vec3 position;

uniform mat4 mvp;

out vec3 Position;
out vec3 Normal;

void main ()
{
  gl_Position = mvp * vec4 (position, 1.0);
  Position = vec3 (position.xy, -1.0);
  Normal = vec3 (0,0,1);
}