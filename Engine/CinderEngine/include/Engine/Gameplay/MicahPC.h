/******************************************************************************/
/*!
\file   MicahPC.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Main Player Controller

*/
/******************************************************************************/


#pragma once


#include "Common.h"
#include "Component.h"
#include "GraphicsCommon.h"

namespace Framework
{
  class UpdateEvent;
  class KeyEvent;
  class PingEvent;

  class MicahPC : public Component
  {
  public:
    //ZilchDeclareBaseType(MicahPC, Zilch::TypeCopyMode::ReferenceType);

    MicahPC();
    virtual ~MicahPC();
    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);


    void Update(UpdateEvent* e);
    void KeyPressed(KeyEvent* e);
    void GetCreatedFireStarters(PingEvent* e);
    

    glm::vec2 WindDirection;


    // The non-base component usees DefineComponentName macro to name component
    const static string Name;
  private:

    std::vector<GameObject*> FirePoints;
    float Micintensity;

  };




}