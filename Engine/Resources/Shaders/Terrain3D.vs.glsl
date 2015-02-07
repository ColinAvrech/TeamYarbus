#version 430

in vec3 position;
in vec3 normal;

uniform mat4 mvp;
uniform mat4 modelMatrix;

out vec3 Position;
out vec3 Vert_Pos;
out vec3 Normal;

void main ()
{
  Position = vec3 (modelMatrix * vec4 (position, 1.0));
  gl_Position = mvp * vec4(position, 1.0);
  Normal = normal;
  Vert_Pos = position;
}