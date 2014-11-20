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


    BodyContact CirclevsLine(const glm::vec3& ps, const glm::vec3& pe, const CircleCollider& c, const LineCollider& l)
    {
      float ps2l = DistancePoint2Line(ps, l);
      float pe2l = DistancePoint2Line(pe, l);
      float rad = c.radius * c.gameObject->Transform->GetScale().x;
      BodyContact res;
      res.t = -1.0f;
      if ((ps2l <= 0 && pe2l >= 0) ||
        (ps2l >= 0 && pe2l <= 0))
      {
        glm::vec3 newps(ps);
        glm::vec3 newpe(pe);
        glm::vec3 norm = { l.normalVec.x, l.normalVec.y, 0.0f };
        if (ps2l > 0)
        {
          newps -= norm * rad;
          newpe -= norm * rad;
        }
        else
        {
          newps += norm * rad;
          newpe += norm * rad;
        }
        res = PointvsLine(newps, newpe, l);
      }
      return res;
    }

    //change these to return float -- penetration
    /*BodyContact CirclevsPoint(const float rad, glm::vec3& cpos, glm::vec3& ppos)
    {
      return rad >= Distance(cpos, ppos);
    }*/

    BodyContact PointvsLine(const glm::vec3& ps, const glm::vec3& pe, const LineCollider& l)
    {
      glm::vec2 ps_ = { ps.x, ps.y };
      glm::vec2 pe_ = { pe.x, pe.y };
      float nDotps = glm::dot(ps_, l.normalVec);
      glm::vec2 vVec = pe_ - ps_;
      float tVel = glm::dot(glm::vec2(vVec.x, vVec.y), l.normalVec);
      BodyContact res;
      res.t = -1.0f;
      if (tVel == 0)
        return res;

      float t = (l.p1dotNormal - nDotps) / tVel;
      std::cout << t << std::endl;
      //if (t >= 0 && t <= 1)
      //{
        res.pi.x = ps_.x + (vVec.x * res.t);
        res.pi.y = ps_.y + (vVec.y * res.t);
        if (((res.pi.x > l.p2.x && res.pi.x > l.p1.x) ||
          (res.pi.x < l.p2.x && res.pi.x < l.p1.x)) &&
          ((res.pi.y > l.p2.y && res.pi.y > l.p1.y) ||
          (res.pi.y < l.p2.y && res.pi.y < l.p1.y)))
          return res;
        res.t = t;
        return res;
      //}
     // return res;
    } //function pvsl
  } //namespace physics
}  //namespace framework