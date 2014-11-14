/******************************************************************************/
/*!
\file   Sprite.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _SPRITE_H
#define _SPRITE_H

#include "GraphicsCommon.h"
#include "IGraphicsObject.h"
#include "SpriteSheet.h"
#include "JSONSerializer.h"
#include "ZilchCompiledlib.h"
#include "Zilch.hpp"

#define TEXTURE_NONE -100

namespace Framework
{
  class Sprite : public IGraphicsObject
  {
  public:
    ZilchDeclareBaseType(Sprite, Zilch::TypeCopyMode::ReferenceType);

    Sprite () {}
    virtual ~Sprite();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    virtual void Draw ();

    void Create_Sprite (Shader* _shader, Texture* _texture = NULL);
    void Create_Sprite (Shader* _shader, SpriteSheet* _atlas);
    void Change_Shader (Shader* _shaderID);
    void Change_Shader (Zilch::String);
    void Change_Texture (Texture* _textureID);
    void Change_Texture (Zilch::String);
    void Update_Shader ();

    Shader* Get_Shader ();
    Texture* Get_Texture ();
    void Specify_Attributes ();

    const static std::string Name;

  private:

    // Resources
    Shader* shader;
    Texture* texture;
    SpriteSheet* atlas;
    glm::vec4 color;
    GLenum BlendMode;

    // Animated Sprites
    bool animated;
    GLuint uniTexOffset;
    GLuint uniFrameRatio;
    glm::vec2 texOffset;
    glm::vec2 frameRatio;
    unsigned frameNumber;

    // Helper Functions
    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    void Use_Shader (GLuint shaderID);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Draw_Animated ();
    void Change_Color (float r, float g, float b, float a);
    int GetCurrentFrame ();
    int GetAnimationSpeed ();
    // Function Pointer - Draw Texture - Draw Solid Color - Draw Animated
    (void) (Sprite::*DrawFunction)(void);

    // Buffer Objects
    // One Buffer for all Quads
    static VAO* vao;
    static VBO* vbo;
    static EBO* ebo;
  };

}
#endif
