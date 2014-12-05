/******************************************************************************/
/*!
\file   PingEvent.h
\author Micah
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/


#pragma once

#include "Common.h"
#include "BaseEvent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{
  class GameObject;

  class PingEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(PingEvent, Zilch::TypeCopyMode::ReferenceType);

    GameObject * Ping;
  };
}