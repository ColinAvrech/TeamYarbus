/*****************************************************************************/
/*!
\file   TerrainCreator3D.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate 3d terrains (for thermo testing)
*/
/*****************************************************************************/

#pragma once

namespace Framework
{
  namespace Procedural
  {
    class TerrainCreator3D
    {
    public:
      TerrainCreator3D(){}
      //Constructor
      //Takes map width, map height, base height, smoothing passes, number of waves
      TerrainCreator3D(int width, int depth, int baseHeight, int passes = 3, int waves = 2, int peak = 100);
      //Destructor
      ~TerrainCreator3D();
      float **const GetMap();

      inline int Get_Width();
      inline int Get_Depth();

      void Generate();
      void Save(const char *file);
      void Load(const char *file);

    private:
      //Terrain settings
      int passes;
      int waves;
      int MapWidth;
      int MapDepth;
      int BaseHeight;
      TerrainCreator **HeightMapRock;
      int PeakHeight;

      void GenerateHeightMap(int base, int height);
    };
  }
}