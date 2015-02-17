/******************************************************************************/
/*!
\file   HeightMapCollider2D.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "RigidBody2D.h"
#include <algorithm>
#include "Component.h"

#include "MetaCreator.h"

#define MaxVertices 64

namespace Framework
{	
	class HeightMapCollider : public ShapeCollider2D
	{
	  public:
      META_DECLARE(HeightMapCollider);

      HeightMapCollider();
      virtual ~HeightMapCollider();
	
	    virtual void Serialize (Serializer::DataNode* data);
      virtual ShapeCollider2D *Clone(void) const;
      virtual void Initialize(void);
      virtual void ComputeInertia(float density){}
      virtual void SetOrient(float radians);
      virtual void Draw(void) const;
      virtual ColliderType GetType(void) const;
      virtual float GetArea() const;

  private:
    float x_offset = 0.f;
    float y_offset = 0.f;
    //const float *heightMap = nullptr;
    Terrain2D *t2d;
    int numHeightPoints = 0;
    float baseHeight = 0.f;
    float h_bounds[2];
    float normalized_area = 0.f;
    ///////////////////////////////
    float ComputeArea();
    void ComputeBounds();
	};
}
