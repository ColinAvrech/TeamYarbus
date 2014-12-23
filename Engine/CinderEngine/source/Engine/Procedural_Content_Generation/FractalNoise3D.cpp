/*****************************************************************************/
/*!
\file   FractalNoise.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Generate a simplex fractal noise texture
*/
/*****************************************************************************/

#include <Precompiled.h>
#include "FractalNoise.h"
#include "CinderMath.h"
#include <stdlib.h>
#include <iostream>
namespace Framework
{
  namespace Procedural
  {
    FractalNoise3D::FractalNoise3D(int width, int height, int passes) :
      sizeX(width), sizeY(height)
    {
      Array = new float*[width];
      for (int i = 0; i < sizeX; ++i)
        Array[i] = new float[sizeY];

      float **Buffer = new float*[width];
      for (int i = 0; i < sizeX; ++i)
        Buffer[i] = new float[sizeY];

      for (int i = 0; i < sizeX; i++)
      {
        for (int j = 0; j < sizeY; j++)
        {
          int r = rand() % 2;
          Buffer[i][j] = std::abs(glm::simplex(vec2(r, r)));
        }
      }
      for (int i = 0; i < sizeX; i++)
      {
        for (int j = 0; j < sizeY; j++)
        {
          int r = rand() % 2;
          Array[i][j] = 0;
        }
      }
      int div = 1;
      for (int i = 0; i < passes; ++i)
        div *= 2;
      float weight = 0.5f / div;
      int denom = 1;
      for (int k = 1; k < passes; ++k, weight *= 2, denom *= 2)
      {
        for (int j = 0; j < sizeX; ++j)
        {
          for (int i = 0; i < sizeY; ++i)
          {
            Array[j][i] += weight * Buffer[j / denom][i / denom];
          }
        }
      }
      //for (int i = 0; i < sizeY; i++)
      //{
      //  for (int j = 0; j < sizeX; j++)
      //  {
      //    Array[i][j] -= 1;
      //  }
      //}

      for (int i = 0; i < sizeX; i++)
      {
        for (int j = 0; j < sizeY; j++)
        {
          std::cout << Array[i][j] << " ";
        }
        std::cout << std::endl;
      }

      for (int i = 0; i < sizeX; ++i)
        delete[] Buffer[i];
      delete[] Buffer;
    }

    FractalNoise3D::~FractalNoise3D()
    {
      for (int i = 0; i < sizeX; ++i)
        delete[] Array[i];
      delete Array;
    }
  } //procedural
} //framework