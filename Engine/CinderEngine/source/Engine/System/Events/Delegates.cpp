/******************************************************************************/
/*!
\file   Delegates.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/


#include "Common.h"
#include "Delegates.h"


namespace Framework
{



  void EventDeployer::TriggerEvent(BaseEvent& e)
  {
    for each(auto del in Delegates)
    {
      del->Invoke(&e);
    }
  }

  void EventDeployer::DisconnectAll()
  {
    for each(auto del in Delegates)
    {
      delete del;
    }
    Delegates.clear();
  }

}