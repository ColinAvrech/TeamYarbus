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

namespace Framework
{
  enum LAYER
  {
    DEFAULT = 0,
    PAUSE,
  };

  class Shader;
  
  class IGraphicsObject : public Component
  {
  public:
  ZilchDeclareDerivedType(IGraphicsObject, Component);
  IGraphicsObject ();
  virtual ~IGraphicsObject ();
  
  virtual void Serialize (Serializer::DataNode* data) = 0;
  virtual void Initialize () = 0;
  void Update ();
  virtual void Draw ();

  void Register ();
  void Deregister ();
  void Change_Layer (int lay);

  LAYER layer;

  protected:
    Shader* shader;

  private:

  };  
}

#endif