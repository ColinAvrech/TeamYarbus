/******************************************************************************/
/*!
\file   Rain.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Rain behavior - randomized and interacting with wind
*/
/******************************************************************************/
#include <Precompiled.h>
#include "Rain.h"
#include "Lantern.h"

namespace Framework
{
	Rain::~Rain()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, Rain>(Events::UPDATEEVENT, this, &Rain::Update);
	}
	void Rain::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, Rain>(Events::UPDATEEVENT, this, &Rain::Update);
		rainAngle = 90.f;
	}
	void Rain::Serialize(Serializer::DataNode* data)
	{

	}

	void Rain::Update(UpdateEvent* e)
	{
    GameObject* player = OBJECTSYSTEM->ptrPlayer;
		float afg = player->C<Lantern>()->getAngleFromGround();
		float lt = player->C<Lantern>()->getLightTheta();

		/*check if lantern is blocking rain*/
		if (rainAngle >= afg && rainAngle <= afg + lt)
		{
			player->C<Lantern>()->setRainedOn(false);
		}
		else
		{
			player->C<Lantern>()->setRainedOn(true);
		}
	}
	DefineComponentName(Rain);
}