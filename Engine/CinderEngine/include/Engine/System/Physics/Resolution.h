

#pragma once

#include "RigidBody.h"
namespace Framework
{
	namespace Physics
	{

		void Resolve(Manifold* pre);
		void ResolveStatic(Manifold* pre, Manifold* post);
		void ResolveDynamic(Manifold* pre, Manifold* post);
	}
}