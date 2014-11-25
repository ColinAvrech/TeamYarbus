/******************************************************************************/
/*!
\file   CollisionRepeats.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "IncludeForAllCollision.h"
#include "Resolution.h"

namespace Framework
{
  namespace Physics
  {
    float DistancePoint2Line(const glm::vec3& p, const LineCollider& l);
   
    //BodyContact PointvsLine(const glm::vec3& pos, const LineCollider& l);
    
    BodyContact CirclevsLine(const glm::vec3& pos, const CircleCollider& c, const LineCollider& l);
    
    //BodyContact CirclevsPoint(const float rad, glm::vec3& cpos, glm::vec3& ppos);
  }
}
