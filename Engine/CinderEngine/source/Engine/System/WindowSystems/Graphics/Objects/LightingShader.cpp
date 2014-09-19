/******************************************************************************/
/*!
\file   LightingShader.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "LightingShader.h"

namespace Framework
{

  // Constructor
  LightingShader::LightingShader (const char* vs, const char* fs, const char* gs) : Shader (vs, fs, gs)
  {}
  
  // Destructor
  LightingShader::~LightingShader ()
  {}
}