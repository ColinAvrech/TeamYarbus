/******************************************************************************/
/*!
\file   GOC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once

#include "GameObject.h"
#include "ScriptComponent.h"
#include "Component.h"

namespace Framework
{
  // bindComponent Points
  ZilchDefineType(GameObject, CinderZilch)
  {
    ZilchBindFieldGet(Transform);
    ZilchBindFieldGet(Sprite);
    //ZilchBindFieldGetSet(ShapeCollider);
    //ZilchBindFieldGetSet(Camera);
    //ZilchBindFieldGetSet(RigidBody);
    //ZilchBindFieldGetSet(CharacterController);
  }

  GameObject::GameObject(unsigned gameObjectID)
  :GameObjectID(gameObjectID)
  {
    Parent = NULL;
  }

  GameObject::~GameObject()
  {
    for each (auto comp in Components)
    {
      delete comp.second;
    }
  }

  Component* GameObject::AddComponent(std::string name)
  {
    ErrorIf(Components.find(name) != Components.end(), "COMPONENT CREATED TWICE ON SAME OBJECT");
    if (OBJECTSYSTEM->SerialMap.find(name) != OBJECTSYSTEM->SerialMap.end())
    {
      Component* gc = OBJECTSYSTEM->SerialMap[name]->Create();
      Components[name] = gc;
      return gc;
    }
    else
    {
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << std::endl;
      return NULL;
    }
  }

  Component* GameObject::AddZilchComponent(std::string name)
  {
    //not sure about error handling
    Component* zc = new ZilchComponent(name);
    return zc;
  }

  Component* GameObject::GetComponent(std::string component)
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
    return NULL;
  }

}
