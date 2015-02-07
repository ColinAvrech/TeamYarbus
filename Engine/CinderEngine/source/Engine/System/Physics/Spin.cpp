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
    gameObject->Transform->Rotate(gameObject->Transform->GetRotation() + PI/20);
  }

  DefineComponentName(Spin);
}