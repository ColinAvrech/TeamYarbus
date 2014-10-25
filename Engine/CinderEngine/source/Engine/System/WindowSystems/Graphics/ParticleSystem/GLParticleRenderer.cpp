/******************************************************************************/
/*!
\file   GLParticleRenderer.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "GLParticleRenderer.h"
#include "Particles.h"
#include "glew.h"
#include <assert.h>
#include "ResourceManager.h"

namespace Framework
{

  // GENERATE BUFFERS FOR THE ENTIRE PARTICLE SYSTEM
  // RIGHT NOW PARTICLES ARE JUST POINTS
  // ONCE WE GET A GEOMETRY SHADER, WE WILL HAVE DYNAMICALLY CREATED QUADS!
  void GLParticleRenderer::generate (ParticleSystem *sys, bool)
  {
    assert (sys != nullptr);

    m_system = sys;

    const size_t count = sys->numAllParticles ();

    vao = new VAO ();
    cs = Resources::RS->Get_ComputeShader ("ParticleSystem.cs.glsl");
    shader = Resources::RS->Get_Shader ("Particle");

    vao = new VAO ();

#pragma region Buffer
    glGenBuffers (1, &m_bufPos);
    glBindBuffer (GL_ARRAY_BUFFER, m_bufPos);
    glBufferData (GL_ARRAY_BUFFER, sizeof(float) * 4 * count, nullptr, GL_STATIC_DRAW);
    glEnableVertexAttribArray (0);

    if (GL_ARB_vertex_attrib_binding)
    {
      glBindVertexBuffer (0, m_bufPos, 0, sizeof(float) * 4);
      glVertexAttribFormat (0, 4, GL_FLOAT, GL_FALSE, 0);
      glVertexAttribBinding (0, 0);
    }
    else
      glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, (4)*sizeof(float), (void *) ((0)*sizeof(float)));

    glBindVertexArray (0);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
#pragma endregion

    SSBOCol = new SSBO (count * sizeof (glm::vec4));
    ResetColor (count);
    SSBOCol->BindBufferBase (0);
  }


  void GLParticleRenderer::ResetColor(int particleCount)
{
    glm::vec4* verticesCol = SSBOCol->MapBufferRange<glm::vec4> (0, particleCount);
    for (int i = 0; i < particleCount; i++)
    {
      verticesCol [i].x = 0.0f;
      verticesCol [i].y = 0.0f;
      verticesCol [i].z = 0.0f;
      verticesCol [i].w = 1.0f;
    }
    SSBOCol->UnMapBuffer ();
  }


  void GLParticleRenderer::destroy ()
  {
    if (m_bufPos != 0)
    {
      glDeleteBuffers (1, &m_bufPos);
      m_bufPos = 0;
    }

    if (m_bufCol != 0)
    {
      glDeleteBuffers (1, &m_bufCol);
      m_bufCol = 0;
    }
    delete vao;
  }


  void GLParticleRenderer::update ()
  {
    assert (m_system != nullptr);
    //assert (m_bufPos > 0 && m_bufCol > 0);

    const size_t count = m_system->numAliveParticles ();
    if (count > 0)
    {
      glBindBuffer (GL_ARRAY_BUFFER, m_bufPos);
      float* ptr = (float*) (m_system->finalData ()->m_pos.get ());
      glBufferSubData (GL_ARRAY_BUFFER, 0, count*sizeof(float) * 4, ptr);
      //glBindBuffer (GL_ARRAY_BUFFER, m_bufCol);
      //ptr = (float*) (m_system->finalData ()->m_col.get ());
      //glBufferSubData (GL_ARRAY_BUFFER, 0, count*sizeof(float) * 4, ptr);
      //glBindBuffer (GL_ARRAY_BUFFER, 0);
    }
  }

  void GLParticleRenderer::render ()
  {
    vao->bindVAO ();

    const size_t count = m_system->numAliveParticles ();

    SSBOCol->BindBufferBase (0);
    cs->Use ();
    cs->uni1i ("particleCount", count);
    int workingGroups = count / 16;
    cs->Dispatch_Compute (workingGroups + 1, 1, 1);
    cs->Disable ();
    // Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
    glMemoryBarrier (GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    shader->Use ();
    shader->uni1f ("size", 30.0f);
    GLuint colAttrib = shader->attribLocation ("vColor");
    glBindBuffer (GL_ARRAY_BUFFER, SSBOCol->Get_POS ());
    shader->vertexAttribPtr (colAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    shader->enableVertexAttribArray (colAttrib);

    if (count > 0)
      glDrawArrays (GL_POINTS, 0, count);

    vao->unbindVAO ();
  }
}