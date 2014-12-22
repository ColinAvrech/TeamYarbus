/******************************************************************************/
/*!
\file   FluidBody.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "FluidBody.h"
#include "FluidDynamics.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Thermodynamics.h"
#include "random.hpp"
#include "PhysicsSystem.h"
#include "Thermodynamics.h"
#include "TDLib.h"
#include "Pipeline.h"

namespace Framework
{
  static float time = 0.0f;
  static std::vector <float> lineVertices;
  static VAO* vao1;
  static VBO* vbo1;
  static float *leftDeltas;
  static float *rightDeltas;

  DefineComponentName(FluidBody);

  // Constructor
  FluidBody::FluidBody()
  {
    scale = glm::vec2(10, 10);
    color = glm::vec4(0.f, 0.f, 1.f, 1.f);
  }

  // Destructor
  FluidBody::~FluidBody()
  {
    gameObject->FluidBody = nullptr;
    delete vao1, vbo1;
    vao = nullptr;
    vbo = nullptr;
    vao1 = nullptr;
    vbo1 = nullptr;

    delete[] leftDeltas;
    delete[] rightDeltas;
  }


  void FluidBody::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* temp;
    temp = data->FindElement(data, "k");
    if(temp) temp->GetValue(&k);
    temp = data->FindElement(data, "d");
    if(temp) temp->GetValue(&d);
    temp = data->FindElement(data, "Spread");
    if (temp) temp->GetValue(&Spread);
  }

  void FluidBody::Initialize()
  {
    position.x = this->gameObject->Transform->GetPosition().x;
    position.y = this->gameObject->Transform->GetPosition().y;
    Base_Level = 1.f;
    gameObject->FluidBody = this;
    Generate_Height_Points();
    Generate_Vertices();
    Generate_Buffers();
    FLUIDDYNAMICS->AddFluid(this);

    //Allocate helper arrays
    leftDeltas = new float[height_points.size()];
    rightDeltas = new float[height_points.size()];
    //Splash(height_points.size() * 0.5f, -0.5f);
  }

  void FluidBody::Update(const float dt)
  {
    time += dt;
    //test splash
    if (time >= 1.f)
    {
      time = 0.0f;
      //int rand_index = rand() % height_points.size();
      int index1 = height_points.size() * 0.5f;
      Splash(index1, -10.f);
    }

    WaveUpdate(dt);

    for (unsigned int i = 0; i < height_points.size(); ++i)
    {
      //Modify vertex buffer
      vbo->bindVBO();
      glBufferSubData(GL_ARRAY_BUFFER, (6 * i + 1) * sizeof(float), 
        sizeof(float), &height_points[i].y);
      vbo->unBindVBO();
    }
  }

  void FluidBody::Draw()
  {
    glDisable(GL_BLEND);
    shader->Use();
    vao->bindVAO();
    shader->uniMat4("mvp", glm::value_ptr(gameObject->Transform->GetModelViewProjectionMatrix()));
    shader->uni4f("color", color.r, color.g, color.b, color.a);

    glDrawArrays(GL_QUAD_STRIP, 0, vertices.size() / 3);

    vao->unbindVAO();
    shader->Disable();
    OPENGL->ResetBlendMode();
  }

  float FluidBody::GetWidth()
  {
    return scale.x;
  }

  float FluidBody::GetDepth()
  {
    return scale.y;
  }

  void FluidBody::Generate_Height_Points()
  {
    std::vector <float> heights;

    float offsetX = -1.0f;
    float offsetY = -1.0f;
    float nX = 0.2f / scale.x;

    for (int i = 0; i < scale.x * 10; ++i)
    {
      offsetY = Base_Level;
      height_points.push_back({ offsetX, offsetY });
      speeds.push_back(0.f);
      offsetX += nX;
    }
  }

  void FluidBody::Generate_Vertices()
  {
    float y = -1.0f;
    // Vertices
    for (unsigned i = 0; i < height_points.size() - 1; ++i)
    {
      vertices.push_back(height_points[i].x);
      vertices.push_back(height_points[i].y);
      vertices.push_back(0);

      vertices.push_back(height_points[i].x);
      vertices.push_back(y);
      vertices.push_back(0);
    }
  }

  void FluidBody::Generate_Buffers()
  {
    vao = new VAO();
    shader = Resources::RS->Get_Shader("Terrain");
    shader->Use();

    vbo = new VBO(vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    GLuint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    vao->unbindVAO();
  }

  void FluidBody::Splash(unsigned int index, float speed)
  {
    if (index < speeds.size())
      speeds[index] += speed / scale.y;
  }

  void FluidBody::DepthUpdate(const float dt)
  {
    for (unsigned i = 0; i < height_points.size(); ++i)
    {
      float x = height_points[i].y - Base_Level;
      float a = -k * x - d * speeds[i];
      
      speeds[i] += a;
      height_points[i].y += (speeds[i] / scale.y);
    }
  }

  void FluidBody::WaveUpdate(const float dt)
  {
    DepthUpdate(dt);

    for (unsigned int i = 0; i < height_points.size(); i++)
    {
      if (i > 0)
      {
        leftDeltas[i] = Spread * (height_points[i].y - height_points[i - 1].y);
        speeds[i - 1] += leftDeltas[i];
      }
      if (i < height_points.size() - 1)
      {
        rightDeltas[i] = Spread * (height_points[i].y - height_points[i + 1].y);
        speeds[i + 1] += rightDeltas[i];
      }
    }

    for (unsigned int i = 0; i < height_points.size(); i++)
    {
      if (i > 0)
        height_points[i - 1].y += leftDeltas[i];
      if (i < height_points.size() - 1)
        height_points[i + 1].y += rightDeltas[i];
    }

  }
}