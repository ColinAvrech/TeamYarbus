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
#include "TerrainCreator.h"
#include "JSONSerializer.h"
#include "GameObject.h"

namespace Framework
{

  class Terrain2D : public Component
  {
  public:
  Terrain2D ();
  virtual ~Terrain2D ();
  
  virtual void Serialize (Serializer::DataNode*);
  virtual void Initialize ();

  void Render ();
  std::vector <std::pair <glm::vec2, glm::vec2>>& Get_Edges ();
  void Generate_Height_Points ();
  void Generate_Edges ();
  void Generate_Vertices ();
  void Generate_Buffers ();
  const static std::string Name;

  private:
    int MapSize;
    int BaseHeight;
    int Passes;
    int Waves;
    Procedural::TerrainCreator* tc;

    VAO* vao;
    VBO* vbo;
    Shader* shader;

    std::vector <std::pair <glm::vec2, glm::vec2>> edges;
    std::vector <glm::vec2> height_points;
    std::vector <float> vertices;
  };
}

#endif