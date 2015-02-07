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

#include "MetaCreator.h"

namespace Framework
{
  class PolygonCollider2D;

  class Terrain2D : public IGraphicsObject
  {
  public:
	  META_DECLARE( Terrain2D );

    ZilchDeclareDerivedType (Terrain2D, IGraphicsObject);

  Terrain2D ();
  virtual ~Terrain2D ();
  
  virtual void Serialize (Serializer::DataNode*);
  virtual void Initialize ();
  virtual void Draw ();
  virtual bool Draw_Last() { return AddCollider; }
  virtual bool InViewport() { return true; }

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
    glm::vec2 Slope;
    int WaterDepth = 0;
    bool AddCollider = false;
    std::string MapFile;
    glm::vec4 color1;
    glm::vec4 color2;
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