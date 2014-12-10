/******************************************************************************/
/*!
\file   CharacterController.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Gamelogic for the player character
*/
/******************************************************************************/

#include "CharacterController.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "EventSystem.h"
#include "CollisionEvent.h"
#include "KeyEvent.h"
#include "UpdateEvent.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "InputManager.h"
#include "glfw3.h"


namespace Framework
{
  CharacterController* CharacterController::PLAYER = nullptr;

  ZilchDefineType (CharacterController, CinderZilch)
  {
    ZilchBindStaticFieldGetAs(PLAYER, "Player");
    ZilchBindMethodAs(ToggleFlying, "ToggleFlying");
    //type->HandleManager = ZilchManagerId (Zilch::PointerManager);
    //ZilchBindStaticMethodAs(ZGetCameraMousePosition, "GetCameraMousePosition");
  }

  static bool onGround = false;

  CharacterController::CharacterController ()
  {
    PLAYER = this;
    OBJECTSYSTEM->ptrPlayer = this->gameObject;
    useFlying = false;
  }

  CharacterController::~CharacterController ()
  {
    EVENTSYSTEM->mDisconnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mDisconnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);
    PLAYER = nullptr;
    OBJECTSYSTEM->ptrPlayer = nullptr;
  }

  void CharacterController::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* element = data->FindElement(data, "MicrophoneMultiplier");
    if (element)
      element->GetValue(&micMultiplier);
    else
      micMultiplier = vec2(0.0f, 10.0f);
    
    element = data->FindElement(data, "Acceleration");
    if (element)
      element->GetValue(&acceleration);
    else
      acceleration = vec2(400.0f, 6000.0f);
    
    element = data->FindElement(data, "JumpVelocity");
    if (element)
      element->GetValue(&jumpVel);
    else
      jumpVel = vec2(0.0f, 10.0f);
    
    element = data->FindElement(data, "UseFlying");
    if (element)
      element->GetValue(&useFlying);
    else
      useFlying = false;
  }

  void CharacterController::Initialize ()
  {
    PLAYER = this;
    OBJECTSYSTEM->ptrPlayer = this->gameObject;
    //accel = { 0 , 0 };
    //maxAcceleration = { 50, 100 };
    maxVel = 20.0f;
    //drag = 5;
    //currentforce = 0;
    density = gameObject->ShapeCollider2D->Density;

    EVENTSYSTEM->mConnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mConnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);

    AUDIOSYSTEM->listener = gameObject->Transform;
    
    vec3 pos = gameObject->Transform->GetPosition();
    Camera::main->gameObject->Transform->SetPosition(pos.x, pos.y);
  }

  static void UpdateGroundState(CollisionEvent* collision)
  {
    glm::vec2 normal = glm::vec2(collision->normal);
    if (Physics::Angle_from_Vertical(normal) < 3.14f / 3.f)
      onGround = true;
    else
      onGround = false;
  }

  void CharacterController::OnCollisionEnter (CollisionEvent* collision)
  {
    UpdateGroundState(collision);
  }

  static float t = 1;

  void CharacterController::Update(UpdateEvent* e)
  {
    RigidBody2D* body = gameObject->RigidBody2D;
    
    ErrorIf(body == nullptr, "ERROR, Character controller is missing RigidBody2D");

    Health* hp = gameObject->Health;
    // if we have died
    if (hp && !hp->alive)
    {
      return;
    }

    // Fly up
    if (InputManager::IsKeyDown(GLFW_KEY_UP) && (onGround || useFlying) && (body->velocity.y < 9.5f))
    {
      onGround = false;
      body->ApplyForce(Vector2(0, 4000 * density));
        
      if (hp)
      {
        hp->currentDeathRate = hp->startingDeathRate * 20.0f; // only when you jump
      }
    }
    else // we are not flying
    {
      if (hp)
      {
        hp->currentDeathRate = hp->startingDeathRate;
      }
    }

    // Go down
    if (InputManager::IsKeyDown(GLFW_KEY_DOWN) && (body->velocity.y > -9.5f))
    {
      body->ApplyForce(Vector2(0, -2000 * density));
    }

    if (InputManager::IsKeyDown(GLFW_KEY_RIGHT) && body->velocity.x < 50.0f)
      body->ApplyForce(Vector2(acceleration.x * density, 0));
 
    if (InputManager::IsKeyDown(GLFW_KEY_LEFT) && body->velocity.x > -50.0f)
      body->ApplyForce(Vector2(-acceleration.x * density,0));
    
    if (InputManager::IsKeyTriggered(GLFW_KEY_R))
    {
      OBJECTSYSTEM->RestartLevel();
    }
    
    // Microphone input
    gridPos = gameObject->Transform->GetGridPosition ();
    float micValue = AUDIOSYSTEM->GetMicrophoneValue ();
    body->ApplyForce(Vector2(micValue * micMultiplier.x * density,micValue * micMultiplier.y * density));
    Physics::THERMODYNAMICS->SetCellTemperature (gridPos.x, gridPos.y, 400000, e->Dt);
  }

  bool CharacterController::CanFly() const
  {
    return useFlying;
  }

  void CharacterController::ToggleFlying()
  {
    useFlying = !useFlying;
  }

  DefineComponentName (CharacterController);
}
