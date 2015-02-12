/******************************************************************************/
/*!
\file   EndEventListener.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  DefineComponentName(EndEventListener);

  // Constructor
  EndEventListener::EndEventListener()
  {
    creditsMusic = Resources::RS->Get_Sound("CreditsMusic.ogg");
    for (auto guitext : OPENGL->textObjects)
    {
      guitext->ToggleVisibility();
    }
  }

  // Destructor
  EndEventListener::~EndEventListener()
  {
    creditsMusic->Stop();
    EVENTSYSTEM->mDisconnect<UpdateEvent, EndEventListener>(Events::UPDATEEVENT, this, &EndEventListener::Update);
    EVENTSYSTEM->mDisconnect<EndEvent, EndEventListener>(Events::END_EVENT, this, &EndEventListener::EndEventHandler);
    for (auto guitext : OPENGL->textObjects)
    {
      guitext->ToggleVisibility();
    }
  }

  void EndEventListener::Initialize()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, EndEventListener>(Events::UPDATEEVENT, this, &EndEventListener::Update);
    EVENTSYSTEM->mConnect<EndEvent, EndEventListener>(Events::END_EVENT, this, &EndEventListener::EndEventHandler);

    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    Sprite* sprite = static_cast<Sprite*>(gameObject->GetComponent("Sprite"));

    vec3 worldpos = tform->GetPosition();
    vec3 pos = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"))->GetPosition();
    tform->SetPosition(vec2(worldpos.x, pos.y - (Camera::main->GetSize() * 4.0f)));
    sprite->enabled = false;
    endPosition = { -20, 110, -10 };
  }

  void EndEventListener::Serialize(Serializer::DataNode* data)
  {
  }

  void EndEventListener::OnApplicationPause(PauseEvent* pause)
  {
    throw std::logic_error("The method or operation is not implemented.");
  }

  void EndEventListener::Update(UpdateEvent* update)
  {
    if (loadCredits)
    {
      Transform* camTransform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
      Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
      Sprite* sprite = static_cast<Sprite*>(gameObject->GetComponent("Sprite"));
      switch (ees)
      {
      case Framework::EES_NONE:
        startPosition = camTransform->GetPosition();
        startSize = Camera::main->size;
        endSize = 310.0f;
        ees = EES_CAMERA;
        break;
      case Framework::EES_CAMERA:
      {
        timer += update->Dt * 0.5f;
        glm::vec3 pos = glm::mix(startPosition, endPosition, timer);
        Camera::main->size = glm::mix(startSize, endSize, timer);
        if (timer < 1.0f)
        {
          camTransform->SetPosition(vec2(pos.x, pos.y));
        }
        else
        {
          tform->SetPosition(vec2(pos.x, pos.y - 400));
          ees = EES_CREDITS;
        }
      }
        break;
      case Framework::EES_CREDITS:
        WINDOWSYSTEM->SetCursorVisibility(true);
        tform->Scale(300 * sprite->Get_Texture()->Get_Aspect_Ratio(), 300, 1);
        sprite->enabled = true;
        tform->Translate(0, update->Dt * 10, 0);
        if (exitGame == nullptr)
        {
          std::string name = "ExitButton";
          exitGame = OBJECTSYSTEM->FindObjectByName(name.c_str());
          static_cast<Sprite*>(exitGame->GetComponent("Sprite"))->Change_Color(1.0f, 1.0f, 1.0f, 1.0f);
          string name2 = "RestartButton";
          GameObject* restartGame = OBJECTSYSTEM->FindObjectByName(name2.c_str());
          static_cast<Sprite*>(restartGame->GetComponent("Sprite"))->Change_Color(1.0f, 1.0f, 1.0f, 1.0f);
        }
        CharacterController::PLAYER->useFlying = true;
        break;
      default:
        break;
      }
    }
  }

  void EndEventListener::EndEventHandler(EndEvent* endEvent)
  {
    AUDIOEVENTS->unmanagedSounds.push_back(creditsMusic);
    creditsMusic->Play();
    loadCredits = true;
  }

}