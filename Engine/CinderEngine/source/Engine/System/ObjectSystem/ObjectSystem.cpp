/******************************************************************************/
/*!
\file   ObjectSystem.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates all GameObjectComposition and their respective components using
the temaplted Component creators. The factory also cleans up when an object is
deleted.
*/
/******************************************************************************/
 
#include <Precompiled.h>
#include "ObjectSystem.h"
#include "PhysicsSystem.h"
#include "Thermodynamics.h"
#include "FluidDynamics.h"
#include "BaseSystem.h"
#include "GameEvent.h"
#include "EventSystem.h"
#include "Core.h"
#include "UISystem.h"
#include "ZInterface.h"

//////////////////////////////////////////////////////////////////////////
// COMPONENTS
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// PHYSICS
//////////////////////////////////////////////////////////////////////////
#include "Collider2D.h"
#include "FluidBody.h"
//////////////////////////////////////////////////////////////////////////
// GRAPHICS
//////////////////////////////////////////////////////////////////////////
#include "Terrain2D.h"
#include "Terrain3D.h"
#include "Tree2D.h"
#include "Tree3D.h"
#include "CinderEngine_UI.h"
#include "Trees.h"
#include "PointLight.h"
#include "PlayerEffect.h"
#include "FireSystem.h"
#include "VolcanoEffect.h"
#include "CLParticleRenderer.h"
#include "GUIText.h"
//////////////////////////////////////////////////////////////////////////
// AUDIO
//////////////////////////////////////////////////////////////////////////
#include "Microphone.h"
#include "AudioComponent.h"
#include "AudioEvents.h"
//////////////////////////////////////////////////////////////////////////
// GAMEPLAY
//////////////////////////////////////////////////////////////////////////
#include "CharacterController.h"
#include "SparkCollector.h"
#include "ScriptComponent.h"
#include "FireStarter.h"
#include "Health.h"
#include "LevelTimer.h"
#include "CheatCodes.h"
#include "Follow.h"
#include "InterpolateBackground.h"
#include "SpriteColorUpdate.h"
#include "Rotator.h"
#include "PlayerStats.h"
#include "CameraShake.h"
#include "EndTrigger.h"
#include "EndEventListener.h"
#include "Spin.h"
#include "Splitscreen.h"
#include "Lantern.h"
#include "Rain.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////




namespace Framework
{
	//!Null untill the ObjectSystem has been created
	ObjectSystem * OBJECTSYSTEM = NULL;

	//!Set first object's id to zero
	unsigned ObjectSystem::LastGameObjectId = 0;

	int ObjectSystem::currentLevel = 0;
	ZilchDefineType(BaseSystem, CinderZilch)
	{

	}
	ZilchDefineType(ObjectSystem, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);

		//ZilchBindMethod(CreateObject);
		ZilchBindMethod(DestroyAllObjects);
		ZilchBindMethodOverload(LoadLevelAdditive, ZArray*, Zilch::String);
		ZilchBindMethodAs(ZilchLoadLevel, "LoadLevel");
		ZilchBindMethodOverload(FindObjectByName, GameObject*, Zilch::String);
		ZilchBindMethodAs(FindObjectByID, "FindObjectByID");
		ZilchBindMethod(DestroyObject);
		//ZilchBindMethod(LoadLevel);
		//ZilchBindConstructor(Transform);
		//ZilchBindMethodOverload(LoadLevel, void, Zilch::String);
		//ZilchBindMethodOverload(Scale, void, float);
		//ZilchBindMethod(Rotate);
	}

	ObjectSystem::ObjectSystem()
	{
		ErrorIf(OBJECTSYSTEM != NULL, "Factory Already Created");
		OBJECTSYSTEM = this;
		ptrPlayer = nullptr;
		RegisterComponents();
	}

	ObjectSystem::~ObjectSystem()
	{
		DestroyAllObjects();

		for (auto level : levelList)
		{
			delete level;
			level = nullptr;
		}
		levelList.clear();
	}


	bool ObjectSystem::Initialize()
	{
		return true;
	}

	/*!Deletes all objects int eh ObjectsToBeDelted List.*/
	void ObjectSystem::Update(const float &dt)
	{
		DestroyGameObjectsToBeDestroyed();

		while (!CommandList.empty())
		{
			Serializer::ZeroSerializer data;
			Serializer::DataNode* Trunk;

			switch (CommandList.back())
			{
			case OSC_IDLE:
				break;

			case OSC_LOADLEVEL:
				PHYSICS->Clear();
				//Physics::THERMODYNAMICS->Clear();
				Physics::FLUIDDYNAMICS->Clear();
				DestroyAllObjects();
				EVENTSYSTEM->DeleteAllEvents();

				data.open(currentLevelName.c_str());
				data.CreateArchive();
				Trunk = data.GetTrunk();
				SerializeObject(Trunk);

				Cheats::InitializeCheats();
				AUDIOEVENTS->Initialize();
				PHYSICS->Reset();
				break;

      case OSC_RESTARTLEVEL:
        ptrPlayer->C<CharacterController>()->ResetPosition();

			default:
				break;
			}
			CommandList.pop();
		}
	}

	GameObject* ObjectSystem::CreateObject(const std::string &objname)
	{
		GameObject * obj = this->CreateObject();
    obj->Name = objname;
		return obj;
	}

  GameObject* ObjectSystem::CreateObject()
  {
    GameObject * obj = new GameObject(LastGameObjectId);
    GameObjects[LastGameObjectId] = obj;
    ++LastGameObjectId;
    return obj;
  }

	//Called When the ObjectSystem is created
	void ObjectSystem::RegisterComponents(void)
	{
		//////////////////////////////////////////////////////////////////////////
		// PHYSICS
		//////////////////////////////////////////////////////////////////////////
		RegisterComponent(RigidBody2D);
		RegisterComponent(ShapeCollider2D);
		RegisterComponent(CircleCollider2D);
		RegisterComponent(PolygonCollider2D);
    RegisterComponent(CompoundCollider2D);
		RegisterComponent(FluidBody);
		//RegisterComponent (SplineCollider);
		//////////////////////////////////////////////////////////////////////////
		// GRAPHICS
		//////////////////////////////////////////////////////////////////////////
		RegisterComponent(Transform);
		RegisterComponent(Sprite);
		RegisterComponent(Camera);
		RegisterComponent(Terrain2D);
		RegisterComponent(Terrain3D);
		RegisterComponent(Tree2D);
    RegisterComponent(Tree3D);
		RegisterComponent(EcoSystem);
		RegisterComponent(UIBox);
		RegisterComponent(PointLight);
		RegisterComponent(PlayerEffect);
		RegisterComponent(FireSystem);
		RegisterComponent(FountainEffect);
		RegisterComponent(CLParticleRenderer);
		RegisterComponent(GUIText);
		//////////////////////////////////////////////////////////////////////////
		// AUDIO
		//////////////////////////////////////////////////////////////////////////
		RegisterComponent(Microphone);
		RegisterComponent(AudioComponent);
		//////////////////////////////////////////////////////////////////////////
		// GAMEPLAY
		//////////////////////////////////////////////////////////////////////////
		RegisterComponent(CharacterController);
		RegisterComponent(SparkCollector);
		RegisterComponent(Health);
		RegisterComponent(FireGroup);
		RegisterComponent(LevelTimer);
		RegisterComponent(Follow);
		RegisterComponent(InterpolateBackground);
		RegisterComponent(SpriteColorUpdate);
		RegisterComponent(Rotator);
		RegisterComponent(PlayerStats);
		RegisterComponent(CameraShake);
		RegisterComponent(EndTrigger);
		RegisterComponent(EndEventListener);
        RegisterComponent(Spin);
	    RegisterComponent(Splitscreen);
	    RegisterComponent(Lantern);
	    RegisterComponent(Rain);
		//////////////////////////////////////////////////////////////////////////
	}
	void ObjectSystem::AddComponentCreator(string name, ComponentCreator* creator)
	{
		SerialMap[name] = creator;
	}



	void ObjectSystem::DestroyObjectNow(GameObject* obj)
	{
		for (auto object : GameObjects)
		{
			if (object.second == obj)
			{
				delete object.second;
				object.second = nullptr;
			}
		}
	}

	void ObjectSystem::DestroyObject(GameObject* obj)
	{
		GameObjectsToBeDestroyed.push_back(obj);
	}

	void ObjectSystem::DestroyAllObjects()
	{
		//EVENTSYSTEM->DeleteAllEvents();
		for (auto obj : GameObjects)
		{
			delete obj.second;
			obj.second = nullptr;
		}
		GameObjects.clear();
	}

	void ObjectSystem::DestroyGameObjectsToBeDestroyed()
	{
		for (auto obj : GameObjectsToBeDestroyed)
		{
			delete obj;
			obj = nullptr;
		}
	}

	//#########   ERROR THIS FUNCTION'S will always cause a memory leak
	ZArray* ObjectSystem::LoadLevelAdditive(Zilch::String level)
	{
		Serializer::ZeroSerializer data;

		data.open(level.c_str());
		data.CreateArchive();
		Serializer::DataNode* Trunk = data.GetTrunk();
		return new ZArray(SerializeObject(Trunk));
	}


	void ObjectSystem::ZilchLoadLevel(Zilch::String level)
	{
		LoadLevel(level.c_str());
	}

	void ObjectSystem::LoadLevel(const char* name)
	{
		CommandList.push(ObjectSystemCommand::OSC_LOADLEVEL);
		currentLevelName = name;
	}

	void ObjectSystem::NextLevel()
	{
		bool loadSuccess = false;
		string nextLevel;
		if (ptrPlayer)
		{
      PlayerStats* ps = static_cast<PlayerStats*>(ptrPlayer->GetComponent("PlayerStats"));
      if (ps)
      {
			  loadSuccess = true;
			  nextLevel = ps->NextLevel.c_str();
      }
		}
		else//HACK for splash screens
		{
			GameObject* go = FindObjectByName("Logo");
			if (go)
			{
				LevelTimer* lt = reinterpret_cast<LevelTimer*>(go->GetComponent("LevelTimer"));
				if (lt)
				{
					loadSuccess = true;
					nextLevel = lt->nextLevel.c_str();
				}
				else
				{
					__debugbreak();
				}
			}
			else // couldn't load beyond last level
			{
				__debugbreak();
			}
		} // End HACK

		if (loadSuccess)
		{
			cout << CinderConsole::green;
			printf("%s completed. Loading next level: %s\n", currentLevelName.c_str(), nextLevel.c_str());
			cout << CinderConsole::red;
			LoadLevel(nextLevel.c_str());
		}
		else
		{
			assert(loadSuccess && "No next level found.\n");
		}
	}

	void ObjectSystem::RestartLevel()
	{
		CommandList.push(ObjectSystemCommand::OSC_RESTARTLEVEL);
	}

	GameObject* ObjectSystem::FindObjectByName(Zilch::String name)
	{
		for (auto& i : GameObjects)
		{

			if (i.second && i.second->Name == name)
			{
				return i.second;
			}
		}

		return nullptr;
	}

	GameObject* ObjectSystem::FindObjectByName(const char* name)
	{
		for (auto& i : GameObjects)
		{
			
			if (i.second && string(i.second->Name.c_str()).compare(name) == 0)
			{
				return i.second;
			}
		}

		return nullptr;
	}

	GameObject* ObjectSystem::FindObjectByID(unsigned id)
	{
		return GameObjects[id];
	}

	//Private function to create and serilize an object
	//#########   ERROR THIS FUNCTION'S will always cause a memory leak
	Zilch::Array<GameObject*>* ObjectSystem::SerializeObject(Serializer::DataNode* data)
	{
		std::cout << "SERIALIZING" << std::endl;

		Zilch::Array<GameObject*>* objectlist = new Zilch::Array<GameObject*>();
		vector<Handle> scripts;

		auto it = data->branch;
		while (it)
		{
      string objName;
      Serializer::DataNode *n = it->FindElement(it->branch, "Named");
      n->FindElement(n->branch, "Name")->GetValue(&objName);
			if (it->objectName.compare("Cog") == 0 &&
				objName.compare("EditorCamera") != 0)
			{
				GameObject* newobj = OBJECTSYSTEM->CreateObject(objName);

				//if (LastGameObjectId <= it->branch->branch->value_.UInt_)
				//{ LastGameObjectId = it->branch->branch->value_.UInt_ + 1; } // Makes sure that every created object has a unique ID

        auto ct = it->branch->next;
				while (ct)
				{
					Component* newcomp = newobj->AddComponent(ct->objectName);
					if (newcomp)
					{
						newcomp->gameObject = newobj;
						newcomp->Serialize(ct->branch);
						newcomp->Initialize(); //Set pointer to GameObject, Setup Component
					}
					else
					{

						Handle zilchComp = newobj->AddZilchComponent(ct->objectName);


						Zilch::Array<Zilch::Type*> args;
						args.push_back(ZilchTypeId(Serializer::DataNode*));
						Function* ZilchSerialize = zilchComp.Type->FindFunction("Serialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
						ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Serialize' on Zilch type ", zilchComp.Type);

						{
							Zilch::ExceptionReport report;
							// Invoke the Create function, which assigns this object an owner.
							Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
							call.SetHandle(Zilch::Call::This, zilchComp);
							call.SetHandle(0, ct->branch);
							call.Invoke(report);
						}
						scripts.push_back(zilchComp);
						//newcomp = zilchComp->Type;
						//scripts.push_back(std::pair<ZilchComponent*, Serializer::DataNode*>(zilchComp, ct->branch));

					}
					ct = ct->next;
				}

        //Follow cam setup
        /*
        if (newobj->Name == "Player" && Camera::main)
        {
          Follow* followComp = static_cast<Follow*>(Camera::main->gameObject->AddComponent("Follow"));
          followComp->SetTarget("Player");
        }
        */

				objectlist->append(newobj);

        ErrorIf(!newobj->GetComponent("Transform"), (string("Transform component missing on GameObject ") + newobj->Name.c_str()).c_str());
			}
			it = it->next;
		}

		//Initializing Zilch Components
		Zilch::Array<Zilch::Type*> args;
		for (auto i : scripts)
		{

			Function* ZilchInitialize = i.Type->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
			ErrorIf(!ZilchInitialize, "Failed to find function 'Initialize' on Zilch type ", i.Type);

			{
				Zilch::ExceptionReport report;
				// Invoke the Initialize function, which Initializes all of the zilch scripts.
				Zilch::Call call(ZilchInitialize, ZILCH->GetDependencies());
				call.SetHandle(Zilch::Call::This, i);
				call.Invoke(report);
			}

			//it->Initialize();
		}

		return objectlist;
	}

	GameObject* ObjectSystem::LoadArchetype(const char *archtype_file)
	{
		Serializer::ZeroSerializer file;
		file.open(archtype_file);
		file.CreateArchive();
		Serializer::DataNode* Trunk = file.GetTrunk();
    std::string obj_name;
    Serializer::DataNode *n = Trunk->FindElement(Trunk->branch, "Named");
    n->FindElement(n->branch, "Name")->GetValue(&obj_name);
		GameObject* newobj = this->CreateObject(obj_name);
    
		//Skip 1st 3 objects
		auto ct = Trunk->branch->next->next;
		while (ct)
		{
			Component* newcomp = newobj->AddComponent(ct->objectName);
			if (newcomp)
			{
				newcomp->gameObject = newobj;
				newcomp->Serialize(ct->branch);
				newcomp->Initialize();
			}
			ct = ct->next;
		} //while
		return newobj;
	}

  GameObject* ObjectSystem::CreateAtPosition(const char *archetype, float _x, float _y, float _z)
  {
    GameObject *newobj = LoadArchetype(archetype);
    newobj->C<Transform>()->Translate(_x, _y, _z);
    return newobj;
  }



	//Colin's Stuff  (DO NOT TOUCH!)
	/*
	void ObjectSystem::LoadAllLevels(const string& p_levellist)
	{
	Level* defaultLevel = new Level();
	defaultLevel->SetName("Default");
	defaultLevel->SetFile("ZilchTestLevel");

	std::cout << CinderConsole::cyan << "--------------------------------\nLoading Textures...\n" << CinderConsole::gray;

	std::ifstream levelFile(p_levellist);

	if (!levelFile.good())
	{
	std::cout << CinderConsole::red << "Failed to Load Levels...\n" << CinderConsole::gray;
	return;
	}
	else
	{
	string str;
	Level* newLevel;
	while (!levelFile.eof())
	{
	levelFile >> str;
	newLevel = new Level(str.c_str());
	levelList.push_back(newLevel);
	std::cout << CinderConsole::green << str << std::endl << CinderConsole::gray;
	}
	}
	std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;

	if (levelList.size() == 0)
	{
	levelList.push_back(defaultLevel);
	}


	}

	void ObjectSystem::LoadLevel(const string &levelName, const string &fn_level)
	{
	for (auto level : levelList)
	{
	if (levelName != "" && levelName == level->GetName())
	{
	return;
	}
	else if (fn_level != "" && fn_level == level->GetName())
	{
	return;
	}
	}

	DestroyAllObjects();

	Level* newLevel = new Level(levelName, fn_level);
	Serializer::DataNode* Trunk = newLevel->GetData()->GetTrunk();
	SerializeObject(Trunk);
	levelList.push_back(newLevel);
	}

	void ObjectSystem::StartLevel()
	{
	//DestroyAllObjects();
	GameEvent e;
	EVENTSYSTEM->TriggerEvent(Events::GAME_ALLOBJECTSINITIALIZED, e);
	//InitializeObject ();
	EVENTSYSTEM->TriggerEvent(Events::GAME_LEVELSTARTED, e);
	}

	void ObjectSystem::ChangeLevel(const string& name)
	{
	for (unsigned i = 0; i < levelList.size(); ++i)
	{
	if (name == levelList[i]->GetName())
	{
	ChangeLevel(i);
	return;
	}
	}
	}

	void ObjectSystem::ChangeLevel(const int& iNewLevel)
	{
	currentLevel = iNewLevel;
	StartLevel();
	}

	void ObjectSystem::RestartLevel()
	{
	DestroyAllObjects();
	StartLevel();
	}
	*/
}