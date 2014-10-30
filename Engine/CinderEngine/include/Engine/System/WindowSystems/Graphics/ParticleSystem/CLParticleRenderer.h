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

#include "glew.h"
#include "ResourceManager.h"
#include "ShaderStorageBufferObject.h"

#define MILLION 1000000
#define HT       100000

namespace Framework
{
  class KeyEvent;
  class CLParticleEmitter
  {
    glm::vec4 position;
    int particles;
  };


  class CLParticleRenderer
  {
  public:
    int particleCount;
    float particleSize;
    CLParticleRenderer ();
    ~CLParticleRenderer();
    void GenerateShaders ();
    void GenerateBuffers ();
    void ResetBuffers ();
    void GenerateTextures ();
    void Set_Cursor_Pos (double, double);
    void Render ();

    //////////////////////////////////////////////////////////////////////////
    // Properties to be put in Updaters, Generators
    float frameDelta = 0.0f;
    float speedMultiplier = 0.15f;
    bool borderEnabled = true;
    bool colorFade = false;
    bool pause = false;
    float color [4];
    float colVec [3];
    float colorChangeTimer;
    float colorChangeLength;
    void ResetPosition ();
    void ResetVelocity ();
    void Interpolate_Colors ();
    void OnKeyPressed (KeyEvent* key);
    //////////////////////////////////////////////////////////////////////////


  private:
    SSBO* SSBOPos, *SSBOVel;
    VAO* vao;
    Shader* shader;
    ComputeShader* computeshader;
    Texture* texture;
    CLParticleEmitter emitters [3];
    float radius;
  };
}

#endif