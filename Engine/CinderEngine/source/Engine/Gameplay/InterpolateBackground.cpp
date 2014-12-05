/******************************************************************************/
/*!
\file   InterpolateBackground.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "InterpolateBackground.h"
#include "EventSystem.h"
#include "UpdateEvent.h"

namespace Framework
{
  DefineComponentName (InterpolateBackground);

  // Constructor
  InterpolateBackground::InterpolateBackground ()
  {}
  
  // Destructor
  InterpolateBackground::~InterpolateBackground ()
  {}

  void InterpolateBackground::Serialize (Serializer::DataNode* data)
  {
    data->FindElement (data, "MinColor")->GetValue (&minColor);
    data->FindElement (data, "MaxColor")->GetValue (&maxColor);
  }

  void InterpolateBackground::Initialize ()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, InterpolateBackground> (Events::UPDATEEVENT, this, &InterpolateBackground::Update);
  }

  void InterpolateBackground::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void InterpolateBackground::Update (UpdateEvent* update)
  {
    Interpolate_Background ();
    glClearColor (color.r, color.g, color.b, color.a);
  }

  void InterpolateBackground::Interpolate_Background ()
  {
    switch (cState)
    {
    case Framework::BACKGROUND_IDLE:
      time += 0.016f;
      if (time > 1.0f)
      {
        cState = BACKGROUND_INTERPOLATE;
        time = 0.0f;
        startColor = color;
        endColor = glm::linearRand (minColor, maxColor);
      }
      break;
    case Framework::BACKGROUND_INTERPOLATE:
      time += 0.016f;
      color = glm::mix (startColor, endColor, time * 5);
      if (time > 0.2f)
      {
        time = 0.0f;
        cState = BACKGROUND_IDLE;
      }
      break;
    default:
      break;
    }
  }

}