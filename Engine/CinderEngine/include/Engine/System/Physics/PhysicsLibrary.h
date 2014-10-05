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

namespace Framework
{
  namespace Physics
  {
    float Distance (const glm::vec2& pos1, const glm::vec2& pos2);
    float DotProduct (const glm::vec2& u, const glm::vec2& v);
    glm::vec2 applyAccel (glm::vec2 accel, double dt);
    glm::vec2 getAccel (glm::vec2& force, double mass);
    glm::vec2 getReflection (glm::vec2& normal, glm::vec2& velocity);
  } //Physics
} //Framework