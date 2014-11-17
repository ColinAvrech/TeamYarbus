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

#include "TerrainCreator.h"
#include "ThermoDynamics.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator::TerrainCreator(int w, int h, int bh, int detail, int wl, int peak, int water) :
      MapWidth (w), MapHeight (h), BaseHeight (bh), passes (detail), waves (wl), PeakHeight (peak),
      WaterDepth(water)
    {
      HeightMap = new float[MapWidth];
      Map = new int*[MapWidth];
      for (int i = 0; i < MapWidth; ++i)
        Map[i] = new int[MapHeight];

      for (int j = 0; j < MapWidth; ++j)
        for (int i = 0; i < MapHeight; ++i)
          Map[j][i] = Physics::AIR;

      Generate();
    }

    TerrainCreator::~TerrainCreator()
    {
      for (int i = 0; i < MapWidth; ++i)
        delete[] Map[i];
      delete[] Map;
      delete[] HeightMap;
    }

    void TerrainCreator::Save(const char* file)
    {
      //save
      std::ofstream terrainsave(file);
      for (int i = 0; i < MapWidth; ++i)
      {
        for (int j = 0; j < MapHeight; ++j){
          terrainsave << Map[i][j] << " ";
        }
        terrainsave << std::endl;
      }
    }

    void TerrainCreator::GenerateHeightMap()
    {
      for (int i = 0; i < MapWidth; ++i)
        HeightMap[i] = 0.0f;

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
        HeightMap[i] = BaseHeight + (PeakHeight) * (0.0625f * x1[i] + 0.125f * x2[i / 2] + 0.25f * x4[i / 4] + 0.5f * x8[i / 8] + 0.5f * WaveBuffer[(i * 2 * waves) / MapWidth] - 0.5f);

      for (int i = 0; i < passes; ++i)
      {
        for (int j = 0; j < MapWidth - 2; ++j)
        {
          float dH = HeightMap[j] - HeightMap[j + 1];
          HeightMap[j] -= dH / 3;
          HeightMap[j + 1] += dH / 3;
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

    void TerrainCreator::ApplyHeightMap()
    {
      for (int i = 0; i < MapWidth; ++i){
        int j = 0;
        for (; j < MapHeight && Map[i][j] != Physics::AIR; ++j);
        for (int k = 0; k < (int)HeightMap[i]; ++k)
          Map[i][j+k] = Physics::SOIL;
      }
    }

    void TerrainCreator::AddSoil()
    {
      GenerateHeightMap();
      ApplyHeightMap();
    }

    void TerrainCreator::AddRock()
    {
      GenerateHeightMap();
      ApplyHeightMap();
    }

    void TerrainCreator::AddWater()
    {

    }

    void TerrainCreator::AddLife()
    {

    }

  } //Procedural
} //Framework