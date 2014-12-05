/******************************************************************************/
/*!
\file   ResourceManager.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "ResourceManager.h"
#include <algorithm>
#include <fstream>
#include "ZilchCompiledLib.h"

namespace Framework
{
	

  Resources* Resources::RS;

  Resources::Resources ()
  {
    RS = this;
  }

	ZilchDefineType(Resources, CinderZilch)
	{
		ZilchBindMethodAs(ZGetSound, "GetSound");
	}
  Resources::~Resources ()
  {
    for (auto i : shaders)
    {
      delete (i.second);
    }
    for (auto i : computeShaders)
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
  Sound* Resources::ZGetSound(String soundName)
  {
	  return RS->Get_Sound(soundName.c_str());
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
    std::cout << CinderConsole::cyan << "--------------------------------\nLoading Textures...\n" << CinderConsole::gray;

    textures ["Default"] = new Texture ((TextureResourcePath + "Default.jpg").c_str ());
    std::ifstream texFile (TextureResourcePath + "TextureAssets.txt");

    if (!texFile.good ())
    {
      std::cout << CinderConsole::red << "Failed to Load Textures...\n" << CinderConsole::gray;
      return;
    }
    else
    {
      string str;
      while (!texFile.eof ())
      {
        texFile >> str;
        textures [str] = new Texture ((TextureResourcePath + str).c_str ());
        std::cout << CinderConsole::green << str << std::endl << CinderConsole::gray;
      }
    }
    std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;
  }


  void Resources::Load_SpriteSheets ()
  {
    std::cout << CinderConsole::cyan << "--------------------------------\nLoading SpriteSheets...\n" << CinderConsole::gray;

    std::ifstream ssFile (SpriteSheetResourcePath + "SpriteSheetAssets.txt");

    if (!ssFile.good ())
    {
      std::cout << CinderConsole::red << "Failed to Load Sprite Sheets...\n" << CinderConsole::gray;
      return;
    }
    else
    {
      string data;
      string filename;
      string f;
      int rows, columns, samples;
      while (!ssFile.eof())
      {
        ssFile >> data;
        ssFile >> data;
        f = data;
        std::cout << CinderConsole::green << data << std::endl << CinderConsole::gray;
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

    std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;
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
        std::cout << CinderConsole::red << "Failed to load Shader File..." << "VertexShaders.txt\n" << CinderConsole::gray;
      }
      if (!fsList.good ())
      {
        std::cout << CinderConsole::red << "Failed to load Shader File..." << "FragmentShaders.txt\n" << CinderConsole::gray;
      }
      if (!gsList.good ())
      {
        std::cout << CinderConsole::red << "Failed to load Shader File..." << "GeometryShaders.txt\n" << CinderConsole::gray;
      }
      if (!shaderComposition.good ())
      {
        std::cout << CinderConsole::red << "Failed to load Shader File..." << "Shaders.txt\n" << CinderConsole::gray;
      }
      return;
    }
    else
    {
      string data;
      Shader* s = new Shader ();
      std::unordered_map <string, GLuint> vSource;
      std::unordered_map <string, GLuint> fSource;
      std::unordered_map <string, GLuint> gSource;

      std::cout << CinderConsole::cyan << "--------------------------------\nCompiling Shaders...\n" << CinderConsole::gray;

      while (!vsList.eof())
      {
        // Compile All Vertex Shaders
        vsList >> data;
        vSource [data] = s->Create_Shader ((data).c_str (), s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_VERTEX_SHADER);
      }

      while (!fsList.eof ())
      {
        // Compile All Fragment Shaders
        fsList >> data;
        fSource [data] = s->Create_Shader ((data).c_str (), s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_FRAGMENT_SHADER);
      }

      while (!gsList.eof ())
      {
        // Compile All Geometry Shaders
        gsList >> data;
        gSource [data] = s->Create_Shader ((data).c_str (), s->Read_Shader ((ShaderResourcePath + data).c_str ()), GL_GEOMETRY_SHADER);
      }

      while (!shaderComposition.eof())
      {
        string vs, fs, gs, name;
        shaderComposition >> name; name = "";
        shaderComposition >> vs;
        shaderComposition >> fs;
        shaderComposition >> gs;
        shaderComposition >> name;
        if (gs == "0")
        {
          // Link The Corresponding Vertex And Fragment Shaders in the Shader Program File
          shaders [name] = new Shader ();
          if (vs == "0")
          {
            shaders [name]->shaderProgram = shaders [name]->Create_Program (name, fSource [fs]);
          }
          else
          {
            shaders [name]->shaderProgram = shaders [name]->Create_Program (name, vSource [vs], fSource [fs]);
          }
        }
        else
        {
          // Link The Corresponding Vertex And Fragment Shaders in the Shader Program File
          shaders [name] = new Shader ();
          shaders [name]->shaderProgram = shaders [name]->Create_Program (name, vSource [vs], fSource [fs], gSource[gs]);
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
      std::cout << CinderConsole::red << "Failed To Load Compute Shaders\n" << CinderConsole::gray;
    }
    else
    {
      string cs;
      while (!compute.eof())
      {
        compute >> cs;
        computeShaders [cs] = new ComputeShader ((ShaderResourcePath + cs).c_str());
      }
    }

    std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;
  }


  void Resources::Load_Sounds()
  {
    Framework::Sound::SoundID type;
    std::cout << CinderConsole::cyan << "--------------------------------\nLoading Sounds...\n" << CinderConsole::gray;
    std::ifstream audioFile (AudioResourcePath + "SoundAssets.txt");

    if (!audioFile.good())
    {
      std::cout << "Failed to Load Sounds...\n";
      return;
    }
    else
    {
      string str;
      while (!audioFile.eof())
      {
        string t;
        audioFile >> str;
        audioFile >> t;
        if (t == "BG")
        {
          type = Sound::MUSIC;
        }
        else if (t == "2D")
        {
          type = Sound::SOUND_2D;
        }
        else if (t == "3D")
        {
          type = Sound::SOUND_3D;
        }
        char* c = strstr ((char*)str.c_str(), str.c_str ());
        Sound* sound = AUDIOSYSTEM->LoadSound ((AudioResourcePath + str).c_str (), c, type, 0.0f);
        sounds[str] = sound;
        std::cout << CinderConsole::green << str << std::endl << CinderConsole::gray;
      }
    }

    std::cout << CinderConsole::cyan << "--------------------------------\n" << CinderConsole::gray;
  }

  Texture* Resources::Get_Texture (string textureName)
  {
    for (auto i : textures)
    {
      if (i.first == textureName)
      {
        return i.second;
      }
    }

    std::cout << CinderConsole::red << textureName << " Invalid Name...Using Default Texture\n" << CinderConsole::gray;
    return textures ["Default"];
  }

  SpriteSheet* Resources::Get_SpriteSheet (string spriteSheetName)
  {
    for (auto i : spriteSheets)
    {
      if (i.first == spriteSheetName)
      {
        return i.second;
      }
    }

    std::cout << CinderConsole::red << spriteSheetName << " Invalid Name...\n" << CinderConsole::gray;
    throw ("Invalid Name...");
  }

  Shader* Resources::Get_Shader (string shaderName)
  {
    for (auto i : shaders)
    {
      if (i.first == shaderName)
      {
        return i.second;
      }
    }

    std::cout << CinderConsole::red << shaderName << " Invalid Name...Using Default Shader\n" << CinderConsole::gray;
    return shaders ["Default"];
  }

  Sound* Resources::Get_Sound (string soundName)
  {
    for (auto i : sounds)
    {
      if (i.first == soundName)
      {
        return i.second;
      }
    }

    std::cout << CinderConsole::red << soundName << " Invalid Name...\n" << CinderConsole::gray;
    throw ("Invalid Name...");
  }

  ComputeShader* Resources::Get_ComputeShader (string shaderName)
  {
    //return textures [textureName];
    for (auto i : computeShaders)
    {
      if (i.first == shaderName)
      {
        return i.second;
      }
    }

    std::cout << CinderConsole::red << shaderName << " Invalid Name...\n" << CinderConsole::gray;
    throw ("Invalid Name...");
  }

}
