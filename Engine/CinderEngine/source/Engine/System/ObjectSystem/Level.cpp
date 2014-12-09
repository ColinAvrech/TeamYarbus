/******************************************************************************/
/*!
\file   Level.cpp
\author Collin Avrech
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Level.h"
#include "ObjectSystem.h"

namespace Framework
{
  Level::Level(const char* newname, const char* newfile)
  {
    loaded = false;
    if (newname != "" || newfile != "")
    {
      if (newname == "")
      {
        SetName(newfile);
      }
      else
      {
        SetName(newname);
      }
    
      if (newfile == "")
      {
        SetFile(name.c_str());
      }
      else
      {
        SetFile(newfile);
      }
    }
  }

  string Level::GetFileName() const
  {
    return fn_level;
  }

  Serializer::ZeroSerializer* Level::GetData()
  {
    return &data;
  }

  string Level::GetName() const
  {
    return name;
  }

  Serializer::DataNode* Level::GetTrunk()
  {
    return trunk;
  }

  bool Level::IsLoaded() const
  {
    return loaded;
  }

  void Level::SetName(const char* newname)
  {
    name = newname;
  }

  void Level::SetFile(const char* filename)
  {
    fn_level = filename;
    data.open(filename);
    data.CreateArchive();
    trunk = data.GetTrunk();
    OBJECTSYSTEM->SerializeObject(trunk);
    loaded = true;
  }
}


