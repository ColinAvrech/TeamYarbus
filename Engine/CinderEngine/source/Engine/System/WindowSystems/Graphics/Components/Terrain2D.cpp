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
#include "TerrainCreator.h"
#include "random.hpp"

namespace Framework
{
  DefineComponentName (Terrain2D);
  Procedural::TerrainCreator* tc;
  // Constructor
  Terrain2D::Terrain2D ()
  {}
  
  // Destructor
  Terrain2D::~Terrain2D ()
  {
    delete vao, vbo, tc;
  }


  void Terrain2D::Serialize (Serializer::DataNode* data)
  {
  }

  void Terrain2D::Initialize ()
  {
    WindowSystem::terrain = this;

    Generate_Height_Points ();
    Generate_Edges ();
    Generate_Vertices ();
    Generate_Buffers ();
  }


  void Terrain2D::Render ()
  {
    shader->Use ();
    vao->bindVAO ();
    shader->enableVertexAttribArray (shader->attribLocation ("position"));

    glDrawArrays (GL_TRIANGLES, 0, vertices.size () / 2);

    vao->unbindVAO ();
    shader->Disable ();
  }

  std::vector <std::pair <glm::vec2, glm::vec2>>& Terrain2D::Get_Edges()
{
    return edges;
  }

  void Terrain2D::Generate_Height_Points ()
  {
    tc = new Procedural::TerrainCreator (25, 25, 5);
    Procedural::TerrainCreator& t = *tc;
    int** Map = t.GetMap ();

    float offsetX = -1.0f;
    float offsetY = -1.0f;
    float nX = 2.f / (t.Get_Width () - 1);
    float nY = 2.f / (t.Get_Height () - 1);
    float previousHeight = -1.f;

    for (int i = 0; i < t.Get_Height (); ++i)
    {
      for (int j = 0; j < t.Get_Width (); ++j)
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
      offsetX += nX;
    }
  }

  void Terrain2D::Generate_Edges ()
  {
    // Edges for Line Colliders
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      edges.push_back (std::make_pair (height_points [i], height_points [i + 1]));
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