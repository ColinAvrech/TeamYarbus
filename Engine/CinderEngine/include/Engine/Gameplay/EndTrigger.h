/******************************************************************************/
/*!
\file   EndTrigger.h
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _END_TRIGGER_H
#define _END_TRIGGER_H

#include "Component.h"

namespace Framework
{
  class CollisionEvent;


  class EndTrigger : public Component
  {
  public:
  EndTrigger ();
  virtual ~EndTrigger ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void OnApplicationPause (PauseEvent* pause);
  void OnCollisionEnter (CollisionEvent* coll);

  const static std::string Name;

  private:
    bool triggered = false;
  };  
}

#endif