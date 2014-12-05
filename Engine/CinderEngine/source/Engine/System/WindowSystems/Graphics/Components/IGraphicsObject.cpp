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
	/*
	ZilchDefineType(Shader, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	}
	*/
	ZilchDefineType(IGraphicsObject, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethodAs(Change_Layer, "ChangeLayer");
	}
  // Constructor
  IGraphicsObject::IGraphicsObject ()
  {
    //shader = nullptr;
    layer = DEFAULT;
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
    OPENGL->graphicsObjects [layer].push_back (this);
  }

  void IGraphicsObject::Deregister ()
  {
    //WindowSystem::graphicsObjects.remove (this);
    OPENGL->graphicsObjects [layer].remove (this);
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

  void IGraphicsObject::Change_Layer (int lay)
  {
    OPENGL->graphicsObjects [layer].remove (this);
    layer = LAYER (lay);
    OPENGL->graphicsObjects [layer].push_back (this);
  }

}