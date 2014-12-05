/******************************************************************************/
/*!
\file   CollisionEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseEvent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{

  class CollisionEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(CollisionEvent, Zilch::TypeCopyMode::ReferenceType);

  GameObject * OtherObject;
	GameObject * thisObject;
	float Penetration;
	vec3 normal;

	float Dt;
  };

}