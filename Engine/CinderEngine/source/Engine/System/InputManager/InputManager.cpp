/******************************************************************************/
/*!
\file   InputManager.cpp
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains an interface to acuuretly determine key states.
*/
/******************************************************************************/

#include "InputManager.h"
#include "WindowSystem.h"

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

	bool InputManager::IsMouseDown(unsigned button)
	{
		
		return MouseDown[button];
	}

	bool InputManager::IsMouseTriggered(unsigned button)
	{
		
		return MouseDown[button] && !MouseWasDown[button];
		
	}

	bool InputManager::IsMouseReleased(unsigned button)
	{
		return !MouseDown[button] && MouseWasDown[button];
	}

	bool InputManager::IsKeyDown(unsigned key)
	{
		return KeyPressed[key];
	}

	bool InputManager::IsKeyTriggered(unsigned key)
	{
		return KeyPressed[key] && !KeyWasPressed[key];
	}

	bool InputManager::IsKeyReleased(unsigned key)
	{
		return !KeyPressed[key] && KeyWasPressed[key];
	}

	void InputManager::KeyChange(const int& key, const int& scanCode, const int& state, const int& mod)
	{
		if (key < GLFW_KEY_LAST)
		{
			KeyPressed[key] = bool(state);
		}
		
	}

	void InputManager::MouseChange(const int& button, const int& action, const int& mod)
	{
		if (button < GLFW_KEY_LAST)
		{
			MouseDown[button] = bool(action);
			
		}
		
		
	}

	void InputManager::Update()
	{
		
		
		for (unsigned i = 0; i < GLFW_KEY_LAST; ++i)
		{
			KeyWasPressed[i] = KeyPressed[i];
			
		}

		for (unsigned j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
		{
			MouseWasDown[j] = MouseDown[j];
			
		}

		
	}
}