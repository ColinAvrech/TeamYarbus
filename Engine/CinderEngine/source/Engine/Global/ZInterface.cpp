/******************************************************************************/
/*!
\file   ZInterface.cpp
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains several helper macros for programming in ZILCH!
*/
/******************************************************************************/
#include "ZInterface.h"
#include "Zilch.hpp"

namespace Framework
{
	ZilchDefineType(ZilchFile, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);

		
		ZilchBindMethod(ReadLine);

	}

	ZilchDefineType(ZInterface, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);

		ZilchBindStaticMethod(OpenFile);
		
		
	}

	ZilchFile* ZInterface::OpenFile(Zilch::String filepath)
	{
		ifstream myfile;

		myfile.open(filepath.c_str(), std::ios::out);
		//ErrorIf(dataFile.is_open() == false, "Could not open Level File");
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
	}

	Zilch::String ZilchFile::ReadLine(Zilch::Integer line)
	{
		if (int(line) > Data.size() - 1)
		{
			return Zilch::String("NULL");
		}

		return Zilch::String(Data.at(line).c_str());
		//File->
	}
}