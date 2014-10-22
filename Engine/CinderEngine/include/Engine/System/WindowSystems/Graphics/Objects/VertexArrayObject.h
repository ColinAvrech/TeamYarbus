#ifndef _VAO_H
#define _VAO_H

#include "glew.h"

namespace Framework
{
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

    inline void BindVAO ()
    {
      glBindVertexArray (vao);
    }

    inline void unbindVAO ()
    {
      glBindVertexArray (0);
    }

  private:
    GLuint vao;
  };

  typedef VertexArrayObject VAO;
}

#endif
