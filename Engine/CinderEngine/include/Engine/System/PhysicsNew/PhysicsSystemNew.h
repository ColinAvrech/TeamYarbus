#ifndef SCENE_H
#define SCENE_H

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
	  PhysicsSystemNew( float dt, unsigned iterations );
	
	  void Step( void );
	  void Render( void );
	  RigidBody2D *Add( ShapeCollider2D *shape, float x, float y );
	  void Clear( void );

    virtual void Update (const double& dt);

    virtual const string GetName ();



	  float m_dt;
	  unsigned m_iterations;
	  std::vector<RigidBody2D *> rigidBodies;
	  std::vector<Manifold> contacts;
	};

  extern PhysicsSystemNew* PHYSICS;
}


#endif // SCENE_H