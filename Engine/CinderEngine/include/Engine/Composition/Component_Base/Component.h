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

#include "ComponentTypeIds.h"

namespace Framework
{
  //!Forward Declartion of Game Object Composition clas
  class GameObjectComposition;
  //! Acrynim for Game Object Composition
  typedef GameObjectComposition GOC;

  /*! A Component is added to a Game object composition and is
  a small piece of logic for an object. 
  Ex, Transform component, Sprite Components*/
  class GameComponent
  {
  public:
    /*! Game Object Composition will need to be
    able to see our privates.*/
    friend class GameObjectComposition;

    /*!Telegraph that the component is active*/
    virtual void Initalize(){};

    /*!TODO IMPLIMENT SERIALIZATION!*/
    virtual void Serialize(){};

    GOC* GetOwner(){ return Base; }

    /*! Id of the component used to find the component in array*/
    ComponentTypeId TypeId;

    /*!Protected members/variables are only accessible from within
    the class defining them. This prevents other GOCs from destroying
    the wrong components.*/
  protected:
    /*!Destroy he component. This can only be done by the Game Object Composition*/
    virtual ~GameComponent(){};

  private:
    /*!Each compoent has a pointer to its base composition*/
    GOC* Base;
  };

}