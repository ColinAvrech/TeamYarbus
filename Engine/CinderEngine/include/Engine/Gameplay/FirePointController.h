/******************************************************************************/
/*!
\file   FirePointController.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Main Player Controller

*/
/******************************************************************************/


#pragma once

#include "MetaCreator.h"

namespace Framework
{
  class GameObject;
  class UpdateEvent;
  class KeyEvent;
  class PingEvent;

  class FirePointController : public Component
  {
  public:
	  META_DECLARE( FirePointController );

    //ZilchDeclareBaseType(FirePointController, Zilch::TypeCopyMode::ReferenceType);

    FirePointController();
    virtual ~FirePointController();
    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);

    void Update(UpdateEvent* e);


    // The non-base component usees DefineComponentName macro to name component
    const static string Name;
  private:

    std::vector<GameObject*> FireStarters;
    void CreateFireStarters();
  };




}

META_DEFINE( Framework::FirePointController, FirePointController )
{

}