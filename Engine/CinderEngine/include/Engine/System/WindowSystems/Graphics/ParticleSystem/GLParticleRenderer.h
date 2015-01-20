/******************************************************************************/
/*!
\file   GLParticleRenderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _GL_PARTICLE_RENDERER_H
#define _GL_PARTICLE_RENDERER_H

#include "ParticleRenderer.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ShaderStorageBufferObject.h"
#include "ComputeShader.h"

namespace Framework
{

  // GL PARTICLE RENDERER
  // RENDERS PARTICLES AS POINTS
  class GLParticleRenderer : public IParticleRenderer
  {
	  ZilchDeclareDerivedType(GLParticleRenderer, IParticleRenderer);
    VAO* vao;
    VBO* vbo;
    SSBO* SSBOParticles, *SSBOCol;
    ComputeShader* cs;
    Shader* shader;
  protected:
    ParticleSystem *m_system{ nullptr };

    unsigned int m_bufPos{ 0 };
    unsigned int m_bufCol{ 0 };
    unsigned int m_vao{ 0 };
  public:
    GLParticleRenderer () { }
    ~GLParticleRenderer () { destroy (); }

    void generate (ParticleSystem *sys, bool useQuads) override;
    void destroy () override;
    void update () override;
    void render () override;
    void ResetColor (int particleCount);
    void ResetParticles (int count);
  };
}

#endif