/******************************************************************************/
/*!
\file   ZInterface.cpp
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains several helper macros for programming in ZILCH!
*/
/******************************************************************************/

#include <Precompiled.h>
#include "ZInterface.h"
#include "Zilch.hpp"
#include "Core.h"
#include "WindowSystem.h"
#include "ResourceManager.h"
#include "Pipeline.h"

namespace Framework
{
	ObjectSystem* ZInterface::ObjectSys = OBJECTSYSTEM;
	WindowSystem* ZInterface::WindowSys = WINDOWSYSTEM;
	Resources* ZInterface::ResourceSystem = nullptr;
	Pipeline* ZInterface::GraphicsPipeline = OPENGL;
	
	
	ZilchDefineType(ZilchFile, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethod(ReadLine);
	}
	ZilchDefineType(ZArray, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethod(Get);
		ZilchBindFieldGet(Length);
		//ZilchBindFieldGet(Objects);
	}


	ZilchDefineType(ZInterface, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);

		ZilchBindStaticMethod(OpenFile);
		ZilchBindStaticMethod(QuitGame);
		ZilchBindStaticFieldGetAs(ObjectSys, "ObjectSystem");
		ZilchBindStaticFieldGetAs(WindowSys, "WindowSystem");
		ZilchBindStaticFieldGet(ResourceSystem);
		ZilchBindStaticMethod(TogglePaused);
		ZilchBindStaticMethod(IsPaused);
		ZilchBindStaticMethod(SetPaused);
		ZilchBindStaticMethod(SetSceneShader);
		
		
	}

	ZilchFile* ZInterface::OpenFile(Zilch::String filepath)
	{
		ifstream myfile;

		myfile.open(filepath.c_str(), std::ios::out);
		
		ErrorIf(myfile.is_open() == false, "Could not open Level File");
		/*Zilch::Array<Zilch::String>* returnfile = new Zilch::Array<Zilch::String>;
		if (myfile.is_open())
		{
		while (getline(myfile, line))
		{
		returnfile->append(Zilch::String(line.c_str()));
		}
		}
		else
		{
		std::cout << "ERROR COULD NOT OPEN FILE" << std::endl;
		}*/

		return new ZilchFile(&myfile);


	}

	void ZInterface::SetSceneShader(String shader)
	{
		if (CORE)
		{
			OPENGL->sceneShader = Resources::RS->Get_Shader(shader.c_str());
		}
	}

	

	void ZInterface::TogglePaused()
	{
		CORE->TogglePaused();
	}

	Boolean ZInterface::IsPaused()
	{
		return CORE->IsPaused();
	}

	void ZInterface::SetPaused(bool state)
	{
		return CORE->SetPaused(state);
	}

	ZilchFile::ZilchFile(ifstream* file)
	{
		File = file;
		string line;
		
		while (getline(*File, line))
		{
			Data.push_back(line);
		}
	}

	ZilchFile::~ZilchFile()
	{
		File->close();

		delete File;
    File = nullptr;
	}

	Zilch::String ZilchFile::ReadLine(Zilch::Integer line)
	{
		if ((unsigned)line > Data.size() - 1)
		{
			return Zilch::String('\0');
		}

		return Zilch::String(Data.at(line).c_str());
		//File->
	}

	void ZInterface::QuitGame()
	{
		CORE->QuitGame();
	}

	ZArray::ZArray(Zilch::Array<GameObject*>* arr)
	{
		Objects = *arr;
		Length = Objects.size();
	}

	GameObject* ZArray::Get(unsigned index)
	{
		return Objects[index];
	}

	ZArray::~ZArray()
	{
		Objects.deallocate();
	}
}