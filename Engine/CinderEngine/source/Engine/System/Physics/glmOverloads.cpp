#include "glmOverloads.h"

namespace Framework
{

	glm::vec2 operator*(glm::vec2 vec, float scale)
	{
		glm::vec2 temp;
		vec.x = temp.x * scale;
		vec.y = temp.x * scale;
		return temp;
	}
}