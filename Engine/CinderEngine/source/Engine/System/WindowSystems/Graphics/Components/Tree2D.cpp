/******************************************************************************/
/*!
\file   Tree2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Tree2D.h"
#include "WindowSystem.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "random.hpp"

namespace Framework
{
  DefineComponentName (Tree2D);

  static float myrand (float R)
  {
    return (2 * R * rand ()) / RAND_MAX - R;
  }


  // Constructor
  Tree2D::Tree2D ()
  {}
  
  // Destructor
  Tree2D::~Tree2D ()
  {
    IGraphicsObject::Deregister ();
  }

  void Tree2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Tree_Type");
    value->GetValue ((int*)&type);

    value = data->FindElement (data, "Color");
    value->GetValue (&color);
  }

  void Tree2D::Initialize ()
  {
    IGraphicsObject::Register ();

    switch (type)
    {
    case Framework::TREE_0:
      Make_Tree0 (0, -0.1f, 0.1f, 1.5, 10);
      break;
    case Framework::TREE_1:
      Make_Tree1 (0, -0.5f, 0.0f, 0.25f, 45, 5, 10);
      break;
    default:
      break;
    }

    shader = Resources::RS->Get_Shader ("Terrain");
    vao = new VAO ();
    vbo = new VBO (treeMesh.size () * sizeof (float), treeMesh.data ());
    GLint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);
    vao->unbindVAO ();
  }

  void Tree2D::Draw ()
  {
    shader->Use ();
    shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix()));
    shader->uni4fv ("color", glm::value_ptr(color));
    vao->bindVAO ();
    glDrawArrays (GL_LINES, 0, treeMesh.size () / 2);
    vao->unbindVAO ();
    shader->Disable ();
  }

  void Tree2D::Make_Tree0 (float x1, float y1, float length1, float angle1, int depth)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length1 * cos (angle1);
      float y2 = y1 + length1 * sin (angle1);

      treeMesh.push_back (x1);
      treeMesh.push_back (y1);
      treeMesh.push_back (x2);
      treeMesh.push_back (y2);

      float length2 = length1 * (SCALE + myrand (RAND));
      float angle2 = angle1 + ANGLE + myrand (RAND);
      int factor = 80 + rand() % 20;
      float f = factor / 100.f;
      int fork = rand() % 100;
      if (fork > 30)
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1);
      length2 = length1 * (SCALE + myrand (RAND));
      angle2 = angle1 - ANGLE + myrand (RAND);
      fork = rand() % 100;
      if (fork > 30)
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1);
    }
  }


  void Tree2D::Make_Tree1 (float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
  {
    treeMesh.push_back (x1);
    treeMesh.push_back (y1);
    treeMesh.push_back (x2);
    treeMesh.push_back (y2);

    if (depth < 1)
    {
      return;
    }

    float treeRatio = glm::linearRand (0.5f, 0.8f);
    int nn = depth - 1;
    GLfloat x3 = (x2 - x1)*treeRatio + x1 - x2;
    GLfloat y3 = (y2 - y1)*treeRatio + y1 - y2;
    if (branchCount == 2)
    {
      // Right Branch
      Make_Tree1 (x2, y2, x3 * cos (angle) + y3 * sin (angle) + x2, -x3 * sin (angle) + y3 * cos (angle) + y2, angle, nn, branchCount);
      // Left Branch
      Make_Tree1 (x2, y2, x3 * cos (-angle) + y3 * sin (-angle) + x2, -x3 * sin (-angle) + y3 * cos (-angle) + y2, angle, nn, branchCount);
    }
    else
    {
      GLfloat nowAngle = -angle;
      GLfloat angleTone = angle / (branchCount - 1) * 2;
      for (int i = 0; i < branchCount; i++, nowAngle += angleTone)
      {
        Make_Tree1 (x2, y2, x3 * cos (nowAngle) + y3 * sin (nowAngle) + x2, -x3 * sin (nowAngle) + y3 * cos (nowAngle) + y2, angle, nn, branchCount);
      }
    }
  }

}