#include "Sprite.h"

namespace Framework
{

  Sprite::Sprite ()
  {
    drawable = false;
  }


  // NON-DEFAULT CONSTRUCTOR
  // MUST SPECIFY SHADER AND TEXTURE
  // MESH AND TRI DATA WILL BE PASSED TO CREATE CUSTOM MESH
  // ELSE THE DEFAULT QUAD WILL BE USED
  Sprite::Sprite (GLuint _shader, GLuint _texture /*=NULL*/, GLfloat* _meshData /*= NULL*/, GLuint* _triData /*= NULL*/)
  {
    //Create_Mesh (_meshData, _triData);

    shaderID = _shader;
    textureID = _texture;
    Use_Shader (shaderID);
    drawable = true;
  }


  // Sprite can be created using this method of in the Non-Default Constructor
  // Get Shader will return Default shader if wrong name specified
  // Get Texture is optional
  // For Custom Mesh use the next two parameters
  void Sprite::Create (GLuint _shader, GLuint _texture, GLfloat* _meshData /*= NULL*/, GLuint* _triData /*= NULL*/)
  {
    shaderID = _shader;
    textureID = _texture;
    Use_Shader (shaderID);
    Specify_Attributes ();
    drawable = true;
  }


  // Create Sprite Only Using Shader.
  // No Texture - Solid Color Will BW Used
  void Sprite::Create (GLuint _shader)
  {
    shaderID = _shader;
    textureID = TEXTURE_NONE;
    Use_Shader (shaderID);
    Specify_Attributes ();
  }


  // Destructor
  // Destroy All Buffers Used By Sprite
  // This Might Be Used For Derived Classes Which Will Need Special Buffer Objects
  // This Destructor Does Nothing Right Now
  Sprite::~Sprite ()
  {
  }


  // Call To Change Shader Used By Sprite
  void Sprite::Change_Shader (GLuint changeShader)
  {
    shaderID = changeShader;
    Specify_Attributes ();
  }

  // Call To Change Texture Used By Sprite
  void Sprite::Change_Texture (GLuint _textureID)
  {
    textureID = _textureID;
    Specify_Attributes ();
  }


  // Used To Communicate With Shader and Specify Attributes from Vertex Data
  void Sprite::Specify_Attributes ()
  {
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation (shaderID, "position");
    glEnableVertexAttribArray (posAttrib);
    glVertexAttribPointer (posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation (shaderID, "color");
    glEnableVertexAttribArray (colAttrib);
    glVertexAttribPointer (colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    if (textureID != TEXTURE_NONE)
    {
      GLint texAttrib = glGetAttribLocation (shaderID, "texcoord");
      glEnableVertexAttribArray (texAttrib);
      glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));

      glUniform1i (glGetUniformLocation (shaderID, "image"), 0);

      // If Texture Is To Be Used, Use Draw Texture Method To Draw Sprite
      DrawFunction = &Sprite::Draw_Texture;
    }
    else
    {
      // If Texture Is Not Used, Use Draw No Texture Method To Draw Sprite
      DrawFunction = &Sprite::Draw_No_Texture;
    }
  }


  // This Is To Create A Custom Mesh
  // Not Complete
  void Sprite::Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize)
  {
    /*vao = new VAO ();
      spriteData = new GLfloat [arraySize];
      spriteData = vertices;
      vbo = new VBO (sizeof(spriteData), spriteData);

      if (faces == NULL)
      {
      ebo = new EBO (sizeof(tris), tris);
      faceData = tris;
      }
      else
      {
      faceData = new GLuint [6];
      faceData = faces;
      ebo = new EBO (sizeof(faceData), faceData);
      }*/
  }


  // Called By Renderer Component
  void Sprite::Draw ()
  {
    (this->*DrawFunction)();
  }


  // Draw Sprite Using Texture
  void Sprite::Draw_Texture ()
  {
    Use_Shader (shaderID);
    glBindTexture (GL_TEXTURE_2D, textureID);
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }


  // Draw Sprite Without Using Texture
  void Sprite::Draw_No_Texture ()
  {
    Use_Shader (shaderID);
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }


  // Use Shader ID as the Shader
  void Sprite::Use_Shader (GLuint _shaderID)
  {
    glUseProgram (_shaderID);
  }

}