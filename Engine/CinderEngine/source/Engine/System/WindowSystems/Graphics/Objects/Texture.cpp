#include "Texture.h"
#include <windows.h>

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
    std::string file = filename;
    bool hasAlpha = false;
    unsigned pos = file.find_last_of ('.');
    std::string format = file.substr (pos + 1, file.size () - 1);
    if (format == "png")
    {
      hasAlpha = true;
    }
    // Load textures
    GLuint texture;
    //texture = SOIL_load_OGL_texture
    //  (
    //  "Data/NeHe.bmp",
    //  SOIL_LOAD_AUTO,
    //  SOIL_CREATE_NEW_ID,
    //  SOIL_FLAG_INVERT_Y
    //  );

    //if (texture == 0)
    //{
    //  std::cout << "Exit";
    //}
    glGenTextures (1, &texture);
    glBindTexture (GL_TEXTURE_2D, texture);
    int w, h;

    unsigned char* image = SOIL_load_image (filename, &w, &h, 0, SOIL_LOAD_AUTO);
    glTexImage2D (GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, w, h, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

    width = w;
    height = h;
    aspect = float (width) / height;
    SOIL_free_image_data (image);

    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    textureID = texture;
  }


  void Texture::Bind_Texture (int num /*=0*/)
  {
    glActiveTexture (GL_TEXTURE0 + num);
    glBindTexture (GL_TEXTURE_2D, textureID);
  }


  void Texture::Release_Texture ()
  {
    glDeleteTextures (1, &textureID);
  }


  GLuint Texture::Get_ID ()
  {
    return textureID;
  }

  float Texture::Get_Aspect_Ratio ()
  {
    return aspect;
  }

}