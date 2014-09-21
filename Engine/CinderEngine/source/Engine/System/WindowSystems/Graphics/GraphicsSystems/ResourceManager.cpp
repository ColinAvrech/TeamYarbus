#include "ResourceManager.h"
#include <algorithm>
#include <fstream>

namespace Framework
{
  ResourceManager* ResourceManager::RESOURCE_MANAGER;

  ResourceManager::ResourceManager ()
  {
    RESOURCE_MANAGER = this;
  }


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
    Load_Sounds();
    Load_SpriteSheets ();
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
        textures [str] = new Texture ((TextureResourcePath + str).c_str ());
        std::cout << str << std::endl;
      }
    }
  }


  void ResourceManager::Load_SpriteSheets ()
  {
    std::ifstream ssFile (SpriteSheetResourcePath + "SpriteSheetAssets.txt");

    if (!ssFile.good ())
    {
      std::cout << "Failed to Load Sprite Sheets...\n";
      return;
    }
    else
    {
      std::string data;
      std::string filename;
      std::string f;
      int rows, columns, samples;
      //while (!ssFile.eof())
      {
        while (data != ">")
        {
          ssFile >> data;
          ssFile >> data;
          f = data;
          filename = (SpriteSheetResourcePath + data);
          data = "";
          ssFile >> data;
          rows = std::stoi (data);
          data = "";
          ssFile >> data;
          columns = std::stoi (data);
          ssFile >> data;
          samples = std::stoi (data);
          ssFile >> data;
          spriteSheets [f] = new SpriteSheet (filename.c_str(), rows, columns, samples);
        }
      }
    }
  }

  void ResourceManager::Load_Shaders ()
  {
    /*shaders ["Default"] = new Shader ((ShaderResourcePath + "Basic.vs").c_str (), (ShaderResourcePath + "Basic.frag").c_str ());

    std::ifstream vertexShaderFile (ShaderResourcePath + "VertexShaders.txt");
    std::ifstream fragShaderFile (ShaderResourcePath + "FragmentShaders.txt");

    if (!vertexShaderFile.good () || !fragShaderFile.good ())
    {
      std::cout << "Failed to load Shader File" << std::endl;
      return;
    }
    else
    {
      std::string vs;
      std::string fs;
      while (!vertexShaderFile.eof () && !fragShaderFile.eof ())
      {
        vertexShaderFile >> vs;
        fragShaderFile >> fs;
        shaders [fs] = new Shader (((ShaderResourcePath + vs).c_str ()), (ShaderResourcePath + fs).c_str ());
      }
    }*/

    std::ifstream vsList (ShaderResourcePath + "VertexShaders.txt");
    std::ifstream fsList (ShaderResourcePath + "FragmentShaders.txt");
    std::ifstream comp (ShaderResourcePath + "Shaders.txt");

    if (!(vsList.good () || fsList.good () || comp.good ()))
    {
      if (!vsList.good ())
      {
        std::cout << "Failed to load Shader File..." << "VertexShaders.txt\n";
      }
      if (!fsList.good ())
      {
        std::cout << "Failed to load Shader File..." << "FragmentShaders.txt\n";
      }
      if (!comp.good ())
      {
        std::cout << "Failed to load Shader File..." << "Shaders.txt\n";
      }
      return;
    }
    else
    {
      std::string data;
      Shader* s = new Shader ();
      std::unordered_map <std::string, std::string> vSource;
      std::unordered_map <std::string, std::string> fSource;

      while (!vsList.eof())
      {
        vsList >> data;
        vSource [data] = s->Read_Shader ((ShaderResourcePath + data).c_str ());
      }

      while (!fsList.eof ())
      {
        fsList >> data;
        fSource [data] = s->Read_Shader ((ShaderResourcePath + data).c_str ());
      }

      while (!comp.eof())
      {
        unsigned vPos, fPos;
        std::string vs;
        std::string fs;
        std::string name;
        comp >> data;
        vPos = data.find ("-");
        fPos = data.find (">");
        vs = data.substr (0, vPos);
        
        for (int i = vPos + 1; i < fPos; ++i)
        {
          fs += data [i];
        }

        name = data.substr (fPos + 1, data.size () - 1);

        shaders [name] = new Shader ();
        GLuint vID = shaders [name]->vertexShader = shaders [name]->Create_Shader (vSource [vs], GL_VERTEX_SHADER);
        GLuint fID = shaders [name]->fragmentShader = shaders [name]->Create_Shader (fSource [fs], GL_FRAGMENT_SHADER);
        shaders [name]->shaderProgram = shaders [name]->Create_Program (vID, fID);
      }
    }
  }


  void ResourceManager::Load_Sounds()
  {
    std::ifstream audioFile(AudioResourcePath + "SoundAssets.txt");

    if (!audioFile.good())
    {
      std::cout << "Failed to Load Sounds...\n";
      return;
    }
    else
    {
      std::string str;
      while (!audioFile.eof())
      {
        audioFile >> str;
        char* c = strstr ((char*)str.c_str(), str.c_str ());
        Sound* sound = AUDIOSYSTEM->LoadSound ((AudioResourcePath + str).c_str (), c, Sound::SOUND_2D, 0.5f);
        sounds[str] = sound;
        std::cout << str << std::endl;
      }
    }
  }

  Texture* ResourceManager::Get_Texture (std::string textureName)
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

  SpriteSheet* ResourceManager::Get_SpriteSheet (std::string spriteSheetName)
  {
    //return textures [textureName];
    for (auto i : spriteSheets)
    {
      if (i.first == spriteSheetName)
      {
        return i.second;
      }
    }

    throw ("Invalid Name...");
  }

  Shader* ResourceManager::Get_Shader (std::string shaderName)
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

  Sound* ResourceManager::Get_Sound (std::string soundName)
  {
    for (auto i : sounds)
    {
      if (i.first == soundName)
      {
        return i.second;
      }
    }

    throw ("Invalid Name...");
  }

}
