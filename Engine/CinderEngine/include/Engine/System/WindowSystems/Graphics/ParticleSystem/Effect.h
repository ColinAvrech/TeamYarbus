/******************************************************************************/
/*!
\file   Effect.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _EFFECT_H
#define _EFFECT_H

#include <memory>
#include "AntTweakBar.h"
#include "Texture.h"
#include "Shader.h"
#include "IGraphicsObject.h"

namespace Framework
{
  class IEffect : public IGraphicsObject
  {
  public:
    static const size_t DEFAULT_PARTICLE_NUM_FLAG = 0;	// for initialize method
    //enum Name { };
  public:
    IEffect () { }
    virtual ~IEffect () { }
    Shader* shader;
    virtual void Initialize () {}
    virtual void Serialize (Serializer::DataNode* data) {}

    // creates the effect with desired num of particles, (0 means default for the effect)
    virtual bool initialize (size_t numParticles) = 0;
    virtual bool initializeRenderer () = 0;
    virtual void reset () = 0;
    virtual void clean () = 0;
    virtual void addUI (CTwBar*) = 0;
    virtual void removeUI (CTwBar*) = 0;

    virtual void update (double dt) = 0;
    virtual void cpuUpdate (double dt) = 0;
    virtual void gpuUpdate (double dt) = 0;
    virtual void render () = 0;

    virtual int numAllParticles () = 0;
    virtual int numAliveParticles () = 0;

    const static std::string Name;
  };

  class EffectFactory
  {
  public:
    static std::shared_ptr<IEffect> create (const char *name);
  };
}

#endif