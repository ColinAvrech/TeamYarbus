/******************************************************************************/
/*!
\file   Manifold.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "GameObject.h"
#include "Manifold.h"
#include "Collision.h"
#include "MathExtensionLibrary.h"

namespace Framework
{
  void Manifold::Solve(void)
  {
    ShapeCollider2D* shapeA = A->gameObject->C<ShapeCollider2D>();
    ShapeCollider2D* shapeB = B->gameObject->C<ShapeCollider2D>();
    if (shapeA && shapeB)
      Dispatch[shapeA->GetType()][shapeB->GetType()](this, shapeA, shapeB);
  }

  void Manifold::Initialize(void)
  {
    // Calculate average restitution
    e = (std::min)(A->mat->restitution, B->mat->restitution);

    // Calculate static and dynamic friction
    sf = std::sqrt(A->mat->staticFriction * A->mat->staticFriction);
    df = std::sqrt(A->mat->dynamicFriction * A->mat->dynamicFriction);

    for (unsigned i = 0; i < contact_count; ++i)
    {
      // Calculate radii from Center Of Model to contact
      vec2 ra = contacts[i] - A->gameObject->C<ShapeCollider2D>()->GetCenter();
      vec2 rb = contacts[i] - B->gameObject->C<ShapeCollider2D>()->GetCenter();

      vec2 rv = B->velocity + Cross(B->angularVelocity, rb) -
        A->velocity - Cross(A->angularVelocity, ra);


      // Determine if we should perform a resting collision or not
      // The idea is if the only thing moving this object is gravity,
      // then the collision should be performed without any restitution
      if (glm::length2(rv) < glm::length2(dt * gravity) + EPSILON)
        e = 0.0f;
    }
  }

  void Manifold::ApplyImpulse(void)
  {
    // Early out and positional correct if both objects have infinite mass
    if (Equal(A->invMass + B->invMass, 0))
    {
      InfiniteMassCorrection();
      return;
    }

    for (unsigned i = 0; i < contact_count; ++i)
    {
      // Calculate radii from COM to contact
      vec2 ra = contacts[i] - A->gameObject->C<ShapeCollider2D>()->GetCenter();
      vec2 rb = contacts[i] - B->gameObject->C<ShapeCollider2D>()->GetCenter();

      // Relative velocity
      vec2 rv = B->velocity + Cross(B->angularVelocity, rb) -
        A->velocity - Cross(A->angularVelocity, ra);

      // Relative velocity along the normal
      float contactVel = glm::dot(rv, normal);

      // Do not resolve if velocities are separating
      if (contactVel > 0)
        return;

      float raCrossN = Cross(ra, normal);
      float rbCrossN = Cross(rb, normal);
      float invMassSum = A->invMass + B->invMass + Sqr(raCrossN) * A->invI + Sqr(rbCrossN) * B->invI;

      // Calculate impulse scalar
      float j = -(1.0f + e) * contactVel;
      j /= invMassSum;
      j /= (float)contact_count;

      // ***** SUPER BUG AHEAD
      //if ((OBJECTSYSTEM->ptrPlayer != nullptr) && (OBJECTSYSTEM->ptrPlayer->RigidBody2D->velocity.Len()) > 1000)
      //  __debugbreak();
      //

      // Apply impulse         !!  SUPER BUG IS HERE  !
      vec2 impulse = normal * j;
      A->ApplyImpulse(-impulse, ra);
      B->ApplyImpulse(impulse, rb);

      // ***** SUPER BUG ABOVE
      //if ((OBJECTSYSTEM->ptrPlayer != nullptr) && (OBJECTSYSTEM->ptrPlayer->RigidBody2D->velocity.Len()) > 1000)
      //  __debugbreak();
      //

      // Friction impulse
      rv = B->velocity + Cross(B->angularVelocity, rb) -
        A->velocity - Cross(A->angularVelocity, ra);

      vec2 t = glm::normalize(rv - (normal * glm::dot(rv, normal)));

      // j tangent magnitude
      float jt = -glm::dot(rv, t);
      jt /= invMassSum;
      jt /= (float)contact_count;

      // Don't apply tiny friction impulses
      if (Equal(jt, 0.0f))
        return;

      // Coulumb's law
      vec2 tangentImpulse;
      if (std::abs(jt) < j * sf)
        tangentImpulse = t * jt;
      else
        tangentImpulse = t * -j * df;

      // Apply friction impulse
      A->ApplyImpulse(-tangentImpulse, ra);
      B->ApplyImpulse(tangentImpulse, rb);
    }
  }

  void Manifold::PositionalCorrection(void)
  {
    const float k_slop = 0.05f; // Penetration allowance
    const float percent = 0.4f; // Penetration percentage to correct
    vec2 correction = ((std::max)(penetration - k_slop, 0.0f) / (A->invMass + B->invMass)) * normal * percent;
    if (A->gameObject != nullptr)
    {
      Transform* tformA = A->gameObject->C<Transform>();
      tformA->Translate2D(-correction * A->invMass);
    }
    if (B->gameObject != nullptr)
    {
      Transform* tformB = B->gameObject->C<Transform>();
      tformB->Translate2D(correction * B->invMass);
    }
  }

  void Manifold::InfiniteMassCorrection(void)
  {
    A->velocity = vec2();
    B->velocity = vec2();
    if (!A->gameObject->C<RigidBody2D>()->isStatic)
      A->gameObject->C<Transform>()->SetPosition(vec2());
    if (!B->gameObject->C<RigidBody2D>()->isStatic)
      B->gameObject->C<Transform>()->SetPosition(vec2());
  }
}

