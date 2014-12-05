/*****************************************************************************/
/*!
\file   TerrainCreator.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

//This is only for testing thermo
#pragma once
#include <utility>
#include <string>

namespace Framework
{
  namespace Procedural
  {
    class TerrainCreator
    {
    public:
      TerrainCreator (){}
      //Constructor
      //Takes map width, map height, base height, smoothing passes, number of waves, peak height, water depth and preset shape
      TerrainCreator(int width, int baseHeight, int passes = 3, int waves = 2, 
        int peak = 100, int water = 50, const std::string& HMap = "");
      //Destructor
      ~TerrainCreator();
      float * const GetRockMap() { return HeightMapRock;  }
      float * const GetSoilMapl(){ return HeightMapSoil;  }
      float * const GetWaterMap(){ return HeightMapWater; }

      float GetPeakHeight () const
      {
        return float(BaseHeight + PeakHeight);
      }

      inline int Get_Width ();

      void Generate(const std::string& MapFile);
      void Save(const char *file);
      void Load(const char *file);

    private:
      //Terrain settings
      int passes;
      int waves;
      unsigned MapWidth;
      int BaseHeight; //Must be a number between 0 and height.
      float *HeightMapRock;
      float *HeightMapSoil;
      float *HeightMapWater;
      int PeakHeight;
      int WaterDepth;

      //Private Enum
      enum FlowDirection
      {
        NONE,
        FLOW_LEFT,
        FLOW_RIGHT,
        PEAK,
        TROUGH,
        WATER_BODY
      };

      //Private Member Functions
      //Layering
      void AddSoil();
      void AddRock(const std::string& File);
      void AddWater();

      //Helper functions
      void GenerateHeightMap(float **Array, int base, int height, const std::string& File = "");
      void SettleWater();
      FlowDirection EvaluateSlope(const float *map, unsigned int pos);
      std::pair<int, unsigned> EvaluateSpread(const float *map, unsigned int pos);
      void Evaluate_Flat(float *&map, unsigned int pos);
      void FlowLeft(float *&map, unsigned int start_pos);
      void FlowRight(float *&map, unsigned int start_pos);
      void Make_Rain(float *&map, unsigned int pos);
      bool ReadFile(int **Buffer, const std::string& _File);
    }; //class terrain

    inline int TerrainCreator::Get_Width ()
    {
      return MapWidth;
    }

  } //Procedural
} //Framework

