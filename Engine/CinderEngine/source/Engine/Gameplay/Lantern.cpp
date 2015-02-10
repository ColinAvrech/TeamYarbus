#include <Precompiled.h>
#include "Lantern.h"
#include "PhysicsSystem.h"

#define PI 4.0f*atan(1.0f)

namespace Framework
{
	void Lantern::Initialize(){};

	void Lantern::Serialize(Serializer::DataNode* data)
	{
		//SPECIFY SHADERS HERE FOR LIGHT WORLD___
		data->FindElement(data, "lightRadius")->GetValue(&lightRadius);
		data->FindElement(data, "lightTheta")->GetValue(&lightTheta);
	}

	void Lantern::Update(UpdateEvent* e)
	{
		CalculateBounds();
	}

	void Lantern::CalculateBounds()
	{
		//flaslight follows the mouse
		Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
		origin.x = WINDOWSYSTEM->Get_Mouse_Position().x - tform->GetPosition().x;
		origin.y = WINDOWSYSTEM->Get_Mouse_Position().y - tform->GetPosition().y;
		leftBounds.x = lightRadius*cos(lightTheta / 2);
		leftBounds.y = lightRadius*sin(lightTheta / 2);
		rightBounds.x = lightRadius*cos(-lightTheta / 2);
		rightBounds.y = lightRadius*sin(-lightTheta / 2);

		glm::normalize(origin);
		glm::normalize(rightBounds);
		glm::normalize(leftBounds);
	}

	void Lantern::CheckCollision()
	{
		//for (int i = 0; i < OBJECTSYSTEM->GameObjects.size(); ++i)
		//{
		//	//glm::vec2 playerToObj = OBJECTSYSTEM->GameObjects[i].second->GetComponent("Transform");
		//	//float dotToTarget = glm::dot(OBJECTSYSTEM->GameObjects[i], origin);
		//	//float angleBetweenTarget = acosf(dotToTarget) * 180 / PI;

		//	//if (angleBetweenTarget < 90.0f && )
		//}
		
	}

	void Lantern::reFuel(){};

	DefineComponentName(Lantern);
}