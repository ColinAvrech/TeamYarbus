#pragma once

#include "ZilchCompiledLib.h"


namespace Framework
{
	class InputManager
	{
		ZilchDeclareBaseType(InputManager, Zilch::TypeCopyMode::ReferenceType);

	public:
		static bool IsMouseDown(unsigned key);
		static bool IsMouseTriggered(unsigned key);
		static bool IsMouseReleased(unsigned key);
		static bool IsKeyDown(unsigned key);
		static bool IsKeyTriggered(unsigned key);
		static bool IsKeyReleased(unsigned key);
		static void KeyChange(const int& key, const int& scanCode, const int& state, const int& mod);
		static void MouseChange(const int& button, const int& state, const int& mod);
		static void Update();
	private:
		static bool MouseDown[8];
		static bool MouseWasDown[8];
		static bool KeyPressed[348];
		static bool KeyWasPressed[348];

	};
}
