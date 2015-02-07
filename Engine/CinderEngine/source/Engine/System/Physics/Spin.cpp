#include <Precompiled.h>
#include "Spin.h"

namespace Framework
{
  Spin::Spin()
  {
  
  }

  Spin::~Spin()
  {
    EVENTSYSTEM->mDisconnect<UpdateEvent, Spin> (Events::UPDATEEVENT, this, &Spin::OnUpdate);
  }

  void Spin::Initialize()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, Spin>(Events::UPDATEEVENT, this, &Spin::OnUpdate);
  }

  void Spin::Serialize(Serializer::DataNode* data)
  {
  
  }

  void Spin::OnUpdate(UpdateEvent *e)
  {
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    tform->Rotate(tform->GetRotation() + PI/20);
  }

  DefineComponentName(Spin);
}