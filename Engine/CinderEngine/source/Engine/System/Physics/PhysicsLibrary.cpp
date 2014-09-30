#include "PhysicsLibrary.h"

namespace Framework
{
	namespace Physics
	{
		/*
		Vec2 computeFriction(Manifold* m)
		{
		  //
		}
		*/
		Vec2 applyNetForce(Vec2 sumForce, float mass)
		{
			Vec2 accel = sumForce / mass;
			return accel;
		}

		Vec2 getReflection(Vec2& normal, Vec2& velocity)
		{
			Vec2 reflect;
			double dP = DotProduct(velocity, normal);
			reflect = velocity - (2 * dP * normal);
			return reflect;
		}

		float Distance(const Vec2& pos1, const Vec2& pos2)
		{
			return sqrt((pos2.x_ - pos1.x_) * (pos2.x_ - pos1.x_)
				+ (pos2.y_ - pos1.y_) * (pos2.y_ - pos1.y_));
		}

		Vec2 getNormal(Vec2 side)
		{
			Vec2 norm;
			norm.x_ = -side.y_;
			norm.y_ = side.x_;

			return norm;
		}

		float DotProduct(const Vec2& vec1, const Vec2& vec2)
		{
			float result = (vec1.x_ * vec2.x_) + (vec1.y_ * vec2.y_);
			return result;
		}

		Vec2 applyAccel(Vec2 accel, double dt)
		{
			Vec2 velChange = accel * dt;
			return velChange;
		}

		Vec2 getAccel(Vec2& force, double mass)
		{
			Vec2 accel = force / mass;
			return accel;
		}
	}
}
