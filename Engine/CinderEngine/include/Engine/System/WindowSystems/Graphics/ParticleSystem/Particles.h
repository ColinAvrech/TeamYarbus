/******************************************************************************/
/*!
\file   Particles.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <vector>
#include <memory>
#include "glm.hpp"

namespace Framework
{

  // THE CONTAINER FOR ALL PARTICLES
  // POSITION, COLOR, START, END (EULER METHOD)
  class ParticleData
  {
  public:
    std::unique_ptr<glm::vec4 []> m_pos;
    std::unique_ptr<glm::vec4 []> m_col;
    std::unique_ptr<glm::vec4 []> m_startCol;
    std::unique_ptr<glm::vec4 []> m_endCol;
    std::unique_ptr<glm::vec4 []> m_vel;
    std::unique_ptr<glm::vec4 []> m_acc;
    std::unique_ptr<glm::vec4 []> m_time;
    std::unique_ptr<bool []>  m_alive;

    size_t m_count{ 0 };
    size_t m_countAlive{ 0 };
  public:
    ParticleData () { }
    explicit ParticleData (size_t maxCount) { generate (maxCount); }
    ~ParticleData () { }

    ParticleData (const ParticleData &) = delete;
    ParticleData &operator=(const ParticleData &) = delete;

    // GENERATE maxSize PARTICLES
    void generate (size_t maxSize);
    // KILL PARTICLE WITH ID id
    void kill (size_t id);
    // AWAKE PARTICLE WITH ID id
    void wake (size_t id);
    // USED TO KILL AND WAKE PARTICLES WITHOUT DESTROYING ANYTHING
    void swapData (size_t a, size_t b);

    // USED TO RENDER
    // ONLY ALIVE PARTICLES (ONES AT THE FRONT OF THE CONTAINER WILL BE RENDERED)
    static void copyOnlyAlive (const ParticleData *source, ParticleData *destination);
    // FOR OPTIMIZING MORE AND MORE
    static size_t computeMemoryUsage (const ParticleData &p);
  };


  // GENERATES PARTICLES
  class ParticleGenerator
  {
  public:
    ParticleGenerator () { }
    virtual ~ParticleGenerator () { }

    virtual void generate (double dt, ParticleData *p, size_t startId, size_t endId) = 0;
  };


  // EMITS PARTICLES AT A SPECIFIC EMIT RATE
  class ParticleEmitter
  {
  protected:
    std::vector<std::shared_ptr<ParticleGenerator>> m_generators;
  public:
    glm::vec3 position;
    float m_emitRate{ 0.0 };
  public:
    ParticleEmitter () { }
    virtual ~ParticleEmitter () { }

    // calls all the generators and at the end it activates (wakes) particle
    virtual void emit (double dt, ParticleData *p);

    void addGenerator (std::shared_ptr<ParticleGenerator> gen) { m_generators.push_back (gen); }
  };


  // UPDATES PARTICLES EVERY FRAME
  // ONLY THE ONES WHICH NEED TO BE UPDATED
  // SAVES A LOT OF ITERATIONS
  class ParticleUpdater
  {
  public:
    ParticleUpdater () { }
    virtual ~ParticleUpdater () { }

    virtual void update (double dt, ParticleData *p) = 0;
  };


  // THE ALMIGHTY
  // CONTROLS EMITTERS AND UPDATERS
  class ParticleSystem
  {
  protected:
    ParticleData m_particles;
    ParticleData m_aliveParticles;

    size_t m_count;

    std::vector<std::shared_ptr<ParticleEmitter>> m_emitters;
    std::vector<std::shared_ptr<ParticleUpdater>> m_updaters;

  public:
    explicit ParticleSystem (size_t maxCount);
    virtual ~ParticleSystem () { }

    // AVOID UNNECESARY COPIES
    ParticleSystem (const ParticleSystem &) = delete;
    ParticleSystem &operator=(const ParticleSystem &) = delete;

    // CALLED EVERY FRAME
    virtual void update (double dt);
    // RESET THE ENTIRE SYSTEM
    // RESET ALL PARTICLES IN THE CONTAINER
    virtual void reset ();

    // NUMBER OF PARTICLES
    virtual size_t numAllParticles () const { return m_particles.m_count; }
    // NUMBER OF ALIVE PARTICLES
    virtual size_t numAliveParticles () const { return m_particles.m_countAlive; }

    // COMPONENT BASED SYSTEM
    // ADD AS MANY EMITTERS, GENERATORS, UPDATERS
    void addEmitter (std::shared_ptr<ParticleEmitter> em) { m_emitters.push_back (em); }
    void addUpdater (std::shared_ptr<ParticleUpdater> up) { m_updaters.push_back (up); }

    // SENDS THE FINAL DATA TO THE RENDERER (ONLY ALIVE PARTICLES)
    ParticleData *finalData () { return &m_particles; }
    // FOR MORE AND MORE OPTIMIZATION
    // A PARTICLE SYSTEM WITH A FAKE RENDERER CAN BE CREATED TO KNOW THE BENCHMARK
    static size_t computeMemoryUsage (const ParticleSystem &p);
  };
}

#endif