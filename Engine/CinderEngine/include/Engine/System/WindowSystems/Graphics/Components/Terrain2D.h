/******************************************************************************/
/*!
\file   Terrain2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TERRAIN_2D_H
#define _TERRAIN_2D_H

#include "GraphicsCommon.h"
#include "JSONSerializer.h"
#include "GameObject.h"

namespace Framework
{
  class UpdateEvent;
  class Terrain2D : public Component
  {
  public:
  Terrain2D ();
  ~Terrain2D ();
  
  virtual void Serialize (Serializer::DataNode*);
  virtual void Initialize ();

  void Render ();

  const static std::string Name;

  private:
    VAO* vao;
    VBO* vbo;
    Shader* shader;

    std::vector <glm::vec2> height_points;
    std::vector <float> vertices;
  };  
}

#endif