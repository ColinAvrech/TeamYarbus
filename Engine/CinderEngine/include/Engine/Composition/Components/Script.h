/******************************************************************************/
/*!
\file   Script.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This component loads Zilch scripts
*/
/******************************************************************************/


#pragma once

#include "Common.h"
#include "Zilch.hpp"

namespace Framework
{
  namespace Scripting
  {
    class ZilchComponent : public GameComponent
    {
    public:
      //Constructor
      ZilchComponent(GOC* obj);
      ~ZilchComponent();
      virtual void Initialize();

    private:
      //The script file name
      std::string ScriptSource;
      
    };
  }
}