/******************************************************************************/
/*!
\file   InterpolateBackground.h
\author Manas Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _INTERPOLATE_BACK_H
#define _INTERPOLATE_BACK_H

namespace Framework
{
  enum COLOR_STATE
  {
    BACKGROUND_IDLE,
    BACKGROUND_INTERPOLATE,
  };

  class UpdateEvent;

  class InterpolateBackground : public Component
  {
  public:
  InterpolateBackground ();
  ~InterpolateBackground ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual  void Update (UpdateEvent* update);
  virtual void OnApplicationPause (PauseEvent* pause);

  const static std::string Name;

  private:
    float time;
    glm::vec4 color;
    glm::vec4 minColor;
    glm::vec4 maxColor;
    glm::vec4 startColor;
    glm::vec4 endColor;
    COLOR_STATE cState = BACKGROUND_IDLE;

    void Interpolate_Background (const float &dt);
  };  
}

#endif