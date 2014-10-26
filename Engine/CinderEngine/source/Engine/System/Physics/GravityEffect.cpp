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
		//Active
		bool Active;
		Serializer::DataNode* temp = data->FindElement(data, "Active");	
		temp->GetValue(&Active);
		if (Active)
			active = true;
		else
			active = false;

		//Strength
		float Strength;
		temp = data->FindElement(data, "Strength");
		temp->GetValue(&Strength);
		strength = Strength;

		//Direction
		glm::vec3 Direction;
		temp = data->FindElement(data, "Direction");
		temp->GetValue(&Active);
		direction.x = Direction.x;
		direction.y = Direction.y;
	}

}