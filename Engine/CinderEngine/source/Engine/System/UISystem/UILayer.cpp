/******************************************************************************/
/*!
\file   UISystem.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "UILayer.h"

namespace Framework
{
  UILayer::UILayer(const char *layername)
  {
    name = std::string(layername);
    LoadLayer(layername);
  }

  UILayer::~UILayer()
  {
    this->Clear();
  }

  const char* UILayer::GetName()
  {
    return this->name.c_str();
  }

  void UILayer::LoadLayer(const char *layername)
  {
    //this->Clear();
    Serializer::ZeroSerializer data;
    data.open(layername);
    data.CreateArchive();
    Serializer::DataNode *Trunk = data.GetTrunk();
  }

  void UILayer::SerializeLayer(Serializer::DataNode *trunk)
  {
    std::cout << "Loading " << name << std::endl;
    auto it = trunk->branch;
    while (it != nullptr)
    {
      //create object
      std::string objName;
      Serializer::DataNode *n = it->FindElement(it->branch, "Named");
      n->FindElement(n->branch, "Name")->GetValue(&objName);
      GameObject *newobj = this->CreateObject(objName);
      //attach components
      auto ci = it->branch->next;
      while (ci != nullptr)
      {
        Component *newcomp = newobj->AddComponent(ci->objectName);
        if (newcomp != nullptr)
        {
          newcomp->gameObject = newobj;
          newcomp->Serialize(ci->branch);
        }
        ci = ci->next;
      }
      //Initialize all components after adding them so they are independent of serializing order
      for (auto component : newobj->Components)
      {
        component.second->Initialize();
      }
      it = it->next;
    }
  }

  GameObject *UILayer::CreateObject(const std::string &objname)
  {
    GameObject *newobj = new GameObject(UIObjects.size());
    UIObjects.push_back(newobj);
    return newobj;
  }

  void UILayer::Clear()
  {
    //remove all objects from list
    for (auto obj : UIObjects)
    {
      delete obj;
    }
  }

} //namespace framework