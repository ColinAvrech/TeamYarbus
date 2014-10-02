#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GraphicsCommon.h"
#include "SOIL2.h"

namespace Framework
{


  class Texture
  {
  public:
    Texture ();
    Texture (const char* filename);
    ~Texture ();

    GLuint Get_ID ();
    float Get_Aspect_Ratio ();
    void Load_Texture (const char* filename);

    void Bind (int num = 0);
    void Unbind (int num = 0);
    void Release_Texture ();

  protected:
    GLuint textureID;
    int width;
    int height;
    float aspect;

  };

}

#endif

