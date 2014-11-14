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
    //static CoreEngine * Engine;
    //static WindowSystem * Windows;
    //static AudioSystem* Audio;
    static EventSystem * CinderEvents;
    //static ScriptSystem * Zilch;
    // I hate namespaces
    //Physics::ThermodynamicsSystem * Thermo;
    //Physics::PhysicsSystem * Physics;
    static ObjectSystem* ObjectSystem;
  };

}
