#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 modelViewProjectionMatrix;


void main()
{
  gl_Position = modelViewProjectionMatrix * vec4(position, 0.5);
  Position = vec3 (position.xy, 1.0);
  Color = color;
  Normal = normal;
  Texcoord = texcoord;
}