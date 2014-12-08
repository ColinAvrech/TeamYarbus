/******************************************************************************/
/*!
\file   EndEventListener.cpp
\author Manas Sudhir Kulkarni
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
#include "Pipeline.h"
#include "ResourceManager.h"

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
    OPENGL->textObjects.clear();
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
      {timer += update->Dt * 0.5f;
      glm::vec3 pos = glm::mix (startPosition, endPosition, timer);
      Camera::main->size = glm::mix (startSize, endSize, timer);
      if (timer < 1.0f)
      {
        Camera::main->gameObject->Transform->SetPosition (pos.x, pos.y);
      }
      else
      {
        gameObject->Transform->SetPosition (pos.x, pos.y - 400);
        ees = EES_CREDITS;
      }}
        break;
      case Framework::EES_CREDITS:
        WINDOWSYSTEM->SetCursorVisibility(true);
        gameObject->Transform->Scale (300 * gameObject->Sprite->Get_Texture ()->Get_Aspect_Ratio(), 300, 1);
        gameObject->Sprite->enabled = true;
        gameObject->Transform->Translate (0, update->Dt * 10, 0);
        if (exitGame == nullptr)
        {
          std::string name = "ExitButton";
          exitGame = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName (name.c_str())->GetComponent("UIBox"));
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
    Sound *creditsMusic = Resources::RS->Get_Sound("CreditsMusic.ogg");
    creditsMusic->Play();
    loadCredits = true;
  }

}