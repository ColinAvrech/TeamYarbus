/******************************************************************************/
/*!
\file   MicahPC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Main Player Controller

*/
/******************************************************************************/


#include "Common.h"
#include "MicahPC.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "AudioSystem.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "ObjectSystem.h"

namespace Framework
{
  DefineComponentName(MicahPC);


  MicahPC::MicahPC(){}

  MicahPC::~MicahPC()
  {

  }

  void MicahPC::Initialize()
  {
    EVENTSYSTEM->mConnect<KeyEvent, MicahPC>(Events::KEY_ANY, this, &MicahPC::KeyPressed);
    EVENTSYSTEM->mConnect<UpdateEvent, MicahPC>(Events::UPDATEEVENT, this, &MicahPC::Update);

    CreateFireStarters();
  }
  
  void MicahPC::Serialize(Serializer::DataNode* data)
  {

  }


  void MicahPC::Update(UpdateEvent* e)
  {
    std::cout << "MicahPC: Update" << std::endl;
    WindDirection = { 0, 0 };

  }


  void MicahPC::CreateFireStarters()
  {
    
    FireStarters.push_back(OBJECTSYSTEM->CreateObject());

    for each(auto starter in FireStarters)
    {
    }
  }

  void MicahPC::KeyPressed(KeyEvent * e)
  {
    std::cout << "MicahPC: KeyPressed" << std::endl;

    unsigned key_pressed = e->KeyValue;

    // Get Vector direction for wind
    if (key_pressed == GLFW_KEY_W) { WindDirection.y += 1; }
    if (key_pressed == GLFW_KEY_S) { WindDirection.y -= 1; }
    if (key_pressed == GLFW_KEY_D) { WindDirection.y += 1; }
    if (key_pressed == GLFW_KEY_A) { WindDirection.y -= 1; }
    
    glm::normalize(WindDirection);
  }



}