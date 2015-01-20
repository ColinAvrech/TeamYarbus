/******************************************************************************/
/*!
\file   ParticleRenderer.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "ParticleRenderer.h"
#include <string>
#include "GLParticleRenderer.h"

namespace Framework
{
  std::shared_ptr<IParticleRenderer> RendererFactory::create (const char *name)
  {
    string renderer{ name };

    if (renderer == "gl")
      return std::make_shared<GLParticleRenderer> ();

    return nullptr;
  }
}