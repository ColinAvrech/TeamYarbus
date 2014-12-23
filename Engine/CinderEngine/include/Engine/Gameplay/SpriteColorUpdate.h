/******************************************************************************/
/*!
\file   SpriteColorUpdate.h
\author Manas Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SPRITE_COLOR_UPDATE_H
#define _SPRITE_COLOR_UPDATE_H

namespace Framework
{

  enum SIDE
  {
    NONE,
    LEFT,
    RIGHT,
  };

  class SpriteColorUpdate : public Component
  {
  public:
  SpriteColorUpdate ();
  ~SpriteColorUpdate ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  void Update (UpdateEvent* update);
  virtual void OnApplicationPause (PauseEvent* pause);

  const static std::string Name;

  private:
    SIDE side = RIGHT;
    glm::vec4 minColor;
    glm::vec4 maxColor;
    bool loop;
    float timer = 0.0f;
    float multiplier;

  };  
}

#endif