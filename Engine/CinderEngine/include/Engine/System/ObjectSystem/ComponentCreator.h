/******************************************************************************/
/*!
\file   ComponentCreator.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "Common.h"

namespace Framework
{
  //Forward declaration of component
  class Component;
  class GameObject;

  ///Game Component Creator Interface.
  ///Used to create components in data driven method in the ObjectSystem.
  class ComponentCreator
  {
  public:
    ComponentCreator(string componentName)
      :m_ComponentCreatorName(componentName)
    {
    }

    string m_ComponentCreatorName;
    ///Create the component

    virtual Component * Create() = 0;
  };

  ///Templated helper class used to make concrete component creators.
  template<typename T>
  class ComponentCreatorType : public ComponentCreator
  {
  public:
    ComponentCreatorType(string componentName)
      :ComponentCreator(componentName)
    {
    }

    virtual Component * Create()
    {
      return new T();
    }
  };

};

//Register component macro
#define RegisterComponent(type) AddComponentCreator( #type, new ComponentCreatorType<type>(#type) );