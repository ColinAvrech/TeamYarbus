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
  //!Set the factory to null to indicate is hasn't been created yet
  ObjectSystem * OBJECTSYSTEM = NULL;

  ObjectSystem::ObjectSystem()
  {
    ErrorIf(OBJECTSYSTEM != NULL, "Factory Already Created");
    OBJECTSYSTEM = this;
    LastGameObjectId = 0;
  }

  ObjectSystem::~ObjectSystem()
  {
    /*!Delete all component creators*/
    ComponentMapType::iterator it = ComponentMap.begin();
    for (; it != ComponentMap.end(); ++it)
      delete it->second;            //! Delete the second element of the map (the ComponentCreator)
  }

  GameObject * ObjectSystem::CreateEmptyComposition()
  {
    /*!Creat ehe composition and give it an Id this
    function is used to create compositions progammatically
    versus through serialization*/
    GameObject * gameObject = new GameObject(LastGameObjectId);
    ++LastGameObjectId;
    GameObjectIdMap[LastGameObjectId] = gameObject;

    //! just increment the last id used.
#ifdef _DEBUG
    auto idcheck = LastGameObjectId;
    idcheck = -1;
    Assert(LastGameObjectId != idcheck, "All GameObject Ids have been used... Probably a loop somewhere creating millions of GameObjects");
#endif

    return gameObject;
  }

  /*!TODO: NEEDS TO BE CREATED CURRENTLY A PLACE HOLDER!*/
  GameObject * ObjectSystem::BuildAndSerialize(const std::string& filename)
  {
    return NULL;
  }
  
  /*!Find an object throught the GameObjectIdMap throught its Id.*/
  GameObject * ObjectSystem::GetObjectWithId(unsigned id)
  {
    //!Search the map for the specific GOC connected to an Id
    GameObjectedIdMapType::iterator it = GameObjectIdMap.find(id);

    if (it == GameObjectIdMap.end())
      return NULL;
    else
      return it->second;
  }

  GameObject * ObjectSystem::CreateGameObject(const std::string& filename)
  {
    GameObject * goc = BuildAndSerialize(filename);
    if (goc) goc->Initialize();
    return goc;
  }

  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void ObjectSystem::Update(const double dt)
  {

  }

  /*!Bind the creator for his component with the component name*/
  void ObjectSystem::AddComponentCreator(const std::string& name, ComponentCreator* creator)
  {
    ComponentMap[name] = creator;
  }

  void ObjectSystem::DestroyAllObjects()
  {
    GameObjectedIdMapType::iterator it = GameObjectIdMap.begin();
    for (; it != GameObjectIdMap.end(); ++it)
      delete it->second;

    //! clear the IdMap of all ids
    GameObjectIdMap.clear();
    
  }

  static unsigned LastGameObjectId = 0;
}