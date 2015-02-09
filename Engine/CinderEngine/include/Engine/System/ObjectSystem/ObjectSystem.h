/******************************************************************************/
/*!
\file   ObjectSystem.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/


#pragma once

namespace Framework
{
  //forward Declareation for ObjectSystem
  class ObjectSystem;
  //!Set the factory to null to indicate is hasn't been created yet
  extern ObjectSystem * OBJECTSYSTEM;
}


#include "ComponentCreator.h"


namespace Framework
{
  class Level;


  enum ObjectSystemCommand
  {
    _Idle,
    _LoadLevel,
    _LoadLevelAdditive,
  };

  //!Set the factory to null to indicate is hasn't been created yet
  extern ObjectSystem * OBJECTSYSTEM;

  class ObjectSystem : public BaseSystem
  {
  public:
    ZilchDeclareDerivedType(ObjectSystem, BaseSystem);
	
	friend class ZArray;
    friend class GameObject;
    friend class Component;

    const char* LevelAssetsPath = "../../Assets/Levels/";

    ObjectSystem();
    ~ObjectSystem();

    virtual bool Initialize ();
    //!Update the factory, Destroying dead objects
    virtual void Update(const float& dt);

    //!name of the system if the Factory, duh.
    virtual const string GetName(){ return "ObjectSystem"; }

    GameObject* CreateObject();

    //!Destroy all the GOC. Used in final shutdown procedure.
    void DestroyObjectNow(GameObject* obj);
    void DestroyObject(GameObject* obj);
    void DestroyAllObjects();

    void ZilchLoadLevel(Zilch::String level);
    void LoadLevel(const char* name);
    void NextLevel();
    GameObject* LoadArchetype(const char *);

    // Adds more objects to an existing level
    ZArray* LoadLevelAdditive(Zilch::String level);

    void RestartLevel();
    
	  GameObject* FindObjectByName(Zilch::String name);
    GameObject* FindObjectByName (const char *name);
    GameObject* FindObjectByID(unsigned id);
	  
    static unsigned LastGameObjectId;
    std::string currentLevelName;
    static int currentLevel;

    typedef std::unordered_map<string, ComponentCreator *> SerializationMap;
    SerializationMap SerialMap;

    typedef std::unordered_map<unsigned, GameObject*> GameObjectMap;
    GameObjectMap GameObjects;

    GameObject* ptrPlayer;

  private:
    Zilch::Array<GameObject*>* SerializeObject(Serializer::DataNode* data);

    void DestroyGameObjectsToBeDestroyed();

    void RegisterComponents(void);
    void AddComponentCreator(string name, ComponentCreator* creator);
    
    typedef std::vector<GameObject *> ObjectsToBeDestroyed;
    ObjectsToBeDestroyed GameObjectsToBeDestroyed;
    std::vector<Level*> levelList;
    std::queue<ObjectSystemCommand> CommandList;
  };
}


// Collins Stuff (DO NOT TOUCH) hi colin
/*
void LoadAllLevels(const string &levellist);
void LoadLevel(const string &fn_level = "", const string &levelName = "");

void ChangeLevel(const string& name);
void ChangeLevel(const int& iNewLevel);

void StartLevel();
void RestartLevel();
*/