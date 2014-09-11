/******************************************************************************/
/*!
\file   Template.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.

\brief  Game Object Compositions make up everything in the Cinder Engine.
They are the core of every object and hold a Vector of pointers to all of
their components. A component is a part of an object (ie. Physics, graphic,
AI, controlers). Game Object Composition have three different states which
they should be in during their lifespan.

-Constructed: The Composition has been created and there are components
which are being added to it.  Data on the components is being added via
serialization or hard code.

-Intialization: After all data have been added, the composition is
ready to be used. If the game is running the systems will then
access each aplicable component when running their update function.

-R.I.P. : Composition is about to die and will be deleted by the
factory in the next loop.


*/
/******************************************************************************/


#pragma once

#include "Common.h"
#include "Vec2.h"

namespace Framework
{
  class GameObject
  {
    //forward declairation
    class GameComponent;

  public:
    //! Called by Factory
    GameObject(unsigned gameObjectID)
      :GameObjectID(gameObjectID)
    {
      Parent = NULL;
    }


    template<typename T>
    GameComponent* AddComponent()
    {
      GameComponent* gc = new T(this);
      ComponentMap.Insert(T::Name, gc);
      return gc;
    }

    GameComponent* GetComponent(std::string component)
    {
      ComponentMap::iterator it = Components.find(component);
      if (it == Components.end())
      {
        return NULL;
      }
      else
      {
        return it->second;
      }
    }
    /*
    GameObject* CreateChildObject()
    {
      GameObject* go = new GameObject(OBJECTSYSTEM->LastGameObjectId, this);
      ++OBJECTSYSTEM->LastGameObjectId;
      ChildObjects.push_back(go);
      return go;
    }*/

    //Public Variables
    GameObject* Parent;
    Vec2 GOPosition;
    unsigned GameObjectID;
    /*
    std::list<GameObject*> ChildObjects;
    */

  private:
    /*
    GameObject(unsigned gameObjectID, GameObject* parent)
      :GameObjectID(gameObjectID), Parent(parent){}*/

    typedef std::map<std::string, GameComponent *> ComponentMap;
    ComponentMap Components;

  };
}
