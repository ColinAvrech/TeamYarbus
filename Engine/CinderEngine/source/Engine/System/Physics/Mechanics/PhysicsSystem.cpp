/******************************************************************************/
/*!
\file   PhyiscSystemNew.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "PhysicsSystem.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "GameObject.h"
#include "WindowSystem.h"
#include "Pipeline.h"
#include "Core.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include "Events.h"


namespace Framework
{
  namespace Physics
  {
    PhysicsSystem* PHYSICS = NULL;

    PhysicsSystem::PhysicsSystem(float dt, unsigned iterations) : m_dt(dt)
      , m_iterations(iterations)
    {
      PHYSICS = this;
    }


    void IntegrateForces(RigidBody2D *b, float dt)
    {
      if (b->invMass == 0.0f)
        return;

      b->velocity += (b->force * b->invMass + gravity) * (dt / 2.0f);
      //std::cout << b->velocity.x << "\n";
      b->angularVelocity += b->torque * b->invI * (dt / 2.0f);
    }

    void IntegrateVelocity(RigidBody2D *b, float dt)
    {
      if (b->invMass == 0.0f)
        return;

      vec3 deltaPosition = b->velocity * dt;
      b->orient += b->angularVelocity * dt;
      b->SetOrient(b->orient);
      if (b->gameObject != nullptr)
      {
        Transform* tform = static_cast<Transform*>(b->gameObject->GetComponent("Transform"));
        tform->Translate(deltaPosition);
        tform->Rotate(180 / M_PI * b->orient);
      }
      IntegrateForces(b, dt);
    }

    void PhysicsSystem::Step(void)
    {
      // Generate new collision info
      contacts.clear();
      for (unsigned i = 0; i < rigidBodies.size(); ++i)
      {
        RigidBody2D *A = rigidBodies[i];

        for (unsigned j = i + 1; j < rigidBodies.size(); ++j)
        {
          RigidBody2D *B = rigidBodies[j];
          if (A->invMass == 0 && B->invMass == 0)
            continue;
          Manifold m(A, B);
          m.Solve();
          if (m.contact_count)
            contacts.emplace_back(m);
        }
      }

      // Integrate forces
      for (unsigned i = 0; i < rigidBodies.size(); ++i)
        IntegrateForces(rigidBodies[i], m_dt);

      // Initialize collision
      for (unsigned i = 0; i < contacts.size(); ++i)
        contacts[i].Initialize();

      // Solve collisions
      for (unsigned j = 0; j < m_iterations; ++j)
        for (unsigned i = 0; i < contacts.size(); ++i)
          contacts[i].ApplyImpulse();

      // Integrate velocities
      for (unsigned i = 0; i < rigidBodies.size(); ++i)
        IntegrateVelocity(rigidBodies[i], m_dt);

      // Correct positions
      for (unsigned i = 0; i < contacts.size(); ++i)
        contacts[i].PositionalCorrection();

      // Clear all forces
      for (unsigned i = 0; i < rigidBodies.size(); ++i)
      {
        RigidBody2D *b = rigidBodies[i];
        b->force = vec3();
        b->torque = 0;
      }
    }

    void PhysicsSystem::Render(void)
    {
      if (Pipeline::cameras.size() > 0)
      {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(Camera::main->GetFOV(), (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(), 0, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
        glm::vec3 eye = glm::vec3(0, 0, 1) * Camera::main->GetSize() + glm::vec3(tform->GetPosition().x, tform->GetPosition().y, 0);
        glm::vec3 center = tform->GetPosition();
        glm::vec3 up = glm::vec3(0, 1, 0);

        gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

        for (unsigned i = 0; i < rigidBodies.size(); ++i)
        {
          RigidBody2D *b = rigidBodies[i];
          assert(b);
          ShapeCollider2D *shape = b->gameObject->C<ShapeCollider2D>();
          //assert(shape);
          if (shape)
          {
            switch (shape->GetType())
            {
              case ShapeCollider2D::ColliderType::eCircle:
                static_cast<CircleCollider2D*>(shape)->Draw();
                break;

              case ShapeCollider2D::ColliderType::ePoly:
                static_cast<PolygonCollider2D*>(shape)->Draw();
                break;

              case ShapeCollider2D::ColliderType::eCompound:
                static_cast<CompoundCollider2D*>(shape)->Draw();
                break;
            }
          }
        }

        glPointSize(4.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        for (unsigned i = 0; i < contacts.size(); ++i)
        {
          Manifold& m = contacts[i];
          for (unsigned j = 0; j < m.contact_count; ++j)
          {
            vec3 c = m.contacts[j];
            glVertex2f(c.x, c.y);
          }
        }
        glEnd();
        glPointSize(1.0f);

        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        for (unsigned i = 0; i < contacts.size(); ++i)
        {
          Manifold& m = contacts[i];
          vec3 n = m.normal;
          for (unsigned j = 0; j < m.contact_count; ++j)
          {
            vec3 c = m.contacts[j];
            glVertex2f(c.x, c.y);
            n *= 0.75f;
            c += n;
            glVertex2f(c.x, c.y);
          }
        }
        glEnd();

        glLoadIdentity();
      }
    }

    RigidBody2D * PhysicsSystem::Add(ShapeCollider2D *shape, float x, float y)
    {
      assert(shape);
      RigidBody2D *b = new RigidBody2D(shape, (float)x, (float)y);
      return Add(b);
    }

    RigidBody2D* PhysicsSystem::Add(RigidBody2D* b)
    {
      assert(b);
      if (b)
      {
        rigidBodies.push_back(b);
      }
      return b;
    }

    void PhysicsSystem::Remove(RigidBody2D* rb)
    {
      for (auto it = rigidBodies.begin(); it != rigidBodies.end(); ++it)
      {
        if ((*it) == rb)
        {
          rigidBodies.erase(it);
          break;
        }
      }
    }

    void PhysicsSystem::Clear()
    {
      contacts.clear();
      rigidBodies.clear();
    }

    void PhysicsSystem::Reset()
    {
      EVENTSYSTEM->mConnect<PauseEvent, PhysicsSystem>(Events::PAUSE, this, &PhysicsSystem::OnApplicationPause);
    }

    void PhysicsSystem::Update(const float& dt)
    {
      paused = CORE->IsPaused();
      if (!paused)
      {
        Step();
      }
    }

    const string PhysicsSystem::GetName()
    {
      return "PhysicsSystemNew";
    }

    bool PhysicsSystem::Initialize()
    {
      std::cout << GetName() + " initialized\n";
      EVENTSYSTEM->mConnect<PauseEvent, PhysicsSystem>(Events::PAUSE, this, &PhysicsSystem::OnApplicationPause);

      return true;
    }

    void PhysicsSystem::OnApplicationPause(PauseEvent* pauseEvent)
    {
      paused = pauseEvent->Paused;
    }
  }
}
