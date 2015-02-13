/******************************************************************************/
/*!
\file   Lantern.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players Lantern
*/
/******************************************************************************/
#include <Precompiled.h>
#include "Lantern.h"
#include "PhysicsSystem.h"

#define PI 4.0f*atan(1.0f)

namespace Framework
{
  Lantern::~Lantern()
  {
    EVENTSYSTEM->mDisconnect<UpdateEvent, Lantern>(Events::UPDATEEVENT, this, &Lantern::Update);
  }
  void Lantern::Initialize()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, Lantern>(Events::UPDATEEVENT, this, &Lantern::Update);
  }

  void Lantern::Serialize(Serializer::DataNode* data)
  {
    data->FindElement(data, "lightRadius")->GetValue(&lightRadius);
    data->FindElement(data, "lightTheta")->GetValue(&lightTheta);
    lightTheta *= PI / 180;
  }

  void Lantern::Update(UpdateEvent* e)
  {
    CalculateBounds();
  }

  void Lantern::CalculateBounds()
  {
    //flaslight follows the mouse
    //lantern position
    glm::vec2 tform = (glm::vec2)gameObject->C<Transform>()->GetPosition();
    //lantern to mouse
    origin = glm::normalize(Camera::GetWorldMousePosition(0.f) - tform);

    glm::vec2 orthogonalRight = glm::normalize(glm::vec2(-origin.y, origin.x));
    glm::vec2 orthogonalLeft = -orthogonalRight;
    
    leftBounds = tform + origin * lightRadius*cos(lightTheta / 2) +
      orthogonalLeft * lightRadius*sin(lightTheta / 2);
    rightBounds = tform + origin * lightRadius*cos(lightTheta / 2) +
      orthogonalRight * lightRadius*sin(lightTheta / 2);
  }

  void Lantern::CheckCollision()
  {
    for (int i = 0; i < OBJECTSYSTEM->GameObjects.size(); ++i)
    {
      glm::vec2 objPos = (glm::vec2)OBJECTSYSTEM->GameObjects[i]->C<Transform>()->GetPosition();
      glm::vec2 playerPos = (glm::vec2)gameObject->C<Transform>()->GetPosition();
      glm::vec2 playerToObj = (glm::vec2)OBJECTSYSTEM->GameObjects[i]->C<Transform>()->GetPosition() - origin;
      float dotToTarget = glm::dot(playerToObj, origin);
      float angleBetweenTarget = acosf(dotToTarget) * 180 / PI;
      float distToTarget = glm::distance2(playerPos, objPos);

      //if (angleBetweenTarget < lightTheta && distToTarget < lightRadius)
      //{
      //	//make objects interactable
      //}
    }
  }

  void Lantern::reFuel(){};

  void Lantern::Draw()
  {
    Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
    glm::vec2 c_center = (glm::vec2)gameObject->C<Transform>()->GetPosition();
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(Camera::main->GetFOV(), 
      (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(), 
      Camera::main->GetPlanes().first, Camera::main->GetPlanes().second);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::vec3 eye = tform->GetPosition();
    glm::vec3 center = tform->GetPosition() + glm::vec3(0.f, 0.f, 1.f) * Camera::main->FocalPoint();
    glm::vec3 up = glm::vec3(0, 1, 0);
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

    
    glBegin(GL_LINES);
    {
      glColor4f(223/255.f, 0, 1.f, 1.f);
      //left and right bounds
      glVertex3f(c_center.x, c_center.y, 0);
      glVertex3f(rightBounds.x, rightBounds.y, 0);
      glVertex3f(c_center.x, c_center.y, 0);
      glVertex3f(leftBounds.x, leftBounds.y, 0);
      glColor4f(123 / 255.f, 0.5f, 1.f, 1.f);
      //lantern to mouse
      glVertex3f(c_center.x, c_center.y, 0);
      glVertex3f(Camera::GetWorldMousePosition(0.f).x, Camera::GetWorldMousePosition(0.f).y, 0);
    }
    glEnd();
  }

  DefineComponentName(Lantern);
}