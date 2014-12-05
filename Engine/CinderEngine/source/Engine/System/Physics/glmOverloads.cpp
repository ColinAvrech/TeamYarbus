/******************************************************************************/
/*!
\file   glmOverloads.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "glmOverloads.h"

namespace Framework
{

	vec3 operator*(const vec3& vec, const float scale)
	{
		vec3 temp;
		temp.x = vec.x * scale;
		temp.y = vec.y * scale;
		temp.z = vec.z * scale;
		return temp;
	}


	vec3 operator*(const float scale, const vec3& vec)
	{
		vec3 temp;
		temp.x = vec.x * scale;
		temp.y = vec.y * scale;
		temp.z = vec.z * scale;
		return temp;
	}
}