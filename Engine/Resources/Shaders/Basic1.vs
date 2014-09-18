#version 430

in vec2 position;
in vec4 color;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec2 Texcoord;
out vec3 Normal;

void main()
{
   Color = color;
   Texcoord = texcoord;
   gl_Position = vec4(position, 0.0, 0.5);
   Position = vec3 (position, 1.0);
   Normal = vec3 (0.0, 0.0, 1.0);
}