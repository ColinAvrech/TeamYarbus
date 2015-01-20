/******************************************************************************/
/*!
\file   FireEffect.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FIRE_EFFECT_H
#define _FIRE_EFFECT_H

#include "Effect.h"
#include "Particles.h"
#include "ParticleGenerators.h"
#include "ParticleUpdaters.h"
#include "ParticleRenderer.h"

namespace Framework
{
  class KeyEvent;
  class FireStarter;

  class FireSystem : public IEffect
  {
  public:
    FireSystem ();
    virtual ~FireSystem ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);

    virtual bool initialize (size_t numParticles);
    virtual bool initializeRenderer ();
    virtual void reset ();
    virtual void clean ();
    virtual void addUI (CTwBar*);
    virtual void removeUI (CTwBar*);
    virtual void update (double dt);
    virtual void cpuUpdate (double dt);
    virtual void gpuUpdate (double dt);
    virtual void render ();
    virtual int numAllParticles ();
    virtual int numAliveParticles ();
    virtual void Draw ();
    //virtual BoundType* ZilchGetDerivedType () const;

    void OnKeyPressed (KeyEvent* key);
    void AddFire (float x, float y, float emitRate);
    void RemoveFire(FireStarter* param1);

    const static std::string Name;
  private:
    std::shared_ptr<ParticleSystem> m_system;
    std::shared_ptr<IParticleRenderer> m_renderer;

    float size;
    Texture* texture;

    void AddFireEmitter (glm::vec3 position, float emitRate, glm::vec3 minVel, glm::vec3 maxVel);
    virtual void OnApplicationPause (PauseEvent* pause);
  };
}

#endif