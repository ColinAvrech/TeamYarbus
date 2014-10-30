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

	outColor = vec4 (Temperature, Temperature, Temperature, 1);
}
