#ifndef _COLLIDER_2D_H
#define _COLLIDER_2D_H

#include "RigidBody2D.h"
#include <algorithm>
#include "Component.h"

#define MaxVertices 64

namespace Framework
{
	class ShapeCollider2D : public Component
	{
	public:
	  const static std::string Name;
	  enum Type
	  {
	    eCircle,
	    ePoly,
	    eCount
	  };
	
    ShapeCollider2D ();
    virtual ~ShapeCollider2D ();
	
	  virtual void Serialize (Serializer::DataNode* data){}
    void SerializeMaterial (std::string name);
    virtual ShapeCollider2D *Clone (void) const
    {
      return nullptr;
    }
	  virtual void Initialize (void){}
    virtual void ComputeMass (float density) {}
    virtual void SetOrient (float radians){}
    virtual void Draw (void) const{}
    virtual Type GetType (void) const
    {
      return (Type)0;
    }
	
	  RigidBody2D *rigidBody;
	
	  // For circle shape
	  float radius;
    bool isStatic;
    float Density;
    float StaticFriction;
    float DynamicFriction;
    float Bounciness;
	
	  // For Polygon shape
	  Mat2 u; // Orientation matrix from model to world
	};
	
	class CircleCollider2D : public ShapeCollider2D
	{
	public:
	  const static std::string Name;
	  CircleCollider2D () {}
    virtual ~CircleCollider2D ();
	  CircleCollider2D( float r );
	
	  ShapeCollider2D *Clone( void ) const;
	
	  virtual void Serialize (Serializer::DataNode* data);
	  virtual void Initialize ();
	
	  void ComputeMass( float density );
	
	  void SetOrient( float radians );
	
	  void Draw( void ) const;
	
	  Type GetType( void ) const;
	};
	
	class PolygonCollider2D : public ShapeCollider2D
	{
	public:
	  const static std::string Name;
    PolygonCollider2D ();
    virtual ~PolygonCollider2D ();
	  virtual void Serialize (Serializer::DataNode* data);
	
	  virtual void Initialize( void );
	
	  ShapeCollider2D *Clone( void ) const;
	  void ComputeMass( float density );
	  void SetOrient( float radians );
	  void Draw( void ) const;

	  Type GetType( void ) const;
	
	  // Half width and half height
	  void SetBox( float hw, float hh );
	
	  void Set( Vector2 *vertices, unsigned count );
	
	  Vector2 GetSupport( const Vector2& dir );
	
	  unsigned m_vertexCount;
	  Vector2 m_vertices[MaxVertices];
	  Vector2 m_normals[MaxVertices];
	};
}


#endif // _COLLIDER_2D_H
