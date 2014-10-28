/******************************************************************************/
/*!
\file   HeatMap.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "HeatMap.h"
#include "ResourceManager.h"
#include "Thermodynamics.h"

namespace Framework
{

  // Constructor
  HeatMap::HeatMap (float w, float h)
  {
    Size.x = w;
    Size.y = h;
  }
  
  // Destructor
  HeatMap::~HeatMap ()
  {
    glDeleteTextures (1, &texture_id);
    glDeleteBuffers (4, vbo);
  }

  void HeatMap::Initialize ()
  {
    const int N = 256;
    Generate_Graph (N);

    vao = new VAO ();
    Generate_Texture (N, N);
    glGenBuffers (4, vbo);
    Generate_Vertex_Buffer ();
    Generate_Temperature_Buffer ();
    Generate_Line_Buffer ();
    Generate_Triangle_Buffer ();

    shader = Resources::RS->Get_Shader ("ColorCodeCell");
    shader->Use ();
    attribute_coord2d = shader->attribLocation ("coord2d");
    attrib_temperature = shader->attribLocation ("temperature");
    uniform_vertex_transform = shader->uniLocation("vertex_transform");
    uniform_texture_transform = shader->uniLocation ("texture_transform");
    uniform_mytexture = shader->uniLocation ("mytexture");
    uniform_color = shader->uniLocation ("color");
    shader->Disable ();
  }

  void HeatMap::Update (double dt)
  {
    shader->Use ();
    vao->bindVAO ();
    glm::mat4 model;
    float offset_x = 0.0;
    float offset_y = 0.0;
    float scale = 1.0;

    model = glm::scale (glm::mat4 (1.0f), glm::vec3 (1.6f, 0.9f, 1));

    glm::mat4 view = glm::lookAt (glm::vec3 (0.0, 0.0, 5.0), glm::vec3 (0.0, 0.0, -1.0), glm::vec3 (0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective (18.0f * 3.14f / 180, 1.0f * 1280 / 720.f, 0.1f, 200.0f);

    glm::mat4 vertex_transform = projection * view * model;
    glm::mat4 texture_transform = glm::translate (glm::scale (glm::mat4 (1.0f), glm::vec3 (scale, scale, 1)), glm::vec3 (offset_x, offset_y, 0));

    glUniformMatrix4fv (uniform_vertex_transform, 1, GL_FALSE, glm::value_ptr (vertex_transform));
    glUniformMatrix4fv (uniform_texture_transform, 1, GL_FALSE, glm::value_ptr (texture_transform));

    /* Set texture wrapping mode */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* Set texture interpolation mode */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ///* Draw the triangles, a little dark, with a slight offset in depth. */
    GLfloat grey [4] = { 1.0, 1.0, 1.0, 1 };
    glUniform4fv (uniform_color, 1, grey);

    glEnableVertexAttribArray (attrib_temperature);
    glEnableVertexAttribArray (attribute_coord2d);

    float x = -1.0f;
    float y = 1.0f;
    for (int i = 0; i < 101; ++i)
    {
      for (int j = 0; j < 101; ++j)
      {
        //std::cout << x << ", " << y << "\n";
        temperatures [i][j] = Physics::THERMODYNAMICS->GetCellTemperature (x, y);
        x += 1.f / 50;
      }

      y -= 1.f / 50;
      x = -1.0f;
    }

    glBindBuffer (GL_ARRAY_BUFFER, vbo [1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof temperatures, temperatures, GL_STREAM_DRAW);
    glVertexAttribPointer (attrib_temperature, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo [0]);
    glVertexAttribPointer (attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }

  void HeatMap::Draw ()
  {
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo [3]);
    glDrawElements (GL_TRIANGLES, 100 * 100 * 6, GL_UNSIGNED_SHORT, 0);

    /* Stop using the vertex buffer object */
    glDisableVertexAttribArray (attribute_coord2d);
    glDisableVertexAttribArray (attrib_temperature);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    vao->unbindVAO ();
  }

  void HeatMap::Generate_Graph (const int N)
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        float x = float ((i - N / 1) / (N / 1));
        float y = float ((j - N / 1) / (N / 1));
        float d = 1;
        float z = 1;

        graph [i][j] = GLbyte (roundf (z * 127 + 128));
      }
    }
  }

  void HeatMap::Generate_Texture(int w, int h)
{
    /* Upload the texture with our datapoints */
    glActiveTexture (GL_TEXTURE0);
    glGenTextures (1, &texture_id);
    glBindTexture (GL_TEXTURE_2D, texture_id);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, graph);
  }

  void HeatMap::Generate_Vertex_Buffer ()
  {
    for (int i = 0; i < Size.y; i++)
    {
      for (int j = 0; j < Size.x; j++)
      {
        vertices [i][j].x = (j - 50) / 50.0f;
        vertices [i][j].y = (i - 50) / 50.0f;
      }
    }

    // Copy Vertices Data to Vertex Buffer
    glBindBuffer (GL_ARRAY_BUFFER, vbo [0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
  }

  void HeatMap::Generate_Temperature_Buffer ()
  {
    for (int i = 0; i < Size.y; i++)
    {
      for (int j = 0; j < Size.x; j++)
      {
        temperatures [i][j] = float (rand () % 300);
      }
    }

    // Copy Temperature Data to Temperature Buffer With Stream Draw
    glBindBuffer (GL_ARRAY_BUFFER, vbo [1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof temperatures, temperatures, GL_STREAM_DRAW);
  }

  void HeatMap::Generate_Line_Buffer ()
  {
    int i = 0;

    for (int y = 0; y < Size.y - 1; y++)
    {
      for (int x = 0; x < Size.x - 1; x++)
      {
        indices [i++] = GLushort (y * Size.x + x);
        indices [i++] = GLushort (y * Size.x + x + 1);
      }
    }

    for (int x = 0; x < Size.x; x++)
    {
      for (int y = 0; y < Size.x; y++)
      {
        indices [i++] = GLushort (y * Size.x + x);
        indices [i++] = GLushort ((y + 1) * Size.y + x);
      }
    }

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo [2]);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr ((Size.x - 1) * Size.x * 4 * sizeof *indices), indices, GL_STATIC_DRAW);
  }

  void HeatMap::Generate_Triangle_Buffer ()
  {
    // Create another array of indices that describes all the triangles needed to create a completely filled surface
    int i = 0;

    for (int y = 0; y < Size.y; ++y)
    {
      for (int x = 0; x < Size.x - 1; ++x)
      {
        indices [i++] = GLushort (y * Size.x + x);
        indices [i++] = GLushort (y * Size.y + x + 1);
        indices [i++] = GLushort ((y + 1) * Size.y + x + 1);

        indices [i++] = GLushort (y * Size.y + x);
        indices [i++] = GLushort ((y + 1) * Size.x + x + 1);
        indices [i++] = GLushort ((y + 1) * Size.x + x);
      }
    }

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo [3]);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
  }

}