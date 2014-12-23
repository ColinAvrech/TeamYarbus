/******************************************************************************/
/*!
\file   Rotator.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
  DefineComponentName (Rotator);

  // Constructor
  Rotator::Rotator ()
  {}
  
  // Destructor
  Rotator::~Rotator ()
  {}

  void Rotator::Serialize (Serializer::DataNode* data)
  {
    data->FindElement (data, "Velocity")->GetValue(&velocity);

  }

  void Rotator::Initialize ()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, Rotator> (Events::UPDATEEVENT, this, &Rotator::Update);
  }

  void Rotator::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void Rotator::Update (UpdateEvent* update)
  {
    gameObject->Transform->Rotate (velocity * 0.016f);
  }

}