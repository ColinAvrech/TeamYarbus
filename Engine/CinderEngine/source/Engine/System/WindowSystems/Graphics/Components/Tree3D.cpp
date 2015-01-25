/******************************************************************************/
/*!
\file   Tree3D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Tree3D.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "WindowSystem.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Thermodynamics.h"
#include "Pipeline.h"
#include "random.hpp"

#define PI 3.141f
#define IX(i) i*6
#define V_DATA 6

namespace Framework
{
  DefineComponentName(Tree3D);

  static std::vector<float> joints_x;
  static std::vector<float> joints_y;

  static float myrand(float R)
  {
    return (2 * R * rand()) / RAND_MAX - R;
  }


  // Constructor
  Tree3D::Tree3D() : vertex_count(0)
  {}

  // Destructor
  Tree3D::~Tree3D()
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
  }

  void Tree3D::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement(data, "Tree_Type");
    value->GetValue((int*)&type);
    //mandatory
    data->FindElement(data, "Segments")->GetValue(&segments);

    value = data->FindElement(data, "Color");
    value->GetValue(&color);
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

  void Tree3D::Initialize()
  {
    //FireGroup* fsm = nullptr;
    glm::vec3 pos(0.f, -0.1f, 0.f);
    glm::vec2 angle(PI / 2, PI / 2);

    switch (type)
    {
    case Framework::TREE_0_3D:
      Make_Tree0(pos, length, angle, segments, base_radius);
      break;
    //case Framework::TREE_1:
    //  Make_Tree1(0, -0.1f, 0.0f, 0.25f, 45, segments, 3);
    //  break;
    //case Framework::TREE_2:
    //  Make_Tree2(0, -0.1f, length, 1.5, segments, base_radius);
    //  break;
    //case Framework::TREE_3:
    //  Make_Tree3(0, -0.1f, length, 1.5, segments, base_radius);
    //  break;
    //case Framework::TREE_4:
    //  Make_Tree4(0, -0.1f, length, 1.5, segments, base_radius);
    //  break;
    //case Framework::TREE_LONG:
    //  Make_TreeLong(0, -0.1f, 0.5f, 2.0, 6, segments, 10);
    //  break;
    //case Framework::TREE_5:
    //  break;
    //
    //case Framework::TREE_SHORT_GRASS:
    //  Make_Grass(0, -0.2f, length, segments);
    //  break;
    default:
      break;
    }

    /* if (fsm != nullptr && fsm->firePoints.size())
    {
    Physics::THERMODYNAMICS->Add_Group(fsm);
    }*/

    Generate_Buffers();
    CalculateBounds();
    vertex_count = indices.size();
    treeMesh.clear();
  }

  bool Tree3D::InViewport()
  {
    //Object bounds
    glm::vec2 plt = (glm::vec2)gameObject->Transform->GetScale() * glm::vec2(bound_l, bound_t) + (glm::vec2)gameObject->Transform->GetPosition();
    glm::vec2 prb = (glm::vec2)gameObject->Transform->GetScale() * glm::vec2(bound_r, bound_b) + (glm::vec2)gameObject->Transform->GetPosition();
    //Viewport bounds
    glm::vec2 cam_pos = (glm::vec2)Camera::main->gameObject->Transform->GetPosition();
    float fov = Camera::main->GetSize();

    if (plt.x > cam_pos.x + fov || prb.x < cam_pos.x - fov ||
      prb.y > cam_pos.y + fov || plt.y < cam_pos.y - fov)
      return false;
    //Otherwise
    return true;

  }

  void Tree3D::Generate_Buffers()
  {
    shader = Resources::RS->Get_Shader("Tree");
    vao = new VAO();
    vbo = new VBO(treeMesh.size() * sizeof(float), treeMesh.data(), GL_STATIC_DRAW);

    GLint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    vao->unbindVAO();

    // Generate a buffer for the indices
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  }

  void Tree3D::CalculateBounds()
  {
    bound_r = *std::max_element(joints_x.begin(), joints_x.end());
    bound_l = *std::min_element(joints_x.begin(), joints_x.end());
    bound_t = *std::max_element(joints_y.begin(), joints_y.end());
    bound_b = *std::min_element(joints_y.begin(), joints_y.end());
    joints_x.clear();
    joints_y.clear();
  }

  void Tree3D::Draw()
  {
    if (!InViewport())
      return;

    glDisable(GL_BLEND);
    shader->Use();
    vao->bindVAO();
    shader->uniMat4("mvp", glm::value_ptr(gameObject->Transform->GetModelViewProjectionMatrix()));
    shader->uni4fv("color", glm::value_ptr(color));
    //glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
      GL_TRIANGLES,      // mode
      indices.size(),    // count
      GL_UNSIGNED_INT,   // type
      (void*)0           // element array buffer offset
      );

    vao->unbindVAO();
    shader->Disable();
    OPENGL->ResetBlendMode();
  }

  void Tree3D::Set(glm::vec4& _color, Tree3D_Type _type)
  {
    color = _color;
    type = _type;
  }

  void Tree3D::Make_Tree0(glm::vec3 &pos, float length1, glm::vec2 &angle1, int depth, float rad, unsigned parent)
  {
    //rad = 0.001f;
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      glm::vec3 pos2;
      pos2.x = pos.x + length1 * cos(angle1.y);
      pos2.y = pos.y + length1 * sin(angle1.x + angle1.y);
      pos2.z = pos.z + length1 * cos(angle1.x);

      parent = Add_Branch(pos, pos2, rad, parent);

      float newrad = rad * decay_rate;

      //branch 1
      float length2 = length1 * (SCALE + myrand(RAND));
      glm::vec2 angle2;
      angle2.y = angle1.y + ANGLE + myrand(RAND);
      angle2.x = angle1.x + myrand(2 * RAND) - RAND;

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;
      //decide whether or not to spawn a branch
      int fork;
      if (depth % 2 != 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);

      if (fork > 20 - depth * 2)
        Make_Tree0(pos2, length2 * f, angle2, depth - 1, newrad, parent);

      //branch 2
      length2 = length1 * (SCALE + myrand(RAND));
      angle2.y = angle1.y - ANGLE + myrand(RAND);
      angle2.x = angle2.x - myrand(2 * RAND) + RAND;

      if (depth % 2 == 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);

      if (fork > 20 - depth * 2)
        Make_Tree0(pos2, length2 * f, angle2, depth - 1, newrad, parent);
    }
  }


  void Tree3D::Make_Tree1(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
  {
    //Add_Branch(x1, y1, x2, y2);

    if (depth < 1)
    {
      return;
    }

    float treeRatio = glm::linearRand(0.5f, 0.8f);
    int nn = depth - 1;
    GLfloat x3 = (x2 - x1)*treeRatio + x1 - x2;
    GLfloat y3 = (y2 - y1)*treeRatio + y1 - y2;
    if (branchCount == 2)
    {
      // Right Branch
      Make_Tree1(x2, y2, x3 * cos(angle) + y3 * sin(angle) + x2, -x3 * sin(angle) + y3 * cos(angle) + y2, angle, nn, branchCount);
      // Left Branch
      Make_Tree1(x2, y2, x3 * cos(-angle) + y3 * sin(-angle) + x2, -x3 * sin(-angle) + y3 * cos(-angle) + y2, angle, nn, branchCount);
    }
    else
    {
      GLfloat nowAngle = -angle;
      GLfloat angleTone = angle / (branchCount - 1) * 2;
      for (int i = 0; i < branchCount; i++, nowAngle += angleTone)
      {
        Make_Tree1(x2, y2, x3 * cos(nowAngle) + y3 * sin(nowAngle) + x2, -x3 * sin(nowAngle) + y3 * cos(nowAngle) + y2, angle, nn, branchCount);
      }
    }
  }

  void Tree3D::Make_Tree2(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      //parent = Add_Branch(x1, y1, x2, y2, rad, parent);

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

      x2 = x1 + length * (2 * (SCALE + 3 * myrand(RAND))) * cos(angle - angle2);

      if (depth < segments - 1)
      {
        //Add_Branch(x1, y1, x2, y2);
        Make_Tree2(x1, y1, length * 0.75, angle - 1.2f * angle2, depth / 3, newrad / 2, parent);

        x2 = x1 + length * (2 * (SCALE + 3 * myrand(RAND))) * cos(angle + angle2);

        y2 = y1 + length * (2 * (SCALE + myrand(RAND))) * sin(-((15 - depth) / 10.f)*angle);

        //Add_Branch(x1, y1, x2, y2);
        Make_Tree2(x1, y1, length * 0.75, angle + 1.2f * angle2, depth / 3, newrad / 2, parent);
      }
    }
  }

  void Tree3D::Make_Tree3(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent)
  {
    //float SCALE = 1.0f;
    //float ANGLE = 0.0f;
    //float RAND = 0.1f;
    //if (depth > 0)
    //{
    //  float x2 = x1 + length * cos(angle);
    //  float y2 = y1 + length * sin(angle);

    //  //parent = Add_Branch(x1, y1, x2, y2, rad, parent);

    //  x1 = x1 + length / 2 * cos(angle);
    //  y1 = y1 + length / 2 * sin(angle);

    //  float length2 = length / 2;
    //  float angle2;
    //  if (depth % 2 == 0)
    //    angle2 = angle + ANGLE + myrand(RAND);
    //  else
    //    angle2 = angle - ANGLE + myrand(RAND);

    //  int factor = 80 + rand() % 20;
    //  float f = factor / 100.f;
    //  float newrad = rad * decay_rate;

    //  Make_Tree3(x2, y2, length * f, angle2, depth - 1, newrad, parent);

    //  if (depth < segments - 2)
    //  {
    //    int r = rand() % 100;
    //    if (r > 20 - depth)
    //      Make_Tree0(x2, y2, length2, angle / 2 + angle2, depth / 2, newrad, parent);
    //    r = rand() % 100;
    //    if (r > 20 - depth)
    //      Make_Tree0(x2, y2, length2, -angle / 2 + angle2, depth / 2, newrad, parent);
    //  }
    //}
  }

  void Tree3D::Make_Tree4(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent)
  {
    //float SCALE = 1.0f;
    //float ANGLE = 0.0f;
    //float RAND = 0.1f;
    //if (depth > 0)
    //{
    //  float x2 = x1 + length * cos(angle);
    //  float y2 = y1 + length * sin(angle);

    //  //parent = Add_Branch(x1, y1, x2, y2, rad, parent);

    //  float angle2;
    //  if (depth % 2 == 0)
    //    angle2 = angle + ANGLE + myrand(RAND);
    //  else
    //    angle2 = angle - ANGLE + myrand(RAND);

    //  int factor = 80 + rand() % 20;
    //  float f = factor / 100.f;
    //  float newrad = rad * decay_rate;

    //  Make_Tree4(x2, y2, length * f, angle2, depth - 1, newrad, parent);
    //}
    //else
    //{
    //  int min_branches = 3;
    //  int branches = min_branches + rand() % 3;
    //  float spawn_angle = angle + PI / 3.f;
    //  decay_rate *= 0.75f;
    //  for (int i = 0; i < branches; ++i)
    //  {
    //    Make_Tree0(x1, y1, 0.05f, spawn_angle, 1.5f * segments, rad, parent);
    //    spawn_angle -= (2 * PI / 3) / branches;
    //  }
    //}
  }

  void Tree3D::Make_TreeLong(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount)
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

  void Tree3D::Make_Grass(float x1, float y1, float length1, int depth)
  {
    float ANGLE = 0.2f;
    float RAND = 0.2f;
    int tuft = 10 + rand() % 10;

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

  void Tree3D::Make_Grass_Blade(float x1, float y1, float length, float angle, int depth, float width, unsigned parent)
  {
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      //parent = Add_Branch(x1, y1, x2, y2, width, parent);
      if (depth == 2)
        decay_rate = 0.f;
      Make_Grass_Blade(x2, y2, length, angle + myrand(0.3f), depth - 1, width * decay_rate, parent);
    }
  }

  void Tree3D::Make_Pine_Branch(float x1, float y1, float length, float angle, int depth, int curve, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      float x2 = x1 + length * cos(angle);
      float y2 = y1 + length * sin(angle);

      //unsigned newParent = Add_Branch(x1, y1, x2, y2, rad, parent);

      x1 = x1 + length / 2 * cos(angle);
      y1 = y1 + length / 2 * sin(angle);

      float newrad = rad * decay_rate;

      float length2 = length / 2;
      float angle2;
      if (curve == 0)
        angle2 = angle - ANGLE - myrand(RAND);
      else
        angle2 = angle + ANGLE + myrand(RAND);

      Make_Pine_Branch(x2, y2, length / 1.5f, angle2, depth - 2, curve, newrad, parent);
      if (curve == 0) //Right side
        angle2 = -angle2 + angle / 8;
      else            //Left side
        angle2 = -angle2 + angle / 8;

      Make_Pine_Branch(x1, y1, length / 2, angle2, depth / 2, curve, newrad, parent);
    }
  }

  unsigned Tree3D::Add_Branch(glm::vec3 &pos1, glm::vec3 &pos2, float rad, unsigned parent, bool branch_tip)
  {
    enum BranchID
    {
      B_LEFT = 0,
      B_RIGHT,
      B_BACK,
      B_FRONT
    };

    joints_x.push_back(pos1.x);
    joints_y.push_back(pos1.y);

    glm::vec3 dis_s(pos2.y - pos2.y, pos1.x - pos1.x, 0.f);
    glm::vec3 dis_d(dis_s.z, dis_s.y, dis_s.x);

    glm::vec3 new_vertices[4];
    glm::vec3 parent_vertices[4];
    glm::vec3 normals[4];

    //If it's the first branch in the tree add new base vertices
    if (parent == 0)
    {
      //base l
      parent_vertices[B_LEFT] = pos1 - rad * dis_s;
      //base r
      parent_vertices[B_RIGHT] = pos1 + rad * dis_s;
      //base b
      parent_vertices[B_BACK] = pos1 - rad * dis_d;
      //base f
      parent_vertices[B_FRONT] = pos1 + rad * dis_d;
    }

    if (branch_tip)
    {
      //single tip
      new_vertices[0] = pos2;
      //calculate normals
      normals[B_LEFT]  = Physics::Normal(new_vertices[0], parent_vertices[B_LEFT],  parent_vertices[B_FRONT]);
      normals[B_BACK]  = Physics::Normal(new_vertices[0], parent_vertices[B_BACK],  parent_vertices[B_LEFT] );
      normals[B_RIGHT] = Physics::Normal(new_vertices[0], parent_vertices[B_RIGHT], parent_vertices[B_BACK] );
      normals[B_FRONT] = Physics::Normal(new_vertices[0], parent_vertices[B_FRONT], parent_vertices[B_RIGHT]);
    }
    else
    {
      //tip l
      new_vertices[B_LEFT] = pos2 - rad * decay_rate * dis_s;
      //tip r
      new_vertices[B_RIGHT] = pos2 + rad * decay_rate * dis_s;
      //tip b
      new_vertices[B_BACK] = pos2 - rad * decay_rate * dis_d;
      //tip f
      new_vertices[B_FRONT] = pos2 + rad * decay_rate * dis_d;
      //calculate normals
      normals[B_LEFT] = Physics::Normal(new_vertices[B_LEFT], parent_vertices[B_LEFT], parent_vertices[B_FRONT]);
      normals[B_BACK] = Physics::Normal(new_vertices[B_BACK], parent_vertices[B_BACK], parent_vertices[B_LEFT]);
      normals[B_RIGHT] = Physics::Normal(new_vertices[B_RIGHT], parent_vertices[B_RIGHT], parent_vertices[B_BACK]);
      normals[B_FRONT] = Physics::Normal(new_vertices[B_FRONT], parent_vertices[B_FRONT], parent_vertices[B_RIGHT]);
    }

    //add to vertex/index array
    if (parent == 0)
    {
      //add 4 new vertices
      //vl
      /*1*/treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      /*1*///nl
      /*1*/glm::vec3 n_l = normals[B_LEFT] + normals[B_BACK]; n_l = glm::normalize(n_l);
      /*1*/treeMesh.push_back(n_l.x); treeMesh.push_back(n_l.y); treeMesh.push_back(n_l.z);
      //vr
      /*2*/treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      /*2*///nr
      /*2*/glm::vec3 n_r = normals[B_RIGHT] + normals[B_FRONT]; n_r = glm::normalize(n_r);
      /*2*/treeMesh.push_back(n_r.x); treeMesh.push_back(n_r.y); treeMesh.push_back(n_r.z);
      //vb
      /*3*/treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      /*3*///nb
      /*3*/glm::vec3 n_b = normals[B_BACK] + normals[B_RIGHT]; n_b = glm::normalize(n_b);
      /*3*/treeMesh.push_back(n_b.x); treeMesh.push_back(n_b.y); treeMesh.push_back(n_b.z);
      //vf
      /*4*/treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      /*4*///nf
      /*4*/glm::vec3 n_f = normals[B_FRONT] + normals[B_LEFT]; n_f = glm::normalize(n_f);
      /*4*/treeMesh.push_back(n_f.x); treeMesh.push_back(n_f.y); treeMesh.push_back(n_f.z);

      vertex_count += 4;

      parent = vertex_count;
    }

    if (branch_tip)
    {
      //add one new vertex
      treeMesh.push_back(new_vertices[0].x); treeMesh.push_back(new_vertices[0].y); treeMesh.push_back(new_vertices[0].z);
      glm::vec3 n = normals[B_BACK] + normals[B_FRONT] + normals[B_LEFT] + normals[B_RIGHT]; n = glm::normalize(n);
      treeMesh.push_back(n.x); treeMesh.push_back(n.y); treeMesh.push_back(n.z);

      //increment vertex count
      ++vertex_count;

      //add 4 new triangles
      /*1*/indices.push_back(parent - 4); indices.push_back(parent - 2); indices.push_back(vertex_count - 1);
      /*2*/indices.push_back(parent - 3); indices.push_back(parent - 2); indices.push_back(vertex_count - 1);
      /*3*/indices.push_back(parent - 4); indices.push_back(parent - 1); indices.push_back(vertex_count - 1);
      /*4*/indices.push_back(parent - 1); indices.push_back(parent - 3); indices.push_back(vertex_count - 1);
    }
    else
    {
      //add 4 new vertices
      //vl
      treeMesh.push_back(new_vertices[B_LEFT].x); treeMesh.push_back(new_vertices[B_LEFT].y); treeMesh.push_back(new_vertices[B_LEFT].z);
      //nl
      glm::vec3 n_l = normals[B_LEFT] + normals[B_BACK]; n_l = glm::normalize(n_l);
      treeMesh.push_back(n_l.x); treeMesh.push_back(n_l.y); treeMesh.push_back(n_l.z);
      //vr
      treeMesh.push_back(new_vertices[B_RIGHT].x); treeMesh.push_back(new_vertices[B_RIGHT].y); treeMesh.push_back(new_vertices[B_RIGHT].z);
      //nr
      glm::vec3 n_r = normals[B_RIGHT] + normals[B_FRONT]; n_r = glm::normalize(n_r);
      treeMesh.push_back(n_r.x); treeMesh.push_back(n_r.y); treeMesh.push_back(n_r.z);
      //vb
      treeMesh.push_back(new_vertices[B_BACK].x); treeMesh.push_back(new_vertices[B_BACK].y); treeMesh.push_back(new_vertices[B_BACK].z);
      //nb
      glm::vec3 n_b = normals[B_BACK] + normals[B_RIGHT]; n_b = glm::normalize(n_b);
      treeMesh.push_back(n_b.x); treeMesh.push_back(n_b.y); treeMesh.push_back(n_b.z);
      //vf
      treeMesh.push_back(new_vertices[B_FRONT].x); treeMesh.push_back(new_vertices[B_FRONT].y); treeMesh.push_back(new_vertices[B_FRONT].z);
      //nf
      glm::vec3 n_f = normals[B_FRONT] + normals[B_LEFT]; n_f = glm::normalize(n_f);
      treeMesh.push_back(n_f.x); treeMesh.push_back(n_f.y); treeMesh.push_back(n_f.z);

      vertex_count += 4;

      //add 8 new triangles
      /*1*/indices.push_back(parent - 4); indices.push_back(parent - 2);       indices.push_back(vertex_count - 4);
      /*2*/indices.push_back(parent - 2); indices.push_back(vertex_count - 4); indices.push_back(vertex_count - 2);
      /*3*/indices.push_back(parent - 2); indices.push_back(parent - 3);       indices.push_back(vertex_count - 2);
      /*4*/indices.push_back(parent - 3); indices.push_back(vertex_count - 2); indices.push_back(vertex_count - 3);
      /*5*/indices.push_back(parent - 3); indices.push_back(parent - 1);       indices.push_back(vertex_count - 3);
      /*6*/indices.push_back(parent - 1); indices.push_back(vertex_count - 3); indices.push_back(vertex_count - 1);
      /*7*/indices.push_back(parent - 1); indices.push_back(parent - 4);       indices.push_back(vertex_count - 1);
      /*8*/indices.push_back(parent - 4); indices.push_back(vertex_count - 1); indices.push_back(vertex_count - 4);
    }
    
    return vertex_count;
  }

}