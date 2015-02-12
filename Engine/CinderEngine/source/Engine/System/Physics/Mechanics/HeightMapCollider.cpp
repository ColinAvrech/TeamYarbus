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
#include "HeightMapCollider.h"

namespace Framework
{
  HeightMapCollider::HeightMapCollider()
  {
    gameObject = nullptr;
  }

  HeightMapCollider::~HeightMapCollider()
  {
    t2d = nullptr;
  }

  void HeightMapCollider::Serialize(Serializer::DataNode* data)
  {
    //nothing to serialize
  }

  ShapeCollider2D *HeightMapCollider::Clone(void) const
  {
    return nullptr;
  }

  void HeightMapCollider::Initialize(void)
  {
    t2d = gameObject->C<Terrain2D>();
    numHeightPoints = t2d->GetWidth();
    normalized_area = this->ComputeArea();
    ComputeBounds();
  }

  void HeightMapCollider::ComputeBounds()
  {
    Transform *t = gameObject->C<Transform>();
    h_bounds[0] = t->GetPosition().x - t->GetScale().x;
    h_bounds[1] = t->GetPosition().x + t->GetScale().x;
  }

  float HeightMapCollider::ComputeArea()
  {
    float area = 0.f;
    /////////////////
    float nX = 2.f / (t2d->GetWidth() - 1);
    float nY = 2.f / (t2d->GetPeakHeight() - 1);
    const float *t = t2d->GetTerrain();
    ///////////////////
    for (int i = 0; i < numHeightPoints - 1; ++i)
    {
      float a = (t[i] * nY) / 2.f;
      float b = (t[i + 1] * nY) / 2.f;
      float tr_area = (a + b) / 2.f * nX;
      area += tr_area;
    }
    return area;
  }

  void HeightMapCollider::SetOrient(float radians)
  {
    
  }

  void HeightMapCollider::Draw(void) const
  {

  }

  ShapeCollider2D::ColliderType HeightMapCollider::GetType(void) const
  {
    return eHeightMap;
  }

  float HeightMapCollider::GetArea()
  {
    Transform *t = gameObject->C<Transform>();
    return normalized_area * t->GetScale().x * t->GetScale().y;
  }

  vec3 HeightMapCollider::GetCenter()
  {
    return glm::vec3();
  }
}
