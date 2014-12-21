/******************************************************************************/
/*!
\file   CollisionNew.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _COLLISION_NEW_H
#define _COLLISION_NEW_H

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


#endif // _COLLISION_NEW_H