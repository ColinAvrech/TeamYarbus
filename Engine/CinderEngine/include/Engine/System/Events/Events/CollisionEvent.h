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
    typedef void(*CollisionCall)(GameObject*, CollisionEvent*);

    CollisionEvent(const std::string eventname)
    {
	}

    ~CollisionEvent()
    {}
    
    void DispatchEvent()
    {
      for each(auto it in Listeners)
      {
        ((CollisionCall)(it.second))(it.first, this);
      }
    }
    
    GameObject * OtherObject;
	GameObject * thisObject;
	//float penetration;
	glm::vec2 normal;

  };

}