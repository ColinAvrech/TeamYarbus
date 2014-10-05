/******************************************************************************/
/*!
\file   CollisionRepeats.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "CollisionRepeats.h"
#include "PhysicsLibrary.h"

namespace Framework
{
  namespace Physics
  {
    bool CirclevsLine (const float rad, glm::vec2& pos, LineCollider& l)
    {
      //Make sure buildlinesegment (private memb fuct) composes the right normals
      float ldotProduct = l.GetP1DotNormal ();
      glm::vec2 normVec = l.GetNormal ();
      glm::normalize (normVec);
      float dist = ldotProduct - DotProduct (normVec, pos);

      return rad >= dist;
    }

    bool CirclevsPoint (const float rad, glm::vec2& cpos, glm::vec2& ppos)
    {
      return rad >= Distance (cpos, ppos);
    }

    bool PointvsLine (glm::vec2& pos, LineCollider& l)
    {
      float ldotProduct = l.GetP1DotNormal ();
      glm::vec2 normVec = l.GetNormal ();
      glm::normalize (normVec);
      float dist = ldotProduct - DotProduct (normVec, pos);

      return dist <= 0;
    }
  }
}