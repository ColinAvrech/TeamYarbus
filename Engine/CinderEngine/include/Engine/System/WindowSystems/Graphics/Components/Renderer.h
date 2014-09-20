/******************************************************************************/
/*!
\file   Renderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _RENDERER_H
#define _RENDERER_H

#include "ComponentInclude.h"

namespace Framework
{
  class Renderer : public GameComponent
  {
  public:
  Renderer (GameObject* obj);
  ~Renderer ();
  
  // The non-base component usees DefineComponentName macro to name component
  const static std::string Name;
  private:

  };  
}

#endif