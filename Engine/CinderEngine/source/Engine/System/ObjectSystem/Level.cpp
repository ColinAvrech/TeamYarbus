#include "Level.h"

namespace Framework
{
  Level::Level(const string& newname, const string& newfile)
  {
    if (newname != "")
    {
      SetName(newname);
    }
    if (newfile != "")
    {
      if (name == "")
      {
        name = newfile;
      }

      SetFile(newfile);
    }
  }

  string Level::GetName() const
  {
    return name;
  }

  void Level::SetName(const string& newname)
  {
    name = newname;
  }

  void Level::SetFile(const string& filename)
  {
    fn_level = filename;
    data.open(fn_level.c_str());
    data.CreateArchive();
  }

  Serializer::ZeroSerializer* Level::GetData()
  {
    return &data;
  }
}


