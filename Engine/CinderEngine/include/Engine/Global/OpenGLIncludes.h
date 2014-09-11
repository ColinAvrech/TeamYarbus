/******************************************************************************/
/*!
\file   OpenGLIncludes.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file has all of the code to include OpenGL using GLEW
*/
/******************************************************************************/

#pragma once

#define GLEW_STATIC
#define GLUW_BUILD

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "../../../dependency/glew/include/glew.h"

