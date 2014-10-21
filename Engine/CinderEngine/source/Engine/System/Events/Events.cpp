/******************************************************************************/
/*!
\file   EventSystem.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/




#include "Events.h"

namespace Framework
{
  std::string CollisionEventName(unsigned GameObjectID)
  {
    std::string ColEventStr = std::string("C");
    ColEventStr.append(std::to_string(GameObjectID));
    return ColEventStr;
  }
}