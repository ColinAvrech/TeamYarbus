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
    void BufferSubData (GLuint size, GLuint offset, const void* data);
    
    template <typename T>
    T* MapBufferRange (int offset, int size)
    {
      return (T*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, offset * sizeof (T), size * sizeof (T), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
    void UnMapBuffer ();
    void UnBindBuffer ();
    void DeleteBuffer ();

    inline GLuint Get_POS ()
    {
      return bufferPos;
    }
    bool IsBuffer ();
  private:
    GLuint bufferPos;
  };

  typedef ShaderStorageBufferObject SSBO;

}

#endif