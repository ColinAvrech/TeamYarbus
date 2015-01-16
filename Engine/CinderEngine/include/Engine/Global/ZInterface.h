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

#include "ZilchCompiledLib.h"

using namespace std;
namespace Framework
{
  class Resources;
  class Sound;

	class ZilchFile
	{
	public:
		ZilchDeclareBaseType(ZilchFile, Zilch::TypeCopyMode::ReferenceType);
		
		ZilchFile(ifstream* file);
		~ZilchFile();

		
		Zilch::String ReadLine(Zilch::Integer line);
		

	private:
		ifstream* File;
		vector<string> Data;
	};

	class ZInterface
	{
	public:
		ZilchDeclareBaseType(ZInterface, Zilch::TypeCopyMode::ReferenceType);
		static ObjectSystem* ObjectSys;
		static WindowSystem* WindowSys;
		static Resources* ResourceSystem;
		static Pipeline* ZInterface::GraphicsPipeline;
		static PhysicsSystem* Physics;

		static void QuitGame();

		static Sound* GetSound(String soundName);
		static void TogglePaused();
		static Boolean IsPaused();
		static void ZInterface::SetPaused(bool state);
		static void SetSceneShader(String shader);

		static int DT;
		
		static Real2* VecToReal(vec2* vec);
		static Real3* VecToReal(vec3* vec);
		static Real4* VecToReal(vec4* vec);

		static vec2 RealToVec(Real2* real);
		static vec3 RealToVec(Real3* real);
		static vec4 RealToVec(Real4* real);
		
		static ZilchFile* OpenFile(Zilch::String filepath);
		
	};

	class ZArray
	{
	public:
		ZilchDeclareBaseType(ZArray, Zilch::TypeCopyMode::ReferenceType);
		ZArray(Zilch::Array<GameObject*>* arr);
		~ZArray();
		unsigned Length;

		GameObject* Get(unsigned index);

		Zilch::Array<GameObject*> Objects;
	private:

	};

	
}// namespace Framework