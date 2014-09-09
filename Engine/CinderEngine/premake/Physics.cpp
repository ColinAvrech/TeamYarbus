/******************************************************************************/
/*!
\file   Physics.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "Physics.h"
//Seperated Axis Theorem
/*------------------------------------------------------------------------------
// Get outer edge of triangle
------------------------------------------------------------------------------*/
Vec2& GetEdge(Point a, Point b)
{
	Vec2 edge;
	edge.x_ = a.x - b.x;
	edge.y_ = a.y - b.y;
	return edge;
}
/*------------------------------------------------------------------------------
// Calculate the normal of the edge
------------------------------------------------------------------------------*/
Vec2 GetEdgeNormal(Vec2 vec)
{
	Vec2 normal;
	normal.x_ = -vec.y_;
	normal.y_ = vec.x_;
	return normal;
}

/*------------------------------------------------------------------------------
// Project each vertex (from both polygons) on edge's normal vector
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Determine intervals (min/max)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// If intervals overlap, then check the next edge. Only when all edges overlap
// is there a collision
------------------------------------------------------------------------------*/