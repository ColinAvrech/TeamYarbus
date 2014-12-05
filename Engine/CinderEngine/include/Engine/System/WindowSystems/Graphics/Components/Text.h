/******************************************************************************/
/*!
\file   Text.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TEXT_H
#define _TEXT_H

#include <string>
#include "glm.hpp"
#include "Component.h"

namespace Framework
{
  class BoundType;

  class Text : public Component
  {
  public:
    Text ();
    virtual ~Text ();

    virtual void Serialize (Serializer::DataNode* data);
    virtual void Initialize ();
    virtual void OnApplicationPause (PauseEvent* pause);

    // X, Y BETWEEN -1 to +1
    void Draw ();

    vec2 position;
    string text;
  };  
}

#endif