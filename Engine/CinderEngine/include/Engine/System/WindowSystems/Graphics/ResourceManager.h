#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H


#include "Resources.h"
#include <unordered_map>

class ResourceManager
{
public:
  ResourceManager ()
  {
    //Load_Resources ();
  }
  ~ResourceManager ();

  void Load_Resources ();
  Texture* Get_Texture (std::string textureName);
  Shader* Get_Shader (std::string shaderName);
  std::string TextureResourcePath = "Resources/Textures/";
  std::string ShaderResourcePath = "Resources/Shaders/";

private:
  //std::unordered_map <std::string, Texture*> textures;
  //std::unordered_map <std::string, Shader*> shaders;

  std::unordered_map <std::string, Texture*> textures;
  std::unordered_map <std::string, Shader*> shaders;
  void Load_Textures ();
  void Load_Shaders ();
};
#endif // !_RESOURCE_MANAGER_H

