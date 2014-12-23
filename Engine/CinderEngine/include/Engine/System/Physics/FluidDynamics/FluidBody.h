/******************************************************************************/
/*!
\file   FireStarter.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#pragma once

using std::vector;

namespace Framework
{
  class FluidBody : public IGraphicsObject
  {
  public:
    FluidBody();
    virtual ~FluidBody();
    virtual void Serialize(Serializer::DataNode*);
    virtual void Initialize();
    virtual void Draw();
    void Update(const float dt);

    void Generate_Height_Points();
    void Generate_Vertices();
    void Generate_Buffers();
    float GetWidth();
    float GetDepth();
    const static string Name;

  private:
    glm::vec2 position;
    glm::vec2 scale;
    float Base_Level;
    //Default properties//
    float k = 0.025f;
    float d = 0.0025f;
    float Spread = 0.25f;
    //////////////////////
    glm::vec4 color1;
    glm::vec4 color2;
    VAO* vao;
    VBO* vbo;
    std::vector <vec2> height_points;
    std::vector<float> speeds;
    std::vector <float> vertices;

    float *leftDeltas;
    float *rightDeltas;

    void Splash(unsigned int index, float speed);
    void DepthUpdate(const float dt);
    void WaveUpdate(const float dt);
  };

}
