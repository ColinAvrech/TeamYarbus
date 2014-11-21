/******************************************************************************/
/*!
\file   Terrain2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Terrain2D.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Thermodynamics.h"
#include "IncludeForAllCollision.h"
#include "PhysicsSystem.h"
#include "random.hpp"

namespace Framework
{
  static std::vector <float> lineVertices;
  static VAO* vao1;
  static VBO* vbo1;
  static SplineCollider* spline;
  DefineComponentName (Terrain2D);

  // Constructor
  Terrain2D::Terrain2D ()
  {}
  
  // Destructor
  Terrain2D::~Terrain2D ()
  {
    delete vao, vbo, tc, spline;
    delete vao1, vbo1;
  }


  void Terrain2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement(data, "MapSize");
    value->GetValue (&MapSize);

    value = data->FindElement (data, "BaseHeight");
    value->GetValue (&BaseHeight);

    value = data->FindElement (data, "PeakHeight");
    value->GetValue (&PeakHeight);

    value = data->FindElement (data, "Passes");
    value->GetValue (&Passes);

    value = data->FindElement (data, "Waves");
    value->GetValue (&Waves);

    value = data->FindElement(data, "Water");
    value->GetValue(&WaterDepth);

    value = data->FindElement(data, "AddCollider");
    value->GetValue(&AddCollider);
  }

  void Terrain2D::Initialize ()
  {
    Generate_Height_Points ();
    Generate_Edges ();
    Generate_Vertices ();
    Generate_Buffers ();

    if (AddCollider)
    {
      spline = new SplineCollider ();
      spline->gameObject = this->gameObject;
      Physics::PHYSICSSYSTEM->SplineColliders.push_back (spline);
      spline->AddLineCollider (edges);
    }

    vao1 = new VAO ();
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      lineVertices.push_back (height_points [i].x);
      lineVertices.push_back (height_points [i].y);
      lineVertices.push_back (height_points [i + 1].x);
      lineVertices.push_back (height_points [i + 1].y);
    }

    vbo1 = new VBO (lineVertices.size () * sizeof (float), lineVertices.data ());
    GLint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);
    vao1->unbindVAO ();
  }


  void Terrain2D::Draw ()
  {
    shader->Use ();
    vao->bindVAO ();
    shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix()));
    shader->uni4f ("color", 0.25f, 0.25f, 0.25f, 1.0f);
    //shader->enableVertexAttribArray (shader->attribLocation ("position"));

    glDrawArrays (GL_TRIANGLES, 0, vertices.size () / 2);

    vao->unbindVAO ();

    vao1->bindVAO ();
    shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
    shader->uni4f ("color", 1, 1, 1, 1.0f);
    //glDrawArrays (GL_LINES, 0, lineVertices.size () / 2);
    vao1->unbindVAO ();
    shader->Disable ();

    std::vector <LineCollider*>& c = spline->Get_Colliders ();

    //for (unsigned i = 0; i < height_points.size () - 1; ++i)
    //{
    //  c [i]->p1 = (glm::mat3)gameObject->Transform->GetModelViewProjectionMatrix () * glm::vec3 (height_points[i], 0.0f);
    //  c [i]->p2 = (glm::mat3)gameObject->Transform->GetModelViewProjectionMatrix () * glm::vec3 (height_points [i + 1], 0.0f);
    //}
  }

  std::vector <std::pair <vec2, vec2>>& Terrain2D::Get_Edges()
{
    return edges;
  }

  void Terrain2D::Generate_Height_Points ()
  {
    tc = new Procedural::TerrainCreator (MapSize, MapSize, BaseHeight, Passes, Waves, PeakHeight, WaterDepth);
    Procedural::TerrainCreator& t = *tc;
    int** Map = t.GetMap ();

    float offsetX = -1.0f;
    float offsetY = -1.0f;
    float nX = 2.f / (t.Get_Width () - 1);
    float nY = 2.f / (t.Get_Height () - 1);
    float previousHeight = -1.f;

    for (int i = 0; i < t.Get_Height (); ++i)
    {
      for (int j = 0; j < t.Get_Width (); j += 4)
      {
        if (Map [i][j] == 0)
        {
          /*height_points.push_back ({ offsetX, offsetY });
          offsetY = -1.0f;
          break;*/
          if (previousHeight != offsetY || j == t.Get_Width() - 1)
          {
            height_points.push_back({ offsetX, offsetY });
            previousHeight = offsetY;
          }
          offsetY = -1.0f;
          break;
        }
        offsetY += nY;
      }
      offsetX += 4 * nX;
    }
  }

  void Terrain2D::Generate_Edges ()
  {
    // Edges for Line Colliders
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      edges.push_back
        (
        std::make_pair
        (
        height_points [i],
        height_points [i + 1]
        )
        );
    }
  }

  void Terrain2D::Generate_Vertices ()
  {
    float y = -1.0f;
    // Vertices
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      // Triangle 1
      vertices.push_back (height_points [i].x);
      vertices.push_back (y);
      vertices.push_back (height_points [i + 1].x);
      vertices.push_back (y);
      vertices.push_back (height_points [i].x);
      vertices.push_back (height_points [i].y);
      // Triangle 2
      vertices.push_back (height_points [i + 1].x);
      vertices.push_back (height_points [i + 1].y);
      vertices.push_back (height_points [i].x);
      vertices.push_back (height_points [i].y);
      vertices.push_back (height_points [i + 1].x);
      vertices.push_back (y);
    }
  }

  void Terrain2D::Generate_Buffers ()
  {
    vao = new VAO ();
    shader = Resources::RS->Get_Shader ("Terrain");
    shader->Use ();

    vbo = new VBO (vertices.size () * sizeof (float), vertices.data ());
    GLuint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);

    vao->unbindVAO ();
  }

}