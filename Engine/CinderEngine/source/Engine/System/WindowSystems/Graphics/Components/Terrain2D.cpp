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
#include "random.hpp"

namespace Framework
{
  DefineComponentName (Terrain2D);

  // Constructor
  Terrain2D::Terrain2D ()
  {}
  
  // Destructor
  Terrain2D::~Terrain2D ()
  {
    delete vao, vbo;
  }


  void Terrain2D::Serialize (Serializer::DataNode* data)
  {
  }

  void Terrain2D::Initialize ()
  {
    WindowSystem::terrain = this;
    float y = -1.0f;
    vao = new VAO ();
    shader = Resources::RS->Get_Shader ("Terrain");
    shader->Use ();

    // Hard Coded
    height_points.push_back ({ -1.0f, -0.9f });
    height_points.push_back ({ -0.8f, -0.7f });
    height_points.push_back ({ -0.6f, -0.8f });
    height_points.push_back ({ -0.5f, -0.9f });
    height_points.push_back ({ -0.2f, -0.8f });
    height_points.push_back ({ +0.0f, -0.8f });
    height_points.push_back ({ +0.6f, -0.3f });
    height_points.push_back ({ +0.8f, -0.6f });
    height_points.push_back ({ +1.0f, -0.9f });

    // Edges for Line Colliders
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      edges.push_back (std::make_pair (height_points [i], height_points [i + 1]));
    }

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

    vbo = new VBO (vertices.size () * sizeof (float), vertices.data ());
    GLuint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);

    vao->unbindVAO ();
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

}