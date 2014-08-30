/*!

/auther Micah
/file Transform.h

/brief The transform component holds object's position, and rotation.

*/

#pragma once

#include "Component.h"
#include "GOC.h"
namespace Framework
{
  class Transform : public GameComponent
  {
    Transform(GOC* obj)
      :Position(obj->GOCPosition), Rotation(0){}
    double Rotation;
    Vec2& Position;
    //! Add serialization overload
  };
}