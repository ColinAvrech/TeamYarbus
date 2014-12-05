/******************************************************************************/
/*!
\file   PhysicsLibrary.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PhysicsLibrary.h"
#include "RigidBody.h"
#include "GraphicsCommon.h"

namespace Framework
{
  namespace Physics
  {

    float DetermineRestitution(GameObject* a, GameObject* b)
    {
      //return std::min(a->RigidBody->Restitution, b->Restitution);
      return 0;
    }

    float DetermineFriction(GameObject* a, GameObject* b)
    {
      //return sqrt(a->Friction * b->Friction);
      return 0;
    }

    glm::vec3 applyNetForce(const glm::vec3& sumForce, double InvMass)
    {
      glm::vec3 accel = sumForce * (float)InvMass;
      return accel;
    }

    glm::vec3 getReflection(const glm::vec3& normal, const glm::vec3& velocity)
    {
      glm::vec3 reflect;
      float dP = glm::dot(velocity, normal);
      reflect = velocity - (2.0f * dP * normal);
      return reflect;
    }

    float Distance(const glm::vec3& pos1, const glm::vec3& pos2)
    {
      return float(sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x)
        + (pos2.y - pos1.y) * (pos2.y - pos1.y)));
    }

    float DistanceSquared(const glm::vec3& pos1, const glm::vec3& pos2)
    {
      return float((pos2.x - pos1.x) * (pos2.x - pos1.x)
        + (pos2.y - pos1.y) * (pos2.y - pos1.y));
    }

    glm::vec2 Normal(const glm::vec3& p1, const glm::vec3& p2)
    {
      glm::vec2 norm;
      glm::vec3 line = p2 - p1;
      norm.x = -line.y;
      norm.y = line.x;
      norm = glm::normalize(norm);

      return norm;
    }

    glm::vec3 Normal(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
    {
      glm::vec3 edge_A = v2 - v1;
      glm::vec3 edge_B = v3 - v1;
      glm::vec3 x_product = glm::cross(edge_B, edge_A);
      return x_product;
    }

    float DotProduct(const glm::vec3& u, const glm::vec3& v)
    {
      float result = float((u.x * v.x) + (u.y * v.y));
      return result;
    }

    glm::vec3 applyAccel(const glm::vec3& accel, double dt)
    {
      glm::vec3 velChange = accel * (float)dt;
      return velChange;
    }

    glm::vec3 getAccel(const glm::vec3& force, double mass)
    {
      glm::vec3 accel = force / (float)mass;
      return accel;
    }

    float Angle_from_Vertical(const glm::vec2& d_vec)
    {
      glm::vec2 y_axis = { 0.0f, 1.0f };

      float cos_t = glm::dot(y_axis, d_vec);

      return glm::acos(cos_t);
    }
  }
}
