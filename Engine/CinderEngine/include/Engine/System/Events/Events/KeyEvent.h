/******************************************************************************/
/*!
\file   InputEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Sends an event Whenever there is a key state change
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseEvent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{

  class KeyEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(KeyEvent, Zilch::TypeCopyMode::ReferenceType);
    //Key value as dictated by GLFW
    int KeyValue;


    // Set the Key's State
    bool KeyDown;
    bool KeyRepeat;

    // Active Key Modifiers
    bool CTRLPressed;
    bool ALTPressed;
    bool SHIFTPressed;
  };
}