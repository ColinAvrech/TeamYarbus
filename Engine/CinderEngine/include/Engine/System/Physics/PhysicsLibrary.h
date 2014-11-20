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
    namespace Constant
    {
      //Add physics constants here
      const float g       = 9.80665f;     //m.s-1
      const long double G = 6.67259e-11;  //m3.Kg-1.s-2

    } //namespace Constant
	//float DetermineRestitution(GameObject* a, GameObject* b);
	//float DetermineFriction(GameObject* a, GameObject* b);
    float Distance(glm::vec3& pos1, glm::vec3& pos2);
	float DistanceSquared(glm::vec3& pos1, glm::vec3& pos2);
    float DotProduct(glm::vec3& u, glm::vec3& v);
	glm::vec3 applyNetForce(glm::vec3 sumForce, double InvMass);
    glm::vec3 applyAccel(glm::vec3 accel, double dt);
    glm::vec3 getNormal(glm::vec3 p1, glm::vec3 p2);
    glm::vec3 getAccel(glm::vec3 force, double mass);
    glm::vec3 getReflection(glm::vec3& normal, glm::vec3& velocity);
    //glm::vec3 applyConstVelocity(GameObject * go, glm::vec3 initial);

  } //Physics
} //Framework
