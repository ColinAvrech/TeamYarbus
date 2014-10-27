/******************************************************************************/
/*!
\file   FrameBufferObject.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FBO_H
#define _FBO_H

#include "glew.h"

namespace Framework
{

  class FrameBufferObject
  {
  public:
    FrameBufferObject ()
    {
      glGenFramebuffers (1, &fbo);
      glBindFramebuffer (GL_FRAMEBUFFER, fbo);
    }

    void bind ()
    {
      glBindFramebuffer (GL_FRAMEBUFFER, fbo);
    }

    void unBind ()
    {
      glBindFramebuffer (GL_FRAMEBUFFER, 0);
    }

    void attachTexture (GLuint texture, GLenum attachment = GL_COLOR_ATTACHMENT0)
    {
      glFramebufferTexture2D (GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    }

    GLuint getBufferPos ()
    {
      return fbo;
    }

    ~FrameBufferObject ()
    {
      glDeleteFramebuffers (1, &fbo);
    }
  
  private:
    GLuint fbo;
  };

  typedef FrameBufferObject FBO;
}

#endif