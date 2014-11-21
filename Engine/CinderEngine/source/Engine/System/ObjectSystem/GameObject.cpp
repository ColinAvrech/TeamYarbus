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
#include "Zilch.hpp"

namespace Framework
{
  GameObject* Component::GetOwner() { return gameObject; }

  ZilchDefineType(Component, CinderZilch)
  {
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	
	ZilchBindConstructor();
    ZilchBindMethod(GetOwner);	
	ZilchBindMethod(Initialize);
	ZilchBindFieldGetSetAs(gameObject, "Owner");
  }

  ZilchDefineType(GameObject, CinderZilch)
  {
    //Pointerrs
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);

    ZilchBindFieldGet(Transform);
    ZilchBindFieldGet(Sprite);
	  //ZilchBindFieldGet(Name);
	  ZilchBindMethodOverloadAs(ZGetComponent, "GetComponent", Component*, Zilch::String);
    //ZilchBindFieldGetSet(ShapeCollider);
    //ZilchBindFieldGetSet(Camera);
    ZilchBindFieldGet(RigidBody);
    //ZilchBindFieldGetSet(CharacterController);

    // 
    ZilchBindMethodOverloadAs(ZAddComponent, "AddComponent", Component*, Zilch::String);
    ZilchBindMethodOverloadAs(ZAddZilchComponent, "AddZilchComponent", ZilchComponent*, Zilch::String);
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

  // Zilch method for adding components to GameObjects
  Component* GameObject::ZAddComponent(Zilch::String name)
  {
    string stdname = name.c_str();
      ErrorIf(Components.find(stdname) != Components.end(), "COMPONENT CREATED TWICE ON SAME OBJECT");
    if (OBJECTSYSTEM->SerialMap.find(stdname) != OBJECTSYSTEM->SerialMap.end())
    {
      Component* gc = OBJECTSYSTEM->SerialMap[stdname]->Create();
      Components[stdname] = gc;
      gc->gameObject = this;
      gc->Initialize();
      return gc;
    }
    else
    {
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << std::endl;
      return NULL;
    }
  }

  Component* GameObject::AddComponent(string name)
  {
    ErrorIf(Components.find(name) != Components.end(), "COMPONENT CREATED TWICE ON SAME OBJECT");
    if (OBJECTSYSTEM->SerialMap.find(name) != OBJECTSYSTEM->SerialMap.end())
    {
      Component* gc = OBJECTSYSTEM->SerialMap[name]->Create();
      Components[name] = gc;
      gc->gameObject = this;
      return gc;
    }
    else
    {
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << std::endl;
      return NULL;
    }
  }

  ZilchComponent* GameObject::ZAddZilchComponent(Zilch::String name)
  {
    string stdname = name.c_str();

    //not sure about error handling
    ZilchComponent* zc = new ZilchComponent(stdname);
    return zc;
  }
  ZilchComponent* GameObject::AddZilchComponent(string name)
  {
	  
    //bob = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);

    //not sure about error handling
    ZilchComponent* zc = new ZilchComponent(name);
    //zc->Initialize();
    return zc;
  }

  Component* GameObject::GetComponent(string component)
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

  //JOSH
  Component* GameObject::ZGetComponent(Zilch::String component)
  {
	  string stdcomp = component.c_str();
	  ComponentMap::iterator it = Components.find(stdcomp);
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
