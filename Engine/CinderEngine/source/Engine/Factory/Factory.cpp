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

#include "Factory.h"

namespace Framework
{
  //!Set the factory to null to indicate is hasn't been created yet
  GameObjectFactory * FACTORY = NULL;

  GameObjectFactory::GameObjectFactory()
  {
    ErrorIf(FACTORY != NULL, "Factory Already Created");
    FACTORY = this;
    LastGameObjectId = 0;
  }

  GameObjectFactory::~GameObjectFactory()
  {
    /*!Delete all component creators*/
    ComponentMapType::iterator it = ComponentMap.begin();
    for (; it != ComponentMap.end(); ++it)
      delete it->second;            //! Delete the second element of the map (the ComponentCreator)
  }

  GOC * GameObjectFactory::CreateEmptyComposition()
  {
    /*!Creat ehe composition and give it an Id this
    function is used to create compositions progammatically
    versus through serialization*/
    GOC * gameObject = new GOC();
    IdGameObject(gameObject);
    return gameObject;
  }

  /*!TODO: NEEDS TO BE CREATED CURRENTLY A PLACE HOLDER!*/
  GOC * GameObjectFactory::BuildAndSerialize(const std::string& filename)
  {
    return NULL;
  }

  /*!Gives a game Object an Id and Indexes it into the GameObjectIdMap.*/
  void GameObjectFactory::IdGameObject(GOC* gameObject)
  {
    //! just increment the last id used.
#ifdef _DEBUG
    auto idcheck = LastGameObjectId;
    idcheck = -1;
    Assert(LastGameObjectId != idcheck, "All GameObject Ids have been used... Probably a loop somewhere creating millions of GOCs");
#endif
    ++LastGameObjectId;
    gameObject->ObjectId = LastGameObjectId;

    //! Store the game object in the global object id map
    GameObjectIdMap[LastGameObjectId] = gameObject;
  }

  /*!Find an object throught the GameObjectIdMap throught its Id.*/
  GOC * GameObjectFactory::GetObjectWithId(GOCId id)
  {
    //!Search the map for the specific GOC connected to an Id
    GameObjectedIdMapType::iterator it = GameObjectIdMap.find(id);

    if (it == GameObjectIdMap.end())
      return NULL;
    else
      return it->second;
  }

  /*!TODO: NEEDS TO BE CREAETED CURRENTLY A PLACE HOLDER!*/
  GOC * GameObjectFactory::Create(const std::string& filename)
  {
    GOC * goc = BuildAndSerialize(filename);
    if (goc) goc->Initialize();
    return goc;
  }

  /*!Destory a Game Object by pointer, faster route*/
  void GameObjectFactory::DestroyByPointer(GOC * gameObject)
  {
    /*!Add the object to the to be deleted list and then
    during the factory update it will be deleted*/
    ObjectsToBeDeleted.insert(gameObject);
  }

  /*!Destory a Game Object by Id, slower route*/
  void GameObjectFactory::DestroyById(GOCId id)
  {
    /*!Add the object to the to be deleted list and then
    during the factory update it will be deleted*/
    ObjectsToBeDeleted.insert(GetObjectWithId(id));
  }

  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void GameObjectFactory::Update(const double dt)
  {
    
    for (std::set<GOC*>::iterator it = ObjectsToBeDeleted.begin(); it != ObjectsToBeDeleted.end(); ++it)
    {
      GameObjectedIdMapType::iterator idItr = GameObjectIdMap.find((*it)->ObjectId);
      Assert(idItr != GameObjectIdMap.end(), "Object %d was double deleted or is bad memeory.", (*it)->ObjectId);
      if (idItr != GameObjectIdMap.end())
      {
        //! if object was found. Delete it and remove its entry from the Id map.
        delete (*it);
        GameObjectIdMap.erase(idItr);
      }
    }

    //!All objects ont he ObjectsToBeDeleted list have been deleted, clear list.
    ObjectsToBeDeleted.clear();
  }

  /*!Bind the creator for his component with the component name*/
  void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
  {
    ComponentMap[name] = creator;
  }

  void GameObjectFactory::DestroyAllObjects()
  {
    GameObjectedIdMapType::iterator it = GameObjectIdMap.begin();
    for (; it != GameObjectIdMap.end(); ++it)
      delete it->second;

    //! clear the IdMap of all ids
    GameObjectIdMap.clear();
    
  }
}