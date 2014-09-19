/******************************************************************************/
/*!
\file   LightingShader.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _LIGHTING_SHADER_H
#define _LIGHTING_SHADER_H

#include "Shader.h"

namespace Framework
{

  class LightingShader : public Shader
  {
  public:
    LightingShader (const char* vs, const char* fs, const char* gs = 0);
  ~LightingShader ();
  
  private:

  };  
}

#endif