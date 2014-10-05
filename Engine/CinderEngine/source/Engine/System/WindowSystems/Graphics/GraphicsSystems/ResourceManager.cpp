#include "ResourceManager.h"
#include <algorithm>
#include <fstream>

namespace Framework
{
  Resources* Resources::RS;

  Resources::Resources ()
  {
    RS = this;
  }


  Resources::~Resources ()
  {
    for (auto i : shaders)
    {
      delete (i.second);
    }
    for (auto i : textures)
    {
      delete (i.second);
    }
    for (auto i : spriteSheets)
    {
      delete (i.second);
    }
  }

  void Resources::Load_Resources ()
  {
    Load_Textures ();
    Load_Shaders ();
    Load_Sounds();
    Load_SpriteSheets ();
  }

  void Resources::Load_Textures ()
  {
    textures ["Default"] = new Texture ((TextureResourcePath + "Default.jpg").c_str ());
    std::ifstream texFile (TextureResourcePath + "TextureAssets.txt");

    if (!texFile.good ())
    {
      std::cout << Console::red << "Failed to Load Textures...\n" << Console::gray;
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


  void Resources::Load_SpriteSheets ()
  {
    std::ifstream ssFile (SpriteSheetResourcePath + "SpriteSheetAssets.txt");

    if (!ssFile.good ())
    {
      std::cout << Console::red << "Failed to Load Sprite Sheets...\n" << Console::gray;
      return;
    }
    else
    {
      std::string data;
      std::string filename;
      std::string f;
      int rows, columns, samples;
      while (!ssFile.eof())
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
        spriteSheets [f] = new SpriteSheet (filename.c_str (), rows, columns, samples);
      }
    }
  }

  void Resources::Load_Shaders ()
  {
    // List of Vertex Shaders, Fragment Shaders, Shader Programs
    std::ifstream vsList (ShaderResourcePath + "VertexShaders.txt");
    std::ifstream fsList (ShaderResourcePath + "FragmentShaders.txt");
    std::ifstream gsList (ShaderResourcePath + "GeometryShaders.txt");
    std::ifstream shaderComposition (ShaderResourcePath + "Shaders.txt");

    if (!(vsList.good () || fsList.good () || shaderComposition.good ()))
    {
      if (!vsList.good ())
      {
        std::cout << Console::red << "Failed to load Shader File..." << "VertexShaders.txt\n" << Console::gray;
      }
      if (!fsList.good ())
      {
        std::cout << Console::red << "Failed to load Shader File..." << "FragmentShaders.txt\n" << Console::gray;
      }
      if (!gsList.good ())
      {
        std::cout << Console::red << "Failed to load Shader File..." << "GeometryShaders.txt\n" << Console::gray;
      }
      if (!shaderComposition.good ())
      {
        std::cout << Console::red << "Failed to load Shader File..." << "Shaders.txt\n" << Console::gray;
      }
      return;
    }
    else
    {
      std::string data;
      Shader* s = new Shader ();
      std::unordered_map <std::string, GLuint> vSource;
      std::unordered_map <std::string, GLuint> fSource;
      std::unordered_map <std::string, GLuint> gSource;

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

      while (!gsList.eof ())
      {
        // Compile All Geometry Shaders
        gsList >> data;
        gSource [data] = s->Create_Shader (s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_GEOMETRY_SHADER);
      }

      while (!shaderComposition.eof())
      {
        std::string vs, fs, gs, name;
        shaderComposition >> name; name = "";
        shaderComposition >> vs;
        shaderComposition >> fs;
        shaderComposition >> gs;
        shaderComposition >> name;
        if (gs == "0")
        {
          // Link The Corresponding Vertex And Fragment Shaders in the Shader Program Document
          shaders [name] = new Shader ();
          shaders [name]->shaderProgram = shaders [name]->Create_Program (vSource [vs], fSource [fs]);
        }
        else
        {
          // Link The Corresponding Vertex And Fragment Shaders in the Shader Program Document
          shaders [name] = new Shader ();
          shaders [name]->shaderProgram = shaders [name]->Create_Program (vSource [vs], fSource [fs], gSource[gs]);
        }
      }

      // Free memory
      delete s;
    }

    //////////////////////////////////////////////////////////////////////////
    // COMPUTE SHADERS
    //////////////////////////////////////////////////////////////////////////

    std::ifstream compute (ShaderResourcePath + "ComputeShaders.txt");

    if (!compute.good ())
    {
      std::cout << Console::red << "Failed To Load Compute Shaders\n" << Console::gray;
    }
    else
    {
      std::string cs;
      while (!compute.eof())
      {
        compute >> cs;
        computeShaders [cs] = new ComputeShader ((ShaderResourcePath + cs).c_str());
      }
    }

  }


  void Resources::Load_Sounds()
  {
    std::ifstream audioFile (AudioResourcePath + "SoundAssets.txt");

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
        Sound* sound = AUDIOSYSTEM->LoadSound ((AudioResourcePath + str).c_str (), c, Sound::SOUND_2D, 1.0f);
        sounds[str] = sound;
        std::cout << str << std::endl;
      }
    }
  }

  Texture* Resources::Get_Texture (std::string textureName)
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

  SpriteSheet* Resources::Get_SpriteSheet (std::string spriteSheetName)
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

  Shader* Resources::Get_Shader (std::string shaderName)
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

  Sound* Resources::Get_Sound (std::string soundName)
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

  ComputeShader* Resources::Get_ComputeShader (std::string shaderName)
  {
    //return textures [textureName];
    for (auto i : computeShaders)
    {
      if (i.first == shaderName)
      {
        return i.second;
      }
    }

    throw ("Invalid Name...");
  }

}
