/******************************************************************************/
/*!
\file   CollisionNew.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Collision.h"
#include "Manifold.h"
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "Events.h"

namespace Framework
{
	CollisionCallback Dispatch[ShapeCollider2D::eCount][ShapeCollider2D::eCount] =
	{
	  {
	    CircletoCircle, CircletoPolygon, CompoundSolve
	  },
	  {
	    PolygontoCircle, PolygontoPolygon, CompoundSolve
	  },
    {
      CompoundSolve, CompoundSolve, CompoundSolve
    }
	};
	
	void CircletoCircle( Manifold *m, ShapeCollider2D* shapeA, ShapeCollider2D* shapeB)
  {
    CircleCollider2D *circleA = static_cast<CircleCollider2D *>(shapeA);
    CircleCollider2D *circleB = static_cast<CircleCollider2D *>(shapeB);
	
    RigidBody2D *rbA = shapeA->gameObject->C<RigidBody2D>();
    RigidBody2D *rbB = shapeB->gameObject->C<RigidBody2D>();

    Transform* tformA = shapeA->gameObject->C<Transform>();
    Transform* tformB = shapeB->gameObject->C<Transform>();

	  // Calculate translational vector, which is normal
    vec2 normal = tformB->GetPosition2D() - tformA->GetPosition2D();
	
	  float dist_sqr = normal.length() * normal.length();
	  float radius = circleA->GetRadius() + circleB->GetRadius();
	
	  // Not in contact
	  if(dist_sqr >= radius * radius)
	  {
	    m->contact_count = 0;
	    return;
	  }
	  
    //has collided
    CollisionEvent collisionEvent;
    collisionEvent.thisObject = circleB->gameObject;
    collisionEvent.OtherObject = circleA->gameObject;
    //collisionEvent.normal.x = B->m_normals [faceNormal].x;
    //collisionEvent.normal.y = B->m_normals [faceNormal].y;
    //collisionEvent.normal.z = 0.0f;
    EVENTSYSTEM->TriggerEvent (Events::COLLISION, collisionEvent);

	  float distance = std::sqrt( dist_sqr );
	
	  m->contact_count = 1;
	
	  if(distance == 0.0f)
	  {
	    m->penetration = circleA->GetRadius();
	    m->normal = vec2( 1, 0);
	    m->contacts [0] = tformA->GetPosition2D();
	  }
	  else
	  {
	    m->penetration = radius - distance;
	    m->normal = normal / distance; // Faster than using Normalized since we already performed sqrt
      m->contacts[0] = m->normal * circleA->GetRadius() + tformA->GetPosition2D();
	  }
	}
	

  void CircletoPolygon(Manifold *m, ShapeCollider2D *a, ShapeCollider2D *b)
  {
    CircleCollider2D *circle = static_cast<CircleCollider2D *>(a);
    PolygonCollider2D *polygon = static_cast<PolygonCollider2D *>(b);
    
    m->contact_count = 0;

    // Transform circle center to Polygon model space
    vec2 center = circle->GetCenter();
    //center = glm::transpose(polygon->u) * (center - polygon->GetCenter());

    // Find edge with minimum penetration
    // Exact concept as using support points in Polygon vs Polygon
    float separation = -FLT_MAX;
    unsigned faceNormal = 0;
    for (unsigned i = 0; i < polygon->m_vertexCount; ++i)
    {
      float s = glm::dot(polygon->m_normals[i], center - polygon->m_vertices[i]);

      if (s > circle->GetRadius())
        return;

      if (s > separation)
      {
        separation = s;
        faceNormal = i;
      }
    }
    //has collided
    CollisionEvent collisionEvent;
    collisionEvent.thisObject = polygon->gameObject;
    collisionEvent.OtherObject = circle->gameObject;
    collisionEvent.normal = polygon->m_normals[faceNormal];
    EVENTSYSTEM->TriggerEvent(Events::COLLISION, collisionEvent);

    // Grab face's vertices
    vec2 v1 = polygon->m_vertices[faceNormal];
    unsigned i2 = faceNormal + 1 < polygon->m_vertexCount ? faceNormal + 1 : 0;
    vec2 v2 = polygon->m_vertices[i2];

    // Check to see if center is within polygon
    if (separation < EPSILON)
    {
      m->contact_count = 1;
      m->normal = -(polygon->u * polygon->m_normals[faceNormal]);
      m->penetration = circle->GetRadius();
      m->contacts[0] = m->normal * m->penetration + circle->GetCenter();
      return;
    }

    // Determine which voronoi region of the edge center of circle lies within
    float dot1 = glm::dot(center - v1, v2 - v1);
    float dot2 = glm::dot(center - v2, v1 - v2);
    m->penetration = circle->GetRadius() - separation;

    // Closest to v1
    if (dot1 <= 0.0f)
    {
      if (glm::distance(center, v1) > circle->GetRadius())
        return;

      m->contact_count = 1;
      vec2 n = v1 - center;
      n = polygon->u * n;
      m->normal = glm::normalize(n);
      v1 = polygon->u * v1 + polygon->GetCenter();
      m->contacts[0] = v1;
    }

    // Closest to v2
    else if (dot2 <= 0.0f)
    {
      if (glm::distance(center, v2) > circle->GetRadius())
        return;

      m->contact_count = 1;
      vec2 n = v2 - center;
      v2 = polygon->u * v2 + polygon->GetCenter();
      m->contacts[0] = v2;
      n = polygon->u * n;
      m->normal = glm::normalize(n);
    }

    // Closest to face
    else
    {
      vec2 n = polygon->m_normals[faceNormal];
      if (glm::dot(center - v1, n) > circle->GetRadius())
        return;

      m->contact_count = 1;
      n = polygon->u * n;
      m->normal = -n;
      m->contacts[0] = m->normal * circle->GetRadius() + circle->GetCenter();
    }
  }

  void PolygontoCircle(Manifold *m, ShapeCollider2D* polygon, ShapeCollider2D* circle)
  {
    CircletoPolygon(m, circle, polygon);
    m->normal = -m->normal;
  }


  float FindAxisLeastPenetration(unsigned *faceIndex, PolygonCollider2D *A, PolygonCollider2D *B)
  {
    float bestDistance = -FLT_MAX;
    unsigned bestIndex = -1;

    RigidBody2D* rbA = A->gameObject->C<RigidBody2D>();
    RigidBody2D* rbB = B->gameObject->C<RigidBody2D>();

    Transform* tformA = A->gameObject->C<Transform>();
    Transform* tformB = B->gameObject->C<Transform>();

    for (unsigned i = 0; i < A->m_vertexCount; ++i)
    {
      // Retrieve a face normal from A
      vec2 n = A->m_normals[i];
      vec2 nw = A->u * n;

      // Transform face normal into B's model space
      glm::mat2 buT = glm::transpose(B->u);
      n = buT * nw;

      // Retrieve support point from B along -n
      vec2 s = B->GetSupport(-n);

      // Retrieve vertex on face from A, transform into
      // B's model space
      vec2 v = A->m_vertices[i];
      v = A->u * v + tformA->GetPosition2D();
      v -= tformB->GetPosition2D();
      v = buT * v;

      // Compute penetration distance (in B's model space)
      float d = glm::dot(n, s - v);

      // Store greatest distance
      if (d > bestDistance)
      {
        bestDistance = d;
        bestIndex = i;
      }
    }

    assert(bestIndex != -1);

    *faceIndex = bestIndex;
    return bestDistance;
  }

  void FindIncidentFace(vec2 *v, PolygonCollider2D *RefPoly, PolygonCollider2D *IncPoly, unsigned referenceIndex)
  {
    vec2 referenceNormal = RefPoly->m_normals[referenceIndex];

    // Calculate normal in incident's frame of reference
    referenceNormal = RefPoly->u * referenceNormal; // To world space
    referenceNormal = glm::transpose(IncPoly->u) * referenceNormal; // To incident's model space

    // Find most anti-normal face on incident polygon
    int incidentFace = 0;
    float minDot = FLT_MAX;
    for (unsigned i = 0; i < IncPoly->m_vertexCount; ++i)
    {
      float dot = glm::dot(referenceNormal, IncPoly->m_normals[i]);
      if (dot < minDot)
      {
        minDot = dot;
        incidentFace = i;
      }
    }

    // Assign face vertices for incidentFace
    RigidBody2D* rb = reinterpret_cast<RigidBody2D*>(IncPoly->gameObject->GetComponent("RigidBody2D"));
    v[0] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->GetCenter();
    incidentFace = incidentFace + 1 >= (int)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
    v[1] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->GetCenter();
  }

  int Clip(vec2 n, float c, vec2 *face)
  {
    unsigned sp = 0;
    vec2 out[2] = {
      face[0],
      face[1]
    };

    // Retrieve distances from each endpoint to the line
    // d = ax + by - c
    float d1 = glm::dot(n, face[0]) - c;
    float d2 = glm::dot(n, face[1]) - c;

    // If negative (behind plane) clip
    if (d1 <= 0.0f) out[sp++] = face[0];
    if (d2 <= 0.0f) out[sp++] = face[1];

    // If the points are on different sides of the plane
    if (d1 * d2 < 0.0f) // less than to ignore -0.0f
    {
      // Push interesection point
      float alpha = d1 / (d1 - d2);
      out[sp] = face[0] + alpha * (face[1] - face[0]);
      ++sp;
    }

    // Assign our new converted values
    face[0] = out[0];
    face[1] = out[1];

    assert(sp != 3);

    return sp;
  }

	void PolygontoPolygon( Manifold *m, ShapeCollider2D* shapeA, ShapeCollider2D* shapeB)
	{
    PolygonCollider2D *A = static_cast<PolygonCollider2D *>(shapeA);
    PolygonCollider2D *B = static_cast<PolygonCollider2D *>(shapeB);

    RigidBody2D* rbA = A->gameObject->C<RigidBody2D>();
    RigidBody2D* rbB = B->gameObject->C<RigidBody2D>();

	  m->contact_count = 0;
	
	  // Check for a separating axis with A's face planes
	  unsigned faceA;
	  float penetrationA = FindAxisLeastPenetration( &faceA, A, B );
	  if(penetrationA >= 0.0f)
	    return;
	
	  // Check for a separating axis with B's face planes
	  unsigned faceB;
	  float penetrationB = FindAxisLeastPenetration( &faceB, B, A );
	  if(penetrationB >= 0.0f)
	    return;
	
	  unsigned referenceIndex;
	  bool flip; // Always point from a to b
	
	  PolygonCollider2D *RefPoly; // Reference
	  PolygonCollider2D *IncPoly; // Incident
	
	  // Determine which shape contains reference face
	  if(BiasGreaterThan( penetrationA, penetrationB ))
	  {
	    RefPoly = A;
	    IncPoly = B;
	    referenceIndex = faceA;
	    flip = false;
	  }
	
	  else
	  {
	    RefPoly = B;
	    IncPoly = A;
	    referenceIndex = faceB;
	    flip = true;
	  }
	
	  // World space incident face
	  vec2 incidentFace[2];
	  FindIncidentFace( incidentFace, RefPoly, IncPoly, referenceIndex );
	
	  //        y
	  //        ^  ->n       ^
	  //      +---c ------posPlane--
	  //  x < | i |\
	  //      +---+ c-----negPlane--
	  //             \       v
	  //              r
	  //
	  //  r : reference face
	  //  i : incident poly
	  //  c : clipped point
	  //  n : incident normal
	
	  // Setup reference face vertices
	  vec2 v1 = RefPoly->m_vertices[referenceIndex];
	  referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
	  vec2 v2 = RefPoly->m_vertices[referenceIndex];
	
	  // Transform vertices to world space
    Transform* tform = RefPoly->gameObject->C<Transform>();
	  v1 = RefPoly->u * v1 + tform->GetPosition2D();
	  v2 = RefPoly->u * v2 + tform->GetPosition2D();
	
	  // Calculate reference face side normal in world space
	  vec2 sidePlaneNormal = glm::normalize(v2 - v1);
	  
	  // Orthogonalize
	  vec2 refFaceNormal( sidePlaneNormal.y, -sidePlaneNormal.x);
	
	  // ax + by = c
	  // c is distance from origin
	  float refC = glm::dot( refFaceNormal, v1 );
	  float negSide = -glm::dot( sidePlaneNormal, v1 );
    float posSide = glm::dot(sidePlaneNormal, v2);
	
	  // Clip incident face to reference face side planes
	  if(Clip( -sidePlaneNormal, negSide, incidentFace ) < 2)
	    return; // Due to floating point error, possible to not have required points
	
	  if(Clip(  sidePlaneNormal, posSide, incidentFace ) < 2)
	    return; // Due to floating point error, possible to not have required points
	
	  // Flip
	  m->normal = flip ? -refFaceNormal : refFaceNormal;
	
	  // Keep points behind reference face
	  unsigned cp = 0; // clipped points behind reference face
	  float separation = glm::dot( refFaceNormal, incidentFace[0] ) - refC;
	  if(separation <= 0.0f)
	  {
	    m->contacts[cp] = incidentFace[0];
	    m->penetration = -separation;
	    ++cp;
	  }
	  else
	    m->penetration = 0;
	
	  separation = glm::dot( refFaceNormal, incidentFace[1] ) - refC;
	  if(separation <= 0.0f)
	  {
	    m->contacts[cp] = incidentFace[1];
	
	    m->penetration += -separation;
	    ++cp;
	
	    // Average penetration
	    m->penetration /= (float)cp;
	  }
	
	  m->contact_count = cp;
	}

  void CompoundSolve(Manifold *m, ShapeCollider2D* a, ShapeCollider2D* b)
  {
    glm::vec2 normal = vec2();
    std::vector<vec2> normals;
    bool flipnormal = false;

    CompoundCollider2D* cc;
    if (a->GetType() == ShapeCollider2D::ColliderType::eCompound)
    {
      cc = static_cast<CompoundCollider2D*>(a);
    }
    else
    {
      cc = static_cast<CompoundCollider2D*>(b);
      flipnormal = true;
    }

    bool fCollision = false;
    ShapeCollider2D* c;
    for (unsigned i = 0; i < cc->childColliders.size() && !fCollision; ++i)
    {      
      c = cc->childColliders[i];
      if (!flipnormal)
      {
        Dispatch[c->GetType()][b->GetType()](m, c, b);
      }
      else
      {
        Dispatch[c->GetType()][a->GetType()](m, c, a);
        m->normal = -m->normal;
      }

      if (m->normal != vec2())
      {
        fCollision = true;
        //normals.push_back(m->normal);
        //normal = m->normal;
      }
    }

    /*Physics Debugging ##CBA*/
//    if (fCollision)
//      CORE->TogglePaused();
    
    //m->normal = glm::normalize(normal);
    /*
    if (normals.size())
    {
      __debugbreak();
    }
    */
  }
}

