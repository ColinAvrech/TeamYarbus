#version 430

in vec3 Position;
in vec4 Color;
in vec3 Normal;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;

void main ()
{
  outColor = Color;
}