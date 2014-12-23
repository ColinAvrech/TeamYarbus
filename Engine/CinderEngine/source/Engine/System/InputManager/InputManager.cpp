/******************************************************************************/
/*!
\file   InputManager.cpp
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains an interface to acuuretly determine key states.
*/
/******************************************************************************/

#include <Precompiled.h>
#include "InputManager.h"
#include "WindowSystem.h"

#pragma warning(disable : 4800)

namespace Framework
{
	bool InputManager::MouseDown[8] = { false };
	bool InputManager::MouseWasDown[8] = { false };
	bool InputManager::KeyPressed[348] = { false };
	bool InputManager::KeyWasPressed[348] = { false };

	ZilchDefineType(InputManager, CinderZilch)
	{
		ZilchBindStaticMethod(IsMouseDown);
		ZilchBindStaticMethod(IsMouseTriggered);
		ZilchBindStaticMethod(IsMouseReleased);
		ZilchBindStaticMethod(IsKeyDown);
		ZilchBindStaticMethod(IsKeyTriggered);
		ZilchBindStaticMethod(IsKeyReleased);
	}

  //Mouse
	bool InputManager::IsMouseDown(unsigned button)
	{
		return MouseWasDown[button];
	}

	bool InputManager::IsMouseTriggered(unsigned button)
	{
		return !MouseDown[button] && MouseWasDown[button];
	}

	bool InputManager::IsMouseReleased(unsigned button)
	{
		return MouseDown[button] && !MouseWasDown[button];
	}

  //Keyboard
	bool InputManager::IsKeyDown(unsigned key)
	{
    return KeyWasPressed[key];
	}

	bool InputManager::IsKeyTriggered(unsigned key)
	{
		return !KeyPressed[key] && KeyWasPressed[key];
	}

	bool InputManager::IsKeyReleased(unsigned key)
	{
		return KeyPressed[key] && !KeyWasPressed[key];
	}

  //Callbacks
	void InputManager::KeyChange(const int& key, const int& scanCode, const int& state, const int& mod)
  {
    if (key < GLFW_KEY_LAST)
    {
      // Set the Key's State      
      KeyWasPressed[key] = static_cast<bool>(state);
    }	
  }

	void InputManager::MouseChange(const int& button, const int& action, const int& mod)
  {
    if (button < GLFW_MOUSE_BUTTON_LAST)
    {
      // Set the Mouse's State
      MouseWasDown[button] = static_cast<bool>(action);
    }
  }

  //Bool update
	void InputManager::Update()
	{
		for (unsigned i = 0; i < GLFW_KEY_LAST; ++i)
		{
       KeyPressed[i] = KeyWasPressed[i];
		}

		for (unsigned i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
		{
      MouseDown[i] = MouseWasDown[i];
		}

	}
}