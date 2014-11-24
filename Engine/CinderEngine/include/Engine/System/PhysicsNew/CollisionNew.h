#ifndef COLLISION_H
#define COLLISION_H

#include "Collider2D.h"

namespace Framework
{
	class Manifold;
	class RigidBody2D;
	
	typedef void (*CollisionCallback)( Manifold *m, RigidBody2D *a, RigidBody2D *b );
	
	extern CollisionCallback Dispatch[ShapeCollider2D::eCount][ShapeCollider2D::eCount];
	
	void CircletoCircle( Manifold *m, RigidBody2D *a, RigidBody2D *b );
	void CircletoPolygon( Manifold *m, RigidBody2D *a, RigidBody2D *b );
	void PolygontoCircle( Manifold *m, RigidBody2D *a, RigidBody2D *b );
	void PolygontoPolygon( Manifold *m, RigidBody2D *a, RigidBody2D *b );
}


#endif // COLLISION_H