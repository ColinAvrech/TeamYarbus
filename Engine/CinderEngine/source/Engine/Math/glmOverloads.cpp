/******************************************************************************/
/*!
\file   glmOverloads.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "GraphicsCommon.h"

namespace Framework
{
  vec2 operator*(const vec2& vec, const float scale)
  {
    vec2 newvec = vec;
    newvec *= scale;
    return newvec;
  }

  vec2 operator*(const float scale, const vec2& vec)
  {
    vec2 newvec = vec;
    newvec *= scale;
    return newvec;
  }

}