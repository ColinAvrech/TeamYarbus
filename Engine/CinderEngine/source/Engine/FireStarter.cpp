/******************************************************************************/
/*!
\file   GameLogic.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/
#include "FireStarter.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Thermodynamics.h"

namespace Framework
{ 
  void FireStarter::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* temp = data->FindElement(data, "onFire");
    temp->GetValue(&onFire);

    temp = data->FindElement(data, "Fuel");
    temp->GetValue(&Fuel);

    temp = data->FindElement(data, "InitTemp");
    temp->GetValue(&initTemp);
  }

  void FireStarter::Update(const double dt)
  {
    if (Fuel > 0.0f)
      Fuel -= dt;
  }

  void FireStarter::Initialize()
  {
    gameObject->FireStarter = this;
    float x = this->gameObject->Transform->GetPosition().x;
    float y = this->gameObject->Transform->GetPosition().y;
    Physics::THERMODYNAMICS->Add_Object(x, y, this);
  }

  void FireStarter::LightOnFire(void)
  {
    if (!onFire)
    {
      onFire = true;
      cout << "Lit\n";
    }
  }

  void FireStarter::DouseFire(void)
  {
    if (onFire)
    {
      onFire = false;
      cout << "Doused\n";
    }
  }

  DefineComponentName(FireStarter);
}