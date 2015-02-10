/******************************************************************************/
/*!
\file   Sprite.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#define TEXTURE_NONE -100

#include "IGraphicsObject.h"
#include "Zilch.hpp"
#include "ZilchCompiledLib.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

#include "MetaCreator.h"

namespace Framework
{
  class Texture;
  class SpriteSheet;

  class Sprite : public IGraphicsObject
  {
  public:

	  META_DECLARE( Sprite );

    ZilchDeclareDerivedType (Sprite, IGraphicsObject);

    Sprite ();
    virtual ~Sprite ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    virtual void Draw ();
    virtual bool Draw_Last() { return false; }
    virtual bool InViewport() { return true; }

    void LoadSprite (Zilch::String texturename, Zilch::String shadername);
    void Create_Sprite (Shader* _shader, Texture* _texture = NULL);
    void Create_Sprite (Shader* _shader, SpriteSheet* _atlas);
    void Change_Shader (Shader* _shaderID);
    void Change_Texture (Texture* _texture);
    void Change_Shader (Zilch::String);
    void Change_Texture (Zilch::String);
    void Update_Shader ();
    void Change_Color (float r, float g, float b, float a);
    Zilch::Real Width;
    Zilch::Real Height;

    Zilch::Real4 GetColor ();

    Shader* Get_Shader ();
    Texture* Get_Texture ();
    void Specify_Attributes ();

    // Public Variables
    vec4 color;

    const static string Name;

  private:

    // Resources
    Texture* texture;
    SpriteSheet* atlas;
    GLenum BlendMode;

    // Animated Sprites
    bool animated;
    GLuint uniTexOffset;
    GLuint uniFrameRatio;
    vec2 texOffset;
    vec2 frameRatio;
    unsigned frameNumber;

    // Helper Functions
    void Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize);
    void Use_Shader (GLuint shaderID);
    void Draw_Texture ();
    void Draw_No_Texture ();
    void Draw_Animated ();
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


//META_DEFINE( Framework::Sprite, Sprite )
//{
//	MEMBER( color );
//}
