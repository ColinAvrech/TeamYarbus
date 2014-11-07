/*****************************************************************************/
/*!
\file   FractalNoise.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Generate a simplex fractal noise texture
*/
/*****************************************************************************/
#include "FractalNoise.h"
#include "glm/gtc/random.hpp"
#include "glm/gtc/noise.hpp"
#include <stdlib.h>
#include <iostream>
namespace Framework
{
  namespace Procedural
  {
    FractalNoise2D::FractalNoise2D(int width, int passes) :
      size(width)
    {
      Array = new float[size];
      float *Buffer = new float[size];

      for (int i = 0; i < size; ++i)
        Array[i] = 0;

      for (int i = 0; i < size; ++i)
      {
        int r = rand() % 2;
        Buffer[i] = std::abs(glm::simplex(glm::vec2(r, r)));
      }

      int div = 1;
      for (int i = 0; i < passes - 1; ++i)
        div *= 2;
      for (int i = 0; i < size; ++i)
      {
        float weight = 0.5f / div;
        int denom = 1;
        for (int k = 1; k < passes; ++k, weight *= 2, denom *= 2)
        {
          Array[i] += weight * Buffer[i / denom];
        }
      }
    }

    FractalNoise2D::~FractalNoise2D()
    {
      delete[] Array;
    }
  } //Procedural
} //Framework