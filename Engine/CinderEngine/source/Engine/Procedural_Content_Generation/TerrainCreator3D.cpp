/*****************************************************************************/
/*!
\file   TerrainCreator3D.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate 3d terrains (for thermo testing)
*/
/*****************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator3D::TerrainCreator3D(int w, int d, int bh, int detail, int wl, int peak) :
      MapWidth(w), MapDepth(d), BaseHeight(bh), passes(detail), waves(wl), PeakHeight(peak)
    {
      GenerateHeightMap(BaseHeight, PeakHeight);
    }

    TerrainCreator3D::~TerrainCreator3D()
    {
      delete[] HeightMapRock;
      HeightMapRock = nullptr;
    }

    void TerrainCreator3D::GenerateHeightMap(int base, int height)
    {
      HeightMapRock = new TerrainCreator*[MapDepth];
      for (int i = 0; i < MapDepth; ++i)
      {
        HeightMapRock[i] = new TerrainCreator(MapWidth, BaseHeight, passes, waves, PeakHeight);
      }

    }

    float **const TerrainCreator3D::GetMap()
    {
      float **const HeightMap = new float*[MapDepth];
      for (int i = 0; i < MapDepth; ++i)
        HeightMap[i] = HeightMapRock[i]->GetRockMap();

      return HeightMap;
    }
  } //Procedural
}  //Framework