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
		switch (_key->KeyValue)
		{
		case GLFW_KEY_W:

			break;



		default:
			break;
		}


    if (_key->KeyDown)
    {
      if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_A))
      {
        glm::vec2 force, accel;
        force = { -5.0f, 0 }; //WASD control    
        //float mass = 2;
        //force should be multiplied by friction to get smaller every update
        accel = Physics::getAccel (force, 0.01);
		
        gameObject->RigidBody->vel = Physics::applyAccel (accel, .064f);
		gameObject->Transform->Translate(gameObject->RigidBody->vel.x, 0, 0);
      }

      else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_D))
        gameObject->Transform->Translate (0.01f, 0, 0);

      if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_S))
		  gameObject->Transform->Translate(0, -0.01f, 0);
      else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_W))
		  gameObject->Transform->Translate(0, 0.01f, 0);
      //if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
      //  useDebug = !useDebug;
    }
    ////////////////////////////////////////////////////////////////////////
  }

  void CharacterController::OnCollisionEnter(CollisionEvent* collision)
  {
    collision->OtherObject->Transform->Translate (-collision->normal.x * 0.05f, -collision->normal.y * 0.05f, 0.0f);
  }

  void CharacterController::Update(UpdateEvent* e)
  {
	 
  }


	/*!Telegraph that the component is active*/
	void CharacterController::Initialize()
	{
		EVENTSYSTEM->mConnect<KeyEvent, CharacterController>(Events::KEY_ANY, this, &CharacterController::OnKeyPressed);
		EVENTSYSTEM->mConnect<CollisionEvent, CharacterController>(Events::COLLISION, this, &CharacterController::OnCollisionEnter);
		EVENTSYSTEM->mConnect<UpdateEvent, CharacterController>(Events::UPDATEEVENT, this, &CharacterController::Update);
	}

	void CharacterController::Serialize(Serializer::DataNode* data)
	{

	}

	DefineComponentName(CharacterController);
}