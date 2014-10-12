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
  glm::vec2 operator*(const glm::vec2& vec, const float scale);
  glm::vec2 operator*(const float scale, const glm::vec2& vec);

}