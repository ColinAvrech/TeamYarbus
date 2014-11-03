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
    enum Material
    {
      AIR,
      WATER,
      WOOD,
      SOIL,
      GRASS,
      STONE,
      IRON,
      LEAD,
      STEEL,
      COTTON,
      CEMENT,
      CARBON,
      FUEL,
    };//enum material

    class TerrainCreator
    {
    public:
      //Constructor
      TerrainCreator(int, int, int, int = 4, int = 2);
      //Destructor
      ~TerrainCreator();
      int ** const GetMap(){ return Map; };
    private:
      int passes;
      int waves;
      int MapWidth;
      int MapHeight;
      int BaseHeight; //Must be a number between 0 and height.
      float *HeightMap;
      int ** Map;

      //private member functions
      void GenerateHeightMap();
      void ApplyHeightMap();
    }; //class terrain
  } //Procedural
} //Framework

