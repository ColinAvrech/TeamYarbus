/******************************************************************************/
/*!
\file   Level.h
\author Colin Avrech
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once
#include "Common.h"
#include "JSONSerializer.h"

namespace Framework
{
  class Level
  {
    public:
      Level(const char* newname = "", const char* newfile = "");

      Serializer::ZeroSerializer* GetData();
      string GetFileName() const;
      string GetName() const;
      Serializer::DataNode* GetTrunk();
      void SetName(const char* levelname);
      void SetFile(const char* filename);
      bool IsLoaded() const;


    private:
      string name;
      string fn_level;
      Serializer::ZeroSerializer data;
      Serializer::DataNode* trunk;
      bool loaded;
  };
}