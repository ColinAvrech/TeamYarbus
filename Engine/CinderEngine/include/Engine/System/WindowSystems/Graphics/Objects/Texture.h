#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GraphicsCommon.h"
#include "SOIL.h"

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

  protected:
    GLuint textureID;
    int width;
    int height;
    float aspect;

  };

}

#endif

