/******************************************************************************/
/*!
\file   FireStarter.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#include <Precompiled.h>
#include "FireStarter.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Tree2D.h"
#include "AudioComponent.h"
#include "EventSystem.h"
#include "PingEvent.h"
#include "Events.h"

/*
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "Events.h"
#include "GameEvent.h"
#include "GUIText.h"
#include "EventSystem.h"
#include "PingEvent.h"
*/

namespace Framework
{
  FireGroup::FireGroup()
  {
    onFire = false; 
  }

  FireGroup::~FireGroup()
  {
    for (auto firePoint : firePoints)
    {
      firePoint->manager = nullptr;
    }

    firePoints.clear();
  }

  void FireGroup::Initialize()
  {
  }

  void FireGroup::AddFireStarter(FireStarter *newFirePoint)
  {
    firePoints.push_back(newFirePoint);
  }

	void FireGroup::Serialize(Serializer::DataNode* data)
	{
    //TODO:: Serialize in FSM instead of here once we switch away from procedural
		//Component::Get_Enabled(data);
    /*
    Serializer::DataNode* temp = data->FindElement(data, "OnFire");
		temp->GetValue(&onFire);

		temp = data->FindElement(data, "Fuel");
		temp->GetValue(&Fuel);

		temp = data->FindElement(data, "InitTemp");
		temp->GetValue(&initTemp);

		temp = data->FindElement(data, "MaterialType");
		temp->GetValue(&material_type);
    */
	}

	void FireStarter::Update(const double dt)
	{
		if (Fuel > 0.0f)
			Fuel -= (float)dt;
	}

	FireStarter::FireStarter(const vec2& pos, FireGroup* fsm)
	{
		//manager->gameObject->FireStarter = this;
    positionOffset = pos;
    onFire = false;
    Fuel = 100.0f;
    initTemp = 400.0f;
    material_type = GRASS;
    manager = fsm;

		//grid.Create (this);
	}

  vec2 FireStarter::GetPosition()
  {
    vec3 pos = static_cast<Transform*>(manager->gameObject->GetComponent("Transform"))->GetPosition(); //Get Tree position

    return vec2(pos.x + positionOffset.x, pos.y + positionOffset.y);
  }

  vec2 FireStarter::GetGridPosition()
  {
    //manager->gameObject->Transform->GetGridPosition(positionOffset + (glm::vec2) manager->gameObject->Transform->GetPosition ());
    return static_cast<Transform*>(manager->gameObject->GetComponent("Transform"))->GetGridPosition() + positionOffset;
  }

	void FireStarter::LightOnFire(void)
	{
		//CollisionEvent collisionEvent;
		//collisionEvent.Dt = 2;
		////EVENTSYSTEM->TriggerEvent(Events::COLLISION, collisionEvent);
		//EVENTSYSTEM->TriggerEvent("CollisionEvent", collisionEvent);

		if (!onFire)
		{
			onFire = true;
      vec2 pos = GetPosition();
      Physics::ThermodynamicsSystem::FIRE->AddFire( pos.x, pos.y, 60);

      if (manager && !manager->onFire)
      {
        manager->onFire = true;
        for (auto fs : manager->firePoints)
        {
          fs->LightOnFire();
        }
      }
		}
	}

	void FireStarter::DouseFire(void)
	{
		if (onFire)
		{
			onFire = false;
			//cout << "Doused\n";
			PingEvent e;
			e.Ping = manager->gameObject;
			EVENTSYSTEM->TriggerEvent(Events::PING_DOUSEPLANT, e);
      //Physics::ThermodynamicsSystem::FIRE->RemoveFire (this);
		}
	}

	DefineComponentName(FireGroup);
  /* TODO:: Use Firegrid
  void FireGrid::Create(FireStarter* fs)
	{
		Tree2D* tree = reinterpret_cast<Tree2D*> (fs->manager->gameObject->GetComponent("Tree2D"));
		if (tree != nullptr)
		{
      Transform* tform = fs->manager->gameObject->Transform;
			vec2 pos = vec2(tform->GetPosition());
      vec2 scale = vec2(tform->GetScale());
			vector <vec2>& edges = tree->Get_Edges();
      vec2 MIN = (glm::mat2)(tform->GetModelMatrix()) * edges.at(0);
			MIN.y += scale.y * 0.5f;
      vec2 MAX = (glm::mat2)(tform->GetModelMatrix()) * edges.at(3);
			MAX.y += scale.y * 0.5f;
			temperatures.allocate(2, 2);
			temperatures.fill(fs->initTemp);
			positions.allocate(2, 2);

			positions.Set(0, 0, vec2(pos.x + MIN.x, pos.y + MIN.y));
			positions.Set(0, 1, vec2(pos.x + MAX.x, pos.y + MIN.y));

			positions.Set(1, 0, vec2(pos.x + MIN.x, pos.y + MAX.y));
			positions.Set(1, 1, vec2(pos.x + MAX.x, pos.y + MAX.y));
		}
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
  */
}