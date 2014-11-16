/******************************************************************************/
/*!
\file   ComputeShader.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _COMPUTE_SHADER_H
#define _COMPUTE_SHADER_H

#include "Shader.h"

namespace Framework
{

  class ComputeShader : public Shader
  {
  public:
    ComputeShader (const char* cs);
    ~ComputeShader ();

    GLuint Create_Program (string name, GLuint _computeShader);

    inline void Dispatch_Compute (GLuint workGroups_x, GLuint workGroups_y, GLuint workGroups_z)
    {
      glDispatchCompute (workGroups_x, workGroups_y, workGroups_z);
    }
  
  private:
    GLuint computeShader;
  };  
}

#endif