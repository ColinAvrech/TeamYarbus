/*****************************************************************************/
/*!
\file   ScriptComponent.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Scriptcomponents bind components from Zilch to the engine.
*/
/*****************************************************************************/

//!makes sure this is only included once
#pragma once

#include "Common.h"
#include "Component.h"
#include "Zilch.hpp"

namespace Framework
{
  namespace ScriptSystem
  {
    //NOTES-------------------------------------------------------------
    //To later check if a GameComponent is a ZilchComponent:
    //GameComponent *c = ....
    //ZilchComponent *zc = dynamic_cast<ZilchComponent*>(c);
    //if(zc != 0) // is ZilchComponent

    //pass std::string str.c_str() to convert to const char*
    //------------------------------------------------------------------

    class ZilchComponent : public Component
    {
    public:

      //Constructor. Initialize with owner and component name.
      ZilchComponent(GameObject *obj, std::string* name);

      //!Updated every frame.
      void Update(const double dt);

      //!Initialize the system.
      virtual bool Initialize();

      /*!TODO IMPLIMENT SERIALIZATION!*/
      virtual void Serialize(){};

      //Destructor
      ~ZilchComponent();

#ifdef _DEBUG
    private:
      //Private data ----------------------------------
      //Zilch class name
      Zilch::StringParam ZilchClass;
      //Zilch class (Zilch::BoundType*)
      Zilch::BoundType* This;
      //Zilch library for this component
      Zilch::LibraryRef* library;
      //Zilch functions (Zilch::Function*)
      //Initialize
      Zilch::Function* ZilchInitialize;
      //Update
      Zilch::Function* Run;
      //Zilch exception handler (Zilch::ExceptionReport)
      Zilch::ExceptionReport report;

#endif
    }; //class scriptcomponent
  }  //namespace ScriptSystem
}  //namespace framework