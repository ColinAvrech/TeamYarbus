#include "Sprite.h"

GLfloat verts [] =
{
  // x     y     r     g     b      u     v
  -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
};

GLuint tris [] =
{
  0, 1, 2,
  2, 3, 0
};


// NON-DEFAULT CONSTRUCTOR
// MUST SPECIFY SHADER AND TEXTURE
// MESH AND TRI DATA WILL BE PASSED TO CREATE CUSTOM MESH
// ELSE THE DEFAULT QUAD WILL BE USED
Sprite::Sprite (Shader* _shader, Texture* _texture, GLfloat* _meshData /*= NULL*/, GLuint* _triData /*= NULL*/)
{
  Create_Mesh (_meshData, _triData);

  shaderProgram = _shader;
  texture = _texture;
  shaderProgram->Use ();
}

// DESTRUCTOR
// DELETE ALL BUFFER OBJECTS
Sprite::~Sprite ()
{
  delete vao;
  delete vbo;
  delete ebo;
}


// CALL TO CHANGE SHADER ATTACHED TO SPRITE
void Sprite::Change_Shader (Shader* changeShader)
{
  shaderProgram = changeShader;
  Specify_Attributes ();
  changeShader->Use ();
}


void Sprite::Change_Texture (Texture* changeTexture)
{
  texture = changeTexture;
  Specify_Attributes ();
}


// USED TO COMMUNICATE WITH THE SHADER AND SSPECIFY ATTRIBUTES - POSITION, COLOR, TEX-COORDS
void Sprite::Specify_Attributes ()
{
  GLint posAttrib = glGetAttribLocation (shaderProgram->shaderProgram, "position");
  glEnableVertexAttribArray (posAttrib);
  glVertexAttribPointer (posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

  GLint colAttrib = glGetAttribLocation (shaderProgram->shaderProgram, "color");
  glEnableVertexAttribArray (colAttrib);
  glVertexAttribPointer (colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

  GLint texAttrib = glGetAttribLocation (shaderProgram->shaderProgram, "texcoord");
  glEnableVertexAttribArray (texAttrib);
  glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));

  glUniform1i (glGetUniformLocation (shaderProgram->shaderProgram, "image"), 0);
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
  glBindTexture (GL_TEXTURE_2D, texture->textureID);
  glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
