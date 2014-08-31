
/******************************************************************************/
/*!
\file   ComponentCreator.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The factory uses the templated component creator to create all
every component nessesary.
*/
/******************************************************************************/

#pragma once

#include "ComponentTypeIds.h"

namespace Framework
{
  /*!Forward declaration of GameComponent*/
  class GameComponent;

  /*! Creates a interface for all componentcreators to create
  their components*/
  class ComponentCreator
  {
  public:
    ComponentCreator(ComponentTypeId typeId)
      :TypeId(typeId){}

    virtual ~ComponentCreator()=0;

    ComponentTypeId TypeId;

    /*! templated create function for creating */
    virtual GameComponent* Create() = 0;
  };

  /*!Templated helper shell to create a templated component*/
  template<typename type>
  class ComponentCreatorType : public ComponentCreator
  {
  public:
    //!Call constuctor of the component creator
    ComponentCreatorType(ComponentTypeId typeId)
      :ComponentCreator(typeId){}

    //!Override abstract fucntion to create
    virtual  GameComponent* Create()
    {
      return new type();
    }

    // Virtual Destructor for special creators
    virtual ~ComponentCreatorType()
    {}

  };
};

//! Registering a component puts the creater into a componentn creator map
//! This can then be used to create components from the factory.
#define RegisterComponent(type) FACTORY->AddComponentCreator( #type, new ComponentCreatorType<type>(CT_##type) );