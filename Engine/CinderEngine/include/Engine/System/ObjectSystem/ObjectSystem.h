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

#include "BaseSystem.h"
#include "Component.h"
#include "ComponentCreator.h"

namespace Framework
{
  /*!Game Object Factory:
  The Game object factory creates composition objects from data streams and manages
  their lifetimes. As part of controlling the life times of the GameObjectComposition (GOC)
  it also provides an integer based Id system for safe referencing of game objects
  through integer Id Handles.
  */
  class ObjectSystem : public BaseSystem
  {
  public:

    const char* LevelAssetsPath = "../../Assets/Levels/";

    ObjectSystem();
    ~ObjectSystem();

    //!Update the factory, Destroying dead objects
    virtual void Update(const double dt);

    //!name of the system if the Factory, duh.
    virtual const std::string GetName(){ return "ObjectSystem"; }

    GameObject* CreateObject();

    //!Destroy all the GOC. Used in final shutdown procedure.
    void DestroyAllObjects();

    void DestroyGameObjectsToBeDestroyed();

    void LoadLevel(std::string level);

    /*!Used to generator unique GOCIds*/
    static unsigned LastGameObjectId;

  private:

    void RegisterComponents(void);
    void AddComponentCreator(std::string name, ComponentCreator* creator);


    typedef std::map<unsigned, GameObject*> GameObjectMap;
    GameObjectMap GameObjects;

    typedef std::map<std::string, ComponentCreator *> SerializationMap;
    SerializationMap SerialMap;

    typedef std::vector<GameObject *> ObjectsToBeDestroyed;
    ObjectsToBeDestroyed GameObjectsToBeDestroyed;


  };


  extern ObjectSystem * OBJECTSYSTEM;
}
