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
      for (unsigned i = 0; i < MapWidth; ++i)
        (*Array)[i] = 0.0f;

      int* WaveBuffer = new int[ 2 * waves];
      for (int i = 0; i < 2* waves; ++i)
        WaveBuffer[i] = rand() % 2;

      int *x1/*[100]*/, *x2/*[50]*/, *x4/*[25]*/, *x8/*[12]*/;
      x1 = new int[MapWidth];
      for (unsigned i = 0; i < MapWidth; ++i)
        x1[i] = rand() % 2;
      x2 = new int[MapWidth / 2];
      for (unsigned i = 0; i < MapWidth / 2; ++i)
        x2[i] = rand() % 2;
      x4 = new int[MapWidth / 4];
      for (unsigned i = 0; i < MapWidth / 4; ++i)
        x4[i] = rand() % 2;
      x8 = new int[MapWidth / 8];
      for (unsigned i = 0; i < MapWidth / 8; ++i)
        x8[i] = rand() % 2;

      for (unsigned i = 0; i < MapWidth; ++i)
        (*Array)[i] = base + height * (0.03125f * x1[i] + 0.0625f * x2[i / 2] + 0.125f * x4[i / 4] + 0.25f * x8[i / 8] + 0.5f * WaveBuffer[(i * 2 * waves) / MapWidth]);

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
      delete[] WaveBuffer;
    } //function

    void TerrainCreator::Generate()
    {
      AddRock();
      AddSoil();
      //AddWater();
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
        GenerateHeightMap(&HeightMapWater, 0, WaterDepth);
        SettleWater();
      }
    }

    void TerrainCreator::SettleWater()
    {
      float * Res_Water_Map = new float[MapWidth];
      for (unsigned i = 0; i < MapWidth; ++i)
        Res_Water_Map[i] = 0.0f;
      //Iterate through water map and drop water into terrain
      for (unsigned i = 0; i < MapWidth; ++i)
      {
        //drop whole units of water into terrain and settle them at
        //the lowest possible point.
        for (; HeightMapWater[i] >= 1.f;)
        {
          Make_Rain(Res_Water_Map, i);
        }
      }
      for (unsigned i = 0; i < MapWidth; ++i)
        Res_Water_Map[i] += HeightMapWater[i];

      delete[] HeightMapWater;
      HeightMapWater = Res_Water_Map;
    }

    void TerrainCreator::FlowLeft(float *&map, unsigned int start_pos)
    {
      unsigned int it = start_pos;
      for (; map[it] == 0.0f; --it)
      {
        if (it == 0)
          return;
        if (it > 0 && HeightMapRock[it - 1] + map[it - 1] >= HeightMapRock[it])
          break;
      }
      int spread;
      unsigned l_pos;
      std::make_pair(spread, l_pos) = EvaluateSpread(map, it);
      for (unsigned i = l_pos; i <= l_pos + spread; ++i)
        map[i] += 1.f / spread;
    }

    void TerrainCreator::FlowRight(float *&map, unsigned int start_pos)
    {
      unsigned int it = start_pos;
      for (; map[it] == 0.0f; ++it)
      {
        if (it == MapWidth - 1)
          return;
        if (it < MapWidth - 1 && HeightMapRock[it + 1] + map[it + 1] >= HeightMapRock[it])
          break;
      }

      std::pair<int, unsigned> res = EvaluateSpread(map, it);
      for (unsigned i = res.second; i <= res.second + res.first + 1; ++i)
        map[i] += 1.f / res.first;
    }

    std::pair<int, unsigned> TerrainCreator::EvaluateSpread(const float *map, unsigned pos)
    {
      unsigned int left = 0, right = MapWidth - 1;
      for (unsigned i = pos; i >= 0; --i)
      {
        if (HeightMapRock[i] + map[i] > HeightMapRock[i + 1] + map[i + 1]){
          left = i + 1;
          break;
        }
      }
      for (unsigned i = pos; i < MapWidth; ++i)
      {
        if (HeightMapRock[i] + map[i] > HeightMapRock[i - 1] + map[i - 1]){
          right = i - 1;
          break;
        }
      }
      return std::make_pair(right - left + 1, left);
    }

    void TerrainCreator::Make_Rain(float *&map, unsigned int pos)
    {
      HeightMapWater[pos] -= 1.f;
      FlowDirection dir = EvaluateSlope(map, pos);

      if (dir == FLOW_LEFT)
        FlowLeft(map, pos);
      else if (dir == FLOW_RIGHT)
        FlowRight(map, pos);
      else if (dir == PEAK || dir == TROUGH || dir == WATER_BODY)
        Evaluate_Flat(map, pos);
    }

    void TerrainCreator::Evaluate_Flat(float *&map, unsigned int pos)
    {
      unsigned int left = 0, right = MapWidth - 1;
      for (unsigned i = pos; i >= 0; --i)
      {
        if (map[i] > 0.0f){
          left = i;
          break;
        }
        if (HeightMapRock[i] > HeightMapRock[i + 1]){
          left = i + 1;
          break;
        }
      }
      for (unsigned i = pos; i < MapWidth; ++i)
      {
        if (map[i] > 0.0f){
          right = i;
          break;
        }
        if (HeightMapRock[i] > HeightMapRock[i - 1]){
          right = i - 1;
          break;
        }
      }
      for (unsigned i = left; i < right; ++i)
      {
        map[i] += (1.f / (right - left));
      }
    }

    TerrainCreator::FlowDirection TerrainCreator::EvaluateSlope(const float *map, unsigned int pos)
    {
      if (map[pos] >= 1.f)
        return WATER_BODY;
      if (pos == 0 && HeightMapRock[pos] > HeightMapRock[pos + 1])
        return FLOW_RIGHT;
      if (pos == 0 && HeightMapRock[pos] <= HeightMapRock[pos + 1])
        return FLOW_LEFT;
      if (HeightMapRock[pos] >= HeightMapRock[pos - 1] && HeightMapRock[pos] >= HeightMapRock[pos + 1])
        return PEAK;
      if (HeightMapRock[pos] > HeightMapRock[pos - 1] && HeightMapRock[pos] < HeightMapRock[pos + 1])
        return FLOW_LEFT;
      if (HeightMapRock[pos] < HeightMapRock[pos - 1] && HeightMapRock[pos] > HeightMapRock[pos + 1])
        return FLOW_RIGHT;
      if (HeightMapRock [pos] < HeightMapRock [pos - 1] && HeightMapRock [pos] < HeightMapRock [pos + 1])
        return TROUGH;
      else
        return NONE;
    }
  } //Procedural
} //Framework