#version 430

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Particle Struct
struct Particle
{
  vec4 m_pos;
  vec4 m_col;
  vec4 m_vel;
  int m_alive;
  vec4 m_time;
  void Generate (int maxSize);
};

int count;
int totalParticles = 20000;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SSBO Target 0: Particle Data
layout(std140, binding = 0) buffer Prt
{
   Particle Particles [ ];
};

layout (binding = 2, offset = 0) uniform atomic_uint particleCount;

layout (local_size_x = 16, local_size_y = 16) in;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void SwapData (int a, int b)
{
  Particles [a] = Particles [b];
}

void Kill (int id)
{
  Particles [id].m_alive = 0;
  SwapData (id, count - 1);
  --count;
  //atomicCounterDecrement (particleCount);
}

void Wake (int id)
{
  Particles [id].m_alive = 1;
  //atomicCounterIncrement (particleCount);
  ++count;
}

void UpdateTime (int id, float time)
{
  Particles [id].m_time.x = time;
  Particles [id].m_time.y = time;
  Particles [id].m_time.z = 0.0;
  Particles [id].m_time.w = 1.0 / time;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Generators
struct BoxPosGenerator
{
  vec4 m_pos;
  vec4 m_maxStartPosOffset;
  void Generate (double dt, int start, int end);
};

struct BasicTimeGenerator
{
  float m_minTime;
  float m_maxTime;
  void Generate (double dt, int start, int end);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Emitter Struct
struct ParticleEmitter
{
  BoxPosGenerator m_bPosGen;
  BasicTimeGenerator m_bTGen;
  int m_emitRate;
  void Emit (double dt);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Updaters Struct
struct EulerUpdater
{
  vec4 m_globalAcceleration;
  void Update (double dt);
};

struct BasicTimeUpdater
{
  void Update (double dt);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Particle System Struct
struct ParticleSystem
{
  ParticleEmitter m_emitter;
  EulerUpdater m_eUpdater;
  BasicTimeUpdater m_tUpdater;
  void Update (double dt);
}
m_system;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Particle Struct Implementation

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Generators Struct Implementation
void BoxPosGenerator::Generate (double dt, int startId, int endId)
{
  vec4 posMin = vec4 (m_pos.x - m_maxStartPosOffset.x, m_pos.y - m_maxStartPosOffset.y, m_pos.z - m_maxStartPosOffset.z, 1.0);
  vec4 posMax = vec4 (m_pos.x + m_maxStartPosOffset.x, m_pos.y + m_maxStartPosOffset.y, m_pos.z + m_maxStartPosOffset.z, 1.0);

  for (int i = startId; i < endId; ++i)
  {
    Particles [i].m_pos = posMin;
  }
}

void BasicTimeGenerator::Generate (double dt, int startId, int endId)
{
  for (int i = startId; i < endId; ++i)
  {
    float time = m_minTime;
    UpdateTime (i, time);
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Emitter Struct Implementation
void ParticleEmitter::Emit (double dt)
{
  const int maxNewParticles = int (0.1f * m_emitRate);
  const int startId = int (count);
  const int endId = min (startId + maxNewParticles, totalParticles - 1);

  m_bTGen.Generate (dt, startId, endId);
  m_bPosGen.Generate (dt, startId, endId);
  //for (auto &gen : m_generators)
  //  gen->generate (dt, p, startId, endId);

  for (int i = startId; i < endId; ++i)
  {
    Wake (i);
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Updaters Struct Implementation
void EulerUpdater::Update (double dt)
{}

void BasicTimeUpdater::Update (double dt)
{
  //unsigned int endId = count;
  //float localDT = float (dt);

  ////vec4 * __restrict t = p->m_time.get ();

  //if (endId == 0) return;

  //for (int i = 0; i < endId; ++i)
  //{
  //  Particles [i].m_time.x = localDT;
  //  // interpolation: from 0 (start of life) till 1 (end of life)
  //  Particles [i].m_time.z = 1.0;/* - (t [i].x*t [i].w); // .w is 1.0/max life time	*/	

  //  if (Particles [i].m_time.x < 0.0)
  //  {
  //    Kill (i);
  //    if (count < totalParticles) endId = count;
  //    else endId = totalParticles;
  //    //endId = p->m_countAlive < p->m_count ? p->m_countAlive : p->m_count;
  //  }
  //}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Particle System Struct Implementation
void ParticleSystem::Update (double dt)
{
  m_emitter.Emit (dt);
  m_tUpdater.Update (dt);
  m_eUpdater.Update (dt);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// MAIN
void main ()
{
  m_system.m_emitter.m_emitRate = 100;
  // Current SSBO index
  uint index = gl_GlobalInvocationID.x;
  m_system.Update (0.016);
  vec4 col = Particles [index].m_col;
  col = vec4 (1, 0.4, 0, 0.4);
  Particles [index].m_col = col;
}