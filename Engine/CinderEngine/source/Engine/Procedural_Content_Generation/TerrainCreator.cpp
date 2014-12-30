/*****************************************************************************/
/*!
\file   Terrain.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

#include <Precompiled.h>
#include "TerrainCreator.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

namespace Framework
{
  namespace Procedural
  {
    TerrainCreator::TerrainCreator(int _width, int _baseHeight, int _passes,
      int _waves, int _peak, int _water, const std::string& _HMap) :
      MapWidth (_width), BaseHeight (_baseHeight), passes (_passes), waves (_waves), PeakHeight (_peak),
      WaterDepth(_water)
    {
      Generate(_HMap);
    }

    TerrainCreator::~TerrainCreator()
    {
      delete[] HeightMapRock;
      HeightMapRock = nullptr;
      //delete soil and water once implemented
    }

    void TerrainCreator::Save(const char* file)
    {
      //save to file for futur loading
     
    }

    void TerrainCreator::GenerateHeightMap(float **Array, int base, int height, const std::string& _File)
    {
      int* WaveBuffer;
      bool success = false;
      if (_File.size() > 0)
      {
        success = ReadFile(&WaveBuffer, _File);
      }
      if (!success)
      {
        WaveBuffer = new int[2 * waves];
        for (int i = 0; i < 2 * waves; ++i)
          WaveBuffer[i] = rand() % height;
      }
      
      *Array = new float[MapWidth];
      for (unsigned i = 0; i < MapWidth; ++i)
        (*Array)[i] = 0.0f;

      int *x1/*[100]*/, *x2/*[50]*/, *x4/*[25]*/, *x8/*[12]*/;
      x1 = new int[MapWidth];
      for (unsigned i = 0; i < MapWidth; ++i)
        x1[i] = rand() % height;
      x2 = new int[MapWidth / 2];
      for (unsigned i = 0; i < MapWidth / 2; ++i)
        x2[i] = rand() % height;
      x4 = new int[MapWidth / 4];
      for (unsigned i = 0; i < MapWidth / 4; ++i)
        x4[i] = rand() % height;
      x8 = new int[MapWidth / 8];
      for (unsigned i = 0; i < MapWidth / 8; ++i)
        x8[i] = rand() % height;

      for (unsigned i = 0; i < MapWidth; ++i)
        (*Array)[i] = (0.03125f * x1[i] + 0.0625f * x2[i / 2] + 0.125f * x4[i / 4] + 0.25f * x8[i / 8] + 0.5f * WaveBuffer[(i * 2 * waves) / MapWidth]);

      for (int i = 0; i < passes; ++i)
      {
        for (unsigned j = 0; j < MapWidth - 2; ++j)
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
      x1 = nullptr;
      x2 = nullptr;
      x4 = nullptr;
      x8 = nullptr;
      //delete[] WaveBuffer;
    } //function

    void TerrainCreator::Generate(const std::string& MapFile)
    {
      AddRock(MapFile);
      AddSoil();
      AddWater();
    }

    void TerrainCreator::AddSoil()
    {
      //GenerateHeightMap(HeightMapSoil, 0, WaterDepth);
    }

    void TerrainCreator::AddRock(const std::string& _File)
    {
      GenerateHeightMap(&HeightMapRock, BaseHeight, PeakHeight, _File);
    }

    void TerrainCreator::AddWater()
    {
      if (WaterDepth > 0)
      {
        GenerateHeightMap(&HeightMapWater, 0, WaterDepth);
        //SettleWater();
      }
    }

    bool TerrainCreator::ReadFile(int **Buffer, const std::string& _File)
    {
      std::string path("..//..//Resources//Levels//");
      std::string extension(".terrain");
      path.append(_File);
      path.append(extension);
      std::ifstream t(path);
      if (!t.is_open())
        return false;
      //If file was successfully opened
      PeakHeight = 0;
      t >> waves;
      *Buffer = new int[2 * waves];
      for (int i = 0; i < 2 * waves; ++i)
      {
        t >> (*Buffer)[i];
        if ((*Buffer)[i] > PeakHeight)
          PeakHeight = (*Buffer)[i];
      }
      return true;
    }

  } //Procedural
} //Framework