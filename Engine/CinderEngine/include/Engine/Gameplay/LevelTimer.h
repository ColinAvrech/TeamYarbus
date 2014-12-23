/******************************************************************************/
/*!
\file   LevelTimer.h
\author Manas Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _LEVEL_TIMER_H
#define _LEVEL_TIMER_H


namespace Framework
{
  class UpdateEvent;

  class LevelTimer : public Component
  {
  public:
  LevelTimer ();
  virtual ~LevelTimer ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  void Update (UpdateEvent* update);

  float time = 0.0f;
  float endTime = 0.0f;
  std::string nextLevel;

  const static std::string Name;
  };  
}

#endif