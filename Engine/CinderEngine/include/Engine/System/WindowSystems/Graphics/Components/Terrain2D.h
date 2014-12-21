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
#include "IGraphicsObject.h"
#include "ZilchCompiledlib.h"
#include "Zilch.hpp"

namespace Framework
{
  class PolygonCollider2D;

  class Terrain2D : public IGraphicsObject
  {
  public:
    ZilchDeclareDerivedType (Terrain2D, IGraphicsObject);

  Terrain2D ();
  virtual ~Terrain2D ();
  
  virtual void Serialize (Serializer::DataNode*);
  virtual void Initialize ();
  virtual void Draw ();

  std::vector <std::pair <vec2, vec2>>& Get_Edges ();
  void Generate_Height_Points ();
  void Generate_Edges ();
  void Generate_Vertices ();
  void Generate_Buffers ();
  const float* GetTerrain();
  const float* GetWater();
  int GetWidth();
  int GetPeakHeight();
  const static string Name;

  private:
    int MapSize;
    int BaseHeight;
    int Passes;
    int Waves;
    int WaterDepth;
    bool AddCollider;
    std::string MapFile;
    glm::vec4 color;
    Procedural::TerrainCreator* tc;

    VAO* vao;
    VBO* vbo;

    std::vector <PolygonCollider2D*> edges;
    std::vector <vec2> height_points;
    std::vector <float> vertices;
    int PeakHeight;
  };
}

#endif