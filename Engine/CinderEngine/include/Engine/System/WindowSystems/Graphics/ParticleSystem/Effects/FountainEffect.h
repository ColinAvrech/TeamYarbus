/******************************************************************************/
/*!
\file   FountainEffect.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FOUNTAIN_EFFECT_H
#define _FOUNTAIN_EFFECT_H

#include "Effect.h"
#include "Particles.h"
#include "ParticleGenerators.h"
#include "ParticleUpdaters.h"
#include "ParticleRenderer.h"

namespace Framework
{

  class FountainEffect : public IEffect
  {
  private:
    std::shared_ptr<ParticleSystem> m_system;
    std::shared_ptr<IParticleRenderer> m_renderer;
    std::shared_ptr<BoxPosGen> m_posGenerator;
    std::shared_ptr<BasicColorGen> m_colGenerator;
    std::shared_ptr<EulerUpdater> m_eulerUpdater;
    std::shared_ptr<FloorUpdater> m_floorUpdater;
    Texture* texture;
    Shader* shader;
  public:
    FountainEffect () { }
    ~FountainEffect () { }

    bool initialize (size_t numParticles) override;
    bool initializeRenderer () override;
    void reset () override { m_system->reset (); }
    void clean () override;
    void addUI (CTwBar*) override;
    void removeUI (CTwBar*) override;

    void update (double dt) override;
    void cpuUpdate (double dt) override;
    void gpuUpdate (double dt) override;
    void render () override;

    int numAllParticles () override { return m_system->numAllParticles (); }
    int numAliveParticles () override { return m_system->numAliveParticles (); }
  };
}

#endif