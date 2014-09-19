#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec2 Texcoord;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
   Color = color;
   Texcoord = texcoord;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 0.5);
   Position = vec3 (position.x, position.y, 1.0);
   Normal = normal;
}