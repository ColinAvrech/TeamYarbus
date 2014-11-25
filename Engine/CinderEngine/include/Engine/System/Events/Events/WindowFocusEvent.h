/******************************************************************************/
/*!
\file   WindowFocusEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  For alt-tab
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseEvent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{

  class WindowFocusEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(WindowFocusEvent, Zilch::TypeCopyMode::ReferenceType);

    int InFocus;
  };

}
