/******************************************************************************/
/*!
\file   Component.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The GameComponent class is inherited by all components. It has 
an initialize function which will talk with the other system to set
itself up. (ex. Sprite talks to Graphics, Colliders talk to physics 
relaying information)
*/
/******************************************************************************/

/*!

/auther Micah
/file component.h

/brief

*/
#pragma once
#include "Common.h"
#include "Serializer/DataNode.h"

#define DefineComponentName(Component) const std::string Component::Name("Component")

namespace Framework
{
  class GameObject;

  /*! A Component is added to a Game object composition and is
  a small piece of logic for an object. 
  Ex, Transform component, Sprite Components*/
  class Component
  {
  public:

    /*!Telegraph that the component is active*/
    virtual void Initialize ()
    {
      ErrorIf (true, "Initialize Component Not Overriden");
    }

    /*!TODO IMPLIMENT SERIALIZATION!*/
    virtual void Serialize (Serializer::DataNode* data)
    {
      ErrorIf (true, "Serialize Component Not Overriden");
    }

    //Variables that all components have
    GameObject* gameObject;
  };

}