#version 430 core

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;
uniform vec4 overrideColor;

void main()
{
  outColor = texture (image, Texcoord) * overrideColor;
}