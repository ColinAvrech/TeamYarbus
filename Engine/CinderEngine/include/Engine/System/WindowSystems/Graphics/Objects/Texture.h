#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GraphicsCommon.h"
#include "SOIL2.h"
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


	Zilch::String Name;
  protected:
    GLuint textureID;
    int width;
    int height;
    float aspect;

  };

}

#endif

