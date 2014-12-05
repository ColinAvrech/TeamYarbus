/******************************************************************************/
/*!
\file   FirePointController.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Main Player Controller

*/
/******************************************************************************/


#include "Common.h"
#include "FirePointController.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "AudioSystem.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "ObjectSystem.h"
#include "AudioSystem.h"
#include "PingEvent.h"

namespace Framework
{
  DefineComponentName(FirePointController);


  FirePointController::FirePointController(){}

  FirePointController::~FirePointController()
  {
	  EVENTSYSTEM->mDisconnect<UpdateEvent, FirePointController>(Events::UPDATEEVENT, this, &FirePointController::Update);
  }

  void FirePointController::Initialize()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, FirePointController>(Events::UPDATEEVENT, this, &FirePointController::Update);

    CreateFireStarters();
  }
  
  void FirePointController::Update(UpdateEvent* e)
  {

  }

  void FirePointController::Serialize(Serializer::DataNode* data)
  {

  }

  void FirePointController::CreateFireStarters()
  {
    unsigned i = 20;
    while (i > 0)
    {
      GameObject* obj = OBJECTSYSTEM->CreateObject();

      // Create FireStarters
      FireStarters.push_back(obj);
      obj->Name = "FireStarter";

      // Transform
      obj->AddComponent("Transform")->Initialize();
      obj->Transform->Translate(gameObject->Transform->GetPosition());

      // Sphere Collider
      obj->AddComponent("SphereCollider")->Initialize();

      // Ping the creation of the FireStarter Object
      PingEvent e;
      e.Ping = obj;
      EVENTSYSTEM->TriggerEvent(Events::PING_GAMEOBJECT, e);

      --i;
    }
  }

}