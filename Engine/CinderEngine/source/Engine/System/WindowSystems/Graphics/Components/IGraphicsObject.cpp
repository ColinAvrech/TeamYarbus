/******************************************************************************/
/*!
\file   IGraphicsObject.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "IGraphicsObject.h"
#include "WindowSystem.h"

namespace Framework
{

  // Constructor
  IGraphicsObject::IGraphicsObject ()
  {
    Register ();
  }
  
  // Destructor
  IGraphicsObject::~IGraphicsObject ()
  {
    Deregister ();
  }


  void IGraphicsObject::Register ()
  {
    WindowSystem::graphicsObjects.push_back (this);
  }

  void IGraphicsObject::Deregister ()
  {
    WindowSystem::graphicsObjects.remove (this);
  }

  void IGraphicsObject::Draw ()
  {
  }

}