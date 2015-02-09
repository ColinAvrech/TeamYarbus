/******************************************************************************/
/*!
\file   Collider2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "GameObject.h"
#include "Collider2D.h"
#include "PhysicsSystem.h"

namespace Framework
{
  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // SHAPE COLLIDER 2D IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////

  ShapeCollider2D::ShapeCollider2D ()
  {
    gameObject = nullptr;
  }

  ShapeCollider2D::~ShapeCollider2D ()
  {
    gameObject = nullptr;
  }



  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // CIRCLE COLLIDER 2D IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////

  CircleCollider2D::CircleCollider2D(float r) : ShapeCollider2D()
  {
    radius = r;
  }

  // First and last use of CS225
  ShapeCollider2D * CircleCollider2D::Clone (void) const
  {
    return new CircleCollider2D (radius);
  }

  void CircleCollider2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Radius");
    value->GetValue (&radius);
    radius *= static_cast<Transform*>(gameObject->GetComponent("Transform"))->GetScale().x;
  }

  void CircleCollider2D::Initialize ()
  { 
  }

  void CircleCollider2D::Draw (void) const
  {
    const unsigned k_segments = 20;

    // Render a circle with a bunch of lines
    glColor3f (1, 0, 0);
    glBegin (GL_LINE_LOOP);
    RigidBody2D* rigidBody = static_cast<RigidBody2D*>(gameObject->GetComponent("RigidBody2D"));
    float theta = rigidBody->orient;
    float inc = PI * 2.0f / (float) k_segments;
    for (unsigned i = 0; i < k_segments; ++i)
    {
      theta += inc;
      vec3 p (std::cos (theta), std::sin (theta), 0.0f);
      p *= radius;
      p += rigidBody->position;
      glVertex2f (p.x, p.y);
    }
    glEnd ();

    // Render line within circle so orientation is visible
    glBegin (GL_LINE_STRIP);
    vec3 r (0, 1.0f, 0.0f);
    float c = std::cos (rigidBody->orient);
    float s = std::sin (rigidBody->orient);
    r = vec3(r.x * c - r.y * s, r.x * s + r.y * c, 0.0f);
    r *= radius;
    r += rigidBody->position;
    glVertex2f (rigidBody->position.x, rigidBody->position.y);
    glVertex2f (r.x, r.y);
    glEnd ();
  }

  float CircleCollider2D::GetArea()
  {
    return PI * radius * radius; 
  }

  ShapeCollider2D::ColliderType CircleCollider2D::GetType (void) const
  {
    return eCircle;
  }

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // POLYGON COLLIDER 2D IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////

  PolygonCollider2D::PolygonCollider2D() : ShapeCollider2D()
  {
  }

  // First and last use of CS225
  ShapeCollider2D* PolygonCollider2D::Clone (void) const
  {
    PolygonCollider2D* poly = new PolygonCollider2D ();
    poly->u = u;
    for (unsigned i = 0; i < m_vertexCount; ++i)
    {
      poly->m_vertices [i] = m_vertices [i];
      poly->m_normals [i] = m_normals [i];
    }
    poly->m_vertexCount = m_vertexCount;
    return poly;
  }

  void PolygonCollider2D::Serialize (Framework::Serializer::DataNode* data)
  {
    glm::vec2 dimension;
    Serializer::DataNode* value = data->FindElement(data, "Dimensions");
    value->GetValue(&dimension);
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    dimensions.x = tform->GetScale ().x * dimension.x;
    dimensions.y = tform->GetScale ().y * dimension.y;

    value->FindElement (data, "Orientation")->GetValue (&orientation);
    //TODO: Possible orientation fix,research more
    //orientation += tform->GetRotation();
  }

  void PolygonCollider2D::CenterPolygon()
  {
    vec3 c = GetCenter();
    // Translate vertices to centroid (make the centroid (0, 0)
    // for the polygon in model space)
    // Not floatly necessary, but I like doing this anyway
    for (unsigned i = 0; i < m_vertexCount; ++i)
      m_vertices[i] -= c;
  }

  void PolygonCollider2D::Initialize ()
  {
    SetBox (dimensions.x, dimensions.y);
    SetOrient (orientation);
  }

	vec3 PolygonCollider2D::GetSupport (const vec3& dir)
	{
	  float bestProjection = -FLT_MAX;
	  vec3 bestVertex;
	
	  for (unsigned i = 0; i < m_vertexCount; ++i)
	  {
	    vec3 v = m_vertices [i];
	    float projection = glm::dot(v, dir);
	
	    if (projection > bestProjection)
	    {
	      bestVertex = v;
	      bestProjection = projection;
	    }
	  }
	
	  return bestVertex;
	}
	
	void PolygonCollider2D::Set (vec3 *vertices, unsigned count)
	{
	  // No hulls with less than 3 vertices (ensure actual polygon)
	  assert (count > 2 && count <= MaxVertices);
	  count = (std::min)((int) count, MaxVertices);
	
	  // Find the right most point on the hull
	  int rightMost = 0;
	  float highestXCoord = vertices [0].x;
	  for (unsigned i = 1; i < count; ++i)
	  {
	    float x = vertices [i].x;
	    if (x > highestXCoord)
	    {
	      highestXCoord = x;
	      rightMost = i;
	    }
	
	    // If matching x then take farthest negative y
	    else if (x == highestXCoord)
	    if (vertices [i].y < vertices [rightMost].y)
	      rightMost = i;
	  }
	
	  int hull [MaxVertices];
	  int outCount = 0;
	  int indexHull = rightMost;
	
	  for (;;)
	  {
	    hull [outCount] = indexHull;
	
	    // Search for next index that wraps around the hull
	    // by computing cross products to find the most counter-clockwise
	    // vertex in the set, given the previos hull index
	    int nextHullIndex = 0;
	    for (int i = 1; i < (int) count; ++i)
	    {
	      // Skip if same coordinate as we need three unique
	      // points in the set to perform a cross product
	      if (nextHullIndex == indexHull)
	      {
	        nextHullIndex = i;
	        continue;
	      }
	
	      // Cross every set of three unique vertices
	      // Record each counter clockwise third vertex and add
	      // to the output hull
	      vec3 e1 = vertices [nextHullIndex] - vertices [hull [outCount]];
	      vec3 e2 = vertices [i] - vertices [hull [outCount]];
	      float c = Cross (e1, e2);
	      if (c < 0.0f)
	        nextHullIndex = i;
	
	      // Cross product is zero then e vectors are on same line
	      // therefor want to record vertex farthest along that line
	      if (c == 0.0f && glm::length(e2) > glm::length(e1))
	        nextHullIndex = i;
	    }
	
	    ++outCount;
	    indexHull = nextHullIndex;
	
	    // Conclude algorithm upon wrap-around
	    if (nextHullIndex == rightMost)
	    {
	      m_vertexCount = outCount;
	      break;
	    }
	  }
	
	  // Copy vertices into shape's vertices
	  for (unsigned i = 0; i < m_vertexCount; ++i)
	    m_vertices [i] = vertices [hull [i]];
	
	  // Compute face normals
	  for (unsigned i1 = 0; i1 < m_vertexCount; ++i1)
	  {
	    unsigned i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
	    vec3 face = m_vertices [i2] - m_vertices [i1];
	
	    // Ensure no zero-length edges, because that's bad
	    assert (glm::length(face) > EPSILON);
	
	    // Calculate normal with 2D cross product between vector and scalar
	    m_normals [i1] = glm::normalize(vec3(face.y, -face.x, 0));
	  }
	}
	
	void PolygonCollider2D::SetBox (float hw, float hh)
	{
	  m_vertexCount = 4;
    m_vertices[0] = vec3(-hw, -hh, 1.0f);
    m_vertices[1] = vec3(hw, -hh, 1.0f);
    m_vertices[2] = vec3(hw, hh, 1.0f);
    m_vertices[3] = vec3(-hw, hh, 1.0f);
	  m_normals[0] = vec3(0.0f, -1.0f, 0.0f);
    m_normals[1] = vec3(1.0f, 0.0f, 0.0f);
    m_normals[2] = vec3(0.0f, 1.0f, 0.0f);
    m_normals[3] = vec3(-1.0f, 0.0f, 0.0f);
	}
	
	ShapeCollider2D::ColliderType PolygonCollider2D::GetType (void) const
	{
	  return ePoly;
	}
	
	void PolygonCollider2D::Draw (void) const
	{
    RigidBody2D* rigidBody = static_cast<RigidBody2D*>(gameObject->GetComponent("RigidBody2D"));
    if (rigidBody)
    {
	    glColor3f (rigidBody->r, rigidBody->g, rigidBody->b);
	    glBegin (GL_LINE_LOOP);
	    for (unsigned i = 0; i < m_vertexCount; ++i)
	    {
	      vec3 v = rigidBody->position + u * m_vertices [i];
	      glVertex2f (v.x, v.y);
	    }
	    glEnd ();
    }
	}
	
	void PolygonCollider2D::SetOrient (float radians)
  {
    u = glm::mat3();
    /*
    float c = std::cos(radians);
    float s = std::sin(radians);

    m00 = c; m01 = -s;
    m10 = s; m11 = c;
    */
	}

  float PolygonCollider2D::GetArea()
  {
    float area = 0.0f;

    for (unsigned i1 = 0; i1 < m_vertexCount; ++i1)
    {
      // Triangle vertices, third vertex implied as (0, 0)
      vec3 p1(m_vertices[i1]);
      unsigned i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
      vec3 p2(m_vertices[i2]);

      float D = Cross(p1, p2);
      float triangleArea = 0.5f * D;

      area += triangleArea;
    }

    return area;
  }

  vec3 PolygonCollider2D::GetCenter()
  {
    // Calculate centroid and moment of interia
    vec3 c; // centroid
    const float k_inv3 = 1.0f / 3.0f;

    for (unsigned i1 = 0; i1 < m_vertexCount; ++i1)
    {
      // Triangle vertices, third vertex implied as (0, 0)
      vec3 p1(m_vertices[i1]);
      unsigned i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
      vec3 p2(m_vertices[i2]);

      float D = Cross(p1, p2);
      float triangleArea = 0.5f * D;
      // Use area to weight the centroid average, not just vertex position
      c += triangleArea * k_inv3 * (p1 + p2);
    }

    //Normalizes the center to be that for a unit polygon(max extent of 1)
    c *= 1.0f / GetArea();

    return c;
  }

  float PolygonCollider2D::ComputeInertia(float density)
  {
	  // Calculate centroid and moment of interia
	  float I = 0.0f;
	  const float k_inv3 = 1.0f / 3.0f;
	
	  for (unsigned i1 = 0; i1 < m_vertexCount; ++i1)
	  {
	    // Triangle vertices, third vertex implied as (0, 0)
	    vec3 p1 (m_vertices [i1]);
	    unsigned i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
	    vec3 p2 (m_vertices [i2]);

      float D = Cross(p1, p2);
	    float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
	    float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
	    I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	  }

    return I;
	}

  CompoundCollider2D::CompoundCollider2D() : ShapeCollider2D()
  {
  }

  CompoundCollider2D::~CompoundCollider2D()
  {
    for (auto i : childColliders)
    {
      if (i)
      {
        delete i;
        i = nullptr;
      }
    }
    childColliders.clear();
  }

  ShapeCollider2D* CompoundCollider2D::Clone(void) const
  {
    return new CompoundCollider2D(*this);
  }

  //TODO Finish the following three methods
  void CompoundCollider2D::Initialize()
  {

  }

  void CompoundCollider2D::Serialize(Serializer::DataNode* data)
  {

  }

  void CompoundCollider2D::ComputeMass(float density)
  {
      
  }

  void CompoundCollider2D::SetOrient(float radians)
  {
    vec3 offset;
    Transform* tform;
    for (auto c : childColliders)
    {
      tform = static_cast<Transform*>(c->gameObject->GetComponent("Transform"));
      offset = c->GetOffset();
      tform->Translate(-offset);
      c->SetOrient(radians);
      tform->Translate(offset);
    }
  }

  ShapeCollider2D::ColliderType CompoundCollider2D::GetType(void) const
  {
    return eCompound;
  }

  float CompoundCollider2D::GetArea()
  {
    float sumArea = 0.0f;

    for (auto c : childColliders)
    {
      sumArea += c->GetArea();
    }

    return sumArea;
  }

  vec3 CompoundCollider2D::GetCenter()
  {
    vec3 sumPos;
    
    for (auto c : childColliders)
    {
      sumPos += c->GetCenter();
    }

    return sumPos / (float)childColliders.size();

  }

  void CompoundCollider2D::AddCollider(ShapeCollider2D* newCollider)
  {
    newCollider->gameObject = gameObject;
    childColliders.push_back(newCollider);
    RigidBody2D* rb = static_cast<RigidBody2D*>(gameObject->GetComponent("RigidBody2D"));
    if (rb)
      rb->ComputeMass();
  }

  void CompoundCollider2D::RemoveCollider(ShapeCollider2D* oldCollider)
  {
    bool found = false;
    for (vector<ShapeCollider2D*>::iterator c = childColliders.begin(); c != childColliders.end() && !found; ++c)
    {
      if (*c == oldCollider)
      {
        (*c)->gameObject = nullptr;
        childColliders.erase(c);
        found = true;
      }
    }
    delete oldCollider;
    oldCollider = nullptr;

    RigidBody2D* rb = static_cast<RigidBody2D*>(gameObject->GetComponent("RigidBody2D"));
    rb->ComputeMass();
  }

  void CompoundCollider2D::Draw() const
  {
    for (auto c : childColliders)
    {
      switch (c->GetType())
      {
        case eCircle:
          static_cast<CircleCollider2D*>(c)->Draw();
          break;
        
        case ePoly:
          static_cast<PolygonCollider2D*>(c)->Draw();
          break;

        case eCompound:
          static_cast<CompoundCollider2D*>(c)->Draw();
          break;

        default:
          return;
          break;
      }
    }
  }
}
