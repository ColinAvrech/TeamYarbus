#include "Texture.h"


// DEFAULT CONSTRUCTOR
// WILL BE USE TO DYNAMICALLY GENERATE TEXTURE (FRAME BUFFER)
Texture::Texture ()
{
}


// LOADS TEXTURE FROM SPECIFIED FILE
Texture::Texture (const char* filename)
{
  textureID = Load_Texture (filename);
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
GLuint Texture::Load_Texture(const char* filename)
{
    // Load textures
    //GLuint tex;
    //glGenTextures (1, &tex);
    //glBindTexture (GL_TEXTURE_2D, tex);
  
    int width, height;
    GLuint tex = SOIL_load_OGL_texture
      (
      "Test.bmp",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
      );

    //unsigned char* image = SOIL_load_image (filename, &width, &height, 0, SOIL_LOAD_RGB);
    //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glBindTexture (GL_TEXTURE_2D, tex);
    //SOIL_free_image_data (image);
  
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return tex;
}
