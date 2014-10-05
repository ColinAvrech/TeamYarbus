#ifndef _EBO_H
#define _EBO_H



#include "glew.h"

class ElementBufferObject
{
public:
  template <typename T>
  ElementBufferObject (GLuint bufferSize, T* bufferData)
  {
    glGenBuffers (1, &ebo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
  }
  ~ElementBufferObject ()
  {
    glDeleteBuffers (1, &ebo);
  }

  inline void Bind ()
  {
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
  }

private:
  GLuint ebo;
};

typedef ElementBufferObject EBO;


#endif // !_EBO_H
