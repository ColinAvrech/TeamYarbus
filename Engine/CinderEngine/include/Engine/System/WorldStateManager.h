/******************************************************************************/
/*!
\file   WorldStateManager.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "MathExtensionLibrary.h"
#include "BaseSystem.h"

namespace Framework
{
	class WorldStateManager;
	extern WorldStateManager* WORLDSTATE;
	class WorldStateManager : public BaseSystem
	{
	public:
		enum worldState
		{
			WS_DARK_WORLD = 0,
			WS_LIGHT_WORLD,
			WS_TRANS_TOLIGHT,
			WS_TRANS_TODARK
		};
    WorldStateManager();
    ~WorldStateManager(){}

		virtual bool Initialize();
		virtual void Update(const float& dt);
		const string GetName();
		void OnApplicationPause(PauseEvent* pauseEvent);
		void ToggleState();

	private:
		worldState currentState;
		worldState lastState;
	};
}
