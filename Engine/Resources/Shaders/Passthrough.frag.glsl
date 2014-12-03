#version 430

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;

void main ()
{
  outColor = texture (image, Texcoord) * vec4 (1.0, 1.0, 1.0, 1.0);
}