/******************************************************************************/
/*!
\file   HeatMap.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _HEAT_MAP_H
#define _HEAT_MAP_H

#include "GraphicsCommon.h"

namespace Framework
{

  class HeatMap
  {
  public:
  HeatMap (float w, float h);
  ~HeatMap ();
  
  void Initialize ();
  void Update (double dt);
  void Draw ();
  void Generate_Graph (const int N);
  void Generate_Texture (int w, int h);
  void Generate_Vertex_Buffer ();
  void Generate_Temperature_Buffer ();
  void Generate_Line_Buffer ();
  void Generate_Triangle_Buffer ();
  void Update_Temperature_Buffer ();
  private:
    vec2 Size;
    GLbyte graph [256][256];
    vec2 vertices [101][101];
    GLushort indices [100 * 101 * 6];
    float temperatures [101][101];

    VAO* vao;
    GLuint vbo [4];
    Shader* shader;
    GLuint texture_id;
    GLint attribute_coord2d;
    GLint attrib_temperature;
    GLint uniform_vertex_transform;
    GLint uniform_texture_transform;
    GLint uniform_mytexture;
    GLint uniform_color;
  };
}

#endif