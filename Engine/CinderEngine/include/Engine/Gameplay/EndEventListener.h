/******************************************************************************/
/*!
\file   EndEventListener.h
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _END_EVENT_LISTENER_H
#define _END_EVENT_LISTENER_H

#include "Component.h"

namespace Framework
{
  class UpdateEvent;
  class EndEvent;
  class Sprite;

  enum END_EVENT_STATE
  {
    EES_NONE,
    EES_CAMERA,
    EES_CREDITS,
  };

  class EndEventListener : public Component
  {
  public:
  EndEventListener ();
  virtual ~EndEventListener ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void OnApplicationPause (PauseEvent* pause);
  void Update (UpdateEvent* update);
  void EndEventHandler (EndEvent* endEvent);

  const static std::string Name;

  private:
    bool loadCredits = false;
    float timer = 0.0f;
    glm::vec3 startPosition;
    glm::vec3 endPosition;
    float startSize;
    float endSize;
    Sprite* exitGame = nullptr;
    END_EVENT_STATE ees = EES_NONE;
  };  
}

#endif