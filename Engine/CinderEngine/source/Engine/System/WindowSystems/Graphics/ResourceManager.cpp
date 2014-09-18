#include "ResourceManager.h"
#include <algorithm>
#include <fstream>


//ResourceManager::ResourceManager ()
//{
//}


ResourceManager::~ResourceManager ()
{
  for (auto i : shaders)
  {
    delete (i.second);
  }
  for (auto i : textures)
  {
    delete (i.second);
  }
}

void ResourceManager::Load_Resources ()
{
  Load_Textures ();
  Load_Shaders ();
}

void ResourceManager::Load_Textures ()
{
  textures ["Default"] = new Texture ((TextureResourcePath + "Default.jpg").c_str ());
  std::ifstream texFile (TextureResourcePath + "TextureAssets.txt");

  if (!texFile.good ())
  {
    std::cout << "Failed to Load Textures...\n";
    return;
  }
  else
  {
    std::string str;
    while (!texFile.eof ())
    {
      texFile >> str;
      textures [str] = new Texture ((TextureResourcePath + str).c_str());
      std::cout << str << std::endl;
    }
  }
}


void ResourceManager::Load_Shaders ()
{
  shaders ["Default"] = new Shader ((ShaderResourcePath + "Basic.vs").c_str (), (ShaderResourcePath + "Basic.frag").c_str ());

  std::ifstream vertexShaderFile (ShaderResourcePath + "VertexShaders.txt");
  std::ifstream fragShaderFile (ShaderResourcePath + "FragmentShaders.txt");

  if (!vertexShaderFile.good() || !fragShaderFile.good())
  {
    std::cout << "Failed to load Shader File" << std::endl;
    return;
  }

  else
  {
    std::string vs;
    std::string fs;
    while (!vertexShaderFile.eof() && !fragShaderFile.eof())
    {
      vertexShaderFile >> vs;
      fragShaderFile >> fs;
      shaders [fs] = new Shader (((ShaderResourcePath + vs).c_str ()), (ShaderResourcePath + fs).c_str ());
    }
  }
}

Texture* ResourceManager::Get_Texture(std::string textureName)
{
  //return textures [textureName];
  for (auto i : textures)
  {
    if (i.first == textureName)
    {
      return i.second;
    }
  }

  return textures ["Default"];
}

Shader* ResourceManager::Get_Shader(std::string shaderName)
{
  //return textures [textureName];
  for (auto i : shaders)
  {
    if (i.first == shaderName)
    {
      return i.second;
    }
  }

  return shaders ["Default"];
}
