/******************************************************************************/
/*!
\file   Texture.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GraphicsCommon.h"
#include "SOIL.h"
#include "ZilchCompiledLib.h"

namespace Framework
{


  class Texture
  {
  public:
	ZilchDeclareBaseType(Texture, Zilch::TypeCopyMode::ReferenceType);
    Texture ();
    Texture (const char* filename);
    ~Texture ();

    GLuint Get_ID ();
    float Get_Aspect_Ratio ();
    void Load_Texture (const char* filename);

    void Bind (int num = 0);
    void Unbind (int num = 0);
    void Release_Texture ();
    void Image2D (GLenum format, int w, int h, int border, GLenum type, const void* pixels);
	int width;
    int height;

	Zilch::String Name;
  protected:
    GLuint textureID;
    
    float aspect;

  };

}

#endif

