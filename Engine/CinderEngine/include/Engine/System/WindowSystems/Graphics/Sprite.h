#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"
#include "SpriteData.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

#define TEXTURE_NONE -100

namespace Framework
{

  class Sprite
  {
  public:
    Sprite ();
    Sprite (GLuint _shader, GLuint _texture = TEXTURE_NONE, GLfloat* _meshData = NULL, GLuint* _triData = NULL);
    ~Sprite ();

    void Create (GLuint _shader, GLuint _texture, GLfloat* _meshData = NULL, GLuint* _triData = NULL);
    void Create (GLuint _shader);
    void Change_Shader (GLuint _shaderID);
    void Change_Texture (GLuint _textureID);


    void Specify_Attributes ();


    void Draw ();
    GLuint shaderID;
    GLuint textureID;
  private:
    // HOLDS ALL MESH DATA
    // SET TO DEFAULT MESH DATA IN DEFAULT CONSTRUCTOR
    // FOR CUSTOM MESH, USE THE DEFAULT PARAMETERS
    //GLfloat* spriteData;
    //GLuint*  faceData;

    std::vector <vertexData> spriteData;
    std::vector <textureData> textureData;
    std::vector <GLuint> faces;

    VAO* vao;
    VBO* vbo;
    EBO* ebo;
    bool drawable;

    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    (void) (Sprite::*DrawFunction)(void);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Use_Shader (GLuint shaderID);
  };

}
#endif
