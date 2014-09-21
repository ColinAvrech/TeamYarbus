#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
   Color = color;
   Normal = normal;
   Texcoord = texcoord;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 0.5);
}