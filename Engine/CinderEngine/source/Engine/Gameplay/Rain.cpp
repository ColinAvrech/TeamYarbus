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

namespace Framework
{
	Rain::~Rain()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, Rain>(Events::UPDATEEVENT, this, &Rain::Update);
	}
	void Rain::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, Rain>(Events::UPDATEEVENT, this, &Rain::Update);
	}
	void Rain::Serialize(Serializer::DataNode* data)
	{

	}

	void Rain::Update(UpdateEvent* e)
	{
		
	}
	DefineComponentName(Rain);
}