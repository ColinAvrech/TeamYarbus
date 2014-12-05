/******************************************************************************/
/*!
\file   PointLight.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "GraphicsCommon.h"
#include "Component.h"

namespace Framework
{

  class PointLight : public Component
  {
  public:
    PointLight ();
    virtual ~PointLight ();

    virtual void Serialize (Serializer::DataNode* data);
    virtual void Initialize ();

    void Draw ();

    //virtual BoundType* ZilchGetDerivedType () const;

    float radius;
    float intensity;
    glm::vec3 position;
    glm::vec4 color;

    const static std::string Name;

  private:
    static Shader* shader;

  };  
}

#endif