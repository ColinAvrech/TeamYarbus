/******************************************************************************/
/*!
\file   Level.h
\author Collin Avrech
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Common.h"
#include "JSONSerializer.h"

#pragma once
namespace Framework
{
  class Level
  {
    public:
      Level(const string& newname = "", const string& newfile = "");

      void SetName(const string& levelname);
      void SetFile(const string& filename);

      Serializer::ZeroSerializer* GetData();
      string GetName() const;

    private:
      string name;
      string fn_level;
      Serializer::ZeroSerializer data;
  };
}