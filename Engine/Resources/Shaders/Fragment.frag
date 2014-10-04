#version 430

out vec4 outColor;

uniform vec4 Color;
uniform sampler2D image;

void main ()
{
  outColor = vec4 (texture (image, gl_PointCoord) * Color);
}
