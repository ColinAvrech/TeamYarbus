#version 430

layout(points) in;
layout(line_strip, max_vertices = 64) out;

in vec3 Color[];

out vec3 fragColor;

uniform float radius;
uniform int divisions;
uniform mat4 mvp;

const float PI = 3.1415926;

void main()
{
  float rX = radius;
  float rY = radius * 4.0 / 3.0;
  fragColor = Color[0];

  // Safe, GLfloats can represent small integers exactly
  for (int i = 0; i <= divisions; i++)
  {
    // Angle between each side in radians
    float ang = PI * 2.0 / divisions * i;

    // Offset from center of point (0.3 to accomodate for aspect ratio)
    vec4 offset = vec4(cos(ang) * rX, -sin(ang) * rX, 0.0, 0.0);
    gl_Position = mvp * (gl_in[0].gl_Position + offset);

    EmitVertex();
  }

  EndPrimitive();
}