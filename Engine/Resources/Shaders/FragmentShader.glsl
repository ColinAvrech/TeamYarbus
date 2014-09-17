#version 430 core

in vec4 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D image;

void main()
{
   outColor = texture (image, Texcoord) * Color;
}