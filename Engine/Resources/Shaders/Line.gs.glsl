#version 430

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 modelViewProjectionMatrix;

void main()
{
  gl_Position = modelViewProjectionMatrix * (gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0));
  EmitVertex();

  gl_Position = modelViewProjectionMatrix * gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}