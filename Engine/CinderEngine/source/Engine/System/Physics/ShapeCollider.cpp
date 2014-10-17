/******************************************************************************/
/*!
\file   ShapeCollider.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "ComponentInclude.h"
#include "ShapeCollider.h"
#include "EventSystem.h"

#pragma once
namespace Framework
{
	void LineCollider::Initialize()
	{
		std::string ColEvent = std::string("UpdateEvent");

		CollisionEvent* colevent = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
		if (colevent == nullptr)
		return;

		// FILL OUT EVENT
		//EVENTSYSTEM->Connect(gameObject, "UpdateEvent", Events::LOGICUPDATE);
	}

	void CircleCollider::Initialize()
	{
		std::string ColEvent = std::string("UpdateEvent");

		CollisionEvent* colevent = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
		if (colevent == nullptr)
			return;
	}

  DefineComponentName(ShapeCollider);
	DefineComponentName(CircleCollider);
	DefineComponentName(PointCollider);
	DefineComponentName(LineCollider);
}