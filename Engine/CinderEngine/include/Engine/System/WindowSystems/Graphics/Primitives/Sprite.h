#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"

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
    void Update_Shader ();

    void Specify_Attributes ();


    void Draw ();
    GLuint shaderID;
    GLuint textureID;


  private:

    bool drawable;
    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    (void) (Sprite::*DrawFunction)(void);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Use_Shader (GLuint shaderID);
  };

}
#endif
