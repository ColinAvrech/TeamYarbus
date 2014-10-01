#include "ColliderShape.h"
#include "PhysicsLibrary.h"
#include "Resolution.h"

namespace Framework
{
	namespace Physics
	{
		//void Resolve(Manifold* pre)
		//{
		//	Manifold post;
		//	if (pre->stateA == RigidBody::Static ||
		//		pre->stateB == RigidBody::Static)
		//	{
		//		ResolveStatic(pre, &post);
		//	}
		//	else
		//	{
		//		ResolveDynamic(pre, &post);
		//	}
		//}

		//void ResolveStatic(Manifold* pre, Manifold* post)
		//{
		//	*post = *pre;
		//	if (pre->stateA == RigidBody::Static)
		//	{
		//		post->velB = getReflection(pre->normal, pre->velB);
		//	}
		//	else
		//	{
		//		post->velA = getReflection(pre->normal, pre->velA);
		//	}
		//}

		//void ResolveDynamic(Manifold* pre, Manifold* post)
		//{
		//	*post = *pre;
		//	float bunch11 = (pre->massA - pre->massB) / (pre->massA + pre->massB);
		//	float bunch12 = (2 * pre->massB) / (pre->massA + pre->massB);
		//	post->velA.x_ = bunch11 * pre->velA.x_ + bunch12 * pre->velB.x_;
		//	post->velA.y_ = bunch11 * pre->velA.y_ + bunch12 * pre->velB.y_;

		//	float bunch21 = (pre->massB - pre->massA) / (pre->massA + pre->massB);
		//	float bunch22 = (2 * pre->massA) / (pre->massA + pre->massB);
		//	post->velA.x_ = bunch21 * pre->velA.x_ + bunch22 * pre->velB.x_;
		//	post->velA.y_ = bunch21 * pre->velA.y_ + bunch22 * pre->velB.y_;
		//}
	} //Physics
} //Framework