/******************************************************************************/
/*!
\file   Renderer.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Renderer.h"

namespace Framework
{

  DefineComponentName (Renderer);

  // Constructor
  Renderer::Renderer (GameObject* obj) : GameComponent (obj)
  {
  }
  
  // Destructor
  Renderer::~Renderer ()
  {}
}