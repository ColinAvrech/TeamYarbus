/******************************************************************************/
/*!
\file   Follow.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Follow.h"
#include "ObjectSystem.h"
#include "EventSystem.h"
#include "UpdateEvent.h"
#include "Events.h"

namespace Framework
{
  DefineComponentName (Follow);

  // Constructor
  Follow::Follow ()
  {}
  
  // Destructor
  Follow::~Follow ()
  {}

  void Follow::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Target");
    value->GetValue (&targetName);
  }

  void Follow::Initialize ()
  {
    target = OBJECTSYSTEM->FindObjectByName (targetName.c_str())->Transform;
    EVENTSYSTEM->mConnect<UpdateEvent, Follow> (Events::UPDATEEVENT, this, &Follow::Update);
  }

  void Follow::Update (UpdateEvent* update)
  {
    gameObject->Transform->SetPosition (target->GetPosition().x, target->GetPosition().y);
  }

  void Follow::OnApplicationPause (PauseEvent* pause)
  {
  }

}