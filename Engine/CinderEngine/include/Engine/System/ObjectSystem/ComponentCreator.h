///////////////////////////////////////////////////////////////////////////////////////
///
///	\file ComponentCreator.h Defines IComponentCreator interface.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Common.h"

namespace Framework
{
  //Forward declaration of component
  class GameComponent;

  ///Game Component Creator Interface.
  ///Used to create components in data driven method in the GameObjectFactory.
  class ComponentCreator
  {
  public:
    ComponentCreator(std::string componentName)
      :m_ComponentCreatorName(componentName)
    {
    }

    std::string m_ComponentCreatorName;
    ///Create the component

    virtual GameComponent * Create(GameObject* obj) = 0;
  };

  ///Templated helper class used to make concrete component creators.
  template<typename type>
  class ComponentCreatorType : public ComponentCreator
  {
  public:
    ComponentCreatorType(std::string componentName)
      :ComponentCreator(componentName)
    {
    }

    virtual GameComponent * Create(GameObject* obj)
    {
      return new type(obj);
    }
  };

};

//Register component macro
#define RegisterComponent(type) OBJECTSYSTEM->AddComponentCreator( #type, new ComponentCreatorType<type>("type") );