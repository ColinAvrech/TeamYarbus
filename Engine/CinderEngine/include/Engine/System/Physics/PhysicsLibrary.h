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
    float Distance(vec3& pos1, vec3& pos2);
	float DistanceSquared(vec3& pos1, vec3& pos2);
    float DotProduct(vec3& u, vec3& v);
	vec3 applyNetForce(vec3 sumForce, double InvMass);
    vec3 applyAccel(vec3 accel, double dt);
    vec3 getNormal(vec3 p1, vec3 p2);
    vec3 getAccel(vec3 force, double mass);
    vec3 getReflection(vec3& normal, vec3& velocity);
    //vec3 applyConstVelocity(GameObject * go, vec3 initial);

  } //Physics
} //Framework
