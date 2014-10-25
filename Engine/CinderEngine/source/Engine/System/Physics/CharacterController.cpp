#include "Common.h"
#include "ComponentInclude.h"
#include "CharacterController.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "CollisionEvent.h"
#include "TimeQuery.h"
#include "glmOverloads.h"
#include "PhysicsLibrary.h"
#include "IncludeForAllCollision.h"
#include "KeyEvent.h"
#include "EventSystem.h"


namespace Framework
{
	static void OnKeyPressed(GameObject* go, KeyEvent* key);
	static void OnCollisionEnter(GameObject* go, CollisionEvent* collision);

	CharacterController::~CharacterController()
	{

	}

  void OnKeyPressed (KeyEvent* _key)
  {
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // CODE NEEDS TO BE CHANGED OR FUNCTION SIGNATURE NEEDS TO BE CHANGED
    //if (_key->KeyDown)
    //{
    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_A))
    //  {
    //    glm::vec2 force, accel;
    //    force = { -5.0f, 0 }; //WASD control    
    //    //float mass = 2;
    //    //force should be multiplied by friction to get smaller every update
    //    accel = Physics::getAccel (force, 0.01);
    //    go->RigidBody->vel = Physics::applyAccel (accel, .064f);
    //    go->Transform->Translate (go->RigidBody->vel.x, 0, 0);
    //  }

    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_D))
    //    go->Transform->Translate (0.01f, 0, 0);

    //  if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_S))
    //    go->Transform->Translate (0, -0.01f, 0);
    //  else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_W))
    //    go->Transform->Translate (0, 0.01f, 0);
    //  //if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
    //  //  useDebug = !useDebug;
    //}
    //////////////////////////////////////////////////////////////////////////
  }

  static void OnCollisionEnter (CollisionEvent* collision)
  {
    collision->OtherObject->Transform->Translate (-collision->normal.x * 0.05f, -collision->normal.y * 0.05f, 0.0f);
  }

	/*!Telegraph that the component is active*/
	void CharacterController::Initialize()
	{
		EVENTSYSTEM->gConnect(Events::KEY_ANY, &OnKeyPressed);
		EVENTSYSTEM->gConnect(Events::COLLISION, &OnCollisionEnter);
	}
	void CharacterController::Serialize(Serializer::DataNode* data)
	{

	}
	void CharacterController::Update()
	{

	}
	
	DefineComponentName(CharacterController);
}
