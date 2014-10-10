/******************************************************************************/
/*!
\file   PhysicsLibrary.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "type_vec.hpp"
#include "glmOverloads.h"
//#include "GameObject.h"

namespace Framework
{
	namespace Physics
	{
		float Distance(const glm::vec2& pos1, const glm::vec2& pos2);
		float DotProduct(const glm::vec2& u, const glm::vec2& v);
		glm::vec2 getNormal(glm::vec2 p1, glm::vec2 p2);
		glm::vec2 applyAccel(glm::vec2 accel, double dt);
		glm::vec2 getAccel(glm::vec2 force, double mass);
		glm::vec2 getReflection(glm::vec2& normal, glm::vec2& velocity);
		//glm::vec2 applyConstVelocity(GameObject * go, glm::vec2 initial);

	} //Physics
} //Framework
