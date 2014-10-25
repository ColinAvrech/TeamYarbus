/******************************************************************************/
/*!
\file   GravityEffect.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "EventSystem.h"
#include "ObjectSystem.h"
#include "GravityEffect.h"

namespace Framework
{
	void GravityEffect::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* temp;
		//Active
		bool Active;
		temp->GetValue(&Active);
		temp = data->FindElement(data, "Active");
		if (Active)
			active = true;
		else
			active = false;

		//Strength
		float Strength;
		temp->GetValue(&Strength);
		temp = data->FindElement(data, "Strength");
		strength = Strength;

		//Direction
		glm::vec3 Direction;
		temp->GetValue(&Active);
		temp = data->FindElement(data, "Direction");
		direction.x = Direction.x;
		direction.y = Direction.y;
	}
}