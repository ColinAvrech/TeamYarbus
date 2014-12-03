#pragma once
#include "Component.h"

namespace Framework
{
  class KeyEvent;
  class CollisionEvent;
  class UpdateEvent;

  class CharacterController : public Component
  {
  public:
    const static string Name;

    CharacterController (GameObject * obj)
    {
      gameObject = obj;
    }

    CharacterController(){}

    ~CharacterController();

    /*!Telegraph that the component is active*/
    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);

    //Registered to events
    void Update(UpdateEvent* e);
    void OnKeyPressed(KeyEvent* key);
    void OnCollisionEnter(CollisionEvent* collision);

  private:
    glm::vec2 gridPos;
    glm::vec2 accel;
    glm::vec2 jumpVel;
    glm::vec2 acceleration;
    glm::vec2 microhponeMultiplier;

    float drag;
    float maxVel;
    float currentforce;
    float density;
    glm::vec2 maxAcceleration;
  };

  extern CharacterController* PLAYER;
}