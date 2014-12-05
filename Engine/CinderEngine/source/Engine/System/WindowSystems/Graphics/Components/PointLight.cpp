/******************************************************************************/
/*!
\file   PointLight.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "PointLight.h"
#include "GameObject.h"
#include "Pipeline.h"
#include "ResourceManager.h"

namespace Framework
{
  DefineComponentName (PointLight);

  Shader* PointLight::shader = nullptr;

  // Constructor
  PointLight::PointLight ()
  {}
  
  // Destructor
  PointLight::~PointLight ()
  {
    OPENGL->pointLights.remove (this);
  }

  void PointLight::Initialize ()
  {
    OPENGL->pointLights.push_back (this);

    if (shader == nullptr)
    {
      shader = Resources::RS->Get_Shader ("PointLight");
    }
  }

  void PointLight::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Radius");
    value->GetValue (&radius);

    value = data->FindElement (data, "Intensity");
    value->GetValue (&intensity);

    value = data->FindElement (data, "Color");
    value->GetValue (&color);
  }

  //BoundType* PointLight::ZilchGetDerivedType () const
  //{
  //  throw std::logic_error ("The method or operation is not implemented.");
  //}

  void PointLight::Draw ()
  {
    shader->uni1i ("image", 0);
    shader->uni1f ("lightRadius", radius);
    shader->uni1f ("intensity", intensity);
    position = glm::vec3 (gameObject->Transform->GetNDCPosition (), 0.0f);
    shader->uni2fv ("lightPosition", glm::value_ptr (position));
    shader->uni4fv ("lightColor", glm::value_ptr (color));
  }

}