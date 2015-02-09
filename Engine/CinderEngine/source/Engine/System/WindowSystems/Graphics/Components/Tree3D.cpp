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


META_DEFINE( Framework::Tree3D, Tree3D )
{
	MEMBER( bound_l );
	MEMBER( color );
}

namespace Framework
{
  DefineComponentName(Tree3D);

  

  static std::vector<float> joints_x;
  static std::vector<float> joints_y;
  static float time = 0.f;

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
    glDeleteBuffers(1, &elementbuffer);
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
    glm::vec2 angle(0.f, PI / 2);

    switch (type)
    {
    case Framework::TREE_0_3D:
      Make_Tree0(pos, length, angle, segments, base_radius);
      break;
    case Framework::TREE_1_3D:
      Make_Tree1(pos, length, angle, segments, base_radius);
      break;
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
    poly_count = treeMesh.size() / 12;
    treeMesh.clear();
    indices.clear();
  }

  bool Tree3D::InViewport()
  {
    //Object bounds
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    glm::vec2 plt = (glm::vec2)tform->GetScale() * glm::vec2(bound_l, bound_t) + (glm::vec2)tform->GetPosition();
    glm::vec2 prb = (glm::vec2)tform->GetScale() * glm::vec2(bound_r, bound_b) + (glm::vec2)tform->GetPosition();
    //Viewport bounds
    glm::vec2 cam_pos = (glm::vec2)static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"))->GetPosition();
    float fov = Camera::main->GetSize();

    if (plt.x > cam_pos.x + fov || prb.x < cam_pos.x - fov ||
      prb.y > cam_pos.y + fov || plt.y < cam_pos.y - fov)
      return false;
    //Otherwise
    return true;

  }

  void Tree3D::Generate_Buffers()
  {
    shader = Resources::RS->Get_Shader("Tree3D");
    vao = new VAO();
    vbo = new VBO(treeMesh.size() * sizeof(float), treeMesh.data(), GL_STATIC_DRAW);
    // Generate a buffer for the indices
    //ebo = new EBO(indices.size() * sizeof(unsigned), indices.data());

    GLint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint normAttrib = shader->attribLocation("normal");
    shader->enableVertexAttribArray(normAttrib);
    shader->vertexAttribPtr(normAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));

    vao->unbindVAO();
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

    //glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    shader->Use();
    vao->bindVAO();
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    shader->uniMat4("mvp", glm::value_ptr(tform->GetModelViewProjectionMatrix()));
    shader->uniMat4("modelMatrix", glm::value_ptr(tform->GetModelMatrix()));
    shader->uni4fv("color", glm::value_ptr(color));
    time += dt;
    //glm::vec3 lightPos(10 * std::cos(time / 5.f), 45 + 10 * std::sin(time / 10.f), 10 * std::sin(time / 5.f));
    
    // Draw the triangles !
    //glDrawElements(
    //  GL_TRIANGLES,      // mode
    //  poly_count,        // count
    //  GL_UNSIGNED_INT,   // type
    //  0                  // element array buffer offset
    //  );

    glDrawArrays(GL_TRIANGLES, 0, poly_count);

    vao->unbindVAO();
    shader->Disable();
    //OPENGL->ResetBlendMode();
    glDisable(GL_CULL_FACE);
  }

  void Tree3D::Set(glm::vec4& _color, Tree3D_Type _type)
  {
    color = _color;
    type = _type;
  }

  void Tree3D::Make_Tree0(glm::vec3 &pos, float length1, glm::vec2 &angle1, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.3f;
    float RAND = 0.1f;
    if (depth > 0)
    {
      glm::vec3 pos2;
      pos2.x = pos.x + length1 * cos(angle1.y);
      pos2.y = pos.y + length1 * sin(angle1.x + angle1.y);
      pos2.z = pos.z + length1 * sin(angle1.x);

      bool tip = false;
      if (depth == 1)
        tip = true;

      parent = Add_Branch(pos, pos2, rad, parent, tip);

      float newrad = rad * decay_rate;

      //branch 1
      glm::vec2 angle2;
      angle2.y = angle1.y + ANGLE + myrand(RAND);
      //angle2.x = angle1.x + myrand(2 * RAND);
      angle2.x = angle1.x + myrand(RAND) - 0.75f * RAND;

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;
      //decide whether or not to spawn a branch
      int fork;
      if (depth % 2 != 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);

      if (fork > 20 - depth * 2)
        Make_Tree0(pos2, length1 * f, angle2, depth - 1, newrad, parent);

      //branch 2
      angle2.y = angle1.y - ANGLE + myrand(RAND);
      angle2.x = angle1.x - myrand(RAND) + 0.75f * RAND;

      if (depth % 2 == 0)
        fork = rand() % 100;
      else
        fork = (rand() % 10) * (rand() % 10);

      if (fork > 20 - depth * 2)
        Make_Tree0(pos2, length1 * f, angle2, depth - 1, newrad, parent);
    }
  }


  void Tree3D::Make_Tree1(glm::vec3 &pos, float length1, glm::vec2 &angle1, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.0f;
    float RAND = 0.1f;
    if (depth > 1)
    {
      glm::vec3 pos2;
      pos2.x = pos.x + length1 * cos(angle1.y);
      pos2.y = pos.y + length1 * sin(angle1.x + angle1.y);
      pos2.z = pos.z + length1 * sin(angle1.x);

      parent = Add_Branch(pos, pos2, rad, parent);

      glm::vec2 angle2;


      if (depth % 2 == 0)
      {
        angle2.y = angle1.y + ANGLE + myrand(RAND);
      }
      else
      {
        angle2.y = angle1.y - ANGLE + myrand(RAND);
      }

      angle2.x = angle1.x + myrand(RAND) - 0.5f * RAND;

      int factor = 80 + rand() % 20;
      float f = factor / 100.f;
      float newrad = rad * decay_rate;

      Make_Tree1(pos2, length * f, angle2, depth - 1, newrad, parent);
    }
    else
    {
      //Make_Tree_Fan(pos, length, glm::vec2(0.f, PI / 2), 4 + rand() % 6, rad, parent);
    }
  }

  void Tree3D::Make_Tree_Fan(glm::vec3 &pos, float length1, glm::vec2 &angle1, int depth, float rad, unsigned parent)
  {
    float SCALE = 1.0f;
    float ANGLE = 0.5f;
    float RAND = 0.3f;
    if (depth > 0)
    {
      glm::vec3 pos2;
      pos2.x = pos.x + length1 * cos(angle1.y);
      pos2.y = pos.y + length1 * sin(angle1.x + angle1.y);
      pos2.z = pos.z + length1 * sin(angle1.x);

      bool tip = false;
      if (depth == 1)
        tip = true;

      parent = Add_Branch(pos, pos2, rad, parent, tip);

      float newrad = rad * decay_rate;

      //branch 1
      float length2 = length1 * decay_rate;
      glm::vec2 angle2;

      if (depth % 2 != 0)
      {
        angle2.y = angle1.y + ANGLE + myrand(RAND);
        angle2.x = angle1.x + myrand(RAND) - 0.75f * RAND;
      }
      else
      {
        angle2.x = angle1.x + ANGLE + myrand(RAND);
        angle2.y = angle1.y + myrand(RAND) - 0.75f * RAND;
      }

      Make_Tree_Fan(pos2, length2, angle2, depth - 1, newrad, parent);

      //branch 2
      length2 = length1 * decay_rate;
      if (depth % 2 == 0)
      {
        angle2.y = angle1.y - ANGLE + myrand(RAND);
        angle2.x = angle1.x - myrand(RAND) + 0.75f * RAND;
      }
      else
      {
        angle2.x = angle1.x - ANGLE + myrand(RAND);
        angle2.y = angle1.y - myrand(RAND) + 0.75f * RAND;
      }
      Make_Tree_Fan(pos2, length2, angle2, depth - 1, newrad, parent);
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
    enum NormalID
    {
      N_LB = 0,
      N_RB,
      N_RF,
      N_LF
    };

    joints_x.push_back(pos1.x);
    joints_y.push_back(pos1.y);

    glm::vec3 dir_v = pos2 - pos1;
    glm::vec3 dis_s(pos2.y - pos1.y, pos1.x - pos2.x, 0.f);
    glm::vec3 dis_d = glm::cross(dis_s, dir_v);

    dis_s = glm::normalize(dis_s);
    dis_d = glm::normalize(dis_d);

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
    else
    {
      parent_vertices[B_LEFT] = glm::vec3(treeMesh[parent - 24 * 6], treeMesh[parent - 24 * 6 + 1], treeMesh[parent - 24 * 6 + 2]);
      parent_vertices[B_BACK] = glm::vec3(treeMesh[parent - 18 * 6], treeMesh[parent - 18 * 6 + 1], treeMesh[parent - 18 * 6 + 2]);
      parent_vertices[B_RIGHT] = glm::vec3(treeMesh[parent - 12 * 6], treeMesh[parent - 12 * 6 + 1], treeMesh[parent - 12 * 6 + 2]);
      parent_vertices[B_FRONT] = glm::vec3(treeMesh[parent - 6 * 6], treeMesh[parent - 6 * 6 + 1], treeMesh[parent - 6 * 6 + 2]);
    }

    if (branch_tip)
    {
      //single tip
      new_vertices[0] = pos2;
      //calculate normals
      normals[N_LB] = glm::normalize(Physics::Normal(new_vertices[0], parent_vertices[B_BACK], parent_vertices[B_LEFT]));
      normals[N_RB] = glm::normalize(Physics::Normal(new_vertices[0], parent_vertices[B_RIGHT], parent_vertices[B_BACK]));
      normals[N_RF] = glm::normalize(Physics::Normal(new_vertices[0], parent_vertices[B_FRONT], parent_vertices[B_RIGHT]));
      normals[N_LF] = glm::normalize(Physics::Normal(new_vertices[0], parent_vertices[B_LEFT], parent_vertices[B_FRONT]));
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
      normals[N_LB] = glm::normalize(Physics::Normal(new_vertices[B_LEFT], parent_vertices[B_BACK], parent_vertices[B_LEFT]));
      normals[N_RB] = glm::normalize(Physics::Normal(new_vertices[B_BACK], parent_vertices[B_RIGHT], parent_vertices[B_BACK]));
      normals[N_RF] = glm::normalize(Physics::Normal(new_vertices[B_RIGHT], parent_vertices[B_FRONT], parent_vertices[B_RIGHT]));
      normals[N_LF] = glm::normalize(Physics::Normal(new_vertices[B_FRONT], parent_vertices[B_LEFT], parent_vertices[B_FRONT]));
    }

    //add to vertex/index array

    if (branch_tip)
    {
      //add 4 new triangles
      //1
      treeMesh.push_back(new_vertices[0].x); treeMesh.push_back(new_vertices[0].y); treeMesh.push_back(new_vertices[0].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);

      //2
      treeMesh.push_back(new_vertices[0].x); treeMesh.push_back(new_vertices[0].y); treeMesh.push_back(new_vertices[0].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);

      //3
      treeMesh.push_back(new_vertices[0].x); treeMesh.push_back(new_vertices[0].y); treeMesh.push_back(new_vertices[0].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);

      //4
      treeMesh.push_back(new_vertices[0].x); treeMesh.push_back(new_vertices[0].y); treeMesh.push_back(new_vertices[0].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
    }
    else
    {
      //add 8 new triangles
      //1
      treeMesh.push_back(new_vertices[B_LEFT].x); treeMesh.push_back(new_vertices[B_LEFT].y); treeMesh.push_back(new_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      //2
      treeMesh.push_back(new_vertices[B_BACK].x); treeMesh.push_back(new_vertices[B_BACK].y); treeMesh.push_back(new_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      treeMesh.push_back(new_vertices[B_LEFT].x); treeMesh.push_back(new_vertices[B_LEFT].y); treeMesh.push_back(new_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_LB].x); treeMesh.push_back(normals[N_LB].y); treeMesh.push_back(normals[N_LB].z);
      //3
      treeMesh.push_back(new_vertices[B_BACK].x); treeMesh.push_back(new_vertices[B_BACK].y); treeMesh.push_back(new_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(parent_vertices[B_BACK].x); treeMesh.push_back(parent_vertices[B_BACK].y); treeMesh.push_back(parent_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      //4
      treeMesh.push_back(new_vertices[B_RIGHT].x); treeMesh.push_back(new_vertices[B_RIGHT].y); treeMesh.push_back(new_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      treeMesh.push_back(new_vertices[B_BACK].x); treeMesh.push_back(new_vertices[B_BACK].y); treeMesh.push_back(new_vertices[B_BACK].z);
      treeMesh.push_back(normals[N_RB].x); treeMesh.push_back(normals[N_RB].y); treeMesh.push_back(normals[N_RB].z);
      //5
      treeMesh.push_back(new_vertices[B_RIGHT].x); treeMesh.push_back(new_vertices[B_RIGHT].y); treeMesh.push_back(new_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(parent_vertices[B_RIGHT].x); treeMesh.push_back(parent_vertices[B_RIGHT].y); treeMesh.push_back(parent_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      //6
      treeMesh.push_back(new_vertices[B_FRONT].x); treeMesh.push_back(new_vertices[B_FRONT].y); treeMesh.push_back(new_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      treeMesh.push_back(new_vertices[B_RIGHT].x); treeMesh.push_back(new_vertices[B_RIGHT].y); treeMesh.push_back(new_vertices[B_RIGHT].z);
      treeMesh.push_back(normals[N_LF].x); treeMesh.push_back(normals[N_LF].y); treeMesh.push_back(normals[N_LF].z);
      //7
      treeMesh.push_back(new_vertices[B_FRONT].x); treeMesh.push_back(new_vertices[B_FRONT].y); treeMesh.push_back(new_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(parent_vertices[B_FRONT].x); treeMesh.push_back(parent_vertices[B_FRONT].y); treeMesh.push_back(parent_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      //8
      treeMesh.push_back(new_vertices[B_LEFT].x); treeMesh.push_back(new_vertices[B_LEFT].y); treeMesh.push_back(new_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(parent_vertices[B_LEFT].x); treeMesh.push_back(parent_vertices[B_LEFT].y); treeMesh.push_back(parent_vertices[B_LEFT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
      treeMesh.push_back(new_vertices[B_FRONT].x); treeMesh.push_back(new_vertices[B_FRONT].y); treeMesh.push_back(new_vertices[B_FRONT].z);
      treeMesh.push_back(normals[N_RF].x); treeMesh.push_back(normals[N_RF].y); treeMesh.push_back(normals[N_RF].z);
    }
    
    return treeMesh.size();
  }

}