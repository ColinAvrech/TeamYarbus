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
      //Calculate starting weight
      for (auto i = 0; i < passes; ++i, weight *= 0.25f);
      //Generate initial noise map
      for (auto i = 0; i < MapWidth; ++i)
      {
        for (auto j = 0; j < MapDepth; ++j)
        {
          if (i == 0 || i == MapWidth - 1 || j == 0 || j == MapDepth - 1)
            HeightMapRock.Set(i, j, 0.0f);
          else
            HeightMapRock.Set(i, j, weight * (rand() % PeakHeight));
        } //for j
      } // for i

      int factor = 1;
      //Calculate index size
      for (auto i = 0; i < passes; ++i, factor *= 2);
      //Make multiple passes
      for (auto p = 0; p < passes && factor > 0; ++p)
      {
        weight *= 6.f;
        factor *= 0.5f;
        for (auto i = factor; i < MapWidth; ++i)
        {
          for (auto j = factor; j < MapDepth; ++j)
          {
            if (i != 0 && i != MapWidth - 1 && j != 0 && j != MapDepth - 1)
            {
              float value = HeightMapRock.Get(i, j);
              value += weight * HeightMapRock.Get(i / factor, j / factor);
              HeightMapRock.Set(i, j, value);
            }
          } //for j
        } // for i
      } // for p

      for (auto p = 0; p < passes; ++p)
      {
        for (auto i = 0; i < MapWidth - 1; ++i)
        {
          for (auto j = 0; j < MapDepth - 1; ++j)
          {
            float value_o = HeightMapRock.Get(i, j);
            float value_r = HeightMapRock.Get(i + 1, j);
            float value_d = HeightMapRock.Get(i, j + 1);
            float diff_r = value_r - value_o;
            float diff_d = value_d - value_o;
            //value += weight * HeightMapRock.Get(i / factor, j / factor);
            HeightMapRock.Set(i, j, value_o + diff_r / 3.f + diff_d / 3.f);
            HeightMapRock.Set(i + 1, j, value_r - diff_r / 6.f);
            HeightMapRock.Set(i, j + 1, value_d - diff_d / 6.f);
          } //for j
        } // for i
      } //for p
    }

    float *const TerrainCreator3D::GetMap()
    {
      return HeightMapRock.GetArray();
    }

    void TerrainCreator3D::DrawTexture()
    {
      float dx = 1.f / MapWidth;
      float dy = 1.f / MapDepth;
      glBegin(GL_QUADS);
      {
        for (int i = 0; i < MapDepth; ++i)
        {
          for (int j = 0; j < MapWidth; ++j)
          {
            float h = HeightMapRock.Get(j, i) / PeakHeight;
            glColor4f(h, h, h, 1.f);
            glVertex2f(j * dx - 0.5 * MapWidth * dx, i * dy - 0.5 * MapDepth * dy);
            glVertex2f((j + 1) * dx - 0.5 * MapWidth * dx, i * dy - 0.5 * MapDepth * dy);
            glVertex2f((j + 1) * dx - 0.5 * MapWidth * dx, (i + 1) * dy - 0.5 * MapDepth * dy);
            glVertex2f(j * dx - 0.5 * MapWidth * dx, (i + 1) * dy - 0.5 * MapDepth * dy);
          }
        }
      }
      glEnd();
    }
  } //Procedural
}  //Framework