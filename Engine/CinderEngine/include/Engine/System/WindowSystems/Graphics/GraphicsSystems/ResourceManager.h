/******************************************************************************/
/*!
\file   ResourceManager.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H


#include "Resources.h"
#include "SpriteSheet.h"
#include "ComputeShader.h"
#include <unordered_map>
#include "Font.h"

namespace Framework
{


  class Resources
  {
  public:
	  ZilchDeclareBaseType(Resources, Zilch::TypeCopyMode::ReferenceType);
    Resources();
    ~Resources ();

    void Load_Resources ();

    string TextureResourcePath = "../../Resources/Textures/";
    string SpriteSheetResourcePath = "../../Resources/SpriteSheets/";
    string ShaderResourcePath = "../../Resources/Shaders/";
    string AudioResourcePath = "../../Resources/Audio/";
	string FontResourcePath = "../../Resources/Fonts/";

    Texture* Get_Texture (string textureName);
    SpriteSheet* Get_SpriteSheet (string spriteSheetName);
    Shader* Get_Shader (string shaderName);
    Sound* Get_Sound(string soundName);
	Sound* ZGetSound(String soundName);
    ComputeShader* Get_ComputeShader (string shaderName);
	Font* Get_Font(String fontName);

    static Resources* RS;
	FT_Library  FontLibrary;

  private:
    std::unordered_map <string, Texture*> textures;
    std::unordered_map <string, SpriteSheet*> spriteSheets;
    std::unordered_map <string, Shader*> shaders;
    std::unordered_map<string, ComputeShader*> computeShaders;
    std::unordered_map <string, Sound*> sounds;
	std::unordered_map <string, Font*> fonts;
    void Load_Textures ();
    void Load_SpriteSheets ();
    void Load_Shaders ();
    void Load_Sounds ();
	void Load_Fonts();
  };


}

#endif // !_RESOURCE_MANAGER_H

