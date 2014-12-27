#version 430

in vec4 Position;
in vec3 Normal;
uniform vec4 color1;
uniform vec4 color2;
out vec4 outColor;

void main ()
{
  float factor = Position.z / 1750; 
  float r = color1.r + (color2.r) * Normal.r;
  float r_range = 1.5f - r - 0.5f;
  float g = color1.g + (color2.g) * Normal.g;
  float g_range = 1.5f - g - 0.5f;
  float b = color1.b + color2.b;
  float b_range = 0.5f * b - 0.5f;

  float r_f = 1.5f - r - r_range * factor;
  if(r_f < 0.5f)
    r_f = 0.5f;
  float g_f = 1.5f - g - g_range * factor;
  if(g_f < 0.5f)
    g_f = 0.5f;
  float b_f = 0.5f * b - b_range * factor;
  if(b_f < 0.5f)
    b_f = 0.5f;
  
  outColor = vec4(r_f, g_f, b_f, 1);
  //outColor = vec4(Position.z, Position.z, Position.z, Position.z / 500);
}