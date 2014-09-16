#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"
#include "Rect.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"



class Sprite
{
public:
  Sprite (Shader* _shader, Texture* _texture, GLfloat* _meshData = NULL, GLuint* _triData = NULL);
  ~Sprite ();

  void Change_Shader (Shader* changeShader);
  void Change_Texture (Texture* changeTexture);


  void Specify_Attributes ();


  void Draw ();

private:
  // HOLDS ALL MESH DATA
  // SET TO DEFAULT MESH DATA IN DEFAULT CONSTRUCTOR
  // FOR CUSTOM MESH, USE THE DEFAULT PARAMETERS
  GLfloat* spriteData;
  GLuint*  faceData;

  // POINTER TO SHADER AND TEXTURE ATTACHED TO SPRITE
  Shader* shaderProgram;
  Texture* texture;
  VAO* vao;
  VBO* vbo;
  EBO* ebo;

  void Create_Mesh (GLfloat*, GLuint*);
};


#endif
