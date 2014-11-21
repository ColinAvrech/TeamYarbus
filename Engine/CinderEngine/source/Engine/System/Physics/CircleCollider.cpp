/******************************************************************************/
/*!
\file   CircleCollider.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ComponentInclude.h"
#include "IncludeForAllCollision.h"
#include "EventSystem.h"
#include "CollisionRepeats.h"
#include "Resolution.h"
#include "CollisionEvent.h"
#include "PhysicsSystem.h"
#include "JSONSerializer.h"
#include "DebugRenderer.h"

namespace Framework
{
  //serialize, initialize, update
  void CircleCollider::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* temp;
    temp = data->FindElement(data, "Radius");
    temp->GetValue(&radius);
    radius *= gameObject->Transform->GetScale ().x;
    //Material Properties
    temp = data->FindElement(data, "MaterialName");
    std::string name;
    temp->GetValue(&name);

    SerializeMaterial(name.c_str());
  }

  void CircleCollider::Initialize()
  {
    gameObject->ShapeCollider = this;
    Physics::PHYSICSSYSTEM->CircleColliders.push_back(gameObject);
  }

  void CircleCollider::Update()
  {

  }

  void CircleCollider::DetectCollision(SplineCollider* s)
  {
    std::vector <LineCollider*> splineArray = s->Get_Colliders();
    for (auto it = splineArray.begin(); it != splineArray.end(); ++it)
    {
      DetectCollision(*it);
    }
  }

  //collision detection
  void CircleCollider::DetectCollision(CircleCollider* c)
  {
    // not counting offset
    glm::vec3 pos;
    pos.x = (gameObject->Transform)->GetPosition().x;
    pos.y = (gameObject->Transform)->GetPosition().y;
    glm::vec3 cpos;
    cpos.x = (c->gameObject->Transform)->GetPosition().x;
    cpos.y = (c->gameObject->Transform)->GetPosition().y;
    float rad = GetRadius() + c->GetRadius();
    float dist = Physics::Distance(pos, cpos);
    if (rad >= dist)
    {
      if (gameObject->FireStarter)
        std::cout << "Sparked!" << std::endl;
      glm::vec3 normalVec = gameObject->Transform->GetPosition() -
        c->gameObject->Transform->GetPosition();
      float penetration = rad - dist;
      glm::normalize(normalVec);
      BodyContact contact(gameObject, c->gameObject,
        normalVec, penetration);
      Physics::PHYSICSSYSTEM->addContact(&contact);

      //CollisionEvent collision;
      // EVENTSYSTEM->TriggerEvent(CollisionEventName(gameObject->GameObjectID), collision);
    }
  }

  //repeat
  void CircleCollider::DetectCollision(PointCollider* p)
  {
    //glm::vec3 ppos = p->getPosition();
    //glm::vec3 pos = getPosition();
    //float rad = GetRadius();

    //if (Physics::CirclevsPoint(rad, pos, ppos))
    //{
    //  glm::vec3 normalVec = gameObject->Transform->GetPosition() -
    //    p->gameObject->Transform->GetPosition();
    //  float penetration = rad - Physics::Distance(gameObject->Transform->GetPosition(),
    //    p->gameObject->Transform->GetPosition());
    //  glm::normalize(normalVec);

    //  BodyContact contact(gameObject, p->gameObject,
    //    normalVec, penetration);
    //  //CollisionEvent collision;
    //  //collision.OtherObject = p->gameObject;
    //  //collision.normal = pos - ppos;
    //  //glm::normalize(collision.normal);
    //}
  }

  void CircleCollider::DetectCollision(LineCollider* l)
  {
    glm::vec3 pos_s, pos_e;
    pos_e.x = pos_s.x = gameObject->Transform->GetPosition().x;
    pos_e.y = pos_s.y = gameObject->Transform->GetPosition().y;
    pos_e.z = pos_s.z = 0.0f;
    if (gameObject->RigidBody)
    {
      pos_e.x += gameObject->RigidBody->vel.x * 0.016f;
      pos_e.y += gameObject->RigidBody->vel.y * 0.016f;
    }

    BodyContact contact = Physics::CirclevsLine(pos_s, pos_e, *this, *l);
    if (contact.t != -1.0f)
    {
      contact.Bodies[0] = gameObject;
      contact.Bodies[1] = l->gameObject;
      contact.ContactNormal = glm::vec3(l->normalVec, 0.0f);

      Physics::PHYSICSSYSTEM->addContact(&contact);
    }
  }

  void CircleCollider::Draw ()
  {
    dr->Draw (this);
  }

  DefineComponentName(CircleCollider);
}
