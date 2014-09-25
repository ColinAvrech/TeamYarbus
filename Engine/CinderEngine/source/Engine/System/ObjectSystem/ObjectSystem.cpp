/******************************************************************************/
/*!
\file   Factory.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates all GameObjectComposition and their respective components using 
the temaplted Component creators. The factory also cleans up when an object is 
deleted.
*/
/******************************************************************************/

#include "ObjectSystem.h"
#include "Component.h"
#include "ComponentInclude.h"
#include "Common.h"

namespace Framework
{
  enum LineTypes
  {
    LeftBracket,
    RightBracket,

    Level,
    Object,
    Variables,
    Property

  };

  //!Set the factory to null to indicate is hasn't been created yet
  ObjectSystem * OBJECTSYSTEM = NULL;

  ObjectSystem::ObjectSystem()
  {
    ErrorIf(OBJECTSYSTEM != NULL, "Factory Already Created");
    OBJECTSYSTEM = this;
    RegisterComponents();
  }

  ObjectSystem::~ObjectSystem()
  {
  }

  /*!Deletes all objects int eh ObjectsToBeDelted List.*/
  void ObjectSystem::Update(const double dt)
  {
  }

  void ObjectSystem::RegisterComponents(void)
  {
    RegisterComponent(Transform);
    //RegisterComponent(Sprite);
  }

  void ObjectSystem::AddComponentCreator(std::string name, ComponentCreator* creator)
  {
    SerialMap[name] = creator;
  }

  void ObjectSystem::DestroyAllObjects()
  {    

  }
  /*
  void ObjectSystem::LoadLevel(std::string level)
  {
    std::string levelpath(LevelAssetsPath);
    levelpath.append(level);

    std::ifstream levelfile;
    levelfile.open(levelpath.c_str());
    
    ErrorIf(!levelfile.is_open(), "Could Not Open Levelfile");

    std::cout << "Loading Level: " << Console::magenta << levelpath << Console::gray << " Please Wait" << std::endl;

    while (!levelfile.eof())
    {
      //ReadLine(levelfile.getline);
    }

    levelfile.close();
  }
  std::string ObjectSystem::ReadLine(std::string line)
  {
    return NULL;
  }
  */
}