/******************************************************************************/
/*!
\file   Factory.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates all GameObjectComposition and their respective components using
the temaplted Component creators. The factory also cleans up when an object is
deleted.
*/
/******************************************************************************/

#include "ObjectSystem.h"
#include "BaseSystem.h"
#include "IncludeForAllCollision.h"
#include "CharacterController.h"
#include "PlayerEffect.h"
#include "Terrain2D.h"
#include "Terrain3D.h"
#include "Tree2D.h"
#include "FireStarter.h"
//#include "Health.h"
#include "Microphone.h"
#include "CinderEngine_UI.h"
#include "GameEvent.h"
#include "EventSystem.h"

#include "Core.h"
#include "PhysicsSystem.h"
#include "ZInterface.h"
#include "ScriptComponent.h"
#include "Trees.h"
#include "Collider2D.h"


namespace Framework
{
  //!Null untill the ObjectSystem has been created
  ObjectSystem * OBJECTSYSTEM = NULL;
  //!Set first object's id to zero
  unsigned ObjectSystem::LastGameObjectId = 0;
  int ObjectSystem::currentLevel = 0;
  ZilchDefineType (BaseSystem, CinderZilch)
  {

  }
  ZilchDefineType (ObjectSystem, CinderZilch)
  {
    type->HandleManager = ZilchManagerId (Zilch::PointerManager);

    ZilchBindMethod (CreateObject);
    ZilchBindMethod (DestroyAllObjects);
    ZilchBindMethod (LoadLevelAdditive);
    ZilchBindMethodAs (ZilchLoadLevel, "LoadLevel");
    ZilchBindMethod (FindObjectByName);
    ZilchBindMethod (FindObjectByID);
    //ZilchBindMethod(LoadLevel);
    //ZilchBindConstructor(Transform);
    //ZilchBindMethodOverload(LoadLevel, void, Zilch::String);
    //ZilchBindMethodOverload(Scale, void, float);
    //ZilchBindMethod(Rotate);
  }



  ObjectSystem::ObjectSystem ()
  {
    ErrorIf (OBJECTSYSTEM != NULL, "Factory Already Created");
    OBJECTSYSTEM = this;

    RegisterComponents ();
  }

  ObjectSystem::~ObjectSystem ()
  {
    if (GameObjects.size () > 0)
    {
      DestroyAllObjects ();
    }

    for (auto level : levelList)
    {
      delete level;
    }
    levelList.clear ();
  }


  bool ObjectSystem::Initialize ()
  {
    return true;
  }


  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void ObjectSystem::Update (const double &dt)
  {
    DestroyGameObjectsToBeDestroyed ();

  }

  GameObject* ObjectSystem::CreateObject ()
  {
    GameObject * obj = new GameObject (LastGameObjectId);

    GameObjects [LastGameObjectId] = obj;
    ++LastGameObjectId;
    return obj;
  }

  /*
  Called When the ObjectSystem is created
  */
  void ObjectSystem::RegisterComponents (void)
  {
    RegisterComponent (Transform);
    RegisterComponent (Sprite);
    RegisterComponent (Camera);
    RegisterComponent (CharacterController);
    //RegisterComponent(Health);
    RegisterComponent (RigidBody);
    RegisterComponent (PlayerEffect);
    RegisterComponent (Terrain2D);
    RegisterComponent (Terrain3D);
    RegisterComponent (Tree2D);
    RegisterComponent (EcoSystem);
    RegisterComponent (FireStarter);
    RegisterComponent (Microphone);
    RegisterComponent (UIBox);
    RegisterComponent (RigidBody2D);
    RegisterComponent (ShapeCollider2D);
    RegisterComponent (CircleCollider2D);
    RegisterComponent (PolygonCollider2D);
    //RegisterComponent (SplineCollider);
    AddComponentCreator ("SplineCollider", new ComponentCreatorType<SplineCollider> ("SplineCollider"));
    AddComponentCreator ("SphereCollider", new ComponentCreatorType<CircleCollider> ("SphereCollider"));
    AddComponentCreator ("BoxCollider", new ComponentCreatorType<LineCollider> ("BoxCollider"));
  }

  void ObjectSystem::AddComponentCreator (string name, ComponentCreator* creator)
  {
    SerialMap [name] = creator;
  }

  void ObjectSystem::DestroyAllObjects ()
  {
    for each(auto obj in GameObjects)
    {
      delete obj.second;
      obj.second = NULL;
    }
    GameObjects.clear ();
  }

  void ObjectSystem::DestroyGameObjectsToBeDestroyed ()
  {
    for each(auto obj in GameObjectsToBeDestroyed)
    {
      delete obj;
      obj = NULL;
    }

  }

  void ObjectSystem::LoadAllLevels (const string& p_levellist)
  {
    Level* defaultLevel = new Level ();
    defaultLevel->SetName ("Default");
    defaultLevel->SetFile ("ZilchTests");

    std::cout << CinderConsole::cyan << "--------------------------------\nLoading Textures...\n" << CinderConsole::gray;

    std::ifstream levelFile (p_levellist);

    if (!levelFile.good ())
    {
      std::cout << CinderConsole::red << "Failed to Load Levels...\n" << CinderConsole::gray;
      return;
    }
    else
    {
      string str;
      Level* newLevel;
      while (!levelFile.eof ())
      {
        levelFile >> str;
        newLevel = new Level (str.c_str ());
        levelList.push_back (newLevel);
        std::cout << CinderConsole::green << str << std::endl << CinderConsole::gray;
      }
    }
    std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;

    if (levelList.size () == 0)
    {
      levelList.push_back (defaultLevel);
    }
  }

  void ObjectSystem::LoadLevel (const string &levelName, const string &fn_level)
  {
    for (auto level : levelList)
    {
      if (levelName != "" && levelName == level->GetName ())
      {
        return;
      }
      else if (fn_level != "" && fn_level == level->GetName ())
      {
        return;
      }
    }

    DestroyAllObjects ();

    Level* newLevel = new Level (levelName, fn_level);
    Serializer::DataNode* Trunk = newLevel->GetData ()->GetTrunk ();
    SerializeObject (Trunk);
    levelList.push_back (newLevel);
  }

  void ObjectSystem::StartLevel ()
  {
    DestroyAllObjects ();
    GameEvent e;
    EVENTSYSTEM->TriggerEvent (Events::GAME_ALLOBJECTSINITIALIZED, e);
    //InitializeObject ();
    EVENTSYSTEM->TriggerEvent (Events::GAME_LEVELSTARTED, e);
  }

  void ObjectSystem::ChangeLevel (const string& name)
  {
    for (unsigned i = 0; i < levelList.size (); ++i)
    {
      if (name == levelList [i]->GetName ())
      {
        ChangeLevel (i);
        return;
      }
    }
  }

  void ObjectSystem::ChangeLevel (const int& iNewLevel)
  {
    currentLevel = iNewLevel;
    StartLevel ();
  }

  GameObject* ObjectSystem::FindObjectByName (Zilch::String name)
  {
    for each (auto i in GameObjects)
    {
      if (name == Zilch::String (i.second->Name.c_str ()))
      {
        return i.second;
      }
    }

    return NULL;

    //return Zilch::Array<GameObject*>();
  }

  GameObject* ObjectSystem::FindObjectByID (Zilch::Integer id)
  {
    return GameObjects [unsigned (id)];

    //return Zilch::Array<GameObject*>();
  }

  void ObjectSystem::FindAllObjectsByName (Zilch::String name)
  {
    //return Zilch::Array<GameObject*>();
  }

  void ObjectSystem::RestartLevel ()
  {
    DestroyAllObjects ();
    StartLevel ();
  }

  void ObjectSystem::ZilchLoadLevel (Zilch::String level)
  {
    DestroyAllObjects ();

    Serializer::ZeroSerializer data;

    data.open (level.c_str ());
    data.CreateArchive ();
    Serializer::DataNode* Trunk = data.GetTrunk ();
    SerializeObject (Trunk);
    //return objects;
    //InitializeObject ();
  }

  void ObjectSystem::LoadLevelAdditive (Zilch::String level)
  {
    Serializer::ZeroSerializer data;

    data.open (level.c_str ());
    data.CreateArchive ();
    Serializer::DataNode* Trunk = data.GetTrunk ();
    SerializeObject (Trunk);
    // return objects;
    //InitializeObject ();
  }

  //Private function to create and serilize an object
  void ObjectSystem::SerializeObject (Serializer::DataNode* data)
  {
    //GameObject* go = new GameObject(data->branch->value_.UInt_);
    auto it = data->branch;

    Zilch::Array<GameObject*> objectlist;
    vector<std::pair<ZilchComponent*, Serializer::DynamicElement*> > scripts;
    /*
    go->Name = data->objectName;
    GameObjects[go->GameObjectID] = go;
    */
    std::cout << "SERIALIZING" << std::endl;
    //Create and Serilize Objects
    while (it)
    {
      if (it->objectName.compare ("Cog") == 0 &&
        it->branch->next->branch->value_.String_->compare ("EditorCamera") != 0)
      {
        GameObject* newobj = new GameObject (it->branch->branch->value_.UInt_);
        newobj->Name = *it->branch->next->branch->value_.String_;

        GameObjects [newobj->GameObjectID] = newobj;
        auto ct = it->branch->next->next;
        while (ct)
        {
          Component* newcomp = newobj->AddComponent (ct->objectName);
          if (newcomp)
          {
            newcomp->gameObject = newobj;
            std::cout << newobj->Name << std::endl;
            newcomp->Serialize (ct->branch);
            newcomp->Initialize (); //Set pointer to GameObject, Setup Component
          }
          else
          {
            ZilchComponent* zilchComp = newobj->AddZilchComponent (ct->objectName);
            newcomp = zilchComp;
            newcomp->gameObject = newobj;
            //newcomp->Serialize(ct->branch);


            scripts.push_back (std::pair<ZilchComponent*, Serializer::DynamicElement*> (zilchComp, ct->branch));
            newcomp->Initialize ();
          }
          objectlist.append (newobj);
          ct = ct->next;
        }

        ErrorIf (newobj->Transform == nullptr, (string ("Transform component missing on GameObject ") + newobj->Name).c_str ());
        GameObjects [newobj->GameObjectID] = newobj;
      }
      it = it->next;
    }

    //Initializing Zilch COmponents
    for each (auto i in scripts)
    {
      //i.first->InitializeAndSerialize(i.second);
      //i.first->Initialize();
    }

    //return objectlist;
  }

  //Private function to create and serilize a component
  void ObjectSystem::SerializeComponent (string ComponentName, Serializer::DataNode* data)
  {


  }

  void ObjectSystem::InitializeObject ()
  {
    // Need Component List
    for each (auto i in GameObjects)
    {
      i.second->Transform->Initialize ();
      i.second->Sprite->Initialize ();
    }
  }

}