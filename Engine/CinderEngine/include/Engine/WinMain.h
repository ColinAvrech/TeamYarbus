/******************************************************************************/
/*!
\file   WinMain.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  WinMain.cpp is the starting place for the engine. This initializes all the systems and
starts the game loop.
*/
/******************************************************************************/

#pragma once

#include "Zilch.hpp"


//#include "EventSystem.h"
//#include "ObjectSystem.h"
//#include "WindowSystem.h"

namespace Framework
{
  class CoreEngine;
  class WindowSystem;
  class AudioSystem;
  class EventSystem;
  class ScriptSystem;
  class ObjectSystem;

 class Cinder
  {
    ZilchDeclareBaseType(Cinder, Zilch::TypeCopyMode::ReferenceType);

    EventSystem* GetEventSystem();
    ObjectSystem* GetObjectSystem();
  };

}
