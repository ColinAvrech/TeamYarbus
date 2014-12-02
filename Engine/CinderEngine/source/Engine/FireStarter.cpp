/******************************************************************************/
/*!
\file   FireStarter.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#include "Thermodynamics.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "CollisionEvent.h"
#include "EventSystem.h"

namespace Framework
{
	void FireStarter::Serialize(Serializer::DataNode* data)
	{
		Component::Get_Enabled(data);
		Serializer::DataNode* temp = data->FindElement(data, "OnFire");
		temp->GetValue(&onFire);

		temp = data->FindElement(data, "Fuel");
		temp->GetValue(&Fuel);

		temp = data->FindElement(data, "InitTemp");
		temp->GetValue(&initTemp);

		temp = data->FindElement(data, "MaterialType");
		temp->GetValue(&material_type);
	}

	void FireStarter::Update(const double dt)
	{
		if (Fuel > 0.0f)
			Fuel -= dt;
	}

	void FireStarter::Initialize()
	{
		gameObject->FireStarter = this;
		float x = this->gameObject->Transform->GetGridPosition().x;
		float y = this->gameObject->Transform->GetGridPosition().y;
		Physics::THERMODYNAMICS->Add_Object(x, y, this);

		//grid.Create (gameObject->Transform);
	}

	void FireStarter::LightOnFire(void)
	{
		CollisionEvent collisionEvent;
		collisionEvent.Dt = 2;
		//EVENTSYSTEM->TriggerEvent(Events::COLLISION, collisionEvent);
		EVENTSYSTEM->TriggerEvent("CollisionEvent", collisionEvent);

		if (!onFire)
		{
			onFire = true;
			Physics::ThermodynamicsSystem::FIRE->AddFire
				(gameObject->Transform->GetPosition().x,
				gameObject->Transform->GetPosition().y,
				300);
			cout << "Lit\n";
		}
	}

	void FireStarter::DouseFire(void)
	{
		if (onFire)
		{
			onFire = false;
			cout << "Doused\n";
		}
	}

	FireStarter::FireStarter()
	{
		onFire = false;
		Fuel = 100.0f;
		material_type = WOOD;
	}

	DefineComponentName(FireStarter);

	void FireGrid::Create(FireStarter* fs)
	{
		glm::vec2 pos = glm::vec2(fs->gameObject->Transform->GetPosition());
		glm::vec2 scale = glm::vec2(fs->gameObject->Transform->GetScale());
		temperatures.allocate(2, 2);
		temperatures.fill(fs->initTemp);
		positions.allocate(2, 2);

		positions.Set(0, 0, glm::vec2(pos.x - scale.x, pos.y - scale.y));
		positions.Set(0, 0, glm::vec2(pos.x - (scale.x * 0.5f), pos.y - scale.y));
		positions.Set(0, 0, glm::vec2(pos.x, pos.y - scale.y));
		positions.Set(0, 0, glm::vec2(pos.x - scale.x, pos.y - scale.y));
	}

	void FireGrid::Destroy()
	{

	}

	void FireGrid::Update()
	{

	}

	void FireGrid::Draw()
	{

	}

}