/******************************************************************************/
/*!
\file   ComputeShader.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "ComputeShader.h"

namespace Framework
{
  ComputeShader::ComputeShader (const char* cs)
  {
    computeShader = Create_Shader (cs, GL_COMPUTE_SHADER);
    Create_Program (computeShader);
  }


  // Destructor
  ComputeShader::~ComputeShader ()
  {
    glDetachShader (shaderProgram, computeShader);
    glDeleteShader (computeShader);
    glDeleteProgram (shaderProgram);
  }

  GLuint ComputeShader::Create_Program (GLuint _computeShader)
  {
    GLuint program = glCreateProgram ();
    glAttachShader (program, computeShader);
    glLinkProgram (program);

    shaderProgram = program;
    computeShader = _computeShader;

    return shaderProgram;
  }

}