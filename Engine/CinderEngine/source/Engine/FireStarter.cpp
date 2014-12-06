/******************************************************************************/
/*!
\file   FireStarter.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#include "FireStarter.h"
#include "Thermodynamics.h"
#include "GameObject.h"
#include "PingEvent.h"
#include "ComponentInclude.h"
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "Tree2D.h"
#include "AudioComponent.h"
#include "Events.h"
#include "GameEvent.h"
#include "EventSystem.h"
#include "Text.h"

namespace Framework
{
  int FireStarterManager::numTreesLeft = 0;
  int FireStarterManager::numTreesStart = 0;
  GUIText* FireStarterManager::guiText = nullptr;

  FireStarterManager::FireStarterManager()
  {
    onFire = false; 
    
    ++numTreesLeft;
    if(numTreesLeft > numTreesStart)
      numTreesStart = numTreesLeft;
  }

  FireStarterManager::~FireStarterManager()
  {
    for (auto firePoint : firePoints)
    {
      delete firePoint;
    }

    firePoints.clear();
    numPoints = 0;
    numTreesLeft = 0;
    numTreesStart = 0;
  }

  void FireStarterManager::Initialize()
  {
    gameObject->FireStarterManager = this;
    if (guiText == nullptr)
    {
      GameObject* go = new GameObject(10000);
      guiText = reinterpret_cast<GUIText*> (go->AddComponent("GUIText"));
      guiText->position = { -0.2f, -0.9f };
      guiText->Initialize();
    }
  }

  void FireStarterManager::AddFireStarter(FireStarter *newFirePoint)
  {
    firePoints.push_back(newFirePoint);
    Physics::THERMODYNAMICS->Add_Object(newFirePoint);
    ++numPoints;
  }

	void FireStarterManager::Serialize(Serializer::DataNode* data)
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

	FireStarter::FireStarter(const vec2& pos, FireStarterManager* fsm)
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
    vec3 pos = manager->gameObject->Transform->GetPosition(); //Get Tree position

    return vec2(pos.x + positionOffset.x, pos.y + positionOffset.y);
  }

  vec2 FireStarter::GetGridPosition()
  {
    manager->gameObject->Transform->GetGridPosition(positionOffset + (glm::vec2) manager->gameObject->Transform->GetPosition ());
    return manager->gameObject->Transform->GetGridPosition() + positionOffset;
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

      if (manager && !manager->onFire)
      {
        --manager->numPoints;
        if (manager->numPoints <= 0)
        {
          manager->onFire = true;
          --manager->numTreesLeft;
          std::cout << CinderConsole::green;
          printf ("Percentage of fuel unused: %f\n", (manager->numTreesLeft/(float)manager->numTreesStart));
          printf("Trees Remaining: %d, Total Trees: %d", manager->numTreesLeft, manager->numTreesStart);
          FireStarterManager::guiText->text = "Trees Remaining: " + std::to_string(FireStarterManager::numTreesLeft);
          std::cout << CinderConsole::red;
        }
        if (FireStarterManager::numTreesLeft <= 0)
        {
          BaseEvent b;
          EVENTSYSTEM->TriggerEvent (Events::ALLLTREESBURNED, b);
          return;
        }
      }

      vec2 pos = GetPosition();
      Physics::ThermodynamicsSystem::FIRE->AddFire( pos.x, pos.y, 60);
      /*
			Physics::ThermodynamicsSystem::FIRE->AddFire
				(
				gameObject->Transform->GetPosition().x,
				gameObject->Transform->GetPosition().y,
				30
				);
      */
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

	DefineComponentName(FireStarterManager);
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