#version 430

in vec4 outColor;

out vec4 vFragColor;

uniform sampler2D tex;

void main()
{
	vFragColor = texture(tex, gl_PointCoord) * outColor;
}