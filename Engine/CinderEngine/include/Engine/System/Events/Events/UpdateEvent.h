/******************************************************************************/
/*!
\file   UpdateEvent.h
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

  class UpdateEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(UpdateEvent, Zilch::TypeCopyMode::ReferenceType);

    float Dt;
    float TimePassed;
  };

}