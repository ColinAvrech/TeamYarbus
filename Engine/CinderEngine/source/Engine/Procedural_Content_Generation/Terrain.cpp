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
#include "FractalNoise.h"
#include <stdlib.h>
#include <iostream>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator::TerrainCreator(int w, int h, int bh, int detail, int wl) :
      MapWidth(w), MapHeight(h), BaseHeight(bh), passes(detail), waves(wl)
    {
      Map = new int*[MapWidth];
      for (int i = 0; i < MapWidth; ++i)
        Map[i] = new int[MapHeight];

      for (int j = 0; j < MapWidth; ++j)
        for (int i = 0; i < MapHeight; ++i)
          Map[j][i] = 0;

      FractalNoise2D fractal(MapWidth, passes);

      HeightMap = fractal.GetArray();

      for (int i = 0; i < MapWidth; ++i)
        for (int j = 0; j < BaseHeight + (MapHeight / 2) * HeightMap[i]; ++j)
          Map[i][j] = 1;

      for (int i = 0; i < MapWidth; ++i)
      {
        for (int j = 0; j < MapHeight; ++j){
          if (Map[i][j] != 0)
            std::cout << Map[i][j];
        }
        std::cout << std::endl;
      }
    }

    TerrainCreator::~TerrainCreator()
    {
      for (int i = 0; i < MapWidth; ++i)
        delete[] Map[i];
      delete[] Map;
    }
  } //Procedural
} //Framework