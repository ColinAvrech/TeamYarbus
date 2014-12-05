/******************************************************************************/
/*!
\file   PlayerEffect.h
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

  class KeyEvent;

  class PlayerEffect : public IEffect
  {
  private:
    std::shared_ptr<ParticleSystem> m_system;
    std::shared_ptr<IParticleRenderer> m_renderer;
    std::shared_ptr<EulerUpdater> m_eulerUpdater;
    std::shared_ptr<FloorUpdater> m_floorUpdater;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // EMITTERS - GENERATORS
    //////////////////////////////////////////////////////////////////////////
    std::shared_ptr<ParticleEmitter> trailEmitter;
    std::shared_ptr<BoxPosGen> trailGenerator;
    std::shared_ptr<BoxPosGen> trailGenerator1;
    std::shared_ptr<ParticleEmitter> ringEmitter;
    //////////////////////////////////////////////////////////////////////////

    Texture* texture;
  public:
    PlayerEffect (){}
    virtual ~PlayerEffect ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    virtual void Draw ();
    virtual void OnApplicationPause (PauseEvent* pause);

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
    void OnKeyPressed (KeyEvent*);
    int numAllParticles () override { return m_system->numAllParticles (); }
    int numAliveParticles () override { return m_system->numAliveParticles (); }
    void CreateTrailEmitter (std::shared_ptr<BoxPosGen> trail, bool active, vec3 position, vec3 minVelocity, vec3 maxVelocity, float emitRate);
    void CreateRingEmitter (bool active, vec3 position, vec3 minVelocity, vec3 maxVelocity, float emitRate);

    const static string Name;
  };
}

#endif