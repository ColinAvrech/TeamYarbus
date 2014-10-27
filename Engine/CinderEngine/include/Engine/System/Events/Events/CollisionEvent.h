/******************************************************************************/
/*!
\file   InputEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseEvent.h"

namespace Framework
{

  class CollisionEvent : public BaseEvent
  {
  public:

  GameObject * OtherObject;
	GameObject * thisObject;
	float penetration;
	glm::vec2 normal;

  };

}