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
  }

  ObjectSystem::~ObjectSystem()
  {
  }

  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void ObjectSystem::Update(const double dt)
  {

  }

  void ObjectSystem::DestroyAllObjects()
  {    
  }
}