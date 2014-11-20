/******************************************************************************/
/*!
\file   glmOverloads.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "GraphicsCommon.h"

namespace Framework
{
  vec3 operator*(const vec3& vec, const float scale);
  vec3 operator*(const float scale, const vec3& vec);

}