#version 430

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec4 vColor;

out vec4 outColor;

uniform mat4 matModelview;
uniform mat4 matProjection;
uniform float size;

void main() 
{
  vec4 eyePos = matModelview * vVertex;
  gl_Position = matProjection * eyePos;
	
	float dist = length (eyePos.xyz);
	float att = inversesqrt (0.1f * dist);
	gl_PointSize = size * att;

  outColor = vColor;
}