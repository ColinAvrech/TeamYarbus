/******************************************************************************/
/*!
\file   ParticleRenderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PARTICLE_RENDERER_H
#define _PARTICLE_RENDERER_H

#include <memory>

namespace Framework
{

  // PARTICLE RENDERER INTERFACE
  // ALL RENDERERS WILL INHERIT FROM THIS CLASS
  class ParticleSystem;

  class IParticleRenderer
  {
  public:
	  ZilchDeclareBaseType(IParticleRenderer, TypeCopyMode::ReferenceType);
    IParticleRenderer () { }
    virtual ~IParticleRenderer () { }

    virtual void generate (ParticleSystem *sys, bool useQuads) = 0;
    virtual void destroy () = 0;
    virtual void update () = 0;
    virtual void render () = 0;
  };

  class RendererFactory
  {
  public:
    static std::shared_ptr<IParticleRenderer> create (const char *name);
  };
}

#endif