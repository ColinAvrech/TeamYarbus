/******************************************************************************/
/*!
\file   PhysicsSystem.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "BaseSystem.h"
#include "Common.h"
#include "glm.hpp"
#include "Resolution.h"

namespace Framework
{
  namespace Physics
  {
    class PhysicsSystem : public BaseSystem
    {
    public:
	const string GetName(){ return "PhysicsSystem"; }
	  //Step f(x)
	  bool AdvanceStep;
	  bool StepModeActive;

	  //max cap
	  float maxVel;
	  float maxVelSq;

	  //resolution
	  float penetrationEpsilon;
	  float penetrationResolvePercent;
    
      PhysicsSystem(){};
      ~PhysicsSystem(){};

      bool Initialize(void);
	  virtual void Update(const double& dt);

	  //Containers
	  std::vector<GameObject*> Bodies;
	  std::vector<GameObject*> CircleColliders;
	  std::vector<GameObject*> LineColliders;
      std::vector<SplineCollider*> SplineColliders;

	private:
      // Called every frame    
	  void IntegrateBodies(const float dt);
	  void DetectContacts(const float dt);
	  void Step(const float dt);
	  float accumulator;

	  ContactSet Contacts;

    };
    extern PhysicsSystem *PHYSICSSYSTEM;
  }
}