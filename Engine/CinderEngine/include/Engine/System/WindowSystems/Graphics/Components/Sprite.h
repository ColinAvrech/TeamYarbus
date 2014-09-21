#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"

#define TEXTURE_NONE -100

namespace Framework
{

  class Sprite : public Component
  {
  public:
    Sprite (GameObject* go);
    Sprite (GameObject* go, Shader* _shader, Texture* _texture = NULL);
    ~Sprite ();

    void Create (Shader* _shader, Texture* _texture = NULL);
    void Change_Shader (Shader* _shaderID);
    void Change_Texture (Texture* _textureID);
    void Update_Shader ();

    Shader* Get_Shader ();
    Texture* Get_Texture ();
    void Specify_Attributes ();


    void Draw ();
    glm::mat4 modelMatrix;
  private:

    Shader* shader;
    Texture* texture;
    bool drawable;
    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    (void) (Sprite::*DrawFunction)(void);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Use_Shader (GLuint shaderID);
  };

}
#endif
