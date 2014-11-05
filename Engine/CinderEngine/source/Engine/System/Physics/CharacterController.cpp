#include "Common.h"
#include "CharacterController.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "TimeQuery.h"
#include "glmOverloads.h"
#include "PhysicsLibrary.h"
#include "IncludeForAllCollision.h"

#include "GameObject.h"
#include "RigidBody.h"

#include "EventSystem.h"
#include "CollisionEvent.h"
#include "KeyEvent.h"
#include "UpdateEvent.h"



namespace Framework
{

  CharacterController::~CharacterController()
  {

  }

  void CharacterController::OnKeyPressed(KeyEvent* _key)
  {
    glm::vec2 force, accelH = { 1, 0 }, accelV = { 0, 1 };
    switch (_key->KeyValue)
    {
    case GLFW_KEY_W:
      gameObject->RigidBody->vel.y = jumpVel;
      //Physics::applyAccel(accelV, 0.016);
      //gameObject->Transform->Translate(0, 1, 0);
      break;

    case GLFW_KEY_D:
      if (gameObject->RigidBody->vel.x <= maxXVel)
        gameObject->RigidBody->vel.x += accel*0.016f;
      //gameObject->Transform->Translate(1, 0, 0);
      break;

    case GLFW_KEY_A:
      if (gameObject->RigidBody->vel.x >= -maxXVel)
        gameObject->RigidBody->vel.x -= accel*0.016f;
      //gameObject->Transform->Translate(-1, 0, 0);
      break;

    default:
      break;
    }


    //if (_key->KeyDown)
    //{
    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_A))
    //  {
    //    glm::vec2 force, accel;
    //    force = { -5.0f, 0 }; //WASD control    
    //    //float mass = 2;
    //    //force should be multiplied by friction to get smaller every update
    //    accel = Physics::getAccel (force, 0.01);


    //  }

    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_D))
    //    gameObject->Transform->Translate (0.01f, 0, 0);

    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_S))
    //
    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_W))
    //
    //  //if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
    //  //  useDebug = !useDebug;
    //}
    ////////////////////////////////////////////////////////////////////////
  }

  void CharacterController::OnCollisionEnter(CollisionEvent* collision)
  {
    //if (collision->OtherObject->LineCollider)
    //  collision->OtherObject->Transform->Translate(-collision->normal.x * 0.05f, -collision->normal.y * 0.05f, 0.0f);
  }

  void CharacterController::Update(UpdateEvent* e)
  {
    //how to get line collider
    //gameObject->CircleCollider->DetectLine(gameObject->LineCollider);
    //go1->CircleCollider->DetectCircle(go2->CircleCollider);
    glm::vec2 dragDirection = -gameObject->RigidBody->vel;
    if (gameObject->RigidBody->vel.x != 0 && gameObject->RigidBody->vel.y != 0)
      dragDirection = glm::normalize(dragDirection);
    gameObject->RigidBody->vel += Physics::applyAccel(dragDirection * drag, .016);
    gameObject->Transform->Translate(gameObject->RigidBody->vel.x * e->Dt, gameObject->RigidBody->vel.y * e->Dt, 0);
    std::cout << AUDIOSYSTEM->input.peaklevel[0] << std::endl;
    if (currentforce <= 1 && AUDIOSYSTEM->input.peaklevel[0] > 0.07)
      currentforce += AUDIOSYSTEM->input.peaklevel[0] * 2;
    else if (currentforce > 0 && AUDIOSYSTEM->input.peaklevel[0] <= 0.07)
      currentforce -= AUDIOSYSTEM->input.peaklevel[0];

    gameObject->RigidBody->vel.y = currentforce * jumpVel;
  }

  //void CharacterController::UpdateVelocity()
  //{
  //}


  /*!Telegraph that the component is active*/
  void CharacterController::Initialize()
  {
    jumpVel = 5.0f;
    accel = 10;
    maxXVel = 2.0f;
    drag = 5;
    currentforce = 0;
    EVENTSYSTEM->mConnect<KeyEvent, CharacterController>(Events::KEY_ANY, this, &CharacterController::OnKeyPressed);
    EVENTSYSTEM->mConnect<CollisionEvent, CharacterController>(Events::COLLISION, this, &CharacterController::OnCollisionEnter);
    EVENTSYSTEM->mConnect<UpdateEvent, CharacterController>(Events::UPDATEEVENT, this, &CharacterController::Update);
  }

  void CharacterController::Serialize(Serializer::DataNode* data)
  {

  }

  DefineComponentName(CharacterController);
}
