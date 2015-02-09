#version 430

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec3 Color [];

out vec3 fragColor;

uniform mat4 mvp;
uniform vec2 p1;
uniform vec2 p2;

void main()
{
  fragColor = Color[0];
  gl_Position = mvp * (vec4(p1.x, p1.y, 0.0, 1.0));
  EmitVertex();

  gl_Position = mvp * (vec4(p2.x, p2.y, 0.0, 1.0));
  EmitVertex();

  EndPrimitive();
}