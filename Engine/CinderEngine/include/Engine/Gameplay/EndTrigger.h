/******************************************************************************/
/*!
\file   EndTrigger.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _END_TRIGGER_H
#define _END_TRIGGER_H

namespace Framework
{

  class EndTrigger : public Component
  {
  public:
  EndTrigger ();
  virtual ~EndTrigger ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void OnApplicationPause (PauseEvent* pause);
  void Update(UpdateEvent* update);
  void OnCollisionEnter (CollisionEvent* coll);

  const static std::string Name;

  private:
    bool triggered = false;
  };  
}

#endif