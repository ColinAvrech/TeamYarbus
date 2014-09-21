#include "Texture.h"

namespace Framework
{

  // DEFAULT CONSTRUCTOR
  // WILL BE USE TO DYNAMICALLY GENERATE TEXTURE (FRAME BUFFER)
  Texture::Texture ()
  {
    glDeleteTextures (1, &textureID);
  }


  // LOADS TEXTURE FROM SPECIFIED FILE
  Texture::Texture (const char* filename)
  {
    Load_Texture (filename);
  }


  // DESTRUCTOR
  // DELETES TEXTURE OBJECT
  Texture::~Texture ()
  {
    glDeleteTextures (1, &textureID);
  }


  // LOADS TEXTURE USING 3RD PARTY LIBRARY - SOIL
  // BINDS TEXTURE
  // RETURNS TEXTURE ID
  void Texture::Load_Texture (const char* filename)
  {
    // Load textures
    GLuint tex;
    glGenTextures (1, &tex);
    glBindTexture (GL_TEXTURE_2D, tex);

    int width, height;

    unsigned char* image = SOIL_load_image (filename, &width, &height, 0, SOIL_LOAD_AUTO);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data (image);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    textureID = tex;
  }


  GLuint Texture::Get_ID ()
  {
    return textureID;
  }

}