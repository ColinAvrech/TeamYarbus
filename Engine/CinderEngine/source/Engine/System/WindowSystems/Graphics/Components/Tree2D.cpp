/******************************************************************************/
/*!
\file   Tree2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
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

#define PI 3.141f

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
    if (vao)
    {
      delete vao;
      vao = nullptr;
    }

    if (vbo)
    {
      delete vbo;
      vbo = nullptr;
    }

    if (tree)
    {
      delete tree;
      tree = nullptr;
    }
  }

  void Tree2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Tree_Type");
    value->GetValue ((int*)&type);
    //mandatory
    data->FindElement(data, "Segments")->GetValue(&segments);

    value = data->FindElement (data, "Color");
    value->GetValue (&color);
    //mandatory 
    data->FindElement(data, "Length")->GetValue(&length);

    //trunk thickness
    value = data->FindElement(data, "Thickness");
    if (value != nullptr) value->GetValue(&base_radius);
    else base_radius = 1.f;
    //scale size down for mesh creation
    //base_radius *= 0.05f;

    //rate at which branch thickness reduces
    value = data->FindElement(data, "Decay");
    if (value != nullptr) value->GetValue(&decay_rate);
    else decay_rate = 0.75f;
  }

  void Tree2D::Initialize ()
  {
    FireGroup* fsm = nullptr;
    switch (type)
    {
    case Framework::TREE_0:
      Make_Tree0(0, -0.1f, length, 1.5, segments, base_radius);
      break;
    case Framework::TREE_1:
      Make_Tree1(0, -0.1f, 0.0f, 0.25f, 45, segments, 3);
      break;
    case Framework::TREE_2:
      Make_Tree2(0, -0.1f, length, 1.5, segments, base_radius);
      break;
    case Framework::TREE_3:
      Make_Tree3(0, -0.1f, length, 1.5, segments, base_radius);
      break;
    case Framework::TREE_4:
      Make_Tree4(0, -0.1f, length, 1.5, segments, base_radius);
	  break;
	case Framework::TREE_LONG:
    Make_TreeLong(0, -0.1f, 0.5f, 2.0, 6, segments, 10);
      break;
    case Framework::TREE_5:
      tree = new FractalGenerator ();
      tree->Generate_Tree ();
      shader = Resources::RS->Get_Shader ("Tree");
      tree->Create_Mesh (tree->getTotalLines (), &treeMesh, &edges);

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

    case Framework::TREE_SHORT_GRASS:
      Make_Grass(0, -0.2f, length, segments);
      break;
    default:
      break;
    }

   /* if (fsm != nullptr && fsm->firePoints.size())
    {
      Physics::THERMODYNAMICS->Add_Group(fsm);
    }*/

    Generate_Buffers();
  }


  void Tree2D::Generate_Buffers ()
  {
    shader = Resources::RS->Get_Shader("Tree");
    vao = new VAO();
    vbo = new VBO(treeMesh.size() * sizeof(float), treeMesh.data());
    GLint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    vao->unbindVAO();
  }

  void Tree2D::Draw ()
  {
    glDisable(GL_BLEND);
    shader->Use();
    vao->bindVAO();
    shader->uniMat4("mvp", glm::value_ptr(gameObject->Transform->GetModelViewProjectionMatrix()));
    shader->uni4fv("color", glm::value_ptr(color));
    glDrawArrays(GL_TRIANGLES, 0, treeMesh.size() / 2);

    vao->unbindVAO();
    shader->Disable();
    OPENGL->ResetBlendMode();
  }

  void Tree2D::Set(glm::vec4& _color, Tree_Type _type)
  {
    color = _color;
    type = _type;
  }

  void Tree2D::Make_Tree0(float x1, float y1, float length1, float angle1, int depth, float rad, unsigned parent)
  {
    //rad = 0.001f;
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length1 * cos (angle1);
      float y2 = y1 + length1 * sin (angle1);

      unsigned newParent = Add_Branch(x1, y1, x2, y2, rad, parent);

      float newrad = rad * decay_rate;

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
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1, newrad, newParent);
      length2 = length1 * (SCALE + myrand (RAND));
      angle2 = angle1 - ANGLE + myrand (RAND);
      if (depth % 2 == 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);
      if (fork > 20 - depth * 2)
        Make_Tree0 (x2, y2, length2 * f, angle2, depth - 1, newrad, newParent);
    }
  }


  void Tree2D::Make_Tree1 (float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
  {
    //Add_Branch(x1, y1, x2, y2);

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

  void Tree2D::Make_Tree2(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      parent = Add_Branch(x1, y1, x2, y2, rad, parent);

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
      float newrad = rad * decay_rate;

      Make_Tree2(x2, y2, length2 * f, angle2, depth - 1, newrad, parent);

      y2 = y1 + length * (2 * (SCALE + myrand(RAND))) * sin(-((15 - depth) / 10.f)*angle);
     
      x2 = x1 + length * (2 * (SCALE + 3*myrand(RAND))) * cos(angle - angle2);
      
      //Add_Branch(x1, y1, x2, y2);
      Make_Pine_Branch(x1, y1, length, angle - 1.2f * angle2, depth - 1, 1, newrad, parent);

      x2 = x1 + length * (2 * (SCALE + 3*myrand(RAND))) * cos(angle + angle2);

      y2 = y1 + length * (2 * (SCALE + myrand(RAND))) * sin(-((15 - depth)/10.f)*angle);

      //Add_Branch(x1, y1, x2, y2);
      Make_Pine_Branch(x1, y1, length, angle + 1.2f * angle2, depth - 1, 0, newrad, parent);
    }
  }

  void Tree2D::Make_Tree3(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      parent = Add_Branch(x1, y1, x2, y2, rad, parent);

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
      float newrad = rad * decay_rate;

      Make_Tree3(x2, y2, length * f, angle2, depth - 1, newrad, parent);

      if (depth < segments - 2)
      {
        int r = rand() % 100;
        if (r > 20 - depth)
          Make_Tree0(x2, y2, length2, angle / 2 + angle2, depth / 2, newrad, parent);
        r = rand() % 100;
        if (r > 20 - depth)
          Make_Tree0(x2, y2, length2, -angle / 2 + angle2, depth / 2, newrad, parent);
      }
    }
  }

  void Tree2D::Make_Tree4(float x1, float y1, float length, float angle, int depth, float rad)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      Add_Branch(x1, y1, x2, y2, rad);

      float angle2;
      if (depth % 2 == 0)
        angle2 = angle + ANGLE + myrand(RAND);
      else
        angle2 = angle - ANGLE + myrand(RAND);

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;
      float newrad = rad * decay_rate;

      Make_Tree4(x2, y2, length * f, angle2, depth - 1, newrad);
    }
    else
    {
      int min_branches = 3;
      int branches = min_branches + rand() % 3;
      float spawn_angle = angle + PI / 3.f;
      decay_rate *= 0.75f;
      for (int i = 0; i < branches; ++i)
      {
        Make_Tree0(x1 * 0.75f, y1 * 0.75f, 0.05f, spawn_angle, 1.5f * segments, 1.5f * rad);
        spawn_angle -= (2 * PI / 3) / branches - myrand(0.1f);
      }
    }
  }

  void Tree2D::Make_TreeLong(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
  {
	 // Add_Branch(x1, y1, x2, y2);

	  if (depth < 1)
	  {
		  return;
	  }

	  float treeRatio = glm::linearRand(0.7f, 0.9f);
	  int nn = depth - 1;
	  GLfloat x3 = (x2 - x1)*treeRatio + x1 - x2;
	  GLfloat y3 = (y2 - y1)*treeRatio + y1 - y2;
	  if (branchCount == 2)
	  {
		  // Right Branch
		  Make_TreeLong(x2, y2, x3 * cos(angle) + y3 * sin(angle) + x2, -x3 * sin(angle) + y3 * cos(angle) + y2, angle, nn, branchCount);
		  // Left Branch
		  Make_TreeLong(x2, y2, x3 * cos(-angle) + y3 * sin(-angle) + x2, -x3 * sin(-angle) + y3 * cos(-angle) + y2, angle, nn, branchCount);
	  }
	  else
	  {
		  GLfloat nowAngle = -angle;
		  GLfloat angleTone = angle / (branchCount - 1) * 2;
		  for (int i = 0; i < branchCount; i++, nowAngle += angleTone)
		  {
			  Make_TreeLong(x2, y2, x3 * cos(nowAngle) + y3 * sin(nowAngle) + x2, -x3 * sin(nowAngle) + y3 * cos(nowAngle) + y2, angle, nn, branchCount);
		  }
	  }
  }

  void Tree2D::Make_Grass(float x1, float y1, float length1, int depth)
  {
    float ANGLE = 0.2f;
    float RAND = 0.2f;
    int tuft = 10 + rand() % 10;
    //if (tuft >= 15)
      //Make_Grass_Stalk(x1, y1, length, 1.5f, 2 + rand() % 3, rand() % 2);
    float angle = 2.f + myrand(ANGLE);
    float x = -1.f;
    for (int i = 0; i < tuft - 1; ++i)
    {
      float angle2 = angle - (float(i) / tuft) + myrand(ANGLE);
      float length2 = length1 * (1 + std::sin(i * 3.14f / (tuft - 2)));
      //float y2 = y1 - 0.125 * (1 - std::sin(i * 3.14f / (tuft - 2)));
      float d = decay_rate;
      Make_Grass_Blade(x, y1, length2, angle2, depth, base_radius);
      decay_rate = d;
      x += 0.5f * base_radius;
    }
  }

  void Tree2D::Make_Grass_Blade(float x1, float y1, float length, float angle, int depth, float width, unsigned parent)
  {
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      parent = Add_Branch(x1, y1, x2, y2, width, parent);
      if (depth == 2)
        decay_rate = 0.f;
      Make_Grass_Blade(x2, y2, length, angle + myrand(0.3f), depth - 1, width * decay_rate, parent);
    }
  }

  void Tree2D::Make_Grass_Stalk(float x1, float y1, float length, float angle, int depth, int curve, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.2f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      parent = Add_Branch(x1, y1, x2, y2, base_radius, parent);

      x1 = x1 + length * cos(angle);
      y1 = y1 + length * sin(angle);
      float angle2;
      if (curve == 0)
        angle2 = angle + ANGLE + myrand(RAND);
      else
        angle2 = angle - ANGLE + myrand(RAND);

      Make_Grass_Stalk(x1, y1, length, angle2, depth - 1, curve, parent);
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

  void Tree2D::Make_Pine_Branch(float x1, float y1, float length, float angle, int depth, int curve, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      unsigned newParent = Add_Branch(x1, y1, x2, y2, rad, parent);

      x1 = x1 + length / 2 * cos(angle);
      y1 = y1 + length / 2 * sin(angle);

      float newrad = rad * decay_rate;

      float length2 = length / 2;
      float angle2;
      if (curve == 0)
        angle2 = angle - ANGLE - myrand(RAND);
      else
        angle2 = angle + ANGLE + myrand(RAND);

      Make_Pine_Branch(x2, y2, length / 1.5f, angle2, depth - 2, curve, newrad, newParent);
      if (curve == 0) //Right side
        angle2 = -angle2 + angle / 8;
      else            //Left side
        angle2 = -angle2 + angle / 8;

      Make_Pine_Branch(x1, y1, length / 2, angle2, depth / 2, curve, newrad, newParent);
    }
  }

  unsigned Tree2D::Add_Branch(float x1, float y1, float x2, float y2, float rad, unsigned parent)
  {
    glm::vec2 dis(y2 - y1, x1 - x2);

    float _x;
    float _y;

    if (parent == 0)
    {
      _x = x1 + dis.x * rad;
      _y = y1 + dis.y * rad;

      //pt1
      treeMesh.push_back(x1 - dis.x * rad);
      treeMesh.push_back(y1 - dis.y * rad);
      //pt2
      treeMesh.push_back(_x);
      treeMesh.push_back(_y);
    }

    else
    {
      _x = treeMesh[parent - 2];
      _y = treeMesh[parent - 1];

      //pt1
      treeMesh.push_back(treeMesh[parent - 4]);
      treeMesh.push_back(treeMesh[parent - 3]);
      //pt2
      treeMesh.push_back(_x);
      treeMesh.push_back(_y);
    }
    //pt3
    treeMesh.push_back(x2 - dis.x * rad * decay_rate);
    treeMesh.push_back(y2 - dis.y * rad * decay_rate);

    //triangle 2
    //pt2
    treeMesh.push_back(_x);
    treeMesh.push_back(_y);
    //pt3
    treeMesh.push_back(x2 - dis.x * rad * decay_rate);
    treeMesh.push_back(y2 - dis.y * rad * decay_rate);
    //pt4
    treeMesh.push_back(x2 + dis.x * rad * decay_rate);
    treeMesh.push_back(y2 + dis.y * rad * decay_rate);

    return treeMesh.size();
  }

}