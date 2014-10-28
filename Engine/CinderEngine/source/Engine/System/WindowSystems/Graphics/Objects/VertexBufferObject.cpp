/******************************************************************************/
/*!
\file   VertexBufferObject.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "VertexBufferObject.h"

namespace Framework
{
  void VertexBufferObject::createVBO (int _size)
  {
    glGenBuffers (1, &vbo);
    data.reserve (_size);
    bufferSize = _size;
  }


  void VertexBufferObject::createMultiple (int _size, GLuint* ptr)
  {
    glGenBuffers (_size, ptr);
  }

  void VertexBufferObject::releaseVBO ()
  {
    glDeleteBuffers (1, &vbo);
    bDataUploaded = false;
    data.clear ();
  }

  void* VertexBufferObject::mapBufferToMemory (int iUsageHint)
  {
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBuffer (bufferType, iUsageHint);
    return ptrRes;
  }

  void* VertexBufferObject::mapSubBufferToMemory (int iUsageHint, GLuint uiOffset, GLuint uiLength)
  {
    if (!bDataUploaded)return NULL;
    void* ptrRes = glMapBufferRange (bufferType, uiOffset, uiLength, iUsageHint);
    return ptrRes;
  }

  void VertexBufferObject::unmapBuffer ()
  {
    glUnmapBuffer (bufferType);
  }

  void VertexBufferObject::bindVBO(GLenum type /*= GL_ARRAY_BUFFER*/)
{
    bufferType = type;
    glBindBuffer (bufferType, vbo);
  }

  void VertexBufferObject::unBindVBO ()
  {
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }

  void VertexBufferObject::uploadDataToGPU(GLenum drawMode)
{
    glBufferData (bufferType, data.size (), &data [0], drawMode);
    bDataUploaded = true;
    data.clear ();
  }

  void VertexBufferObject::addData (void* dataPtr, GLuint size)
  {
    data.insert (data.end (), (unsigned char*) dataPtr, (unsigned char*) dataPtr + size);
  }

  void* VertexBufferObject::getDataPointer ()
  {
    if (bDataUploaded)return NULL;
    return (void*) data [0];
  }

  GLuint VertexBufferObject::getBuffer ()
  {
    return vbo;
  }
}
