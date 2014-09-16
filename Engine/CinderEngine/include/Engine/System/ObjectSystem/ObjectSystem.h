/******************************************************************************/
/*!
\file   Factory.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/



#pragma once

#include "BaseSystem.h"
#include "Component.h"

namespace Framework
{
  /*!Game Object Factory:
  The Game object factory creates composition objects from data streams and manages
  their lifetimes. As part of controlling the life times of the GameObjectComposition (GOC)
  it also provides an integer based Id system for safe referencing of game objects
  through integer Id Handles.
  */
  class ObjectSystem : public BaseSystem
  {
  public:

    const char* LevelAssetsPath = "../../Assets/Levels/";

    ObjectSystem();
    ~ObjectSystem();

    //!Update the factory, Destroying dead objects
    virtual void Update(const double dt);

    //!name of the system if the Factory, duh.
    virtual const std::string GetName(){ return "ObjectSystem"; }

    //!Destroy all the GOC. Used in final shutdown procedure.
    void DestroyAllObjects();

    void LoadLevel(std::string level);


    /*!Used to generator unique GOCIds*/
    static unsigned LastGameObjectId;

  private:


    std::string ReadLine(std::string line);
    //typedef std::map<std::string, size_t> SerializationMap;
    //SerializationMap  SerialMap;


  };


  extern ObjectSystem * OBJECTSYSTEM;
}
