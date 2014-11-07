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
      TerrainCreator (){}
      //Constructor
      //Takes map width, map height, base height, smoothing passes, number of waves
      TerrainCreator(int width, int height, int baseHeight, int passes = 3, int waves = 2);
      //Destructor
      ~TerrainCreator();
      int ** const GetMap(){ return Map; };

      inline int Get_Width ();
      inline int Get_Height ();

    private:
      int passes;
      int waves;
      int MapWidth;
      int MapHeight;
      int BaseHeight; //Must be a number between 0 and height.
      float *HeightMap;
      int ** Map;

      void GenerateHeightMap();
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

