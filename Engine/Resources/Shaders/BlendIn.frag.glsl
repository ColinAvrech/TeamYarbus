#version 430

in vec4 Color;
in vec3 Normal;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;
uniform float alpha;

void main ()
{
  outColor = texture (image, Texcoord) * vec4 (1.0, 1.0, 1.0, alpha);
}