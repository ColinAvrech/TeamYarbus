/******************************************************************************/
/*!
\file   CameraShake.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{

  DefineComponentName (CameraShake);

  // Constructor
  CameraShake::CameraShake ()
  {}
  
  // Destructor
  CameraShake::~CameraShake ()
  {}

  void CameraShake::Serialize (Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    data->FindElement (data, "Intensity")->GetValue (&shake);
    data->FindElement (data, "Duration")->GetValue (&duration);
  }

  void CameraShake::Initialize ()
  {
    originalPosition = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"))->GetPosition();
    EVENTSYSTEM->mConnect<UpdateEvent, CameraShake> (Events::UPDATEEVENT, this, &CameraShake::Update);
    EVENTSYSTEM->mConnect<PauseEvent, CameraShake> (Events::PAUSE, this, &CameraShake::OnApplicationPause);
  }

  void CameraShake::Update (UpdateEvent* update)
  {
    Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
    switch (state)
    {
    case Framework::CS_NONE:
      if (enabled)
      {
        state = CS_SHAKE;
      }
      break;
    case Framework::CS_SHAKE:
      time += 0.016f;
      if (enabled && time < duration)
      {
        tform->SetPosition
          (vec2(originalPosition.x + glm::linearRand (-shake.x / time, shake.x / time),
          originalPosition.y + glm::linearRand (-shake.y / time, shake.y / time)));
      }
      else
      {
        state = CS_END;
      }
      break;
    case Framework::CS_END:
      startPosition = tform->GetPosition ();
      time = 0.0f;
      state = CS_RESET;
      break;
    case Framework::CS_RESET:
      time += 0.016f;
      if (time < 1.0f)
      {
        glm::vec3 position = glm::mix (startPosition, originalPosition, time);
        tform->SetPosition (vec2(position.x, position.y));
      }
      else
      {
        time = 0.0f;
        enabled = false;
        state = CS_NONE;
      }
      break;
    default:
      break;
    }
  }

  void CameraShake::OnApplicationPause (PauseEvent* pause)
  {
    //enabled = pause->Paused;
  }

}