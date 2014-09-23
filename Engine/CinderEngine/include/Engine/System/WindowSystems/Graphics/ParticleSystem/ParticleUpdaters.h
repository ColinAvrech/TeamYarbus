/******************************************************************************/
/*!
\file   ParticleUpdaters.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PARTICLE_UPDATERS_H
#define _PARTICLE_UPDATERS_H

#include "Particles.h"

namespace Framework
{

  // UPDATES PARTICLES USING THE EULER METHOD
  // pos = pos + newPos;
  // THIS GIVES US MORE OPTIONS TO CREATE BETTER PARTICLE EFFECTS
  class EulerUpdater : public ParticleUpdater
  {
  public:
    glm::vec4 m_globalAcceleration;
  public:
    EulerUpdater () : m_globalAcceleration (0.0) { }

    virtual void update (double dt, ParticleData *p) override;
  };


  // SIMPLE COLLISION MODEL FOR A STRAIGHT LINE AND A DOT
  class FloorUpdater : public ParticleUpdater
  {
  public:
    float m_floorY;
    float m_bounceFactor;
  public:
    FloorUpdater () : m_floorY (0.0), m_bounceFactor (0.5f) { }

    virtual void update (double dt, ParticleData *p) override;
  };


  // UPDATES THE ATTRACTOR EFFECT
  // DESIGNED TO HAVE MORE THAN ONE COMPONENTS
  class AttractorUpdater : public ParticleUpdater
  {
  protected:
    std::vector<glm::vec4> m_attractors; // .w is force
  public:
    virtual void update (double dt, ParticleData *p) override;

    size_t collectionSize () const { return m_attractors.size (); }
    void add (const glm::vec4 &attr) { m_attractors.push_back (attr); }
    glm::vec4 &get (size_t id) { return m_attractors [id]; }
  };



  class BasicColorUpdater : public ParticleUpdater
  {
  public:
    virtual void update (double dt, ParticleData *p) override;
  };


  // UPDATES THE COLOR USING POSITION
  // CREATES A RELATION BETWEEN POSITION AND COLOR
  // JUST LIKE outColor = Position;
  class PosColorUpdater : public ParticleUpdater
  {
  public:
    glm::vec4 m_minPos;
    glm::vec4 m_maxPos;
  public:
    PosColorUpdater () : m_minPos (0.0), m_maxPos (1.0) { }

    virtual void update (double dt, ParticleData *p) override;
  };


  // UPDATES COLOR BASED ON VELOCITY
  // WILL BE USEFUL FOR FIRE PARTICLES
  class VelColorUpdater : public ParticleUpdater
  {
  public:
    glm::vec4 m_minVel;
    glm::vec4 m_maxVel;
  public:
    VelColorUpdater () : m_minVel (0.0), m_maxVel (1.0) { }

    virtual void update (double dt, ParticleData *p) override;
  };


  // TIMER
  class BasicTimeUpdater : public ParticleUpdater
  {
  public:
    virtual void update (double dt, ParticleData *p) override;
  };
}

#endif