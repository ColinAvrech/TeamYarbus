/******************************************************************************/
/*!
\file   FractalGenerator.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _FRACTAL_GENERATOR_H
#define _FRACTAL_GENERATOR_H

#include <vector>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace Framework
{
  class Shader;

  class FractalGenerator
  {
  public:
  FractalGenerator ();
  ~FractalGenerator ();
  
  void Generate_Tree ();
  void Create_Mesh (int lines);
  void Draw ();

  int getTotalLines ()
  {
    return xPositions.size ();
  }

  private:
    std::vector <GLfloat> xPositions;
    std::vector <GLfloat> yPositions;
    std::vector <GLfloat> sizes;
    std::vector <GLfloat> colors;
    std::vector <GLfloat> angles;
    GLfloat treeRed;
    GLfloat treeGreen;

    VAO* vao;
    VBO* vbo;
    Shader* shader;
    std::vector <float> mesh;
    void Generate_Fractal (GLfloat xPos, GLfloat yPos, GLfloat size, GLfloat degrees);
  };  
}

#endif