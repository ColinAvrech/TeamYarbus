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
	class CircleCollider;
	class LineCollider;
	class ShapeCollider : public Component
	{
	public:
    const static std::string Name;
	void SerializeMaterial(const char* name);
	int numOfShapes;
		// Getters
		glm::vec3 getPosition(void){
			return position;
		}
		glm::vec3 getOffset(void){
			return offset;
		}
		float getDensity(void)
		{
			return material.density;
		}

		virtual void DetectCollision(CircleCollider * circle) = 0;
		virtual void DetectCollision(LineCollider * line) = 0;

	private:
		// Private data
		struct MaterialProperties
		{
			std::string name;
			float density;
			float restitution;
			float staticfriction;
			float dynamicfriction;
		};
		MaterialProperties material;
		glm::vec3 position;
		glm::vec3 offset;
		bool ghost;
		bool sendsEvents;
	};
} //Framework

