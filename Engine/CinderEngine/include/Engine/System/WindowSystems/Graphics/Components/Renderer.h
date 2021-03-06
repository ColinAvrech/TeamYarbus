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
  class Renderer : public Component
  {
  public:
  Renderer ();
  ~Renderer ();
  
  // The non-base component usees DefineComponentName macro to name component
  const static string Name;
  private:

  };  
}

#endif