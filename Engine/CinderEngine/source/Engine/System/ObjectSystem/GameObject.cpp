/******************************************************************************/
/*!
\file   GameObject.cpp
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
	  ZilchBindMethod(GetName);
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
    Parent = nullptr;
  }

  GameObject::~GameObject()
  {
    for(auto comp : Components)
    {
      if (comp.second != nullptr)
      {
        delete comp.second;
        comp.second = nullptr;
      }
    }
  }
	

  Zilch::String GameObject::GetName()
  {
	  return String(Name.c_str());
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
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component: " << stdname.c_str() << std::endl;
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
      std::cout << CinderConsole::red << "ERROR, Object System requested to create unregistered component" << CinderConsole::cyan << name.c_str() << std::endl;
      return NULL;
    }
  }

  //DOES NOT WORK!!!
  ZilchComponent* GameObject::ZAddZilchComponent(Zilch::String name)
  {
    string stdname = name.c_str();
	//DOES NOT WORK
    //not sure about error handling
    //ZilchComponent* zc = new ZilchComponent(stdname);
    //Components[name.c_str()] = zc;
    return nullptr;
  }
  Zilch::Handle GameObject::AddZilchComponent(string name)
  {
	  
    //bob = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);
	  //Get pointer to appropriate library
	  Zilch::LibraryRef* library = &(ZILCH->lib);
	  //Bind the Zilch class
	  Zilch::BoundType* ZilchClass = (*library)->BoundTypes.findValue(name.c_str(), nullptr);
	  ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", name);
	  
	  Zilch::ExceptionReport report;
	  Zilch::ExecutableState* state = ZILCH->GetDependencies();
	  Zilch::Handle ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);
	  
	  //Create an array of arguments
	  Zilch::Array<Zilch::Type*> args;
	  args.push_back(ZilchTypeId(GameObject*));
	  //Find the Create function
	  // We pass in an array of types to specify the arguments we want, in this case its an empty array
	  // We also pass in the void type because we don't expect a return value
	  
	  Function* ZilchCreate = ZilchClass->FindFunction("Create", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
	  ErrorIf(ZilchCreate == nullptr, "Failed to find function 'Create' on Zilch type ", ZilchClass);

	  {
		  // Invoke the Create function, which assigns this object an owner.
		  Zilch::Call call(ZilchCreate, ZILCH->GetDependencies());
		  call.SetHandle(Zilch::Call::This, ActiveScript);
		  call.SetHandle(0, this);
		  call.Invoke(report);
	  }
	  
	  /*
	  for (int i; i < ActiveScript.Type->GetFieldMap(false).count; ++i)
	  {
		  
	  }
	  */
	  //Field* owner = ZilchClass->InstanceFields["Butts"];
	  //ActiveScript.Type->AddRawField(owner);

	  Components[name] = (Component*) ActiveScript.Type;
	  //Components.at(name)->gameObject = this;
	  //Function* ZilchInitialize = ZilchClass->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
	  //ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ZilchClass);

	  
	  // The exception report stores any exceptions that may have occurred while executing code
	  // Exceptions include accessing arrays out of bounds, dereferencing null, etc
	  
    //not sure about error handling
    //ZilchComponent* zc = new ZilchComponent(name);
    //zc->Initialize();
    return ActiveScript;
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
