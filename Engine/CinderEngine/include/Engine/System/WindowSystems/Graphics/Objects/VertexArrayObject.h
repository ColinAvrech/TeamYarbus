/******************************************************************************/
/*!
\file   VertexArrayObject.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _VAO_H
#define _VAO_H


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

    inline void bindVAO ()
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
