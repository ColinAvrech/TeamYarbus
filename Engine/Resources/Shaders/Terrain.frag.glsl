#version 430

out vec4 outColor;
in vec3 Position;

uniform vec4 color1;
uniform vec4 color2;

void main ()
{
  //vec3 p = original_pos;
  vec3 p = Position;
  outColor = vec4((0.5 * (1 - p.y) * color2.r) + (0.5 * (1 + p.y) * color1.r),
                  (0.5 * (1 - p.y) * color2.g) + (0.5 * (1 + p.y) * color1.g),
				  (0.5 * (1 - p.y) * color2.b) + (0.5 * (1 + p.y) * color1.b),
				  (0.5 * (1 - p.y) * color2.a) + (0.5 * (1 + p.y) * color1.a));
}