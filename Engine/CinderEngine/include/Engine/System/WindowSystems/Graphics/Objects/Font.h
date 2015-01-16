/******************************************************************************/
/*!
\file   Font.h
\author Joshua Biggs
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#include "Zilch.hpp"

namespace Framework
{

  class Font
  {
  public:
	ZilchDeclareBaseType(Font, Zilch::TypeCopyMode::ReferenceType);
    Font ();
	Font(const char* filename, string name);
    ~Font ();

	
	unsigned int size;

	void Load_Font(const char* filename);
	FT_Face GetFontFaceCopy();
	Zilch::String Name;
	FT_Face  face;

  };

}