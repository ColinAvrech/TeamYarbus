/*****************************************************************************/
/*!
\file   Terrain.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

//This is only for testing thermo
#pragma once
#include <utility>

namespace Framework
{
  namespace Procedural
  {
    class TerrainCreator
    {
    public:
      TerrainCreator (){}
      //Constructor
      //Takes map width, map height, base height, smoothing passes, number of waves
      TerrainCreator(int width, int baseHeight, int passes = 3, int waves = 2, int peak = 100, int water = 50);
      //Destructor
      ~TerrainCreator();
      float * const GetRockMap() { return HeightMapRock;  }
      float * const GetSoilMapl(){ return HeightMapSoil;  }
      float * const GetWaterMap(){ return HeightMapWater; }

      float GetPeakHeight () const
      {
        return BaseHeight + PeakHeight;
      }

      inline int Get_Width ();

      void Generate();
      void Save(const char *file);
      void Load(const char *file);

    private:
      //Terrain settings
      int passes;
      int waves;
      int MapWidth;
      int BaseHeight; //Must be a number between 0 and height.
      float *HeightMapRock;
      float *HeightMapSoil;
      float *HeightMapWater;
      int PeakHeight;
      int WaterDepth;

      //Private Enum
      enum FlowDirection
      {
        FLOW_LEFT,
        FLOW_RIGHT,
        PEAK,
        TROUGH,
        WATER_BODY
      };

      //Private Member Functions
      //Layering
      void AddSoil();
      void AddRock();
      void AddWater();

      //Helper functions
      void GenerateHeightMap(float **Array, int base, int height);
      void SettleWater();
      FlowDirection EvaluateSlope(const float *map, unsigned int pos);
      std::pair<int, unsigned> EvaluateSpread(const float *map, unsigned int pos);
      void Evaluate_Flat(float *&map, unsigned int pos);
      void FlowLeft(float *&map, unsigned int start_pos);
      void FlowRight(float *&map, unsigned int start_pos);
      void Make_Rain(float *&map, unsigned int pos);
    }; //class terrain

    inline int TerrainCreator::Get_Width ()
    {
      return MapWidth;
    }

  } //Procedural
} //Framework

