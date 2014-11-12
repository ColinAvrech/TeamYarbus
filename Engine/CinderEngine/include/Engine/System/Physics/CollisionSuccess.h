#pragma once
#include "PhysicsLibrary.h"


namespace Framework
{
	class RigidBody;
	///Data for a contact between two bodies.
	///Used to resolve world collisions.
	class BodyContact
	{
		RigidBody* Bodies[2];
		glm::vec3 Movement[2];
		glm::vec3 ContactNormal;
		float Penetration;
		float Restitution;
		float FrictionCof;

		float SeperatingVelocity;
		float ContactImpulse;
		float CalculateSeparatingVelocity();
	};

	class ContactSet;
	class ShapeCollider;

	//macro - what do?
	typedef bool(*CollisionTest)(ShapeCollider*a, glm::vec3 at, ShapeCollider*b, Vec2 bt, ContactSet*c);
	///The collision database provides collision detection between shape types.
	class CollisionDatabase
	{
	public:
		CollisionDatabase();
		//CollisionTest CollisionRegistry[ShapeCollider::numOfShapes][ShapeCollider::SidNumberOfShapes];
		bool GenerateContacts(ShapeCollider* shapeA, glm::vec3 poistionA, ShapeCollider* shapeB, glm::vec3 poistionB, ContactSet*c);
		//void RegisterCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test);
	};
}