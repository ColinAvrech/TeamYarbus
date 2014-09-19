/******************************************************************************/
/*!
\file   ShapeData.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SHAPE_DATA_H
#define _SHAPE_DATA_H

#include "GraphicsCommon.h"
#include "VertexData.h"

namespace Framework
{
  struct ShapeData
  {
    ShapeData () : vertices (0), numVertices (0), indices (0), numIndices (0) {}

    void Clean ()
    {
      delete [] vertices;
      delete [] indices;
      numIndices = numVertices = 0;
    }

    GLuint vbo_size ()
    {
      return numVertices * sizeof (VertexData);
    }

    GLuint ibo_size ()
    {
      return numIndices * sizeof (GLuint);
    }

    VertexData* vertices;
    GLuint numVertices;
    GLuint* indices;
    GLuint numIndices;
  };
}

#endif