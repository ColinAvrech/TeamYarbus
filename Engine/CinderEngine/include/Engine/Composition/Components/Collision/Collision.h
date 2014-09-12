/******************************************************************************/
/*!
\file   Collision.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Collision detection for point, circle, and line.
*/
/******************************************************************************/

/*
This method makes use of three seperate collision checks
1) The first is the area check, which wakes up a section
[some arbitrary number of grid cells] when it detects the player
has intersected with the lowest x-value of the section. Slight area
of interlap over sections for smooth section integration. (drawing)

2) The second checks the specific grid square cell that contains a
collidable object.
-Manually set in Zero? (drawing)

3) The third checks for the circle or point collider attached to
the colliding edge of the gameobject.

Relevant Questions:
Do lines need to be dynamic (falling corner of house)?
Can the section be comprised of 2d array(s) of ghost colliders?
*/
#include <vector>
#include "Circle.h"
#include "Point.h"

// 1) LISTEN FOR AREA COLLISION
bool WakeUpArea(std::vector<float> ground, Circle* c);	//if player is a circle
bool WakeUpArea(std::vector<float> ground, Point p);	//if player is a point

// 2) LISTEN FOR GRID SQUARE COLLISION
bool WakeUpCell(Point bottomLeft, Circle c); //if player is a circle
bool WakeUpCell(Point bottomLeft, Point p);	 //if player is a point

// 3) LISTEN FOR POINT/CIRCLE COLLISION
/*
Circle to Circle
*/
bool DetectCircletoCircle(Circle c1, Circle c2);
std::vector<float> ResolveDynamicCircletoStaticCircle(Circle dc, Circle sc);
std::vector<float> ResolveDynamicCircletoDynamiCircle(Circle d1c, Circle d2c);

/*
Circle to Point
*/
bool DetectCircletoPoint(Circle c, Point p);
std::vector<float> ResolveDynamicCircletoStaticPoint(Circle c, Point p); //if player is a circle
std::vector<float> ResolveStaticCircletoDynamicPoint(Circle c, Point p); //if player is a point

/*
Circle & Point to Line
*/
//Does the line need a position? 
//How do we get line position from Zero file?
bool DetectCircletoLine(Circle c, std::vector<float> Line); //if player is a circle
bool DetectPointtoLine(Point p, std::vector<float> Line);	  //if player is a point
std::vector<float> ResolveDynamicCircletoLine(Circle dc, Point p);
std::vector<float> ResolveDynamicPointtoLine(Point p, std::vector<float> Line);
