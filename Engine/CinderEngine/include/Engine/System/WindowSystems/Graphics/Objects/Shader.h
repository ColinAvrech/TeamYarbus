#ifndef _SHADER_H
#define _SHADER_H


#include "GraphicsCommon.h"
#include <string>

namespace Framework
{

  class Shader
  {
  public:
    Shader (const char* vs, const char* fs, const char* gs = 0);
    ~Shader ();

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;
    GLuint shaderProgram;

    void Use ()
    {
      glUseProgram (shaderProgram);
    }

  private:

    std::string Read_Shader (const char* filename);
    GLuint Create_Shader (const char* filename, GLenum shaderType);
    GLuint Create_Shader_From_String ();
    GLuint Create_Program (GLuint _vertexShader, GLuint _fragmentShader, GLuint _geometryShader = 0);

  };

}

#endif // !_SHADER_H