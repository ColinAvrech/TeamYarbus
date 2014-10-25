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

	/*!Telegraph that the component is active*/
	void CharacterController::Initialize()
	{
		EVENTSYSTEM->Connect(NULL, Events::KEY_ANY, BaseEvent::BaseCall(OnKeyPressed));
		EVENTSYSTEM->Connect(NULL, Events::COLLISION, BaseEvent::BaseCall(OnCollisionEnter));
	}
	void CharacterController::Serialize(Serializer::DataNode* data)
	{

	}
	void CharacterController::Update()
	{

	}

	
	static void OnCollisionEnter(GameObject* go, CollisionEvent* collision)
	{
		collision->OtherObject->Transform->Translate(-collision->normal.x * 0.05f, -collision->normal.y * 0.05f, 0.0f);
	}
	
	void OnKeyPressed(GameObject* go, KeyEvent* _key)
	{
		//int key = _key->KeyValue;
		//int action = (int)_key->KeyDown;
		//if (key == GLFW_KEY_D && action == GLFW_PRESS && circleDivisions < 85)
		//  circleDivisions += 1;
		//if (key == GLFW_KEY_A && action == GLFW_PRESS &&  circleDivisions > 3)
		//  circleDivisions -= 1;
		//if (key == GLFW_KEY_W && action == GLFW_PRESS)
		//  circle.SetRadius (circle.GetRadius() + 0.025f);
		//if (key == GLFW_KEY_S && action == GLFW_PRESS && circleRadius >= 0.1f)
		//  circle.SetRadius (circle.GetRadius () - 0.025f);

		if (_key->KeyDown)
		{
			if (glfwGetKey(WINDOWSYSTEM->Get_Window(), GLFW_KEY_A))
			{
				glm::vec2 force, accel;
				force = { -5.0f, 0 }; //WASD control    
				//float mass = 2;
				//force should be multiplied by friction to get smaller every update
				accel = Physics::getAccel(force, 0.01);
				go->RigidBody->vel = Physics::applyAccel(accel, .064f);
				go->Transform->Translate(go->RigidBody->vel.x, 0, 0);
			}

			else if (glfwGetKey(WINDOWSYSTEM->Get_Window(), GLFW_KEY_D))
				go->Transform->Translate(0.01f, 0, 0);

			if (glfwGetKey(WINDOWSYSTEM->Get_Window(), GLFW_KEY_S))
				go->Transform->Translate(0, -0.01f, 0);
			else if (glfwGetKey(WINDOWSYSTEM->Get_Window(), GLFW_KEY_W))
				go->Transform->Translate(0, 0.01f, 0);
			//if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
			//  useDebug = !useDebug;
		}
	}
	
	DefineComponentName(CharacterController);
}
