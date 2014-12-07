/******************************************************************************/
/*!
\file   Tree2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "FractalGenerator.h"
#include "Tree2D.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "WindowSystem.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Thermodynamics.h"
#include "Pipeline.h"
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
  {
    tree = nullptr;
  }
  
  // Destructor
  Tree2D::~Tree2D ()
  {
    delete vao, vbo;
    delete tree;
    vao = nullptr;
    vbo = nullptr;
    tree = nullptr;
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
    FireGroup* fsm = nullptr;
    switch (type)
    {
    case Framework::TREE_0:
      Make_Tree0 (0, -0.1f, 0.1f, 1.5, 10);
      break;
    case Framework::TREE_1:
      Make_Tree1 (0, -0.5f, 0.0f, 0.25f, 45, 5, 3);
      break;
    case Framework::TREE_2:
      Make_Tree2 (0, -0.1f, 0.1f, 1.5, 15);
      break;
    case Framework::TREE_3:
      Make_Tree3 (0, -0.1f, 0.1f, 1.5, 15);
      break;
    case Framework::TREE_4:
      Make_Tree4 (0, -0.1f, 0.1f, 1.5, 5);
      break;
    case Framework::TREE_5:
      tree = new FractalGenerator ();
      tree->Generate_Tree ();
      shader = Resources::RS->Get_Shader ("Tree");
      tree->Create_Mesh (tree->getTotalLines (), &treeMesh, &edges);
      Generate_Buffers ();

      fsm = gameObject->FireGroup;
      if (fsm)
      {
        FireStarter* fs;;
        for (unsigned i = 0; i < edges.size (); ++i)
        {
          //set the pointer to a new firestarter with the offset position
          fs = new FireStarter(glm::mat2 (gameObject->Transform->GetModelMatrix ()) * edges.at (i), fsm);
          fs->manager = fsm;
          fs->material_type = GRASS;
          fs->initTemp = 400.0f;
          fsm->AddFireStarter(fs);
        }
      }
      break;

    case Framework::TREE_GRASS:
      Make_Grass (0, -0.1f, 0.1f);
      break;
    default:
      break;
    }

    if (fsm != nullptr && fsm->firePoints.size())
    {
      Physics::THERMODYNAMICS->Add_Group(fsm);
    }

    if (type != TREE_5)
    {
      shader = Resources::RS->Get_Shader ("Tree");
      vao = new VAO ();
      vbo = new VBO (treeMesh.size () * sizeof (float), treeMesh.data ());
      GLint posAttrib = shader->attribLocation ("position");
      shader->enableVertexAttribArray (posAttrib);
      shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), 0);
      vao->unbindVAO ();
    }
  }


  void Tree2D::Generate_Buffers ()
  {
    shader = Resources::RS->Get_Shader ("Tree");
    vao = new VAO ();
    vbo = new VBO (treeMesh.size () * sizeof (float), treeMesh.data ());
    GLint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof (float), 0);
    GLint colAttrib = shader->attribLocation ("color");
    shader->enableVertexAttribArray (colAttrib);
    shader->vertexAttribPtr (colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof (float), 2 * sizeof (float));
    vao->unbindVAO ();
  }

  void Tree2D::Draw ()
  {
    if (type == TREE_5)
    {
      ////glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
      //static bool newTree = false;
      //static int counter = 0;
      //static unsigned timer = unsigned (glfwGetTime () * 1000);

      //// Tree Growth
      //if (counter < tree->getTotalLines ())
      //{
      //  tree->Create_Mesh (counter);
      //  counter += int (0.005f * tree->getTotalLines ());
      //}
      //else
      //{
      //  if (!newTree)
      //    timer = unsigned (glfwGetTime () * 1000);
      //  tree->Create_Mesh (tree->getTotalLines ());
      //  newTree = true;
      //}

      //if (newTree && float (glfwGetTime ()) * 1000 - timer > 500)
      //{
      //  do
      //  {
      //    tree->Generate_Tree ();
      //  }
      //  while (0.0001*tree->getTotalLines () < 1);

      //  counter = 0;
      //  newTree = false;
      //}
      glLineWidth (7.0f);
      shader->Use ();
      shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
      vao->bindVAO ();
      //shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
      glDrawArrays (GL_LINE_STRIP, 0, treeMesh.size () / 6);
      vao->unbindVAO ();
      shader->Disable ();
      OPENGL->ResetBlendMode ();
    }
    else
    {
      shader->Use ();
      shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
      shader->uni4fv ("color", glm::value_ptr (color));
      vao->bindVAO ();
      glDrawArrays (GL_LINES, 0, treeMesh.size () / 2);
      vao->unbindVAO ();
      shader->Disable ();
    }
  }

  void Tree2D::Set(glm::vec4& _color, Tree_Type _type)
  {
    color = _color;
    type = _type;
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

      Add_Branch(x1, y1, x2, y2);

      float length2 = length1 * (SCALE + myrand (RAND));
      float angle2 = angle1 + ANGLE + myrand (RAND);
      int factor = 80 + rand () % 20;
      float f = factor / 100.f;
      int fork;
      if (depth % 2 != 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);
      if (fork > 20 - depth * 2)
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1);
      length2 = length1 * (SCALE + myrand (RAND));
      angle2 = angle1 - ANGLE + myrand (RAND);
      if (depth % 2 == 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);
      if (fork > 20 - depth * 2)
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1);
    }
  }


  void Tree2D::Make_Tree1 (float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
  {
    Add_Branch(x1, y1, x2, y2);

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

  void Tree2D::Make_Tree2(float x1, float y1, float length, float angle, int depth)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      Add_Branch(x1, y1, x2, y2);

      x1 = x1 + length / 2 * cos(angle);
      y1 = y1 + length / 2 * sin(angle);

      float length2 = length * (SCALE + myrand(RAND));
      float angle2;
      if (depth % 2 == 0)
        angle2 = angle + ANGLE + myrand(RAND);
      else
        angle2 = angle - ANGLE + myrand(RAND);

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;

      Make_Tree2(x2, y2, length2 * f, angle2, depth - 1);

      y2 = y1 + length * (2 * (SCALE + myrand(RAND))) * sin(-((15 - depth) / 10.f)*angle);
     
      x2 = x1 + length * (2 * (SCALE + 3*myrand(RAND))) * cos(angle - angle2);
      
      //Add_Branch(x1, y1, x2, y2);
      Make_Pine_Branch(x1, y1, length, angle - 1.2f * angle2, depth - 1, 1);

      x2 = x1 + length * (2 * (SCALE + 3*myrand(RAND))) * cos(angle + angle2);

      y2 = y1 + length * (2 * (SCALE + myrand(RAND))) * sin(-((15 - depth)/10.f)*angle);

      //Add_Branch(x1, y1, x2, y2);
      Make_Pine_Branch(x1, y1, length, angle + 1.2f * angle2, depth - 1, 0);
    }
  }

  void Tree2D::Make_Tree3(float x1, float y1, float length, float angle, int depth)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      Add_Branch(x1, y1, x2, y2);

      x1 = x1 + length / 2 * cos(angle);
      y1 = y1 + length / 2 * sin(angle);

      float length2 = length / 2;
      float angle2;
      if (depth % 2 == 0)
        angle2 = angle + ANGLE + myrand(RAND);
      else
        angle2 = angle - ANGLE + myrand(RAND);

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;

      Make_Tree3(x2, y2, length * f, angle2, depth - 1);

      int r = rand() % 100;
      if (r > 20 - depth)
        Make_Tree0(x2, y2, length2, angle/2 + angle2, depth/2);
      r = rand() % 100;
      if (r > 20 - depth)
        Make_Tree0(x2, y2, length2, -angle/2 + angle2, depth/2);
    }
  }

  void Tree2D::Make_Tree4(float x1, float y1, float length, float angle, int depth)
  {
    //Not yet come up with a type 4.
  }

  void Tree2D::Make_Grass(float x1, float y1, float length1)
  {
    float ANGLE = 0.2f;
    float RAND = 0.2f;
    int tuft = rand() % 10;
    if (tuft >= 7)
      Make_Grass_Stalk(x1, y1, 0.1f, 1.5f, 2 + rand() % 3, rand() % 2);
    float angle = 1.5f + ANGLE + myrand(RAND);
    float x = x1 - myrand(RAND);
    for (int i = 0; i < tuft - 1; ++i)
    {
      float angle2 = angle + i * myrand(RAND);
      float length2 = length1 * (1 + std::sin(i * 3.14f / (tuft - 2)));
      Make_Grass_Blade(x1, y1, length2, angle2);
      x1 += myrand(RAND) / (tuft/2);
    }
  }

  void Tree2D::Make_Grass_Blade(float x1, float y1, float length, float angle)
  {
    float x2 = x1 + length * cos(angle);
    float y2 = y1 + length * sin(angle);

    Add_Branch(x1, y1, x2, y2);
  }

  void Tree2D::Make_Grass_Stalk(float x1, float y1, float length, float angle, int depth, int curve)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.2f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      Add_Branch(x1, y1, x2, y2);

      x1 = x1 + length * cos(angle);
      y1 = y1 + length * sin(angle);
      float angle2;
      if (curve == 0)
        angle2 = angle + ANGLE + myrand(RAND);
      else
        angle2 = angle - ANGLE + myrand(RAND);

      Make_Grass_Stalk(x1, y1, length, angle2, depth - 1, curve);
    }
    else
    {
      Make_Stalk_Head(x1, y1, length / 2, angle);
    }
  }

  void Tree2D::Make_Stalk_Head(float x1, float y1, float length, float angle)
  {
    //Make a fluffy head
  }

  void Tree2D::Make_Pine_Branch(float x1, float y1, float length, float angle, int depth, int curve)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      Add_Branch(x1, y1, x2, y2);

      x1 = x1 + length / 2 * cos(angle);
      y1 = y1 + length / 2 * sin(angle);

      float length2 = length / 2;
      float angle2;
      if (curve == 0)
        angle2 = angle - ANGLE + myrand(RAND);
      else
        angle2 = angle + ANGLE + myrand(RAND);

      Make_Pine_Branch(x2, y2, length / 1.5f, angle2, depth - 2, curve);
      if (curve == 0) //Right side
        angle2 = -angle2 + angle / 8;
      else            //Left side
        angle2 = -angle2 + angle / 8;

      Make_Pine_Branch(x1, y1, length / 2, angle2, depth / 2, curve);
    }
  }

  void Tree2D::Add_Branch(float x1, float y1, float x2, float y2)
  {
    treeMesh.push_back(x1);
    treeMesh.push_back(y1);
    treeMesh.push_back(x2);
    treeMesh.push_back(y2);
  }

}