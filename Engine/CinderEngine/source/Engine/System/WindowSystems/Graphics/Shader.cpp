#include "Shader.h"


Shader::Shader (const char* vs, const char* fs, const char* gs/*= NULL*/)
{
  vertexShader = Create_Shader (vs, GL_VERTEX_SHADER);
  fragmentShader = Create_Shader (fs, GL_FRAGMENT_SHADER);
  shaderProgram = Create_Program (vertexShader, fragmentShader, geometryShader);
}


Shader::~Shader ()
{
  glDetachShader (shaderProgram, vertexShader);
  glDetachShader (shaderProgram, fragmentShader);
  glDeleteShader (vertexShader);
  glDeleteShader (fragmentShader);
  glDeleteProgram (shaderProgram);
}


std::string Shader::Read_Shader (const char* filename)
{
  std::ifstream shaderFile (filename);

  if (!shaderFile.good ())
  {
    std::cout << "File cannot be loaded...  " << filename;
    return NULL;
  }

  return std::string (std::istreambuf_iterator<char> (shaderFile),
    std::istreambuf_iterator<char> ());
}


GLuint Shader::Create_Shader (const char* filename, GLenum shaderType)
{
  std::string shaderSource;
  shaderSource = Read_Shader (filename);
  const char* source = shaderSource.c_str ();
  GLuint shader = glCreateShader (shaderType);
  glShaderSource (shader, 1, &source, NULL);
  glCompileShader (shader);
  char log [1000];
  glGetShaderInfoLog (shader, 1000, NULL, log);

  std::cout << "Compile Status:\n" << log << std::endl;

  return shader;
}


GLuint Shader::Create_Program (GLuint _vertexShader, GLuint _fragmentShader, GLuint _geometryShader/*= 0*/)
{
  GLuint program = glCreateProgram ();
  glAttachShader (program, _vertexShader);
  glAttachShader (program, _fragmentShader);
  glLinkProgram (program);

  return program;
}
