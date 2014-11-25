/*****************************************************************************/
/*!
\file   Terrain.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

#include "TerrainCreator.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator::TerrainCreator(int w, int bh, int detail, int wl, int peak, int water) :
      MapWidth (w), BaseHeight (bh), passes (detail), waves (wl), PeakHeight (peak),
      WaterDepth(water)
    {
      Generate();
    }

    TerrainCreator::~TerrainCreator()
    {
      delete[] HeightMapRock;
      //delete soil and water once implemented
    }

    void TerrainCreator::Save(const char* file)
    {
      //save to file for futur loading
     
    }

    void TerrainCreator::GenerateHeightMap(float **Array, int base, int height)
    {
      *Array = new float[MapWidth];
      for (int i = 0; i < MapWidth; ++i)
        (*Array)[i] = 0.0f;

      int* WaveBuffer = new int[ 2 * waves];
      for (int i = 0; i < 2* waves; ++i)
        WaveBuffer[i] = rand() % 2;

      int *x1/*[100]*/, *x2/*[50]*/, *x4/*[25]*/, *x8/*[12]*/;
      x1 = new int[MapWidth];
      for (int i = 0; i < MapWidth; ++i)
        x1[i] = rand() % 2;
      x2 = new int[MapWidth / 2];
      for (int i = 0; i < MapWidth / 2; ++i)
        x2[i] = rand() % 2;
      x4 = new int[MapWidth / 4];
      for (int i = 0; i < MapWidth / 4; ++i)
        x4[i] = rand() % 2;
      x8 = new int[MapWidth / 8];
      for (int i = 0; i < MapWidth / 8; ++i)
        x8[i] = rand() % 2;

      for (int i = 0; i < MapWidth; ++i)
        (*Array)[i] = base + height * (0.03125f * x1[i] + 0.0625f * x2[i / 2] + 0.125f * x4[i / 4] + 0.25f * x8[i / 8] + 0.5f * WaveBuffer[(i * 2 * waves) / MapWidth]);

      for (int i = 0; i < passes; ++i)
      {
        for (int j = 0; j < MapWidth - 2; ++j)
        {
          float dH = (*Array)[j] - (*Array)[j + 1];
          (*Array)[j] -= dH / 3;
          (*Array)[j + 1] += dH / 3;
        }
      }
      delete[] x1;
      delete[] x2;
      delete[] x4;
      delete[] x8;
      delete[] WaveBuffer;
    } //function

    void TerrainCreator::Generate()
    {
      AddRock();
      AddSoil();
      AddWater();
      AddLife();
    }

    void TerrainCreator::AddSoil()
    {
      //GenerateHeightMap(HeightMapSoil, 0, WaterDepth);
    }

    void TerrainCreator::AddRock()
    {
      GenerateHeightMap(&HeightMapRock, BaseHeight, PeakHeight);
    }

    void TerrainCreator::AddWater()
    {
      if (WaterDepth > 0)
      {
        GenerateHeightMap(&HeightMapWater, 0, 1);
        SettleWater();
      }
    }

    void TerrainCreator::SettleWater()
    {
      int MaxHeight = BaseHeight + PeakHeight;

    }

    void TerrainCreator::AddLife()
    {

    }

  } //Procedural
} //Framework