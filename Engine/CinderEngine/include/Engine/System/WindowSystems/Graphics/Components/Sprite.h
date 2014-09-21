#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"
#include "SpriteSheet.h"

#define TEXTURE_NONE -100

namespace Framework
{

  class Sprite : public Component
  {
  public:
    Sprite (GameObject* go);
    Sprite (GameObject* go, Shader* _shader, Texture* _texture = NULL);
    Sprite (GameObject* go, Shader* _shader, SpriteSheet* _atlas);
    ~Sprite ();

    void Create (Shader* _shader, Texture* _texture = NULL);
    void Create (Shader* _shader, SpriteSheet* _atlas);
    void Change_Shader (Shader* _shaderID);
    void Change_Texture (Texture* _textureID);
    void Update_Shader ();

    Shader* Get_Shader ();
    Texture* Get_Texture ();
    void Specify_Attributes ();


    void Draw ();
    glm::mat4 modelMatrix;
  private:
    GLuint uniTexOffset;
    GLuint uniFrameRatio;
    glm::vec2 texOffset;
    glm::vec2 frameRatio;
    unsigned frameNumber;
    Shader* shader;
    Texture* texture;
    SpriteSheet* atlas;
    bool animated;
    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    (void) (Sprite::*DrawFunction)(void);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Draw_Animated ();
    void Use_Shader (GLuint shaderID);
  };

}
#endif
