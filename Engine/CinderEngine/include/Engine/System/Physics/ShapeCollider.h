/******************************************************************************/
/*!
\file   ShapeCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Common.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "PhysicsLibrary.h"
#include "GraphicsCommon.h"
#include "Events.h"

namespace Framework
{
	class ShapeCollider : public Component
	{
	public:
    const static std::string Name;
	int numOfShapes;
		// Getters
		glm::vec2 getPosition(void){
			return position;
		}
		glm::vec2 getOffset(void){
			return offset;
		}

	private:
		// Private data
		struct MaterialProperties
		{
			std::string name;
			float density;
			float restitution;
			float friction;
		};
		glm::vec2 position;
		glm::vec2 offset;
		bool ghost;
		bool sendsEvents;
	};
} //Framework

