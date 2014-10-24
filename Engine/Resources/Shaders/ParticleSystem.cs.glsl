#version 430

struct ParticleData
{
    vec4 m_pos;
    vec4 m_col;
    vec4 m_startCol;
    vec4 m_endCol;
    vec4 m_vel;
    vec4 m_acc;
    vec4 m_time;
    bool  m_alive;
    uint m_count;
    uint m_countAlive;
};

// Target 0 : Vertex position
layout(std140, binding = 0) buffer Pos
{
   vec4 Positions [ ];
};

layout (local_size_x = 16, local_size_y = 16) in;

vec4 vel = vec4 (0, 1, 0, 0);
uniform float deltaT;

void main ()
{
  // Current SSBO index
  uint index = gl_GlobalInvocationID.x;
  vec4 pos = Positions [index];

  pos += deltaT * vel;

  Positions [index] = pos;
}