/******************************************************************************/
/*!
\file   MenuChangeEvent.h
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

  class MenuChangeEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(MenuChangeEvent, Zilch::TypeCopyMode::ReferenceType);

    unsigned ActiveState = 0;

    // Function call which may or may not be called.
    typedef void(*MenuAction)(void);
    MenuAction Action = nullptr;
  };
}