/******************************************************************************/
/*!
\file   ShaderStorageBufferObject.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SSBO_H
#define _SSBO_H

#include "glew.h"
#include "CinderMath.h"

namespace Framework
{

  class ShaderStorageBufferObject
  {
  public:
    ShaderStorageBufferObject::ShaderStorageBufferObject () : bufferPos (0) {}
    ShaderStorageBufferObject (GLuint size, GLenum mode = GL_STATIC_DRAW);
    ~ShaderStorageBufferObject ();
  
    void GenerateBuffer ();
    void BindBuffer (GLenum mode = GL_SHADER_STORAGE_BUFFER);
    void BindBufferBase (GLuint index = 0);
    void BufferData (GLuint size, const void* data = NULL, GLenum mode = GL_STATIC_DRAW);
    glm::vec4* MapBufferRange (GLuint offset, GLuint size);
    void UnMapBuffer ();
    void UnBindBuffer ();
    void DeleteBuffer ();

    inline GLuint Get_POS ()
    {
      return bufferPos;
    }

  private:
    GLuint bufferPos;
  };

  typedef ShaderStorageBufferObject SSBO;

}

#endif