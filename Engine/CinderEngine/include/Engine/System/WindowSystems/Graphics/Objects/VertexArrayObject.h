#ifndef _VAO_H
#define _VAO_H

#include "glew.h"

class VertexArrayObject
{
public:
  VertexArrayObject ()
  {
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
  }
  ~VertexArrayObject ()
  {
    glDeleteVertexArrays (1, &vao);
  }

private:
  GLuint vao;
};

typedef VertexArrayObject VAO;

#endif
