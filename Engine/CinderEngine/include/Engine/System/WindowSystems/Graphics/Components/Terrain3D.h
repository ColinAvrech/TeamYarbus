/*****************************************************************************/
/*!
\file   Terrain3D.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

#ifndef _TERRAIN_3D_H
#define _TERRAIN_3D_H

#include "GraphicsCommon.h"
#include "TerrainCreator3D.h"
#include "JSONSerializer.h"
#include "IGraphicsObject.h"

namespace Framework
{
  class Terrain3D : public IGraphicsObject
  {
  public:
    Terrain3D();
    virtual ~Terrain3D();

    virtual void Serialize(Serializer::DataNode*);
    virtual void Initialize();
    virtual void Draw();
    virtual bool Draw_Last() { return false; }

    void Generate_Height_Points();
    void Generate_Vertices();
    void Generate_Buffers();
    const float** GetTerrain();
    int GetWidth();
    int GetDepth();
    const static string Name;

  private:
    int MapSize;
    int MapDepth;
    int BaseHeight;
    int PeakHeight;
    int Passes;
    int Waves;
    glm::vec4 color1;
    glm::vec4 color2;
    Procedural::TerrainCreator3D* tc;

    VAO* vao;
    VBO* vbo;

    std::vector<vec3> height_points;
    std::vector<float> vertices;
  };
}

#endif