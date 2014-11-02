/*****************************************************************************/
/*!
\file   FractalNoise.h
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
    class FractalNoise2D
    {
    public:
      FractalNoise2D(int size, int passes);
      ~FractalNoise2D();
    private:
      int *Array;
    };

    //3D perlin noise generator (2D array)
    class FractalNoise3D
    {
    public:
      FractalNoise3D(int width, int height, int passes);
      ~FractalNoise3D();
    private:
      float **Array;
      int sizeX;
      int sizeY;
    };
  } //procedural
} //framework