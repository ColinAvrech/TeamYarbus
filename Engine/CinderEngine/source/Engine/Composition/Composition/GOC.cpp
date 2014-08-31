/******************************************************************************/
/*!
\file   GOC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The Game Object Composition (GOC) makes up all objects in the game engine.
it holds an array of pointers to its components which will be initialized before
the level runs.
*/
/******************************************************************************/

#include "ComponentTypeIds.h"
#include "Factory.h"
#include "GOC.h"
#include <algorithm> //! std::sort

namespace Framework
{
  //! Sorts component by Id.
  struct ComponentSorter
  {
    /*!When overloading the functaion call we can type the struct/class name
    then "()" to call the function. so to use ComponentSorter we call Component
    Sorter(left,right)*/
    bool operator()(GameComponent* left, GameComponent* right)const
    {
      return left->TypeId < right->TypeId;
    }
  };

  //! Binary Search in a sorted array of Components. If the component cannot be found NULL is returned.
  GameComponent* BinaryComponentSearch(ComponentArray& components, ComponentTypeId name)
  {
    unsigned begin = 0;
    unsigned end = components.size();

    while (begin < end)
    {
      unsigned mid = (begin + end) / 2;
      if (components[mid]->TypeId < name)
        begin = mid + 1;
      else
        end = mid;
    }

    //! still in index (not size + 1) and valid. Return component
    if ((begin < components.size()) && (components[begin]->TypeId == name))
      return components[begin];
    else
      return NULL;
  }


  void GameObjectComposition::Initialize()
  {
    /*! We need to sort the components before initalization so that they 
    are initialized in a specific order so that component dependencies aren't 
    an issue.*/
    std::sort(Components.begin(), Components.end(), ComponentSorter() );

    /*!Initialize all component on this composition.
    Set the base pointer. This allows each component to
    initialize itself without a constructor which is useful
    in combination with serialization.*/
    for (ComponentArray::iterator it = Components.begin(); it != Components.end(); ++it)
    {
      (*it)->Base = this;
      (*it)->Initalize();
    }
  }

  GameObjectComposition::GameObjectComposition() : ObjectId(0){}

  GameObjectComposition::~GameObjectComposition()
  {
    /*! Delete each component using the component's virtual destructor
    takes care of all resouces in memeory*/
    for (ComponentArray::iterator it = Components.begin(); it != Components.end(); ++it)
      delete *it;
  }

  //! Adds an unsorted component to the component array
  void GameObjectComposition::AddComponent(ComponentTypeId typeId, GameComponent* component)
  {
    //! Store the component type id in the component.
    component->TypeId = typeId;
    Components.push_back(component);
  }

  /*! Searches the Component array for a specific component. CANNNOT BE DONE
  BEFORE INITIALIZATION*/
  GameComponent* GameObjectComposition::GetComponent(ComponentTypeId typeId)
  {
    return BinaryComponentSearch(Components, typeId);
  }

  void GameObjectComposition::Destroy()
  {
    //! Signal the factory to destory this composition next frame
    FACTORY->DestroyByPointer(this);
  }
}