#version 430

in vec3 Position;
in vec3 Normal;
uniform vec4 color1;
uniform vec4 color2;
out vec4 outColor;

void main ()
{
  float r = color1.r + (color2.r) * Normal.r;
  float g = color1.g + (color2.g) * Normal.g;
  float b = color1.b + color2.b;
  
  outColor = vec4(1.5f - r, 1.5f - g, 0.5f * b, 1);
}