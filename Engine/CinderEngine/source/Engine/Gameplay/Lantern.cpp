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
		glm::vec2 orthogonalRight = glm::vec2(-origin.y, origin.x);
		glm::vec2 orthogonalLeft = -orthogonalRight;
		glm::vec2 tform = (glm::vec2)gameObject->C<Transform>()->GetPosition();
		origin = Camera::GetWorldMousePosition() - tform;
		leftBounds = tform + origin * lightRadius*cos(lightTheta / 2) +
			orthogonalLeft * lightRadius*sin(-lightTheta / 2);
		rightBounds = tform + origin * lightRadius*cos(lightTheta / 2) +
			orthogonalRight * lightRadius*sin(lightTheta / 2);

		glm::normalize(origin);
		glm::normalize(rightBounds);
		glm::normalize(leftBounds);
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
		gluPerspective(Camera::main->GetFOV(), (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(), 0, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glm::vec3 eye = glm::vec3(0, 0, 1) * Camera::main->GetSize() + glm::vec3(tform->GetPosition().x, tform->GetPosition().y, 0);
		glm::vec3 center = tform->GetPosition();
		glm::vec3 up = glm::vec3(0, 1, 0);
		gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

		
		glBegin(GL_LINES);
		{
			glColor4f(223/255.f, 0, 1.f, 1.f);
			glVertex3f(c_center.x, c_center.y, 0);
			glVertex3f(Camera::GetWorldMousePosition().x, Camera::GetWorldMousePosition().y, 0);
			glVertex3f(c_center.x, c_center.y, 0);
			glVertex3f(c_center.x + rightBounds.x, c_center.y + rightBounds.y, 0);
			glVertex3f(c_center.x, c_center.y, 0);
			glVertex3f(c_center.x + leftBounds.x, c_center.y + leftBounds.y, 0);
		}
		glEnd();
	}

	DefineComponentName(Lantern);
}