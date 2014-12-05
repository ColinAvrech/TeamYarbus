/******************************************************************************/
/*!
\file   CharacterController.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Gamelogic for the player character
*/
/******************************************************************************/

#include "Common.h"
#include "CharacterController.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "TimeQuery.h"
#include "glmOverloads.h"
#include "PhysicsLibrary.h"
#include "IncludeForAllCollision.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "EventSystem.h"
#include "CollisionEvent.h"
#include "KeyEvent.h"
#include "UpdateEvent.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "InputManager.h"


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
    useFlying = false;
  }

  CharacterController::~CharacterController ()
  {
    EVENTSYSTEM->mDisconnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mDisconnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);
    PLAYER = nullptr;
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

  void CharacterController::Update (UpdateEvent* e)
  {
    RigidBody2D* body = gameObject->RigidBody2D;

    if (!body)
      return;

    if (InputManager::IsKeyDown(GLFW_KEY_UP))
    {
      if (useFlying)
      {
        body->velocity.y += jumpVel.y;
      }
      if (onGround)
      {
        body->velocity.y += jumpVel.y;
      }

      onGround = false;
    }
    
    if (InputManager::IsKeyDown(GLFW_KEY_RIGHT))
    {
      if (body->force.x < 0.4f && body->force.x > -0.4f)
        body->ApplyForce(Vector2(acceleration.x * density,acceleration.y * density));
    }
    if (InputManager::IsKeyDown(GLFW_KEY_LEFT))
    {
      if (body->force.x < 0.4f && body->force.x > -0.4f)
        body->ApplyForce(Vector2(-acceleration.x * density,acceleration.y * density));
    }
    
    if (InputManager::IsKeyDown(GLFW_KEY_R))
    {
      glm::vec2 position = glm::vec2(0, 500);
      body->position = Vector2(position.x, position.y);
      body->angularVelocity = 0.0f;
      body->velocity = Vector2(0, 0);
      body->force = Vector2(0, 0);
      Camera::main->gameObject->Transform->SetPosition(position.x, position.y);
    }

    /*
    Vector2* vel = &body->velocity;
    if (vel->Len() > maxVel)
    {
      vel->Normalize();
      *vel = *vel * maxVel;
    }
    */

    // Microphone input
    gridPos = gameObject->Transform->GetGridPosition ();
    float micValue = AUDIOSYSTEM->GetMicrophoneValue ();
    body->ApplyForce(Vector2(micValue * microhponeMultiplier.x * density,micValue * microhponeMultiplier.y * density));
    Physics::THERMODYNAMICS->SetCellTemperature (gridPos.x, gridPos.y, 400000, 0.016);

  }


  /*!Telegraph that the component is active*/
  void CharacterController::Initialize ()
  {
    OBJECTSYSTEM->ptrPlayer = this->gameObject;
    PLAYER = this;
    //accel = { 0 , 0 };
    //maxAcceleration = { 50, 100 };
    maxVel = 20.0f;
    //drag = 5;
    //currentforce = 0;
    density = gameObject->ShapeCollider2D->Density;

    EVENTSYSTEM->mConnect<CollisionEvent, CharacterController> (Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mConnect<UpdateEvent, CharacterController> (Events::UPDATEEVENT, this, &CharacterController::Update);

    AUDIOSYSTEM->listener = gameObject->Transform;
  }

  void CharacterController::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "MicrophoneMultiplier");
    value->GetValue (&microhponeMultiplier);

    value = data->FindElement (data, "Acceleration");
    value->GetValue (&acceleration);

    value = data->FindElement (data, "JumpVelocity");
    value->GetValue (&jumpVel);
  }

  void CharacterController::ToggleFlying()
  {
    useFlying = !useFlying;
  }

  DefineComponentName (CharacterController);
}
