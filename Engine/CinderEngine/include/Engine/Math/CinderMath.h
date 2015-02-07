/******************************************************************************/
/*!
\file   CinderMath.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once

#define  GLM_FORCE_RADIANS
#include "glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/noise.hpp"
#include "glm\gtx\transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Vec2.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <glm/gtx/norm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

#ifndef M_PI
#define M_PI 		  3.1415926535897932384626433832795f
#define M_2_PI 		6.28318530717958647692528676655901f		// PI*2
#endif