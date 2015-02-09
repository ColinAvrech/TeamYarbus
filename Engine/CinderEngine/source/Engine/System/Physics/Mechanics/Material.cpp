#include <Precompiled.h>
#include "Material.h"

namespace Framework
{
  Material::Material()
  {
    density = 1.0f;
    staticFriction = 0.4f;
    dynamicFriction = 0.4f;
    restitution = 0.2f;
  }


  Material::~Material()
  {
  }

  void Material::Serialize(Serializer::DataNode* data)
  {
    //opens file

    Serializer::DataNode* temp2;
    temp2 = data->FindElement(data, "Restitution");
    temp2->GetValue(&restitution);

    temp2 = data->FindElement(data, "StaticFriction");
    temp2->GetValue(&staticFriction);

    temp2 = data->FindElement(data, "DynamicFriction");
    temp2->GetValue(&dynamicFriction);

    temp2 = data->FindElement(data, "Density");
    temp2->GetValue(&density);
  }
}
