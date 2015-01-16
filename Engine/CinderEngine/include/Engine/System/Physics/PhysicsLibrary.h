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
    float Distance(const vec3& pos1, const vec3& pos2);
	float DistanceSquared(const vec3& pos1, const vec3& pos2);
    float DotProduct(const vec3& u, const vec3& v);
	vec3 applyNetForce(const vec3& sumForce, double InvMass);
    vec3 applyAccel(const vec3& accel, double dt);
    vec2 Normal(const vec3& p1, const vec3& p2);
    vec3 getAccel(const vec3& force, double mass);
    vec3 getReflection(const vec3& normal, const vec3& velocity);
    vec3 Normal(const vec3& v1, const vec3& v2, const vec3& v3);
    float Angle_from_Vertical(const glm::vec2& d_vec);
	
    //vec3 applyConstVelocity(GameObject * go, vec3 initial);

  } //Physics
} //Framework
