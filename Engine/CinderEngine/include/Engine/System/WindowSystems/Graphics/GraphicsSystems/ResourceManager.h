#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H


#include "Resources.h"
#include "SpriteSheet.h"
#include "ComputeShader.h"
#include <unordered_map>

namespace Framework
{


  class Resources
  {
  public:
    Resources();
    ~Resources ();

    void Load_Resources ();

    std::string TextureResourcePath = "../../Resources/Textures/";
    std::string SpriteSheetResourcePath = "../../Resources/SpriteSheets/";
    std::string ShaderResourcePath = "../../Resources/Shaders/";
    std::string AudioResourcePath = "../../Resources/Audio/";

    Texture* Get_Texture (std::string textureName);
    SpriteSheet* Get_SpriteSheet (std::string spriteSheetName);
    Shader* Get_Shader (std::string shaderName);
    Sound* Get_Sound(std::string soundName);
    ComputeShader* Get_ComputeShader (std::string shaderName);

    static Resources* RS;

  private:
    std::unordered_map <std::string, Texture*> textures;
    std::unordered_map <std::string, SpriteSheet*> spriteSheets;
    std::unordered_map <std::string, Shader*> shaders;
    std::unordered_map<std::string, ComputeShader*> computeShaders;
    std::unordered_map <std::string, Sound*> sounds;
    void Load_Textures ();
    void Load_SpriteSheets ();
    void Load_Shaders ();
    void Load_Sounds ();
  };


}

#endif // !_RESOURCE_MANAGER_H

