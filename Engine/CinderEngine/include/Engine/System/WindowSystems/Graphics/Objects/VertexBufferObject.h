#ifndef _VBO_H
#define _VBO_H

#include "glew.h"

class VertexBufferObject
{
public:
  template <typename T>
  VertexBufferObject (GLuint bufferSize, T* bufferData)
  {
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
  }
  ~VertexBufferObject ()
  {
    glDeleteBuffers (1, &vbo);
  }

private:
  GLuint vbo;
};

typedef VertexBufferObject VBO;

#endif

