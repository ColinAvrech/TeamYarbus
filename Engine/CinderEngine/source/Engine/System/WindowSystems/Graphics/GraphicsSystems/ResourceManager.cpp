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


    // List of Vertex Shaders, Fragment Shaders, Shader Programs
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
      std::unordered_map <std::string, GLuint> vSource;
      std::unordered_map <std::string, GLuint> fSource;

      while (!vsList.eof())
      {
        // Compile All Vertex Shaders
        vsList >> data;
        vSource [data] = s->Create_Shader (s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_VERTEX_SHADER);
      }

      while (!fsList.eof ())
      {
        // Compile All Fragment Shaders
        fsList >> data;
        fSource [data] = s->Create_Shader (s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_FRAGMENT_SHADER);
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
        
        for (unsigned i = vPos + 1; i < fPos; ++i)
        {
          fs += data [i];
        }

        name = data.substr (fPos + 1, data.size () - 1);

        // Link The Corresponding Vertex And Fragment Shaders in the Shader Program Document
        shaders [name] = new Shader ();
        shaders [name]->shaderProgram = shaders [name]->Create_Program (vSource[vs], fSource[fs]);
      }

      // Free memory
      delete s;
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
