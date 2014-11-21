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
#include "Pipeline.h"

namespace Framework
{

  // Constructor
  IGraphicsObject::IGraphicsObject ()
  {
    //shader = nullptr;
    Register ();
  }
  
  // Destructor
  IGraphicsObject::~IGraphicsObject ()
  {
    Deregister ();
  }


  void IGraphicsObject::Register ()
  {
    //WindowSystem::graphicsObjects.push_back (this);
    OPENGL->graphicsObjects.push_back (this);
  }

  void IGraphicsObject::Deregister ()
  {
    //WindowSystem::graphicsObjects.remove (this);
    OPENGL->graphicsObjects.remove (this);
  }

  void IGraphicsObject::Update ()
  {
    if (shader != nullptr)
    {
      shader->Use ();
      shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
      shader->Disable ();
    }
  }

  void IGraphicsObject::Draw ()
  {
  }

}