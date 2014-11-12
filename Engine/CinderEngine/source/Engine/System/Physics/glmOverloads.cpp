#include "glmOverloads.h"

namespace Framework
{

	glm::vec3 operator*(const glm::vec3& vec, const float scale)
	{
		glm::vec3 temp;
		temp.x = vec.x * scale;
		temp.y = vec.y * scale;
		temp.z = vec.z * scale;
		return temp;
	}


	glm::vec3 operator*(const float scale, const glm::vec3& vec)
	{
		glm::vec3 temp;
		temp.x = vec.x * scale;
		temp.y = vec.y * scale;
		temp.z = vec.z * scale;
		return temp;
	}
}