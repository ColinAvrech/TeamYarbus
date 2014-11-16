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
      TerrainCreator(int width, int height, int baseHeight, int passes = 3, int waves = 2, int peak = 100, int water = 50);
      //Destructor
      ~TerrainCreator();
      int ** const GetMap(){ return Map; };

      inline int Get_Width ();
      inline int Get_Height ();

      void Generate();
      void Save(const char *file);
      void Load(const char *file);

    private:
      //Terrain settings
      int passes;
      int waves;
      int MapWidth;
      int MapHeight;
      int BaseHeight; //Must be a number between 0 and height.
      float *HeightMap;
      int ** Map;
      int PeakHeight;
      int WaterDepth;

      //Private Member Functions
      //Layering
      void AddLife();
      void AddSoil();
      void AddRock();
      void AddWater();

      //Helper functions
      void GenerateHeightMap();
      void ApplyHeightMap();
    }; //class terrain

    inline int TerrainCreator::Get_Width ()
    {
      return MapWidth;
    }

    inline int TerrainCreator::Get_Height ()
    {
      return MapHeight;
    }


  } //Procedural
} //Framework

