/******************************************************************************/
/*!
\file   EventSystem.tpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Delegates.h"

namespace Framework
{

  // C/C++ Global functions use this connect function to conntect to events.
  template<typename EventType>
  void EventSystem::gConnect(const string eventname, void(*func)(EventType*))
  {
    auto eDeployer = RegisteredEvents.find(eventname);

    //If deployer does not exist
    if (eDeployer == RegisteredEvents.end())
    {
      //Add Deployer
      RegisteredEvents[eventname] = new EventDeployer();

      //Add Delegate
      GlobalDelegate<EventType>* gd = new GlobalDelegate<EventType>();
      gd->Function = func;
      RegisteredEvents[eventname]->AddDelegate(gd);
    }
    else
    {
      //Add Delegate
      GlobalDelegate<EventType>* gd = new GlobalDelegate<EventType>();
      gd->Function = func;
      RegisteredEvents[eventname]->AddDelegate(gd);
    }
  }

  
  //Global functions use this connect function to conntect to events.
  template<typename EventType>
  void EventSystem::gDisconnect(const string eventname, void(*func)(EventType*))
  {
    EventDeployer* eDeployer = GetEventDeployer(eventname);
	if(eDeployer != nullptr)
	{
	  for (auto it = eDeployer->Delegates.begin(); it != eDeployer->Delegates.end(); ++it)
      {
        auto gd = static_cast<GlobalDelegate<EventType>*>(*it);
        if (gd->Function == func)
        {
          delete gd;
          eDeployer->Delegates.erase(it);
		  return;
        }
      }
	}
	return;
  }

  // C++ Member functions use this connect function to connect to events.
  template<typename EventType, typename ClassType>
  void EventSystem::mConnect(const string eventname, ClassType *this_ptr, void(ClassType::*func)(EventType*))
  {
    auto eDeployer = RegisteredEvents.find(eventname);

    //If deployer does not exist
    if (eDeployer == RegisteredEvents.end())
    {
      //Add deployer
      RegisteredEvents[eventname] = new EventDeployer();

      //Add Delegate
      MemberDelegate<EventType, ClassType>* md = new MemberDelegate<EventType, ClassType>();
      md->Function = func;
      md->This_ptr = this_ptr;
      RegisteredEvents[eventname]->AddDelegate(md);
    }
    else
    {
      //Add Delegate
      MemberDelegate<EventType, ClassType>* md = new MemberDelegate<EventType, ClassType>();
      md->Function = func;
      md->This_ptr = this_ptr;
      eDeployer->second->AddDelegate(md);
    }
  }
  
  // C++ Member functions use this connect function to connect to events.
  template<typename EventType, typename ClassType>
  void EventSystem::mDisconnect(const string eventname, ClassType *this_ptr, void(ClassType::*func)(EventType*))
  {
    EventDeployer* eDeployer = GetEventDeployer(eventname);
	if(eDeployer != nullptr)
	{
	  for (auto it = eDeployer->Delegates.begin(); it != eDeployer->Delegates.end(); ++it)
      {
        auto md = static_cast<MemberDelegate<EventType, ClassType>*>(*it);
        if (md->This_ptr == this_ptr && md->Function == func)
        {
          delete md;
          eDeployer->Delegates.erase(it);
		  return;
        }
      }
	}
	return;
  }

} //namespace Framework
