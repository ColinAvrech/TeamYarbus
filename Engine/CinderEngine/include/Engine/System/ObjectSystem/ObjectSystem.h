/******************************************************************************/
/*!
\file   Factory.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/


#pragma once

namespace Framework
{
  //forward Declareation for ObjectSystem
  class ObjectSystem;
  //!Set the factory to null to indicate is hasn't been created yet
  extern ObjectSystem * OBJECTSYSTEM;
}

#include "Common.h"
#include "ComponentInclude.h"
#include "BaseSystem.h"
#include "ComponentCreator.h"
#include "GameObject.h"
#include "JSONSerializer.h"

namespace Framework
{
  /*!Game Object Factory:
  The Game object factory creates composition objects from data streams and manages
  their lifetimes. As part of controlling the life times of the GameObjectComposition (GOC)
  it also provides an integer based Id system for safe referencing of game objects
  through integer Id Handles.
  */

  //!Set the factory to null to indicate is hasn't been created yet
  extern ObjectSystem * OBJECTSYSTEM;

  class ObjectSystem : public BaseSystem
  {
  public:
    ZilchDeclareBaseType(ObjectSystem, Zilch::TypeCopyMode::ReferenceType);

    friend class GameObject;
    friend class Component;

    const char* LevelAssetsPath = "../../Assets/Levels/";

    ObjectSystem();
    ~ObjectSystem();

    virtual bool Initialize ();
    //!Update the factory, Destroying dead objects
    virtual void Update(const double& dt);

    //!name of the system if the Factory, duh.
    virtual const string GetName(){ return "ObjectSystem"; }

    GameObject* CreateObject();

    //!Destroy all the GOC. Used in final shutdown procedure.
    void DestroyAllObjects();

    void DestroyGameObjectsToBeDestroyed();

    void ObjectSystem::LoadLevel(string level);

    /*!Used to generator unique GOCIds*/
    static unsigned LastGameObjectId;


    typedef std::unordered_map<string, ComponentCreator *> SerializationMap;
    SerializationMap SerialMap;

    typedef std::unordered_map<unsigned, GameObject*> GameObjectMap;
    GameObjectMap GameObjects;

  private:

    void ObjectSystem::SerializeObject(Serializer::DataNode* data);
    void ObjectSystem::SerializeComponent(string ComponentName, Serializer::DataNode* data);

    void RegisterComponents(void);
    void AddComponentCreator(string name, ComponentCreator* creator);
    void InitializeObject ();
    typedef std::vector<GameObject *> ObjectsToBeDestroyed;
    ObjectsToBeDestroyed GameObjectsToBeDestroyed;
  };

}
