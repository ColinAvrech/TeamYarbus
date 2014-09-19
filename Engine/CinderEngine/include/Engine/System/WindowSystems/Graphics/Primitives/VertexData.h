/******************************************************************************/
/*!
\file   SpriteData.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SPRITE_DATA_H
#define _SPRITE_DATA_H

#include "CinderMath.h"


namespace Framework
{
  struct VertexData
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
    glm::vec2 texCoord;
  };

  struct textureData
  {
    unsigned int id;
    unsigned int type;
  };
}

#endif