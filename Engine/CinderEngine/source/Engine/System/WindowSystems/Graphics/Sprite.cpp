#include "Sprite.h"

GLfloat verts [] =
{
  //  Position   Color             Texcoords
  -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Top-right
  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
};

GLuint tris [] =
{
  0, 1, 2,
  2, 3, 0
};

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


void Sprite::Create (GLuint _shader, GLuint _texture, GLfloat* _meshData /*= NULL*/, GLuint* _triData /*= NULL*/)
{
  shaderID = _shader;
  textureID = _texture;
  Use_Shader (shaderID);
  Specify_Attributes ();
  drawable = true;
}

void Sprite::Create (GLuint _shader)
{
  shaderID= _shader;
  textureID = TEXTURE_NONE;
  Use_Shader (shaderID);
  Specify_Attributes ();
}

// DESTRUCTOR
// DELETE ALL BUFFER OBJECTS
Sprite::~Sprite ()
{
}


// CALL TO CHANGE SHADER ATTACHED TO SPRITE
void Sprite::Change_Shader (GLuint changeShader)
{
  shaderID = changeShader;
  Specify_Attributes ();
}


void Sprite::Change_Texture (GLuint _textureID)
{
  textureID = _textureID;
  Specify_Attributes ();
}


// USED TO COMMUNICATE WITH THE SHADER AND SSPECIFY ATTRIBUTES - POSITION, COLOR, TEX-COORDS
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
    DrawFunction = &Sprite::Draw_Texture;
  }
  else
  {
    DrawFunction = &Sprite::Draw_No_Texture;
  }
}


void Sprite::Create_Mesh (GLfloat* vertices, GLuint* faces)
{
  vao = new VAO ();
  if (vertices == NULL)
  {
    vbo = new VBO (sizeof(verts), verts);
    spriteData = verts;
  }
  else
  {
    spriteData = new GLfloat [48];
    spriteData = vertices;
    vbo = new VBO (sizeof(spriteData), spriteData);
  }

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
  }
}


// CALLED BY RENDERER
void Sprite::Draw ()
{
  (this->*DrawFunction)();
}


void Sprite::Draw_Texture ()
{
  Use_Shader (shaderID);
  glBindTexture (GL_TEXTURE_2D, textureID);
  glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void Sprite::Draw_No_Texture ()
{
  Use_Shader (shaderID);
  glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::Use_Shader (GLuint _shaderID)
{
  glUseProgram (_shaderID);
}
