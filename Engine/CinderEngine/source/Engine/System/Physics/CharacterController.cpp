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
    ZilchBindStaticFieldGetAs (PLAYER, "Player");
    //type->HandleManager = ZilchManagerId (Zilch::PointerManager);
    //ZilchBindStaticMethodAs(ZGetCameraMousePosition, "GetCameraMousePosition");
  }

  static bool onGround = false;

  CharacterController::CharacterController ()
  {
    PLAYER = this;
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

    if (InputManager::IsKeyDown(GLFW_KEY_UP))
    {
      gameObject->RigidBody2D->velocity.y += jumpVel.y;
      onGround = false;
    }
    
    if (InputManager::IsKeyDown(GLFW_KEY_RIGHT))
    {
      if (gameObject->RigidBody2D->force.x < 0.4f && gameObject->RigidBody2D->force.x > -0.4f)
        gameObject->RigidBody2D->ApplyForce(Vector2(acceleration.x * density,acceleration.y * density));
    }
    if (InputManager::IsKeyDown(GLFW_KEY_LEFT))
    {
      if (gameObject->RigidBody2D->force.x < 0.4f && gameObject->RigidBody2D->force.x > -0.4f)
        gameObject->RigidBody2D->ApplyForce(Vector2(-acceleration.x * density,acceleration.y * density));
    }
    
    if (InputManager::IsKeyDown(GLFW_KEY_R))
    {
      glm::vec2 position = glm::vec2(0, 500);
      gameObject->RigidBody2D->position = Vector2(position.x, position.y);
      gameObject->RigidBody2D->angularVelocity = 0.0f;
      gameObject->RigidBody2D->velocity = Vector2(0, 0);
      gameObject->RigidBody2D->force = Vector2(0, 0);
      Camera::main->gameObject->Transform->SetPosition(position.x, position.y);
    }

    Vector2* vel = &gameObject->RigidBody2D->velocity;
    if (vel->Len() > maxVel)
    {
      vel->Normalize();
      *vel = *vel * maxVel;
    }


    // Microphone input
    gridPos = gameObject->Transform->GetGridPosition ();
    float micValue = AUDIOSYSTEM->GetMicrophoneValue ();
    gameObject->RigidBody2D->ApplyForce(Vector2(micValue * microhponeMultiplier.x * density,micValue * microhponeMultiplier.y * density));
    Physics::THERMODYNAMICS->SetCellTemperature (gridPos.x, gridPos.y, 400000, 0.016);

  }


  /*!Telegraph that the component is active*/
  void CharacterController::Initialize ()
  {
    PLAYER = this;
    //accel = { 0 , 0 };
    //maxAcceleration = { 50, 100 };
    maxVel = 128.0f;
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

  DefineComponentName (CharacterController);
}
