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

		vec3 applyNetForce(vec3 sumForce, double InvMass)
		{
			vec3 accel = sumForce * (float)InvMass;
			return accel;
		}

		vec3 getReflection(vec3& normal, vec3& velocity)
		{
			vec3 reflect;
			float dP = DotProduct(velocity, normal);
			reflect = velocity - (2.0f * dP * normal);
			return reflect;
		}

		float Distance(vec3& pos1, vec3& pos2)
		{
			return float(sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x)
				+ (pos2.y - pos1.y) * (pos2.y - pos1.y)));
		}

		float DistanceSquared(vec3& pos1, vec3& pos2)
		{
			return float((pos2.x - pos1.x) * (pos2.x - pos1.x)
				+ (pos2.y - pos1.y) * (pos2.y - pos1.y));
		}

		vec3 getNormal(vec3 p1, vec3 p2)
		{
			vec3 norm;
			vec3 line = p2 - p1;
			norm.x = -line.y;
			norm.y = line.x;
			norm = glm::normalize(norm);

			return norm;
		}

		float DotProduct(vec3& u, vec3& v)
		{
			float result = float((u.x * v.x) + (u.y * v.y));
			return result;
		}

		vec3 applyAccel(vec3 accel, double dt)
		{
			vec3 velChange = accel * (float)dt;
			return velChange;
		}

		vec3 getAccel(vec3 force, double mass)
		{
			vec3 accel = force / (float)mass;
			return accel;
		}
	}
}
