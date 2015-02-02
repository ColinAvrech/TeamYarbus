/******************************************************************************/
/*!
\file   GameObject.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.

\brief  Game Object make up everything in the Cinder Engine.
They are the core of every object and hold a Vector of pointers to all of
their components. A component is a part of an object (ie. Physics, graphic,
AI, controlers). Game Object have three different states which
they should be in during their lifespan.

-Constructed: The Game Object has been created and there are components
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


#define ComponentPointer(Name) Name * Name = nullptr

#include "Common.h"
#include "ObjectSystem.h"
#include "Component.h"
#include "Vec2.h"
#include "CharacterController.h"
#include "Zilch.hpp"
#include "ScriptComponent.h"
#include "Health.h"
//#include "FireStarter.h"
//#include "Microphone.h"

namespace Framework
{
  class Camera;
  class ButtonController;
  class MenuController;
  class Component;
  class FireGroup;
  class Microphone;
  class Terrain2D;
  class Terrain3D;
  class ShapeCollider2D;
  class RigidBody2D;
  class Health;
  class AudioComponent;
  class PlayerStats;
  class SparkCollector;
  class FluidBody;
  class GUIText;

  class GameObject
  {
  public:
    ZilchDeclareBaseType(GameObject, Zilch::TypeCopyMode::ReferenceType);
	
	
	//ZilchDefineType(Zilch::Array<GameObject*>);
    //! Called by Factory
    GameObject(unsigned gameObjectID);
    virtual ~GameObject();

    // Adding Components
    Component* ZAddComponent(Zilch::String name);
    Component* AddComponent(string name);

   ZilchComponent* ZAddZilchComponent(Zilch::String name);
   Handle AddZilchComponent(string name);

    Component* ZGetComponent(Zilch::String component);
    Component* GetComponent(string component);
	
    //Public Variables
    String Name;
	  String GetName();
    GameObject* Parent;
	bool InheritRotation = true;
	bool InheritPosition = true;
	bool InheritScale = true;

	std::vector<GameObject*> children;

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

    const unsigned GameObjectID;


    //Components
    ComponentPointer(Transform);
    ComponentPointer(Sprite);
	ComponentPointer(Health);
    ComponentPointer (ShapeCollider2D);
    ComponentPointer (RigidBody2D);
    ComponentPointer(Camera);
    ComponentPointer(CharacterController);
	ComponentPointer(SparkCollector);
	ComponentPointer(FireGroup);
    ComponentPointer(Microphone);
    ComponentPointer(Terrain2D);
    ComponentPointer(Terrain3D);
    ComponentPointer(AudioComponent);
	ComponentPointer(PlayerStats);
    ComponentPointer(FluidBody);
    ComponentPointer(GUIText);
	ComponentPointer(Splitscreen);
	

   // Menu Components
   ComponentPointer(MenuController);
   ComponentPointer(ButtonController);

  public:

    typedef std::map<string, Component *> ComponentMap;
    ComponentMap Components;

  };
}

