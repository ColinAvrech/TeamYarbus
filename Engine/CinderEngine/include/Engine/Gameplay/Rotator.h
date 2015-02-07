/******************************************************************************/
/*!
\file   Rotator.h
\author Manas Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _ROTATOR_H
#define _ROTATOR_H

namespace Framework
{
  class UpdateEvent;

  class Rotator : public Component
  {
  public:
	  META_DECLARE( Rotator );

  Rotator ();
  ~Rotator ();

  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void OnApplicationPause (PauseEvent* pause);
  void Update (UpdateEvent* update);

  const static std::string Name;

  private:
    float velocity;
  };  
}

META_DEFINE( Framework::Rotator, Rotator )
{

}

#endif