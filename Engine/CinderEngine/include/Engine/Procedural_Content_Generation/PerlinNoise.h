/*****************************************************************************/
/*!
\file   PerlinNoise.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Generate a perlin noise texture
*/
/*****************************************************************************/

namespace Framework
{
  namespace Procedural
  {
    //2D perlin noise generator (1D array)
    class PerlinNoise2D
    {
    public:
      PerlinNoise2D(int size, int wavelength);
    private:
      int *Array;
    };

    //3D perlin noise generator (2D array)
    class PerlinNoise3D
    {
    public:
      PerlinNoise3D(int width, int height, int wavelength);
    private:
      int **Array;
    };
  } //procedural
} //framework