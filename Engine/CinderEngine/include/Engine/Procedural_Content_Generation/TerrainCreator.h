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
        int peak = 100, int water = 50, const std::string& HMap = "", glm::vec2 &_slope = glm::vec2(1.f, 0.f));
      //Destructor
      ~TerrainCreator();
      float * const GetRockMap() { return HeightMapRock;  }
      float * const GetSoilMapl(){ return HeightMapSoil;  }
      float * const GetWaterMap(){ return HeightMapWater; }

      float GetPeakHeight () const
      {
        float s = std::abs(Slope.y / Slope.x);
        float t0 = MapWidth * s;
        return float(PeakHeight + t0);
      }

      inline int Get_Width ();

      void Generate(const std::string& MapFile);
      void Save(const char *file);
      void Load(const char *file);

    private:
      int detail = 10;
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
      glm::vec2 Slope;

      //Private Member Functions
      //Layering
      void AddSoil();
      void AddRock(const std::string& File);
      void AddWater();

      //Helper functions
      void GenerateHeightMap(float **Array, int base, int height, const std::string& File = "");
      void SettleWater();

      //Store these in a temporary buffer
      void Find_Valleys();
      
      bool ReadFile(int **Buffer, const std::string& _File);
    }; //class terrain

    inline int TerrainCreator::Get_Width ()
    {
      return MapWidth;
    }

  } //Procedural
} //Framework

