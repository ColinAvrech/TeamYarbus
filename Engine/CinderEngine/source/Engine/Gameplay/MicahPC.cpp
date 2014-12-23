/******************************************************************************/
/*!
\file   MicahPC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Main Player Controller

*/
/******************************************************************************/

#include <Precompiled.h>

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
    EVENTSYSTEM->mConnect<PingEvent, MicahPC>(Events::PING_GAMEOBJECT, this, &MicahPC::GetCreatedFireStarters);
  }
  
  void MicahPC::Serialize(Serializer::DataNode* data)
  {

  }


  void MicahPC::Update(UpdateEvent* e)
  {
    //std::cout << "MicahPC: Update" << std::endl;
    WindDirection.x = 0;
    WindDirection.y = 0;

    //std::cout << AUDIOSYSTEM->input.peaklevel << std::endl;
  }


  void MicahPC::KeyPressed(KeyEvent * e)
  {
    std::cout << "MicahPC: KeyPressed" << std::endl;

    unsigned key_pressed = e->KeyValue;

    // Get Vector direction for wind
    if (key_pressed == GLFW_KEY_W) { WindDirection.y += 1; glm::normalize(WindDirection);}
    if (key_pressed == GLFW_KEY_S) { WindDirection.y -= 1; glm::normalize(WindDirection);}
    if (key_pressed == GLFW_KEY_D) { WindDirection.x += 1; glm::normalize(WindDirection);}
    if (key_pressed == GLFW_KEY_A) { WindDirection.x -= 1; glm::normalize(WindDirection);}
    
    std::cout << (float)WindDirection.x << " " << (float)WindDirection.y << std::endl;
  }


  void MicahPC::GetCreatedFireStarters(PingEvent* e)
  {
    if (e->Ping->Name == "FirePoint")
    {
      FirePoints.push_back(e->Ping);
    }
  }



}