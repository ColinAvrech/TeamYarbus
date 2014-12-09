/******************************************************************************/
/*!
\file   EndTrigger.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "EndTrigger.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include "CollisionEvent.h"

namespace Framework
{
  DefineComponentName (EndTrigger);

  // Constructor
  EndTrigger::EndTrigger ()
  {}
  
  // Destructor
  EndTrigger::~EndTrigger ()
  {}

  void EndTrigger::Initialize ()
  {
    EVENTSYSTEM->mConnect<CollisionEvent, EndTrigger> (Events::COLLISION, this, &EndTrigger::OnCollisionEnter);
  }

  void EndTrigger::Serialize (Serializer::DataNode* data)
  {
  }

  void EndTrigger::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void EndTrigger::OnCollisionEnter (CollisionEvent* coll)
  {
    if ( !triggered && coll && coll->thisObject && coll->OtherObject
      && coll->thisObject->Name == gameObject->Name && coll->OtherObject->Name == "Player")
    {
      triggered = true;
      std::cout << "CREDITS TRIGGERED\n";
      BaseEvent b;
      EVENTSYSTEM->TriggerEvent(Events::END_EVENT, b);
    }
  }
}