/*****************************************************************************/
/*!
\file   Terrain3D.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate terrains (for thermo testing)
*/
/*****************************************************************************/

#include <Precompiled.h>
#include "Terrain3D.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Thermodynamics.h"
#include "PhysicsLibrary.h"
#include "random.hpp"

META_DEFINE( Framework::Terrain3D, Terrain3D )
{

}

namespace Framework
{
  static VAO* vao1;
  static VBO* vbo1;
  static float time = 0.0f;

  DefineComponentName(Terrain3D);

  //Constructor
  Terrain3D::Terrain3D()
  {}

  //Destructor
  Terrain3D::~Terrain3D()
  {
    delete vao, vbo, tc;
    delete vao1, vbo1;
    vao = nullptr;
    vbo = nullptr;
    tc = nullptr;
    vao1 = nullptr;
    vbo1 = nullptr;
  }

  void Terrain3D::Serialize(Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    Serializer::DataNode* value = data->FindElement(data, "MapSize");
    value->GetValue(&MapSize);

    value = data->FindElement(data, "MapDepth");
    if (value != nullptr)
      value->GetValue(&MapDepth);
    else
      MapDepth = MapSize;

    value = data->FindElement(data, "BaseHeight");
    value->GetValue(&BaseHeight);

    value = data->FindElement(data, "PeakHeight");
    value->GetValue(&PeakHeight);

    value = data->FindElement(data, "Passes");
    value->GetValue(&Passes);

    value = data->FindElement(data, "Waves");
    value->GetValue(&Waves);

    value = data->FindElement(data, "Color1");
    value->GetValue(&color1);

    value = data->FindElement(data, "Color2");
    if (value != nullptr)
      value->GetValue(&color2);
    else
      color2 = color1;
  }

  void Terrain3D::Initialize()
  {
    Generate_Height_Points();
    Generate_Vertices();
    Generate_Buffers();

    vao1 = new VAO();
  }

  void Terrain3D::Draw()
  {
    time += 0.016f;
    //glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    
    assert(shader && "Invalid Shader!!!");
    shader->Use();
      
    assert(vao && "VAO doesn't exist!!!");
    vao->bindVAO();

    shader->uniMat4("mvp", glm::value_ptr(static_cast<Transform*>(gameObject->GetComponent("Transform"))->GetModelViewProjectionMatrix()));
    shader->uni4f("color1", color1.r, color1.g, color1.b, color1.a);
    shader->uni4f("color2", color2.r, color2.g, color2.b, color2.a);
    shader->uni3f("lightDir", std::sin(time / 10.f), std::cos(time / 10.f), -2.f);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

    vao->unbindVAO();
      
    shader->Disable();
    OPENGL->ResetBlendMode();
  }

  void Terrain3D::Generate_Height_Points()
  {
    std::vector<float> heights;
    tc = new Procedural::TerrainCreator3D(MapSize, MapDepth, BaseHeight, Passes, Waves, PeakHeight);
    Procedural::TerrainCreator3D& t = *tc;
    float* Map = t.GetMap();
   
    float offsetX = -1.0f;
    float offsetY = -1.0f;
    float offsetZ = -1.0f;

    float nX = 2.0f / (MapSize - 1);
    //float nY = 1.0f / (PeakHeight);
    float nZ = 2.0f / (MapDepth - 1);

    for (int j = 0; j < MapDepth; ++j)
    {
      offsetX = -1.f;
      for (int i = 0; i < MapSize; ++i)
      {
        offsetY = (Map[j * MapDepth + i]);

        height_points.push_back({ offsetX, offsetY, offsetZ });

        offsetX += nX;
      }
      offsetZ += nZ;
    } //for
  } //function generate height points

  void Terrain3D::Generate_Vertices()
  {
    //Vertices
    for (unsigned i = 0; i < height_points.size() - (MapDepth + 1); ++i)
    {
      if (i == 0 || (i + 1) % MapDepth != 0)
      {
        glm::vec3 surfaceNormal = Physics::Normal(height_points[i], height_points[i + MapDepth], height_points[i + 1]);
        surfaceNormal = glm::normalize(surfaceNormal);
        //Triangle 1
        //pt1
        vertices.push_back(height_points[i].x);
        vertices.push_back(height_points[i].y);
        vertices.push_back(height_points[i].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);
        //pt2
        vertices.push_back(height_points[i + 1].x);
        vertices.push_back(height_points[i + 1].y);
        vertices.push_back(height_points[i + 1].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);
        //pt3
        vertices.push_back(height_points[i + MapDepth].x);
        vertices.push_back(height_points[i + MapDepth].y);
        vertices.push_back(height_points[i + MapDepth].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);

        surfaceNormal = Physics::Normal(height_points[i + 1], height_points[i + MapDepth], height_points[i + MapDepth + 1]);
        surfaceNormal = glm::normalize(surfaceNormal);
        //Triangle 2
        //pt1
        vertices.push_back(height_points[i + MapDepth + 1].x);
        vertices.push_back(height_points[i + MapDepth + 1].y);
        vertices.push_back(height_points[i + MapDepth + 1].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);
        //pt2
        vertices.push_back(height_points[i + MapDepth].x);
        vertices.push_back(height_points[i + MapDepth].y);
        vertices.push_back(height_points[i + MapDepth].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);
        //pt3
        vertices.push_back(height_points[i + 1].x);
        vertices.push_back(height_points[i + 1].y);
        vertices.push_back(height_points[i + 1].z);
        vertices.push_back(surfaceNormal.x);
        vertices.push_back(surfaceNormal.y);
        vertices.push_back(surfaceNormal.z);
      }
    }
    height_points.clear();
  }

  void Terrain3D::Generate_Buffers()
  {
    vao = new VAO();
    shader = Resources::RS->Get_Shader("Terrain3D");
    shader->Use();

    vbo = new VBO(vertices.size() * sizeof(float), vertices.data());
    GLuint posAttrib = shader->attribLocation("position");
    shader->enableVertexAttribArray(posAttrib);
    shader->vertexAttribPtr(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLuint normalAttrib = shader->attribLocation ("normal");
    shader->enableVertexAttribArray (normalAttrib);
    shader->vertexAttribPtr (normalAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof (float));
    vao->unbindVAO();
  }

} //framework