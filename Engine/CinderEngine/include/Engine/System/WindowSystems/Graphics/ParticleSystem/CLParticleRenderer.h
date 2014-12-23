/******************************************************************************/
/*!
\file   CLParticleRenderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _CL_PARTICLE_RENDERER
#define _CL_PARTICLE_RENDERER

#define MILLION 1000000
#define HT       100000

namespace Framework
{
  class KeyEvent;
  class PauseEvent;


  class CLParticleRenderer : public IGraphicsObject
  {
  public:

    CLParticleRenderer ();
    virtual ~CLParticleRenderer();

    virtual void Serialize (Serializer::DataNode* data);
    virtual void Initialize ();
    virtual void Draw ();
    void OnApplicationPause (PauseEvent* pause);

    void GenerateShaders ();
    void GenerateBuffers ();
    void ResetBuffers ();
    void GenerateTextures ();
    void Set_Cursor_Pos (double, double);
    void ResetPosition ();
    void ResetVelocity ();
    void Interpolate_Colors ();
    void OnKeyPressed (KeyEvent* key);

    const static std::string Name;

  private:
    SSBO* SSBOPos, *SSBOVel;
    VAO* vao;
    ComputeShader* computeshader;
    Texture* texture;
    float radius;
    float radiusMultiplier;
    float minAlpha;
    int particleCount;
    float particleSize;
    float frameDelta = 0.0f;
    float speedMultiplier = 0.15f;
    glm::vec4 color;
    float colVec [3];
    float colorChangeTimer;
    float colorChangeLength;
    glm::vec2 destPos;
  };
}

#endif