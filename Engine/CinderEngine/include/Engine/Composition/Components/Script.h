/******************************************************************************/
/*!
\file   Script.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The generic class for loading lua scripts and attaching them as
        game object components.
*/
/******************************************************************************/


#pragma once

#include "Component.h"
#include "GOC.h"
#include "lua.hpp"
#include "Luabridge.h"

namespace Framework
{
  class ScriptComponent : public GameComponent
  {
  public:
    ScriptComponent(GOC* obj);          //Constructor
    ~ScriptComponent();                 //Destructor
    bool isScript() { return true; }    //Confirm the component is a script
    void RunScript();                   //Run the script on update

  private:
    luabridge::LuaRef *luaClass;        //pointer to a lua table
    lua_State *LState;                  //pointer to the lua state in which the script is loaded
  };
}