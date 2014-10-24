#version 430

// Target 0 : Vertex position
layout(std140, binding = 0) buffer Col
{
   vec4 Colors [ ];
};

layout (local_size_x = 16, local_size_y = 16) in;

uniform int particleCount;

void main ()
{
  // Current SSBO index
  uint index = gl_GlobalInvocationID.x;
  if (index < particleCount)
  {
    vec4 col = Colors [index];
    col = vec4 (1, 0, 0, 1);
    Colors [index] = col;
  }
}