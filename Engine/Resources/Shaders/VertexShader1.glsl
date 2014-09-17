#version 430

in vec2 position;
in vec4 color;
in vec2 texcoord;
out vec4 Color;
out vec2 Texcoord;

void main()
{
   Color = color;
   Texcoord = texcoord;
   gl_Position = vec4(position * 0.6 + 0.5, 0.0, 1.0);
}