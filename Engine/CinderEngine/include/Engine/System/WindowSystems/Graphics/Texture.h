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

    GLuint textureID;
    GLuint imageID;
    void Load_Texture (const char* filename);

  private:
  };

}

#endif

