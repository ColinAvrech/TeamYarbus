/******************************************************************************/
/*!
\file   UILayer.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  An individual layer of the UI
*/
/******************************************************************************/

#pragma once
#include <string>
#include <vector>

namespace Framework
{
  //Forward declares------------
  class GameObject;
  //---------------------------

  class UILayer
  {
  public:
    //Forward declares------------
    friend class Component;
    //---------------------------
    UILayer(const char *layer);
    ~UILayer();
    //!Initialize the system
    bool Initialize();
    const char* GetName();
    GameObject* CreateObject(const std::string &);
    //May not be required
    //GameObject* LoadArchetype(const char *);
    GameObject *FindObjectByName(const std::string &name);

  private:
    std::string name;
    std::vector<GameObject*>UIObjects;
    void Clear();

    //private methods
    void LoadLayer(const char *name);
    void SerializeLayer(Serializer::DataNode *trunk);
  };
} //namespace framework
