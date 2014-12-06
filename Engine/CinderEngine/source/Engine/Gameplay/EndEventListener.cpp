/******************************************************************************/
/*!
\file   EndEventListener.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "EndEventListener.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include "UpdateEvent.h"
#include "GameObject.h"
#include "Sprite.h"
#include "CinderEngine_UI.h"
#include "CharacterController.h"
#include "ObjectSystem.h"
#include "WindowSystem.h"

namespace Framework
{
  DefineComponentName (EndEventListener);

  // Constructor
  EndEventListener::EndEventListener ()
  {}
  
  // Destructor
  EndEventListener::~EndEventListener ()
  {}

  void EndEventListener::Initialize ()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, EndEventListener> (Events::UPDATEEVENT, this, &EndEventListener::Update);
    EVENTSYSTEM->mConnect<EndEvent, EndEventListener> (Events::END_EVENT, this, &EndEventListener::EndEventHandler);
    glm::vec3 pos = Camera::main->gameObject->Transform->GetPosition ();
    gameObject->Transform->SetPosition (pos.x, pos.y - (Camera::main->GetSize() * 4.0f));
    gameObject->Sprite->enabled = false;
    endPosition = { -30, 110, -10 };
  }

  void EndEventListener::Serialize (Serializer::DataNode* data)
  {
  }

  void EndEventListener::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void EndEventListener::Update (UpdateEvent* update)
  {
    if (loadCredits)
    {
      switch (ees)
      {
      case Framework::EES_NONE:
        startPosition = Camera::main->gameObject->Transform->GetPosition ();
        startSize = Camera::main->size;
        endSize = 310.0f;
        ees = EES_CAMERA;
        break;
      case Framework::EES_CAMERA:
      {timer += 0.016f * 0.5f;
      glm::vec3 pos = glm::mix (startPosition, endPosition, timer);
      Camera::main->size = glm::mix (startSize, endSize, timer);
      if (timer < 1.0f)
      {
        Camera::main->gameObject->Transform->SetPosition (pos.x, pos.y);
      }
      else
      {
        gameObject->Transform->SetPosition (pos.x, pos.y - 300);
        ees = EES_CREDITS;
      }}
        break;
      case Framework::EES_CREDITS:
        WINDOWSYSTEM->SetCursorVisibility(true);
        gameObject->Transform->Scale (250 * gameObject->Sprite->Get_Texture ()->Get_Aspect_Ratio(), 250, 1);
        gameObject->Sprite->enabled = true;
        gameObject->Transform->Translate (0, 0.016f * 10, 0);
        if (exitGame == nullptr)
        {
          std::string name = "ExitButton";
          exitGame = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName (name)->GetComponent("UIBox"));
          if (exitGame != nullptr)
          {
            //exitGame->gameObject->Transform->SetPosition (40, 0);
            exitGame->enabled = true;
          }
        }
        CharacterController::PLAYER->useFlying = true;
        break;
      default:
        break;
      }
    }
  }

  void EndEventListener::EndEventHandler (EndEvent* endEvent)
  {
    loadCredits = true;
  }

}