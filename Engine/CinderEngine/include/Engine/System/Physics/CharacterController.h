/******************************************************************************/
/*!
\file   CharacterController.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Gamelogic for the character controller
*/
/******************************************************************************/
#pragma once
#include "Component.h"
#include "Zilch.hpp"

#include "MetaCreator.h"

namespace Framework
{
  class KeyEvent;
  class CollisionEvent;
  class UpdateEvent;

  class CharacterController : public Component
  {
  public:
	  META_DECLARE( CharacterController );

    ZilchDeclareDerivedType (CharacterController, Component);
    const static string Name;

    CharacterController (GameObject * obj)
    {
      gameObject = obj;
    }

    CharacterController ();

    ~CharacterController();

    /*!Telegraph that the component is active*/
    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);

    //Registered to events
    void Update(UpdateEvent* e);
    void OnKeyPressed(KeyEvent* key);
    void OnCollisionEnter(CollisionEvent* collision);

    static CharacterController* PLAYER;

    bool CanFly() const;
    void ToggleFlying();

    bool useFlying;

  private:
    glm::vec2 gridPos;
    glm::vec2 accel;
    glm::vec2 jumpVel;
    glm::vec2 acceleration;
    glm::vec2 micMultiplier;

    float drag;
    float maxVel;
    float currentforce;
    float density;
    glm::vec2 maxAcceleration;

  };

}