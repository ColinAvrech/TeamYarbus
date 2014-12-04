/******************************************************************************/
/*!
\file   Events.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/



#include "Common.h"
#include "Events.h"
#include "ZilchCompiledLib.h"

#include "GameEvent.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "CollisionEvent.h"
#include "PingEvent.h"
#include "MenuChangeEvent.h"
//#include "UpdateEvent.h"
//#include "UpdateEvent.h"

namespace Framework
{
  
  string CollisionEventName(unsigned GameObjectID)
  {
    string ColEventStr = string("C");
    ColEventStr.append(std::to_string(GameObjectID));
    return ColEventStr;
  }


  ZilchDefineType(UpdateEvent, CinderZilch)
  {
    type->HandleManager = ZilchManagerId(PointerManager);
    ZilchBindFieldGet(Dt);
    ZilchBindFieldGet(TimePassed);
  }

  ZilchDefineType(KeyEvent, CinderZilch)
  {
    type->HandleManager = ZilchManagerId(PointerManager);
    ZilchBindFieldGet(KeyValue);
    ZilchBindFieldGet(KeyDown);
    ZilchBindFieldGet(KeyRepeat);
    ZilchBindFieldGet(CTRLPressed);
    ZilchBindFieldGet(ALTPressed);
    ZilchBindFieldGet(SHIFTPressed);
  }

  ZilchDefineType(CollisionEvent, CinderZilch)
  {
    type->HandleManager = ZilchManagerId(PointerManager);
    ZilchBindFieldGet(Penetration);

    //Need to add Game Object to zilch Library Before using these
    ZilchBindFieldGetAs(thisObject, "Object");
    ZilchBindFieldGet(OtherObject);
	//ZilchBindFieldGetAs(normal, "Normal");
  }

  ZilchDefineType(PingEvent, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(PointerManager);
	  ZilchBindFieldGet(Ping);

  }
  ZilchDefineType(MenuChangeEvent, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(PointerManager);
  }

  ZilchDefineType(UIEvent, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(PointerManager);
	  ZilchBindFieldGetSet(Message);
  }

  ZilchDefineType(GameEvent, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(PointerManager);
	  
  }
  ZilchDefineType(PauseEvent, CinderZilch)
  {
	  ZilchBindFieldGet(Paused);
  }

}