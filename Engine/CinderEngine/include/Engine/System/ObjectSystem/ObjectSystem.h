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
    ObjectSystem();
    ~ObjectSystem();

    //!Create initialized the Id a GOC from a file.
    GameObject * GameObject(const std::string& filename);

    //!Add a GOC to the list to be destroyed
    void DestroyByPointer(GameObject * gameObject);

    //!Add a GOC to the list to be destroyed
    void DestroyById(unsigned id);

    //!Update the factory, Destroying dead objects
    virtual void Update(const double dt);

    //!name of the system if the Factory, duh.
    virtual const std::string GetName(){ return "Factory"; }

    //!Destroy all the GOC. Used in final shutdown procedure.
    void DestroyAllObjects();

    /*!Create and Id a GOC at runtime. Used to dynamically build GOC.
    After components have been added call GOC->Initializee().*/
    GameObject * CreateEmptyComposition();

    /*!Build a composition and serialize from the data file but do not initialize
    the GOC.
    Usage: to create composition and then adjust its data before initialization.
    Check GameObjectComposition::Initialize for details.*/
    GameObject * BuildAndSerialize(const std::string& filename);

    /*!Used to generator unique GOCIds*/
    static unsigned LastGameObjectId;

    /*!Add a component creator enabling data driven composition.*/
    void AddComponentCreator(const std::string& name, ComponentCreator* creator);

    /*!Get the game object with given id. This function will return NULL if
    the boject is destoryed.*/
    GameObject * GetObjectWithId(unsigned id);

  private:

    /*!Map the component creator used for data driven composition.*/
    typedef std::map<std::string, ComponentCreator*> ComponentMapType;
    ComponentMapType ComponentMap;

    /*!Map of GOC to their Ids used for safe referencing of game objects*/
    typedef std::map<unsigned, GameObject *> GameObjectedIdMapType;
    GameObjectedIdMapType GameObjectIdMap;
    typedef std::map<std::string, size_t> SerializationMap;
    SerializationMap  SerialMap;


  };


  extern ObjectSystem * OBJECTSYSTEM;
}
