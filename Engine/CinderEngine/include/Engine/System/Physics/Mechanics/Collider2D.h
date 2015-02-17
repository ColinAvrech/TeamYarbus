/******************************************************************************/
/*!
\file   Collider2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _COLLIDER_2D_H
#define _COLLIDER_2D_H

#include "RigidBody2D.h"
#include <algorithm>
#include "Component.h"

#include "MetaCreator.h"

#define MaxVertices 64

namespace Framework
{
	class ShapeCollider2D : public Component
	{
  	public:
		  META_DECLARE( ShapeCollider2D );

	    const static std::string Name;
	    enum ColliderType
	    {
        eCircle,
        ePoly,
        eCompound,
        eHeightMap,
	      eCount,
        eInvalid
	    };
	
      ShapeCollider2D ();
      virtual ~ShapeCollider2D ();
	
	    virtual void Serialize (Serializer::DataNode* data){}
      virtual ShapeCollider2D *Clone (void) const
      {
        return nullptr;
      }
	    virtual void Initialize (void){}
      virtual void ComputeMass (float density) {}
      virtual void SetOrient (float radians){}
      virtual void Draw (void) const{}
      virtual ColliderType GetType (void) const
      {
        return eInvalid;
      }

      vec2 GetOffset() const{ return offset; }
      void SetOffset(const vec2 &newOffset){ offset = newOffset; }

      virtual float GetArea() const { return 0.0f; };
      virtual vec2 GetCenter() const;

    protected: 
      vec2 offset = vec2();
	};
	
	class CircleCollider2D : public ShapeCollider2D
	{
	  public:
		  META_DECLARE( CircleCollider2D );

	    CircleCollider2D () {}
      virtual ~CircleCollider2D () = default;
	    CircleCollider2D( float r );
	
	    ShapeCollider2D *Clone( void ) const;
	
	    virtual void Serialize (Serializer::DataNode* data);
	    virtual void Initialize ();
	
	    void Draw( void ) const;

      float GetArea() const;

      float GetRadius() const { return radius; }
      void SetRadius(const float& newrad);
	
      ColliderType GetType(void) const;

  private:
      float radius = 0;
	};
	
	class PolygonCollider2D : public ShapeCollider2D
	{
    public:
      META_DECLARE(PolygonCollider2D);

      PolygonCollider2D ();
      virtual ~PolygonCollider2D () = default;
	    virtual void Serialize (Serializer::DataNode* data);
	
	    virtual void Initialize( void );
	
	    ShapeCollider2D *Clone( void ) const;
	    void SetOrient( float radians );
	    void Draw( void ) const;
	    ColliderType GetType( void ) const;
	
	    // Half width and half height
	    void SetBox( float hw, float hh );
	
	    void Set( vec2 *vertices, unsigned count );
	
	    vec2 GetSupport( const vec2& dir );

      float GetArea() const;
      vec2 GetCenter() const;
      float ComputeInertia(float density);

      // For Polygon shape
      glm::mat2 u = glm::mat2(); // Orientation matrix from model to world
      vec2 dimensions = vec2();

      float orientation = 0;
	    unsigned m_vertexCount = 0;
	    vec2 m_vertices[MaxVertices];
	    vec2 m_normals[MaxVertices];

    private:
      void CenterPolygon();
	};

  class CompoundCollider2D : public ShapeCollider2D
  {
    public:
      META_DECLARE(CompoundCollider2D);
      CompoundCollider2D();
      ~CompoundCollider2D();

      void Serialize(Serializer::DataNode* data);
      ShapeCollider2D *Clone(void) const;
      void Initialize(void);
      void ComputeMass(float density);
      void SetOrient(float radians);
      ColliderType GetType(void) const;

      float GetArea() const;
      vec2 GetCenter() const;

      void AddCollider(ShapeCollider2D* newCollider);
      void RemoveCollider(ShapeCollider2D* oldCollider);

      void Draw(void) const;

      std::vector<ShapeCollider2D*> childColliders;
  };
}

META_DEFINE( Framework::ShapeCollider2D, ShapeCollider2D )
{

}

META_DEFINE( Framework::CircleCollider2D, CircleCollider2D )
{

}

META_DEFINE(Framework::PolygonCollider2D, PolygonCollider2D)
{

}

META_DEFINE(Framework::CompoundCollider2D, CompoundCollider2D)
{

}

#endif // _COLLIDER_2D_H
