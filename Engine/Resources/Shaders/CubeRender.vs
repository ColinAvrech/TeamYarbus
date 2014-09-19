#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
  Texcoord = texcoord;
  Color = color;
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (position, 1.0);
}