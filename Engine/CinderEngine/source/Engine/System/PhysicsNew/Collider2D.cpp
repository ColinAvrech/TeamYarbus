/******************************************************************************/
/*!
\file   Collider2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "GameObject.h"
#include "Collider2D.h"
#include "PhysicsSystemNew.h"

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
    PHYSICS->Remove (rigidBody);
    delete rigidBody;
    rigidBody = nullptr;
    if (gameObject != nullptr)
    {
      gameObject->ShapeCollider2D = nullptr;
    }
  }

  void ShapeCollider2D::SerializeMaterial (std::string name)
  {
    //opens file
    Serializer::DataNode* temp;
    Serializer::ZeroSerializer materialtype;
    name = Serializer::SkipHash (name.c_str ());
    materialtype.open (name.c_str ());
    materialtype.CreateArchive ();
    temp = materialtype.GetTrunk ()->branch;

    Serializer::DataNode* temp2;
    temp2 = temp->FindElement (temp, "Restitution");
    temp2->GetValue (&Bounciness);

    temp2 = temp->FindElement (temp, "StaticFriction");
    temp2->GetValue (&StaticFriction);

    temp2 = temp->FindElement (temp, "DynamicFriction");
    temp2->GetValue (&DynamicFriction);

    temp2 = temp->FindElement (temp, "Density");
    temp2->GetValue (&Density);

    materialtype.close ();
  }

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // CIRCLE COLLIDER 2D IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////

  CircleCollider2D::CircleCollider2D (float r)
  {
    radius = r;
  }

  // First and last use of CS225
  ShapeCollider2D * CircleCollider2D::Clone (void) const
  {
    return new CircleCollider2D (radius);
  }

  CircleCollider2D::~CircleCollider2D ()
  {
    rigidBody = nullptr;
  }

  void CircleCollider2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Radius");
    value->GetValue (&radius);
    radius *= gameObject->Transform->GetScale ().x;

    value = data->FindElement (data, "Static");
    value->GetValue (&isStatic);

    std::string Mat;
    value = data->FindElement (data, "Material");
    value->GetValue (&Mat);

    if (value->FindElement (data, "Velocity") != nullptr)
    {
      value->GetValue (&velocity);
    }

    SerializeMaterial (Mat);
  }

  void CircleCollider2D::Initialize ()
  {
    gameObject->ShapeCollider2D = this;
    RigidBody2D* b = PHYSICS->Add (this, gameObject->Transform->GetPosition ().x, gameObject->Transform->GetPosition ().y);
    rigidBody = b;
    b->gameObject = this->gameObject;
    gameObject->RigidBody2D = b;
    b->velocity = Vector2 (velocity.x, velocity.y);
    b->restitution = Bounciness;
    b->dynamicFriction = DynamicFriction;
    b->staticFriction = StaticFriction;
    ComputeMass (Density);

    if (isStatic)
    {
      b->SetStatic ();
    }
  }

  void CircleCollider2D::Draw (void) const
  {
    const unsigned k_segments = 20;

    // Render a circle with a bunch of lines
    glColor3f (1, 0, 0);
    glBegin (GL_LINE_LOOP);
    float theta = rigidBody->orient;
    float inc = PI * 2.0f / (float) k_segments;
    for (unsigned i = 0; i < k_segments; ++i)
    {
      theta += inc;
      Vector2 p (std::cos (theta), std::sin (theta));
      p *= radius;
      p += rigidBody->position;
      glVertex2f (p.x, p.y);
    }
    glEnd ();

    // Render line within circle so orientation is visible
    glBegin (GL_LINE_STRIP);
    Vector2 r (0, 1.0f);
    float c = std::cos (rigidBody->orient);
    float s = std::sin (rigidBody->orient);
    r.Set (r.x * c - r.y * s, r.x * s + r.y * c);
    r *= radius;
    r = r + rigidBody->position;
    glVertex2f (rigidBody->position.x, rigidBody->position.y);
    glVertex2f (r.x, r.y);
    glEnd ();
  }

  void CircleCollider2D::SetOrient (float radians)
  {

  }

  void CircleCollider2D::ComputeMass (float density)
  {
    rigidBody->m = PI * radius * radius * density;
    rigidBody->im = (rigidBody->m) ? 1.0f / rigidBody->m : 0.0f;
    rigidBody->I = rigidBody->m * radius * radius;
    rigidBody->iI = (rigidBody->I) ? 1.0f / rigidBody->I : 0.0f;
  }

  ShapeCollider2D::Type CircleCollider2D::GetType (void) const
  {
    return eCircle;
  }

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // POLYGON COLLIDER 2D IMPLEMENTATION
  //////////////////////////////////////////////////////////////////////////

  PolygonCollider2D::PolygonCollider2D ()
  {}

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

  PolygonCollider2D::~PolygonCollider2D ()
  {
  }

  void PolygonCollider2D::Serialize (Framework::Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Static");
    value->GetValue (&isStatic);

    std::string Mat;
    value = data->FindElement (data, "Material");
    value->GetValue (&Mat);
    SerializeMaterial (Mat);

    glm::vec2 dimension;
    value->FindElement (data, "Dimensions")->GetValue (&dimension);
    dimensions.x = gameObject->Transform->GetScale ().x * dimension.x;
    dimensions.y = gameObject->Transform->GetScale ().y * dimension.y;

    value->FindElement (data, "Orientation")->GetValue (&orientation);
  }

  void PolygonCollider2D::Initialize ()
  {
    SetBox (dimensions.x, dimensions.y);
    SetOrient (orientation);
    gameObject->ShapeCollider2D = this;
    RigidBody2D* b = PHYSICS->Add (this, gameObject->Transform->GetPosition ().x, gameObject->Transform->GetPosition ().y);
    rigidBody = b;
    b->gameObject = this->gameObject;
    gameObject->RigidBody2D = b;
    b->restitution = Bounciness;
    b->dynamicFriction = DynamicFriction;
    b->staticFriction = StaticFriction;
    //ComputeMass (Density);

    if (isStatic)
    {
      b->SetStatic ();
    }
  }

	Vector2 PolygonCollider2D::GetSupport (const Vector2& dir)
	{
	  float bestProjection = -FLT_MAX;
	  Vector2 bestVertex;
	
	  for (unsigned i = 0; i < m_vertexCount; ++i)
	  {
	    Vector2 v = m_vertices [i];
	    float projection = Dot (v, dir);
	
	    if (projection > bestProjection)
	    {
	      bestVertex = v;
	      bestProjection = projection;
	    }
	  }
	
	  return bestVertex;
	}
	
	void PolygonCollider2D::Set (Vector2 *vertices, unsigned count)
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
	      Vector2 e1 = vertices [nextHullIndex] - vertices [hull [outCount]];
	      Vector2 e2 = vertices [i] - vertices [hull [outCount]];
	      float c = Cross (e1, e2);
	      if (c < 0.0f)
	        nextHullIndex = i;
	
	      // Cross product is zero then e vectors are on same line
	      // therefor want to record vertex farthest along that line
	      if (c == 0.0f && e2.LenSqr () > e1.LenSqr ())
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
	    Vector2 face = m_vertices [i2] - m_vertices [i1];
	
	    // Ensure no zero-length edges, because that's bad
	    assert (face.LenSqr () > EPSILON * EPSILON);
	
	    // Calculate normal with 2D cross product between vector and scalar
	    m_normals [i1] = Vector2 (face.y, -face.x);
	    m_normals [i1].Normalize ();
	  }
	}
	
	void PolygonCollider2D::SetBox (float hw, float hh)
	{
	  m_vertexCount = 4;
	  m_vertices [0].Set (-hw, -hh);
	  m_vertices [1].Set (hw, -hh);
	  m_vertices [2].Set (hw, hh);
	  m_vertices [3].Set (-hw, hh);
	  m_normals [0].Set (0.0f, -1.0f);
	  m_normals [1].Set (1.0f, 0.0f);
	  m_normals [2].Set (0.0f, 1.0f);
	  m_normals [3].Set (-1.0f, 0.0f);
	}
	
	ShapeCollider2D::Type PolygonCollider2D::GetType (void) const
	{
	  return ePoly;
	}
	
	void PolygonCollider2D::Draw (void) const
	{
	  glColor3f (rigidBody->r, rigidBody->g, rigidBody->b);
	  glBegin (GL_LINE_LOOP);
	  for (unsigned i = 0; i < m_vertexCount; ++i)
	  {
	    Vector2 v = rigidBody->position + u * m_vertices [i];
	    glVertex2f (v.x, v.y);
	  }
	  glEnd ();
	}
	
	void PolygonCollider2D::SetOrient (float radians)
	{
	  u.Set (radians);
	}
	
	void PolygonCollider2D::ComputeMass (float density)
	{
	  // Calculate centroid and moment of interia
	  Vector2 c (0.0f, 0.0f); // centroid
	  float area = 0.0f;
	  float I = 0.0f;
	  const float k_inv3 = 1.0f / 3.0f;
	
	  for (unsigned i1 = 0; i1 < m_vertexCount; ++i1)
	  {
	    // Triangle vertices, third vertex implied as (0, 0)
	    Vector2 p1 (m_vertices [i1]);
	    unsigned i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
	    Vector2 p2 (m_vertices [i2]);
	
	    float D = Cross (p1, p2);
	    float triangleArea = 0.5f * D;
	
	    area += triangleArea;
	
	    // Use area to weight the centroid average, not just vertex position
	    c += triangleArea * k_inv3 * (p1 + p2);
	
	    float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
	    float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
	    I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	  }
	
	  c *= 1.0f / area;
	
	  // Translate vertices to centroid (make the centroid (0, 0)
	  // for the polygon in model space)
	  // Not floatly necessary, but I like doing this anyway
	  for (unsigned i = 0; i < m_vertexCount; ++i)
	    m_vertices [i] -= c;
	
	  rigidBody->m = density * area;
	  rigidBody->im = (rigidBody->m) ? 1.0f / rigidBody->m : 0.0f;
	  rigidBody->I = I * density;
	  rigidBody->iI = rigidBody->I ? 1.0f / rigidBody->I : 0.0f;
	}

}
