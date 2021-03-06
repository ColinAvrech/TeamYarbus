/******************************************************************************/
/*!
\file   CameraShake.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _CAMERA_SHAKE_H
#define _CAMERA_SHAKE_H


namespace Framework
{
  enum CAMERA_SHAKE_STATE
  {
    CS_NONE,
    CS_SHAKE,
    CS_END,
    CS_RESET,
  };

  class UpdateEvent;

  class CameraShake : public Component
  {
  public:
	  META_DECLARE( CameraShake );

    CameraShake ();
    ~CameraShake ();

    virtual void Serialize (Serializer::DataNode* data);
    virtual void Initialize ();
    virtual void OnApplicationPause (PauseEvent* pause);
    void Update (UpdateEvent* update);
    void Shake ();

    const static std::string Name;
    glm::vec2 shake;
    float duration;
    float time = 0.0f;
    glm::vec3 originalPosition;
    glm::vec3 startPosition;
    CAMERA_SHAKE_STATE state = CS_NONE;
  };  
}

META_DEFINE( Framework::CameraShake, CameraShake )
{
	
}

#endif