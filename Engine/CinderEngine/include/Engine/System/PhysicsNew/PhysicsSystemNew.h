/******************************************************************************/
/*!
\file   PhysicsSystemNew.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _PHYSICS_SYSTEM_NEW_H
#define _PHYSICS_SYSTEM_NEW_H

#include <vector>
#include "Manifold.h"
#include "MathExtensionLibrary.h"
#include "BaseSystem.h"

namespace Framework
{
	class RigidBody2D;
	class ShapeCollider2D;

	class PhysicsSystemNew : public BaseSystem
	{
  public:
    PhysicsSystemNew (float dt, unsigned iterations);

    virtual bool Initialize ();
    virtual void Update (const double& dt);
    virtual void OnApplicationPause (PauseEvent* pause);

    RigidBody2D *Add (ShapeCollider2D *shape, float x, float y);
    void Remove (RigidBody2D* rb);
    void Step (void);
    void Render (void);
    void Clear (void);

    virtual const string GetName ();



	  float m_dt;
	  unsigned m_iterations;
	  std::vector<RigidBody2D *> rigidBodies;
	  std::vector<Manifold> contacts;
	};

  extern PhysicsSystemNew* PHYSICS;
}


#endif // _PHYSICS_SYSTEM_NEW_H