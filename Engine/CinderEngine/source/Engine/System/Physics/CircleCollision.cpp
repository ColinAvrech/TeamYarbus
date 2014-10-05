#include "ColliderShape.h"
#include "Collision.h"
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "ObjectSystem.h"

namespace Framework
{
  namespace Physics
  {
    void Circle::DetectCircle(Circle* c)
    {
      //// not counting offset
      //Vec2 pos;
      //pos.x_ = static_cast<Transform*>(Base->Transform)->position.x;
      //pos.y_ = static_cast<Transform*>(Base->Transform)->position.y;
      //Vec2 cpos;
      //cpos.x_ = static_cast<Transform*>(c->Base->Transform)->position.x;
      //cpos.y_ = static_cast<Transform*>(c->Base->Transform)->position.y;
      //float rad = GetRadius() + c->GetRadius();
      //float dist = Distance(pos, cpos);
      //if (rad >= dist)
      //{
      //  //std::string ColEvent = std::string("COLLISION");
      //  //CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
      //  //collision->OtherObject = c->Base;
      //  //collision->normal = pos - cpos;
      //  //collision->normal.normalize();
      //  //collision->DispatchEvent();
      //  std::cout << "COLLISION" << std::endl;
      //}
    }

    //repeat
    void Circle::DetectPoint(Point p)
    {
      Vec2 ppos = p.getPosition();
      Vec2 pos = getPosition();
      float rad = GetRadius();
      if (CirclevsPoint(rad, pos, ppos))
      {
        std::string ColEvent = std::string("COLLISION");
        CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
        collision->OtherObject = p.Base;
        collision->normal = pos - ppos;
        collision->normal.normalize();
        collision->DispatchEvent();
      }
    }

    void Circle::DetectLine(LineSegment l)
    {
      float rad = GetRadius();
      Vec2 pos = getPosition();
      if (CirclevsLine(rad, pos, l))
      {
        std::string ColEvent = std::string("COLLISION");
        CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
        collision->OtherObject = l.Base;
        collision->normal = l.GetNormal();
        collision->normal.normalize();
        collision->DispatchEvent();
      }
    }
  } //Physics
} //Framework
