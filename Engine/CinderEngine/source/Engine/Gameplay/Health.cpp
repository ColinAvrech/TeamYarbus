/******************************************************************************/
/*!
\file   Health.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/
#include "Health.h"
#include "EventSystem.h"
#include "UpdateEvent.h"

namespace Framework
{
	void Health::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* value = data->FindElement(data, "alive");
		value->GetValue(&alive);

		value = data->FindElement(data, "maxRadius");
		value->GetValue(&maxRadius);

		value = data->FindElement(data, "minRadius");
		value->GetValue(&minRadius);

		value = data->FindElement(data, "reFueling");
		value->GetValue(&reFueling);
	}

	void Health::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
		gameObject->Health = this;

		currentRadius = maxRadius;
	}

	void Health::Update(UpdateEvent* e)
	{
		float deathRate = .01f;
		if (!reFueling)
		{
			currentRadius -= deathRate * e->Dt;
			gameObject->Transform->Scale(currentRadius / maxRadius);
		}

		if (currentRadius == 0)
			printf("dead");
	}

	void Health::reFuel()
	{
		reFueling = true;
	}

	DefineComponentName(Health);
}