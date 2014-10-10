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

namespace Framework
{
  //!Null untill the ObjectSystem has been created
  ObjectSystem * OBJECTSYSTEM = NULL;
  //!Set first object's id to zero
  unsigned ObjectSystem::LastGameObjectId = 0;


  ObjectSystem::ObjectSystem()
  {
    ErrorIf(OBJECTSYSTEM != NULL, "Factory Already Created");
    OBJECTSYSTEM = this;
    RegisterComponents();
  }

  ObjectSystem::~ObjectSystem()
  {
  }

  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void ObjectSystem::Update(const double dt)
  {
    DestroyGameObjectsToBeDestroyed();

  }

  GameObject* ObjectSystem::CreateObject()
  {
    GameObject * obj = new GameObject(LastGameObjectId);

    GameObjects[LastGameObjectId] = obj;
    ++LastGameObjectId;
    return obj;
  }

  /*
  Called When the ObjectSystem is created
  */
  void ObjectSystem::RegisterComponents(void)
  {
    RegisterComponent(Transform);
    RegisterComponent(Sprite);
	//RegisterComponent(CircleCollider);
	//RegisterComponent(PointCollider);
	//RegisterComponent(LineCollider);
	//RegisterComponent(RigidBody);
  }

  void ObjectSystem::AddComponentCreator(std::string name, ComponentCreator* creator)
  {
    SerialMap[name] = creator;
  }

  void ObjectSystem::DestroyAllObjects()
  {
    for each(auto obj in GameObjects)
    {
      delete obj.second;
      obj.second = NULL;
    }
  }

  void ObjectSystem::DestroyGameObjectsToBeDestroyed()
  {
    for each(auto obj in GameObjectsToBeDestroyed)
    {
      delete obj;
      obj = NULL;
    }

  }

  void ObjectSystem::LoadLevel(std::string level)
  {
    DestroyAllObjects();

    Serializer::ZeroSerializer data;

    data.open(level.c_str());
    data.CreateArchive();
    Serializer::DataNode* Trunk = data.GetTrunk();
  

  }

  //Private function to create and serilize an object
  void ObjectSystem::SerializeObject(Serializer::DataNode* data)
  {
    //GameObject* go = new GameObject(data->branch->value_.UInt_);
    auto it = data->branch;
    /*
    go->Name = data->objectName;
    GameObjects[go->GameObjectID] = go;
    */

    //Create and Serilize Objects
    while (it)
    {
      if (it->objectName.compare("Cog") == 0)
      {
        GameObject* newobj = new GameObject(it->branch->value_.UInt_);
        newobj->Name = *it->branch->next->value_.String_;
        GameObjects[newobj->GameObjectID] = newobj;
        auto ct = it->branch->next->next;
        while (ct)
        {
          Component* newcomp = newobj->AddComponent(ct->objectName);

          newcomp->Serialize(ct->branch);

          ct = ct->next;
        }
        GameObjects[newobj->GameObjectID] = newobj;
      }
      it = it->next;
    }
  }

  //Private function to create and serilize a component
  void ObjectSystem::SerializeComponent(std::string ComponentName, Serializer::DataNode* data)
  {


  }

}