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

#include "GameObject.h"

#define DefineComponentName(Component) const static std::string Component::Name("Component")

namespace Framework
{
  /*! A Component is added to a Game object composition and is
  a small piece of logic for an object. 
  Ex, Transform component, Sprite Components*/
  class GameComponent
  {
  public:
    GameComponent(GameObject *obj)
      : Obj(obj){}

    // The non-base component usees DefineComponentName macro to name component
    const static std::string Name;

    /*!Telegraph that the component is active*/
    virtual void Initalize(){};

    /*!TODO IMPLIMENT SERIALIZATION!*/
    virtual void Serialize(){};

    //Variables that all components have
    const GameObject* Obj;
  };

}