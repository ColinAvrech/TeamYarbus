#include <Precompiled.h>
#include "WorldStateManager.h"

namespace Framework
{
	WorldStateManager* WORLDSTATE;
	WorldStateManager::WorldStateManager()
	{
		WORLDSTATE = this;
	}

	bool WorldStateManager::Initialize()
	{
		currentState = WS_DARK_WORLD;
		std::cout << GetName() + " initialized\n";
		EVENTSYSTEM->mConnect<PauseEvent, WorldStateManager>(Events::PAUSE, this, &WorldStateManager::OnApplicationPause);

		return true;
	}
	void WorldStateManager::Update(const float& dt)
	{
		switch (currentState)
		{
		case WS_DARK_WORLD:
			break;
		case WS_LIGHT_WORLD:
			break;
		case WS_TRANS_TOLIGHT:
			break;
		case WS_TRANS_TODARK:
			break;
		default:
			break;

		};
	}

	const string WorldStateManager::GetName()
	{
		return "WorldStateManager";
	}
	void WorldStateManager::OnApplicationPause(PauseEvent* pauseEvent)
	{
		paused = pauseEvent->Paused;
	}
	void WorldStateManager::ToggleState()
	{

	}

	//transitional period
	//if (currentState == TRANS_TOLIGHT ||
	//	currentState == TRANS_TODARK &&
	//	t_timer <= t_maxtime)
	//{
	//	t_timer += e->Dt;
	//	if (currentState == TRANS_TOLIGHT)
	//	{
	//		currentState = LIGHT_WORLD;
	//		lastState = TRANS_TOLIGHT;
	//	}
	//	else if (currentState == TRANS_TODARK)
	//	{
	//		currentState = DARK_WORLD;
	//		lastState = TRANS_TODARK;
	//	}
	//	lworld_timer += e->Dt;
	//}
	//if (lworld_timer <= lworld_maxtime)
	//{
	//	EnableInteractions();
	//}
	//else
	//	currentState = TRANS_TODARK;
	//if (currentState == DARK_WORLD)
	//{
	//	CalculateFlashlight();
	//}
	//}
} // namespace Frameowkr
