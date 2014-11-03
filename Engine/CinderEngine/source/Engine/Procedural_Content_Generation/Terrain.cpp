/*****************************************************************************/
/*!
\file   Terrain.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

//This is only for testing thermo

#include "Terrain.h"
#include <stdlib.h>
#include <iostream>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator::TerrainCreator(int w, int h, int bh, int detail, int wl) :
      MapWidth(w), MapHeight(h), BaseHeight(bh), passes(detail), waves(wl)
    {
      HeightMap = new float[MapWidth];
      Map = new int*[MapWidth];
      for (int i = 0; i < MapWidth; ++i)
        Map[i] = new int[MapHeight];

      for (int j = 0; j < MapWidth; ++j)
        for (int i = 0; i < MapHeight; ++i)
          Map[j][i] = 0;

      GenerateHeightMap();
      //for (int i = 0; i < MapWidth; ++i)
      //  std::cout << (int)HeightMap[i] << " ";

      for (int i = 0; i < MapWidth; ++i)
        for (int j = 0; j < (int)HeightMap[i]; ++j)
          Map[i][j] = 1;

      for (int i = MapWidth - 1; i >= 0; --i)
      {
        for (int j = 0; j < MapHeight; ++j){
          if (Map[i][j] != 0)
            std::cout << Map[i][j];
        }
        std::cout << std::endl;
      }
      delete[] HeightMap;
    }

    TerrainCreator::~TerrainCreator()
    {
      for (int i = 0; i < MapWidth; ++i)
        delete[] Map[i];
      delete[] Map;
    }

    void TerrainCreator::GenerateHeightMap()
    {
      for (int i = 0; i < MapWidth; ++i)
        HeightMap[i] = 0.0f;
      //float factor = 0.5f;
      //int frequency = MapWidth / (waves + 1);
      //for (int i = 0; i < passes; ++i, frequency /= 2, factor /= 2)
      //{
      //  for (int j = 0; j < MapWidth; ++j)
      //  {
      //    int r = rand() % 2;
      //    for (int k = 0; k < frequency && j < MapWidth; ++k, ++j)
      //    {
      //      HeightMap[j + k] += factor * r;
      //    }
      //  } //for
      //} //for
      int x1[100], x2[50], x4[25], x8[12];
      for (int i = 0; i < 100; ++i)
        x1[i] = rand() % 2;
      for (int i = 0; i < 50; ++i)
        x2[i] = rand() % 2;
      for (int i = 0; i < 25; ++i)
        x4[i] = rand() % 2;
      for (int i = 0; i < 13; ++i)
        x8[i] = rand() % 2;

      for (int i = 0; i < 100; ++i)
        HeightMap[i] = BaseHeight + 50 * (0.0625 * x1[i] + 0.125 * x2[i / 2] + 0.2 * x4[i / 4] + 0.25 * x8[i / 8] - 0.5);
      for (int i = 0; i < 4; ++i)
      {
        for (int j = 0; j < MapWidth - 1; ++j)
        {
          int dH = HeightMap[j] - HeightMap[j + 1];
          HeightMap[j] -= dH / 3;
          HeightMap[j + 1] += dH / 3;
        } //for
      } //for
    } //function

    void TerrainCreator::ApplyHeightMap()
    {

    }
  } //Procedural
} //Framework