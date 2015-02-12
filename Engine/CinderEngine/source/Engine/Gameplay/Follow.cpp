/******************************************************************************/
/*!
\file   Follow.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  DefineComponentName (Follow);

  // Constructor
  Follow::Follow ()
  {
    target = nullptr;
    targetName = "";
  }
  
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
    SetTarget(targetName);
  }

  void Follow::SetTarget(string newTargetName)
  {
    targetName = newTargetName;
    Transform* newTarget = static_cast<Transform*>(OBJECTSYSTEM->FindObjectByName(targetName.c_str())->GetComponent("Transform"));
    SetTarget(newTarget);
  }

  void Follow::SetTarget(Transform* newTarget)
  {
    target = newTarget;
    EVENTSYSTEM->mConnect<UpdateEvent, Follow>(Events::UPDATEEVENT, this, &Follow::Update);
  }

  void Follow::Update (UpdateEvent* update)
  {
    Transform* tform = gameObject->C<Transform>();
    if (tform && target)
      tform->SetPosition(target->GetPosition2D());
  }

  void Follow::OnApplicationPause (PauseEvent* pause)
  {
  }

}