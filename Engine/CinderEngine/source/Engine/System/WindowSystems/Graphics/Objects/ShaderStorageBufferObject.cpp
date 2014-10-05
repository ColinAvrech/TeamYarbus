/******************************************************************************/
/*!
\file   ShaderStorageBufferObject.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "ShaderStorageBufferObject.h"

namespace Framework
{

  ShaderStorageBufferObject::ShaderStorageBufferObject (GLuint size, GLenum mode)
  {
    glGenBuffers (1, &bufferPos);
    BindBuffer ();
    BufferData (size, NULL, mode);
  }


  void ShaderStorageBufferObject::GenerateBuffer ()
  {
    glGenBuffers (1, &bufferPos);
  }


  void ShaderStorageBufferObject::BindBuffer(GLenum mode /*= GL_SHADER_STORAGE_BUFFER*/)
{
    glBindBuffer (mode, bufferPos);
  }


  void ShaderStorageBufferObject::BindBufferBase (GLuint index /*= 0*/)
  {
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, index, bufferPos);
  }


  void ShaderStorageBufferObject::BufferData(GLuint size, const void* data /*= NULL*/, GLenum mode /*= GL_STATIC_DRAW*/)
{
    glBufferData (GL_SHADER_STORAGE_BUFFER, size, data, mode);
  }


  ShaderStorageBufferObject::~ShaderStorageBufferObject ()
  {
    DeleteBuffer ();
  }

  glm::vec4* ShaderStorageBufferObject::MapBufferRange(GLuint offset, GLuint size)
  {
    return (glm::vec4*)glMapBufferRange (GL_SHADER_STORAGE_BUFFER, offset, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  }

  void ShaderStorageBufferObject::UnMapBuffer ()
  {
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
  }

  void ShaderStorageBufferObject::UnBindBuffer ()
  {
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, 0);
  }

  void ShaderStorageBufferObject::DeleteBuffer ()
  {
    glDeleteBuffers (1, &bufferPos);
  }

}