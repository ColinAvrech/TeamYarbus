#version 430

in vec4 Graph_Coord;
in float Temperature;
uniform vec4 color;

out vec4 outColor;

void main (void)
{
	float factor;

	if (gl_FrontFacing)
		factor = 1.0;
	else
		factor = 0.5;
  if (Temperature < 0.5)
  {
    outColor = vec4 (0, Temperature * 0.4, Temperature, Temperature); 
  }
  else if (Temperature < 0.7)
  {
	  outColor = vec4 ((Temperature - 0.5) / 0.2, Temperature * 0.4, 1-Temperature, Temperature);
  }
  else
  {
    outColor = vec4 (1, 1 - Temperature, 0, Temperature);
  }
}
