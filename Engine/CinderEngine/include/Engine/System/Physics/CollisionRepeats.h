/******************************************************************************/
/*!
\file   CollisionRepeats.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "IncludeForAllCollision.h"
#include "Resolution.h"

namespace Framework
{
	namespace Physics
	{
    /*
    This function determines the distance separating a point from a line

    - Parameters
    - P:		The point whose location should be determined
    - LS:		The line segment

    - Returned value: The distance. Note that the returned value should be:
    - Negative if the point is in the line's inside half plane
    - Positive if the point is in the line's outside half plane
    - Zero if the point is on the line
    */
    float DistancePoint2Line(const glm::vec3& p, const LineCollider& l);
    /*
    This function checks whether an animated point is colliding with a line segment

    - Parameters
    - Ps:		The point's starting location
    - Pe:		The point's ending location
    - LS:		The line segment
    - Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

    - Returned value: Intersection time t
    - -1.0f:				If there's no intersection
    - Intersection time:	If there's an intersection
    */
    BodyContact PointvsLine(const glm::vec3& ps, const glm::vec3& pe, const LineCollider& l);
    /*
    This function checks whether an animated circle is colliding with a line segment

    - Parameters
    - Ps:		The center's starting location
    - Pe:		The center's ending location
    - Radius:	The circle's radius
    - LS:		The line segment
    - Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

    - Returned value: Intersection time t
    - -1.0f:				If there's no intersection
    - Intersection time:	If there's an intersection
    */
    BodyContact CirclevsLine(const glm::vec3& ps, const glm::vec3& pe, const CircleCollider& c, const LineCollider& l);
    /*
    This function checks whether an animated point is colliding with a static circle

    - Parameters
    - Ps:		The point's starting location
    - Pe:		The point's ending location
    - Center:	The circle's center
    - Radius:	The circle's radius
    - Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

    - Returned value: Intersection time t
    - -1.0f:		If there's no intersection
    - Intersection time:	If there's an intersection
    */
    //BodyContact CirclevsPoint(const float rad, glm::vec3& cpos, glm::vec3& ppos);
	}
}
