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
    float DistancePoint2Line(const glm::vec3& p, const LineCollider& l)
    {
      glm::vec3 vec = p - l.p1;
      glm::vec2 vec2d = { vec.x, vec.y };
      return glm::dot(vec2d, l.normalVec);
    }

    float Angle_from_Vertical(const glm::vec2& d_vec)
    {
      glm::vec2 y_axis = { 0.0f, 1.0f };

      float cos_t = glm::dot(y_axis, d_vec);

      return glm::acos(cos_t);
    }

    glm::vec2 Closest_Point_on_Seg(glm::vec2 seg_a, glm::vec2 seg_b, glm::vec2 circ_pos)
    {
      //the segment vector, seg_v (from seg_a to seg_b)
      glm::vec2 seg_v = seg_b - seg_a;
      seg_v = glm::normalize(seg_v);
      //the position of circ_pos relative to seg_a, pt_v.
      glm::vec2 pt_v = circ_pos - seg_a;

      // find the closest point to the circle's center on the segment.
      // To do this, we must project pt_v onto seg_v:
      float proj_s = glm::dot(pt_v, seg_v);

      //If |proj_v| is less than 0 or greater than |seg_v|, 
      //the closest point to circ_pos on the segment will be 
      //one of the segment's endpoints.
      if (proj_s < 0.0f)
        return seg_a;
      else if (proj_s > glm::length(seg_v))
        return seg_b;

      glm::vec2 proj_v = seg_v * proj_s;
      glm::vec2 Closest = proj_v + seg_a;
      return Closest;
    }

    BodyContact CirclevsLine(const glm::vec3& pos, const CircleCollider& c, const LineCollider& l)
    {
      BodyContact res;
      res.t = -1.0f;
      float circ_rad = c.radius;
      
      glm::vec2 circ_pos = { pos.x, pos.y };

      glm::vec2 seg_a = { l.p1.x, l.p1.y };
      glm::vec2 seg_b = { l.p2.x, l.p2.y };

      glm::vec2 Closest = Closest_Point_on_Seg(seg_a, seg_b, circ_pos);

      glm::vec2 dist_v = circ_pos - Closest;

      float dist_l = glm::length(dist_v);
      if (dist_l < circ_rad)
      {
        res.normal_angle = Angle_from_Vertical(l.normalVec);
        res.t = 1.0f;
        res.pi = glm::vec3(Closest, 0.0f);
        res.Penetration = dist_v / dist_l * (circ_rad - dist_l);
      }
      return res;
    }

    //change these to return float -- penetration
    /*BodyContact CirclevsPoint(const float rad, glm::vec3& cpos, glm::vec3& ppos)
    {
      return rad >= Distance(cpos, ppos);
    }*/

    //BodyContact PointvsLine(const glm::vec3& pos, const LineCollider& l)
    //{
    //  
    //} //function pvsl
  } //namespace physics
}  //namespace framework