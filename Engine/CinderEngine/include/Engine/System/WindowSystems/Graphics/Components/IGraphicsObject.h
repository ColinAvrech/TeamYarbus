/******************************************************************************/
/*!
\file   IGraphicsObject.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _I_GRAPHICS_OBJ_H
#define _I_GRAPHICS_OBJ_H

#include "Component.h"
#include "JSONSerializer.h"

namespace Framework
{

  class IGraphicsObject : public Component
  {
  public:
  IGraphicsObject ();
  virtual ~IGraphicsObject ();
  
  virtual void Serialize (Serializer::DataNode* data) = 0;
  virtual void Initialize () = 0;
  virtual void Draw ();

  void Register ();
  void Deregister ();
  private:

  };  
}

#endif