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