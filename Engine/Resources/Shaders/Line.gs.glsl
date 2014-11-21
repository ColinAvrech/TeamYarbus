#version 430

layout(points) in;
layout(line_strip, max_vertices = 64) out;

in vec3 Color [];

out vec3 fragColor;

uniform mat4 mvp;
uniform vec3 p1;
uniform vec3 p2;

void main()
{
  fragColor = Color[0];
  gl_Position = mvp * (gl_in[0].gl_Position + vec4(p1.xy, 0.0, 0.0));
  EmitVertex();

  gl_Position = mvp * (gl_in[0].gl_Position + vec4(p2.xy, 0.0, 0.0));
  EmitVertex();

  EndPrimitive();
}