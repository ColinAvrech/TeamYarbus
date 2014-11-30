/******************************************************************************/
/*!
\file   FireStarter.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#include "Thermodynamics.h"
#include "GameObject.h"
#include "ComponentInclude.h"

namespace Framework
{ 
  void FireStarter::Serialize(Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    Serializer::DataNode* temp = data->FindElement(data, "OnFire");
    temp->GetValue(&onFire);

    temp = data->FindElement(data, "Fuel");
    temp->GetValue(&Fuel);

    temp = data->FindElement(data, "InitTemp");
    temp->GetValue(&initTemp);

    temp = data->FindElement (data, "MaterialType");
    temp->GetValue (&material_type);
  }

  void FireStarter::Update(const double dt)
  {
    if (Fuel > 0.0f)
      Fuel -= dt;
  }

  void FireStarter::Initialize()
  {
    gameObject->FireStarter = this;
    float x = this->gameObject->Transform->GetGridPosition().x;
    float y = this->gameObject->Transform->GetGridPosition().y;
    Physics::THERMODYNAMICS->Add_Object(x, y, this);
  }

  void FireStarter::LightOnFire(void)
  {
    if (!onFire)
    {
      onFire = true;
      Physics::ThermodynamicsSystem::FIRE->AddFire
        (gameObject->Transform->GetPosition ().x,
        gameObject->Transform->GetPosition ().y,
        300);
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