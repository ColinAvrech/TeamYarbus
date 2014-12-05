/******************************************************************************/
/*!
\file   PlayerStats.h
\author Manas Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PLAYER_STATS_H
#define _PLAYER_STATS_H

#include "Component.h"

namespace Framework
{
  class AllTreesBurnedEvent;
  class UpdateEvent;

  class PlayerStats : public Component
  {
  public:
  PlayerStats ();
  ~PlayerStats ();

  virtual void Initialize ();
  virtual void Serialize (Serializer::DataNode* data);
  virtual void OnApplicationPause (PauseEvent* pause);
  void AllTreesBurned (AllTreesBurnedEvent* atb);
  void Update (UpdateEvent* update);

  private:
    bool levelComplete = false;
    std::string NextLevel;
    float timer = 0.0f;
  };  
}

#endif