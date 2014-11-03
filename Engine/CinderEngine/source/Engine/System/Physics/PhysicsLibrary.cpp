#include "PhysicsLibrary.h"
#include "RigidBody.h"
#include "GraphicsCommon.h"

namespace Framework
{
	namespace Physics
	{
		float DetermineRestitution(GameObject* a, GameObject* b)
		{
			//return std::min(a->RigidBody->Restitution, b->Restitution);
			return 0;
		}

		float DetermineFriction(GameObject* a, GameObject* b)
		{
			//return sqrt(a->Friction * b->Friction);
			return 0;
		}

		glm::vec2 applyNetForce(glm::vec2 sumForce, double InvMass)
		{
			glm::vec2 accel = sumForce * InvMass;
			return accel;
		}

		glm::vec2 getReflection(glm::vec2& normal, glm::vec2& velocity)
		{
			glm::vec2 reflect;
			float dP = DotProduct(velocity, normal);
			reflect = velocity - (2.0f * dP * normal);
			return reflect;
		}

		float Distance(const glm::vec2& pos1, const glm::vec2& pos2)
		{
			return float(sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x)
				+ (pos2.y - pos1.y) * (pos2.y - pos1.y)));
		}

		glm::vec2 getNormal(glm::vec2 p1, glm::vec2 p2)
		{
			glm::vec2 norm;
			glm::vec2 line = p2 - p1;
			norm.x = -line.y;
			norm.y = line.x;
			norm = glm::normalize(norm);

			return norm;
		}

		float DotProduct(const glm::vec2& u, const glm::vec2& v)
		{
			float result = float((u.x * v.x) + (u.y * v.y));
			return result;
		}

		glm::vec2 applyAccel(glm::vec2 accel, double dt)
		{
			glm::vec2 velChange = accel * (float)dt;
			return velChange;
		}

		glm::vec2 getAccel(glm::vec2 force, double mass)
		{
			glm::vec2 accel = force / (float)mass;
			return accel;
		}
	}
}
