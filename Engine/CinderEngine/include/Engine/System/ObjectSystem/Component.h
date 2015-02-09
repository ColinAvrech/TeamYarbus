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

#include "Hash.h"

#define DefineComponentName(Component) const string Component::Name(#Component)

namespace Framework
{
  class GameObject;
  class PauseEvent;

  /*! A Component is added to a Game object composition and is
  a small piece of logic for an object.
  Ex, Transform component, Sprite Components*/
  class Component : public Zilch::IZilchObject
  {
    public:
      ZilchDeclareBaseType(Component, Zilch::TypeCopyMode::ReferenceType);
      // Zilch Definition in GameObject.cpp

      Component()
      {
        enabled = true;
        paused = false;
      }
      virtual ~Component() {}

      /*!Telegraph that the component is active*/
      virtual void Initialize()
      {
        ErrorIf(true, "Initialize Component Not Overriden");
      }

      /*!TODO IMPLIMENT SERIALIZATION!*/
      virtual void Serialize(Serializer::DataNode* data)
      {
        ErrorIf(true, "Serialize Component Not Overriden");
      }

      virtual void OnApplicationPause(PauseEvent* pause)
      {}

      virtual void DeleteThis() { delete this; }

      GameObject* GetOwner();
      // Definition in GameObject.cpp

      //Variables that all components have
      GameObject* gameObject;
      bool enabled;

      Hash::HashValue mComponentType;

    protected:
      void Get_Enabled(Serializer::DataNode* data, std::string enableKey = "Enabled")
      {
        Serializer::DataNode* value = data->FindElement(data, enableKey.c_str());
        value->GetValue(&enabled);
      }

      bool paused;
  };
}