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
      HeightMapRock.clean();
    }

    void TerrainCreator3D::GenerateHeightMap(int base, int height)
    {
      HeightMapRock.allocate(MapWidth, MapDepth);
      float weight = 1.f;
      for (int i = 0; i < passes; ++i, weight *= 0.25f);
      
      for (unsigned int i = 0; i < MapWidth; ++i)
      {
        for (unsigned int j = 0; j < MapDepth; ++j)
        {
          HeightMapRock.Set(i, j, weight * (rand() % PeakHeight));
        } //for j
      } // for i

      int factor = 1;
      for (int i = 0; i < passes; ++i, factor *= 4);

      for (int p = 0; p < passes && factor > 0; ++p)
      {
        weight *= 6.5f;
        factor *= 0.25f;
        for (unsigned int i = 0; i < MapWidth; ++i)
        {
          for (unsigned int j = 0; j < MapDepth; ++j)
          {
            float value = HeightMapRock.Get(i, j);
            value += weight * HeightMapRock.Get(i / factor, j / factor);
            HeightMapRock.Set(i, j, value);
          } //for j
        } // for i
      } // for p
    }

    float *const TerrainCreator3D::GetMap()
    {
      return HeightMapRock.GetArray();
    }
  } //Procedural
}  //Framework