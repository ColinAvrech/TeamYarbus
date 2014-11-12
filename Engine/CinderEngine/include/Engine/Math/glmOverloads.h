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
  glm::vec3 operator*(const glm::vec3& vec, const float scale);
  glm::vec3 operator*(const float scale, const glm::vec3& vec);

}