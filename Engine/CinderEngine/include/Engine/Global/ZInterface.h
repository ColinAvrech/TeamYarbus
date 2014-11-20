/******************************************************************************/
/*!
\file   ZInterface.h
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains several helper macros for programming in ZILCH!
*/
/******************************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "ZilchCompiledLib.h"
#include "GameObject.h"

using namespace std;
namespace Framework
{
	class ZilchFile
	{
	public:
		ZilchDeclareBaseType(ZilchFile, Zilch::TypeCopyMode::ReferenceType);
		ZilchFile(ifstream* file);
		~ZilchFile();

		
		Zilch::String* ReadLine(Zilch::Integer line);

	private:
		ifstream* File;
		vector<string> Data;
	};

	class ZInterface
	{
	public:
		ZilchDeclareBaseType(ZInterface, Zilch::TypeCopyMode::ReferenceType);
		static ObjectSystem* ObjectSys;
		

		static ZilchFile* OpenFile(Zilch::String filepath);
		
	};

	
}// namespace Framework